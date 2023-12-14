/**
 ********************************************************************************
 * @file    MCPTask_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    2023-02-14
 *
 * @brief
 *
 * <ADD_FILE_DESCRIPTION>
 *
 ********************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ********************************************************************************
 */

#ifndef ELOOM_AMANAGED_TASK_EX_MCPTASK_VTBL_H_
#define ELOOM_AMANAGED_TASK_EX_MCPTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* AManagedTask virtual functions */
sys_error_code_t MCPTask_vtblHardwareInit(AManagedTask *_this, void *p_params); /*!< @sa AMTHardwareInit */
sys_error_code_t MCPTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName, VOID **pvStackStart, ULONG *pnStackSize,
                                                UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart, ULONG *pnParams);         /*!< @sa AMTOnCreateTask     */
sys_error_code_t MCPTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode);                  /*!< @sa AMTDoEnterPowerMode */
sys_error_code_t MCPTask_vtblHandleError(AManagedTask *_this, SysEvent xError); /*!< @sa AMTHandleError            */
sys_error_code_t MCPTask_vtblOnEnterTaskControlLoop(AManagedTask *this);        /*!< @sa AMTOnEnterTaskControlLoop */

/* IMotor_Controller_vtbl virtual functions */
uint8_t MCPTask_connect_vtbl(IMotor_Controller_t * _this);
uint8_t MCPTask_start_motor_vtbl(IMotor_Controller_t * _this);
uint8_t MCPTask_stop_motor_vtbl(IMotor_Controller_t * _this);
uint8_t MCPTask_motor_reset_vtbl(IMotor_Controller_t * _this);
uint8_t MCPTask_ack_fault_vtbl(IMotor_Controller_t * _this);

/* AManagedTaskEx virtual functions */
sys_error_code_t MCPTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode eActivePowerMode);                                         /*!< @sa AMTExForceExecuteStep */
sys_error_code_t MCPTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */


#ifdef __cplusplus
}
#endif

#endif /* ELOOM_AMANAGED_TASK_EX_MCPTASK_VTBL_H_ */
