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
  * dtmi:dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC:actuators:slow_mc_telemetries;3
  *
  * Created by: DTDL2PnPL_cGen version 2.1.0
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
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_max(int32_t *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_initial_value(int32_t *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_format(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_enable(bool *value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_unit(char **value);
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_format(char **value);
uint8_t slow_mc_telemetries_get_stream_id(int8_t *value);
uint8_t slow_mc_telemetries_get_ep_id(int8_t *value);
uint8_t slow_mc_telemetries_set_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__id(int32_t value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_unit(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_format(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_unit(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_format(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_unit(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_format(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_max(int32_t value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_initial_value(int32_t value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_format(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_enable(bool value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_unit(const char *value, char **response_message);
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_format(const char *value, char **response_message);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_SLOW_MC_TELEMETRIES_H_ */
