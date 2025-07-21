/**
  ********************************************************************************
  * @file    AIMotor_Classifier.h
  * @author  STMicroelectronics - SRA
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
#ifndef X_CUBE_AI_APP_CHAR_NETWORK_H_
#define X_CUBE_AI_APP_CHAR_NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ANeaiNcc_Model.h"
#include "NanoEdgeAI_ncc.h"

/**
  * Create  type name for _ANeaiNcc_Model.
  */
typedef struct _AI_MotorControl_Classifier AI_MotorControl_Classifier_t;

/**
  * @detail @ANeaiNcc_Model_t abstract class implementation
  */
struct _AI_MotorControl_Classifier
{
  ANeaiNcc_Model_t Super;
};

/* Public API declaration */
/**************************/

/**
  * Allocate an instance of Motor_Classifier.
  * This allocator implement the singleton design pattern.
  *
  * @retval a pointer to the generic object ::INeaiNcc_Model_t if success,
  *         or NULL if out of memory error occurs.
  */
INeaiNcc_Model_t *Motor_Classifier_Alloc(void);

/**
  * @brief Initializes the Motor Classifier object with the given model.
  *
  * @param _this Pointer to the INeaiNcc_Model_t object to be initialized as a Motor Classifier.
  *
  * @retval None
  */
void Motor_Classifier_Init(INeaiNcc_Model_t *_this);

/* IAI_Network virtual functions */
uint8_t ANeai_Classification_vtblInit_ncc(INeaiNcc_Model_t *_this, const float_t knowledge_buffer[]);
uint8_t ANeai_Classification_vtbl_ncc(INeaiNcc_Model_t *_this, float_t data_input[], float_t output_buffer[],
                                      uint16_t *id_class);

/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* X_CUBE_AI_APP_CHAR_NETWORK_H_ */
