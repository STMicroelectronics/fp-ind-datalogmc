#ifndef MCP_CONFIG_H
#define MCP_CONFIG_H

#include "aspep.h"
#include "stm32u5xx_ll_dma.h"


#define USARTB USART3
#define DMA_RX_B GPDMA1
#define DMA_TX_B GPDMA1
#define DMACH_RX_B LL_DMA_CHANNEL_15
#define DMACH_TX_B LL_DMA_CHANNEL_14


#define MCP_TX_ASYNC_PAYLOAD_MAX_B 2048


extern ASPEP_Handle_t aspepOverUartB;
#endif /* MCP_CONFIG_H */
