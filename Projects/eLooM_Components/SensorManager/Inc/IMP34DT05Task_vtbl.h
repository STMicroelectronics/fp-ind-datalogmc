/**
  ******************************************************************************
  * @file    IMP34DT05Task_vtbl.h
  * @author  SRA - MCD
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
#ifndef IMP34DT05TASK_VTBL_H_
#define IMP34DT05TASK_VTBL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* AManagedTask virtual functions */
sys_error_code_t IMP34DT05Task_vtblHardwareInit(AManagedTask *_this, void *pParams); ///< @sa AMTHardwareInit
sys_error_code_t IMP34DT05Task_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pvTaskCode, CHAR **pcName,
                                                VOID **pvStackStart, ULONG *pnStackSize, UINT *pnPriority, UINT *pnPreemptThreshold, ULONG *pnTimeSlice,
                                                ULONG *pnAutoStart, ULONG *pnParams); ///< @sa AMTOnCreateTask
sys_error_code_t IMP34DT05Task_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                    const EPowerMode NewPowerMode); ///< @sa AMTDoEnterPowerMode
sys_error_code_t IMP34DT05Task_vtblHandleError(AManagedTask *_this, SysEvent Error); ///< @sa AMTHandleError
sys_error_code_t IMP34DT05Task_vtblOnEnterTaskControlLoop(AManagedTask *this); ///< @sa AMTOnEnterTaskControlLoop

/* AManagedTaskEx virtual functions */
sys_error_code_t IMP34DT05Task_vtblForceExecuteStep(AManagedTaskEx *_this,
                                                    EPowerMode ActivePowerMode); ///< @sa AMTExForceExecuteStep
sys_error_code_t IMP34DT05Task_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                    const EPowerMode NewPowerMode); ///< @sa AMTExOnEnterPowerMode

uint8_t IMP34DT05Task_vtblMicGetId(ISourceObservable *_this);
IEventSrc *IMP34DT05Task_vtblGetEventSourceIF(ISourceObservable *_this);
EMData_t IMP34DT05Task_vtblMicGetDataInfo(ISourceObservable *_this);
sys_error_code_t IMP34DT05Task_vtblSensorEnable(ISensor_t *_this);
sys_error_code_t IMP34DT05Task_vtblSensorDisable(ISensor_t *_this);
boolean_t IMP34DT05Task_vtblSensorIsEnabled(ISensor_t *_this);
SensorDescriptor_t IMP34DT05Task_vtblSensorGetDescription(ISensor_t *_this);
SensorStatus_t IMP34DT05Task_vtblSensorGetStatus(ISensor_t *_this);
SensorStatus_t *IMP34DT05Task_vtblSensorGetStatusPointer(ISensor_t *_this);
uint32_t IMP34DT05Task_vtblMicGetFrequency(ISensorAudio_t *_this);
uint8_t IMP34DT05Task_vtblMicGetVolume(ISensorAudio_t *_this);
uint8_t IMP34DT05Task_vtblMicGetResolution(ISensorAudio_t *_this);
sys_error_code_t IMP34DT05Task_vtblSensorSetFrequency(ISensorAudio_t *_this, uint32_t frequency);
sys_error_code_t IMP34DT05Task_vtblSensorSetVolume(ISensorAudio_t *_this, uint8_t volume);
sys_error_code_t IMP34DT05Task_vtblSensorSetResolution(ISensorAudio_t *_this, uint8_t bit_depth);

#ifdef __cplusplus
}
#endif

#endif /* IMP34DT05TASK_VTBL_H_ */
