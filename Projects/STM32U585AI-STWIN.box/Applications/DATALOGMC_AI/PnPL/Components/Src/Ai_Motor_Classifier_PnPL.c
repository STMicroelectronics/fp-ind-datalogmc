/**
  ******************************************************************************
  * @file    Ai_Motor_Classifier_PnPL.c
  * @author  SRA
  * @brief   Ai_Motor_Classifier PnPL Component Manager
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "App_model.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"
#include "PnPLCompManager.h"

#include "Ai_Motor_Classifier_PnPL.h"

static const IPnPLComponent_vtbl sAi_Motor_Classifier_PnPL_CompIF_vtbl =
{
  Ai_Motor_Classifier_PnPL_vtblGetKey,
  Ai_Motor_Classifier_PnPL_vtblGetNCommands,
  Ai_Motor_Classifier_PnPL_vtblGetCommandKey,
  Ai_Motor_Classifier_PnPL_vtblGetStatus,
  Ai_Motor_Classifier_PnPL_vtblSetProperty,
  Ai_Motor_Classifier_PnPL_vtblExecuteFunction
};

/**
  *  Ai_Motor_Classifier_PnPL internal structure.
  */
struct _Ai_Motor_Classifier_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Ai_Motor_Classifier_PnPL sAi_Motor_Classifier_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Ai_Motor_Classifier_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sAi_Motor_Classifier_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sAi_Motor_Classifier_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Ai_Motor_Classifier_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  ai_motor_classifier_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Ai_Motor_Classifier_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return ai_motor_classifier_get_key();
}

uint8_t Ai_Motor_Classifier_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Ai_Motor_Classifier_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Ai_Motor_Classifier_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                               uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  int32_t temp_i = 0;
  ai_motor_classifier_get_algorithm_type(&temp_i);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.algorithm_type", temp_i);
  bool temp_b = 0;
  ai_motor_classifier_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ai_motor_classifier.enable", temp_b);
  ai_motor_classifier_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.usb_dps", temp_i);
  char *temp_s = "";
  ai_motor_classifier_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.data_type", temp_s);
  ai_motor_classifier_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.dim", temp_i);
  ai_motor_classifier_get_st_ble_stream__id(&temp_i);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.st_ble_stream.id", temp_i);
  ai_motor_classifier_get_st_ble_stream__class_neai__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ai_motor_classifier.st_ble_stream.class_neai.enable", temp_b);
  ai_motor_classifier_get_st_ble_stream__class_neai__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.st_ble_stream.class_neai.unit", temp_s);
  ai_motor_classifier_get_st_ble_stream__class_neai__format(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.st_ble_stream.class_neai.format", temp_s);
  ai_motor_classifier_get_st_ble_stream__class_neai__labels(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.st_ble_stream.class_neai.labels", temp_s);
  ai_motor_classifier_get_st_ble_stream__probability_neai__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "ai_motor_classifier.st_ble_stream.probability_neai.enable", temp_b);
  ai_motor_classifier_get_st_ble_stream__probability_neai__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.st_ble_stream.probability_neai.unit", temp_s);
  ai_motor_classifier_get_st_ble_stream__probability_neai__format(&temp_s);
  json_object_dotset_string(JSON_Status, "ai_motor_classifier.st_ble_stream.probability_neai.format", temp_s);
  /* Next fields are not in DTDL model but added looking @ the component schema
  field (this is :sensors). ONLY for Sensors, Algorithms and Actuators*/
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.c_type", COMP_TYPE_ALGORITHM);
  int8_t temp_int8 = 0;
  ai_motor_classifier_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.stream_id", temp_int8);
  ai_motor_classifier_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "ai_motor_classifier.ep_id", temp_int8);

  if (pretty == 1)
  {
    *serializedJSON = json_serialize_to_string_pretty(tempJSON);
    *size = json_serialization_size_pretty(tempJSON);
  }
  else
  {
    *serializedJSON = json_serialize_to_string(tempJSON);
    *size = json_serialization_size(tempJSON);
  }

  /* No need to free temp_j as it is part of tempJSON */
  json_value_free(tempJSON);

  return PNPL_NO_ERROR_CODE;
}

uint8_t Ai_Motor_Classifier_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                 uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "ai_motor_classifier.enable");
    ret = ai_motor_classifier_set_enable(enable);
    if (ret == PNPL_NO_ERROR_CODE)
    {
      json_object_dotset_boolean(respJSONObject, "ai_motor_classifier.enable.value", enable);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "ai_motor_classifier.enable.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.id"))
    {
      int32_t st_ble_stream = (int32_t)json_object_dotget_number(tempJSONObject, "ai_motor_classifier.st_ble_stream.id");
      ret = ai_motor_classifier_set_st_ble_stream__id(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_number(respJSONObject, "ai_motor_classifier.st_ble_stream.id.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.id.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.enable");
      ret = ai_motor_classifier_set_st_ble_stream__class_neai__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.enable.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "ai_motor_classifier.st_ble_stream.class_neai.unit");
      ret = ai_motor_classifier_set_st_ble_stream__class_neai__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "ai_motor_classifier.st_ble_stream.class_neai.format");
      ret = ai_motor_classifier_set_st_ble_stream__class_neai__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.format.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.labels"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "ai_motor_classifier.st_ble_stream.class_neai.labels");
      ret = ai_motor_classifier_set_st_ble_stream__class_neai__labels(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.labels.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.class_neai.labels.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject,
                                                      "ai_motor_classifier.st_ble_stream.probability_neai.enable");
      ret = ai_motor_classifier_set_st_ble_stream__probability_neai__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.enable.value",
                                   st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.enable.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "ai_motor_classifier.st_ble_stream.probability_neai.unit");
      ret = ai_motor_classifier_set_st_ble_stream__probability_neai__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.unit.value",
                                  st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.unit.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "ai_motor_classifier.st_ble_stream.probability_neai.format");
      ret = ai_motor_classifier_set_st_ble_stream__probability_neai__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.format.value",
                                  st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "ai_motor_classifier.st_ble_stream.probability_neai.format.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  json_value_free(tempJSON);
  if (pretty == 1)
  {
    *response = json_serialize_to_string_pretty(respJSON);
    *size = json_serialization_size_pretty(respJSON);
  }
  else
  {
    *response = json_serialize_to_string(respJSON);
    *size = json_serialization_size(respJSON);
  }
  json_value_free(respJSON);
  return ret;
}


uint8_t Ai_Motor_Classifier_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                     uint32_t *size, uint8_t pretty)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

