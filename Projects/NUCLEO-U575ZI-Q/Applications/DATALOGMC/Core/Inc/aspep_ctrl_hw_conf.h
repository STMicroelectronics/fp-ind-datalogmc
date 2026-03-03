/**
  ******************************************************************************
  * @file    aspep_slave_hw_conf.h
  * @author  STMicroelectronics
  * @brief   User space HW configuration file for ASPEP slave protocol
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
#ifndef ASPEP_SLAVE_HW_CONF_H
#define ASPEP_SLAVE_HW_CONF_H

/*
 * Include LL header files for the correct platform
 **/
#if defined(STM32U575xx)
/*
 * STM32U5
 **/
#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_usart.h"
#include "stm32u5xx_ll_dma.h"

#elif defined(STM32F4)
/*
 * STM32F4
 **/
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_dma.h"

/*
 * The following functions are defined here as they are missing in STM32F4 LL implementation
 **/

/**
  * @brief  Driver macro reserved for internal use: set a pointer to
  *         a register from a register basis from which an offset
  *         is applied.
  * @param  __REG__ Register basis from which the offset is applied.
  * @param  __REG_OFFFSET__ Offset to be applied (unit number of registers).
  * @retval Pointer to register address
  */
#define __DMA_PTR_REG_OFFSET(__REG__, __REG_OFFFSET__)                         \
  ((__IO uint32_t *)((uint32_t) ((uint32_t)(&(__REG__)) + ((__REG_OFFFSET__) << 2U))))

#define __LL_DMA_IT_TC_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 5 : \
   ((__STREAM_NB__&0x3) == 1U) ? 11 :\
   ((__STREAM_NB__&0x3) == 2U) ? 21 : 27)

#define __LL_DMA_IT_HT_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 4 : \
   ((__STREAM_NB__&0x3) == 1U) ? 10 :\
   ((__STREAM_NB__&0x3) == 2U) ? 20 : 26)

#define __LL_DMA_IT_TE_BIT(__STREAM_NB__) \
  (((__STREAM_NB__&0x3) == 0U) ? 3 : \
   ((__STREAM_NB__&0x3) == 1U) ? 9 :\
   ((__STREAM_NB__&0x3) == 2U) ? 19 : 25)

__STATIC_INLINE void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TC bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_TC_BIT(Stream));
  }
}

__STATIC_INLINE void LL_DMA_ClearFlag_TE(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TE bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_TE_BIT(Stream));
  }
}

//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Stream)
{
  uint32_t retVal;
  if (NULL == DMAx)
  {
    retVal = 0;
  }
  else
  {
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LISR, (Stream <= 3) ? 0 : 1);
    retVal = ((READ_BIT(*preg, 1 << __LL_DMA_IT_TC_BIT(Stream)) == (1 << __LL_DMA_IT_TC_BIT(Stream))) ? 1UL : 0UL);
  }
  return (retVal);
}
//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE void LL_DMA_ClearFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TC bits with bits position depending on parameter "Stream" */
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LIFCR, (Stream <= 3) ? 0 : 1);

    WRITE_REG(*preg, 1 << __LL_DMA_IT_HT_BIT(Stream));
  }
}
//cstat !MISRAC2012-Rule-8.13
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_HT(DMA_TypeDef *DMAx, uint32_t Stream)
{
  uint32_t retVal;
  if (NULL == DMAx)
  {
    retVal = 0;
  }
  else
  {
    register __IO uint32_t *preg = __DMA_PTR_REG_OFFSET(DMAx->LISR, (Stream <= 3) ? 0 : 1);
    retVal = ((READ_BIT(*preg, 1 << __LL_DMA_IT_HT_BIT(Stream)) == (1 << __LL_DMA_IT_HT_BIT(Stream))) ? 1UL : 0UL);
  }
  return (retVal);
}

#elif defined(STM32L4)
/*
 * STM32L4
 **/
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_dma.h"

/*
 * The following functions are defined here as they are missing in STM32L4 LL implementation
 **/
__STATIC_INLINE void LL_DMA_ClearFlag_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear TC bits with bits position depending on parameter "Channel".      */
    WRITE_REG(DMAx->IFCR, DMA_IFCR_CTCIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2));
  }
}

__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return ((NULL == DMAx) ? 0U : ((READ_BIT(DMAx->ISR, (DMA_ISR_TCIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2))) == (DMA_ISR_TCIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2))) ? 1UL : 0UL));
}

__STATIC_INLINE void LL_DMA_ClearFlag_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  if (NULL == DMAx)
  {
    /* Nothing to do */
  }
  else
  {
    /* Clear HT bits with bits position depending on parameter "Channel".      */
    WRITE_REG(DMAx->IFCR, DMA_IFCR_CHTIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2));
  }
}

__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return ((NULL == DMAx) ? 0U : ((READ_BIT(DMAx->ISR, (DMA_ISR_HTIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2))) == (DMA_ISR_HTIF1 << ((Channel - LL_DMA_CHANNEL_1) << 2))) ? 1UL : 0UL));
}

#endif

/**
  * @}
  */

#endif /* ASPEP_SLAVE_HW_CONF_H */
/******************* (C) COPYRIGHT 2022 STMicroelectronics *****END OF FILE****/
