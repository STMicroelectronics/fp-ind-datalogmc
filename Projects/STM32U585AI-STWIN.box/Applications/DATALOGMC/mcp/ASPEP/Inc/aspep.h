/**
  ******************************************************************************
  * @file    aspep.h
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

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef aspep_h
#define aspep_h

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "mcp.h"
#include "mcptl.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define ASPEP_HEADER_SIZE 4
#define ASPEP_DATACRC_SIZE 2

#define ID_MASK 0xF
#define DATA_PACKET 0x9
#define PING  0x6
#define BEACON 0x5
#define ERROR_PACKET 0x7
#define RESPONSE 0xA  //sync
#define ASYNC 0x9

#define ID_MASK_PING    0x10
#define CONFIGURED      0x10
#define UNCONFIGURED    0

#define ASPEP_OK              0
#define ASPEP_BAD_PACKET_TYPE 1
#define ASPEP_BAD_PACKET_SIZE 2
#define ASPEP_BAD_CRC_HEADER  4

#define TBEACON       350
#define TPING         300
#define TTRANSACTION  300
#define TSILENCE      200

#define EVENT_NONE         0
#define EVENT_CONNECTED    1
#define EVENT_RESPONSE     2
#define EVENT_DISCONNECTED 3
#define EVENT_ASYNC        4

typedef bool (* ASPEP_send_cb_t )    (void* pHW_Handle, void *txbuffer, uint16_t length);
typedef void (* ASPEP_receive_cb_t ) (void* pHW_Handle, void *rxbuffer, uint16_t length);
typedef void (* ASPEP_hwinit_cb_t )  (void* pHW_Handle);
typedef void (* ASPEP_hwsync_cb_t )  (void* pHW_Handle);

/* Application level callback */
typedef void (*ASPEP_send_complete_cb_t)(void);
typedef void (*ASPEP_after_sending_cb_t)(void);
typedef void (*ASPEP_receive_new_packet_cb_t)(void);
typedef void (*ASPEP_delay_cb_t)(uint8_t delay);

typedef enum {
  beacon = BEACON,
  ping = PING,
  data = DATA_PACKET,
  response = RESPONSE,
  error = ERROR_PACKET,
  async = ASYNC
} ASPEP_packetType_def;


typedef enum {
  configured = CONFIGURED,
  unconfigured = UNCONFIGURED,
} ASPEP_pingType_def;

typedef struct {
  uint8_t DATA_CRC;
  uint8_t RX_maxSize;
  uint8_t TXS_maxSize;
  uint8_t TXA_maxSize;
  uint8_t version;
} ASPEP_Capabilities_def;

typedef enum {
  ASPEP_IDLE,
  ASPEP_CONFIGURING,
  ASPEP_CONNECTING,
  ASPEP_CONNECTED,
  ASPEP_TRANSACTION,
  ASPEP_PING,
  ASPEP_RECOVERY,
} ASPEP_sm_type;

/* ASPEP Transport Layer Sate Machine Type*/
typedef enum {
  WAITING_PACKET,
  WAITING_PAYLOAD,
} ASPEP_TL_sm_type;

typedef enum {
  RECOVERY_BEACON,
  RECOVERY_SILENCE,
} ASPEP_RECOVERY_sm_type;

typedef struct {
  bool SM_Event_Connected;
  bool SM_Event_Response;
  bool SM_Event_Ping;
  bool SM_Event_Async;
} ASPEP_Event_StateMachine;

typedef enum {
  Tbeacon = TBEACON,
  Tping = TPING,
  Ttransaction = TTRANSACTION,
  Tsilence = TSILENCE,
} ASPEP_Timeout;

typedef struct {
  MCTL_Handle_t _Super;
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
  
  ASPEP_hwinit_cb_t fASPEP_HWInit;
  ASPEP_hwsync_cb_t fASPEP_HWSync;
  ASPEP_receive_cb_t fASPEP_receive;
  ASPEP_send_cb_t fASPEP_send;
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
  
  /* Callback */
  ASPEP_send_complete_cb_t ASPEP_send_complete_cb;
  ASPEP_after_sending_cb_t ASPEP_after_sending_cb;
  ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb;
  ASPEP_delay_cb_t ASPEP_delay_cb;
} ASPEP_Handle_t;

/* Public function declaration */
void ASPEP_StartConnection(MCTL_Handle_t *pSupHandle);
void ASPEP_Disconnection(MCTL_Handle_t *pSupHandle);
void ASPEP_ReceivedIT (ASPEP_Handle_t *pHandle);
void ASPEP_Init(ASPEP_Handle_t *pHandle);
void ASPEP_TimeoutHandle (ASPEP_Handle_t *pHandle);
uint8_t* ASPEP_RxFrameProcess (MCTL_Handle_t *pSupHandle, uint16_t *packetLength);
void ASPEP_TXframeProcess (ASPEP_Handle_t *pHandle, void *txBuffer, uint16_t bufferLength);
void ASPEP_TXframeProcessNoCrc (ASPEP_Handle_t *pHandle, void *txBuffer, uint16_t bufferLength);
void ASPEP_SendPacket (MCTL_Handle_t *pSupHandle, void *txBuffer, uint16_t txDataLength);
bool ASPEP_getBuffer (MCTL_Handle_t *pSupHandle, void **buffer);
void ASPEP_DataTransmittedIT(ASPEP_Handle_t *pHandle);
uint8_t ASPEP_GetStatus(MCTL_Handle_t *pSupHandle, uint8_t Flag, bool ClearFlag);
void ASPEP_HWDMAReset (ASPEP_Handle_t *pHandle);
void ASPEP_Recovery(ASPEP_Handle_t *pHandle);
void ASPEP_FreeBuffer(MCTL_Handle_t *pSupHandle);
void ASPEP_RegisterTxComplete_cb(ASPEP_Handle_t *pHandle, ASPEP_send_complete_cb_t ASPEP_send_complete_cb);
void ASPEP_RegisterTransmitting_cb(ASPEP_Handle_t *pHandle, ASPEP_after_sending_cb_t ASPEP_after_sending_cb);
void ASPEP_RegisterNewPacket_cb(ASPEP_Handle_t *pHandle, ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb);
void ASPEP_RegisterDelay_cb(ASPEP_Handle_t *pHandle, ASPEP_delay_cb_t ASPEP_delay_cb);

#ifdef __cplusplus
}
#endif

#endif /*aspep_h*/
