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
#include "aspep_ctrl.h"
#include "usart_aspep_driver.h"

/* Private defines -----------------------------------------------------------*/
#define MCP_ID_SIZE                 2 /* Number of byte */
#define MCP_HEADER_SIZE             2 //octet

#define TX_ENABLE_ASYNC_MAX_REGISTER      5U
#define TX_ENABLE_ASYNC_HEADER_BYTE_DIM   8U
#define TX_ENABLE_ASYNC_BUFF_DIM ((sizeof(uint16_t)*TX_ENABLE_ASYNC_MAX_REGISTER)+TX_ENABLE_ASYNC_HEADER_BYTE_DIM)

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief MCP command type enumeration.
  *
  * Enumerates the supported MCP command types, mapping each to its corresponding command ID.
  */
typedef enum
{
  SetRampCmd    = SET_DATA_ELEMENT,  /**< Set ramp command. */
  StartMotorCmd = START_MOTOR,       /**< Start motor command. */
  StopMotorCmd  = STOP_MOTOR,        /**< Stop motor command. */
  ReadRegCmd    = GET_DATA_ELEMENT,  /**< Read register command. */
} MCP_Cmd_Type_def;

/**
  * @brief MCP asynchronous request structure.
  *
  * Structure used to manage asynchronous MCP requests and related buffer configurations.
  */
typedef struct
{
  uint8_t confBuff[TX_ENABLE_ASYNC_BUFF_DIM]; /**< Configuration buffer for async requests. */
  uint16_t MaxBuffSize;                       /**< Maximum buffer size. */
  uint8_t HFRate;                             /**< High-frequency rate. */
  uint8_t HFNum;                              /**< High-frequency number. */
  uint8_t MFRate;                             /**< Medium-frequency rate. */
  uint8_t MFNum;                              /**< Medium-frequency number. */
  uint8_t Mark;                               /**< Mark or flag for internal use. */
} MCPA_Handle_t;

/**
  * @brief MCP handle structure.
  *
  * Structure containing all relevant data and callback pointers for MCP communication and event handling.
  */
struct _MCP_Handle_t
{
  uint8_t *rxBuffer;                        /**< Pointer to the receive buffer. */
  uint8_t *txBuffer;                        /**< Pointer to the transmit buffer. */
  uint16_t rxLength;                        /**< Length of received data. */
  uint16_t txLength;                        /**< Length of data to transmit. */
  uint16_t MaxBuffSize;                     /**< Maximum buffer size. */
  MCPA_Handle_t asyncHandle;             /**< Handle for asynchronous MCP requests. */
  MCP_DeliverSyncPacket fDeliverPacket;     /**< Callback for delivering synchronous packets. */
  MCP_Connected fConnected;                 /**< Callback for connection status changes. */
  MCP_DeliverAsync fDeliverAsync;           /**< Callback for delivering asynchronous packets. */
  MCP_Cmd_Type_def LastCmdSent;             /**< Last command sent. */
};

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
MCP_Handle_t MCP_Handle;

/* Private function prototypes -----------------------------------------------*/
static void MCP_SetReg(uint8_t TypeID, uint8_t *dataValue, uint8_t *data, uint16_t *size);
static void MCP_SendPacket(MCP_Handle_t *pHandle, uint16_t command);
static void MCP_EventProcess(MCP_Handle_t *pSupHandle);
static void MCP_ReceivedPacket(MCP_Handle_t *pHandle);

/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Initializes the MCP handle and configures the ASPEP transport layer.
  *
  * @param[out] pHandle  Double pointer to an MCP_Handle_t structure. After initialization,
  *                      *pHandle will point to the global MCP_Handle.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If ASPEP transport layer initialization fails.
  * @retval MCP_SUCCESS                 On successful initialization.
  *
  */
MCP_RetVal_t MCP_Init(MCP_Handle_t **pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    *pHandle = &MCP_Handle;

    MCP_Handle.asyncHandle.MaxBuffSize = ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX;
    MCP_Handle.asyncHandle.HFRate = 0;
    MCP_Handle.asyncHandle.HFNum = 0;
    MCP_Handle.asyncHandle.MFRate = 0xff;
    MCP_Handle.asyncHandle.MFNum = 0;
    MCP_Handle.asyncHandle.Mark = 1;

    ASPEP_Transport_Layer_def_t ASPEP_Transport_Layer;
    ASPEP_Transport_Layer.ASPEP_hwinit_cb = UASPEP_INIT;
    ASPEP_Transport_Layer.ASPEP_config_reception_cb = UASPEP_RECEIVE_BUFFER;
    ASPEP_Transport_Layer.ASPEP_config_transmission_cb = UASPEP_SEND_PACKET;
    ASPEP_Transport_Layer.ASPEP_hwsync_cb = UASPEP_IDLE_ENABLE;

    if (ASPEP_SUCCESS != ASPEP_CTRL_Init(&ASPEP_Transport_Layer))
    {
      retVal = MCP_TRANSPORT_LAYER_ERROR;
    }
  }

  return retVal;
}

/**
  * @brief  Initiates a connection request via the ASPEP control layer.
  *
  * This function triggers the start of a connection process by calling
  * the ASPEP_CTRL_StartConnection function.
  */
void MCP_ConnectionRequest(void)
{
  ASPEP_CTRL_StartConnection();
}

/**
  * @brief  Initiates a disconnection request for the MCP.
  *
  * This function triggers the ASPEP control layer to disconnect the current connection.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure
  */
void MCP_DisconnectionRequest(MCP_Handle_t *pHandle)
{
  ASPEP_CTRL_Disconnection();
  pHandle->fConnected(false);
}

/**
  * @brief Processes received MCP frames and handles related events.
  *
  * This function processes the latest received frame from the ASPEP transport layer
  * and then handles any resulting MCP events, such as connection status changes,
  * response packets, or asynchronous events.
  *
  * @param[in] pSupHandle Pointer to the MCP handle structure.
  */
void MCP_ReceiveAndEventProcess(MCP_Handle_t *pSupHandle)
{
  ASPEP_CTRL_RxFrameProcess(&pSupHandle->rxBuffer, &pSupHandle->rxLength);
  MCP_EventProcess(pSupHandle);
}

/**
  * @brief  Sends a request to start the motor via the MCP interface.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the start motor request was successfully sent.
  */

MCP_RetVal_t MCP_StartMotorRequest(MCP_Handle_t *pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      MCP_SendPacket(pHandle, START_MOTOR);
      retVal = MCP_SUCCESS;
    }
  }

  return retVal;
}

/**
  * @brief  Sends a request to stop the motor via the MCP interface.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the stop motor request was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_StopMotorRequest(MCP_Handle_t *pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      MCP_SendPacket(pHandle, STOP_MOTOR);
      retVal = MCP_SUCCESS;
    }
  }

  return retVal;
}

/**
  * @brief  Sends a fault acknowledgment packet via the MCP interface.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the fault acknowledgment was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_SendAckFault(MCP_Handle_t *pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      MCP_SendPacket(pHandle, FAULT_ACK);
      retVal = MCP_SUCCESS;
    }
  }
  return retVal;
}

/**
  * @brief  Sends a software reset request to the motor via the MCP interface.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the reset request was successfully sent.
  *
  */
MCP_RetVal_t MCP_ResetMotor(MCP_Handle_t *pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      MCP_SendPacket(pHandle, SW_RESET);
      retVal = MCP_SUCCESS;
    }
  }

  return retVal;
}

/**
  * @brief  Writes values to one or more registers via the MCP interface.
  *
  * @param[in] pHandle    Pointer to an MCP_Handle_t structure.
  * @param[in] NumberReg  Number of registers to write.
  * @param[in] RegID      Pointer to an array of register IDs to be written.
  * @param[in] RegValue   Pointer to an array of values to write to the registers.
  *
  * @retval MCP_NULL_POINTER_ERROR      If any input pointer is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the register write request was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_SetRegister(MCP_Handle_t *pHandle, uint16_t NumberReg, uint16_t *pRegID, uint8_t *pRegValue)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle &&
      NULL != pRegID  &&
      NULL != pRegValue)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      uint8_t *txData = pHandle->txBuffer;
      txData += MCP_HEADER_SIZE;

      uint8_t typeID;
      uint16_t dataID;
      uint16_t size = 0u;

      for (uint16_t i = 0; i < NumberReg; i++)
      {

        dataID = (uint16_t) * pRegID;
        pRegID += 1;

        *txData = (uint8_t) dataID;
        txData += 1;
        *txData = (uint8_t)(dataID >> 8);
        txData += 1;

        pHandle->txLength += MCP_ID_SIZE;
        typeID = dataID & TYPE_MASK;

        MCP_SetReg(typeID, pRegValue, txData, &size);

        pRegValue += size;
        pHandle->txLength += size;
        txData += size;
      }

      MCP_SendPacket(pHandle, SET_DATA_ELEMENT);

      retVal = MCP_SUCCESS;
    }
  }

  return retVal;
}

static void MCP_ReceivedPacket(MCP_Handle_t *pHandle)
{
  uint8_t *rxData = (uint8_t *) pHandle->rxBuffer;
  uint16_t Length = (uint16_t) pHandle->rxLength;
  uint8_t MCPResponse = rxData[Length - 1];

  pHandle->txBuffer += MCP_HEADER_SIZE;

  if (NULL != pHandle->fDeliverPacket)
  {
    pHandle->fDeliverPacket(pHandle->LastCmdSent, MCPResponse, rxData, &pHandle->rxLength);
  }
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

/**
  * @brief Handles MCP events based on the latest ASPEP event.
  *
  * This function processes the current ASPEP event and triggers the appropriate
  * MCP callbacks or actions, such as connection status changes, response packet
  * handling, or asynchronous packet delivery.
  *
  * @param[in] pSupHandle Pointer to the MCP handle structure.
  */
static void MCP_EventProcess(MCP_Handle_t *pSupHandle)
{
  ASPEP_CTRL_Event_t aspep_evt = ASPEP_CTRL_GetEvent(true);

  switch (aspep_evt)
  {
    case ASPEP_CTRL_CONNECTED_EVT:
    {
      if (pSupHandle->fConnected)
      {
        pSupHandle->fConnected(true);
      }
    }
    break;

    case ASPEP_CTRL_DISCONNECT_EVT:
    {
      if (pSupHandle->fConnected)
      {
        pSupHandle->fConnected(false);
      }
    }
    break;

    case ASPEP_CTRL_RESPONSE_EVT:
    {
      MCP_ReceivedPacket(pSupHandle);
    }
    break;

    case ASPEP_CTRL_ASYNC_EVT:
    {
      if (pSupHandle->fDeliverAsync)
      {
        pSupHandle->fDeliverAsync(pSupHandle->rxBuffer, pSupHandle->rxLength);
      }
      ASPEP_CTRL_FreeAsyncBuffer();

    }
    break;

    case ASPEP_CTRL_NONE_EVT:
    default:
      break;
  }
}

/**
  * @brief  Requests the value of one or more registers via the MCP interface.
  *
  * @param[in]  pHandle    Pointer to an MCP_Handle_t structure.
  * @param[in]  NumberReg  Number of registers to read.
  * @param[in]  RegID      Pointer to an array of register IDs to be read.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle or RegID is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the register request was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_GetRegister(MCP_Handle_t *pHandle, uint16_t NumberReg, uint16_t *RegID)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle && NULL != RegID)
  {
    retVal = MCP_TRANSPORT_LAYER_ERROR;

    if (ASPEP_SUCCESS == ASPEP_CTRL_GetTxBuffer((void **) &pHandle->txBuffer))
    {
      uint16_t *txData = (uint16_t *) pHandle->txBuffer;
      txData += 1;

      uint16_t i;
      for (i = 0; i < NumberReg; i++)
      {
        *txData = (uint16_t) * RegID;
        RegID += 1;
        txData += 1;
        pHandle->txLength += MCP_ID_SIZE;
      }

      MCP_SendPacket(pHandle, GET_DATA_ELEMENT);

      retVal = MCP_SUCCESS;
    }
  }

  return retVal;
}


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

  ASPEP_CTRL_SendPacket(pHandle->txBuffer, pHandle->txLength);

  pHandle->txLength = 0;
}

/**
  * @brief  Enables asynchronous telemetry transmission for specified registers.
  *
  * @param[in]  pHandle           Pointer to the MCP handle structure.
  * @param[in]  pRegisterIDBuff   Pointer to a buffer containing the list of register IDs to enable.
  * @param[in]  numOfRegisterID   Number of register IDs in the buffer.
  *
  * @retval MCP_NULL_POINTER_ERROR  If pHandle or pRegisterIDBuff is NULL.
  * @retval MCP_ERROR               If numOfRegisterID exceeds the allowed maximum.
  * @retval MCP_SUCCESS             If the configuration was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_EnableAsyncTelemetries(MCP_Handle_t *pHandle, uint16_t *pRegisterIDBuff, uint8_t numOfRegisterID)
{
  uint16_t size;
  uint8_t *RegValBuff = &(pHandle->asyncHandle.confBuff[0]);
  uint16_t RegID = (uint16_t) MC_REG_ASYNC_UARTA;
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle &&
      NULL != pRegisterIDBuff)
  {
    retVal = MCP_ERROR;

    if (numOfRegisterID <= TX_ENABLE_ASYNC_MAX_REGISTER)
    {

      pHandle->asyncHandle.MaxBuffSize = ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX - sizeof(uint16_t) * numOfRegisterID;
      pHandle->asyncHandle.HFNum = numOfRegisterID;
      size = TX_ENABLE_ASYNC_HEADER_BYTE_DIM + (sizeof(uint16_t) * numOfRegisterID);

      /* Copy size into TX buffer */
      *RegValBuff = (uint8_t) size;
      RegValBuff += 1;
      *RegValBuff = (uint8_t)(size >> 8);
      RegValBuff += 1;

      /* Copy MaxBuffSize into TX buffer */
      *RegValBuff = (uint8_t) pHandle->asyncHandle.MaxBuffSize;
      RegValBuff += 1;
      *RegValBuff = (uint8_t)(pHandle->asyncHandle.MaxBuffSize >> 8);
      RegValBuff += 1;

      /* Copy HFRate into TX buffer */
      *RegValBuff = pHandle->asyncHandle.HFRate;
      RegValBuff += 1;

      /* Copy numOfRegisterID into TX buffer */
      *RegValBuff = numOfRegisterID;
      RegValBuff += 1;

      /* Copy MFRate into TX buffer */
      *RegValBuff = pHandle->asyncHandle.MFRate;
      RegValBuff += 1;

      /* Copy MFNum into TX buffer */
      *RegValBuff = pHandle->asyncHandle.MFNum;
      RegValBuff += 1;

      /* Copy registerID into TX buffer */
      uint8_t registerIDByteDim = sizeof(uint16_t) * numOfRegisterID;
      memcpy(RegValBuff, pRegisterIDBuff, sizeof(uint16_t) * numOfRegisterID);
      RegValBuff += registerIDByteDim;

      /* Copy Mark into TX buffer */
      *RegValBuff = pHandle->asyncHandle.Mark;
      pHandle->asyncHandle.Mark++;

      retVal = MCP_SetRegister(pHandle, 1, &RegID, pHandle->asyncHandle.confBuff);
    }
  }

  return retVal;
}

/**
  * @brief  Disables asynchronous telemetry transmission.
  *
  * @param[in] pHandle  Pointer to the MCP handle structure.
  *
  * @retval MCP_NULL_POINTER_ERROR  If pHandle is NULL.
  * @retval MCP_SUCCESS             If the disable request was successfully sent.
  *
  * @note  The MCP handle must be initialized before calling this function.
  */
MCP_RetVal_t MCP_DisableAsyncTelemetries(MCP_Handle_t *pHandle)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle)
  {
    uint8_t *RegValBuff = &pHandle->asyncHandle.confBuff[0];

    uint16_t RegID = MC_REG_ASYNC_UARTA;
    *RegValBuff = 0;

    retVal = MCP_SetRegister(pHandle, 1, &RegID, pHandle->asyncHandle.confBuff);
  }

  return retVal;
}

/**
  * @brief Registers a callback for delivering synchronous MCP packets.
  *
  * Associates a user-defined callback function with the MCP handle to process
  * received synchronous MCP packets.
  *
  * @param[in]  pHandle                      Pointer to the MCP handle structure.
  * @param[in]  mcp_delivered_sync_packet_cb Callback function to deliver synchronous MCP packets.
  *
  * @retval     MCP_SUCCESS                  Callback registered successfully.
  * @retval     MCP_NULL_POINTER_ERROR       Null pointer provided for handle or callback.
  */
MCP_RetVal_t MCP_RegisterDeliveredSyncPacketCb(MCP_Handle_t *pHandle,
                                               MCP_DeliverSyncPacket mcp_delivered_sync_packet_cb)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle && NULL != mcp_delivered_sync_packet_cb)
  {
    pHandle->fDeliverPacket = mcp_delivered_sync_packet_cb;

    retVal = MCP_SUCCESS;
  }

  return retVal;
}

/**
  * @brief Registers a callback for delivering asynchronous MCP packets.
  *
  * Associates a user-defined callback function with the MCP handle to process
  * received asynchronous MCP packets.
  *
  * @param[in] pHandle                        Pointer to the MCP handle structure.
  * @param[in] mcp_delivered_async_packet_cb  Callback function to deliver asynchronous MCP packets.
  *
  * @retval    MCP_SUCCESS                    Callback registered successfully.
  * @retval    MCP_NULL_POINTER_ERROR         Null pointer provided for handle or callback.
  */
MCP_RetVal_t MCP_RegisterDeliveredAsyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverAsync mcp_delivered_async_packet_cb)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle && NULL != mcp_delivered_async_packet_cb)
  {
    pHandle->fDeliverAsync = mcp_delivered_async_packet_cb;

    retVal = MCP_SUCCESS;
  }

  return retVal;
}

/**
  * @brief Registers a callback for MCP connection status changes.
  *
  * Associates a user-defined callback function with the MCP handle to be notified
  * when the connection status changes.
  *
  * @param[in] pHandle                  Pointer to the MCP handle structure.
  * @param[in] mcp_connect_completed_cb  Callback function to handle connection status changes.
  *
  * @retval    MCP_SUCCESS              Callback registered successfully.
  * @retval    MCP_NULL_POINTER_ERROR   Null pointer provided for handle or callback.
  */
MCP_RetVal_t MCP_RegisterConnectCompletedCb(MCP_Handle_t *pHandle, MCP_Connected mcp_connect_completed_cb)
{
  MCP_RetVal_t retVal = MCP_NULL_POINTER_ERROR;

  if (NULL != pHandle && NULL != mcp_connect_completed_cb)
  {
    pHandle->fConnected = mcp_connect_completed_cb;

    retVal = MCP_SUCCESS;
  }

  return retVal;
}

