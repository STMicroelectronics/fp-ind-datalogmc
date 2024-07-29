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

/**
 * @brief Maximum size of the MCP receive sync payload
 */
#define MCP_RX_SYNC_PAYLOAD_MAX 1024

/**
 * @brief Maximum size of the MCP transmit sync payload
 */
#define MCP_TX_SYNC_PAYLOAD_MAX 1024
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define ASPEP_HEADER_SIZE 4
#define ASPEP_DATACRC_SIZE 2

#define EVENT_NONE         0
#define EVENT_CONNECTED    1
#define EVENT_RESPONSE     2
#define EVENT_DISCONNECTED 3
#define EVENT_ASYNC        4


/* Application level callback */
typedef void (*ASPEP_send_complete_cb_t)(void);
typedef void (*ASPEP_after_sending_cb_t)(void);
typedef void (*ASPEP_receive_new_packet_cb_t)(void);
typedef void (*ASPEP_delay_cb_t)(uint8_t delay);


/* Public function declaration */
void ASPEP_Init(void);
void ASPEP_StartConnection(void);
void ASPEP_Disconnection(void);

uint8_t* ASPEP_RxFrameProcess (uint16_t *packetLength);
void ASPEP_SendPacket (void *txBuffer, uint16_t txDataLength);
bool ASPEP_getBuffer ( void **buffer);
uint8_t ASPEP_GetStatus(uint8_t Flag, bool ClearFlag);
void ASPEP_FreeBuffer(void);

void ASPEP_RegisterTxComplete_cb( ASPEP_send_complete_cb_t ASPEP_send_complete_cb);
void ASPEP_RegisterTransmitting_cb( ASPEP_after_sending_cb_t ASPEP_after_sending_cb);
void ASPEP_RegisterNewPacket_cb( ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb);
void ASPEP_RegisterDelay_cb( ASPEP_delay_cb_t ASPEP_delay_cb);

#ifdef __cplusplus
}
#endif

#endif /*aspep_h*/
