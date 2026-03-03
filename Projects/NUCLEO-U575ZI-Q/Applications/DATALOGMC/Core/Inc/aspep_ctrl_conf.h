/**
  ******************************************************************************
  * @file    aspep_ctrl_conf.h
  * @author  STMicroelectronics
  * @brief   User space configuration file for ASPEP Control protocol
  *
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
  ******************************************************************************
  */

#ifndef ASPEP_SLAVE_CONF_H
#define ASPEP_SLAVE_CONF_H

#include "aspep_ctrl_hw_conf.h"

#define ASPEP_CTRL_USART_A                 USART3
#define ASPEP_CTRL_DMA_RX_A                GPDMA1
#define ASPEP_CTRL_DMA_TX_A                GPDMA1
#define ASPEP_CTRL_DMA_RX_CH_A             LL_DMA_CHANNEL_15
#define ASPEP_CTRL_DMA_TX_CH_A             LL_DMA_CHANNEL_14


/* TXS_MAX -> Ranges from 32 to 4096 Bytes */
#define ASPEP_CTRL_TX_SYNC_PAYLOAD_MAX     4096U

/* RXS_MAX -> Ranges from 32 to 2048 Bytes */
#define ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX     2048U

/* TXA_MAX -> Ranges from 0 to 8192 Bytes */
#define ASPEP_CTRL_TX_ASYNC_PAYLOAD_MAX_A  8128U

/* Define payload type */
#define ASPEP_CTRL_PAYLOAD_TYPE            ASPEP_PAYLOAD_TYPE_SSTL

#define ASPEP_CTRL_CALLBACKS_ENABLE             1

#define ASPEP_CTRL_USE_EXTERNAL_ASYNC_BUFFERS   0

/* Set to 0 in aspep_slave_conf to disable DMA */
#define ASPEP_CTRL_USE_DMA                      1


#endif /* ASPEP_SLAVE_CONF_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
