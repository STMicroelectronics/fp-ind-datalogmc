/**
  ******************************************************************************
  * @file    STWIN.box_conf.h
  * @author  SRA
  * @brief   This file contains definitions for the components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *                             
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STWIN_BOX_CONF_H__
#define STWIN_BOX_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "STWIN.box_errno.h"


/* SD card interrupt priority */
#define BSP_SD_IT_PRIORITY            14U  /* Default is lowest priority level */
#define BSP_SD_RX_IT_PRIORITY         14U  /* Default is lowest priority level */
#define BSP_SD_TX_IT_PRIORITY         15U  /* Default is lowest priority level */

#define BUTTON_USER_IT_PRIORITY       14U
#define BUTTON_PWR_IT_PRIORITY        14U


#ifdef __cplusplus
}
#endif

#endif /* STWIN_BOX_CONF_H__*/



