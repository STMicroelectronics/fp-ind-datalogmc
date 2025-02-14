/**
  ******************************************************************************
  * @file    mcp.c
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
#include "string.h"
#include "mcp.h"
#include "register_interface.h"
#include "aspep.h"

#define MCP_MAX_ASYNC_BUFF_SIZE 2048

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
struct _MCP_Handle_t
{
  uint8_t *rxBuffer;
  uint8_t *txBuffer;
  uint16_t rxLength;
  uint16_t txLength;

  /* Async Telemetries Properties */
  uint16_t MaxBuffSize;
  uint8_t HFRate;
  uint8_t HFNum;
  uint8_t MFRate;
  uint8_t MFNum;
  uint8_t Mark;

  MCP_DeliverPacket fDeliverPacket;
  MCP_Connected fConnected;
  MCP_DeliverAsync fDeliverAsync;

  MCP_Cmd_Type_def LastCmdSent;

};

/* Private defines -----------------------------------------------------------*/

/**
  * MCP Async request structure declaration
  */
typedef struct
{
  uint16_t MaxBuffSize;
  uint8_t HFRate;
  uint8_t HFNum;
  uint8_t MFRate;
  uint8_t MFNum;
  uint8_t Mark;
} MCPA_Handle_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MCPA_Handle_t MCPA_handle;
MCP_Handle_t MCP_Handle;
/* Private function prototypes -----------------------------------------------*/
static void MCP_SetReg(uint8_t TypeID, uint8_t *dataValue, uint8_t *data, uint16_t *size);
static void MCP_SendPacket(MCP_Handle_t *pHandle, uint16_t command);
static void MCP_EventProcess(MCP_Handle_t *pSupHandle);
static void MCP_ReceivedPacket(MCP_Handle_t *pHandle);
/* Functions Definition ------------------------------------------------------*/

void MCP_Init(MCP_Handle_t **pHandle)
{
  *pHandle = &MCP_Handle;

  MCPA_handle.MaxBuffSize = MCP_MAX_ASYNC_BUFF_SIZE;
  MCPA_handle.HFRate = 0;
  MCPA_handle.HFNum = 0;
  MCPA_handle.MFRate = 0xff;
  MCPA_handle.MFNum = 0;
  MCPA_handle.Mark = 1;

  ASPEP_Init();
}

void MCP_ReceiveAndEventProcess(MCP_Handle_t *pSupHandle)
{
  pSupHandle->rxBuffer = ASPEP_RxFrameProcess(&pSupHandle->rxLength);
  MCP_EventProcess(pSupHandle);
}

void MCP_ConnectionRequest(void)
{
  ASPEP_StartConnection();
}

void MCP_DisconnectionRequest(MCP_Handle_t *pSupHandle)
{
  ASPEP_Disconnection();
  pSupHandle->fConnected(false);
}

bool MCP_StartMotorRequest(MCP_Handle_t *pSupHandle)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    MCP_SendPacket(pSupHandle, START_MOTOR);
  }
  else
  {
    result = false;
  }
  return result;
}

bool MCP_StopMotorRequest(MCP_Handle_t *pSupHandle)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    MCP_SendPacket(pSupHandle, STOP_MOTOR);
  }
  else
  {
    result = false;
  }
  return result;
}

bool MCP_SendAckFault(MCP_Handle_t *pSupHandle)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    MCP_SendPacket(pSupHandle, FAULT_ACK);
  }
  else
  {
    result = false;
  }
  return result;
}

bool MCP_ResetMotor(MCP_Handle_t *pSupHandle)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    MCP_SendPacket(pSupHandle, SW_RESET);
  }
  else
  {
    result = false;
  }
  return result;
}

bool MCP_SetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t *RegID, uint8_t *RegValue)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    uint8_t *txData = pSupHandle->txBuffer;
    txData += MCP_HEADER_SIZE;

    uint16_t i;
    uint8_t typeID;
    uint16_t dataID;
    uint16_t size = 0u;

    for (i = 0; i < NumberReg; i++)
    {

      dataID = (uint16_t) * RegID;
      RegID += 1; //shift to next Reg ID

      // Data ID
      *txData = (uint8_t) dataID;
      txData += 1;
      *txData = (uint8_t)(dataID >> 8);
      txData += 1;

      pSupHandle->txLength += MCP_ID_SIZE;

      typeID = dataID & TYPE_MASK;
      MCP_SetReg(typeID, RegValue, txData, &size);
      RegValue += size;
      pSupHandle->txLength += size;
      txData += size; //shift buffer
    }

    MCP_SendPacket(pSupHandle, SET_DATA_ELEMENT);
  }

  else
  {
    result = false;
  }
  return result;
}

static void MCP_ReceivedPacket(MCP_Handle_t *pHandle)
{
  uint8_t *rxData = (uint8_t *) pHandle->rxBuffer;
  uint16_t Length = (uint16_t) pHandle->rxLength;
  uint8_t MCPResponse = rxData[Length - 1];

  pHandle->txBuffer += MCP_HEADER_SIZE;
  pHandle->fDeliverPacket(pHandle->LastCmdSent, MCPResponse, rxData, &pHandle->rxLength);
}

static void MCP_SetReg(uint8_t TypeID, uint8_t *dataValue, uint8_t *data, uint16_t *size)
{
  switch (TypeID)
  {
    case TYPE_DATA_8BIT:
    {
      *data = *dataValue;
      *size += 1;
      break;
    }

    case TYPE_DATA_16BIT:
    {
      uint16_t *txData16 = (uint16_t *) data;
      *txData16 = *((uint16_t *) dataValue);
      *size = 2;
      break;
    }

    case TYPE_DATA_32BIT:
    {
      uint16_t *txData16 = (uint16_t *) data;
      *txData16 = ((uint32_t)(*(uint16_t *)&dataValue[2])) << 16 | *(uint16_t *)dataValue;
      *size = 4;
      break;
    }

    case TYPE_DATA_RAW:
    {
      *size = *((uint16_t *) dataValue);
      *size += 2;
      memcpy(data, dataValue, *size);
      break;
    }
  }
}

static void MCP_EventProcess(MCP_Handle_t *pSupHandle)
{
  if (ASPEP_GetStatus(EVENT_CONNECTED, false))
  {
    if (pSupHandle->fConnected)
    {
      pSupHandle->fConnected(true);
    }
    ASPEP_GetStatus(EVENT_CONNECTED, true); //clear flag
  }

  if (ASPEP_GetStatus(EVENT_RESPONSE, false))
  {
    MCP_ReceivedPacket(pSupHandle);
    ASPEP_GetStatus(EVENT_RESPONSE, true); //clear flag
  }

  if (ASPEP_GetStatus(EVENT_DISCONNECTED, false))
  {
    if (pSupHandle->fConnected)
    {
      pSupHandle->fConnected(false);
    }
    ASPEP_GetStatus(EVENT_DISCONNECTED, true);  //clear flag
  }

  if (ASPEP_GetStatus(EVENT_ASYNC, false))
  {
    if (pSupHandle->fDeliverAsync)
    {
      pSupHandle->fDeliverAsync(pSupHandle->rxBuffer, pSupHandle->rxLength);
    }

    ASPEP_GetStatus(EVENT_ASYNC, true);  //clear flag
    ASPEP_FreeBuffer();
  }
}

bool MCP_GetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t *RegID)
{
  bool result = true;
  if (ASPEP_getBuffer((void **) &pSupHandle->txBuffer))
  {
    uint16_t *txData = (uint16_t *) pSupHandle->txBuffer;
    txData += 1; //for header MCP
    uint16_t i;

    for (i = 0; i < NumberReg; i++)
    {
      *txData = (uint16_t) * RegID;
      RegID += 1;
      txData += 1; //shift buffer
      pSupHandle->txLength += MCP_ID_SIZE;
    }

    MCP_SendPacket(pSupHandle, GET_DATA_ELEMENT);
  }

  else
  {
    result = false;
  }
  return result;
}

/*add MCP Header to buffer*/
static void MCP_SendPacket(MCP_Handle_t *pHandle, uint16_t command)
{
  pHandle->LastCmdSent = (MCP_Cmd_Type_def) command;
  uint8_t motorID;
  if (pHandle->LastCmdSent == SetRampCmd)
  {
    motorID = 0;
  }
  else
  {
    motorID = 0x1;
  }
  //if set register : motor ID = 0
  uint16_t *txData = (uint16_t *) pHandle->txBuffer;

  /* Packet header*/
  *txData = motorID | command << 3;
  pHandle->txLength += MCP_HEADER_SIZE;

  ASPEP_SendPacket(pHandle->txBuffer, pHandle->txLength);

  pHandle->txLength = 0;
}

void MCP_SetAsyncs(MCP_Handle_t *p_MCP_Handle, uint16_t *RegisterID, uint8_t *RegisterValBuff, uint8_t HFNum)
{
  uint16_t size;
  uint8_t i = 0;
  MCPA_handle.MaxBuffSize = MCP_MAX_ASYNC_BUFF_SIZE - 2 * HFNum;
  MCPA_handle.HFNum = HFNum;
  uint8_t *RegValBuff = (uint8_t *) RegisterValBuff;

  uint16_t RegID[1];
  RegID[0] = (uint16_t) MC_REG_ASYNC_UARTA;

  size = 7 + 2 * HFNum; //payload size, 16 bits
  *RegValBuff = (uint8_t) size;
  RegValBuff += 1;
  *RegValBuff = (uint8_t)(size >> 8);
  RegValBuff += 1;

  *RegValBuff = (uint8_t) MCPA_handle.MaxBuffSize;
  RegValBuff += 1;
  *RegValBuff = (uint8_t)(MCPA_handle.MaxBuffSize >> 8);
  RegValBuff += 1;

  *RegValBuff = MCPA_handle.HFRate;
  RegValBuff += 1;

  *RegValBuff = HFNum;
  RegValBuff += 1;

  *RegValBuff = MCPA_handle.MFRate;
  RegValBuff += 1;

  *RegValBuff = MCPA_handle.MFNum;
  RegValBuff += 1;

  for (i = 0; i < HFNum; i++)
  {
    *RegValBuff = (uint8_t) * RegisterID;
    RegValBuff += 1;
    *RegValBuff = (uint8_t)(*RegisterID >> 8);
    RegValBuff += 1;
    RegisterID += 1;
  }

  if (1)
  {
    *RegValBuff = MCPA_handle.Mark;
    MCPA_handle.Mark ++;
  }
  else
  {
    *RegValBuff = 0;
  }

  MCP_SetRegister(p_MCP_Handle, 1, RegID, RegisterValBuff);
}

void MCP_DisableAsyncs(MCP_Handle_t *p_MCP_Handle, uint8_t *RegisterValBuff)
{
  uint8_t *RegValBuff = (uint8_t *) RegisterValBuff;

  uint16_t RegID[1];
  RegID[0] = (uint16_t) MC_REG_ASYNC_UARTA;

  *RegValBuff = 0;

  MCP_SetRegister(p_MCP_Handle, 1, RegID, RegisterValBuff);
}


void MCP_RegisterDeliveredSyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverPacket mcp_delivered_sync_packet_cb)
{
  pHandle->fDeliverPacket = mcp_delivered_sync_packet_cb;
}

void MCP_RegisterConnectCompletedCb(MCP_Handle_t *pHandle, MCP_Connected mcp_connect_completed_cb)
{
  if (NULL != pHandle && NULL != mcp_connect_completed_cb)
  {
    pHandle->fConnected = mcp_connect_completed_cb;
  }
}

void MCP_RegisterDeliveredAsyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverAsync mcp_delivered_async_packet_cb)
{
  if (NULL != pHandle && NULL != mcp_delivered_async_packet_cb)
  {
    pHandle->fDeliverAsync = mcp_delivered_async_packet_cb;
  }
}
