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

#include "aspep_ctrl_conf.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief Callback type for configuring transmission.
  *
  * @param pASPEP_Handle Pointer to the ASPEP handle.
  * @param txbuffer Pointer to the transmission buffer.
  * @param length Length of the transmission buffer.
  */
typedef void (*ASPEP_config_transmission_cb_t)(void *pASPEP_Handle, void *txbuffer, uint16_t length);

/**
  * @brief Callback type for configuring reception.
  *
  * @param pASPEP_Handle Pointer to the ASPEP handle.
  * @param rxbuffer Pointer to the reception buffer.
  * @param length Length of the reception buffer.
  */
typedef void (*ASPEP_config_reception_cb_t)(void *pASPEP_Handle, void *rxbuffer, uint16_t length);

/**
  * @brief Callback type for hardware initialization.
  *
  * @param pUASPEP_Handle Pointer to the UASPEP handle.
  */
typedef void (*ASPEP_hwinit_cb_t)(void *pUASPEP_Handle);

/**
  * @brief Callback type for hardware synchronization.
  *
  * @param pUASPEP_Handle Pointer to the UASPEP handle.
  */
typedef void (*ASPEP_hwsync_cb_t)(void *pUASPEP_Handle);


/* Application level callback */
typedef void (*ASPEP_send_complete_cb_t)(void);
typedef void (*ASPEP_after_sending_cb_t)(void);
typedef void (*ASPEP_receive_new_packet_cb_t)(void);
typedef void (*ASPEP_delay_cb_t)(uint8_t delay);

/**
  * @brief Enumeration for ASPEP control events.
  */
typedef enum
{
  ASPEP_CTRL_CONNECTED_EVT,   /**< Event indicating a connection has been established */
  ASPEP_CTRL_DISCONNECT_EVT,  /**< Event indicating a disconnection has occurred */
  ASPEP_CTRL_RESPONSE_EVT,    /**< Event indicating a response has been received */
  ASPEP_CTRL_ASYNC_EVT,       /**< Event indicating an asynchronous event */
  ASPEP_CTRL_NONE_EVT         /**< No event */
} ASPEP_CTRL_Event_t;

typedef enum
{
  ASPEP_SUCCESS,
  ASPEP_NULL_POINTER_ERROR,
  ASPEP_BAD_PACKET,
  ASPEP_ERROR
} ASPEP_RetVal_t;

/**
  * @brief Structure defining the ASPEP transport layer.
  */
typedef struct
{
  ASPEP_hwinit_cb_t ASPEP_hwinit_cb;  /**< Callback for hardware initialization. */
  ASPEP_hwsync_cb_t ASPEP_hwsync_cb;  /**< Callback for hardware synchronization. */
  ASPEP_config_transmission_cb_t ASPEP_config_transmission_cb;  /**< Callback for configuring transmission. */
  ASPEP_config_reception_cb_t ASPEP_config_reception_cb;        /**< Callback for configuring reception. */
} ASPEP_Transport_Layer_def_t;


/* Public Function declaration -----------------------------------------------*/

/**
  * @brief Initializes the ASPEP control layer.
  *
  * This function initializes the ASPEP control by registering the UART transport layer needed for
  * hardware communication and sets internal states and capabilities of the ASPEP handle.
  *
  * @param[in] p_ASPEP_Transport_Layer Pointer to the ASPEP transport layer definition structure.
  * @param[in] p_UASPEP_Handle Pointer to the UASPEP handle.
  *
  * @return ASPEP_RetVal_t Returns ASPEP_NULL_POINTER_ERROR in case of bad input parameters.
  *                                ASPEP_SUCCESS if initialization is successful.
  */
ASPEP_RetVal_t ASPEP_CTRL_Init(ASPEP_Transport_Layer_def_t *const p_ASPEP_Transport_Layer);

/**
  * @brief Starts the ASPEP connection process.
  *
  * This function initiates the ASPEP connection process by sending a beacon packet.
  * The establishment of the connection must be checked by verifying the reception of
  * ASPEP_CTRL_CONNECTED_EVT from ASPEP_CtrlGetEvent.
  */
void ASPEP_CTRL_StartConnection(void);

/**
  * @brief Initiates the ASPEP disconnection process.
  *
  * This function initiates the ASPEP disconnection process by sending a beacon packet and setting
  * the ASPEP state to idle.
  */
void ASPEP_CTRL_Disconnection(void);

/**
  * @brief Processes the received ASPEP frame.
  * This function processes the latest received frame by the ASPEP transport layer and, if a new packet is available,
  *
  * returns it by setting the pointers *p_frame and *p_frameLength.
  *
  * @param[out] p_frame Pointer to the frame data.
  * @param[out] p_frameLength Pointer to the frame length.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if a new packet is ready.
  *                        ASPEP_BAD_PACKET if a bad packet is detected.
  *                        ASPEP_NULL_POINTER_ERROR in case of bad input parameters.
  */
ASPEP_RetVal_t ASPEP_CTRL_RxFrameProcess(uint8_t **const p_frame, uint16_t *const p_frameLength);

/**
  * @brief Sends an ASPEP packet.
  *
  * This function sends an ASPEP packet by preparing the header and processing the frame.
  * It ensures that the ASPEP state is connected before sending the packet.
  *
  * Before calling this function, the user must get the ASPEP transmission buffer by calling:
  * ASPEP_CtrlGetTxBuffer.
  *
  * @param[in] p_txBuffer Pointer to the transmission buffer.
  * @param[in] txBufferLength Length of the data to be transmitted.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the packet is sent successfully.
  *                        ASPEP_ERROR if there is an error in sending the packet.
  *                        ASPEP_NULL_POINTER_ERROR if the input buffer is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_SendPacket(void *const p_txBuffer, uint16_t txBufferLength);

/**
  * @brief Gets the transmission buffer for ASPEP.
  *
  * This function provides a pointer to the transmission buffer for ASPEP. It ensures that the buffer
  * is available and locks it for writing.
  *
  * @param[out] p_buffer Pointer to the buffer pointer.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the buffer is successfully obtained.
  *                        ASPEP_ERROR if the buffer is not available.
  *                        ASPEP_NULL_POINTER_ERROR if the input buffer pointer is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_GetTxBuffer(void **const p_txBuffer);

/**
  * @brief Get the current ASPEP control event.
  *
  * This function retrieves the current event from the ASPEP handle. If the
  * clearEvent parameter is true, it will reset the event to ASPEP_CTRL_NONE_EVT.
  *
  * @param clearEvent A boolean flag indicating whether to clear the event after retrieval.
  * @return The current ASPEP control event.
  */
ASPEP_CTRL_Event_t ASPEP_CTRL_GetEvent(bool clearEvent);

/*To be removed */
void ASPEP_CTRL_FreeAsyncBuffer(void);

/**
  * @brief Registers a callback for receiving new ASPEP packets.
  *
  * This function registers a callback function that will be called when a new ASPEP packet is received.
  *
  * **Attention:** This function is called within the ISR of the ASPEP transport layer.
  * It can be used to notify the application level but should not be used to implement
  * time-consuming procedures as it can cause loss of packets in the protocol.
  *
  * @param[in] ASPEP_receive_new_packet_cb The callback function to be registered.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the callback is registered successfully.
  *                        ASPEP_NULL_POINTER_ERROR if the input callback is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_RegisterNewPacket_cb(ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb);

/**
  * @brief Registers a callback for transmission completion of the ASPEP transport layer.
  *
  * This function registers a callback function that will be called upon the completion of a transmission
  * in the ASPEP transport layer.
  *
  * **Attention:** This function is called within the ISR of the ASPEP transport layer.
  * It can be used to notify the application level but should not be used to implement
  * time-consuming procedures as it can cause loss of packets in the protocol.
  *
  * @param[in] ASPEP_send_complete_cb The callback function to be registered.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the callback is registered successfully.
  *                        ASPEP_NULL_POINTER_ERROR if the input callback is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_RegisterTxComplete_cb(ASPEP_send_complete_cb_t ASPEP_send_complete_cb);


void ASPEP_CTRL_RegisterTransmitting_cb(ASPEP_after_sending_cb_t ASPEP_after_sending_cb);


void ASPEP_CTRL_RegisterDelay_cb(ASPEP_delay_cb_t ASPEP_delay_cb);

/* IRQ handlers when DMA is used*/
void UASPEP_CTRL_DMA_Rx_Handler(void);
void UASPEP_CTRL_Usart_Handler(void);

void ASPEP_CTRL_HWDMAReset(void);
void ASPEP_CTRL_DataTransmittedIT(void);
void ASPEP_CTRL_ReceivedIT(void);

#ifdef __cplusplus
}
#endif

#endif /*aspep_h*/
