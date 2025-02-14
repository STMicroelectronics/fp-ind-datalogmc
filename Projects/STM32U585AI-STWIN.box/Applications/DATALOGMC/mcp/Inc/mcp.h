/**
  ******************************************************************************
  * @file    mcp.h
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
#ifndef motor_control_protocol
#define motor_control_protocol

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"


/* Exported types ------------------------------------------------------------*/
typedef struct _MCP_Handle_t MCP_Handle_t;
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Action supported by the Motor control protocol*/
#define MOTOR_MASK     0x7
#define CMD_MASK       0xFFF8


/*Commands ID*/
#define SET_DATA_ELEMENT 0x01
#define GET_DATA_ELEMENT 0x02
#define START_MOTOR      0x03
#define STOP_MOTOR       0x04
#define STOP_RAMP        0x05
#define START_STOP       0x06
#define FAULT_ACK        0x07
#define SW_RESET         0x0F

/* MCP ERROR CODE */
#define MCP_CMD_OK                  0x00
#define MCP_CMD_NOK                 0x01
#define MCP_CMD_UNKNOWN             0x02
#define MCP_DATAID_UNKNOWN          0x03
#define MCP_ERROR_RO_REG            0x04
#define MCP_ERROR_UNKNOWN_REG       0x05
#define MCP_ERROR_STRING_FORMAT     0x06
#define MCP_ERROR_BAD_DATA_TYPE     0x07
#define MCP_ERROR_NO_TXSYNC_SPACE   0x08
#define MCP_ERROR_NO_TXASYNC_SPACE  0x09
#define MCP_ERROR_BAD_RAW_FORMAT    0x0A
#define MCP_ERROR_UNKNOWN_MOTOR     0x0B
#define MCP_ERROR_UNSUPPORTED_MOTOR 0x0D
#define MCP_ERROR_INVALID_DATAID    0x0E

#define ERROR_MSG_MASK              0x1;

#define MCP_ID_SIZE                 2 /* Number of byte */
#define MCP_HEADER_SIZE             2 //octet

#define CONNECTED                   1
#define DISCONNECTED                3
#define RXRESPONSE                  2

#define EVENT_NONE         0
#define EVENT_CONNECTED    1
#define EVENT_RESPONSE     2
#define EVENT_DISCONNECTED 3
#define EVENT_ASYNC        4

typedef void (* MCP_DeliverPacket)(uint8_t CallbackID, uint8_t MCPResponse, uint8_t *rxData, uint16_t *rxLength);
typedef void (* MCP_Connected)(bool Connected);
typedef void (* MCP_DeliverAsync)(uint8_t *rxAsync, uint32_t rxLength);


typedef enum
{
  SetRampCmd = SET_DATA_ELEMENT,
  StartMotorCmd = START_MOTOR,
  StopMotorCmd = STOP_MOTOR,
  ReadRegCmd = GET_DATA_ELEMENT,
} MCP_Cmd_Type_def;


/* Public Function declaration */

void MCP_Init(MCP_Handle_t **pHandle);
void MCP_ReceiveAndEventProcess(MCP_Handle_t *pSupHandle);
bool MCP_GetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t *RegID);
bool MCP_SetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t *RegID, uint8_t *RegValue);
bool MCP_StopMotorRequest(MCP_Handle_t *pSupHandle);
bool MCP_SendAckFault(MCP_Handle_t *pSupHandle);
bool MCP_StartMotorRequest(MCP_Handle_t *pSupHandle);
void MCP_DisconnectionRequest(MCP_Handle_t *pSupHandle);
void MCP_ConnectionRequest(void);
bool MCP_ResetMotor(MCP_Handle_t *pSupHandle);

void MCP_SetAsyncs(MCP_Handle_t *p_MCP_Handle, uint16_t *RegisterID, uint8_t *RegisterValBuff, uint8_t HFNum);
void MCP_DisableAsyncs(MCP_Handle_t *p_MCP_Handle, uint8_t *RegisterValBuff);

/**
  * Added API to register application layer callback
  */
bool MCP_RegisterTransportLayer(MCP_Handle_t *pHandle, void *p_transport_layer);
void MCP_RegisterDeliveredSyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverPacket mcp_delivered_sync_packet_cb);
void MCP_RegisterConnectCompletedCb(MCP_Handle_t *pHandle, MCP_Connected mcp_connect_completed_cb);
void MCP_RegisterDeliveredAsyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverAsync mcp_delivered_async_packet_cb);

#ifdef __cplusplus
}
#endif

#endif /*motor_control_protocol*/
