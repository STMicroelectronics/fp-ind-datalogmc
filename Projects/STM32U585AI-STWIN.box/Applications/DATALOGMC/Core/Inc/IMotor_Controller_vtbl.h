/**
  ******************************************************************************
  * @file    IMotor_Controller_vtbl.h
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

#ifndef INCLUDE_IMOTOR_CONTROLLER_VTBL_H_
#define INCLUDE_IMOTOR_CONTROLLER_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
  * Create a type name for IMotor_Controller_vtbl.
  */
typedef struct _IMotor_Controller_vtbl IMotor_Controller_vtbl;

struct _IMotor_Controller_vtbl
{
  uint8_t (*motor_controller_start_motor)(IMotor_Controller_t * _this);
  uint8_t (*motor_controller_stop_motor)(IMotor_Controller_t * _this);
  uint8_t (*motor_controller_motor_reset)(IMotor_Controller_t * _this);
  uint8_t (*motor_controller_ack_fault)(IMotor_Controller_t * _this);
};

struct _IMotor_Controller_t
{
  /**
    * Pointer to the virtual table for the class.
    */
  const IMotor_Controller_vtbl *vptr;
};

/* Inline functions definition -----------------------------------------------*/
inline uint8_t IMotor_Controller_start_motor(IMotor_Controller_t *_this)
{
  return _this->vptr->motor_controller_start_motor(_this);
}
inline uint8_t IMotor_Controller_stop_motor(IMotor_Controller_t *_this)
{
  return _this->vptr->motor_controller_stop_motor(_this);
}
inline uint8_t IMotor_Controller_motor_reset(IMotor_Controller_t *_this)
{
  return _this->vptr->motor_controller_motor_reset(_this);
}
inline uint8_t IMotor_Controller_ack_fault(IMotor_Controller_t *_this)
{
  return _this->vptr->motor_controller_ack_fault(_this);
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_IMOTOR_CONTROLLER_VTBL_H_ */
