/**
  ******************************************************************************
  * @file    PnPL_init.c
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

#include "PnPL_init.h"

static IPnPLComponent_t *pSlow_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pFast_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pIis3dwb_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pIlps22qs_Press_PnPLObj = NULL;
static IPnPLComponent_t *pImp23absu_Mic_PnPLObj = NULL;
static IPnPLComponent_t *pIsm330dhcx_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pIsm330dhcx_Gyro_PnPLObj = NULL;
static IPnPLComponent_t *pIsm330dhcx_Mlc_PnPLObj = NULL;
static IPnPLComponent_t *pStts22h_Temp_PnPLObj = NULL;
static IPnPLComponent_t *pLog_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pMotor_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pTags_Info_PnPLObj = NULL;
static IPnPLComponent_t *pAcquisition_Info_PnPLObj = NULL;
static IPnPLComponent_t *pFirmware_Info_PnPLObj = NULL;
static IPnPLComponent_t *pDeviceinformation_PnPLObj = NULL;

uint8_t PnPL_Components_Alloc(void)
{
  /* PnPL Components Allocation */
  pSlow_Mc_Telemetries_PnPLObj = Slow_Mc_Telemetries_PnPLAlloc();
  pFast_Mc_Telemetries_PnPLObj = Fast_Mc_Telemetries_PnPLAlloc();
  pIis3dwb_Acc_PnPLObj = Iis3dwb_Acc_PnPLAlloc();
  pIlps22qs_Press_PnPLObj = Ilps22qs_Press_PnPLAlloc();
  pImp23absu_Mic_PnPLObj = Imp23absu_Mic_PnPLAlloc();
  pIsm330dhcx_Acc_PnPLObj = Ism330dhcx_Acc_PnPLAlloc();
  pIsm330dhcx_Gyro_PnPLObj = Ism330dhcx_Gyro_PnPLAlloc();
  pIsm330dhcx_Mlc_PnPLObj = Ism330dhcx_Mlc_PnPLAlloc();
  pStts22h_Temp_PnPLObj = Stts22h_Temp_PnPLAlloc();
  pLog_Controller_PnPLObj = Log_Controller_PnPLAlloc();
  pMotor_Controller_PnPLObj = Motor_Controller_PnPLAlloc();
  pTags_Info_PnPLObj = Tags_Info_PnPLAlloc();
  pAcquisition_Info_PnPLObj = Acquisition_Info_PnPLAlloc();
  pFirmware_Info_PnPLObj = Firmware_Info_PnPLAlloc();
  pDeviceinformation_PnPLObj = Deviceinformation_PnPLAlloc();
  return PNPL_NO_ERROR_CODE;
}

uint8_t PnPL_Components_Init(void)
{
  /* Init&Add PnPL Components */
  Slow_Mc_Telemetries_PnPLInit(pSlow_Mc_Telemetries_PnPLObj);
  Fast_Mc_Telemetries_PnPLInit(pFast_Mc_Telemetries_PnPLObj);
  Iis3dwb_Acc_PnPLInit(pIis3dwb_Acc_PnPLObj);
  Ilps22qs_Press_PnPLInit(pIlps22qs_Press_PnPLObj);
  Imp23absu_Mic_PnPLInit(pImp23absu_Mic_PnPLObj);
  Ism330dhcx_Acc_PnPLInit(pIsm330dhcx_Acc_PnPLObj);
  Ism330dhcx_Gyro_PnPLInit(pIsm330dhcx_Gyro_PnPLObj);
  Ism330dhcx_Mlc_PnPLInit(pIsm330dhcx_Mlc_PnPLObj);
  Stts22h_Temp_PnPLInit(pStts22h_Temp_PnPLObj);
  Log_Controller_PnPLInit(pLog_Controller_PnPLObj);
  Motor_Controller_PnPLInit(pMotor_Controller_PnPLObj);
  Tags_Info_PnPLInit(pTags_Info_PnPLObj);
  Acquisition_Info_PnPLInit(pAcquisition_Info_PnPLObj);
  Firmware_Info_PnPLInit(pFirmware_Info_PnPLObj);
  Deviceinformation_PnPLInit(pDeviceinformation_PnPLObj);
  return PNPL_NO_ERROR_CODE;
}