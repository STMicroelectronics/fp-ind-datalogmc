/**
  ******************************************************************************
  * @file    App_model_Iis3dwb_Acc.c
  * @author  SRA
  * @brief   Iis3dwb_Acc PnPL Components APIs
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
  * This file has been auto generated from the following DTDL Component:
  * dtmi:vespucci:steval_stwinbx1:fpSnsDatalog2_datalog2:sensors:iis3dwb_acc;4
  *
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"
#include "services/SQuery.h"

/* USER includes -------------------------------------------------------------*/

/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/

/* IIS3DWB_ACC PnPL Component ------------------------------------------------*/
static SensorModel_t iis3dwb_acc_model;
extern AppModel_t app_model;

uint8_t iis3dwb_acc_comp_init(void)
{
  iis3dwb_acc_model.comp_name = iis3dwb_acc_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "iis3dwb", COM_TYPE_ACC);
  iis3dwb_acc_model.id = id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatusPointer(id);
  iis3dwb_acc_model.stream_params.stream_id = -1;
  iis3dwb_acc_model.stream_params.usb_ep = -1;

  addSensorToAppModel(id, &iis3dwb_acc_model);

  iis3dwb_acc_set_sensor_annotation("\0");
#if (HSD_USE_DUMMY_DATA == 1)
  iis3dwb_acc_set_samples_per_ts(0);
#else
  iis3dwb_acc_set_samples_per_ts(1000);
#endif
  __stream_control(true);
  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *iis3dwb_acc_get_key(void)
{
  return "iis3dwb_acc";
}


uint8_t iis3dwb_acc_get_odr(pnpl_iis3dwb_acc_odr_t *enum_id)
{
  float odr = iis3dwb_acc_model.sensor_status->type.mems.odr;
  if(odr > 26666.0f && odr < 26668.0f)
  {
	  *enum_id = pnpl_iis3dwb_acc_odr_hz26667;
  }
  else
  {
	  return 1;
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_fs(pnpl_iis3dwb_acc_fs_t *enum_id)
{
  float fs = iis3dwb_acc_model.sensor_status->type.mems.fs;
  if (fs < 3.0f)
  {
	*enum_id = pnpl_iis3dwb_acc_fs_g2;
  }
  else if (fs < 5.0f)
  {
	*enum_id = pnpl_iis3dwb_acc_fs_g4;
  }
  else if (fs < 9.0f)
  {
	*enum_id = pnpl_iis3dwb_acc_fs_g8;
  }
  else if (fs < 15.0f)
  {
	*enum_id = pnpl_iis3dwb_acc_fs_g16;
  }
  else
  {
	  return 1;
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_enable(bool *value)
{
  *value = iis3dwb_acc_model.sensor_status->is_active;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_samples_per_ts(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.spts;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_dim(int32_t *value)
{
  *value = 3;
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_ioffset(float *value)
{
  *value = iis3dwb_acc_model.stream_params.ioffset;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_measodr(float *value)
{
  *value = iis3dwb_acc_model.sensor_status->type.mems.measured_odr;
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_usb_dps(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.usb_dps;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_sd_dps(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.sd_dps;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_sensitivity(float *value)
{
  *value = iis3dwb_acc_model.sensor_status->type.mems.sensitivity;
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_data_type(char **value)
{
  *value = "int16";
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_sensor_annotation(char **value)
{
  *value = iis3dwb_acc_model.annotation;
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_sensor_category(int32_t *value)
{
  *value = iis3dwb_acc_model.sensor_status->isensor_class;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_stream_id(int8_t *value)
{
  *value = iis3dwb_acc_model.stream_params.stream_id;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_get_ep_id(int8_t *value)
{
  *value = iis3dwb_acc_model.stream_params.usb_ep;
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}


uint8_t iis3dwb_acc_set_fs(pnpl_iis3dwb_acc_fs_t enum_id)
{
  float value;
  switch (enum_id)
  {
	case pnpl_iis3dwb_acc_fs_g2:
		value = 2.0f;
		break;
	case pnpl_iis3dwb_acc_fs_g4:
		value = 4.0f;
		break;
	case pnpl_iis3dwb_acc_fs_g8:
		value = 8.0f;
		break;
	case pnpl_iis3dwb_acc_fs_g16:
		value = 16.0f;
		break;
	default:
		return 1;
  }
  sys_error_code_t ret = SMSensorSetFS(iis3dwb_acc_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    /* USER Code */
  }
  return ret;
}

uint8_t iis3dwb_acc_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if (value)
  {
    ret = SMSensorEnable(iis3dwb_acc_model.id);
  }
  else
  {
    ret = SMSensorDisable(iis3dwb_acc_model.id);
  }
  if (ret == SYS_NO_ERROR_CODE)
  {
    /* USER Code */
    __stream_control(true);
  }
  return ret;
}

uint8_t iis3dwb_acc_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 1000;
  if (value >= min_v && value <= max_v)
  {
	iis3dwb_acc_model.stream_params.spts = value;
  }
  else if (value > max_v)
  {
	  iis3dwb_acc_model.stream_params.spts = max_v;
  }
  else
  {
	  iis3dwb_acc_model.stream_params.spts = min_v;
  }
  return PNPL_NO_ERROR_CODE;
}

uint8_t iis3dwb_acc_set_sensor_annotation(const char *value)
{
  strcpy(iis3dwb_acc_model.annotation, value);
  return PNPL_NO_ERROR_CODE;
}


