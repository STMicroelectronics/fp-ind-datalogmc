/**
  ********************************************************************************
  * @file    MC_Networ_vtbl.h
  * @author  STMicroelectronics - SRA
  * @version 1.0.0
  * @date    Aug 18, 2025
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
#ifndef X_CUBE_AI_APP_MC_NETWOR_VTBL_H_
#define X_CUBE_AI_APP_MC_NETWOR_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* IAI_Network virtual functions */
ai_error   MC_Network_vtblCreate(IAI_Network_t *_this, const ai_buffer *p_network_config);                                   /*!< @sa IAI_Network_Create */
ai_error   MC_Network_vtblCreateAndInit(IAI_Network_t *_this, const ai_handle *p_activations, const ai_handle *p_weights);  /*!< @sa IAI_Network_CreateAndInit */
ai_handle  MC_Network_vtbDestroy(IAI_Network_t *_this);                                                                     /*!< @sa IAI_Network_Destroy */
ai_bool    MC_Network_vtbInit(IAI_Network_t *_this, const ai_network_params *p_params);                                     /*!< @sa IAI_Network_Init */
ai_bool    MC_Network_vtbGetReport(IAI_Network_t *_this, ai_network_report *p_report);                                      /*!< @sa IAI_Network_GetReport */
ai_buffer *MC_Network_vtbInputsGet(IAI_Network_t *_this, ai_u16 *p_buffer);                                                 /*!< @sa IAI_Network_InputGet */
ai_buffer *MC_Network_vtbOutputsGet(IAI_Network_t *_this, ai_u16 *p_buffer);                                                /*!< @sa IAI_Network_OutputGet */
ai_i32     MC_Network_vtbRun(IAI_Network_t *_this, const ai_buffer *p_input, ai_buffer *p_output);                          /*!< @sa IAI_Network_Run*/
ai_error   MC_Network_vtbGetError(IAI_Network_t *_this);                                                                    /*!< @sa IAI_Network_GetError*/


#ifdef __cplusplus
}
#endif

#endif /* X_CUBE_AI_APP_MC_NETWORK_VTBL_H_ */
