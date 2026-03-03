/**
  ******************************************************************************
  * @file    aspep_config.c
  * @author  STMicroelectronics
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "usart_aspep_driver.h"

static UASPEP_Handle_t UASPEP_A =
{
#if ASPEP_CTRL_USE_DMA == 1
  .USARTx = ASPEP_CTRL_USART_A,
  .rxDMA = ASPEP_CTRL_DMA_RX_A,
  .txDMA = ASPEP_CTRL_DMA_TX_A,
  .rxChannel = ASPEP_CTRL_DMA_RX_CH_A,
  .txChannel = ASPEP_CTRL_DMA_TX_CH_A,
#else
  .huartn = ASPEP_USART_HANDLE,
  .rxDMA = 0,
  .txDMA = 0,
  .rxChannel = 0,
  .txChannel = 0,
#endif
};

void *UASPEP_GetHWIp(void)
{
  return (void *)&UASPEP_A;
}

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
