/**
  ******************************************************************************
  * @file    App_model_Ai_Motor_Classifier.h
  * @author  SRA
  * @brief   Ai_Motor_Classifier PnPL Components APIs
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
  * dtmi:appconfig:steval_stwinbx1:fpIndDatalogMC_datalog2MC_AI:algorithms:ai_motor_classifier;1
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

#ifndef APP_MODEL_AI_MOTOR_CLASSIFIER_H_
#define APP_MODEL_AI_MOTOR_CLASSIFIER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/

/* Private definition  -------------------------------------------------------*/
typedef enum
{
  NEAI_BLE_STREAM_CLASS_IDX = 0,
  NEAI_BLE_STREAM_PROBABILITY_IDX
} Neai_ble_stream_idx_t;

/* AI Motor Classifier PnPL Component ----------------------------------------- */
uint8_t ai_motor_classifier_comp_init(void);
char *ai_motor_classifier_get_key(void);
uint8_t ai_motor_classifier_get_algorithm_type(int32_t *value);
uint8_t ai_motor_classifier_get_enable(bool *value);
uint8_t ai_motor_classifier_get_usb_dps(int32_t *value);
uint8_t ai_motor_classifier_get_data_type(char **value);
uint8_t ai_motor_classifier_get_dim(int32_t *value);
uint8_t ai_motor_classifier_get_st_ble_stream__id(int32_t *value);
uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__enable(bool *value);
uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__unit(char **value);
uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__format(char **value);
uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__labels(char **value);
uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__enable(bool *value);
uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__unit(char **value);
uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__format(char **value);
uint8_t ai_motor_classifier_get_stream_id(int8_t *value);
uint8_t ai_motor_classifier_get_ep_id(int8_t *value);
uint8_t ai_motor_classifier_set_enable(bool value);
uint8_t ai_motor_classifier_set_st_ble_stream__id(int32_t value);
uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__enable(bool value);
uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__unit(const char *value);
uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__format(const char *value);
uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__labels(const char *value);
uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__enable(bool value);
uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__unit(const char *value);
uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__format(const char *value);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_AI_MOTOR_CLASSIFIER_H_ */
