/**
  ******************************************************************************
  * @file    Motor_Controller_PnPL.c
  * @author  SRA
  * @brief   Motor_Controller PnPL Component Manager
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:other:motor_controller;1
  *
  * Created by: DTDL2PnPL_cGen version 1.2.2
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

#include "Motor_Controller_PnPL.h"
#include "Motor_Controller_PnPL_vtbl.h"

static const IPnPLComponent_vtbl sMotor_Controller_PnPL_CompIF_vtbl =
{
  Motor_Controller_PnPL_vtblGetKey,
  Motor_Controller_PnPL_vtblGetNCommands,
  Motor_Controller_PnPL_vtblGetCommandKey,
  Motor_Controller_PnPL_vtblGetStatus,
  Motor_Controller_PnPL_vtblSetProperty,
  Motor_Controller_PnPL_vtblExecuteFunction
};

/**
  *  Motor_Controller_PnPL internal structure.
  */
struct _Motor_Controller_PnPL
{
  /* Implements the IPnPLComponent interface. */
  IPnPLComponent_t component_if;
  /* Contains Motor_Controller functions pointers. */
  IMotor_Controller_t *cmdIF;
};

/* Objects instance ----------------------------------------------------------*/
static Motor_Controller_PnPL sMotor_Controller_PnPL;

/* Public API definition -----------------------------------------------------*/
IPnPLComponent_t *Motor_Controller_PnPLAlloc()
{
  IPnPLComponent_t *pxObj = (IPnPLComponent_t *) &sMotor_Controller_PnPL;
  if (pxObj != NULL)
  {
    pxObj->vptr = &sMotor_Controller_PnPL_CompIF_vtbl;
  }
  return pxObj;
}

uint8_t Motor_Controller_PnPLInit(IPnPLComponent_t *_this,  IMotor_Controller_t *inf)
{
  IPnPLComponent_t *component_if = _this;
  PnPLAddComponent(component_if);
  Motor_Controller_PnPL *p_if_owner = (Motor_Controller_PnPL *) _this;
  p_if_owner->cmdIF = inf;
  motor_controller_comp_init();
  return 0;
}


/* IPnPLComponent virtual functions definition -------------------------------*/
char *Motor_Controller_PnPL_vtblGetKey(IPnPLComponent_t *_this)
{
  return motor_controller_get_key();
}

uint8_t Motor_Controller_PnPL_vtblGetNCommands(IPnPLComponent_t *_this)
{
  return 4;
}

char *Motor_Controller_PnPL_vtblGetCommandKey(IPnPLComponent_t *_this, uint8_t id)
{
  switch (id)
  {
  case 0:
    return "motor_controller*start_motor";
    break;
  case 1:
    return "motor_controller*stop_motor";
    break;
  case 2:
    return "motor_controller*motor_reset";
    break;
  case 3:
    return "motor_controller*ack_fault";
    break;
  }
  return 0;
}

uint8_t Motor_Controller_PnPL_vtblGetStatus(IPnPLComponent_t *_this, char **serializedJSON, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON;
  JSON_Object *JSON_Status;

  tempJSON = json_value_init_object();
  JSON_Status = json_value_get_object(tempJSON);

  bool temp_b = 0;
  motor_controller_get_motor_status(&temp_b);
  json_object_dotset_boolean(JSON_Status, "motor_controller.motor_status", temp_b);
  int32_t temp_i = 0;
  motor_controller_get_motor_speed(&temp_i);
  json_object_dotset_number(JSON_Status, "motor_controller.motor_speed", temp_i);
  json_object_dotset_number(JSON_Status, "motor_controller.c_type", COMP_TYPE_OTHER);

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

uint8_t Motor_Controller_PnPL_vtblSetProperty(IPnPLComponent_t *_this, char *serializedJSON, char **response, uint32_t *size, uint8_t pretty)
{
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = 0;
  if (json_object_dothas_value(tempJSONObject, "motor_controller.motor_speed"))
  {
    int32_t motor_speed =(int32_t) json_object_dotget_number(tempJSONObject, "motor_controller.motor_speed");
    ret = motor_controller_set_motor_speed(motor_speed);
    if(ret == 0)
    {
      json_object_dotset_number(respJSONObject, "motor_controller.motor_speed.value",motor_speed);
    }
    else
    {
      json_object_dotset_string(respJSONObject, "motor_controller.motor_speed.value", "PNPL_SET_ERROR");
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


uint8_t Motor_Controller_PnPL_vtblExecuteFunction(IPnPLComponent_t *_this, char *serializedJSON, char **response, uint32_t *size, uint8_t pretty)
{
  Motor_Controller_PnPL *p_if_owner = (Motor_Controller_PnPL *) _this;
  JSON_Value *tempJSON = json_parse_string(serializedJSON);
  JSON_Object *tempJSONObject = json_value_get_object(tempJSON);
  JSON_Value *respJSON = json_value_init_object();
  JSON_Object *respJSONObject = json_value_get_object(respJSON);

  uint8_t ret = 0;
  if (json_object_dothas_value(tempJSONObject, "motor_controller*start_motor"))
  {
    ret = motor_controller_start_motor(p_if_owner->cmdIF);
    if(ret == 0)
    {
      json_object_dotset_string(respJSONObject, "motor_controller*start_motor.response", "PNPL_CMD_OK");
    }
    else
    {
      json_object_dotset_string(respJSONObject, "motor_controller.start_motor.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "motor_controller*stop_motor"))
  {
    ret = motor_controller_stop_motor(p_if_owner->cmdIF);
    if(ret == 0)
    {
      json_object_dotset_string(respJSONObject, "motor_controller*stop_motor.response", "PNPL_CMD_OK");
    }
    else
    {
      json_object_dotset_string(respJSONObject, "motor_controller.stop_motor.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "motor_controller*motor_reset"))
  {
    ret = motor_controller_motor_reset(p_if_owner->cmdIF);
    if(ret == 0)
    {
      json_object_dotset_string(respJSONObject, "motor_controller*motor_reset.response", "PNPL_CMD_OK");
    }
    else
    {
      json_object_dotset_string(respJSONObject, "motor_controller.motor_reset.response", "PNPL_CMD_ERROR");
    }
  }
  if (json_object_dothas_value(tempJSONObject, "motor_controller*ack_fault"))
  {
    ret = motor_controller_ack_fault(p_if_owner->cmdIF);
    if(ret == 0)
    {
      json_object_dotset_string(respJSONObject, "motor_controller*ack_fault.response", "PNPL_CMD_OK");
    }
    else
    {
      json_object_dotset_string(respJSONObject, "motor_controller.ack_fault.response", "PNPL_CMD_ERROR");
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

