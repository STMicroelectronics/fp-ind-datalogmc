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

#ifndef usart_aspep_driver_h
#define usart_aspep_driver_h

#include <stdint.h>
#include <stdbool.h>
#include "stm32u585xx.h"

  
/* To be removed no protocol awarness at this level */

typedef struct {
  USART_TypeDef *USARTx;
  DMA_TypeDef * rxDMA;
  DMA_TypeDef * txDMA;
  uint32_t rxChannel;
  uint32_t txChannel;
} UASPEP_Handle_t;

bool UASPEP_SEND_PACKET(void *pHWHandle, void *data, uint16_t length);
void UASPEP_RECEIVE_BUFFER (void *pHandle, void* buffer, uint16_t length);
void UASPEP_INIT (void *pHandle);
void UASPEP_IDLE_ENABLE (void *pHWHandle);

#endif
