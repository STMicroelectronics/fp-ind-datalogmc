/**
  ******************************************************************************
  * @file    DatalogAppTask.c
  * @author  SRA
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

#include "DatalogAppTask.h"
#include "DatalogAppTask_vtbl.h"
#include "services/sysdebug.h"
#include "services/sysmem.h"
#include "services/SysTimestamp.h"

#include "filex_dctrl_class.h"
#include "usbx_dctrl_class.h"
#include "ble_stream_class.h"
#include "PnPLCompManager.h"
#include "App_model.h"

#include "UtilTask.h"
#include "ISM330DHCXTask.h"
#include "ISM330ISTask.h"
#include "services/SQuery.h"
#include "services/SUcfProtocol.h"

#include "STWIN.box_debug_pins.h"

#include "automode.h"
#include "rtc.h"

#ifndef DT_TASK_CFG_STACK_DEPTH
#define DT_TASK_CFG_STACK_DEPTH                   (TX_MINIMUM_STACK*2)
#endif

#ifndef DT_TASK_CFG_PRIORITY
#define DT_TASK_CFG_PRIORITY                      (TX_MAX_PRIORITIES-1)
#endif

#ifndef DT_TASK_CFG_IN_QUEUE_LENGTH
#define DT_TASK_CFG_IN_QUEUE_LENGTH               20
#endif

#define DT_TASK_CFG_IN_QUEUE_ITEM_SIZE            sizeof(ULONG)

#define DATALOG_APP_TASK_CFG_TIMER_PERIOD_MS      5000U

// BLE Advertise option byte
#define ADV_OB_ALARM                              0U
#define ADV_OB_ICON                               1U

/* Interface ID definition */
#define COMM_ID_SDCARD                            0U
#define COMM_ID_USB                               1U
#define COMM_ID_BLE                               2U

#define BOOTLOADER_ADDRESS                        0x0BF90000

#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_DT, level, message)

#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  sDatalogAppTaskObj
#endif

/**
  *  DatalogAppTask internal structure.
  */
struct _DatalogAppTask
{
  /**
    * Base class object.
    */
  AManagedTaskEx super;
  TX_QUEUE in_queue;
  ULONG message;

  /** Software timer used to send periodical ble advertise messages **/
  TX_TIMER ble_advertise_timer;

  /** Data Event Listener **/
  IDataEventListener_t sensorListener;

  /** Actuator Data Event Listener **/
  IDataEventListener_t actuatorListener;

  void *owner;

  /** USBX ctrl class **/
  usbx_dctrl_class_t *usbx_device;

  /** FILEX ctrl class **/
  filex_dctrl_class_t *filex_device;

  /** FILEX ctrl class **/
  ble_stream_class_t *ble_device;

  ICommandParse_t parser;
//TODO could be more useful to have a CommandParse Class? (ICommandParse + PnPLCommand_t)
  PnPLCommand_t outPnPLCommand;

  uint8_t iis3dwb_pin;

  AppModel_t *datalog_model;

  SensorContext_t sensorContext[SM_MAX_SENSORS];

  /** We are using the same structure context for both sensors
    *  and actuators.
    *  Can we use a single structure?
    **/
  SensorContext_t actuatorContext[ACTUATOR_NUMBER];

  uint32_t mode;  /* logging interface */

};

/**
  * Class object declaration
  */
typedef struct _DatalogAppTaskClass
{
  /**
    * DatalogAppTask class virtual table.
    */
  AManagedTaskEx_vtbl vtbl;

  /**
    * ICommandParse virtual table.
    */
  ICommandParse_vtbl parserVTBL;

  /**
    * ISensorEventListener virtual table.
    */
  IDataEventListener_vtbl ListenerVTBL;

  /**
    * Actuator  IDataEventListener virtual table.
    */
  IDataEventListener_vtbl ActuatorListenerVTBL;

  /**
    * DatalogAppTask (PM_STATE, ExecuteStepFunc) map.
    */
  pExecuteStepFunc_t p_pm_state2func_map[];
} DatalogAppTaskClass_t;

// Private member function declaration
// ***********************************

/**
  * Execute one step of the task control loop while the system is in RUN mode.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
  */
static sys_error_code_t DatalogAppTaskExecuteStepState1(AManagedTask *_this);

/**
  * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
  */
static sys_error_code_t DatalogAppTaskExecuteStepDatalog(AManagedTask *_this);

/**
  * Callback function called when the software timer, dedicated to the advertise option bytes update, expires.
  *
  * @param xTimer [IN] specifies the handle of the expired timer.
  */
static VOID DatalogAppTaskAdvOBTimerCallbackFunction(ULONG timer);

/**
  * IRQ callback: handles HW IRQ dependency between IIS3DWB IRQ PIN and ISM330IS IRQ PIN
  */
void PIN15_EXTI_Callback(uint16_t nPin);
void IIS3DWBTask_EXTI_Callback(uint16_t nPin);
//void ISM330ISTask_EXTI_Callback(uint16_t nPin);

#if defined (__GNUC__)
// Inline function defined inline in the header file DatalogAppTask.h must be declared here as extern function.
#endif

/* Objects instance */
/********************/

/**
  * The only instance of the task object.
  */
static DatalogAppTask sTaskObj;

/**
  * The class object.
  */
static const DatalogAppTaskClass_t sTheClass =
{
  /* class virtual table */
  {
    DatalogAppTask_vtblHardwareInit,
    DatalogAppTask_vtblOnCreateTask,
    DatalogAppTask_vtblDoEnterPowerMode,
    DatalogAppTask_vtblHandleError,
    DatalogAppTask_vtblOnEnterTaskControlLoop,
    DatalogAppTask_vtblForceExecuteStep,
    DatalogAppTask_vtblOnEnterPowerMode
  },
  {
    DatalogAppTask_vtblICommandParse_t_parse_cmd,
    DatalogAppTask_vtblICommandParse_t_serialize_response,
    DatalogAppTask_vtblICommandParse_t_send_ctrl_msg
  },
  {
    DatalogAppTask_OnStatusChange_vtbl,
    DatalogAppTask_SetOwner_vtbl,
    DatalogAppTask_GetOwner_vtbl,
    DatalogAppTask_OnNewDataReady_vtbl
  },

  {
    DatalogAppTask_ActuatorListenerOnStatusChange_vtbl,
    DatalogAppTask_ActuatorListenerSetOwner_vtbl,
    DatalogAppTask_ActuatorListenerGetOwner_vtbl,
    DatalogAppTask_ActuatorListenerOnNewDataReady_vtbl
  },

  /* class (PM_STATE, ExecuteStepFunc) map */
  {
    DatalogAppTaskExecuteStepState1,
    NULL,
    DatalogAppTaskExecuteStepDatalog
  }
};

// Public API definition
// *********************

AManagedTaskEx *DatalogAppTaskAlloc()
{
  // In this application there is only one Keyboard task,
  // so this allocator implement the singleton design pattern.

  // Initialize the super class
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;
  sTaskObj.parser.vptr = &sTheClass.parserVTBL;
  sTaskObj.sensorListener.vptr = &sTheClass.ListenerVTBL;
  sTaskObj.actuatorListener.vptr = &sTheClass.ActuatorListenerVTBL;

  memset(&sTaskObj.outPnPLCommand, 0, sizeof(PnPLCommand_t));

  return (AManagedTaskEx *) &sTaskObj;
}

DatalogAppTask *getDatalogAppTask(void)
{
  return (DatalogAppTask *) &sTaskObj;
}

IEventListener *DatalogAppTask_GetEventListenerIF(DatalogAppTask *_this)
{
  assert_param(_this);

  return (IEventListener *) &_this->sensorListener;
}

IEventListener *DatalogAppTask_GetActuatorEventListenerIF(DatalogAppTask *_this)
{
  assert_param(_this);

  return (IEventListener *) &_this->actuatorListener;
}

ICommandParse_t *DatalogAppTask_GetICommandParseIF(DatalogAppTask *_this)
{
  assert_param(_this != NULL);
  DatalogAppTask *p_obj = (DatalogAppTask *) _this;

  return &p_obj->parser;

}

sys_error_code_t DatalogAppTask_msg(ULONG msg)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ULONG message = msg;

  if (tx_queue_send(&sTaskObj.in_queue, &message, TX_NO_WAIT) != TX_SUCCESS)
  {
    res = 1;
  }

  return res;
}

// AManagedTask virtual functions definition
// ***********************************************

sys_error_code_t DatalogAppTask_vtblHardwareInit(AManagedTask *_this, void *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  // DatalogAppTask *p_obj = (DatalogAppTask*) _this;

#ifdef ENABLE_THREADX_DBG_PIN
  /* Configure DEBUG PIN */
  BSP_DEBUG_PIN_Off(CON34_PIN_5);
  BSP_DEBUG_PIN_Off(CON34_PIN_12);
  BSP_DEBUG_PIN_Off(CON34_PIN_14);
  BSP_DEBUG_PIN_Off(CON34_PIN_16);
  BSP_DEBUG_PIN_Off(CON34_PIN_22);
  BSP_DEBUG_PIN_Off(CON34_PIN_24);
  BSP_DEBUG_PIN_Off(CON34_PIN_26);
  BSP_DEBUG_PIN_Off(CON34_PIN_28);
  BSP_DEBUG_PIN_Off(CON34_PIN_30);
  BSP_DEBUG_PIN_Off(CON34_PIN_30);

  BSP_DEBUG_PIN_Init(CON34_PIN_5);
  BSP_DEBUG_PIN_Init(CON34_PIN_12);
  BSP_DEBUG_PIN_Init(CON34_PIN_14);
  BSP_DEBUG_PIN_Init(CON34_PIN_16);
  BSP_DEBUG_PIN_Init(CON34_PIN_22);
  BSP_DEBUG_PIN_Init(CON34_PIN_24);
  BSP_DEBUG_PIN_Init(CON34_PIN_26);
  BSP_DEBUG_PIN_Init(CON34_PIN_28);
  BSP_DEBUG_PIN_Init(CON34_PIN_30);
  BSP_DEBUG_PIN_Init(CON34_PIN_30);
#endif

  return res;
}

sys_error_code_t DatalogAppTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *pTaskCode, CHAR **pName,
                                                 VOID **pvStackStart,
                                                 ULONG *pStackDepth, UINT *pPriority, UINT *pPreemptThreshold, ULONG *pTimeSlice, ULONG *pAutoStart,
                                                 ULONG *pParams)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) _this;

  // Create task specific sw resources.

  uint16_t item_size = DT_TASK_CFG_IN_QUEUE_ITEM_SIZE;
  VOID *p_queue_items_buff = SysAlloc(DT_TASK_CFG_IN_QUEUE_LENGTH * item_size);
  if (p_queue_items_buff == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  if (TX_SUCCESS != tx_queue_create(&p_obj->in_queue, "DatalogApp_Q", TX_1_ULONG, p_queue_items_buff,
                                    DT_TASK_CFG_IN_QUEUE_LENGTH * item_size))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* create the software timer for advertise messages*/
  if (TX_SUCCESS != tx_timer_create(&p_obj->ble_advertise_timer, "BLE_ADV_T", DatalogAppTaskAdvOBTimerCallbackFunction,
                                    (ULONG)TX_NULL,
                                    AMT_MS_TO_TICKS(DATALOG_APP_TASK_CFG_TIMER_PERIOD_MS), AMT_MS_TO_TICKS(DATALOG_APP_TASK_CFG_TIMER_PERIOD_MS),
                                    TX_NO_ACTIVATE))
  {
    res = SYS_APP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  uint8_t ii;
  for (ii = 0; ii < SM_MAX_SENSORS; ii++)
  {
    p_obj->sensorContext[ii].n_samples_to_timestamp = 0;
    p_obj->sensorContext[ii].old_time_stamp = -1.0f;
  }

  for (ii = 0; ii < ACTUATOR_NUMBER; ii++)
  {
    p_obj->actuatorContext[ii].n_samples_to_timestamp = 0;
    p_obj->actuatorContext[ii].old_time_stamp = -1.0f;
  }

  p_obj->datalog_model = getAppModel();

  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *pTaskCode = AMTExRun;
  *pName = "DatalogApp";
  *pvStackStart = NULL; // allocate the task stack in the system memory pool.
  *pStackDepth = DT_TASK_CFG_STACK_DEPTH;
  *pParams = (ULONG) _this;
  *pPriority = DT_TASK_CFG_PRIORITY;
  *pPreemptThreshold = DT_TASK_CFG_PRIORITY;
  *pTimeSlice = TX_NO_TIME_SLICE;
  *pAutoStart = TX_AUTO_START;

  IEventListenerSetOwner((IEventListener *) &p_obj->sensorListener, (void *) p_obj);

  p_obj->usbx_device = (usbx_dctrl_class_t *) usbx_dctrl_class_alloc((void *) &MX_PCDInitParams);
  IStream_init((IStream_t *) p_obj->usbx_device, COMM_ID_USB, 0);
  IStream_set_parse_IF((IStream_t *) p_obj->usbx_device, DatalogAppTask_GetICommandParseIF(p_obj));

  p_obj->filex_device = (filex_dctrl_class_t *) filex_dctrl_class_alloc();
  IStream_init((IStream_t *) p_obj->filex_device, COMM_ID_SDCARD, 0);
  IStream_set_parse_IF((IStream_t *) p_obj->filex_device, DatalogAppTask_GetICommandParseIF(p_obj));

  p_obj->ble_device = (ble_stream_class_t *) ble_stream_class_alloc();
  IStream_init((IStream_t *) p_obj->ble_device, COMM_ID_BLE, 0);
  IStream_set_parse_IF((IStream_t *) p_obj->ble_device, DatalogAppTask_GetICommandParseIF(p_obj));
  p_obj->ble_device->adv_id = SM_MAX_SENSORS + 1;

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "iis3dwb", COM_TYPE_ACC);
  if (id != SI_NULL_SENSOR_ID)
  {
    p_obj->iis3dwb_pin = 1;
  }
  else
  {
    p_obj->iis3dwb_pin = 0;
  }

  return res;
}


sys_error_code_t DatalogAppTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode ActivePowerMode,
                                                     const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) _this;
  uint8_t stream_id;

  if (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE && NewPowerMode == E_POWER_MODE_STATE1)
  {
    if (p_obj->datalog_model->log_controller_model.interface == LOG_CTRL_MODE_SD) /*stop command from SD*/
    {
      IStream_stop((IStream_t *) p_obj->filex_device);

      int i;
      for (i = 0; i < SENSOR_NUMBER; i++)
      {
        if (p_obj->datalog_model->s_models[i] != NULL)
        {
          if (p_obj->datalog_model->s_models[i]->sensor_status->is_active)
          {
            stream_id = p_obj->datalog_model->s_models[i]->stream_params.stream_id;
            IStream_dealloc((IStream_t *) p_obj->filex_device, stream_id);
            p_obj->sensorContext[i].n_samples_to_timestamp = 0;
            p_obj->sensorContext[i].old_time_stamp = -1.0f;
          }
        }
      }

      for (i = 0; i < ACTUATOR_NUMBER; i++)
      {
        if (p_obj->datalog_model->ac_models[i] != NULL)
        {
          if (p_obj->datalog_model->ac_models[i]->actuatorStatus.is_active)
          {
            stream_id = p_obj->datalog_model->ac_models[i]->stream_params.stream_id;
            IStream_dealloc((IStream_t *) p_obj->filex_device, stream_id);
            p_obj->actuatorContext[i].n_samples_to_timestamp = 0;
            p_obj->actuatorContext[i].old_time_stamp = -1.0f;
          }
        }
      }

      p_obj->datalog_model->log_controller_model.interface = -1;
      IStream_enable((IStream_t *) p_obj->usbx_device); /*Reactivate usb interface*/
    }
    else if (p_obj->datalog_model->log_controller_model.interface == LOG_CTRL_MODE_USB) /*stop command from USB*/
    {
      IStream_stop((IStream_t *) p_obj->usbx_device);

      int i = 0;
      for (i = 0; i < SENSOR_NUMBER; i++)
      {
        if (p_obj->datalog_model->s_models[i] != NULL)
        {
          if (p_obj->datalog_model->s_models[i]->sensor_status->is_active)
          {
            stream_id = p_obj->datalog_model->s_models[i]->stream_params.stream_id;
            IStream_dealloc((IStream_t *) p_obj->usbx_device, stream_id);
            p_obj->sensorContext[i].n_samples_to_timestamp = 0;
            p_obj->sensorContext[i].old_time_stamp = -1.0f;
          }
        }
      }

      for (i = 0; i < ACTUATOR_NUMBER; i++)
      {
        if (p_obj->datalog_model->ac_models[i] != NULL)
        {
          if (p_obj->datalog_model->ac_models[i]->actuatorStatus.is_active)
          {
            stream_id = p_obj->datalog_model->ac_models[i]->stream_params.stream_id;
            IStream_dealloc((IStream_t *) p_obj->usbx_device, stream_id);
            p_obj->actuatorContext[i].n_samples_to_timestamp = 0;
            p_obj->actuatorContext[i].old_time_stamp = -1.0f;
          }
        }
      }

      p_obj->datalog_model->log_controller_model.interface = -1;
      IStream_enable((IStream_t *) p_obj->filex_device); /*Reactivate sdcard interface*/
    }
    SysTsStop(SysGetTimestampSrv());
  }
  if (NewPowerMode == E_POWER_MODE_SENSORS_ACTIVE)
  {

  }

  return res;
}

sys_error_code_t DatalogAppTask_vtblHandleError(AManagedTask *_this, SysEvent xError)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  DatalogAppTask *p_obj = (DatalogAppTask*)_this;

  return res;
}

sys_error_code_t DatalogAppTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) _this;
  ULONG msg = FILEX_DCTRL_CMD_INIT;

#ifdef ENABLE_THREADX_DBG_PIN
  p_obj->super.m_xTaskHandle.pxTaskTag = DT_TASK_CFG_TAG;
#endif

  IStream_enable((IStream_t *) p_obj->usbx_device);
  IStream_enable((IStream_t *) p_obj->filex_device);
  IStream_enable((IStream_t *) p_obj->ble_device);

  filex_dctrl_msg(p_obj->filex_device, &msg);

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("DatalogApp: start.\r\n"));

  // At this point all system has been initialized.
  // Execute task specific delayed one time initialization.

  return res;
}

sys_error_code_t DatalogAppTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode ActivePowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) _this;

  p_obj->message = DT_FORCE_STEP;

  if ((ActivePowerMode == E_POWER_MODE_STATE1) || (ActivePowerMode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if (AMTExIsTaskInactive(_this))
    {
      if (tx_queue_front_send(&p_obj->in_queue, &p_obj->message, AMT_MS_TO_TICKS(100)) != TX_SUCCESS)
      {
        res = SYS_APP_TASK_MSG_LOST_ERROR_CODE;
        SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_APP_TASK_MSG_LOST_ERROR_CODE);
      }
    }
  }
  else
  {
    UINT state;
    if (TX_SUCCESS == tx_thread_info_get(&_this->m_xTaskHandle, TX_NULL, &state, TX_NULL, TX_NULL, TX_NULL, TX_NULL,
                                         TX_NULL, TX_NULL))
    {
      if (state == TX_SUSPENDED)
      {
        tx_thread_resume(&_this->m_xTaskHandle);
      }
    }
  }

  return res;
}

sys_error_code_t DatalogAppTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode ActivePowerMode,
                                                     const EPowerMode NewPowerMode)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  //  DatalogAppTask *p_obj = (DatalogAppTask*)_this;

  AMTExSetPMClass(_this, E_PM_CLASS_2);

  return res;
}

//// IIListener virtual functions
sys_error_code_t DatalogAppTask_OnStatusChange_vtbl(IListener *_this)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

// ISensorEventListener virtual functions
void *DatalogAppTask_GetOwner_vtbl(IEventListener *_this)
{
  DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, sensorListener));

  return p_obj->owner;
}

void DatalogAppTask_SetOwner_vtbl(IEventListener *_this, void *p_owner)
{
  DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, sensorListener));

  p_obj->owner = p_owner;
}

sys_error_code_t DatalogAppTask_OnNewDataReady_vtbl(IEventListener *_this, const DataEvent_t *p_evt)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) IEventListenerGetOwner(_this);

  uint8_t nPMState = (uint8_t) AMTGetTaskPowerMode((AManagedTask *) p_obj);
  uint16_t sId = p_evt->tag;
  uint32_t nBytesPerSample = SMGetnBytesPerSample(sId);
  uint8_t *data_buf = EMD_Data(p_evt->p_data);
  uint32_t samplesToSend = EMD_GetPayloadSize(p_evt->p_data) / nBytesPerSample;

  if (nPMState == E_POWER_MODE_SENSORS_ACTIVE)
  {
    uint8_t stream_id = p_obj->datalog_model->s_models[sId]->stream_params.stream_id;
    if (res != 0)
    {
      SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
      return res;
    }

    if (p_obj->sensorContext[sId].old_time_stamp == -1.0f)
    {
      p_obj->datalog_model->s_models[sId]->stream_params.ioffset = p_evt->timestamp;
      p_obj->sensorContext[sId].old_time_stamp = p_evt->timestamp;
      p_obj->sensorContext[sId].n_samples_to_timestamp = p_obj->datalog_model->s_models[sId]->stream_params.spts;
    }
    else
    {
      while (samplesToSend > 0)
      {
        /* n_samples_to_timestamp = 0 if user setup spts = 0 (no timestamp needed) */
        if (p_obj->sensorContext[sId].n_samples_to_timestamp == 0
            || samplesToSend < p_obj->sensorContext[sId].n_samples_to_timestamp)
        {
          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, data_buf, samplesToSend * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, data_buf, samplesToSend * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->ble_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->ble_device,  stream_id, data_buf, samplesToSend * nBytesPerSample);
          }
          if (p_obj->sensorContext[sId].n_samples_to_timestamp != 0)
          {
            p_obj->sensorContext[sId].n_samples_to_timestamp -= samplesToSend;
          }
          samplesToSend = 0;
        }
        else
        {
          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, data_buf, p_obj->sensorContext[sId].n_samples_to_timestamp * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, data_buf, p_obj->sensorContext[sId].n_samples_to_timestamp * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->ble_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->ble_device,  stream_id, data_buf, p_obj->sensorContext[sId].n_samples_to_timestamp * nBytesPerSample);
          }

          data_buf += p_obj->sensorContext[sId].n_samples_to_timestamp * nBytesPerSample;
          samplesToSend -= p_obj->sensorContext[sId].n_samples_to_timestamp;

          float measuredODR;
          double newTS;

          SensorStatus_t sensor_status = SMSensorGetStatus(sId);
          if (sensor_status.isensor_class == ISENSOR_CLASS_MEMS)
          {
            measuredODR = sensor_status.type.mems.measured_odr;
          }
          else if (sensor_status.isensor_class == ISENSOR_CLASS_AUDIO)
          {
            measuredODR = sensor_status.type.audio.frequency;
          }
          else
          {
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
            return res;
          }

          if (measuredODR != 0.0f)
          {
            newTS = p_evt->timestamp - ((1.0 / (double) measuredODR) * samplesToSend);
          }
          else
          {
            newTS = p_evt->timestamp;
          }

          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, (uint8_t *) &newTS, 8);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, (uint8_t *) &newTS, 8);
          }
          if (res != 0)
          {
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
            return res;
          }
          p_obj->sensorContext[sId].n_samples_to_timestamp = p_obj->datalog_model->s_models[sId]->stream_params.spts;
        }
      }
    }
  }
  return res;
}

sys_error_code_t DatalogAppTask_ActuatorListenerOnStatusChange_vtbl(IListener *_this)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

// ISensorEventListener virtual functions
void *DatalogAppTask_ActuatorListenerGetOwner_vtbl(IEventListener *_this)
{
  DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, actuatorListener));

  return p_obj->owner;
}

void DatalogAppTask_ActuatorListenerSetOwner_vtbl(IEventListener *_this, void *p_owner)
{
  DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, actuatorListener));

  p_obj->owner = p_owner;
}

sys_error_code_t DatalogAppTask_ActuatorListenerOnNewDataReady_vtbl(IEventListener *_this, const DataEvent_t *p_evt)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DatalogAppTask *p_obj = (DatalogAppTask *) IEventListenerGetOwner(_this);
  uint8_t nPMState = (uint8_t) AMTGetTaskPowerMode((AManagedTask *) p_obj);

  if (nPMState == E_POWER_MODE_SENSORS_ACTIVE)
  {
    DatalogAppTask *p_obj = (DatalogAppTask *) IEventListenerGetOwner(_this);
    uint16_t aId = p_evt->tag;

#if (HSD_USE_DUMMY_DATA != 1)
    uint32_t nBytesPerSample = 0;

    uint16_t data_type_size = EMD_GetElementSize(p_evt->p_data);

    uint16_t data_dimension = EMD_GetShape(p_evt->p_data, EMD_GetDimensions(p_evt->p_data) - 1);
    nBytesPerSample = data_type_size * data_dimension;

    uint8_t *data_buf = EMD_Data(p_evt->p_data);
    uint32_t samplesToSend = EMD_GetPayloadSize(p_evt->p_data) / nBytesPerSample;
#endif

    uint8_t stream_id = p_obj->datalog_model->ac_models[aId]->stream_params.stream_id;

#if (HSD_USE_DUMMY_DATA == 1)
    if (IStream_is_enabled((IStream_t *)p_obj->usbx_device))
    {
      res = IStream_post_data((IStream_t *)p_obj->usbx_device, stream_id, EMD_Data(p_evt->p_data), EMD_GetPayloadSize(p_evt->p_data));
    }
    if (IStream_is_enabled((IStream_t *)p_obj->filex_device))
    {
      res = IStream_post_data((IStream_t *)p_obj->filex_device, stream_id, EMD_Data(p_evt->p_data), EMD_GetPayloadSize(p_evt->p_data));
    }
    if (res != 0)
    {
      while (1);
    }
#else

    if (p_obj->actuatorContext[aId].old_time_stamp == -1.0f)
    {
      p_obj->datalog_model->ac_models[aId]->stream_params.ioffset = p_evt->timestamp; /*TODO: can I use PnPL Setter? no*/
      p_obj->actuatorContext[aId].old_time_stamp = p_evt->timestamp;
      p_obj->actuatorContext[aId].n_samples_to_timestamp = p_obj->datalog_model->ac_models[aId]->stream_params.spts;
    }
    else
    {
      while (samplesToSend > 0)
      {
        /* n_samples_to_timestamp = 0 if user setup spts = 0 (no timestamp needed) */
        if (p_obj->actuatorContext[aId].n_samples_to_timestamp == 0
            || samplesToSend < p_obj->actuatorContext[aId].n_samples_to_timestamp)
        {
          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, data_buf, samplesToSend * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, data_buf, samplesToSend * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->ble_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->ble_device, stream_id, data_buf, samplesToSend * nBytesPerSample);
          }

          if (p_obj->actuatorContext[aId].n_samples_to_timestamp != 0)
          {
            p_obj->actuatorContext[aId].n_samples_to_timestamp -= samplesToSend;
          }
          samplesToSend = 0;
        }
        else
        {
          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, data_buf, p_obj->actuatorContext[aId].n_samples_to_timestamp * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, data_buf, p_obj->actuatorContext[aId].n_samples_to_timestamp * nBytesPerSample);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->ble_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->ble_device, stream_id, data_buf, p_obj->actuatorContext[aId].n_samples_to_timestamp * nBytesPerSample);
          }

          data_buf += p_obj->actuatorContext[aId].n_samples_to_timestamp * nBytesPerSample;
          samplesToSend -= p_obj->actuatorContext[aId].n_samples_to_timestamp;

          double newTS;
          newTS = p_evt->timestamp;

          if (IStream_is_enabled((IStream_t *) p_obj->usbx_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->usbx_device, stream_id, (uint8_t *) &newTS, 8);
          }
          if (IStream_is_enabled((IStream_t *) p_obj->filex_device))
          {
            res = IStream_post_data((IStream_t *) p_obj->filex_device, stream_id, (uint8_t *) &newTS, 8);
          }
          if (res != 0)
          {
            SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
            return res;
          }
          p_obj->actuatorContext[aId].n_samples_to_timestamp = p_obj->datalog_model->ac_models[aId]->stream_params.spts;
        }
      }
    }
  }
#endif
    return res;
  }

  /* ICommandParse_t virtual functions */
  sys_error_code_t DatalogAppTask_vtblICommandParse_t_parse_cmd(ICommandParse_t *_this, char *commandString,
                                                                uint8_t comm_interface_id)
  {
    assert_param(_this);
    sys_error_code_t res = SYS_NO_ERROR_CODE;

    DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, parser));

    int pnp_res = PnPLParseCommand(commandString, &p_obj->outPnPLCommand);
    p_obj->outPnPLCommand.comm_interface_id = comm_interface_id;

    if (pnp_res == SYS_NO_ERROR_CODE)
    {
      if (IStream_is_enabled((IStream_t *) p_obj->usbx_device) && (comm_interface_id == COMM_ID_USB))
      {
        if (p_obj->outPnPLCommand.comm_type != PNPL_CMD_COMMAND)
        {
          IStream_set_mode((IStream_t *) p_obj->usbx_device, TRANSMIT);
        }
      }
      else if (IStream_is_enabled((IStream_t *) p_obj->filex_device) && (comm_interface_id == COMM_ID_SDCARD))
      {
        IStream_set_mode((IStream_t *) p_obj->filex_device, TRANSMIT);
      }
      else if (IStream_is_enabled((IStream_t *) p_obj->ble_device) && (comm_interface_id == COMM_ID_BLE))
      {
        if (p_obj->outPnPLCommand.comm_type != PNPL_CMD_COMMAND)
        {
          IStream_set_mode((IStream_t *) p_obj->ble_device, TRANSMIT);
        }
      }
      else
      {
        /**/
      }
    }
    else
    {
      if (p_obj->outPnPLCommand.comm_type == PNPL_CMD_ERROR)
      {
        if (IStream_is_enabled((IStream_t *) p_obj->usbx_device) && (comm_interface_id == COMM_ID_USB))
        {
          IStream_set_mode((IStream_t *) p_obj->usbx_device, TRANSMIT);
        }
        else if (IStream_is_enabled((IStream_t *) p_obj->ble_device) && (comm_interface_id == COMM_ID_BLE))
        {
          IStream_set_mode((IStream_t *) p_obj->ble_device, TRANSMIT);
        }
      }
    }

    return res;
  }

  sys_error_code_t DatalogAppTask_vtblICommandParse_t_serialize_response(ICommandParse_t *_this, char **response_name,
                                                                         char **buff, uint32_t *size,
                                                                         uint8_t pretty)
  {
    assert_param(_this);
    sys_error_code_t res = SYS_NO_ERROR_CODE;
    DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, parser));

//  if (p_obj->outPnPLCommand.comm_type == PNPL_CMD_GET || p_obj->outPnPLCommand.comm_type == PNPL_CMD_SYSTEM_INFO
//      || p_obj->outPnPLCommand.comm_type == PNPL_CMD_ERROR)
//  {
    PnPLSerializeResponse(&p_obj->outPnPLCommand, buff, size, pretty);
//  }
    *response_name = p_obj->outPnPLCommand.comp_name;

    return res;
  }


  sys_error_code_t DatalogAppTask_vtblICommandParse_t_send_ctrl_msg(ICommandParse_t *_this,  uint32_t *msg,
                                                                    uint32_t length)
  {
    assert_param(_this);
    sys_error_code_t res = SYS_NO_ERROR_CODE;
    DatalogAppTask *p_obj = (DatalogAppTask *)((uint32_t) _this - offsetof(DatalogAppTask, parser));

    switch (*msg)
    {
      case BLE_ISTREAM_MSG_START_ADV_OB:

        if (TX_SUCCESS != tx_timer_activate(&p_obj->ble_advertise_timer))
        {
          res = SYS_APP_TASK_TIMER_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
        }

        break;

      case BLE_ISTREAM_MSG_STOP_ADV_OB:

        if (TX_SUCCESS != tx_timer_deactivate(&p_obj->ble_advertise_timer))
        {
          res = SYS_APP_TASK_TIMER_ERROR_CODE;
          SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
        }

        break;

      default:
        res = SYS_INVALID_PARAMETER_ERROR_CODE;
        break;
    }

    return res;
  }

// ILogController_t virtual functions
  uint8_t DatalogAppTask_start(int32_t interface)
  {
    DatalogAppTask *p_obj = getDatalogAppTask();
    SysTsStart(SysGetTimestampSrv(), TRUE);
    bool status;
    uint8_t stream_id;

    log_controller_get_log_status(&status);

    if (!status)
    {

      p_obj->datalog_model->acquisition_info_model.interface = interface;

      if (interface == LOG_CTRL_MODE_SD) /*Start log on SD*/
      {
        uint32_t sd_dps;

        IStream_disable((IStream_t *) p_obj->usbx_device);
        if (IStream_is_enabled((IStream_t *) p_obj->filex_device) == FALSE)
        {
          if (IStream_enable((IStream_t *) p_obj->filex_device) != SYS_NO_ERROR_CODE)
          {
            /* TODO: send msg to util task or error led;*/
            return 1;
          }
        }

        IStream_start((IStream_t *) p_obj->filex_device, 0);

        int i;
        for (i = 0; i < SENSOR_NUMBER; i++)
        {
          if (p_obj->datalog_model->s_models[i] != NULL)
          {
            if (p_obj->datalog_model->s_models[i]->sensor_status->is_active)
            {
              stream_id = p_obj->datalog_model->s_models[i]->stream_params.stream_id;
              sd_dps = p_obj->datalog_model->s_models[i]->stream_params.sd_dps;
              IStream_alloc_resource((IStream_t *) p_obj->filex_device, stream_id, sd_dps,
                                     p_obj->datalog_model->s_models[i]->comp_name);
            }
          }
        }

        for (i = 0; i < ACTUATOR_NUMBER; i++)
        {
          if (p_obj->datalog_model->ac_models[i] != NULL)
          {
            if (p_obj->datalog_model->ac_models[i]->actuatorStatus.is_active)
            {
              stream_id = p_obj->datalog_model->ac_models[i]->stream_params.stream_id;
              sd_dps = p_obj->datalog_model->ac_models[i]->stream_params.sd_dps;
              IStream_alloc_resource((IStream_t *) p_obj->filex_device, stream_id, sd_dps,
                                     p_obj->datalog_model->ac_models[i]->comp_name);
            }
          }
        }


        p_obj->datalog_model->log_controller_model.status = TRUE;

        /* generate the system event.*/
        SysEvent evt =
        {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_DATALOG, 0)
        };
        SysPostPowerModeEvent(evt);
      }
      else if (interface == LOG_CTRL_MODE_USB) /*Start log on USB*/
      {
        uint16_t usb_dps;
        int8_t usb_ep;

        IStream_disable((IStream_t *) p_obj->filex_device);
        if (IStream_is_enabled((IStream_t *) p_obj->usbx_device) == FALSE)
        {
          IStream_enable((IStream_t *) p_obj->usbx_device);
        }

        int i = 0;
        for (i = 0; i < SENSOR_NUMBER; i++)
        {
          if (p_obj->datalog_model->s_models[i] != NULL)
          {
            if (p_obj->datalog_model->s_models[i]->sensor_status->is_active)
            {
              stream_id = p_obj->datalog_model->s_models[i]->stream_params.stream_id;
              usb_ep = p_obj->datalog_model->s_models[i]->stream_params.usb_ep;
              usb_dps = p_obj->datalog_model->s_models[i]->stream_params.usb_dps;
              /** use to set ep**/
              usbx_dctrl_class_set_ep(p_obj->usbx_device, stream_id, usb_ep);
              IStream_alloc_resource((IStream_t *) p_obj->usbx_device, stream_id, usb_dps,
                                     p_obj->datalog_model->s_models[i]->comp_name);
            }
          }
        }

        for (i = 0; i < ACTUATOR_NUMBER; i++)
        {
          if (p_obj->datalog_model->ac_models[i] != NULL)
          {
            if (p_obj->datalog_model->ac_models[i]->actuatorStatus.is_active)
            {
              stream_id = p_obj->datalog_model->ac_models[i]->stream_params.stream_id;
              usb_ep = p_obj->datalog_model->ac_models[i]->stream_params.usb_ep;
              usb_dps = p_obj->datalog_model->ac_models[i]->stream_params.usb_dps;
              /** use to set ep**/
              usbx_dctrl_class_set_ep(p_obj->usbx_device, stream_id, usb_ep);
              IStream_alloc_resource((IStream_t *) p_obj->usbx_device, stream_id, usb_dps,
                                     p_obj->datalog_model->ac_models[i]->comp_name);
            }
          }
        }

        IStream_start((IStream_t *) p_obj->usbx_device, 0);

        p_obj->datalog_model->log_controller_model.status = TRUE;

        /* generate the system event.*/
        SysEvent evt =
        {
          .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_DATALOG, 0)
        };
        SysPostPowerModeEvent(evt);
      }
      else
      {
        /*Start command from other interfaces: not implemented*/
      }
    }

    char *responseJSON;
    uint32_t size;
    char *message = "";
    PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, true);
    DatalogApp_Task_command_response(responseJSON, size);

    return 0;
  }

  uint8_t DatalogAppTask_stop(void)
  {
    DatalogAppTask *p_obj = getDatalogAppTask();
    bool status;
    log_controller_get_log_status(&status);

    if (status)
    {
      p_obj->datalog_model->log_controller_model.status = FALSE;

      SysEvent evt =
      {
        .nRawEvent = SYS_PM_MAKE_EVENT(SYS_PM_EVT_SRC_DATALOG, 0)
      };
      SysPostPowerModeEvent(evt);
    }

    char *responseJSON;
    uint32_t size;
    char *message = "";
    PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, true);
    DatalogApp_Task_command_response(responseJSON, size);

    return 0;
  }

  void DatalogApp_Task_command_response(char *response_msg, uint32_t size)
  {
    DatalogAppTask *p_obj = getDatalogAppTask();
    uint8_t comm_interface_id = p_obj->outPnPLCommand.comm_interface_id;
    if (IStream_is_enabled((IStream_t *) p_obj->usbx_device) && (comm_interface_id == COMM_ID_USB))
    {
      p_obj->outPnPLCommand.comm_type = PNPL_CMD_COMMAND;
      p_obj->outPnPLCommand.response = (char *)pnpl_malloc(size + 1);
      strncpy(p_obj->outPnPLCommand.response, response_msg, size + 1);

      IStream_set_mode((IStream_t *) p_obj->usbx_device, TRANSMIT);
    }

    /* Clear response_msg after sending */
    pnpl_free(response_msg);
  }


  uint8_t DatalogAppTask_save_config(void)
  {
    DatalogAppTask *p_obj = getDatalogAppTask();
    //TODO Save current board configuration into the mounted SD Card
    ULONG msg = FILEX_DCTRL_CMD_SET_DEFAULT_STATUS;
    filex_dctrl_msg(p_obj->filex_device, &msg);

    char *responseJSON;
    uint32_t size;
    char *message = "";
    PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, true);
    DatalogApp_Task_command_response(responseJSON, size);

    return 0;
  }

  uint8_t DatalogAppTask_set_time(const char *datetime)
  {
    char datetimeStr[3];

    //internal input format: yyyyMMdd_hh_mm_ss

    RTC_DateTypeDef sdate;
    RTC_TimeTypeDef stime;

    /** extract year string (only the last two digit). It will be necessary to add 2000*/
    datetimeStr[0] = datetime[2];
    datetimeStr[1] = datetime[3];
    datetimeStr[2] = '\0';
    sdate.Year = atoi(datetimeStr);

    /** extract month string */
    datetimeStr[0] = datetime[4];
    datetimeStr[1] = datetime[5];
    sdate.Month = atoi(datetimeStr);

    /** extract day string */
    datetimeStr[0] = datetime[6];
    datetimeStr[1] = datetime[7];
    sdate.Date = atoi(datetimeStr);

    /** Week day initialization (not used)*/
    sdate.WeekDay = RTC_WEEKDAY_MONDAY; //Not used

    /** extract hour string */
    datetimeStr[0] = datetime[9];
    datetimeStr[1] = datetime[10];
    stime.Hours = atoi(datetimeStr);

    /** extract minute string */
    datetimeStr[0] = datetime[12];
    datetimeStr[1] = datetime[13];
    stime.Minutes = atoi(datetimeStr);

    /** extract second string */
    datetimeStr[0] = datetime[15];
    datetimeStr[1] = datetime[16];
    stime.Seconds = atoi(datetimeStr);

    /** not used */
    //stime.TimeFormat = RTC_HOURFORMAT12_AM;
    stime.SecondFraction = 0;
    stime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    stime.StoreOperation = RTC_STOREOPERATION_RESET;

    char *responseJSON;
    uint32_t size;
    char *message = "";

    if (HAL_RTC_SetTime(&hrtc, &stime, RTC_FORMAT_BIN) != HAL_OK)
    {
      message = "Error: Failed to set RTC time";
      PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, false);
      DatalogApp_Task_command_response(responseJSON, size);
      while (1)
        ;
    }
    if (HAL_RTC_SetDate(&hrtc, &sdate, RTC_FORMAT_BIN) != HAL_OK)
    {
      message = "Error: Failed to set RTC date";
      PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, false);
      DatalogApp_Task_command_response(responseJSON, size);
      while (1)
        ;
    }

    PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, true);
    DatalogApp_Task_command_response(responseJSON, size);

    return 0;
  }


// Private function definition
// ***************************

  static sys_error_code_t DatalogAppTaskExecuteStepState1(AManagedTask *_this)
  {
    assert_param(_this != NULL);
    sys_error_code_t res = SYS_NO_ERROR_CODE;
    DatalogAppTask *p_obj = (DatalogAppTask *) _this;
    AMTExSetInactiveState((AManagedTaskEx *) _this, TRUE);
    if (TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &p_obj->message, TX_WAIT_FOREVER))
    {
      AMTExSetInactiveState((AManagedTaskEx *) _this, FALSE);

      if (p_obj->message == DT_FORCE_STEP)
      {
        __NOP();
      }
    }

    return res;
  }

  static sys_error_code_t DatalogAppTaskExecuteStepDatalog(AManagedTask *_this)
  {
    assert_param(_this);
    sys_error_code_t res = SYS_NO_ERROR_CODE;
    DatalogAppTask *p_obj = (DatalogAppTask *) _this;

    AMTExSetInactiveState((AManagedTaskEx *) _this, TRUE);
    if (TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &p_obj->message, TX_WAIT_FOREVER))
    {
      AMTExSetInactiveState((AManagedTaskEx *) _this, FALSE);

      if (p_obj->message == DT_FORCE_STEP)
      {
        __NOP();
      }
      else   /* ??? is it correct to forward all messages to SDCard?*/
      {
        filex_dctrl_msg(p_obj->filex_device, &p_obj->message);
      }
    }

    return res;
  }

  static VOID DatalogAppTaskAdvOBTimerCallbackFunction(ULONG timer)
  {

    /* update BLE advertise option bytes */
    uint8_t adv_option_bytes[3];
    bool log_status = 0;
    bool SD_Detected;
    log_controller_get_sd_mounted(&SD_Detected);

    log_controller_get_log_status(&log_status);

    if (!SD_Detected)
    {
      adv_option_bytes[ADV_OB_ALARM] = 1;
      adv_option_bytes[ADV_OB_ICON] = 1;
    }
    else if (log_status)
    {
      adv_option_bytes[ADV_OB_ALARM] = 2;
      adv_option_bytes[ADV_OB_ICON] = 2;
    }
    else
    {
      adv_option_bytes[ADV_OB_ALARM] = 0;
      adv_option_bytes[ADV_OB_ICON] = 0;
    }
    adv_option_bytes[2] = 0;

    IStream_post_data((IStream_t *) sTaskObj.ble_device, sTaskObj.ble_device->adv_id, adv_option_bytes, 3);

    streamMsg_t msg;
    msg.messageId = BLE_ISTREAM_MSG_UPDATE_ADV;
    msg.streamID = sTaskObj.ble_device->adv_id;
    ble_stream_msg(&msg);

  }

  uint8_t DatalogAppTask_switch_bank(void)
  {
    /*putMessage switch */
    DatalogAppTask_msg((ULONG) DT_SWITCH_BANK);

    char *responseJSON;
    uint32_t size;
    char *message = "";
    PnPLSerializeCommandResponse(&responseJSON, &size, 0, message, true);
    DatalogApp_Task_command_response(responseJSON, size);
    return 0;
  }

  uint8_t DatalogAppTask_set_dfu_mode(void)
  {
    /*  Disable interrupts for timers */
    HAL_NVIC_DisableIRQ(TIM6_IRQn);

    /*  Disable ICACHE */
    HAL_ICACHE_DeInit();

    /* Jump to user application */
    typedef  void (*pFunction)(void);
    pFunction JumpToApplication;
    uint32_t JumpAddress;
    JumpAddress = *(__IO uint32_t *)(BOOTLOADER_ADDRESS + 4);
    JumpToApplication = (pFunction) JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *) BOOTLOADER_ADDRESS);
    JumpToApplication();
    return 0;
  }


// Interrupt callback
// ***************************
  void PIN15_EXTI_Callback(uint16_t nPin)
  {
    if (sTaskObj.iis3dwb_pin == 1)
    {
      IIS3DWBTask_EXTI_Callback(nPin);
    }
//  else
//  {
//    ISM330ISTask_EXTI_Callback(nPin);
//  }
  }
