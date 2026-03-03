/**
  ******************************************************************************
  * @file    Fast_Mc_Telemetries_PnPL.c
  * @author  SRA
  * @brief   Fast_Mc_Telemetries PnPL Component Manager
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC:actuators:fast_mc_telemetries;3
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

#include "Fast_Mc_Telemetries_PnPL.h"

static const IPnPLComponent_vtbl sFast_Mc_Telemetries_PnPL_CompIF_vtbl =
{
  Fast_Mc_Telemetries_PnPL_vtblGetKey,
  Fast_Mc_Telemetries_PnPL_vtblGetNCommands,
  Fast_Mc_Telemetries_PnPL_vtblGetCommandKey,
  Fast_Mc_Telemetries_PnPL_vtblGetStatus,
  Fast_Mc_Telemetries_PnPL_vtblSetProperty,
  Fast_Mc_Telemetries_PnPL_vtblExecuteFunction
};

/**
  *  Fast_Mc_Telemetries_PnPL internal structure.
  */
struct _Fast_Mc_Telemetries_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Fast_Mc_Telemetries_PnPL sFast_Mc_Telemetries_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Fast_Mc_Telemetries_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sFast_Mc_Telemetries_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sFast_Mc_Telemetries_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Fast_Mc_Telemetries_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  fast_mc_telemetries_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Fast_Mc_Telemetries_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return fast_mc_telemetries_get_key();
}

uint8_t Fast_Mc_Telemetries_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 0;
}

char *Fast_Mc_Telemetries_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  return "";
}

uint8_t Fast_Mc_Telemetries_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                               uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  int32_t temp_i = 0;
  fast_mc_telemetries_get_usb_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.usb_dps", temp_i);
  fast_mc_telemetries_get_sd_dps(&temp_i);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.sd_dps", temp_i);
  bool temp_b = 0;
  fast_mc_telemetries_get_enable(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.enable", temp_b);
  char *temp_s = "";
  fast_mc_telemetries_get_data_type(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.data_type", temp_s);
  fast_mc_telemetries_get_dim(&temp_i);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.dim", temp_i);
  fast_mc_telemetries_get_i_q__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_q.enabled", temp_b);
  fast_mc_telemetries_get_i_q__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_q.unit", temp_s);
  fast_mc_telemetries_get_i_d__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_d.enabled", temp_b);
  fast_mc_telemetries_get_i_d__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_d.unit", temp_s);
  fast_mc_telemetries_get_i_q_ref__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_q_ref.enabled", temp_b);
  fast_mc_telemetries_get_i_q_ref__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_q_ref.unit", temp_s);
  fast_mc_telemetries_get_i_d_ref__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_d_ref.enabled", temp_b);
  fast_mc_telemetries_get_i_d_ref__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_d_ref.unit", temp_s);
  fast_mc_telemetries_get_v_q__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.v_q.enabled", temp_b);
  fast_mc_telemetries_get_v_q__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.v_q.unit", temp_s);
  fast_mc_telemetries_get_v_d__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.v_d.enabled", temp_b);
  fast_mc_telemetries_get_v_d__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.v_d.unit", temp_s);
  fast_mc_telemetries_get_i_a__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_a.enabled", temp_b);
  fast_mc_telemetries_get_i_a__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_a.unit", temp_s);
  fast_mc_telemetries_get_i_b__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.i_b.enabled", temp_b);
  fast_mc_telemetries_get_i_b__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.i_b.unit", temp_s);
  fast_mc_telemetries_get_v_a__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.v_a.enabled", temp_b);
  fast_mc_telemetries_get_v_a__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.v_a.unit", temp_s);
  fast_mc_telemetries_get_v_b__enabled(&temp_b);
  json_object_dotset_boolean(JSON_Status, "fast_mc_telemetries.v_b.enabled", temp_b);
  fast_mc_telemetries_get_v_b__unit(&temp_s);
  json_object_dotset_string(JSON_Status, "fast_mc_telemetries.v_b.unit", temp_s);
  float_t temp_f = 0;
  fast_mc_telemetries_get_sensitivity__voltage(&temp_f);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.sensitivity.voltage", temp_f);
  fast_mc_telemetries_get_sensitivity__current(&temp_f);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.sensitivity.current", temp_f);
  fast_mc_telemetries_get_sensitivity__frequency(&temp_f);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.sensitivity.frequency", temp_f);
  fast_mc_telemetries_get_samples_per_ts(&temp_i);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.samples_per_ts", temp_i);
  fast_mc_telemetries_get_odr(&temp_i);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.odr", temp_i);
  fast_mc_telemetries_get_ioffset(&temp_f);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.ioffset", temp_f);
  /* Next fields are not in DTDL model but added looking @ the component schema
  field (this is :sensors). ONLY for Sensors, Algorithms and Actuators*/
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.c_type", COMP_TYPE_ACTUATOR);
  int8_t temp_int8 = 0;
  fast_mc_telemetries_get_stream_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.stream_id", temp_int8);
  fast_mc_telemetries_get_ep_id(&temp_int8);
  json_object_dotset_number(JSON_Status, "fast_mc_telemetries.ep_id", temp_int8);

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

uint8_t Fast_Mc_Telemetries_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                 uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  bool valid_property = false;
  char *resp_msg;
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.enable");
    valid_property = true;
    ret = fast_mc_telemetries_set_enable(enable, &resp_msg);
    json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
    if (ret == PNPL_NO_ERROR_CODE)
    {
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
    }
    else
    {
      bool old_enable;
      fast_mc_telemetries_get_enable(&old_enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_enable);
      json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q.enabled"))
    {
      bool i_q__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_q.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_q__enabled(i_q__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_q__enabled;
        fast_mc_telemetries_get_i_q__enabled(&old_i_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q.unit"))
    {
      const char *i_q__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_q.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_q__unit(i_q__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_q__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_q__unit;
        fast_mc_telemetries_get_i_q__unit(&old_i_q__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_q__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d.enabled"))
    {
      bool i_d__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_d.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_d__enabled(i_d__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_d__enabled;
        fast_mc_telemetries_get_i_d__enabled(&old_i_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d.unit"))
    {
      const char *i_d__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_d.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_d__unit(i_d__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_d__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_d__unit;
        fast_mc_telemetries_get_i_d__unit(&old_i_d__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_d__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref.enabled"))
    {
      bool i_q_ref__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_q_ref.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_q_ref__enabled(i_q_ref__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_q_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_q_ref__enabled;
        fast_mc_telemetries_get_i_q_ref__enabled(&old_i_q_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_q_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref.unit"))
    {
      const char *i_q_ref__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_q_ref.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_q_ref__unit(i_q_ref__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_q_ref__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_q_ref__unit;
        fast_mc_telemetries_get_i_q_ref__unit(&old_i_q_ref__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_q_ref__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref.enabled"))
    {
      bool i_d_ref__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_d_ref.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_d_ref__enabled(i_d_ref__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_d_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_d_ref__enabled;
        fast_mc_telemetries_get_i_d_ref__enabled(&old_i_d_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_d_ref__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref.unit"))
    {
      const char *i_d_ref__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_d_ref.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_d_ref__unit(i_d_ref__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_d_ref__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_d_ref__unit;
        fast_mc_telemetries_get_i_d_ref__unit(&old_i_d_ref__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_d_ref__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q.enabled"))
    {
      bool v_q__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_q.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_q__enabled(v_q__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", v_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_v_q__enabled;
        fast_mc_telemetries_get_v_q__enabled(&old_v_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_v_q__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q.unit"))
    {
      const char *v_q__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_q.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_q__unit(v_q__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", v_q__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_v_q__unit;
        fast_mc_telemetries_get_v_q__unit(&old_v_q__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_v_q__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d.enabled"))
    {
      bool v_d__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_d.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_d__enabled(v_d__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", v_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_v_d__enabled;
        fast_mc_telemetries_get_v_d__enabled(&old_v_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_v_d__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d.unit"))
    {
      const char *v_d__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_d.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_d__unit(v_d__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", v_d__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_v_d__unit;
        fast_mc_telemetries_get_v_d__unit(&old_v_d__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_v_d__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a.enabled"))
    {
      bool i_a__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_a.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_a__enabled(i_a__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_a__enabled;
        fast_mc_telemetries_get_i_a__enabled(&old_i_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a.unit"))
    {
      const char *i_a__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_a.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_a__unit(i_a__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_a__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_a__unit;
        fast_mc_telemetries_get_i_a__unit(&old_i_a__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_a__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b.enabled"))
    {
      bool i_b__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_b.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_b__enabled(i_b__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", i_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_i_b__enabled;
        fast_mc_telemetries_get_i_b__enabled(&old_i_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_i_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b.unit"))
    {
      const char *i_b__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_b.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_i_b__unit(i_b__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", i_b__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_i_b__unit;
        fast_mc_telemetries_get_i_b__unit(&old_i_b__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_i_b__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a.enabled"))
    {
      bool v_a__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_a.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_a__enabled(v_a__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", v_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_v_a__enabled;
        fast_mc_telemetries_get_v_a__enabled(&old_v_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_v_a__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a.unit"))
    {
      const char *v_a__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_a.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_a__unit(v_a__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", v_a__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_v_a__unit;
        fast_mc_telemetries_get_v_a__unit(&old_v_a__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_v_a__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b.enabled"))
    {
      bool v_b__enabled = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_b.enabled");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_b__enabled(v_b__enabled, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", v_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        bool old_v_b__enabled;
        fast_mc_telemetries_get_v_b__enabled(&old_v_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.value", old_v_b__enabled);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", false);
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b.unit"))
    {
      const char *v_b__unit = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_b.unit");
      valid_property = true;
      ret = fast_mc_telemetries_set_v_b__unit(v_b__unit, &resp_msg);
      json_object_dotset_string(respJSONObject, "PnPL_Response.message", resp_msg);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", v_b__unit);
        json_object_dotset_boolean(respJSONObject, "PnPL_Response.status", true);
      }
      else
      {
        char *old_v_b__unit;
        fast_mc_telemetries_get_v_b__unit(&old_v_b__unit);
        json_object_dotset_string(respJSONObject, "PnPL_Response.value", old_v_b__unit);
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
    char *log_message = "Invalid property for fast_mc_telemetries";
    PnPLCreateLogMessage(response, size, log_message, PNPL_LOG_ERROR);
    ret = PNPL_BASE_ERROR_CODE;
  }
  json_value_free(respJSON);
  return ret;
}


uint8_t Fast_Mc_Telemetries_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                     uint32_t *size, uint8_t pretty)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

