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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:fast_mc_telemetries;1
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
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.enable"))
  {
    bool enable = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.enable");
    ret = fast_mc_telemetries_set_enable(enable);
    if (ret == PNPL_NO_ERROR_CODE)
    {
      json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.enable.value", enable);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "fast_mc_telemetries.enable.value", "PNPL_SET_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q.enabled"))
    {
      bool i_q = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_q.enabled");
      ret = fast_mc_telemetries_set_i_q__enabled(i_q);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_q.enabled.value", i_q);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q.unit"))
    {
      const char *i_q = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_q.unit");
      ret = fast_mc_telemetries_set_i_q__unit(i_q);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q.unit.value", i_q);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d.enabled"))
    {
      bool i_d = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_d.enabled");
      ret = fast_mc_telemetries_set_i_d__enabled(i_d);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_d.enabled.value", i_d);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d.unit"))
    {
      const char *i_d = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_d.unit");
      ret = fast_mc_telemetries_set_i_d__unit(i_d);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d.unit.value", i_d);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref.enabled"))
    {
      bool i_q_ref = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_q_ref.enabled");
      ret = fast_mc_telemetries_set_i_q_ref__enabled(i_q_ref);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_q_ref.enabled.value", i_q_ref);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q_ref.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_q_ref.unit"))
    {
      const char *i_q_ref = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_q_ref.unit");
      ret = fast_mc_telemetries_set_i_q_ref__unit(i_q_ref);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q_ref.unit.value", i_q_ref);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_q_ref.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref.enabled"))
    {
      bool i_d_ref = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_d_ref.enabled");
      ret = fast_mc_telemetries_set_i_d_ref__enabled(i_d_ref);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_d_ref.enabled.value", i_d_ref);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d_ref.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_d_ref.unit"))
    {
      const char *i_d_ref = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_d_ref.unit");
      ret = fast_mc_telemetries_set_i_d_ref__unit(i_d_ref);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d_ref.unit.value", i_d_ref);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_d_ref.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q.enabled"))
    {
      bool v_q = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_q.enabled");
      ret = fast_mc_telemetries_set_v_q__enabled(v_q);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.v_q.enabled.value", v_q);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_q.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_q.unit"))
    {
      const char *v_q = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_q.unit");
      ret = fast_mc_telemetries_set_v_q__unit(v_q);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_q.unit.value", v_q);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_q.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d.enabled"))
    {
      bool v_d = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_d.enabled");
      ret = fast_mc_telemetries_set_v_d__enabled(v_d);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.v_d.enabled.value", v_d);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_d.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_d.unit"))
    {
      const char *v_d = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_d.unit");
      ret = fast_mc_telemetries_set_v_d__unit(v_d);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_d.unit.value", v_d);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_d.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a.enabled"))
    {
      bool i_a = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_a.enabled");
      ret = fast_mc_telemetries_set_i_a__enabled(i_a);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_a.enabled.value", i_a);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_a.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_a.unit"))
    {
      const char *i_a = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_a.unit");
      ret = fast_mc_telemetries_set_i_a__unit(i_a);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_a.unit.value", i_a);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_a.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b.enabled"))
    {
      bool i_b = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.i_b.enabled");
      ret = fast_mc_telemetries_set_i_b__enabled(i_b);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.i_b.enabled.value", i_b);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_b.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.i_b.unit"))
    {
      const char *i_b = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.i_b.unit");
      ret = fast_mc_telemetries_set_i_b__unit(i_b);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_b.unit.value", i_b);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.i_b.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a.enabled"))
    {
      bool v_a = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_a.enabled");
      ret = fast_mc_telemetries_set_v_a__enabled(v_a);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.v_a.enabled.value", v_a);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_a.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_a.unit"))
    {
      const char *v_a = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_a.unit");
      ret = fast_mc_telemetries_set_v_a__unit(v_a);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_a.unit.value", v_a);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_a.unit.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b.enabled"))
    {
      bool v_b = json_object_dotget_boolean(tempJSONObject, "fast_mc_telemetries.v_b.enabled");
      ret = fast_mc_telemetries_set_v_b__enabled(v_b);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_boolean(respJSONObject, "fast_mc_telemetries.v_b.enabled.value", v_b);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_b.enabled.value", "PNPL_SET_ERROR");
      }
    }
  }
  if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b"))
  {
    if (json_object_dothas_value(tempJSONObject, "fast_mc_telemetries.v_b.unit"))
    {
      const char *v_b = json_object_dotget_string(tempJSONObject, "fast_mc_telemetries.v_b.unit");
      ret = fast_mc_telemetries_set_v_b__unit(v_b);
      if (ret == PNPL_NO_ERROR_CODE)
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_b.unit.value", v_b);
      }
      else
      {
        json_object_dotset_string(respJSONObject, "fast_mc_telemetries.v_b.unit.value", "PNPL_SET_ERROR");
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


uint8_t Fast_Mc_Telemetries_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                     uint32_t *size, uint8_t pretty)
{
  return PNPL_NO_COMMANDS_ERROR_CODE;
}

