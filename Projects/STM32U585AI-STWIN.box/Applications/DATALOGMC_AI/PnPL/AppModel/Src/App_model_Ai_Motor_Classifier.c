/**
  ******************************************************************************
  * @file    App_model_Ai_Motor_Classifier.c
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

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/

/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/
/* Motor Classifier definition */
#define AI_MOTOR_CLASSIFIER_DIMENSION 5u
#define AI_MOTOR_CLASSIFIER_ALGORITHM_TYPE 2u
#define AI_MOTOR_CLASSIFIER_SPTS 1
#define AI_MOTOR_CLASSIFIER_DPS  (STREAM_TIMESTAMP_DIM + AI_MOTOR_CLASSIFIER_DIMENSION)

static char ai_motor_classifier_enum_string[] = "[{\"value\":0,\"label\":\"normal\"},"
                                                 "{\"value\":1,\"label\":\"fault\"}]";

/* AI Motor Classifier PnPL Component ----------------------------------------*/
static AlgorithmModel_t ai_motor_classifier_model;
static StBleStreamModel_t neai_ble_stream_model;

extern AppModel_t app_model;

uint8_t ai_motor_classifier_comp_init(void)
{
  ai_motor_classifier_model.comp_name = ai_motor_classifier_get_key();

  ai_motor_classifier_model.stream_params.stream_id = 4;
  ai_motor_classifier_model.stream_params.usb_ep = 4;
  ai_motor_classifier_model.stream_params.spts = AI_MOTOR_CLASSIFIER_SPTS;
  ai_motor_classifier_model.stream_params.usb_dps = AI_MOTOR_CLASSIFIER_DPS;
  ai_motor_classifier_model.enable = true;
  app_model.a_models[0] = &ai_motor_classifier_model;

  ai_motor_classifier_set_st_ble_stream__id(2);
  ai_motor_classifier_set_st_ble_stream__class_neai__enable(true);
  ai_motor_classifier_set_st_ble_stream__class_neai__format("enum");
  ai_motor_classifier_set_st_ble_stream__probability_neai__enable(true);
  ai_motor_classifier_set_st_ble_stream__probability_neai__format("float");

  __stream_control(true);

  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}
char* ai_motor_classifier_get_key(void)
{
  return "ai_motor_classifier";
}

uint8_t ai_motor_classifier_get_algorithm_type(int32_t *value)
{
  *value = AI_MOTOR_CLASSIFIER_ALGORITHM_TYPE;
  return PNPL_NO_ERROR_CODE;
}
uint8_t ai_motor_classifier_get_enable(bool *value)
{
  *value = ai_motor_classifier_model.enable;
  return PNPL_NO_ERROR_CODE;
}
uint8_t ai_motor_classifier_get_usb_dps(int32_t *value)
{
  *value = ai_motor_classifier_model.stream_params.usb_dps;
  return PNPL_NO_ERROR_CODE;
}
uint8_t ai_motor_classifier_get_data_type(char **value)
{
  *value = "uint8_t";
  return PNPL_NO_ERROR_CODE;
}
uint8_t ai_motor_classifier_get_dim(int32_t *value)
{
  *value = AI_MOTOR_CLASSIFIER_DIMENSION;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__id(int32_t *value)
{
  *value = neai_ble_stream_model.st_ble_stream_id;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__enable(bool *value)
{
  *value = neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_CLASS_IDX].status;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__unit(char **value)
{
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__format(char **value)
{
  *value = neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_CLASS_IDX].format;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__enable(bool *value)
{
  *value = neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_PROBABILITY_IDX].status;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__unit(char **value)
{
  *value = "";
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__probability_neai__format(char **value)
{
  *value = neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_PROBABILITY_IDX].format;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_st_ble_stream__class_neai__labels(char **value)
{
  *value = ai_motor_classifier_enum_string;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_get_stream_id(int8_t *value)
{
  *value = ai_motor_classifier_model.stream_params.stream_id;
  return 0;
}
uint8_t ai_motor_classifier_get_ep_id(int8_t *value)
{
  *value = ai_motor_classifier_model.stream_params.usb_ep;
  return 0;
}
uint8_t ai_motor_classifier_set_enable(bool value)
{
  ai_motor_classifier_model.enable = value;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__enable(bool value)
{
  neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_CLASS_IDX].status = value;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__unit(const char *value)
{
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__format(const char *value)
{
  strcpy(neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_CLASS_IDX].format, value);
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__enable(bool value)
{
  neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_PROBABILITY_IDX].status = value;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__unit(const char *value)
{
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__probability_neai__format(const char *value)
{
  strcpy(neai_ble_stream_model.st_ble_stream_objects[NEAI_BLE_STREAM_PROBABILITY_IDX].format, value);
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__id(int32_t value)
{
  /* USER Code */
  neai_ble_stream_model.st_ble_stream_id = value;
  return PNPL_NO_ERROR_CODE;
}

uint8_t ai_motor_classifier_set_st_ble_stream__class_neai__labels(const char *value)
{
  /* USER Code */
  return PNPL_NO_ERROR_CODE;
}




