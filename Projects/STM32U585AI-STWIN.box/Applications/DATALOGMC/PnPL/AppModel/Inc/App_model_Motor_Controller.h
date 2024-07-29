/**
  ******************************************************************************
  * @file    App_model_Motor_Controller.h
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

/**
  ******************************************************************************
  * Component APIs *************************************************************
  * - Component init function
  *    <comp_name>_comp_init(void)
  * - Component get_key function
  *    <comp_name>_get_key(void)
  * - Component GET/SET Properties APIs ****************************************
  *  - GET Functions
  *    uint8_t <comp_name>_get_<prop_name>(prop_type *value)
  *      if prop_type == char --> (char **value)
  *  - SET Functions
  *    uint8_t <comp_name>_set_<prop_name>(prop_type value)
  *      if prop_type == char --> (char *value)
  *  - Component COMMAND Reaction Functions
  *      uint8_t <comp_name>_<command_name>(
  *                     field1_type field1_name, field2_type field2_name, ...,
  *                     fieldN_type fieldN_name);
  *  - Component TELEMETRY Send Functions
  *      uint8_t <comp_name>_create_telemetry(tel1_type tel1_name,
  *                     tel2_type tel2_name, ..., telN_type telN_name,
  *                     char **telemetry, uint32_t *size)
  ******************************************************************************
  */

#ifndef APP_MODEL_MOTOR_CONTROLLER_H_
#define APP_MODEL_MOTOR_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/

typedef struct _MotorControllerModel_t
{
  char *comp_name;
  /* MotorController Component Model USER code */
} MotorControllerModel_t;

/* Motor Controller PnPL Component -------------------------------------------- */
uint8_t motor_controller_comp_init(void);
char *motor_controller_get_key(void);
uint8_t motor_controller_get_motor_status(bool *value);
uint8_t motor_controller_get_motor_speed(int32_t *value);
uint8_t motor_controller_set_motor_speed(int32_t value);
uint8_t motor_controller_start_motor(void);
uint8_t motor_controller_stop_motor(void);
uint8_t motor_controller_motor_reset(void);
uint8_t motor_controller_ack_fault(void);


#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_MOTOR_CONTROLLER_H_ */
