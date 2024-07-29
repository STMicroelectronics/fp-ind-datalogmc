/**
 ******************************************************************************
 * @file    UtilTask.h
 * @author  SRA
 * @brief  Utility task declaration.
 *
 * The Utility task is in export minor functionalities of the application:
 * - Time stamp for the Sensor Manager
 * - Manage the user button and the user LED1
 *
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
#ifndef INC_UTILTASK_H_
#define INC_UTILTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "drivers/BCAdcDriver.h"
#include "drivers/BCAdcDriver_vtbl.h"
#include "drivers/BCTimerDriver.h"
#include "drivers/BCTimerDriver_vtbl.h"
#include "drivers/BCTimChgDriver.h"
#include "drivers/BCTimChgDriver_vtbl.h"
#include "services/BCProtocol.h"
#include "tx_api.h"

#define UTIL_CMD_ID_BUTTON_EVT                        ((uint16_t)0x0004)              ///< Button event. It can be BUTTON_DOWN or BUTTON_UP
#define UTIL_CMD_ID_PB_TIMER_ELAPSED                  ((uint16_t)0x0005)              ///< PB timer is elapsed.
#define UTIL_CMD_ID_DATALOG_LED                       ((uint16_t)0x0006)              ///< Button event. Toggle led

#define UTIL_PARAM_BUTTON_EVT_DOWN                    ((uint16_t)0x0001)              ///< Button event parameter: BUTTON_DOWN
#define UTIL_PARAM_BUTTON_EVT_UP                      ((uint16_t)0x0002)              ///< Button event parameter: BUTTON_UP

/**
 * Create  type name for _UtilTask_t.
 */
typedef struct _UtilTask_t UtilTask_t;

/**
 *  UtilTask_t internal structure.
 */
struct _UtilTask_t
{
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Task variables should be added here. */

  /**
   * HAL user button configuration parameters.
   */
  const void *p_mx_ub_drv_cfg;

  /**
   * HAL led1 configuration parameters.
   */
  const void *p_mx_led1_drv_cfg;

  /**
   * HAL led2 configuration parameters.
   */
  const void *p_mx_led2_drv_cfg;

  /**
   * Software timer used to synthesize the button events:
   *  - SHORT_PRESS
   *  - LONG_PRESS
   *  - DOUBLE_PRESS
   */
  TX_TIMER pb_timer;

  /**
   * Input queue used by other task to request services.
   */
  TX_QUEUE in_queue;

  /**
   * Software timer used make user led blinking
   */
  TX_TIMER user_led_timer;
};

/* Public API declaration */
/**************************/

/**
 * Allocate an instance of UtilTask.
 * @return a pointer to the generic object ::AManagedTaskEx if success, or NULL if out of memory error occurs.
 */
AManagedTaskEx* UtilTaskAlloc(const void *p_mx_led1_drv_cfg, const void *p_mx_led2_drv_cfg);

/**
 * Get the battery level value in percentage
 *
 * @param batt_percentage [OUT]: variable to store battery level value
 *
 * @return error value.
 */
sys_error_code_t UtilTask_GetBatteryStatus(uint8_t *batt_percentage, uint8_t *status);

/**
 * Switch memory bank
 *
 *
 * @return error value.
 */
void SwitchBank(void);

/* Inline functions definition */
/*******************************/

#ifdef __cplusplus
}
#endif

#endif /* INC_UTILTASK_H_ */
