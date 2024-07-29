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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:slow_mc_telemetries;2
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
  slow_mc_telemetries_get_st_ble_stream__temperature__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__temperature__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__temperature__format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.temperature.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__ref_speed__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__ref_speed__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__ref_speed__format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.ref_speed.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__speed__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__speed__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__speed__format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.speed.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__bus_voltage__format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.bus_voltage.format", temp_s);
  slow_mc_telemetries_get_st_ble_stream__fault__enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.enable", temp_b);
  slow_mc_telemetries_get_st_ble_stream__fault__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.unit", temp_s);
  slow_mc_telemetries_get_st_ble_stream__fault__format(&temp_s);
  json_object_dotset_string(JSON_Status, "slow_mc_telemetries.st_ble_stream.fault.format", temp_s);
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
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.enable");
    ret = slow_mc_telemetries_set_enable(enable);
    if (ret == PNPL_NO_ERROR_CODE)
    {
      json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.enable.value", enable);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "slow_mc_telemetries.enable.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.id"))
    {
      int32_t st_ble_stream = (int32_t)json_object_dotget_number(tempJSONObject, "slow_mc_telemetries.st_ble_stream.id");
      ret = slow_mc_telemetries_set_st_ble_stream__id(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_number(respJSONObject, "slow_mc_telemetries.st_ble_stream.id.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.id.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.enable");
      ret = slow_mc_telemetries_set_st_ble_stream__temperature__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.enable.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.temperature.unit");
      ret = slow_mc_telemetries_set_st_ble_stream__temperature__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.temperature.format");
      ret = slow_mc_telemetries_set_st_ble_stream__temperature__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.temperature.format.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.enable");
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.enable.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.ref_speed.unit");
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.ref_speed.format");
      ret = slow_mc_telemetries_set_st_ble_stream__ref_speed__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.ref_speed.format.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.enable");
      ret = slow_mc_telemetries_set_st_ble_stream__speed__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.enable.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.unit");
      ret = slow_mc_telemetries_set_st_ble_stream__speed__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject, "slow_mc_telemetries.st_ble_stream.speed.format");
      ret = slow_mc_telemetries_set_st_ble_stream__speed__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.speed.format.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable");
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.enable.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.bus_voltage.unit");
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject,
                                                            "slow_mc_telemetries.st_ble_stream.bus_voltage.format");
      ret = slow_mc_telemetries_set_st_ble_stream__bus_voltage__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.bus_voltage.format.value",
                                  "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.enable"))
    {
      bool st_ble_stream = json_object_dotget_boolean(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.enable");
      ret = slow_mc_telemetries_set_st_ble_stream__fault__enable(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.enable.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.enable.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.unit"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.unit");
      ret = slow_mc_telemetries_set_st_ble_stream__fault__unit(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.unit.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream"))
  {
    if (json_object_dothas_value(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.format"))
    {
      const char *st_ble_stream = json_object_dotget_string(tempJSONObject, "slow_mc_telemetries.st_ble_stream.fault.format");
      ret = slow_mc_telemetries_set_st_ble_stream__fault__format(st_ble_stream);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.format.value", st_ble_stream);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "slow_mc_telemetries.st_ble_stream.fault.format.value", "PNPL_SET_ERROR");
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


uint8_t Slow_Mc_Telemetries_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                     uint32_t *size, uint8_t pretty)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

