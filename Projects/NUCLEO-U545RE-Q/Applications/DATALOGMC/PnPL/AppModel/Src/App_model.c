/**
  ******************************************************************************
  * @file    App_Model.c
  * @author  SRA
  * @brief   App Application Model and PnPL Components APIs
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/**
  ******************************************************************************
  * This file has been auto generated from the following Device Template Model:
  * dtmi:vespucci:nucleo_u575zi_q:x_nucleo_iks02a1:fpSnsDatalog2_datalog2;1
  *
  * Created by: DTDL2PnPL_cGen version 2.1.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"
#include <string.h>
#include <stdio.h>
/* USER includes -------------------------------------------------------------*/
#include "services/SQuery.h"
#include "services/sysdebug.h"
#include "ux_user.h"
/* USER defines --------------------------------------------------------------*/
#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_DT, level, message)

#define SC_DL2_PROTOCOL_COUNTER_SIZE     4U
#define SC_DL2_PROTOCOL_TIMESTAMP_SIZE   8U
/* Max DPS for USB */
#define SC_USB_DPS_MAX                7000U
/* Under this limit the stream is considered "slow" */
#define SC_USB_SLOW_ODR_LIMIT_HZ      20.0f
/* Maximum time between two consecutive stream packets */
#define SC_USB_MAX_PACKETS_PERIOD     0.05f
/* Maximum data bandwidth supported (byte) */
#define SC_SAFE_BANDWIDTH             800000

/* USER private function prototypes ------------------------------------------*/
AppModel_t app_model;

AppModel_t *getAppModel(void)
{
  return &app_model;
}

/* USER functions ------------------------------------------------------------*/
uint8_t set_mac_address(const char *value)
{
  strcpy(app_model.firmware_info_model.mac_address, value);
  return PNPL_NO_ERROR_CODE;
}

