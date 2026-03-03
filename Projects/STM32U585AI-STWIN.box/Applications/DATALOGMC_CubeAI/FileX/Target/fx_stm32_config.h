/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fx_stm32_config.h
  * @author  SRA
  * @brief   FileX STM32 config header file
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FX_STM32_CONFIG_H__
#define __FX_STM32_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "App_model.h"

/* Private defines -----------------------------------------------------------*/

/* define the number of .dat file */
#define FILEX_DCTRL_DAT_FILES_COUNT             SENSOR_NUMBER + ACTUATOR_NUMBER + ALGORITHM_NUMBER

/* uncomment define if the functionality is supported */
//#define UCF_SUPPORT
//#define AUTOMODE_SUPPORT


#ifdef __cplusplus
}
#endif
#endif  /* __UX_STM32_CONFIG_H__ */
