/**
  ******************************************************************************
  * @file    Log_Controller_PnPL.c
  * @author  SRA
  * @brief   Log_Controller PnPL Component Manager
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
  * dtmi:vespucci:other:log_controller;2
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

#include "Log_Controller_PnPL.h"

static const IPnPLComponent_vtbl sLog_Controller_PnPL_CompIF_vtbl =
{
  Log_Controller_PnPL_vtblGetKey,
  Log_Controller_PnPL_vtblGetNCommands,
  Log_Controller_PnPL_vtblGetCommandKey,
  Log_Controller_PnPL_vtblGetStatus,
  Log_Controller_PnPL_vtblSetProperty,
  Log_Controller_PnPL_vtblExecuteFunction
};

/**
  *  Log_Controller_PnPL internal structure.
  */
struct _Log_Controller_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
};

/* Objects instance ----------------------------------------------------------*/
static Log_Controller_PnPL sLog_Controller_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Log_Controller_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sLog_Controller_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sLog_Controller_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Log_Controller_PnPLInit(IPnPLComponent_t *_this)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  log_controller_comp_init();
  return PNPL_NO_ERROR_CODE;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Log_Controller_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return log_controller_get_key();
}

uint8_t Log_Controller_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 6;
}

char *Log_Controller_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  switch (id)
  {
    case 0:
      return "log_controller*save_config";
      break;
    case 1:
      return "log_controller*start_log";
      break;
    case 2:
      return "log_controller*stop_log";
      break;
    case 3:
      return "log_controller*set_time";
      break;
    case 4:
      return "log_controller*switch_bank";
      break;
    case 5:
      return "log_controller*set_dfu_mode";
      break;
  }
  return "";
}

uint8_t Log_Controller_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size,
                                          uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  bool temp_b = 0;
  log_controller_get_log_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "log_controller.log_status", temp_b);
  log_controller_get_sd_mounted(&temp_b);
  json_object_dotset_boolean(JSON_Status, "log_controller.sd_mounted", temp_b);
  int32_t temp_i = 0;
  log_controller_get_controller_type(&temp_i);
  json_object_dotset_number(JSON_Status, "log_controller.controller_type", temp_i);
  json_object_dotset_number(JSON_Status, "log_controller.c_type", COMP_TYPE_OTHER);

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

uint8_t Log_Controller_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                            uint32_t *size, uint8_t pretty)
{
  *size = 61;
  *response = (char *)pnpl_malloc(*size);
  (void)strcpy(*response, "{\"PnPL_Error\":\"No writable Properties in DeviceInformation\"}\0");
  return PNPL_NO_ERROR_CODE;
}


uint8_t Log_Controller_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response,
                                                uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = PNPL_NO_ERROR_CODE;
  if (json_object_dothas_value(tempJSONObject, "log_controller*save_config"))
  {
    ret = log_controller_save_config();
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*save_config.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.save_config.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "log_controller*start_log.interface"))
  {
    int32_t interface = (int32_t)json_object_dotget_number(tempJSONObject, "log_controller*start_log.interface");
    ret = log_controller_start_log(interface);
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*start_log.interface.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.start_log.interface", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "log_controller*stop_log"))
  {
    ret = log_controller_stop_log();
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*stop_log.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.stop_log.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "log_controller*set_time.datetime"))
  {
    const char *datetime = json_object_dotget_string(tempJSONObject, "log_controller*set_time.datetime");
    ret = log_controller_set_time(datetime);
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*set_time.datetime.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.set_time.datetime", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "log_controller*switch_bank"))
  {
    ret = log_controller_switch_bank();
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*switch_bank.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.switch_bank.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "log_controller*set_dfu_mode"))
  {
    ret = log_controller_set_dfu_mode();
    if (ret == 0)
    {
      json_object_dotset_string(respJSONObject, "log_controller*set_dfu_mode.response", 0);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "log_controller.set_dfu_mode.response", "PNPL_CMD_ERROR");
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
