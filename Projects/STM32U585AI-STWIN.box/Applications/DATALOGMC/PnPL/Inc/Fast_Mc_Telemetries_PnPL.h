/**
  ******************************************************************************
  * @file    Fast_Mc_Telemetries_PnPL.h
  * @author  SRA
  * @brief   Fast_Mc_Telemetries PnPL Component Manager
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:fast_mc_telemetries;1
  *
  * Created by: DTDL2PnPL_cGen version 1.2.3
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PNPL_FAST_MC_TELEMETRIES_H_
#define _PNPL_FAST_MC_TELEMETRIES_H_



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "parson.h"
#include "IPnPLComponent.h"
#include "IPnPLComponent_vtbl.h"


/**
  * Create a type name for _Fast_Mc_Telemetries_PnPL.
 */
typedef struct _Fast_Mc_Telemetries_PnPL Fast_Mc_Telemetries_PnPL;

/* Public API declaration ----------------------------------------------------*/

IPnPLComponent_t *Fast_Mc_Telemetries_PnPLAlloc(void);

/**
  * Initialize the default parameters.
  *
 */
uint8_t Fast_Mc_Telemetries_PnPLInit(IPnPLComponent_t *_this);


/**
  * Get the IPnPLComponent interface for the component.
  * @param _this [IN] specifies a pointer to a PnPL component.
  * @return a pointer to the generic object ::IPnPLComponent if success,
  * or NULL if out of memory error occurs.
 */
IPnPLComponent_t *Fast_Mc_Telemetries_PnPLGetComponentIF(Fast_Mc_Telemetries_PnPL *_this);

#ifdef __cplusplus
}
#endif

#endif /* _PNPL_FAST_MC_TELEMETRIES_H_ */
