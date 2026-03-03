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

/**
  * @brief  Return values for MCP interface functions.
  */
typedef enum
{
  MCP_SUCCESS,                /**< Operation completed successfully. */
  MCP_NULL_POINTER_ERROR,     /**< A required pointer parameter was NULL. */
  MCP_TRANSPORT_LAYER_ERROR,  /**< Error in the transport layer (e.g., buffer acquisition failed). */
  MCP_ERROR                   /**< General error. */
} MCP_RetVal_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

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

/**
  * @brief Function pointer type for delivering a synchronous MCP packet.
  *
  * This callback is used to handle received synchronous MCP packets.
  *
  * @param[in] MCPCmd      MCP command code (e.g., SET_DATA_ELEMENT, GET_DATA_ELEMENT).
  * @param[in] MCPResponse MCP response code (e.g., MCP_CMD_OK, MCP_CMD_NOK).
  * @param[in] pRxData     Pointer to the received data buffer.
  * @param[in] pRxLength   Pointer to the length of the received data.
  */
typedef void (* MCP_DeliverSyncPacket)(uint8_t MCPCmd, uint8_t MCPResponse, uint8_t *pRxData, uint16_t *pRxLength);

/**
  * @brief  Function pointer type for delivering an asynchronous MCP packet.
  *
  * @param[in] pRxAsync   Pointer to the received asynchronous data buffer.
  * @param[in] pRxLength  Length of the received asynchronous data.
  */
typedef void (*MCP_DeliverAsync)(uint8_t *pRxAsync, uint32_t pRxLength);


typedef void (* MCP_Connected)(bool Connected);

/* Public Function declaration */

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
MCP_RetVal_t MCP_Init(MCP_Handle_t **pHandle);

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
MCP_RetVal_t MCP_GetRegister(MCP_Handle_t *pHandle, uint16_t NumberReg, uint16_t *RegID);

/**
  * @brief Processes received MCP frames and handles related events.
  *
  * This function processes the latest received frame from the ASPEP transport layer
  * and then handles any resulting MCP events, such as connection status changes,
  * response packets, or asynchronous events.
  *
  * @param[in] pSupHandle Pointer to the MCP handle structure.
  */
void MCP_ReceiveAndEventProcess(MCP_Handle_t *pSupHandle);

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
MCP_RetVal_t MCP_SetRegister(MCP_Handle_t *pHandle, uint16_t NumberReg, uint16_t *RegID, uint8_t *RegValue);

/**
  * @brief  Sends a request to start the motor via the MCP interface.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure.
  *
  * @retval MCP_NULL_POINTER_ERROR      If pHandle is NULL.
  * @retval MCP_TRANSPORT_LAYER_ERROR   If the transmission buffer could not be acquired.
  * @retval MCP_SUCCESS                 If the start motor request was successfully sent.
  */
MCP_RetVal_t MCP_StartMotorRequest(MCP_Handle_t *pHandle);

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
MCP_RetVal_t MCP_StopMotorRequest(MCP_Handle_t *pHandle);

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
MCP_RetVal_t MCP_SendAckFault(MCP_Handle_t *pHandle);

/**
  * @brief  Initiates a disconnection request for the MCP.
  *
  * This function triggers the ASPEP control layer to disconnect the current connection.
  *
  * @param[in] pHandle  Pointer to an MCP_Handle_t structure
  */
void MCP_DisconnectionRequest(MCP_Handle_t *pSupHandle);

/**
  * @brief  Initiates a connection request via the ASPEP control layer.
  *
  * This function triggers the start of a connection process by calling
  * the ASPEP_CTRL_StartConnection function.
  */
void MCP_ConnectionRequest(void);

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
MCP_RetVal_t MCP_ResetMotor(MCP_Handle_t *pHandle);

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
MCP_RetVal_t MCP_EnableAsyncTelemetries(MCP_Handle_t *pHandle, uint16_t *pRegisterIDBuff, uint8_t numOfRegisterID);

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
MCP_RetVal_t MCP_DisableAsyncTelemetries(MCP_Handle_t *pHandle);

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
                                               MCP_DeliverSyncPacket mcp_delivered_sync_packet_cb);

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
MCP_RetVal_t MCP_RegisterDeliveredAsyncPacketCb(MCP_Handle_t *pHandle, MCP_DeliverAsync mcp_delivered_async_packet_cb);

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
MCP_RetVal_t MCP_RegisterConnectCompletedCb(MCP_Handle_t *pHandle, MCP_Connected mcp_connect_completed_cb);


#ifdef __cplusplus
}
#endif

#endif /*motor_control_protocol*/
