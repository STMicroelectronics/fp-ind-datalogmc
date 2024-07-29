/**
  ******************************************************************************
  * @file    aspep.c
  * @author  SRA
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "aspep.h"
#include "usart_aspep_driver.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_compiler.h"
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_usart.h"


#define MCP_TX_ASYNC_PAYLOAD_MAX_B 2048
/* Global variables ----------------------------------------------------------*/
/**
 * @brief Maximum size of the MCP transmit async payload
 */
#define MCP_TX_ASYNC_PAYLOAD_MAX_B 2048
#define MCP_TX_ASYNC_PAYLOAD_MAX_A 2048

#define MCP_TX_SYNCBUFFER_SIZE (MCP_TX_SYNC_PAYLOAD_MAX+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)
#define MCP_RX_SYNCBUFFER_SIZE (MCP_RX_SYNC_PAYLOAD_MAX+ASPEP_DATACRC_SIZE) // ASPEP_HEADER_SIZE is not stored in the RX buffer.
#define MCP_TX_ASYNCBUFFER_SIZE_A (MCP_TX_ASYNC_PAYLOAD_MAX_A+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)
#define MCP_TX_ASYNCBUFFER_SIZE_B (MCP_TX_ASYNC_PAYLOAD_MAX_B+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)

uint8_t aspepSyncBuffer[MCP_TX_SYNCBUFFER_SIZE];
uint8_t aspepRxBuffer[MCP_RX_SYNCBUFFER_SIZE];
uint8_t aspepAsyncBuffA[MCP_TX_ASYNCBUFFER_SIZE_B];
uint8_t aspepAsyncBuffB[MCP_TX_ASYNCBUFFER_SIZE_B];

/* Private typedef -----------------------------------------------------------*/
#define ID_MASK 0xF
#define DATA_PACKET 0x9
#define PING  0x6
#define BEACON 0x5
#define ERROR_PACKET 0x7
#define RESPONSE 0xA  //sync
#define ASYNC 0x9

#define TBEACON       350
#define TPING         300
#define TTRANSACTION  300
#define TSILENCE      200

#define ID_MASK_PING    0x10
#define CONFIGURED      0x10
#define UNCONFIGURED    0

#define ASPEP_OK              0
#define ASPEP_BAD_PACKET_TYPE 1
#define ASPEP_BAD_PACKET_SIZE 2
#define ASPEP_BAD_CRC_HEADER  4


typedef enum {
  ASPEP_IDLE,
  ASPEP_CONFIGURING,
  ASPEP_CONNECTING,
  ASPEP_CONNECTED,
  ASPEP_TRANSACTION,
  ASPEP_PING,
  ASPEP_RECOVERY,
} ASPEP_sm_type;

typedef enum {
  beacon = BEACON,
  ping = PING,
  data = DATA_PACKET,
  response = RESPONSE,
  error = ERROR_PACKET,
  async = ASYNC
} ASPEP_packetType_def;

typedef struct {
  uint8_t DATA_CRC;
  uint8_t RX_maxSize;
  uint8_t TXS_maxSize;
  uint8_t TXA_maxSize;
  uint8_t version;
}ASPEP_Capabilities_def;

typedef enum {
  Tbeacon = TBEACON,
  Tping = TPING,
  Ttransaction = TTRANSACTION,
  Tsilence = TSILENCE,
} ASPEP_Timeout;

typedef struct {
  bool SM_Event_Connected;
  bool SM_Event_Response;
  bool SM_Event_Ping;
  bool SM_Event_Async;
} ASPEP_Event_StateMachine;

typedef enum {
  configured = CONFIGURED,
  unconfigured = UNCONFIGURED,
} ASPEP_pingType_def;

/* ASPEP Transport Layer Sate Machine Type*/
typedef enum {
  WAITING_PACKET,
  WAITING_PAYLOAD,
} ASPEP_TL_sm_type;

typedef enum {
  RECOVERY_BEACON,
  RECOVERY_SILENCE,
} ASPEP_RECOVERY_sm_type;

/* Private defines -----------------------------------------------------------*/

typedef bool (* ASPEP_send_cb_t )    (void* pHW_Handle, void *txbuffer, uint16_t length);
typedef void (* ASPEP_receive_cb_t ) (void* pHW_Handle, void *rxbuffer, uint16_t length);
typedef void (* ASPEP_hwinit_cb_t )  (void* pHW_Handle);
typedef void (* ASPEP_hwsync_cb_t )  (void* pHW_Handle);

typedef enum {
  freeBuf = 0,
  writeLock = 1,
  pending = 2,
  readLock =3,
}buff_access_t;

typedef struct {
  uint8_t *buffer;
  uint16_t length;
  buff_access_t state;
} MCTL_Buff_t;


typedef struct {

  void* HWIp;
  uint8_t txHeader[ASPEP_HEADER_SIZE];  /* Contains the ASPEP 32 bits header*/
  uint8_t rxHeader[ASPEP_HEADER_SIZE]; /* Contains the ASPEP 32 bits header*/
  uint8_t * rxBuffer; /* Contains the ASPEP Data payload */
  uint16_t rxLength;
  uint16_t txLength;
  MCTL_Buff_t syncBuffer;
  MCTL_Buff_t asyncBufferA;
  MCTL_Buff_t asyncBufferB;

  MCTL_Buff_t * LastUsedAsyncBuff;

  uint16_t maxRXPayload;
  ASPEP_sm_type ASPEP_State;

  ASPEP_packetType_def rxPacketType;
  ASPEP_packetType_def rxPacketTypePending;
  ASPEP_packetType_def CurrentrxPacketType;

  ASPEP_pingType_def rxPingCbit;
  ASPEP_Capabilities_def Capabilities;
  uint8_t badPacketFlag;
  uint16_t packetNumber;

  bool NewPacketAvailable;
  bool NewPacketAvailablePending;

  bool PacketPending;

  ASPEP_TL_sm_type  ASPEP_TL_State;
  ASPEP_RECOVERY_sm_type  ASPEP_RECOVERY_State;
  ASPEP_Event_StateMachine ASPEP_Events;
  uint16_t counter;
  ASPEP_Timeout Timeout;
  void * PacketToSend;
  bool PacketReady;

  bool WaitingForAnswer;

  uint16_t txSyncMaxPayload;
  uint16_t txAsyncMaxPayload;

  /* Callback */
  ASPEP_send_complete_cb_t ASPEP_send_complete_cb;
  ASPEP_after_sending_cb_t ASPEP_after_sending_cb;
  ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb;
  ASPEP_delay_cb_t ASPEP_delay_cb;
} ASPEP_Handle_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static ASPEP_Handle_t aspepHandle;

/**
 * @Brief ASPEP Peripheral definition
 */
UASPEP_Handle_t UASPEP_B =
{
 .USARTx = USARTB,
 .rxDMA = DMA_RX_B,
 .txDMA = DMA_TX_B,
 .rxChannel = DMACH_RX_B,
 .txChannel = DMACH_TX_B,
};

/* Private function prototypes -----------------------------------------------*/
static void ASPEP_Recovery(void);
static void ASPEP_TimeoutHandle (void);
static void ASPEP_TXframeProcess (void *txBuffer, uint16_t bufferLength);
static void ASPEP_sm(ASPEP_sm_type state, uint16_t *packetLength, uint8_t** result);
static void ASPEP_Configure_sm(void);
static void ASPEP_Connecting_sm(void);
static void ASPEP_Connected_sm(uint16_t *packetLength, uint8_t** result);
static void ASPEP_Transaction_sm(uint16_t *packetLength, uint8_t** result);
static void ASPEP_Ping_sm(uint16_t *packetLength, uint8_t** result);
static void ASPEP_Recovery_sm(void);

static void ASPEP_TXframeProcessNoCrc (void *txBuffer, uint16_t bufferLength);

void ASPEP_HWDMAReset (void);
void ASPEP_DataTransmittedIT(void);

/* Functions Definition ------------------------------------------------------*/
#define MIN(a,b) ( (a < b) ? a : b )

bool ASPEP_CheckBeacon (ASPEP_Handle_t * pHandle, ASPEP_Capabilities_def *SlaveCapabilities);
static void ASPEP_SendBeacon (void);
static void ASPEP_SendPing (void );
bool ASPEP_CheckHeaderCRC( uint32_t header );

uint8_t const CRC4_Lookup4[] = {
  /*   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   A,     B,    C,    D,    E,    F */
    0x00, 0x07, 0x0e, 0x09, 0x0b, 0x0c, 0x05, 0x02, 0x01, 0x06, 0x0f, 0x08, 0x0a, 0x0d, 0x04, 0x03
};

#ifndef CRC4_PER_NIBBLE
uint8_t const CRC4_Lookup8[] = {
  /*          0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   A,     B,    C,    D,    E,    F */
  /* 0 */  0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x07, 0x05, 0x03, 0x01, 0x0f, 0x0d, 0x0b, 0x09,
  /* 1 */  0x07, 0x05, 0x03, 0x01, 0x0f, 0x0d, 0x0b, 0x09, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e,
  /* 2 */  0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00, 0x09, 0x0b, 0x0d, 0x0f, 0x01, 0x03, 0x05, 0x07,
  /* 3 */  0x09, 0x0b, 0x0d, 0x0f, 0x01, 0x03, 0x05, 0x07, 0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00,
  /* 4 */  0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05, 0x0c, 0x0e, 0x08, 0x0a, 0x04, 0x06, 0x00, 0x02,
  /* 5 */  0x0c, 0x0e, 0x08, 0x0a, 0x04, 0x06, 0x00, 0x02, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
  /* 6 */  0x05, 0x07, 0x01, 0x03, 0x0d, 0x0f, 0x09, 0x0b, 0x02, 0x00, 0x06, 0x04, 0x0a, 0x08, 0x0e, 0x0c,
  /* 7 */  0x02, 0x00, 0x06, 0x04, 0x0a, 0x08, 0x0e, 0x0c, 0x05, 0x07, 0x01, 0x03, 0x0d, 0x0f, 0x09, 0x0b,
  /* 8 */  0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f, 0x06, 0x04, 0x02, 0x00, 0x0e, 0x0c, 0x0a, 0x08,
  /* 9 */  0x06, 0x04, 0x02, 0x00, 0x0e, 0x0c, 0x0a, 0x08, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f,
  /* A */  0x0f, 0x0d, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01, 0x08, 0x0a, 0x0c, 0x0e, 0x00, 0x02, 0x04, 0x06,
  /* B */  0x08, 0x0a, 0x0c, 0x0e, 0x00, 0x02, 0x04, 0x06, 0x0f, 0x0d, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01,
  /* C */  0x0a, 0x08, 0x0e, 0x0c, 0x02, 0x00, 0x06, 0x04, 0x0d, 0x0f, 0x09, 0x0b, 0x05, 0x07, 0x01, 0x03,
  /* D */  0x0d, 0x0f, 0x09, 0x0b, 0x05, 0x07, 0x01, 0x03, 0x0a, 0x08, 0x0e, 0x0c, 0x02, 0x00, 0x06, 0x04,
  /* E */  0x04, 0x06, 0x00, 0x02, 0x0c, 0x0e, 0x08, 0x0a, 0x03, 0x01, 0x07, 0x05, 0x0b, 0x09, 0x0f, 0x0d,
  /* F */  0x03, 0x01, 0x07, 0x05, 0x0b, 0x09, 0x0f, 0x0d, 0x04, 0x06, 0x00, 0x02, 0x0c, 0x0e, 0x08, 0x0a
};
#endif /* CRC4_PER_NIBBLE */

void ASPEP_ComputeHeaderCRC( uint32_t* headerPtr )
{
    uint8_t crc = 0;
    uint32_t header = *headerPtr;

#ifndef CRC4_PER_NIBBLE
    header &= 0x0fffffff;

    crc = CRC4_Lookup8[ crc ^ (uint8_t)( header        & 0xff) ];
    crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 8 ) & 0xff) ];
    crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 16) & 0xff) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0x0f) ];
#else /* CRC4_PER_NIBBLE */
    crc = CRC4_Lookup4[ crc ^ (uint8_t)( header        & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  4) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  8) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 12) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 16) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 20) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0xf) ];
#endif /* CRC4_PER_NIBBLE */

    *headerPtr |= (uint32_t)crc << 28;

    return;
}

bool ASPEP_CheckHeaderCRC( uint32_t header )
{
    uint8_t crc = 0;

#ifndef CRC4_PER_NIBBLE
    crc = CRC4_Lookup8[ crc ^ (uint8_t)( header        & 0xff) ];
    crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 8 ) & 0xff) ];
    crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 16) & 0xff) ];
    crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 24) & 0xff) ];
#else /* CRC4_PER_NIBBLE */
    crc = CRC4_Lookup4[ crc ^ (uint8_t)( header        & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  4) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  8) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 12) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 16) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 20) & 0xf) ];
    crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0xf) ];
    crc = crc ^ (uint8_t)((header >> 28) & 0xf);
#endif /* CRC4_PER_NIBBLE */

    return crc == 0;
}

bool ASPEP_CheckBeacon (ASPEP_Handle_t * pHandle, ASPEP_Capabilities_def *SlaveCapabilities)
{
  uint32_t packetHeader = *((uint32_t *)pHandle->rxHeader);
  bool result = true;
  SlaveCapabilities->version =  (packetHeader &0x70)>> 4; /*Bits 4 to 6*/
  SlaveCapabilities->DATA_CRC = pHandle->rxHeader[0] >> 7 ;      /*Bit 7 */
  SlaveCapabilities->RX_maxSize =  pHandle->rxHeader[1] &0x3F; /*Bits 8 to  13*/
  SlaveCapabilities->TXS_maxSize = (packetHeader&0x1FC000)  >> 14; /*Bits 14 to 20 */
  SlaveCapabilities->TXA_maxSize = (packetHeader&0xFE00000) >> 21; /*Bits 21 to 27  */
  if ((SlaveCapabilities->DATA_CRC != pHandle->Capabilities.DATA_CRC) ||
      (SlaveCapabilities->RX_maxSize != pHandle->Capabilities.RX_maxSize) || /* Data packet the master can send is bigger than slave can receive */
      (pHandle->Capabilities.TXS_maxSize != SlaveCapabilities->TXS_maxSize ) || /*Sync packet size alignement is required in order for the master to be able to store it, and to not request a response bigger than slave capability */
      (pHandle->Capabilities.TXA_maxSize != SlaveCapabilities->TXA_maxSize ) || /*Async packet the slave can send is bigger than the master can receive (but master must not request a bigger packet than slave can transmit */
      (SlaveCapabilities->version != pHandle->Capabilities.version) ) /* For the time being, master version has to match slave version */
  {
    result = false;
  }
  return result;
}

void ASPEP_Init(void)
{
    UASPEP_INIT(&UASPEP_B);

    /* Set Capabilities */
    aspepHandle.Capabilities.DATA_CRC = 0;
    aspepHandle.Capabilities.RX_maxSize =  (MCP_RX_SYNC_PAYLOAD_MAX>>5)-1;
    aspepHandle.Capabilities.TXS_maxSize = (MCP_TX_SYNC_PAYLOAD_MAX>>5)-1;
    aspepHandle.Capabilities.TXA_maxSize =  MCP_TX_ASYNC_PAYLOAD_MAX_B>>6;
    aspepHandle.Capabilities.version = 0x0;

    aspepHandle.ASPEP_State = ASPEP_IDLE;
    aspepHandle.ASPEP_TL_State = WAITING_PACKET;
    aspepHandle.ASPEP_RECOVERY_State = RECOVERY_BEACON;
    aspepHandle.NewPacketAvailable = false;

    aspepHandle.packetNumber = 0;
    aspepHandle.counter = 0;
    aspepHandle.WaitingForAnswer = false;
    aspepHandle.badPacketFlag = 0;


    aspepHandle.syncBuffer.buffer = (uint8_t*) aspepSyncBuffer;

      /* Synch Telemetries Rx buffer Allocation */
    aspepHandle.rxBuffer = (uint8_t*) aspepRxBuffer;

      /* Asynch Telemetries buffer Allocation */
    aspepHandle.asyncBufferA.buffer = aspepAsyncBuffA;

    aspepHandle.asyncBufferB.buffer = aspepAsyncBuffB;

    UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.rxHeader, ASPEP_HEADER_SIZE);
}

void ASPEP_StartConnection(void)
{
  ASPEP_SendBeacon();
  aspepHandle.ASPEP_State = ASPEP_CONFIGURING;
  aspepHandle.Timeout = Tbeacon;
  aspepHandle.counter = aspepHandle.Timeout; //start Tbeacon
}

void ASPEP_Disconnection(void)
{
  aspepHandle.ASPEP_State = ASPEP_IDLE;
}

void ASPEP_SendPing (void)
{
    uint32_t * packet = (uint32_t*) aspepHandle.txHeader;
    uint8_t Cbit = 0; //In Master Ping: reserved and set to 0 
    uint8_t Nbit = 0; //In Master Ping: reserved and set to 0 
    uint8_t ipID = 0; //In Master Ping: reserved and set to 0
    *packet = PING|(Cbit <<4)|(Cbit <<5)|(Nbit<<6)|(Nbit<<7)|(ipID<<8)|((aspepHandle.packetNumber) <<12);
    
    ASPEP_TXframeProcess (aspepHandle.txHeader, ASPEP_HEADER_SIZE);
    
    aspepHandle.packetNumber++;
}

static void ASPEP_SendBeacon(void)
{
  uint32_t * packet = (uint32_t*) aspepHandle.txHeader;
  *packet =( BEACON |
           (aspepHandle.Capabilities.version <<4) |
           (aspepHandle.Capabilities.DATA_CRC << 7) |
           (((uint32_t)aspepHandle.Capabilities.RX_maxSize) << 8) |
           (((uint32_t)aspepHandle.Capabilities.TXS_maxSize) << 14) |
           (((uint32_t)aspepHandle.Capabilities.TXA_maxSize) << 21));

  ASPEP_TXframeProcess (aspepHandle.txHeader, ASPEP_HEADER_SIZE);
}

void ASPEP_SendPacket (void *txBuffer, uint16_t txDataLength)
{
    uint8_t *packet;
    uint32_t *header;
    uint32_t tmpHeader;
    
    packet = (uint8_t *) txBuffer;
    packet = packet - ASPEP_HEADER_SIZE;
    header = (uint32_t *) packet;
    tmpHeader =  (txDataLength << 4) | DATA_PACKET;
    *header = tmpHeader;

    ASPEP_ComputeHeaderCRC (&tmpHeader);
    ASPEP_TXframeProcessNoCrc((uint8_t*)&tmpHeader , ASPEP_HEADER_SIZE);

    /* Add here a delay*/
    aspepHandle.ASPEP_delay_cb(2);

    ASPEP_TXframeProcessNoCrc(txBuffer , txDataLength);

    aspepHandle.ASPEP_State = ASPEP_TRANSACTION;
}

bool ASPEP_getBuffer (void **buffer)
{
  bool result = true;
  __disable_irq();

  if (aspepHandle.syncBuffer.state <= writeLock ) /* Possible values are free or writeLock*/
  {
    *buffer = &aspepHandle.syncBuffer.buffer[ASPEP_HEADER_SIZE];
    aspepHandle.syncBuffer.state = writeLock;
  }
  else
  {
    result = false;
  }

  __enable_irq();

  return result;
}

void ASPEP_TXframeProcess (void *txBuffer, uint16_t bufferLength)
{ 
    ASPEP_ComputeHeaderCRC ((uint32_t*)txBuffer);
    while (aspepHandle.WaitingForAnswer == true);
    UASPEP_SEND_PACKET(&UASPEP_B, txBuffer, bufferLength);
    if(NULL != aspepHandle.ASPEP_after_sending_cb)
    {
      aspepHandle.ASPEP_after_sending_cb();
    }
}

void ASPEP_TXframeProcessNoCrc(void *txBuffer, uint16_t bufferLength)
{
    while (aspepHandle.WaitingForAnswer == true);
    UASPEP_SEND_PACKET(&UASPEP_B, txBuffer, bufferLength);
    if(NULL != aspepHandle.ASPEP_after_sending_cb)
    {
      aspepHandle.ASPEP_after_sending_cb();
    }
}


void ASPEP_DataTransmittedIT(void)
{
  if(NULL != aspepHandle.ASPEP_send_complete_cb)
  {
    aspepHandle.ASPEP_send_complete_cb();
  }
}

void ASPEP_TimeoutHandle (void)
{
  switch (aspepHandle.ASPEP_State)
  {
    case ASPEP_CONFIGURING: //Tbeacon expired 
      ASPEP_SendBeacon();
      break;
      
    case ASPEP_CONNECTING: //Tping expired 
    case ASPEP_PING: //Tping expired
      ASPEP_SendPing();
      break;
       
    case ASPEP_CONNECTED: 
      break;
      
    case ASPEP_TRANSACTION: //TTransaction expired 
      ASPEP_SendPing();
      aspepHandle.ASPEP_State = ASPEP_PING;
      aspepHandle.ASPEP_Events.SM_Event_Ping = true;
      break;
      
  case ASPEP_RECOVERY :
      switch(aspepHandle.ASPEP_RECOVERY_State)
      {
      case RECOVERY_BEACON: //Tbeacon expired
        ASPEP_SendBeacon();
        break;
        
      case RECOVERY_SILENCE: //Tsilence expired
        ASPEP_SendBeacon ();
        aspepHandle.ASPEP_State = ASPEP_CONFIGURING;
        break;
      }
      break;

   case ASPEP_IDLE:
   default:
   break;
  }
}

void ASPEP_ReceivedIT(void)
{
  switch (aspepHandle.ASPEP_TL_State)
  {
  case WAITING_PACKET:
    if (ASPEP_CheckHeaderCRC (*(uint32_t*)aspepHandle.rxHeader) == true )
    {
      if (aspepHandle.NewPacketAvailable == true)
      {
        aspepHandle.PacketPending = true;
        aspepHandle.rxPacketTypePending = aspepHandle.rxPacketType;
      }
      aspepHandle.rxPacketType = (ASPEP_packetType_def) (aspepHandle.rxHeader[0] & ID_MASK);
       switch (aspepHandle.rxPacketType) {
       case response:
         aspepHandle.rxLength = (*((uint32_t *)aspepHandle.rxHeader) & 0x0001FFF0) >> 4;
         if (aspepHandle.rxLength <= aspepHandle.maxRXPayload)
         {
           UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.rxBuffer , aspepHandle.rxLength+ASPEP_DATACRC_SIZE*aspepHandle.Capabilities.DATA_CRC);
           aspepHandle.ASPEP_TL_State = WAITING_PAYLOAD;
         }
         else
         {
           aspepHandle.badPacketFlag = ASPEP_BAD_PACKET_SIZE;
         }
         break;
       case async:
         aspepHandle.rxLength = (*((uint32_t *)aspepHandle.rxHeader) & 0x0001FFF0) >> 4;
         if (aspepHandle.rxLength <= aspepHandle.txAsyncMaxPayload)
         { 
             if (aspepHandle.asyncBufferA.state == freeBuf)
            {
                aspepHandle.asyncBufferA.state = writeLock;
                UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.asyncBufferA.buffer , aspepHandle.rxLength+ASPEP_DATACRC_SIZE*aspepHandle.Capabilities.DATA_CRC);
                aspepHandle.ASPEP_TL_State = WAITING_PAYLOAD;
                aspepHandle.asyncBufferA.length = aspepHandle.rxLength;
                aspepHandle.LastUsedAsyncBuff = &aspepHandle.asyncBufferA;
            }
          
            else if (aspepHandle.asyncBufferB.state == freeBuf)
            {
                aspepHandle.asyncBufferB.state = writeLock;
                UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.asyncBufferB.buffer , aspepHandle.rxLength+ASPEP_DATACRC_SIZE*aspepHandle.Capabilities.DATA_CRC);
                aspepHandle.ASPEP_TL_State = WAITING_PAYLOAD;
                aspepHandle.asyncBufferB.length = aspepHandle.rxLength;
                aspepHandle.LastUsedAsyncBuff = &aspepHandle.asyncBufferB;
            } 
            else
            {
                aspepHandle.LastUsedAsyncBuff->state = writeLock;
                UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.LastUsedAsyncBuff->buffer , aspepHandle.rxLength+ASPEP_DATACRC_SIZE*aspepHandle.Capabilities.DATA_CRC);
                aspepHandle.ASPEP_TL_State = WAITING_PAYLOAD;
                aspepHandle.LastUsedAsyncBuff->length = aspepHandle.rxLength;
            }
         }
         break;
       case beacon:
       case ping:
           aspepHandle.NewPacketAvailable = true;

            if(aspepHandle.ASPEP_receive_new_packet_cb)
            {
              aspepHandle.ASPEP_receive_new_packet_cb();
            }

            aspepHandle.rxPingCbit = (ASPEP_pingType_def) (aspepHandle.rxHeader[0] & ID_MASK_PING); //configured or unconfigured
            UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.rxHeader, ASPEP_HEADER_SIZE);
        break;

       default:
         aspepHandle.badPacketFlag = ASPEP_BAD_PACKET_TYPE;
         UASPEP_IDLE_ENABLE(&UASPEP_B);
         break;
       }
    }
    else
    {
      aspepHandle.badPacketFlag = ASPEP_BAD_CRC_HEADER;
    }
    break;
    
  case WAITING_PAYLOAD:
    
    aspepHandle.ASPEP_TL_State = WAITING_PACKET;
    UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.rxHeader , ASPEP_HEADER_SIZE );
    
    if (aspepHandle.asyncBufferA.state == writeLock)
    {
      aspepHandle.asyncBufferA.state = readLock;
    }
    else if (aspepHandle.asyncBufferB.state == writeLock)
    {
      aspepHandle.asyncBufferB.state = readLock;
    } 

    if (aspepHandle.PacketPending == true)
    {
      aspepHandle.PacketPending = false;
      aspepHandle.NewPacketAvailablePending = true;

      if(aspepHandle.ASPEP_receive_new_packet_cb)
        {
        aspepHandle.ASPEP_receive_new_packet_cb();
        }
    }
    else 
    {
      aspepHandle.NewPacketAvailable = true;

      if(aspepHandle.ASPEP_receive_new_packet_cb)
      {
        aspepHandle.ASPEP_receive_new_packet_cb();
      }
    }
    break;
  }
}


static void ASPEP_Configure_sm(void)
{
  ASPEP_Capabilities_def SlaveCapabilities;

  switch(aspepHandle.CurrentrxPacketType)
  {
    case ping:  //unconfigured
      ASPEP_SendBeacon();
      aspepHandle.Timeout = Tbeacon;
      aspepHandle.counter = aspepHandle.Timeout; //restart Tbeacon
      break;
    case beacon:
      {
        if(ASPEP_CheckBeacon(&aspepHandle, &SlaveCapabilities) == false) //Beacon NOK
        {
          aspepHandle.Capabilities.DATA_CRC =    MIN(aspepHandle.Capabilities.DATA_CRC, SlaveCapabilities.DATA_CRC);
          aspepHandle.Capabilities.RX_maxSize =  MIN(aspepHandle.Capabilities.RX_maxSize, SlaveCapabilities.RX_maxSize);
          aspepHandle.Capabilities.TXS_maxSize = MIN(aspepHandle.Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
          aspepHandle.Capabilities.TXA_maxSize = MIN(aspepHandle.Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);

          ASPEP_SendBeacon();

          aspepHandle.Timeout = Tbeacon;
          aspepHandle.counter = aspepHandle.Timeout;
          //restart Tbeacon
        }
        else /*  beacon OK : Slave capabilities match Master capabilities.*/
        {
          aspepHandle.txSyncMaxPayload = (aspepHandle.Capabilities.TXS_maxSize + 1) * 32;
          aspepHandle.txAsyncMaxPayload = (aspepHandle.Capabilities.TXA_maxSize) * 64;
          aspepHandle.maxRXPayload = (aspepHandle.Capabilities.RX_maxSize + 1) * 32;

          ASPEP_SendPing();
          aspepHandle.ASPEP_State = ASPEP_CONNECTING;

          aspepHandle.Timeout = Tping;
          aspepHandle.counter = aspepHandle.Timeout; //start Tping
        }
        break;

        case response:
        case error:
        case async:
        default:
        break;
      }
  }
}

static void ASPEP_Connecting_sm(void)
{
  switch(aspepHandle.CurrentrxPacketType)
  {
    case beacon:
      {
        ASPEP_Capabilities_def SlaveCapabilities;
        if(ASPEP_CheckBeacon(&aspepHandle, &SlaveCapabilities) == false) //Unexpected : Beacon NOK
        {
          aspepHandle.Capabilities.DATA_CRC =    MIN(aspepHandle.Capabilities.DATA_CRC, SlaveCapabilities.DATA_CRC);
          aspepHandle.Capabilities.TXS_maxSize = MIN(aspepHandle.Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
          aspepHandle.Capabilities.TXA_maxSize = MIN(aspepHandle.Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);
          ASPEP_SendBeacon();
          aspepHandle.ASPEP_State = ASPEP_CONFIGURING;

          aspepHandle.Timeout = Tbeacon;
          aspepHandle.counter = aspepHandle.Timeout; //restart Tbeacon
        }
        else /*  beacon OK : Slave capabilities match Master capabilities.*/
        {
          ASPEP_SendPing();

          aspepHandle.Timeout = Tping;
          aspepHandle.counter = aspepHandle.Timeout; //start Tping
        }
        break;
      }

    case ping: //configured

      if(aspepHandle.rxPingCbit == configured)
      {
        aspepHandle.ASPEP_State = ASPEP_CONNECTED;
        aspepHandle.ASPEP_Events.SM_Event_Connected = true;
        aspepHandle.counter = 0; //disable timeout
      }
      break;

    default: //RESPONSE or ERROR
      ASPEP_SendBeacon();
      aspepHandle.ASPEP_State = ASPEP_CONFIGURING;
      aspepHandle.counter = Tbeacon; //restart Tbeacon
      break;
  }
}

static void ASPEP_Connected_sm(uint16_t *packetLength, uint8_t** result)
{
  if(aspepHandle.CurrentrxPacketType == async)
  {
    if(aspepHandle.asyncBufferA.state == readLock)
    {
      *packetLength = aspepHandle.asyncBufferA.length;
      *result = aspepHandle.asyncBufferA.buffer;
      aspepHandle.ASPEP_Events.SM_Event_Async = true;
      aspepHandle.asyncBufferA.length = 0;
    }

    else if(aspepHandle.asyncBufferB.state == readLock)
    {
      *packetLength = aspepHandle.asyncBufferB.length;
      *result = aspepHandle.asyncBufferB.buffer;
      aspepHandle.ASPEP_Events.SM_Event_Async = true;
      aspepHandle.asyncBufferB.length = 0;
    }
  }
}

static void ASPEP_Transaction_sm(uint16_t *packetLength, uint8_t** result)
{
  switch(aspepHandle.CurrentrxPacketType)
  {
    case response:
      *packetLength = aspepHandle.rxLength;
      *result = aspepHandle.rxBuffer;

      aspepHandle.ASPEP_Events.SM_Event_Response = true;
      aspepHandle.ASPEP_State = ASPEP_CONNECTED;
      aspepHandle.counter = 0; //disable timeout
      aspepHandle.rxLength = 0;
      break;

    case async:
      if(aspepHandle.asyncBufferA.state == readLock)
      {
        *packetLength = aspepHandle.asyncBufferA.length;
        *result = aspepHandle.asyncBufferA.buffer;
        aspepHandle.ASPEP_Events.SM_Event_Async = true;
        aspepHandle.asyncBufferA.length = 0;
      }

      else if(aspepHandle.asyncBufferB.state == readLock)
      {
        *packetLength = aspepHandle.asyncBufferB.length;
        *result = aspepHandle.asyncBufferB.buffer;
        aspepHandle.ASPEP_Events.SM_Event_Async = true;
        aspepHandle.asyncBufferB.length = 0;
      }
      break;

    case error:
      aspepHandle.PacketReady = true; //Send again last sent packet
      //restart Ttransaction
      aspepHandle.Timeout = Ttransaction;
      aspepHandle.counter = aspepHandle.Timeout;
      break;

    default: //unexpected
      ASPEP_Recovery();
      aspepHandle.ASPEP_State = ASPEP_RECOVERY;
      break;
  }
}

void ASPEP_Ping_sm(uint16_t *packetLength, uint8_t** result)
{
  switch(aspepHandle.CurrentrxPacketType)
  {
    case response:
      *packetLength = aspepHandle.rxLength;
      *result = aspepHandle.rxBuffer;
      aspepHandle.ASPEP_Events.SM_Event_Connected = true;
      aspepHandle.ASPEP_Events.SM_Event_Response = true;
      //stop Tping
      aspepHandle.counter = 0;
      aspepHandle.ASPEP_State = ASPEP_CONNECTED;

      break;

    case ping:

      if(aspepHandle.rxPingCbit == unconfigured)
      {
        ASPEP_SendBeacon();
        //start Tbeacon
        aspepHandle.Timeout = Tbeacon;
        aspepHandle.counter = aspepHandle.Timeout;
        aspepHandle.ASPEP_State = ASPEP_CONFIGURING;
      }

      else if(aspepHandle.rxPingCbit == configured)
      {
        uint32_t packetHeader = *((uint32_t *)aspepHandle.rxHeader);
        uint16_t RxPacketNumber = (packetHeader&0x0FFFF000 ) >> 12;

        if(RxPacketNumber == aspepHandle.packetNumber) //ping N
        {
          aspepHandle.ASPEP_State = ASPEP_TRANSACTION;
          aspepHandle.PacketReady = true; //witout payload?
        }
        else   //ping N-1
        {
          aspepHandle.ASPEP_State = ASPEP_TRANSACTION;
          aspepHandle.PacketReady = true;
        }
        aspepHandle.ASPEP_Events.SM_Event_Connected = true;
        //start Ttransaction
        aspepHandle.Timeout = Ttransaction;
        aspepHandle.counter = aspepHandle.Timeout;
      }

    default: //beacon, Error
      ASPEP_Recovery();
      aspepHandle.ASPEP_State = ASPEP_RECOVERY;
      break;
  }

}

static void ASPEP_Recovery_sm(void)
{
  switch(aspepHandle.ASPEP_RECOVERY_State)
  {
  case RECOVERY_BEACON :
    if (aspepHandle. CurrentrxPacketType == beacon)
    {
        //start Tsilence
        aspepHandle.Timeout = Tsilence;
        aspepHandle.counter = aspepHandle.Timeout;
        aspepHandle.ASPEP_RECOVERY_State = RECOVERY_SILENCE;
    }
    break;

  case RECOVERY_SILENCE:
    aspepHandle.ASPEP_RECOVERY_State = RECOVERY_BEACON;
    ASPEP_Recovery();
    break;
  }
}

static void ASPEP_sm(ASPEP_sm_type state, uint16_t *packetLength, uint8_t** result)
{
  switch (state)
      {
        case ASPEP_IDLE:
        case ASPEP_CONFIGURING:
        {
          ASPEP_Configure_sm();
        }
        break;

        case ASPEP_CONNECTING:
        {
          ASPEP_Connecting_sm();
        }
        break;

        case ASPEP_CONNECTED:
        {
          ASPEP_Connected_sm(packetLength, result);
        }
        break;

         case ASPEP_TRANSACTION:
         {
           ASPEP_Transaction_sm(packetLength, result);

         }
         break;

        case ASPEP_PING:
        {
          ASPEP_Ping_sm(packetLength, result);
        }
        break;

        case ASPEP_RECOVERY :
        {
          ASPEP_Recovery_sm();
        }
        break;
      }
}


uint8_t* ASPEP_RxFrameProcess (uint16_t *packetLength)
{
     uint8_t *result = NULL;
     *packetLength = 0;
  
     
    if (aspepHandle.counter >1)
    {
      aspepHandle.counter--;
    }
   
    else if (aspepHandle.counter == 1)
    {
      ASPEP_TimeoutHandle();
      aspepHandle.counter = aspepHandle.Timeout;
    }
    
    else if (aspepHandle.counter == 0)
    {
      //do nothing
    }
  
  if (aspepHandle.NewPacketAvailable == true || aspepHandle.NewPacketAvailablePending == true)
  {
    if (aspepHandle.NewPacketAvailable == true)
    {
      aspepHandle.NewPacketAvailable = false;
      if (aspepHandle.PacketPending == true)
      {
        aspepHandle.CurrentrxPacketType = aspepHandle.rxPacketTypePending;
        aspepHandle.rxPacketTypePending = aspepHandle.rxPacketType;
      }
      else
      {
        aspepHandle.CurrentrxPacketType = aspepHandle.rxPacketType;
      }
    }
     else if (aspepHandle.NewPacketAvailablePending == true)
    {
       aspepHandle.NewPacketAvailablePending = false;
       aspepHandle.CurrentrxPacketType = aspepHandle.rxPacketTypePending;
    } 
    
    aspepHandle.WaitingForAnswer = false;

    /*Run ASPEP State machine */
    ASPEP_sm(aspepHandle.ASPEP_State, packetLength, &result);
  }
  
  else if (aspepHandle.badPacketFlag > ASPEP_OK )
  {
    if (aspepHandle.badPacketFlag ==  ASPEP_BAD_CRC_HEADER || aspepHandle.badPacketFlag == ASPEP_BAD_PACKET_TYPE)
    {
      aspepHandle.badPacketFlag = ASPEP_OK;
      aspepHandle. ASPEP_State = ASPEP_RECOVERY;
      ASPEP_Recovery();
    }
    else
    {
      aspepHandle.badPacketFlag = ASPEP_OK;
    }
  }
  return result;
}

uint8_t ASPEP_GetStatus(uint8_t Flag, bool ClearFlag)
{
  if (ClearFlag == true)
  {
    switch(Flag)
    {
    case EVENT_CONNECTED:
      aspepHandle.ASPEP_Events.SM_Event_Connected = false;
      break;
      
    case EVENT_RESPONSE:
      aspepHandle.ASPEP_Events.SM_Event_Response = false;
      break;
      
    case EVENT_DISCONNECTED:
      aspepHandle.ASPEP_Events.SM_Event_Ping = false;
      break;
      
    case EVENT_ASYNC:
      aspepHandle.ASPEP_Events.SM_Event_Async = false;
      break;
    }
  }
  
  switch(Flag)
  {
  case EVENT_CONNECTED:
    return aspepHandle.ASPEP_Events.SM_Event_Connected;
    break;
    
  case EVENT_RESPONSE:
    return aspepHandle.ASPEP_Events.SM_Event_Response;
    break;
    
  case EVENT_DISCONNECTED:
    return aspepHandle.ASPEP_Events.SM_Event_Ping;
    break;
    
  case EVENT_ASYNC:
    return aspepHandle.ASPEP_Events.SM_Event_Async;
    break;
  }
  return 0;
}

void ASPEP_FreeBuffer(void)
{

  if(aspepHandle.asyncBufferA.state == readLock)
  {
    aspepHandle.asyncBufferA.state = freeBuf;
  }

  else if(aspepHandle.asyncBufferB.state == readLock)
  {
    aspepHandle.asyncBufferB.state = freeBuf;
  }
}

void ASPEP_Recovery(void)
{
    UASPEP_IDLE_ENABLE(&UASPEP_B);
    ASPEP_SendBeacon();
    aspepHandle.Timeout = Tbeacon;
    aspepHandle.counter = aspepHandle.Timeout; //start Tbeacon
}

void ASPEP_HWDMAReset (void)
{ /* We must reset the RX state machine to be sure to not be in Waiting packet state */
  /* Otherwise the arrival of a new packet will trigger a NewPacketAvailable despite */
  /* the fact that bytes have been lost because of overrun (debugger paused for instance) */
  aspepHandle.PacketPending = false;
  aspepHandle.asyncBufferA.state = freeBuf;
  aspepHandle.asyncBufferB.state = freeBuf;
  
  aspepHandle.ASPEP_TL_State = WAITING_PACKET;
  UASPEP_RECEIVE_BUFFER(&UASPEP_B, aspepHandle.rxHeader, ASPEP_HEADER_SIZE );
}

/* ASPEP Callback */
/******************/
void ASPEP_RegisterTxComplete_cb( ASPEP_send_complete_cb_t ASPEP_send_complete_cb)
{
  if(NULL != ASPEP_send_complete_cb)
  {
    aspepHandle.ASPEP_send_complete_cb = ASPEP_send_complete_cb;
  }
}

void ASPEP_RegisterTransmitting_cb( ASPEP_after_sending_cb_t ASPEP_after_sending_cb)
{
  if(NULL != ASPEP_after_sending_cb)
  {
    aspepHandle.ASPEP_after_sending_cb = ASPEP_after_sending_cb;
  }
}

void ASPEP_RegisterNewPacket_cb( ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb)
{
  if(NULL != ASPEP_receive_new_packet_cb)
  {
    aspepHandle.ASPEP_receive_new_packet_cb = ASPEP_receive_new_packet_cb;
  }
}

void ASPEP_RegisterDelay_cb( ASPEP_delay_cb_t ASPEP_delay_cb)
{
  if(NULL != ASPEP_delay_cb)
  {
    aspepHandle.ASPEP_delay_cb = ASPEP_delay_cb;
  }
}

void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */

  /* USER CODE BEGIN USART3_IRQn 1 */
  if(LL_USART_IsActiveFlag_TC(USARTB))
  {
    LL_DMA_DisableChannel( DMA_TX_B, DMACH_TX_B);
    LL_USART_ClearFlag_TC(USARTB);

    ASPEP_DataTransmittedIT();
  }

  if((LL_USART_IsActiveFlag_ORE(USARTB) || LL_USART_IsActiveFlag_FE(USARTB) || LL_USART_IsActiveFlag_NE(USARTB)) && LL_USART_IsEnabledIT_ERROR(USARTB))
  { /* Stopping the debugger will generate an OverRun error*/
    WRITE_REG(USARTB->ICR, USART_ICR_FECF|USART_ICR_ORECF|USART_ICR_NECF);
    /* We disable ERROR interrupt to avoid to trig one Overrun IT per additional byte recevied*/
    LL_USART_DisableIT_ERROR(USARTB);
    LL_USART_EnableIT_IDLE(USARTB);
  }

  if(LL_USART_IsActiveFlag_IDLE(USARTB) && LL_USART_IsEnabledIT_IDLE(USARTB))
  {
    LL_USART_DisableIT_IDLE(USARTB);
    /* Once the complete unexpected data are received, we enable back the error IT*/
    LL_USART_EnableIT_ERROR(USARTB);
    /* To be sure we fetch the potential pending data*/
    /* We disable the DMA request, Read the dummy data, endable back the DMA request */

    LL_USART_DisableDMAReq_RX(USARTB);
    LL_USART_ReceiveData8(USARTB);
    LL_USART_EnableDMAReq_RX(USARTB);
    ASPEP_HWDMAReset();
    LL_USART_ClearFlag_IDLE(USARTB);
  }

  /* USER CODE END USART3_IRQn 1 */
}

void GPDMA1_Channel15_IRQHandler(void)
{
  /* USER CODE BEGIN GPDMA1_Channel15_IRQn 0 */

  /* USER CODE END GPDMA1_Channel15_IRQn 0 */
  if(LL_DMA_IsActiveFlag_TC(DMA_RX_B, DMACH_RX_B))
  {
    /* Clear full buffer transfer complete ISR*/
    LL_DMA_ClearFlag_TC(DMA_RX_B, DMACH_RX_B);

    ASPEP_ReceivedIT();
  }
  if(LL_DMA_IsActiveFlag_HT(DMA_RX_B, DMACH_RX_B))
  {
    LL_DMA_DisableIT_HT(DMA_RX_B, DMACH_RX_B);
  }

  /* USER CODE BEGIN GPDMA1_Channel15_IRQn 1 */

  /* USER CODE END GPDMA1_Channel15_IRQn 1 */
}
