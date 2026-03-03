/**
  ******************************************************************************
  * @file    usart_aspep_driver.c
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
#include "gpdma.h"
#include "usart.h"


void UASPEP_DMACONFIG_TX(UASPEP_Handle_t *pHandle);
void UASPEP_DMACONFIG_RX(UASPEP_Handle_t *pHandle);

/**
  * @brief  Initialization of the DMAs used for data transmission and reception.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_INIT(void *pHWHandle)
{
  MX_GPDMA1_MCP_Init();
  MX_USART3_UART_Init();
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  UASPEP_DMACONFIG_TX(pHandle);
  UASPEP_DMACONFIG_RX(pHandle);
}

/**
  * @brief  Configures the DMA used for data transmission to controller.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_DMACONFIG_TX(UASPEP_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  if (NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    /* Enable DMA UART */
    LL_USART_ClearFlag_TC(pHandle->USARTx);
    LL_USART_EnableIT_TC(pHandle->USARTx);

    /* Enable DMA UART to start the TX request */
    LL_USART_EnableDMAReq_TX(pHandle->USARTx);

    /* Write the USART_TDR register address in the DMA control register to configure it as
     * the destination of the transfer */
    //cstat !MISRAC2012-Rule-11.4
    LL_DMA_SetDestAddress(pHandle->txDMA, pHandle->txChannel, (uint32_t) &pHandle->USARTx->TDR);
    /* Clear UART ISR */
    LL_USART_ClearFlag_TC(pHandle->USARTx);

    /* DMA end of transfer on UART TX channel completion is not activated */
    /* We prefer to activate UART TC itself to avoid to trig IT while queued data are still to be transmitted */
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  }
#endif
}

/**
  * @brief  Configures the DMA used for data reception from controller.
  *
  * @param  pHandle Handler of the current instance of the UASPEP component
  */
void UASPEP_DMACONFIG_RX(UASPEP_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  if (NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    /* Enable DMA end of transfer on UART RX channel completion */
    LL_DMA_EnableIT_TC(pHandle->rxDMA, pHandle->rxChannel);
    /* Enable Error interrupt (EIE) to unmask Overrun interrupt */
    LL_USART_EnableIT_ERROR(pHandle->USARTx);

    /* Write the USART_RDR register address in the DMA control register to configure it as
     * the source of the transfer */
    //cstat !MISRAC2012-Rule-11.4
    LL_DMA_SetSrcAddress(pHandle->rxDMA, pHandle->rxChannel, (uint32_t) &pHandle->USARTx->RDR);

    /* Clear UART ISR */
    LL_USART_ClearFlag_TC(pHandle->USARTx);

    LL_USART_EnableDMAReq_RX(pHandle->USARTx);
#ifdef NULL_PTR_CHECK_USA_ASP_DRV
  }
#endif
}

void UASPEP_SEND_PACKET(void *pHWHandle, void *data, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *) pHWHandle; //cstat !MISRAC2012-Rule-11.5

  LL_DMA_IsEnabledChannel(pHandle->txDMA, pHandle->txChannel);

  LL_DMA_SetSrcAddress(pHandle->txDMA, pHandle->txChannel, (uint32_t) data);
  LL_DMA_SetBlkDataLength(pHandle->txDMA, pHandle->txChannel, length);
  LL_DMA_EnableChannel(pHandle->txDMA, pHandle->txChannel);
}

void UASPEP_RECEIVE_BUFFER(void *pHWHandle, void *buffer, uint16_t length)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  LL_DMA_DisableChannel(pHandle->rxDMA, pHandle->rxChannel);
  LL_DMA_SetDestAddress(pHandle->rxDMA, pHandle->rxChannel, (uint32_t) buffer);
  LL_DMA_SetBlkDataLength(pHandle->rxDMA, pHandle->rxChannel, length);

  LL_DMA_EnableChannel(pHandle->rxDMA, pHandle->rxChannel);
}

void UASPEP_IDLE_ENABLE(void *pHWHandle)
{
  UASPEP_Handle_t *pHandle = (UASPEP_Handle_t *)pHWHandle; //cstat !MISRAC2012-Rule-11.5
  LL_USART_ClearFlag_IDLE(pHandle->USARTx);
  LL_USART_EnableIT_IDLE(pHandle->USARTx);
}

void UASPEP_CTRL_DMA_Rx_Handler(void)
{
  if (LL_DMA_IsActiveFlag_TC(ASPEP_CTRL_DMA_RX_A, ASPEP_CTRL_DMA_RX_CH_A))
  {
    /* Clear full buffer transfer complete ISR*/
    LL_DMA_ClearFlag_TC(ASPEP_CTRL_DMA_RX_A, ASPEP_CTRL_DMA_RX_CH_A);
    ASPEP_CTRL_ReceivedIT();
  }
}

void UASPEP_CTRL_Usart_Handler(void)
{
  if (LL_USART_IsActiveFlag_TC(ASPEP_CTRL_USART_A))
  {
    LL_DMA_DisableChannel(ASPEP_CTRL_DMA_TX_A, ASPEP_CTRL_DMA_TX_CH_A);
    LL_USART_ClearFlag_TC(ASPEP_CTRL_USART_A);

    ASPEP_CTRL_DataTransmittedIT();
  }

  if ((LL_USART_IsActiveFlag_ORE(ASPEP_CTRL_USART_A) || LL_USART_IsActiveFlag_FE(ASPEP_CTRL_USART_A)
       || LL_USART_IsActiveFlag_NE(ASPEP_CTRL_USART_A))
      && LL_USART_IsEnabledIT_ERROR(ASPEP_CTRL_USART_A))
  {
    /* Stopping the debugger will generate an OverRun error*/
    WRITE_REG(ASPEP_CTRL_USART_A->ICR, USART_ICR_FECF | USART_ICR_ORECF | USART_ICR_NECF);
    /* We disable ERROR interrupt to avoid to trig one Overrun IT per additional byte received*/
    LL_USART_DisableIT_ERROR(ASPEP_CTRL_USART_A);
    LL_USART_EnableIT_IDLE(ASPEP_CTRL_USART_A);
  }

  if (LL_USART_IsActiveFlag_IDLE(ASPEP_CTRL_USART_A) && LL_USART_IsEnabledIT_IDLE(ASPEP_CTRL_USART_A))
  {
    LL_USART_DisableIT_IDLE(ASPEP_CTRL_USART_A);
    /* Once the complete unexpected data are received, we enable back the error IT*/
    LL_USART_EnableIT_ERROR(ASPEP_CTRL_USART_A);
    /* To be sure we fetch the potential pending data*/
    /* We disable the DMA request, Read the dummy data, endable back the DMA request */

    LL_USART_DisableDMAReq_RX(ASPEP_CTRL_USART_A);
    LL_USART_ReceiveData8(ASPEP_CTRL_USART_A);
    LL_USART_EnableDMAReq_RX(ASPEP_CTRL_USART_A);
    ASPEP_CTRL_HWDMAReset();
    LL_USART_ClearFlag_IDLE(ASPEP_CTRL_USART_A);
  }
}
