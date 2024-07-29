/**
  ******************************************************************************
  * @file    App_model_Slow_Mc_Telemetries.h
  * @author  SRA
  * @brief   Slow_Mc_Telemetries PnPL Components APIs
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:slow_mc_telemetries;2
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

#ifndef APP_MODEL_SLOW_MC_TELEMETRIES_H_
#define APP_MODEL_SLOW_MC_TELEMETRIES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/

/* Slow Motor Telemetries PnPL Component -------------------------------------- */
uint8_t slow_mc_telemetries_comp_init(void);
char *slow_mc_telemetries_get_key(void);
uint8_t slow_mc_telemetries_get_usb_dps(int32_t *value);
uint8_t slow_mc_telemetries_get_sd_dps(int32_t *value);
uint8_t slow_mc_telemetries_get_enable(bool *value);
uint8_t slow_mc_telemetries_get_data_type(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__id(int32_t *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature__enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature__unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature__format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed__enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed__unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed__format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed__enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed__unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed__format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage__enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage__unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage__format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault__enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault__unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault__format(char **value);
uint8_t slow_mc_telemetries_get_stream_id(int8_t *value);
uint8_t slow_mc_telemetries_get_ep_id(int8_t *value);
uint8_t slow_mc_telemetries_set_enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__id(int32_t value);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature__enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature__unit(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature__format(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed__enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed__unit(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed__format(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed__enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed__unit(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed__format(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage__enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage__unit(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage__format(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault__enable(bool value);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault__unit(const char *value);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault__format(const char *value);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_SLOW_MC_TELEMETRIES_H_ */
