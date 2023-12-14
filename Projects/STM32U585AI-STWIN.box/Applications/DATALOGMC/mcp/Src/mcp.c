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
#include "stdint.h"
#include "string.h"
#include "mcp.h"
#include "register_interface.h"

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
void MCP_EventProcess(MCP_Handle_t *pSupHandle)
{ 
   if (pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_CONNECTED, false))
   {
     if(pSupHandle->fConnected)
     {
       pSupHandle->fConnected(true);
     }
     pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_CONNECTED, true); //clear flag
   }
   
   if (pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_RESPONSE, false))
   { 
     MCP_ReceivedPacket(pSupHandle);
     pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_RESPONSE, true); //clear flag
   }
   
   if (pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_DISCONNECTED, false))
   {
    if(pSupHandle->fConnected)
    {
      pSupHandle->fConnected(false);
    }
    pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_DISCONNECTED, true); //clear flag
   } 
   
   if (pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_ASYNC, false))
   {
     if(pSupHandle->fDeliverAsync)
     {
       pSupHandle->fDeliverAsync(pSupHandle->rxBuffer, pSupHandle->rxLength);
     }

     pSupHandle->pTransportLayer->fGetStatus(pSupHandle->pTransportLayer, EVENT_ASYNC, true); //clear flag
     pSupHandle->pTransportLayer->fFreeBuffer(pSupHandle->pTransportLayer);
   }
}

void MCP_ConnectionRequest(MCP_Handle_t *pSupHandle)
{
   pSupHandle->pTransportLayer->fStartConnection(pSupHandle->pTransportLayer);
}

void MCP_DisconnectionRequest(MCP_Handle_t *pSupHandle)
{
   pSupHandle->pTransportLayer->fDisconnection(pSupHandle->pTransportLayer);
   pSupHandle->fConnected(false);
}

bool MCP_StartMotorRequest(MCP_Handle_t *pSupHandle)
{
    bool result = true;
    if (pSupHandle->pTransportLayer->fGetBuffer (pSupHandle->pTransportLayer, (void **) &pSupHandle->txBuffer))
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
    if (pSupHandle->pTransportLayer->fGetBuffer (pSupHandle->pTransportLayer, (void **) &pSupHandle->txBuffer))
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
    if (pSupHandle->pTransportLayer->fGetBuffer (pSupHandle->pTransportLayer, (void **) &pSupHandle->txBuffer))
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
  if(pSupHandle->pTransportLayer->fGetBuffer(pSupHandle->pTransportLayer, (void**) &pSupHandle->txBuffer))
  {
    MCP_SendPacket(pSupHandle, SW_RESET);
  }
  else
  {
    result = false;
  }
  return result;
}

bool MCP_SetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t * RegID, uint8_t * RegValue)
{
    bool result = true;
    if (pSupHandle->pTransportLayer->fGetBuffer(pSupHandle->pTransportLayer, (void **) &pSupHandle->txBuffer))
    {
      uint8_t * txData = pSupHandle->txBuffer;
      txData += MCP_HEADER_SIZE;
      
      uint16_t i; 
      uint8_t typeID;
      uint16_t dataID;
      uint16_t size = 0u;
      
      for (i = 0; i < NumberReg; i++) { 
 
        dataID = (uint16_t) *RegID;
        RegID += 1; //shift to next Reg ID
        
        // Data ID 
        *txData = (uint8_t) dataID;
        txData += 1;
        *txData = (uint8_t) (dataID>>8);
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

void MCP_SetReg(uint8_t TypeID, uint8_t * dataValue, uint8_t * data, uint16_t * size)
{
    switch(TypeID)
    {
    case TYPE_DATA_8BIT: {
        *data = *dataValue;
        *size += 1;
        break; }
           
    case TYPE_DATA_16BIT: {
         uint16_t * txData16 = (uint16_t *) data;
         *txData16 = *((uint16_t *) dataValue);
         *size = 2;
         break; }
             
    case TYPE_DATA_32BIT: {
         uint16_t * txData16 = (uint16_t *) data;
         *txData16 = ((uint32_t)(*(uint16_t *)&dataValue[2]))<<16 | *(uint16_t *)dataValue;
         *size = 4;
         break; }
         
     case TYPE_DATA_RAW: {
          *size = *((uint16_t *) dataValue);
          *size += 2;
          memcpy(data, dataValue, *size);
         break; } 
     }
}

bool MCP_GetRegister(MCP_Handle_t *pSupHandle, uint16_t NumberReg, uint16_t * RegID)
{
    bool result = true;
    if (pSupHandle->pTransportLayer->fGetBuffer(pSupHandle->pTransportLayer, (void **) &pSupHandle->txBuffer))
    {
        uint16_t * txData = (uint16_t *) pSupHandle->txBuffer;
        txData += 1; //for header MCP
        uint16_t i;
        
       for (i = 0; i < NumberReg; i++) {
          *txData = (uint16_t) *RegID;
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
void MCP_SendPacket(MCP_Handle_t * pHandle, uint16_t command)
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
    uint16_t * txData = (uint16_t *) pHandle->txBuffer;
    
    /* Packet header*/
    *txData = motorID | command <<3;
    pHandle->txLength += MCP_HEADER_SIZE;

    pHandle->pTransportLayer->fSendPacket (pHandle->pTransportLayer, pHandle->txBuffer, pHandle->txLength);
    pHandle->txLength = 0;
}

void MCP_ReceivedPacket(MCP_Handle_t * pHandle)
{
  uint8_t * rxData = (uint8_t *) pHandle->rxBuffer;
  uint16_t Length = (uint16_t) pHandle->rxLength;
  uint8_t MCPResponse = rxData[Length-1];
  
  pHandle->txBuffer += MCP_HEADER_SIZE;
  pHandle->fDeliverPacket (pHandle->LastCmdSent, MCPResponse, rxData, &pHandle->rxLength);
} 

void MCP_RegisterDeliveredSyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverPacket mcp_delivered_sync_packet_cb)
{
  pHandle->fDeliverPacket = mcp_delivered_sync_packet_cb;
}

void MCP_RegisterConnectCompletedCb(MCP_Handle_t *pHandle, MCP_Connected mcp_connect_completed_cb)
{
  if(NULL != pHandle && NULL != mcp_connect_completed_cb)
  {
    pHandle->fConnected = mcp_connect_completed_cb;
  }
}

void MCP_RegisterDeliveredAsyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverAsync mcp_delivered_async_packet_cb)
{
  if(NULL != pHandle && NULL != mcp_delivered_async_packet_cb)
  {
    pHandle->fDeliverAsync = mcp_delivered_async_packet_cb;
  }
}

bool MCP_RegisterTransportLayer(MCP_Handle_t *pHandle, void *p_transport_layer)
{
  bool retval = false;

  if(NULL != pHandle && NULL != p_transport_layer)
  {
    pHandle->pTransportLayer = (MCTL_Handle_t*)p_transport_layer;

    retval = true;
  }

  return retval;
}
