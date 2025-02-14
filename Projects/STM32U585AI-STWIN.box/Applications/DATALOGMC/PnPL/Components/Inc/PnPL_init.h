/**
  ******************************************************************************
  * @file    PnPL_init.h
  * @author  SRA
  * @brief   PnPL Components initialization functions
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

/**
  ******************************************************************************
  * This file has been auto generated from the following Device Template Model:
  * dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC;2
  *
  * Created by: DTDL2PnPL_cGen version 2.1.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#ifndef PNPL_INIT_H_
#define PNPL_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PnPLCompManager.h"
#include "Slow_Mc_Telemetries_PnPL.h"
#include "Fast_Mc_Telemetries_PnPL.h"
#include "Iis3dwb_Acc_PnPL.h"
#include "Ilps22qs_Press_PnPL.h"
#include "Imp23absu_Mic_PnPL.h"
#include "Ism330dhcx_Acc_PnPL.h"
#include "Ism330dhcx_Gyro_PnPL.h"
#include "Ism330dhcx_Mlc_PnPL.h"
#include "Stts22h_Temp_PnPL.h"
#include "Log_Controller_PnPL.h"
#include "Motor_Controller_PnPL.h"
#include "Tags_Info_PnPL.h"
#include "Acquisition_Info_PnPL.h"
#include "Firmware_Info_PnPL.h"
#include "Deviceinformation_PnPL.h"


uint8_t PnPL_Components_Alloc(void);
uint8_t PnPL_Components_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* PNPL_INIT_H_ */