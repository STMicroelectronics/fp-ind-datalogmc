/**
  ******************************************************************************
  * @file    Slow_Mc_Telemetries_PnPL.c
  * @author  SRA
  * @brief   Slow_Mc_Telemetries PnPL Component Manager
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC:actuators:slow_mc_telemetries;4
  *
  * Created by: DTDL2PnPL_cGen version 2.1.0
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

#include "Slow_Mc_Telemetries_PnPL.h"

static const IPnPLComponent_vtbl sSlow_Mc_Telemetries_PnPL_CompIF_vtbl =
{
  Slow_Mc_Telemetries_PnPL_vtblGetKey,
  Slow_Mc_Telemetries_PnPL_vtblGetNCommands,
  Slow_Mc_Telemetries_PnPL_vtblGetCommandKey,
  Slow_Mc_Telemetries_PnPL_vtblGetStatus,
  Slow_Mc_Telemetries_PnPL_vtblSetProperty,
  Slow_Mc_Telemetries_PnPL_vtblExecuteFunction
};

/**
  *  Slow_Mc_Telemetries_PnPL internal structure.
  */
struct _Slow_Mc_Telemetries_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Slow_Mc_Telemetries_PnPL sSlow_Mc_Telemetries_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Slow_Mc_Telemetries_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sSlow_Mc_Telemetries_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sSlow_Mc_Telemetries_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Slow_Mc_Telemetries_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  slow_mc_telemetries_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Slow_Mc_Telemetries_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return slow_mc_telemetries_get_key();
}

uint8_t Slow_Mc_Telemetries_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Slow_Mc_Telemetries_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Slow_Mc_Telemetries_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                               uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  int32_t temp_i = 0;
  slow_mc_telemetries_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.usb_dps", temp_i);
  slow_mc_telemetries_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.sd_dps", temp_i);
  bool temp_b = 0;
  slow_mc_telemetries_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.enable", temp_b);
  char *temp_s = "";
  slow_mc_telemetries_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.data_type", temp_s);
  slow_mc_telemetries_get_st_ble_stream__id(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.st_ble_stream.id", temp_i);
  slow_mc_telemetries_get_st_ble_stream__temperature_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__temperature_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__temperature_format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__ref_speed_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__ref_speed_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__ref_speed_format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__speed_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__speed_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__speed_format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__speed_max(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.max", temp_i);
  slow_mc_telemetries_get_st_ble_stream__speed_initial_value(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.initial_value", temp_i);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage_format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__fault_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__fault_unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__fault_format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.format", temp_s);
  slow_mc_telemetries_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.dim", temp_i);
  slow_mc_telemetries_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.samples_per_ts", temp_i);
  float_t temp_f = 0;
  slow_mc_telemetries_get_odr(&temp_f);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.odr", temp_f);
  slow_mc_telemetries_get_ioffset(&temp_f);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.ioffset", temp_f);
  /* Next fields are not in DTDL model but added looking @ the component schema
  field (this is :sensors). ONLY for Sensors, Algorithms and Actuators*/
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.c_type", COMP_TYPE_ACTUATOR);
  int8_t temp_int8 = 0;
  slow_mc_telemetries_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.stream_id", temp_int8);
  slow_mc_telemetries_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "slow_mc_telemetries.ep_id", temp_int8);

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

uint8_t Slow_Mc_Telemetries_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                 uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  bool valid_property = false;
  char *resp_msg;
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.enable");
    valid_property = true;
    ret = slow_mc_telemetries_set_enable(enable, &resp_msg);
    json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
    if (ret == PNPL_NO_ERROR_CODE)
    {
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
    }
    else
    {
      bool old_enable;
      slow_mc_telemetries_get_enable(&old_enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.id"))
    {
      int32_t st_ble_stream__id = (int32_t)json_object_dotget_number(tempJSONObject, "slow_mc_telemetries.st_ble_stream.id");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__id(st_ble_stream__id, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", st_ble_stream__id);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        int32_t old_st_ble_stream__id;
        slow_mc_telemetries_get_st_ble_stream__id(&old_st_ble_stream__id);
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", old_st_ble_stream__id);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.enable"))
    {
      bool st_ble_stream__temperature_enable = json_object_dotget_boolean(tempJSONObject,
                                                                          "slow_mc_telemetries.st_ble_stream.temperature.enable");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__temperature_enable(st_ble_stream__temperature_enable, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", st_ble_stream__temperature_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_st_ble_stream__temperature_enable;
        slow_mc_telemetries_get_st_ble_stream__temperature_enable(&old_st_ble_stream__temperature_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_st_ble_stream__temperature_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.unit"))
    {
      const char *st_ble_stream__temperature_unit = json_object_dotget_string(tempJSONObject,
                                                                              "slow_mc_telemetries.st_ble_stream.temperature.unit");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__temperature_unit(st_ble_stream__temperature_unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__temperature_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__temperature_unit;
        slow_mc_telemetries_get_st_ble_stream__temperature_unit(&old_st_ble_stream__temperature_unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__temperature_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.format"))
    {
      const char *st_ble_stream__temperature_format = json_object_dotget_string(tempJSONObject,
                                                                                "slow_mc_telemetries.st_ble_stream.temperature.format");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__temperature_format(st_ble_stream__temperature_format, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__temperature_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__temperature_format;
        slow_mc_telemetries_get_st_ble_stream__temperature_format(&old_st_ble_stream__temperature_format);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__temperature_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.enable"))
    {
      bool st_ble_stream__ref_speed_enable = json_object_dotget_boolean(tempJSONObject,
                                                                        "slow_mc_telemetries.st_ble_stream.ref_speed.enable");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(st_ble_stream__ref_speed_enable, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", st_ble_stream__ref_speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_st_ble_stream__ref_speed_enable;
        slow_mc_telemetries_get_st_ble_stream__ref_speed_enable(&old_st_ble_stream__ref_speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_st_ble_stream__ref_speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.unit"))
    {
      const char *st_ble_stream__ref_speed_unit = json_object_dotget_string(tempJSONObject,
                                                                            "slow_mc_telemetries.st_ble_stream.ref_speed.unit");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed_unit(st_ble_stream__ref_speed_unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__ref_speed_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__ref_speed_unit;
        slow_mc_telemetries_get_st_ble_stream__ref_speed_unit(&old_st_ble_stream__ref_speed_unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__ref_speed_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.format"))
    {
      const char *st_ble_stream__ref_speed_format = json_object_dotget_string(tempJSONObject,
                                                                              "slow_mc_telemetries.st_ble_stream.ref_speed.format");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed_format(st_ble_stream__ref_speed_format, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__ref_speed_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__ref_speed_format;
        slow_mc_telemetries_get_st_ble_stream__ref_speed_format(&old_st_ble_stream__ref_speed_format);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__ref_speed_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.enable"))
    {
      bool st_ble_stream__speed_enable = json_object_dotget_boolean(tempJSONObject,
                                                                    "slow_mc_telemetries.st_ble_stream.speed.enable");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__speed_enable(st_ble_stream__speed_enable, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", st_ble_stream__speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_st_ble_stream__speed_enable;
        slow_mc_telemetries_get_st_ble_stream__speed_enable(&old_st_ble_stream__speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_st_ble_stream__speed_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.unit"))
    {
      const char *st_ble_stream__speed_unit = json_object_dotget_string(tempJSONObject,
                                                                        "slow_mc_telemetries.st_ble_stream.speed.unit");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__speed_unit(st_ble_stream__speed_unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__speed_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__speed_unit;
        slow_mc_telemetries_get_st_ble_stream__speed_unit(&old_st_ble_stream__speed_unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__speed_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.format"))
    {
      const char *st_ble_stream__speed_format = json_object_dotget_string(tempJSONObject,
                                                                          "slow_mc_telemetries.st_ble_stream.speed.format");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__speed_format(st_ble_stream__speed_format, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__speed_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__speed_format;
        slow_mc_telemetries_get_st_ble_stream__speed_format(&old_st_ble_stream__speed_format);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__speed_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.max"))
    {
      int32_t st_ble_stream__speed_max = (int32_t)json_object_dotget_number(tempJSONObject,
                                                                            "slow_mc_telemetries.st_ble_stream.speed.max");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__speed_max(st_ble_stream__speed_max, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", st_ble_stream__speed_max);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        int32_t old_st_ble_stream__speed_max;
        slow_mc_telemetries_get_st_ble_stream__speed_max(&old_st_ble_stream__speed_max);
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", old_st_ble_stream__speed_max);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.initial_value"))
    {
      int32_t st_ble_stream__speed_initial_value = (int32_t)json_object_dotget_number(tempJSONObject,
                                                                                      "slow_mc_telemetries.st_ble_stream.speed.initial_value");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__speed_initial_value(st_ble_stream__speed_initial_value, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", st_ble_stream__speed_initial_value);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        int32_t old_st_ble_stream__speed_initial_value;
        slow_mc_telemetries_get_st_ble_stream__speed_initial_value(&old_st_ble_stream__speed_initial_value);
        json_object_dotset_number(respJSONObject, "PnPL_Response.value", old_st_ble_stream__speed_initial_value);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable"))
    {
      bool st_ble_stream__bus_voltage_enable = json_object_dotget_boolean(tempJSONObject,
                                                                          "slow_mc_telemetries.st_ble_stream.bus_voltage.enable");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(st_ble_stream__bus_voltage_enable, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", st_ble_stream__bus_voltage_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_st_ble_stream__bus_voltage_enable;
        slow_mc_telemetries_get_st_ble_stream__bus_voltage_enable(&old_st_ble_stream__bus_voltage_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_st_ble_stream__bus_voltage_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit"))
    {
      const char *st_ble_stream__bus_voltage_unit = json_object_dotget_string(tempJSONObject,
                                                                              "slow_mc_telemetries.st_ble_stream.bus_voltage.unit");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit(st_ble_stream__bus_voltage_unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__bus_voltage_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__bus_voltage_unit;
        slow_mc_telemetries_get_st_ble_stream__bus_voltage_unit(&old_st_ble_stream__bus_voltage_unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__bus_voltage_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.format"))
    {
      const char *st_ble_stream__bus_voltage_format = json_object_dotget_string(tempJSONObject,
                                                                                "slow_mc_telemetries.st_ble_stream.bus_voltage.format");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage_format(st_ble_stream__bus_voltage_format, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__bus_voltage_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__bus_voltage_format;
        slow_mc_telemetries_get_st_ble_stream__bus_voltage_format(&old_st_ble_stream__bus_voltage_format);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__bus_voltage_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.enable"))
    {
      bool st_ble_stream__fault_enable = json_object_dotget_boolean(tempJSONObject,
                                                                    "slow_mc_telemetries.st_ble_stream.fault.enable");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__fault_enable(st_ble_stream__fault_enable, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", st_ble_stream__fault_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_st_ble_stream__fault_enable;
        slow_mc_telemetries_get_st_ble_stream__fault_enable(&old_st_ble_stream__fault_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_st_ble_stream__fault_enable);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.unit"))
    {
      const char *st_ble_stream__fault_unit = json_object_dotget_string(tempJSONObject,
                                                                        "slow_mc_telemetries.st_ble_stream.fault.unit");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__fault_unit(st_ble_stream__fault_unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__fault_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__fault_unit;
        slow_mc_telemetries_get_st_ble_stream__fault_unit(&old_st_ble_stream__fault_unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__fault_unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.format"))
    {
      const char *st_ble_stream__fault_format = json_object_dotget_string(tempJSONObject,
                                                                          "slow_mc_telemetries.st_ble_stream.fault.format");
      valid_property = true;
      ret = slow_mc_telemetries_set_st_ble_stream__fault_format(st_ble_stream__fault_format, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", st_ble_stream__fault_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_st_ble_stream__fault_format;
        slow_mc_telemetries_get_st_ble_stream__fault_format(&old_st_ble_stream__fault_format);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_st_ble_stream__fault_format);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  json_value_free(tempJSON);
  /* Check if received a valid request to modify an existing property */
  if (valid_property)
  {
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
  }
  else
  {
    /* Set property is not containing a valid property/parameter: PnPL_Error */
    char *log_message = "Invalid property for slow_mc_telemetries";
    PnPLCreateLogMessage(response, size, log_message, PNPL_LOG_ERROR);
    ret = PNPL_BASE_ERROR_CODE;
  }
  json_value_free(respJSON);
  return ret;
}


uint8_t Slow_Mc_Telemetries_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                     uint32_t *size, uint8_t pretty)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

