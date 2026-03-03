/**
  ********************************************************************************
  * @file    MC_Networ.c
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

#include "MC_Network.h"
#include "MC_Network_vtbl.h"
#include "services/sysdebug.h"
#include "network.h"
#include "network_data_params.h"

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sNetworkObj                                  sCharNetworkObj
#endif

#define SYS_DEBUGF(level, message)                   SYS_DEBUGF3(SYS_DBG_CHAR, level, message)


/**
  * Class object declaration.
  */
typedef struct _MC_NetworkClass
{
  /**
    * IAI_Network_t class virtual table.
    */
  IAI_Network_vtbl vtbl;

} MC_NetworkClass_t;


/* Objects instance */
/********************/

/**
  * The class object.
  */
static const MC_NetworkClass_t sTheClass =
{
  /* class virtual table */
  {
    MC_Network_vtblCreate,
    MC_Network_vtblCreateAndInit,
    MC_Network_vtbDestroy,
    MC_Network_vtbInit,
    MC_Network_vtbGetReport,
    MC_Network_vtbInputsGet,
    MC_Network_vtbOutputsGet,
    MC_Network_vtbRun,
    MC_Network_vtbGetError
  }
};

#if (MC_NETOWRK_CGF_USE_DEF_ACTIVATION_BUFF == 1)
static uint8_t sPool[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
//ai_handle data_activations12[] = {sPool};
#endif

/**
  * The only instance of the network.
  */
static MC_Network_t sNetworkObj;


/* Private member function declaration */
/***************************************/


/* Inline function forward declaration */
/***************************************/


/* Public functions definition */
/*******************************/

IAI_Network_t *MC_Network_Alloc()
{
  ((IAI_Network_t *)&sNetworkObj)->vptr = &sTheClass.vtbl;
  ((AAI_Network_t *)&sNetworkObj)->network = NULL;
#if (MC_NETOWRK_CGF_USE_DEF_ACTIVATION_BUFF == 1)
  sNetworkObj.p_data_activations[0] = (ai_handle)sPool;
#endif

  return (IAI_Network_t *)&sNetworkObj;
}


/* IAI_Network virtual functions definition */
/********************************************/

ai_error MC_Network_vtblCreate(IAI_Network_t *_this, const ai_buffer *p_network_config)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return  ai_network_create(&p_obj->super.network, p_network_config);
}

ai_error MC_Network_vtblCreateAndInit(IAI_Network_t *_this, const ai_handle *p_activations, const ai_handle *p_weights)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_create_and_init(&p_obj->super.network, p_activations, p_weights);
}

ai_handle MC_Network_vtbDestroy(IAI_Network_t *_this)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_destroy(p_obj->super.network);
}

ai_bool MC_Network_vtbInit(IAI_Network_t *_this, const ai_network_params *p_params)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_init(&p_obj->super.network, p_params);
}

ai_bool MC_Network_vtbGetReport(IAI_Network_t *_this, ai_network_report *p_report)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_get_report(p_obj->super.network, p_report);
}

ai_buffer *MC_Network_vtbInputsGet(IAI_Network_t *_this, ai_u16 *p_buffer)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_inputs_get(p_obj->super.network, p_buffer);
}

ai_buffer *MC_Network_vtbOutputsGet(IAI_Network_t *_this, ai_u16 *p_buffer)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_outputs_get(p_obj->super.network, p_buffer);
}

ai_i32 MC_Network_vtbRun(IAI_Network_t *_this, const ai_buffer *p_input, ai_buffer *p_output)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_run(p_obj->super.network, p_input, p_output);
}

ai_error MC_Network_vtbGetError(IAI_Network_t *_this)
{
  assert_param(_this != NULL);
  MC_Network_t *p_obj = (MC_Network_t *)_this;

  return ai_network_get_error(p_obj->super.network);
}


/* Private function definition */
/*******************************/

