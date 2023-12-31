/**
  ******************************************************************************
  * @file    Stts22h_Temp_PnPL.c
  * @author  SRA
  * @brief   Stts22h_Temp PnPL Component Manager
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
  * dtmi:vespucci:steval_stwinbx1:fpSnsDatalog2_datalog2:sensors:stts22h_temp;3
  *
  * Created by: DTDL2PnPL_cGen version 1.2.0
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

#include "Stts22h_Temp_PnPL.h"
#include "Stts22h_Temp_PnPL_vtbl.h"

static const IPnPLComponent_vtbl sStts22h_Temp_PnPL_CompIF_vtbl =
{
  Stts22h_Temp_PnPL_vtblGetKey,
  Stts22h_Temp_PnPL_vtblGetNCommands,
  Stts22h_Temp_PnPL_vtblGetCommandKey,
  Stts22h_Temp_PnPL_vtblGetStatus,
  Stts22h_Temp_PnPL_vtblSetProperty,
  Stts22h_Temp_PnPL_vtblExecuteFunction
};

/**
  *  Stts22h_Temp_PnPL internal structure.
  */
struct _Stts22h_Temp_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;

};

/* Objects instance ----------------------------------------------------------*/
static Stts22h_Temp_PnPL sStts22h_Temp_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Stts22h_Temp_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sStts22h_Temp_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sStts22h_Temp_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Stts22h_Temp_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  stts22h_temp_comp_init();
  return 0;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Stts22h_Temp_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return stts22h_temp_get_key();
}

uint8_t Stts22h_Temp_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Stts22h_Temp_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Stts22h_Temp_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  int32_t temp_i = 0;
  stts22h_temp_get_odr(&temp_i);
  uint8_t enum_id = 0;
  if(temp_i == stts22h_temp_odr_hz1)
  {
    enum_id = 0;
  }
  else if(temp_i == stts22h_temp_odr_hz25)
  {
    enum_id = 1;
  }
  else if(temp_i == stts22h_temp_odr_hz50)
  {
    enum_id = 2;
  }
  else if(temp_i == stts22h_temp_odr_hz100)
  {
    enum_id = 3;
  }
  else if(temp_i == stts22h_temp_odr_hz200)
  {
    enum_id = 4;
  }
  json_object_dotset_number(JSON_Status, "stts22h_temp.odr", enum_id);
  stts22h_temp_get_fs(&temp_i);
  enum_id = 0;
  if(temp_i == stts22h_temp_fs_cdeg100)
  {
    enum_id = 0;
  }
  json_object_dotset_number(JSON_Status, "stts22h_temp.fs", enum_id);
  bool temp_b = 0;
  stts22h_temp_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "stts22h_temp.enable", temp_b);
  stts22h_temp_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "stts22h_temp.samples_per_ts", temp_i);
  stts22h_temp_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "stts22h_temp.dim", temp_i);
  float temp_f = 0;
  stts22h_temp_get_ioffset(&temp_f);
  json_object_dotset_number(JSON_Status, "stts22h_temp.ioffset", temp_f);
  stts22h_temp_get_measodr(&temp_f);
  json_object_dotset_number(JSON_Status, "stts22h_temp.measodr", temp_f);
  stts22h_temp_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "stts22h_temp.usb_dps", temp_i);
  stts22h_temp_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "stts22h_temp.sd_dps", temp_i);
  stts22h_temp_get_sensitivity(&temp_f);
  json_object_dotset_number(JSON_Status, "stts22h_temp.sensitivity", temp_f);
  char *temp_s = "";
  stts22h_temp_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "stts22h_temp.data_type", temp_s);
  stts22h_temp_get_sensor_annotation(&temp_s);
  json_object_dotset_string(JSON_Status, "stts22h_temp.sensor_annotation", temp_s);
  stts22h_temp_get_sensor_category(&temp_i);
  json_object_dotset_number(JSON_Status, "stts22h_temp.sensor_category", temp_i);
  /* Next fields are not in DTDL model but added looking @ the component schema
  field (this is :sensors). ONLY for Sensors, Algorithms and Actuators*/
  json_object_dotset_number(JSON_Status, "stts22h_temp.c_type", COMP_TYPE_SENSOR);
  int8_t temp_int8 = 0;
  stts22h_temp_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "stts22h_temp.stream_id", temp_int8);
  stts22h_temp_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "stts22h_temp.ep_id", temp_int8);

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

  return 0;
}

uint8_t Stts22h_Temp_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = 0;
  if(json_object_dothas_value(tempJSONObject, "stts22h_temp.odr"))
  {
    int odr = (int)json_object_dotget_number(tempJSONObject, "stts22h_temp.odr");
    switch(odr)
    {
    case 0:
      ret = stts22h_temp_set_odr(stts22h_temp_odr_hz1);
      break;
    case 1:
      ret = stts22h_temp_set_odr(stts22h_temp_odr_hz25);
      break;
    case 2:
      ret = stts22h_temp_set_odr(stts22h_temp_odr_hz50);
      break;
    case 3:
      ret = stts22h_temp_set_odr(stts22h_temp_odr_hz100);
      break;
    case 4:
      ret = stts22h_temp_set_odr(stts22h_temp_odr_hz200);
      break;
    }
    if(ret == 0)
    {
      json_object_dotset_number(respJSONObject, "stts22h_temp.odr.value",odr);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "stts22h_temp.odr.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "stts22h_temp.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "stts22h_temp.enable");
    ret =stts22h_temp_set_enable(enable);
    if(ret == 0)
    {
      json_object_dotset_boolean(respJSONObject, "stts22h_temp.enable.value",enable);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "stts22h_temp.enable.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "stts22h_temp.samples_per_ts"))
  {
    int32_t samples_per_ts =(int32_t) json_object_dotget_number(tempJSONObject, "stts22h_temp.samples_per_ts");
    ret = stts22h_temp_set_samples_per_ts(samples_per_ts);
    if(ret == 0)
    {
      json_object_dotset_number(respJSONObject, "stts22h_temp.samples_per_ts.value",samples_per_ts);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "stts22h_temp.samples_per_ts.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "stts22h_temp.sensor_annotation"))
  {
    const char *sensor_annotation = json_object_dotget_string(tempJSONObject, "stts22h_temp.sensor_annotation");
    ret =stts22h_temp_set_sensor_annotation(sensor_annotation);
    if(ret == 0)
    {
      json_object_dotset_string(respJSONObject, "stts22h_temp.sensor_annotation.value",sensor_annotation);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "stts22h_temp.sensor_annotation.value", "PNPL_SET_ERROR");
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


uint8_t Stts22h_Temp_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response, uint32_t *size, uint8_t pretty)
{
  return 1;
}

