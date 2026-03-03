/**
  ******************************************************************************
  * @file    mcp_config.c
  * @author  SRA
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

#include "usart_aspep_driver.h"
#include "aspep.h"
#include "mcp_config.h"
#include "mcpa.h"
#include "appli.h"


//#define MCP_RX_SYNC_PAYLOAD_MAX 1024
//#define MCP_TX_SYNC_PAYLOAD_MAX 1024
//
//#define MCP_TX_ASYNC_PAYLOAD_MAX_B 2048
//#define MCP_TX_ASYNC_PAYLOAD_MAX_A 2048
//
//#define MCP_TX_SYNCBUFFER_SIZE (MCP_TX_SYNC_PAYLOAD_MAX+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)
//#define MCP_RX_SYNCBUFFER_SIZE (MCP_RX_SYNC_PAYLOAD_MAX+ASPEP_DATACRC_SIZE) // ASPEP_HEADER_SIZE is not stored in the RX buffer.
//#define MCP_TX_ASYNCBUFFER_SIZE_A (MCP_TX_ASYNC_PAYLOAD_MAX_A+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)
//#define MCP_TX_ASYNCBUFFER_SIZE_B (MCP_TX_ASYNC_PAYLOAD_MAX_B+ASPEP_HEADER_SIZE+ASPEP_DATACRC_SIZE)
//
//
///* Asynchronous buffer dedicated to UART_A*/
//uint8_t MCPAsyncBuffUARTB_A[MCP_TX_ASYNCBUFFER_SIZE_B];
//uint8_t MCPAsyncBuffUARTB_B[MCP_TX_ASYNCBUFFER_SIZE_B];
//
//
//uint8_t MCPSyncTxBuff[MCP_TX_SYNCBUFFER_SIZE];
//uint8_t MCPSyncRXBuff[MCP_RX_SYNCBUFFER_SIZE];
//
//
//UASPEP_Handle_t UASPEP_B =
//{
// .USARTx = USARTB,
// .rxDMA = DMA_RX_B,
// .txDMA = DMA_TX_B,
// .rxChannel = DMACH_RX_B,
// .txChannel = DMACH_TX_B,
//};
//
//
//
//ASPEP_Handle_t aspepOverUartB =
//{
//  ._Super =
//   {
//    .fGetBuffer = &ASPEP_getBuffer,
//    .fSendPacket = &ASPEP_SendPacket,
//    .fRXPacketProcess = &ASPEP_RxFrameProcess,
//    .fStartConnection = &ASPEP_StartConnection,
//    .fDisconnection = &ASPEP_Disconnection,
//    .fGetStatus = &ASPEP_GetStatus,
//    .fFreeBuffer = &ASPEP_FreeBuffer,
//    },
//  .HWIp = &UASPEP_B,
//  .Capabilities = {
//    .DATA_CRC = 0,
//    .RX_maxSize =  (MCP_RX_SYNC_PAYLOAD_MAX>>5)-1,
//    .TXS_maxSize = (MCP_TX_SYNC_PAYLOAD_MAX>>5)-1,
//    .TXA_maxSize =  MCP_TX_ASYNC_PAYLOAD_MAX_B>>6,
//    .version = 0x0,
//  },
//  .syncBuffer = {
//   .buffer = MCPSyncTxBuff,
//  },
//  .asyncBufferA = {
//    .buffer = MCPAsyncBuffUARTB_A,
//  },
//  .asyncBufferB = {
//    .buffer = MCPAsyncBuffUARTB_B,
//  },
//  .rxBuffer = MCPSyncRXBuff,
//  .fASPEP_HWInit = &UASPEP_INIT,
//  .fASPEP_HWSync = &UASPEP_IDLE_ENABLE,
//  .fASPEP_receive = &UASPEP_RECEIVE_BUFFER,
//  .fASPEP_send = &UASPEP_SEND_PACKET,
//  .packetNumber = 0,
//  .counter = 0,
//  .WaitingForAnswer = false,
//  .badPacketFlag = 0,
//};


//MCP_Handle_t MCP_Over_UartB =
//{
//  .pTransportLayer = (MCTL_Handle_t *) &aspepOverUartB,
//  .fDeliverPacket = NULL,
//  .fConnected = &APPLI_Connected_cb,
//  .fDeliverAsync = &APPLI_Async_cb,
//};


//MCPA_Handle_t MCPA_UART_B =
//{
//  .pTransportLayer = (MCTL_Handle_t *) &aspepOverUartB,
//  .MaxBuffSize = MCP_TX_ASYNC_PAYLOAD_MAX_B,
//  .HFRate = 0,
//  .HFNum = 0,
//  .MFRate = 0xff,
//  .MFNum = 0,
//  .Mark = 1,
//};

