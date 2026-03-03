/**
 ******************************************************************************
 * @file    NUCLEO-U575ZI-Q_debug_pins.h
 * @author  SRA
 * @brief   This file contains definitions for STWIN.box debug pins on
 *          DIL24 connector
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NUCLEO_U575ZI_Q_DEBUG_PINS_H
#define NUCLEO_U575ZI_Q_DEBUG_PINS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STWIN.box
  * @{
  */
      
  /** @addtogroup STWIN.box_LOW_LEVEL
  * @{
  */
   

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Types STWIN.box_LOW_LEVEL Exported Types
  * @{
  */
typedef enum
{
    CON8_PIN_2,
    CON8_PIN_4,
    CON8_PIN_6,
    CON8_PIN_8,
    CON8_PIN_10,
    CON8_PIN_12,
    CON8_PIN_14,
    CON8_PIN_16,
    CON9_PIN_2,
    CON9_PIN_4,
    CON9_PIN_6,
    CON9_PIN_8,
    CON9_PIN_10,
    CON9_PIN_14,
    CON9_PIN_16,
    CON9_PIN_18,
    CON9_PIN_20,
    CON9_PIN_22,
    CON9_PIN_24,
    CON9_PIN_26,
    CON9_PIN_28,
    DEBUG_PIN_MAX_NUM
} Debug_Pin_TypeDef;

extern const uint16_t  DEBUG_PIN[DEBUG_PIN_MAX_NUM];
extern GPIO_TypeDef  *DEBUG_PIN_PORT[DEBUG_PIN_MAX_NUM];

/**
  * @}
  */ 

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Constants STWIN.box_LOW_LEVEL Exported Constants
  * @{
  */ 


/** @defgroup STWIN.box_LOW_LEVEL_DEBUG_PINS PINS
  * @{
  */
  
#define DEBUG_CON8_PIN_2_GPIO_PIN                     GPIO_PIN_8
#define DEBUG_CON8_PIN_2_GPIO_PORT                    GPIOC
#define DEBUG_CON8_PIN_2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_CON8_PIN_2_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define DEBUG_CON8_PIN_4_GPIO_PIN                     GPIO_PIN_9
#define DEBUG_CON8_PIN_4_GPIO_PORT                    GPIOC
#define DEBUG_CON8_PIN_4_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_CON8_PIN_4_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define DEBUG_CON8_PIN_6_GPIO_PIN                     GPIO_PIN_10
#define DEBUG_CON8_PIN_6_GPIO_PORT                    GPIOC
#define DEBUG_CON8_PIN_6_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_CON8_PIN_6_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define DEBUG_CON8_PIN_8_GPIO_PIN                     GPIO_PIN_11
#define DEBUG_CON8_PIN_8_GPIO_PORT                    GPIOC
#define DEBUG_CON8_PIN_8_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_CON8_PIN_8_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

#define DEBUG_CON8_PIN_10_GPIO_PIN                    GPIO_PIN_12
#define DEBUG_CON8_PIN_10_GPIO_PORT                   GPIOC
#define DEBUG_CON8_PIN_10_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define DEBUG_CON8_PIN_10_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()

#define DEBUG_CON8_PIN_12_GPIO_PIN                    GPIO_PIN_2
#define DEBUG_CON8_PIN_12_GPIO_PORT                   GPIOD
#define DEBUG_CON8_PIN_12_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON8_PIN_12_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON8_PIN_14_GPIO_PIN                    GPIO_PIN_3
#define DEBUG_CON8_PIN_14_GPIO_PORT                   GPIOF
#define DEBUG_CON8_PIN_14_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_CON8_PIN_14_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define DEBUG_CON8_PIN_16_GPIO_PIN                    GPIO_PIN_5
#define DEBUG_CON8_PIN_16_GPIO_PORT                   GPIOF
#define DEBUG_CON8_PIN_16_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_CON8_PIN_16_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define DEBUG_CON9_PIN_2_GPIO_PIN                    GPIO_PIN_7
#define DEBUG_CON9_PIN_2_GPIO_PORT                   GPIOD
#define DEBUG_CON9_PIN_2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON9_PIN_2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON9_PIN_4_GPIO_PIN                    GPIO_PIN_6
#define DEBUG_CON9_PIN_4_GPIO_PORT                   GPIOD
#define DEBUG_CON9_PIN_4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON9_PIN_4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON9_PIN_6_GPIO_PIN                   GPIO_PIN_5
#define DEBUG_CON9_PIN_6_GPIO_PORT                  GPIOD
#define DEBUG_CON9_PIN_6_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON9_PIN_6_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON9_PIN_8_GPIO_PIN                   GPIO_PIN_4
#define DEBUG_CON9_PIN_8_GPIO_PORT                  GPIOD
#define DEBUG_CON9_PIN_8_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON9_PIN_8_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON9_PIN_10_GPIO_PIN                   GPIO_PIN_3
#define DEBUG_CON9_PIN_10_GPIO_PORT                  GPIOD
#define DEBUG_CON9_PIN_10_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_CON9_PIN_10_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOD_CLK_DISABLE()

#define DEBUG_CON9_PIN_14_GPIO_PIN                   GPIO_PIN_2
#define DEBUG_CON9_PIN_14_GPIO_PORT                  GPIOE
#define DEBUG_CON9_PIN_14_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define DEBUG_CON9_PIN_14_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define DEBUG_CON9_PIN_16_GPIO_PIN                   GPIO_PIN_4
#define DEBUG_CON9_PIN_16_GPIO_PORT                  GPIOE
#define DEBUG_CON9_PIN_16_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define DEBUG_CON9_PIN_16_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define DEBUG_CON9_PIN_18_GPIO_PIN                   GPIO_PIN_5
#define DEBUG_CON9_PIN_18_GPIO_PORT                  GPIOE
#define DEBUG_CON9_PIN_18_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define DEBUG_CON9_PIN_18_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define DEBUG_CON9_PIN_20_GPIO_PIN                   GPIO_PIN_6
#define DEBUG_CON9_PIN_20_GPIO_PORT                  GPIOE
#define DEBUG_CON9_PIN_20_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define DEBUG_CON9_PIN_20_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()
  
#define DEBUG_CON9_PIN_22_GPIO_PIN                   GPIO_PIN_3
#define DEBUG_CON9_PIN_22_GPIO_PORT                  GPIOE
#define DEBUG_CON9_PIN_22_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()
#define DEBUG_CON9_PIN_22_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOE_CLK_DISABLE()

#define DEBUG_CON9_PIN_24_GPIO_PIN                   GPIO_PIN_8
#define DEBUG_CON9_PIN_24_GPIO_PORT                  GPIOI
#define DEBUG_CON9_PIN_24_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOI_CLK_ENABLE()
#define DEBUG_CON9_PIN_24_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOI_CLK_DISABLE()
  
#define DEBUG_CON9_PIN_26_GPIO_PIN                   GPIO_PIN_7
#define DEBUG_CON9_PIN_26_GPIO_PORT                  GPIOF
#define DEBUG_CON9_PIN_26_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_CON9_PIN_26_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOF_CLK_DISABLE()

#define DEBUG_CON9_PIN_28_GPIO_PIN                   GPIO_PIN_9
#define DEBUG_CON9_PIN_28_GPIO_PORT                  GPIOF
#define DEBUG_CON9_PIN_28_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define DEBUG_CON9_PIN_28_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOF_CLK_DISABLE()



#define DEBUG_PINx_GPIO_CLK_ENABLE(__DEBUG_PIN__)   do { if((__DEBUG_PIN__) == CON8_PIN_2)  { DEBUG_CON8_PIN_2_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_4)  { DEBUG_CON8_PIN_4_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_6)  { DEBUG_CON8_PIN_6_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_8)  { DEBUG_CON8_PIN_8_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_10) { DEBUG_CON8_PIN_10_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_12) { DEBUG_CON8_PIN_12_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_14) { DEBUG_CON8_PIN_14_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_16) { DEBUG_CON8_PIN_16_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_2)  { DEBUG_CON9_PIN_2_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_4)  { DEBUG_CON9_PIN_4_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_6)  { DEBUG_CON9_PIN_6_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_8)  { DEBUG_CON9_PIN_8_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_10) { DEBUG_CON9_PIN_10_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_14) { DEBUG_CON9_PIN_14_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_16) { DEBUG_CON9_PIN_16_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_18) { DEBUG_CON9_PIN_18_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_20) { DEBUG_CON9_PIN_20_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_22) { DEBUG_CON9_PIN_22_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_24) { DEBUG_CON9_PIN_24_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_26) { DEBUG_CON9_PIN_26_GPIO_CLK_ENABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_28) { DEBUG_CON9_PIN_28_GPIO_CLK_ENABLE(); } else \
                                                         { /*No more pin available on STWIN CON34 connector*/ } } while(0)
                                                           
#define DEBUG_PINx_GPIO_CLK_DISABLE(__DEBUG_PIN__)  do { if((__DEBUG_PIN__) == CON8_PIN_2)  { DEBUG_CON8_PIN_2_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_4)  { DEBUG_CON8_PIN_4_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_6)  { DEBUG_CON8_PIN_6_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_8)  { DEBUG_CON8_PIN_8_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_10) { DEBUG_CON8_PIN_10_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_12) { DEBUG_CON8_PIN_12_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_14) { DEBUG_CON8_PIN_14_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON8_PIN_16) { DEBUG_CON8_PIN_16_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_2)  { DEBUG_CON9_PIN_2_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_4)  { DEBUG_CON9_PIN_4_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_6)  { DEBUG_CON9_PIN_6_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_8)  { DEBUG_CON9_PIN_8_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_10) { DEBUG_CON9_PIN_10_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_14) { DEBUG_CON9_PIN_14_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_16) { DEBUG_CON9_PIN_16_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_18) { DEBUG_CON9_PIN_18_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_20) { DEBUG_CON9_PIN_20_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_22) { DEBUG_CON9_PIN_22_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_24) { DEBUG_CON9_PIN_24_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_26) { DEBUG_CON9_PIN_26_GPIO_CLK_DISABLE(); } else \
                                                         if((__DEBUG_PIN__) == CON9_PIN_28) { DEBUG_CON9_PIN_28_GPIO_CLK_DISABLE(); } else \
                                                         { /*No more pin available on STWIN CON34 connector*/ } } while(0)


/**
  * @}
  */ 
  
/**
  * @}
  */ 

/** @defgroup STWIN.box_LOW_LEVEL_Exported_Functions STWIN.box_LOW_LEVEL Exported Functions
  * @{
  */

void BSP_DEBUG_PIN_Init_All(void);
void BSP_DEBUG_PIN_Init(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_DeInit(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_On(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_Off(Debug_Pin_TypeDef Pin);
void BSP_DEBUG_PIN_Toggle(Debug_Pin_TypeDef Pin);

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* NUCLEO_U575ZI_Q_DEBUG_PINS_H */

