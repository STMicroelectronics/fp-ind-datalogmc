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
#include <stdint.h>
#include <string.h>
#include "cmsis_compiler.h"

//#include "stm32g0xx_ll_gpio.h"
//#include "mcp_config.h"

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
#define MIN(a,b) ( (a < b) ? a : b )

bool ASPEP_CheckBeacon (ASPEP_Handle_t * pHandle, ASPEP_Capabilities_def *SlaveCapabilities);
void ASPEP_SendBeacon (ASPEP_Handle_t *pHandle, ASPEP_Capabilities_def *capabilities);
void ASPEP_SendPing (ASPEP_Handle_t *pHandle);
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

void ASPEP_Init(ASPEP_Handle_t *pHandle)
{
    pHandle->fASPEP_HWInit ( pHandle->HWIp );
    pHandle->ASPEP_State = ASPEP_IDLE;
    pHandle->ASPEP_TL_State = WAITING_PACKET;

    pHandle->ASPEP_RECOVERY_State = RECOVERY_BEACON;
    pHandle->NewPacketAvailable = false;
    
    pHandle->fASPEP_receive(pHandle->HWIp, pHandle->rxHeader , ASPEP_HEADER_SIZE );
}

void ASPEP_StartConnection(MCTL_Handle_t *pSupHandle)
{
  ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
  ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
  pHandle->ASPEP_State = ASPEP_CONFIGURING;
  pHandle->Timeout = Tbeacon;
  pHandle->counter = pHandle->Timeout; //start Tbeacon
}

void ASPEP_Disconnection(MCTL_Handle_t *pSupHandle)
{
  ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
  pHandle->ASPEP_State = ASPEP_IDLE;
}

void ASPEP_SendPing (ASPEP_Handle_t *pHandle)
{
    uint32_t * packet = (uint32_t*) pHandle->txHeader;
    uint8_t Cbit = 0; //In Master Ping: reserved and set to 0 
    uint8_t Nbit = 0; //In Master Ping: reserved and set to 0 
    uint8_t ipID = 0; //In Master Ping: reserved and set to 0
    *packet = PING|(Cbit <<4)|(Cbit <<5)|(Nbit<<6)|(Nbit<<7)|(ipID<<8)|((pHandle->packetNumber) <<12);
    
    ASPEP_TXframeProcess (pHandle, pHandle->txHeader, ASPEP_HEADER_SIZE);
    
    pHandle->packetNumber++;
}

void ASPEP_SendBeacon (ASPEP_Handle_t *pHandle, ASPEP_Capabilities_def *capabilities)
{
  uint32_t * packet = (uint32_t*) pHandle->txHeader;
  *packet =( BEACON |
           (capabilities->version <<4) |
           (capabilities->DATA_CRC << 7) |
           (((uint32_t)capabilities->RX_maxSize) << 8) |
           (((uint32_t)capabilities->TXS_maxSize) << 14) |
           (((uint32_t)capabilities->TXA_maxSize) << 21));

  ASPEP_TXframeProcess (pHandle, pHandle->txHeader, ASPEP_HEADER_SIZE);
}

void ASPEP_SendPacket (MCTL_Handle_t *pSupHandle, void *txBuffer, uint16_t txDataLength)
{
    uint8_t *packet;
    uint32_t *header;
    uint32_t tmpHeader;
    ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
    
    packet = (uint8_t *) txBuffer;
    packet = packet - ASPEP_HEADER_SIZE;
    header = (uint32_t *) packet;
    tmpHeader =  (txDataLength << 4) | DATA_PACKET;
    *header = tmpHeader;

    ASPEP_ComputeHeaderCRC (&tmpHeader);
    ASPEP_TXframeProcessNoCrc (pHandle, (uint8_t*)&tmpHeader , ASPEP_HEADER_SIZE);

    /* Add here a delay*/
    pHandle->ASPEP_delay_cb(2);

    ASPEP_TXframeProcessNoCrc(pHandle, txBuffer , txDataLength);

    pHandle-> ASPEP_State = ASPEP_TRANSACTION;
}

bool ASPEP_getBuffer (MCTL_Handle_t *pSupHandle, void **buffer)
{
  ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
  bool result = true;
  __disable_irq();

  if (pHandle->syncBuffer.state <= writeLock ) /* Possible values are free or writeLock*/
  {
    *buffer = &pHandle->syncBuffer.buffer[ASPEP_HEADER_SIZE];
    pHandle->syncBuffer.state = writeLock;
  }
  else
  {
    result = false;
  }

  __enable_irq();

  return result;
}

void ASPEP_TXframeProcess (ASPEP_Handle_t *pHandle, void *txBuffer, uint16_t bufferLength)
{ 
    ASPEP_ComputeHeaderCRC ((uint32_t*)txBuffer);
    while (pHandle->WaitingForAnswer == true);
    pHandle->fASPEP_send (pHandle ->HWIp, txBuffer, bufferLength);
    if(NULL != pHandle->ASPEP_after_sending_cb)
    {
      pHandle->ASPEP_after_sending_cb();
    }
}

void ASPEP_TXframeProcessNoCrc (ASPEP_Handle_t *pHandle, void *txBuffer, uint16_t bufferLength)
{
    while (pHandle->WaitingForAnswer == true);
    pHandle->fASPEP_send (pHandle ->HWIp, txBuffer, bufferLength);
    if(NULL != pHandle->ASPEP_after_sending_cb)
    {
      pHandle->ASPEP_after_sending_cb();
    }
}


void ASPEP_DataTransmittedIT(ASPEP_Handle_t *pHandle)
{
  if(NULL != pHandle->ASPEP_send_complete_cb)
  {
    pHandle->ASPEP_send_complete_cb();
  }
}

void ASPEP_TimeoutHandle (ASPEP_Handle_t *pHandle)
{
  switch (pHandle-> ASPEP_State)
  {
    case ASPEP_CONFIGURING: //Tbeacon expired 
      ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
      break;
      
    case ASPEP_CONNECTING: //Tping expired 
    case ASPEP_PING: //Tping expired
      ASPEP_SendPing(pHandle);
      break;
       
    case ASPEP_CONNECTED: 
      break;
      
    case ASPEP_TRANSACTION: //TTransaction expired 
      ASPEP_SendPing(pHandle);
      pHandle-> ASPEP_State = ASPEP_PING;
      pHandle->ASPEP_Events.SM_Event_Ping = true;
      break;
      
  case ASPEP_RECOVERY :
      switch(pHandle->ASPEP_RECOVERY_State)
      {
      case RECOVERY_BEACON: //Tbeacon expired
        ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
        break;
        
      case RECOVERY_SILENCE: //Tsilence expired
        ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
        pHandle-> ASPEP_State = ASPEP_CONFIGURING;
        break;
      }
      break;

   case ASPEP_IDLE:
   default:
   break;
  }
}

void ASPEP_ReceivedIT (ASPEP_Handle_t *pHandle)
{
  switch (pHandle->ASPEP_TL_State) 
  {
  case WAITING_PACKET:
    if (ASPEP_CheckHeaderCRC (*(uint32_t*)pHandle->rxHeader) == true )
    {
      if (pHandle->NewPacketAvailable == true)
      {
        pHandle->PacketPending = true;
        pHandle->rxPacketTypePending = pHandle->rxPacketType;
      }
       pHandle->rxPacketType = (ASPEP_packetType_def) (pHandle->rxHeader[0] & ID_MASK);
       switch (pHandle->rxPacketType) {
       case response:
         pHandle->rxLength = (*((uint32_t *)pHandle->rxHeader) & 0x0001FFF0) >> 4;
         if (pHandle->rxLength <= pHandle->maxRXPayload)
         {
           pHandle->fASPEP_receive(pHandle->HWIp, pHandle->rxBuffer , pHandle->rxLength+ASPEP_DATACRC_SIZE*pHandle->Capabilities.DATA_CRC);
           pHandle->ASPEP_TL_State = WAITING_PAYLOAD;
         }
         else
         {
           pHandle->badPacketFlag = ASPEP_BAD_PACKET_SIZE;
         }
         break;
       case async:
         pHandle->rxLength = (*((uint32_t *)pHandle->rxHeader) & 0x0001FFF0) >> 4;
         if (pHandle->rxLength <= pHandle->_Super.txAsyncMaxPayload)
         { 
             if (pHandle->asyncBufferA.state == freeBuf)
            {
                pHandle->asyncBufferA.state = writeLock;
                pHandle->fASPEP_receive(pHandle->HWIp, pHandle->asyncBufferA.buffer , pHandle->rxLength+ASPEP_DATACRC_SIZE*pHandle->Capabilities.DATA_CRC);
                pHandle->ASPEP_TL_State = WAITING_PAYLOAD;
                pHandle->asyncBufferA.length = pHandle->rxLength;
                pHandle->LastUsedAsyncBuff = &pHandle->asyncBufferA;
            }
          
            else if (pHandle->asyncBufferB.state == freeBuf)
            {
                pHandle->asyncBufferB.state = writeLock;
                pHandle->fASPEP_receive(pHandle->HWIp, pHandle->asyncBufferB.buffer , pHandle->rxLength+ASPEP_DATACRC_SIZE*pHandle->Capabilities.DATA_CRC);
                pHandle->ASPEP_TL_State = WAITING_PAYLOAD;
                pHandle->asyncBufferB.length = pHandle->rxLength;
                pHandle->LastUsedAsyncBuff = &pHandle->asyncBufferB;
            } 
            else
            {
                pHandle->LastUsedAsyncBuff->state = writeLock;
                pHandle->fASPEP_receive(pHandle->HWIp, pHandle->LastUsedAsyncBuff->buffer , pHandle->rxLength+ASPEP_DATACRC_SIZE*pHandle->Capabilities.DATA_CRC);
                pHandle->ASPEP_TL_State = WAITING_PAYLOAD;
                pHandle->LastUsedAsyncBuff->length = pHandle->rxLength;
            }
         }
         break;
       case beacon:
       case ping:
            pHandle->NewPacketAvailable = true;

            if(pHandle->ASPEP_receive_new_packet_cb)
            {
              pHandle->ASPEP_receive_new_packet_cb();
            }

            pHandle->rxPingCbit = (ASPEP_pingType_def) (pHandle->rxHeader[0] & ID_MASK_PING); //configured or unconfigured
            pHandle->fASPEP_receive(pHandle->HWIp, pHandle->rxHeader, ASPEP_HEADER_SIZE);
        break;

       default:
         pHandle->badPacketFlag = ASPEP_BAD_PACKET_TYPE;
         pHandle->fASPEP_HWSync (pHandle->HWIp);
         break;
       }
    }
    else
    {
      pHandle->badPacketFlag = ASPEP_BAD_CRC_HEADER;
    }
    break;
    
  case WAITING_PAYLOAD:
    
    pHandle->ASPEP_TL_State = WAITING_PACKET;
    pHandle->fASPEP_receive(pHandle->HWIp, pHandle->rxHeader , ASPEP_HEADER_SIZE ); 
    
    if (pHandle->asyncBufferA.state == writeLock)
    {
      pHandle->asyncBufferA.state = readLock;
    }
    else if (pHandle->asyncBufferB.state == writeLock)
    {
      pHandle->asyncBufferB.state = readLock;
    } 

    if (pHandle->PacketPending == true)
    {
      pHandle->PacketPending = false;
      pHandle->NewPacketAvailablePending = true;

      if(pHandle->ASPEP_receive_new_packet_cb)
        {
          pHandle->ASPEP_receive_new_packet_cb();
        }
    }
    else 
    {
      pHandle->NewPacketAvailable = true;

      if(pHandle->ASPEP_receive_new_packet_cb)
      {
        pHandle->ASPEP_receive_new_packet_cb();
      }
    }
    break;
  }
}


uint8_t* ASPEP_RxFrameProcess (MCTL_Handle_t *pSupHandle, uint16_t *packetLength)
{
     uint8_t* result = NULL;
     *packetLength = 0;
  
     ASPEP_Handle_t * pHandle = (ASPEP_Handle_t *) pSupHandle;
     ASPEP_Capabilities_def SlaveCapabilities;
  
     uint32_t packetHeader = *((uint32_t *)pHandle->rxHeader);
     uint16_t RxPacketNumber = (packetHeader&0x0FFFF000 ) >> 12;
     
    if (pHandle->counter >1)
    {
      pHandle->counter--;
    }
   
    else if (pHandle->counter == 1)
    {
      ASPEP_TimeoutHandle(pHandle);
      pHandle->counter = pHandle->Timeout;
    }
    
    else if (pHandle->counter == 0)
    {
      //do nothing
    }
  
  if (pHandle->NewPacketAvailable == true || pHandle->NewPacketAvailablePending == true)
  {
    if (pHandle->NewPacketAvailable == true)
    {
      pHandle->NewPacketAvailable = false;
      if (pHandle->PacketPending == true)
      {
        pHandle->CurrentrxPacketType = pHandle-> rxPacketTypePending;
        pHandle->rxPacketTypePending = pHandle-> rxPacketType;
      }
      else
      {
        pHandle->CurrentrxPacketType = pHandle-> rxPacketType;
      }
    }
     else if (pHandle->NewPacketAvailablePending == true)
    {
      pHandle->NewPacketAvailablePending = false;
      pHandle-> CurrentrxPacketType = pHandle->rxPacketTypePending;
    } 
    
    pHandle->WaitingForAnswer = false;
    switch (pHandle-> ASPEP_State)
    {
      case ASPEP_IDLE:

      case ASPEP_CONFIGURING:
        switch(pHandle-> CurrentrxPacketType)
        {
            case ping:  //unconfigured
               ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
               pHandle->Timeout = Tbeacon;
               pHandle->counter = pHandle->Timeout; //restart Tbeacon
               break;
            case beacon: {
                if (ASPEP_CheckBeacon(pHandle,&SlaveCapabilities) == false) //Beacon NOK
                {
                    pHandle->Capabilities.DATA_CRC = MIN(pHandle->Capabilities.DATA_CRC, SlaveCapabilities.DATA_CRC);
                    pHandle->Capabilities.RX_maxSize = MIN(pHandle->Capabilities.RX_maxSize, SlaveCapabilities.RX_maxSize);
                    pHandle->Capabilities.TXS_maxSize = MIN(pHandle->Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
                    pHandle->Capabilities.TXA_maxSize = MIN(pHandle->Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);
                    
                    ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);

                    pHandle->Timeout = Tbeacon;
                    pHandle->counter = pHandle->Timeout;
                    //restart Tbeacon
                }
                else  /*  beacon OK : Slave capabilities match Master capabilities.*/
                {  
                    pSupHandle->txSyncMaxPayload = (pHandle->Capabilities.TXS_maxSize+1)*32;
                    pSupHandle->txAsyncMaxPayload = (pHandle->Capabilities.TXA_maxSize)*64;
                    pHandle->maxRXPayload = (pHandle->Capabilities.RX_maxSize+1)*32;
               
                    ASPEP_SendPing(pHandle);
                    pHandle-> ASPEP_State = ASPEP_CONNECTING;

                    pHandle->Timeout = Tping;
                    pHandle->counter = pHandle->Timeout; //start Tping
                }
                break;

            case response:
            case error:
            case async:
            default:
            	break;
            }
        }
        break;
        
      case ASPEP_CONNECTING:
         switch(pHandle-> CurrentrxPacketType)
         {
            case beacon: {
                if (ASPEP_CheckBeacon(pHandle,&SlaveCapabilities) == false) //Unexpected : Beacon NOK
                {
                    pHandle->Capabilities.DATA_CRC = MIN(pHandle->Capabilities.DATA_CRC ,SlaveCapabilities.DATA_CRC);
                    pHandle->Capabilities.TXS_maxSize = MIN(pHandle->Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
                    pHandle->Capabilities.TXA_maxSize = MIN(pHandle->Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);
                    ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
                    pHandle-> ASPEP_State = ASPEP_CONFIGURING;

                    pHandle->Timeout = Tbeacon;
                    pHandle->counter = pHandle->Timeout; //restart Tbeacon
                }
                else /*  beacon OK : Slave capabilities match Master capabilities.*/
                {
                    ASPEP_SendPing(pHandle);
 
                    pHandle->Timeout = Tping;
                    pHandle->counter = pHandle->Timeout; //start Tping
                }
                break; }

            case ping: //configured
                
                if (pHandle->rxPingCbit == configured) 
                {
                  pHandle-> ASPEP_State = ASPEP_CONNECTED;
                  pHandle->ASPEP_Events.SM_Event_Connected = true;
                  pHandle->counter = 0; //disable timeout
                }
                break;

            default: //RESPONSE or ERROR
                ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
                pHandle-> ASPEP_State = ASPEP_CONFIGURING;
                pHandle->counter = Tbeacon; //restart Tbeacon
                break;
        }
        break;
        
      case ASPEP_CONNECTED:
        if (pHandle-> CurrentrxPacketType == async)
        {    
            if (pHandle->asyncBufferA.state == readLock)
            {
              *packetLength = pHandle->asyncBufferA.length;
              result = pHandle->asyncBufferA.buffer;
              pHandle->ASPEP_Events.SM_Event_Async = true;
              pHandle->asyncBufferA.length = 0;
            }
          
            else if (pHandle->asyncBufferB.state == readLock)
            {
              *packetLength = pHandle->asyncBufferB.length;
              result = pHandle->asyncBufferB.buffer;
              pHandle->ASPEP_Events.SM_Event_Async = true;
              pHandle->asyncBufferB.length = 0;
            } 
        }
        break;
        
       case ASPEP_TRANSACTION:
          switch(pHandle-> CurrentrxPacketType)
          {
          case response:
              *packetLength = pHandle->rxLength;
              result = pHandle->rxBuffer;
              
              pHandle->ASPEP_Events.SM_Event_Response = true;
              pHandle-> ASPEP_State = ASPEP_CONNECTED;
              pHandle->counter = 0; //disable timeout
              
              pHandle->rxLength = 0;
              break;
                
          case async:
             if (pHandle->asyncBufferA.state == readLock)
              {
                  *packetLength = pHandle->asyncBufferA.length;
                  result = pHandle->asyncBufferA.buffer;
                  pHandle->ASPEP_Events.SM_Event_Async = true;
                  pHandle->asyncBufferA.length = 0;
              }
              
              else if (pHandle->asyncBufferB.state == readLock)
              {
                  *packetLength = pHandle->asyncBufferB.length;
                  result = pHandle->asyncBufferB.buffer;
                  pHandle->ASPEP_Events.SM_Event_Async = true;
                  pHandle->asyncBufferB.length = 0;
              } 
              break;
              
          case error: 
              pHandle->PacketReady=true; //Send again last sent packet
              //restart Ttransaction
              pHandle->Timeout = Ttransaction;
              pHandle->counter = pHandle->Timeout;
              break;  
                
          default: //unexpected
              ASPEP_Recovery(pHandle);
              pHandle-> ASPEP_State = ASPEP_RECOVERY;
              break;
         }
          break;
 
      case ASPEP_PING:
        switch(pHandle-> CurrentrxPacketType)
        { 
            case response:
                *packetLength = pHandle->rxLength;
                result = pHandle->rxBuffer;
                pHandle->ASPEP_Events.SM_Event_Connected = true;
                pHandle->ASPEP_Events.SM_Event_Response = true;
                //stop Tping
                pHandle->counter = 0;
                pHandle-> ASPEP_State = ASPEP_CONNECTED;
                
                break;
            
            case ping:
 
                if (pHandle->rxPingCbit == unconfigured) {
                    ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
                    //start Tbeacon
                    pHandle->Timeout = Tbeacon;
                    pHandle->counter = pHandle->Timeout;
                    pHandle-> ASPEP_State = ASPEP_CONFIGURING;
                }
                
                else if (pHandle->rxPingCbit == configured)
                {
                    if (RxPacketNumber == pHandle->packetNumber) //ping N
                    { 
                        pHandle-> ASPEP_State = ASPEP_TRANSACTION;
                        pHandle->PacketReady = true; //witout payload?
                    }
                    else   //ping N-1
                    {
                        pHandle-> ASPEP_State = ASPEP_TRANSACTION;
                        pHandle->PacketReady = true;
                    }
                    pHandle->ASPEP_Events.SM_Event_Connected = true;
                    //start Ttransaction
                    pHandle->Timeout = Ttransaction;
                    pHandle->counter = pHandle->Timeout;
                }
                    
            default: //beacon, Error
              ASPEP_Recovery(pHandle);
              pHandle-> ASPEP_State = ASPEP_RECOVERY;
              break;
        }
        break;
    
      case ASPEP_RECOVERY :
        switch(pHandle->ASPEP_RECOVERY_State)
        {
        case RECOVERY_BEACON :
          if (pHandle-> CurrentrxPacketType == beacon)
          {
              //start Tsilence
              pHandle->Timeout = Tsilence;
              pHandle->counter = pHandle->Timeout;
              pHandle->ASPEP_RECOVERY_State = RECOVERY_SILENCE;
          }
          break;
          
        case RECOVERY_SILENCE:
          pHandle->ASPEP_RECOVERY_State = RECOVERY_BEACON;
          ASPEP_Recovery(pHandle);
          break;
        }
    }
  }
  
  else if (pHandle->badPacketFlag > ASPEP_OK )
  {
    if (pHandle->badPacketFlag ==  ASPEP_BAD_CRC_HEADER || pHandle->badPacketFlag == ASPEP_BAD_PACKET_TYPE)
    {
      pHandle->badPacketFlag = ASPEP_OK;
      pHandle-> ASPEP_State = ASPEP_RECOVERY;
      ASPEP_Recovery(pHandle);
    }
    else
    {
      pHandle->badPacketFlag = ASPEP_OK;
    }
  }
   return result;
}

uint8_t ASPEP_GetStatus(MCTL_Handle_t *pSupHandle, uint8_t Flag, bool ClearFlag)
{
  ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
  
  if (ClearFlag == true)
  {
    switch(Flag)
    {
    case EVENT_CONNECTED:
      pHandle->ASPEP_Events.SM_Event_Connected = false;
      break;
      
    case EVENT_RESPONSE:
      pHandle->ASPEP_Events.SM_Event_Response = false;
      break;
      
    case EVENT_DISCONNECTED:
      pHandle->ASPEP_Events.SM_Event_Ping = false;
      break;
      
    case EVENT_ASYNC:
      pHandle->ASPEP_Events.SM_Event_Async = false;
      break;
    }
  }
  
  switch(Flag)
  {
  case EVENT_CONNECTED:
    return pHandle->ASPEP_Events.SM_Event_Connected;
    break;
    
  case EVENT_RESPONSE:
    return pHandle->ASPEP_Events.SM_Event_Response;
    break;
    
  case EVENT_DISCONNECTED:
    return pHandle->ASPEP_Events.SM_Event_Ping;
    break;
    
  case EVENT_ASYNC:
    return pHandle->ASPEP_Events.SM_Event_Async;
    break;
  }
  return 0;
}

void ASPEP_FreeBuffer(MCTL_Handle_t *pSupHandle)
{
    ASPEP_Handle_t *pHandle = (ASPEP_Handle_t *) pSupHandle;
    
    if (pHandle->asyncBufferA.state == readLock)
    {
      pHandle->asyncBufferA.state = freeBuf;
    }
   
    else if (pHandle->asyncBufferB.state == readLock)
    {
      pHandle->asyncBufferB.state = freeBuf;
    }
}

void ASPEP_Recovery(ASPEP_Handle_t *pHandle)
{
    pHandle->fASPEP_HWSync (pHandle->HWIp);
    ASPEP_SendBeacon (pHandle, &pHandle->Capabilities);
    pHandle->Timeout = Tbeacon;
    pHandle->counter = pHandle->Timeout; //start Tbeacon
}

void ASPEP_HWDMAReset (ASPEP_Handle_t *pHandle)
{ /* We must reset the RX state machine to be sure to not be in Waiting packet state */
  /* Otherwise the arrival of a new packet will trigger a NewPacketAvailable despite */
  /* the fact that bytes have been lost because of overrun (debugger paused for instance) */
  pHandle->PacketPending = false;
  pHandle->asyncBufferA.state = freeBuf;
  pHandle->asyncBufferB.state = freeBuf;
  
  pHandle->ASPEP_TL_State = WAITING_PACKET;
  pHandle->fASPEP_receive(pHandle->HWIp, pHandle->rxHeader , ASPEP_HEADER_SIZE );
}

/* ASPEP Callback */
/******************/
void ASPEP_RegisterTxComplete_cb(ASPEP_Handle_t *pHandle, ASPEP_send_complete_cb_t ASPEP_send_complete_cb)
{
  if(NULL != pHandle && NULL != ASPEP_send_complete_cb)
  {
    pHandle->ASPEP_send_complete_cb = ASPEP_send_complete_cb;
  }
}

void ASPEP_RegisterTransmitting_cb(ASPEP_Handle_t *pHandle, ASPEP_after_sending_cb_t ASPEP_after_sending_cb)
{
  if( NULL != pHandle && NULL != ASPEP_after_sending_cb)
  {
    pHandle->ASPEP_after_sending_cb = ASPEP_after_sending_cb;
  }
}

void ASPEP_RegisterNewPacket_cb(ASPEP_Handle_t *pHandle, ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb)
{
  if(NULL != pHandle && NULL != ASPEP_receive_new_packet_cb)
  {
    pHandle->ASPEP_receive_new_packet_cb = ASPEP_receive_new_packet_cb;
  }
}

void ASPEP_RegisterDelay_cb(ASPEP_Handle_t *pHandle, ASPEP_delay_cb_t ASPEP_delay_cb)
{
  if(NULL != pHandle && NULL != ASPEP_delay_cb)
  {
    pHandle->ASPEP_delay_cb = ASPEP_delay_cb;
  }
}
