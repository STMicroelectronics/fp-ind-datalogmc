/**
  ******************************************************************************
  * @file    IMotor_Controller.h
  * @author  SRA
  * @brief
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

#ifndef INCLUDE_IMOTOR_CONTROLLER_H_
#define INCLUDE_IMOTOR_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
  * Create  type name for IMotor_Controller.
  */
typedef struct _IMotor_Controller_t IMotor_Controller_t;

/* Public API declarations ---------------------------------------------------*/

/* Public interface */
inline uint8_t IMotor_Controller_start_motor(IMotor_Controller_t *_this);
inline uint8_t IMotor_Controller_stop_motor(IMotor_Controller_t *_this);
inline uint8_t IMotor_Controller_motor_reset(IMotor_Controller_t *_this);
inline uint8_t IMotor_Controller_ack_fault(IMotor_Controller_t *_this);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IMOTOR_CONTROLLER_H_ */
