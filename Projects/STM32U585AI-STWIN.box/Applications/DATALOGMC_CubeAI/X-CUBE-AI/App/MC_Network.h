/**
  ********************************************************************************
  * @file    MC_Network.h
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
#ifndef X_CUBE_AI_APP_MC_NETWORK_H_
#define X_CUBE_AI_APP_MC_NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "services/AAI_Network.h"
#include "services/AAI_Network_vtbl.h"

#ifndef MC_NETOWRK_CGF_USE_DEF_ACTIVATION_BUFF
#define MC_NETOWRK_CGF_USE_DEF_ACTIVATION_BUFF    1
#endif

/**
  * Create  type name for _MC_Network.
  */
typedef struct _MC_Network MC_Network_t;

/**
  *  MC_Network internal structure.
  */
struct _MC_Network
{
  /**
    * Base class object.
    */
  AAI_Network_t super;

  /* network variables should be added here. */

  ai_handle p_data_activations[1];
};


/* Public API declaration */
/**************************/

/**
  * Allocate an instance of _MC_Network in the framework. This allocator implement the singleton design pattern.
  *
  * @return a pointer to the generic object ::IAI_Network if success,
  *         or NULL if out of memory error occurs.
  */
IAI_Network_t *MC_Network_Alloc(void);


/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* X_CUBE_AI_APP_MC_NETWORK_H_ */
