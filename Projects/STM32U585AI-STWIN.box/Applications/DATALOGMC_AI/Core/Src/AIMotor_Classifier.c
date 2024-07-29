/**
 ********************************************************************************
 * @file    AIMotor_Classifier.c
 * @author  STMicroelectronics - SRA Team
 * @version 1.0.0
 * @date    Feb 28, 2024
 *
 * @brief
 *
 * <ADD_FILE_DESCRIPTION>
 *
 ********************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ********************************************************************************
 */

#include "AIMotor_Classifier.h"
#include "knowledge_ncc.h"
#include "services/sysdebug.h"


#if defined(DEBUG) || defined (SYS_DEBUG)
#define sNetworkObj                                  sMotor_ClassifierObj
#endif

/**
 * Class object declaration.
 */
typedef struct _AI_MotorControl_ClassifierClass
{
  /**
   * INeaiNcc_Model_t class virtual table.
   */
  INeaiNcc_Model_vtbl vtbl;

  /**
   * Neai ncc axis number
   */
  uint32_t axis_number;

  /**
   * Neai ncc data input user dimension
   */
  uint32_t data_input_user_dim;

  /**
   * Neai ncc class number
   */
  uint32_t class_number;

  /**
   * Neai ncc knowledge
   */
  float_t const *p_knowledge;

} AI_MotorControl_ClassifierClass_t;

/* Objects instance */
/********************/

/**
 * The class object.
 */
static const AI_MotorControl_ClassifierClass_t sTheClass =
{
    /* class virtual table */
    {
        ANeai_Classification_vtblInit_ncc,
        ANeai_Classification_vtbl_ncc },

    /**
     * Neai ncc axis number
     */
    AXIS_NUMBER_NCC,

    /**
     * Neai ncc data input user dimension
     */
    DATA_INPUT_USER_NCC,

    /**
     * Neai ncc class number
     */
    CLASS_NUMBER_NCC,

    /**
     * Neai ncc knowledge
     */
    knowledge_ncc
};


/**
 * The only instance of the NanoEdge Classifier Model.
 * Implementation of the abstract class @ANeaiNcc_Model_t
 */
static AI_MotorControl_Classifier_t sMotor_ClassifierObj;


/* Private member function declaration */
/***************************************/


/* Inline function forward declaration */
/***************************************/


/* Public functions definition */
/*******************************/

INeaiNcc_Model_t *Motor_Classifier_Alloc(void)
{
  ((INeaiNcc_Model_t*)(&sMotor_ClassifierObj.Super))->vptr = &sTheClass.vtbl;
  return (INeaiNcc_Model_t*)&sMotor_ClassifierObj;
}

void Motor_Classifier_Init(INeaiNcc_Model_t * _this)
{
  assert_param(_this);
  AI_MotorControl_Classifier_t *p_obj = (AI_MotorControl_Classifier_t *)_this;

  p_obj->Super.axis_number = sTheClass.axis_number;
  p_obj->Super.class_number = sTheClass.class_number;
  p_obj->Super.data_input_user_dim = sTheClass.data_input_user_dim;
  p_obj->Super.p_knowledge = sTheClass.p_knowledge;
}


/* IAI_Network virtual functions definition */
/********************************************/

uint8_t ANeai_Classification_vtblInit_ncc(INeaiNcc_Model_t *_this, const float knowledge_buffer[])
{
  UNUSED(_this);
  return neai_classification_init_ncc(knowledge_buffer);
}

uint8_t ANeai_Classification_vtbl_ncc(INeaiNcc_Model_t *_this, float data_input[], float output_buffer[], uint16_t *id_class)
{
  UNUSED(_this);
  return neai_classification_ncc(data_input, output_buffer, id_class);
}

/* Private function definition */
/*******************************/


