/**
  ******************************************************************************
  * @file    App_model_Ism330dhcx_Acc.h
  * @author  SRA
  * @brief   Ism330dhcx_Acc PnPL Components APIs
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
  * dtmi:vespucci:steval_stwinbx1:fpSnsDatalog2_datalog2:sensors:ism330dhcx_acc;4
  *
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

/**
  ******************************************************************************
  * Component APIs *************************************************************
  * - Component init function
  *    <comp_name>_comp_init(void)
  * - Component get_key function
  *    <comp_name>_get_key(void)
  * - Component GET/SET Properties APIs ****************************************
  *  - GET Functions
  *    uint8_t <comp_name>_get_<prop_name>(prop_type *value)
  *      if prop_type == char --> (char **value)
  *  - SET Functions
  *    uint8_t <comp_name>_set_<prop_name>(prop_type value)
  *      if prop_type == char --> (char *value)
  *  - Component COMMAND Reaction Functions
  *      uint8_t <comp_name>_<command_name>(
  *                     field1_type field1_name, field2_type field2_name, ...,
  *                     fieldN_type fieldN_name);
  *  - Component TELEMETRY Send Functions
  *      uint8_t <comp_name>_create_telemetry(tel1_type tel1_name,
  *                     tel2_type tel2_name, ..., telN_type telN_name,
  *                     char **telemetry, uint32_t *size)
  ******************************************************************************
  */

#ifndef APP_MODEL_ISM330DHCX_ACC_H_
#define APP_MODEL_ISM330DHCX_ACC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Ism330dhcx_Acc_PnPL.h"

/* USER includes -------------------------------------------------------------*/

/* ISM330DHCX_ACC PnPL Component ---------------------------------------------- */
uint8_t ism330dhcx_acc_comp_init(void);
char *ism330dhcx_acc_get_key(void);
uint8_t ism330dhcx_acc_get_odr(pnpl_ism330dhcx_acc_odr_t *enum_id);
uint8_t ism330dhcx_acc_get_fs(pnpl_ism330dhcx_acc_fs_t *enum_id);
uint8_t ism330dhcx_acc_get_enable(bool *value);
uint8_t ism330dhcx_acc_get_samples_per_ts(int32_t *value);
uint8_t ism330dhcx_acc_get_dim(int32_t *value);
uint8_t ism330dhcx_acc_get_ioffset(float *value);
uint8_t ism330dhcx_acc_get_measodr(float *value);
uint8_t ism330dhcx_acc_get_usb_dps(int32_t *value);
uint8_t ism330dhcx_acc_get_sd_dps(int32_t *value);
uint8_t ism330dhcx_acc_get_sensitivity(float *value);
uint8_t ism330dhcx_acc_get_data_type(char **value);
uint8_t ism330dhcx_acc_get_sensor_annotation(char **value);
uint8_t ism330dhcx_acc_get_sensor_category(int32_t *value);
uint8_t ism330dhcx_acc_get_stream_id(int8_t *value);
uint8_t ism330dhcx_acc_get_ep_id(int8_t *value);
uint8_t ism330dhcx_acc_set_odr(pnpl_ism330dhcx_acc_odr_t enum_id);
uint8_t ism330dhcx_acc_set_fs(pnpl_ism330dhcx_acc_fs_t enum_id);
uint8_t ism330dhcx_acc_set_enable(bool value);
uint8_t ism330dhcx_acc_set_samples_per_ts(int32_t value);
uint8_t ism330dhcx_acc_set_sensor_annotation(const char *value);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_ISM330DHCX_ACC_H_ */