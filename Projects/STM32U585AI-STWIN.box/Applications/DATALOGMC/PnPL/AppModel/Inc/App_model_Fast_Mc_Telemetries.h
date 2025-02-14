/**
  ******************************************************************************
  * @file    App_model_Fast_Mc_Telemetries.h
  * @author  SRA
  * @brief   Fast_Mc_Telemetries PnPL Components APIs
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC:actuators:fast_mc_telemetries;2
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

#ifndef APP_MODEL_FAST_MC_TELEMETRIES_H_
#define APP_MODEL_FAST_MC_TELEMETRIES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/

/* Fast Motor Telemetries  PnPL Component ------------------------------------- */
uint8_t fast_mc_telemetries_comp_init(void);
char *fast_mc_telemetries_get_key(void);
uint8_t fast_mc_telemetries_get_usb_dps(int32_t *value);
uint8_t fast_mc_telemetries_get_sd_dps(int32_t *value);
uint8_t fast_mc_telemetries_get_enable(bool *value);
uint8_t fast_mc_telemetries_get_data_type(char **value);
uint8_t fast_mc_telemetries_get_dim(int32_t *value);
uint8_t fast_mc_telemetries_get_i_q__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_q__unit(char **value);
uint8_t fast_mc_telemetries_get_i_d__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_d__unit(char **value);
uint8_t fast_mc_telemetries_get_i_q_ref__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_q_ref__unit(char **value);
uint8_t fast_mc_telemetries_get_i_d_ref__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_d_ref__unit(char **value);
uint8_t fast_mc_telemetries_get_v_q__enabled(bool *value);
uint8_t fast_mc_telemetries_get_v_q__unit(char **value);
uint8_t fast_mc_telemetries_get_v_d__enabled(bool *value);
uint8_t fast_mc_telemetries_get_v_d__unit(char **value);
uint8_t fast_mc_telemetries_get_i_a__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_a__unit(char **value);
uint8_t fast_mc_telemetries_get_i_b__enabled(bool *value);
uint8_t fast_mc_telemetries_get_i_b__unit(char **value);
uint8_t fast_mc_telemetries_get_v_a__enabled(bool *value);
uint8_t fast_mc_telemetries_get_v_a__unit(char **value);
uint8_t fast_mc_telemetries_get_v_b__enabled(bool *value);
uint8_t fast_mc_telemetries_get_v_b__unit(char **value);
uint8_t fast_mc_telemetries_get_sensitivity__voltage(float *value);
uint8_t fast_mc_telemetries_get_sensitivity__current(float *value);
uint8_t fast_mc_telemetries_get_sensitivity__frequency(float *value);
uint8_t fast_mc_telemetries_get_stream_id(int8_t *value);
uint8_t fast_mc_telemetries_get_ep_id(int8_t *value);
uint8_t fast_mc_telemetries_set_enable(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_q__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_q__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_i_d__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_d__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_i_q_ref__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_q_ref__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_i_d_ref__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_d_ref__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_v_q__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_v_q__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_v_d__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_v_d__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_i_a__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_a__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_i_b__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_i_b__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_v_a__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_v_a__unit(const char *value, char **response_message);
uint8_t fast_mc_telemetries_set_v_b__enabled(bool value, char **response_message);
uint8_t fast_mc_telemetries_set_v_b__unit(const char *value, char **response_message);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_FAST_MC_TELEMETRIES_H_ */
