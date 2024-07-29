/**
  ******************************************************************************
  * @file    App_model_Motor_Controller.c
  * @author  SRA
  * @brief   Motor_Controller PnPL Components APIs
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
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/
#include "MCPTask.h"

/* USER private function prototypes ------------------------------------------*/

/* USER defines --------------------------------------------------------------*/
#define MOTOR_CTRL_MIN_SPEED -4000
#define MOTOR_CTRL_MAX_SPEED +4000

/* Motor Controller PnPL Component -------------------------------------------*/
extern AppModel_t app_model;

uint8_t motor_controller_comp_init(void)
{
  app_model.motor_controller_model.comp_name = motor_controller_get_key();
  /* USER Component initialization code */
  return PNPL_NO_ERROR_CODE;
}

char *motor_controller_get_key(void)
{
  return "motor_controller";
}


uint8_t motor_controller_get_motor_status(bool *value)
{
  *value = MCPTask_GetMotorStarted();
   return PNPL_NO_ERROR_CODE;
}

uint8_t motor_controller_get_motor_speed(int32_t *value)
{
  *value = MCPTask_GetMotorSpeed();
  return PNPL_NO_ERROR_CODE;
}


uint8_t motor_controller_set_motor_speed(int32_t value)
{
  int32_t min =MOTOR_CTRL_MIN_SPEED;
  int32_t max =MOTOR_CTRL_MAX_SPEED;

  /* USER Code */
  if (value >= min && value <= max)
  {
    MCPTask_SetMotorSpeed(value);
  }

  return PNPL_NO_ERROR_CODE;
}


uint8_t motor_controller_start_motor(void)
{
  MCPTask_start_motor();
  return PNPL_NO_ERROR_CODE;
}

uint8_t motor_controller_stop_motor(void)
{
  MCPTask_stop_motor();
  return PNPL_NO_ERROR_CODE;
}

uint8_t motor_controller_motor_reset(void)
{
  MCPTask_motor_reset();
  return PNPL_NO_ERROR_CODE;
}

uint8_t motor_controller_ack_fault(void)
{
  MCPTask_ack_fault();
  return PNPL_NO_ERROR_CODE;
}


