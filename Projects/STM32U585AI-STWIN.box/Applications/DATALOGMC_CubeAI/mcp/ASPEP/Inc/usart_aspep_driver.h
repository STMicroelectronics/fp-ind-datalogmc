/**
  ******************************************************************************
  * @file    usart_aspep_driver.h
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

#ifndef USART_ASPEP_DRIVER_H
#define USART_ASPEP_DRIVER_H

#include "aspep_ctrl.h"

#include <stdint.h>
#include <stdbool.h>

/* To be removed no protocol awareness at this level */

typedef struct
{
  USART_TypeDef *USARTx;
  DMA_TypeDef *rxDMA;
  DMA_TypeDef *txDMA;
  uint32_t rxChannel;
  uint32_t txChannel;
#if ASPEP_USE_DMA == 0
  UART_HandleTypeDef *huartn;
#endif
} UASPEP_Handle_t;

void UASPEP_SEND_PACKET(void *pHWHandle, void *data, uint16_t length);
void UASPEP_RECEIVE_BUFFER(void *pHWHandle, void *buffer, uint16_t length);
void UASPEP_INIT(void *pHWHandle);
void UASPEP_IDLE_ENABLE(void *pHWHandle);


/* Handlers prototypes defined in aspep.h */

#endif
