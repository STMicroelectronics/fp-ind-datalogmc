/**
 ********************************************************************************
 * @file    DataAggregationProcessTask_vtbl.h
 * @author  STMicroelectronics - AIS - MCD Team
 * @version 1.0.0
 * @date    2023-07-06
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

#ifndef ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_VTBL_H_
#define ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* AManagedTask virtual functions */
sys_error_code_t DataAggregationProcessTask_vtblHardwareInit(AManagedTask *_this, void *p_params); /*!< @sa AMTHardwareInit */
sys_error_code_t DataAggregationProcessTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName, VOID **pvStackStart, ULONG *pnStackSize,
                                                UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice, ULONG *pnAutoStart, ULONG *pnParams);         /*!< @sa AMTOnCreateTask     */
sys_error_code_t DataAggregationProcessTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode eActivePowerMode, const EPowerMode eNewPowerMode);                  /*!< @sa AMTDoEnterPowerMode */
sys_error_code_t DataAggregationProcessTask_vtblHandleError(AManagedTask *_this, SysEvent xError); /*!< @sa AMTHandleError            */
sys_error_code_t DataAggregationProcessTask_vtblOnEnterTaskControlLoop(AManagedTask *this);        /*!< @sa AMTOnEnterTaskControlLoop */

/* AManagedTaskEx virtual functions */
sys_error_code_t DataAggregationProcessTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode eActivePowerMode);                                         /*!< @sa AMTExForceExecuteStep */
sys_error_code_t DataAggregationProcessTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode); /*!< @sa AMTExOnEnterPowerMode */


sys_error_code_t DataAggregationProcessTask_OnStatusChange_vtbl(IListener *_this)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

/* ISensorEventListener virtual functions */
sys_error_code_t DataAggregationProcessTask_Sensor_OnStatusChange_vtbl(IListener *_this);
void *DataAggregationProcessTask_Sensor_GetOwner_vtbl(IEventListener *_this);
void DataAggregationProcessTask_Sensor_SetOwner_vtbl(IEventListener *_this, void *p_owner);
sys_error_code_t DataAggregationProcessTask_Sensor_OnNewDataReady_vtbl(IEventListener *_this, const DataEvent_t *p_evt);

sys_error_code_t DataAggregationProcessTask_Actuator_OnStatusChange_vtbl(IListener *_this);
void *DataAggregationProcessTask_Actuator_GetOwner_vtbl(IEventListener *_this);
void DataAggregationProcessTask_Actuator_SetOwner_vtbl(IEventListener *_this, void *p_owner);
sys_error_code_t DataAggregationProcessTask_Actuator_OnNewDataReady_vtbl(IEventListener *_this, const DataEvent_t *p_evt);


uint8_t  DataAggregationProcessTask_GetId_vtbl(ISourceObservable *_this);
IEventSrc* DataAggregationProcessTask_GetEventSourceIF_vtbl(ISourceObservable *_this);
#ifdef __cplusplus
}
#endif

#endif /* ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_VTBL_H_ */
