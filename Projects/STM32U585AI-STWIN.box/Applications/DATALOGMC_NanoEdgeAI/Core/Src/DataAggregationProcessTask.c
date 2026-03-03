/**
  ********************************************************************************
  * @file    DataAggregationProcessTask.c
  * @author  STMicroelectronics - SRA
  * @version 2.0.0
  * @date    2025-10-13
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


#include "DataAggregationProcessTask.h"
#include "DataAggregationProcessTask_vtbl.h"
#include "services/sysmem.h"
#include "services/sysdebug.h"
#include "SensorManager.h"
#include "ADPU2.h"


#ifndef DATAAGGREGATIONPROCESSTASK_CFG_STACK_DEPTH
#define DATAAGGREGATIONPROCESSTASK_CFG_STACK_DEPTH        (TX_MINIMUM_STACK*40)
#endif

#ifndef DATAAGGREGATIONPROCESSTASK_CFG_PRIORITY
#define DATAAGGREGATIONPROCESSTASK_CFG_PRIORITY           (TX_MAX_PRIORITIES-1)
#endif

//#define SYS_DEBUGF(level, message)                 SYS_DEBUGF3(SYS_DBG_DATAAGGREGATIONPROCESSTASK, level, message)


/* Data aggregation and process queue item size */
#define DAP_TASK_CFG_IN_QUEUE_ITEM_SIZE   sizeof(ULONG)

/* Data aggregation and process queue length */
#define DAP_TASK_CFG_IN_QUEUE_LENGTH  (10u)

#define SENSOR_DATASOURCE_NUM_OF_AXIS     3u   /**< Number of axes for the sensor data source */
#define SENSOR_DATASOURCE_DIM             256u /**< Base dimension size for the sensor data source */
#define SENSOR_DATASOURCE_DATA_TYPE_DIM   4u   /**< Data type size (in bytes) for sensor data elements */
#define SENSOR_DATASOURCE_NUM_OF_CHUNK    4u   /**< Number of chunks for the sensor data source */

#define ACTUATOR_DATASOURCE_NUM_OF_AXIS   1u   /**< Number of axes for the actuator data source */
#define ACTUATOR_DATASOURCE_DIM           1024u/**< Base dimension size for the actuator data source */
#define ACTUATOR_DATASOURCE_DATA_TYPE_DIM 4u   /**< Data type size (in bytes) for actuator data elements */
#define ACTUATOR_DATASOURCE_NUM_OF_CHUNK  1u   /**< Number of chunks for the actuator data source */

#define SENSOR_DATASOURCE_BUFFER_DIM      (SENSOR_DATASOURCE_DIM * \
                                           SENSOR_DATASOURCE_NUM_OF_AXIS * \
                                           SENSOR_DATASOURCE_NUM_OF_CHUNK) /**< Total buffer dimension for sensor data source (dim * axes * chunks) */

#define ACTUATOR_DATASOURCE_BUFFER_DIM    (ACTUATOR_DATASOURCE_DIM * \
                                           ACTUATOR_DATASOURCE_NUM_OF_AXIS * \
                                           ACTUATOR_DATASOURCE_NUM_OF_CHUNK) /**< Total buffer dimension for actuator data source (dim * axes * chunks) */

#define SENSOR_DATASOURCE_BUFFER_BYTE_DIM   (SENSOR_DATASOURCE_BUFFER_DIM * SENSOR_DATASOURCE_DATA_TYPE_DIM) /**< Total buffer size in bytes for sensor data source */

#define ACTUATOR_DATASOURCE_BUFFER_BYTE_DIM (ACTUATOR_DATASOURCE_BUFFER_DIM * ACTUATOR_DATASOURCE_DATA_TYPE_DIM) /**< Total buffer size in bytes for actuator data source */

#define AGGREGATED_EMDATA_DIM               (SENSOR_DATASOURCE_BUFFER_DIM + ACTUATOR_DATASOURCE_BUFFER_DIM)

#define ACTUATOR_BUFFER_COPY_DIM          ACTUATOR_DATASOURCE_DIM

#define AGGREGATED_TELEMETRY_EMDATA_TAG   2u
#define AI_DPU_NUM_OF_CB_ITEM         5u
#define AI_DPU_EMDATA_TAG             0u
#define DECIMATION                    4u


/* Private function declaration */
static void DataAggregationProcessTask_SendDataAggregationMsg(DataAggregationProcessTask_t *p_obj);
static void DataAggregationProcessTask_AITaskDpuCallback(IDPU2_t *_this, void *p_param);
static void DataAggregationProcessTask_ArrayDecimation(int16_t *p_array, float_t *p_output, uint32_t len,
                                                       uint8_t decimation, uint8_t axis);

/* Inline private function declaration */
inline static void DataAggregationProcessTask_PerformDataAggregation(DataAggregationProcessTask_t *p_obj);
static inline sys_error_code_t DataAggregationProcessTask_PostReportToFront(DataAggregationProcessTask_t *_this);
inline static void DataAggregationProcessTask_PerformPadding(int16_t *p_buffer, uint32_t starting_ele,
                                                             uint32_t num_ele);


/* Data Aggregation and Process message queue*/
typedef enum
{
  DATA_AGGREGATION = 0,
  DPU_READY_TO_PROCESS,
  DAT_FORCE_STEP
} AggregationProcessQueueMsg_t;

/**
  * Class object declaration.
  */
typedef struct _DataAggregationProcessTaskClass
{
  /**
    * DataAggregationProcessTask class virtual table.
    */
  AManagedTaskEx_vtbl vtbl;

  IDataEventListener_vtbl sensorDataSourceListener;

  IDataEventListener_vtbl actuatorDataSourceListener;

  ISourceObservable_vtbl AIDPUInputif;

  /**
    * UtilTask class (PM_STATE, ExecuteStepFunc) map. The map is implemented with an array and
    * the key is the index. Number of items of this array must be equal to the number of PM state
    * of the application. If the managed task does nothing in a PM state, then set to NULL the
    * relative entry in the map.
    */
  pExecuteStepFunc_t p_pm_state2func_map[];

} DataAggregationProcessTaskClass_t;


static int16_t actuator_buffer_copy[ACTUATOR_BUFFER_COPY_DIM];


/* Private member function declaration */
/***************************************/

/**
  * Execute one step of the task control loop while the system is in STATE1 state.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
  */
static sys_error_code_t DataAggregationProcessTask_ExecuteStepState1(AManagedTask *_this);

/**
  * Execute one step of the task control loop while the system is in SENSORS_ACTIVE mode.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
  */
static sys_error_code_t DataAggregationProcessTask_ExecuteStepDataLog(AManagedTask *_this);


/* Module objects instance */
/***************************/

/**
  * The class object.
  */
static const DataAggregationProcessTaskClass_t sTheClass =
{
  /* class virtual table */
  {
    DataAggregationProcessTask_vtblHardwareInit,
    DataAggregationProcessTask_vtblOnCreateTask,
    DataAggregationProcessTask_vtblDoEnterPowerMode,
    DataAggregationProcessTask_vtblHandleError,
    DataAggregationProcessTask_vtblOnEnterTaskControlLoop,
    DataAggregationProcessTask_vtblForceExecuteStep,
    DataAggregationProcessTask_vtblOnEnterPowerMode
  },

  /* Sensor Data Source Listener */
  {
    DataAggregationProcessTask_Sensor_OnStatusChange_vtbl,
    DataAggregationProcessTask_Sensor_SetOwner_vtbl,
    DataAggregationProcessTask_Sensor_GetOwner_vtbl,
    DataAggregationProcessTask_Sensor_OnNewDataReady_vtbl
  },

  /* Actuator Data Source Listener */
  {
    DataAggregationProcessTask_Actuator_OnStatusChange_vtbl,
    DataAggregationProcessTask_Actuator_SetOwner_vtbl,
    DataAggregationProcessTask_Actuator_GetOwner_vtbl,
    DataAggregationProcessTask_Actuator_OnNewDataReady_vtbl
  },

  {
    DataAggregationProcessTask_GetId_vtbl,
    DataAggregationProcessTask_GetEventSourceIF_vtbl,
    NULL,
  },

  /* class (PM_STATE, ExecuteStepFunc) map */
  {
    DataAggregationProcessTask_ExecuteStepState1,
    NULL,
    DataAggregationProcessTask_ExecuteStepDataLog
  }
};


/* Public API definition */
/*************************/

AManagedTaskEx *DataAggregationProcessTask_Alloc()
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *) SysAlloc(sizeof(DataAggregationProcessTask_t));
  if (p_obj != NULL)
  {
    /* Initialize the super class */
    AMTInitEx(&p_obj->super);
    p_obj->super.vptr = &sTheClass.vtbl;
    p_obj->actuatorDataSourceListener.vptr = &sTheClass.actuatorDataSourceListener;
    p_obj->sensorDataSourceListener.vptr = &sTheClass.sensorDataSourceListener;
    p_obj->ai_dpu_input_if.vptr = &sTheClass.AIDPUInputif;
  }

  return (AManagedTaskEx *)p_obj;
}

IEventListener *DataAggregationProcessTask_GetSensorEventListenerIF(DataAggregationProcessTask_t *_this)
{
  assert_param(_this);
  return (IEventListener *) &_this->sensorDataSourceListener;
}

IEventListener *DataAggregationProcessTask_GetActuatorEventListenerIF(DataAggregationProcessTask_t *_this)
{
  assert_param(_this);
  return (IEventListener *) &_this->actuatorDataSourceListener;
}

/**
  * Get the ::IEventSrc interface for the aggregated and process task.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface of the sensor.
  */
IEventSrc *DataAggregationProcessTask_GetEventSrcIF(DataAggregationProcessTask_t *_this)
{
  assert_param(_this != NULL);
  return (IEventSrc *) _this->p_event_src;
}

IDPU2_t *DataAggregationProcessTask_GetClassifierDPU(DataAggregationProcessTask_t *_this)
{
  assert_param(_this != NULL);
  return (IDPU2_t *)_this->p_AImotorCtrlDPU;
}

void DataAggregationProcessTask_SetClassifierDPU(DataAggregationProcessTask_t *_this, IDPU2_t *p_IDPU2_If)
{
  assert_param(_this != NULL);
  assert_param(_this != NULL);

  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)_this;
  p_obj->p_AImotorCtrlDPU = p_IDPU2_If;
}


ISourceObservable *DataAggregationProcessTask_GetDPUSourceObservable(DataAggregationProcessTask_t *_this)
{
  assert_param(_this != NULL);
  return &(_this->ai_dpu_input_if);

}
/* AManagedTask virtual functions definition */
/*********************************************/

sys_error_code_t DataAggregationProcessTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)_this;

  /* Initialize Aggregation data source buffer  */
  p_obj->sensor_dataSource.len = SENSOR_DATASOURCE_BUFFER_BYTE_DIM;
  p_obj->sensor_dataSource.p_data = SysAlloc(p_obj->sensor_dataSource.len);
  p_obj->sensor_dataSource.data_idx = 0;

  p_obj->actuator_dataSource.len = ACTUATOR_DATASOURCE_BUFFER_BYTE_DIM;
  p_obj->actuator_dataSource.p_data = SysAlloc(p_obj->actuator_dataSource.len);
  p_obj->actuator_dataSource.data_idx = 0;

  p_obj->aggregated_dataSource.len = ACTUATOR_DATASOURCE_BUFFER_BYTE_DIM + SENSOR_DATASOURCE_BUFFER_BYTE_DIM;
  p_obj->aggregated_dataSource.p_data = SysAlloc(p_obj->aggregated_dataSource.len);
  p_obj->aggregated_dataSource.data_idx = 0;

  return res;
}

sys_error_code_t DataAggregationProcessTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *p_task_code,
                                                             CHAR **p_name,
                                                             VOID **p_stack_start, ULONG *p_stack_size,
                                                             UINT *p_priority, UINT *p_preempt_threshold,
                                                             ULONG *p_timeSlice, ULONG *p_auto_start,
                                                             ULONG *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)_this;

  uint16_t item_size = DAP_TASK_CFG_IN_QUEUE_ITEM_SIZE;
  VOID *p_queue_items_buff = SysAlloc(DAP_TASK_CFG_IN_QUEUE_LENGTH * item_size);
  if (p_queue_items_buff == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  if (TX_SUCCESS != tx_queue_create(&p_obj->in_queue, "DataAggregationProcess_Q", TX_1_ULONG, p_queue_items_buff,
                                    DAP_TASK_CFG_IN_QUEUE_LENGTH * item_size))
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  IEventListenerSetOwner((IEventListener *) &p_obj->sensorDataSourceListener, (void *) p_obj);
  IEventListenerSetOwner((IEventListener *) &p_obj->actuatorDataSourceListener, (void *) p_obj);

  /* Data Event Source Allocation and Initialization */
  p_obj->p_event_src = DataEventSrcAlloc();
  if (p_obj->p_event_src == NULL)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
  }

  IEventSrcInit(p_obj->p_event_src);

  /* Set DataAggregated ID */
  p_obj->data_aggregated_id = AGGREGATED_TELEMETRY_EMDATA_TAG;

  p_obj->DPUprocessOnDoing = false;

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "DATAAGGREGATIONPROCESSTASK";
  *p_stack_start = NULL; /* allocate the task stack in the system memory pool.*/
  *p_stack_size = DATAAGGREGATIONPROCESSTASK_CFG_STACK_DEPTH;
  *p_params = (ULONG)_this;
  *p_priority = DATAAGGREGATIONPROCESSTASK_CFG_PRIORITY;
  *p_preempt_threshold = DATAAGGREGATIONPROCESSTASK_CFG_PRIORITY;
  *p_timeSlice = TX_NO_TIME_SLICE;
  *p_auto_start = TX_AUTO_START;

  return res;
}

sys_error_code_t DataAggregationProcessTask_vtblDoEnterPowerMode(AManagedTask *_this,
                                                                 const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t*)_this;*/

  return res;
}

sys_error_code_t DataAggregationProcessTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t*)_this;*/

  return res;
}

sys_error_code_t DataAggregationProcessTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)_this;

  if (NULL != p_obj->p_AImotorCtrlDPU)
  {
    /* optional: set an application specific tag for the DPU.*/
    (void) ADPU2_SetTag((ADPU2_t *) p_obj->p_AImotorCtrlDPU, AI_DPU_EMDATA_TAG);

    /* Register DPU DATA READY notification callback */
    IDPU2_RegisterNotifyCallback((IDPU2_t *) p_obj->p_AImotorCtrlDPU, DataAggregationProcessTask_AITaskDpuCallback, p_obj);

  }

  return res;
}


/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t DataAggregationProcessTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)_this;

  if ((active_power_mode == E_POWER_MODE_STATE1) || (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if (AMTExIsTaskInactive(_this))
    {
      res = DataAggregationProcessTask_PostReportToFront(p_obj);
    }
    else
    {
      ;
    }
  }
  else
  {
    UINT state;
    if (TX_SUCCESS == tx_thread_info_get(&_this->m_xTaskHandle, TX_NULL, &state, TX_NULL, TX_NULL, TX_NULL, TX_NULL,
                                         TX_NULL,
                                         TX_NULL))
    {
      if (state == TX_SUSPENDED)
      {
        tx_thread_resume(&_this->m_xTaskHandle);
      }
    }
  }

  return res;
}

sys_error_code_t DataAggregationProcessTask_vtblOnEnterPowerMode(AManagedTaskEx *_this,
                                                                 const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

/*Private function definition*/
/*****************************/

static sys_error_code_t DataAggregationProcessTask_ExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *) _this;

  AMTExSetInactiveState((AManagedTaskEx *) _this, TRUE);
  if (TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &p_obj->message, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx *) _this, FALSE);

    if (p_obj->message == DAT_FORCE_STEP)
    {
      __NOP();
    }
  }

  return res;

}

static sys_error_code_t DataAggregationProcessTask_ExecuteStepDataLog(AManagedTask *_this)
{
  assert_param(_this);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *) _this;

  AMTExSetInactiveState((AManagedTaskEx *) _this, TRUE);
  if (TX_SUCCESS == tx_queue_receive(&p_obj->in_queue, &p_obj->message, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx *) _this, FALSE);

    switch (p_obj->message)
    {
      case DATA_AGGREGATION:
      {
        /* Aggregate incoming data */
        DataAggregationProcessTask_PerformDataAggregation(p_obj);

        /* Notify observer */
        DataEvent_t evt;
        EMD_1dInit(&p_obj->aggregated_EMData, (uint8_t *)p_obj->aggregated_dataSource.p_data, E_EM_FLOAT,
                   AGGREGATED_EMDATA_DIM);
        DataEventInit((IEvent *) &evt, p_obj->p_event_src, &p_obj->aggregated_EMData, 7, AGGREGATED_TELEMETRY_EMDATA_TAG);

        /* Send EMData to the linked observer */
        IEventSrcSendEvent(p_obj->p_event_src, (IEvent *) &evt, NULL);

      }
      break;

      case DPU_READY_TO_PROCESS:
      {
        (void)ADPU2_ProcessAndDispatch((ADPU2_t *) p_obj->p_AImotorCtrlDPU);
        p_obj->DPUprocessOnDoing = false;
      }
      break;

      case DAT_FORCE_STEP:
      {
        __NOP();
      }
      break;

      default:
        break;
    }
  }
  return res;
}

static inline sys_error_code_t DataAggregationProcessTask_PostReportToFront(DataAggregationProcessTask_t *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  ULONG message = DAT_FORCE_STEP;

  if (SYS_IS_CALLED_FROM_ISR())
  {
    if (TX_SUCCESS != tx_queue_front_send(&_this->in_queue, &message, TX_NO_WAIT))
    {
      res = SYS_NO_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }
  else
  {
    if (TX_SUCCESS != tx_queue_front_send(&_this->in_queue, &message, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_NO_ERROR_CODE;
      // this function is private and the caller will ignore this return code.
    }
  }

  return res;
}

/* Sensor IIListener virtual functions */
sys_error_code_t DataAggregationProcessTask_Sensor_OnStatusChange_vtbl(IListener *_this)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

void *DataAggregationProcessTask_Sensor_GetOwner_vtbl(IEventListener *_this)
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, sensorDataSourceListener));

  return p_obj->owner;
}

void DataAggregationProcessTask_Sensor_SetOwner_vtbl(IEventListener *_this, void *p_owner)
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, sensorDataSourceListener));

  p_obj->owner = p_owner;
}


sys_error_code_t DataAggregationProcessTask_Sensor_OnNewDataReady_vtbl(IEventListener *_this, const DataEvent_t *p_evt)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *) IEventListenerGetOwner(_this);

  uint8_t nPMState = (uint8_t) AMTGetTaskPowerMode((AManagedTask *) p_obj);
  int16_t *data_buf_u16 = (int16_t *)EMD_Data(p_evt->p_data);
  uint32_t data_buf_dim = EMD_GetElementsCount(p_evt->p_data);
  uint32_t decimated_data_byte_dim = (data_buf_dim * sizeof(float_t)) / DECIMATION;

  if (nPMState == E_POWER_MODE_SENSORS_ACTIVE)
  {
    /* Get destination buffer byte address */
    uint8_t *p_destBuff = (uint8_t *)p_obj->sensor_dataSource.p_data;

    /* Get destination buffer byte index */
    uint32_t destBuffIdx = p_obj->sensor_dataSource.data_idx;

    /* Perform data decimation and copy the result into destination buffer */
    DataAggregationProcessTask_ArrayDecimation(data_buf_u16, (float_t *)&p_destBuff[destBuffIdx], data_buf_dim, DECIMATION,
                                               3);

    /* Update destination buffer index */
    p_obj->sensor_dataSource.data_idx += decimated_data_byte_dim;

    if (p_obj->sensor_dataSource.data_idx >= p_obj->sensor_dataSource.len)
    {
      p_obj->sensor_dataSource.data_idx = 0;
    }
  }
  return res;
}

/* Actuator IIListener virtual functions */
sys_error_code_t DataAggregationProcessTask_Actuator_OnStatusChange_vtbl(IListener *_this)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  return res;
}

void *DataAggregationProcessTask_Actuator_GetOwner_vtbl(IEventListener *_this)
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, actuatorDataSourceListener));

  return p_obj->owner;
}

void DataAggregationProcessTask_Actuator_SetOwner_vtbl(IEventListener *_this, void *p_owner)
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, actuatorDataSourceListener));

  p_obj->owner = p_owner;
}


sys_error_code_t DataAggregationProcessTask_Actuator_OnNewDataReady_vtbl(IEventListener *_this,
                                                                         const DataEvent_t *p_evt)
{
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *) IEventListenerGetOwner(_this);

  uint8_t nPMState = (uint8_t) AMTGetTaskPowerMode((AManagedTask *) p_obj);
  int16_t *data_buf_16 = (int16_t *)EMD_Data(p_evt->p_data);
  uint32_t byteToStore = EMD_GetPayloadSize(p_evt->p_data);
  uint32_t data_buf_dim = EMD_GetElementsCount(p_evt->p_data);
  uint32_t decimated_buf_byte_dim = ACTUATOR_DATASOURCE_DIM;


  if (nPMState == E_POWER_MODE_SENSORS_ACTIVE)
  {
    memcpy((void *)actuator_buffer_copy, (void *)data_buf_16, byteToStore);

    uint8_t padding = ACTUATOR_DATASOURCE_DIM - data_buf_dim;
    DataAggregationProcessTask_PerformPadding(actuator_buffer_copy, data_buf_dim, padding);

    /* Get destination buffer byte address */
    uint8_t *p_destBuff = (uint8_t *)p_obj->actuator_dataSource.p_data;

    /* Get destination buffer byte index */
    uint32_t destBuffIdx = p_obj->actuator_dataSource.data_idx;

    /* Perform data decimation and copy the result into destination buffer */
    DataAggregationProcessTask_ArrayDecimation(actuator_buffer_copy, (float_t *)&p_destBuff[destBuffIdx],
                                               ACTUATOR_BUFFER_COPY_DIM, DECIMATION, 1);

    /* Update destination buffer index */
    p_obj->actuator_dataSource.data_idx += decimated_buf_byte_dim;

    if (p_obj->actuator_dataSource.data_idx >= p_obj->actuator_dataSource.len)
    {
      /*Reset data index*/
      p_obj->actuator_dataSource.data_idx = 0;

      /* Trigger data aggregation procedure */
      DataAggregationProcessTask_SendDataAggregationMsg(p_obj);
    }
  }
  return res;
}


uint8_t  DataAggregationProcessTask_GetId_vtbl(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  DataAggregationProcessTask_t *p_if_owner = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, ai_dpu_input_if));
  uint8_t res = p_if_owner->data_aggregated_id;

  return res;
}

IEventSrc *DataAggregationProcessTask_GetEventSourceIF_vtbl(ISourceObservable *_this)
{
  assert_param(_this != NULL);
  DataAggregationProcessTask_t *p_if_owner = (DataAggregationProcessTask_t *)((uint32_t) _this - offsetof(DataAggregationProcessTask_t, ai_dpu_input_if));

  return p_if_owner->p_event_src;
}

static void DataAggregationProcessTask_SendDataAggregationMsg(DataAggregationProcessTask_t *p_obj)
{
  if (false == p_obj->DPUprocessOnDoing)
  {
    p_obj->DPUprocessOnDoing = true;
    ULONG message = DATA_AGGREGATION;
    if (TX_SUCCESS != tx_queue_send(&p_obj->in_queue, &message, TX_NO_WAIT))
    {
      // unable to send the message. Signal the error
      sys_error_handler();
    }
  }
}

inline static void DataAggregationProcessTask_PerformDataAggregation(DataAggregationProcessTask_t *p_obj)
{
  /* Get pointer to buffer data*/
  float_t *p_actuator_data_float = (float_t *) p_obj->actuator_dataSource.p_data;
  float_t *p_sensor_data_float = (float_t *) p_obj->sensor_dataSource.p_data;
  float_t *p_aggregated_float = (float_t *) p_obj->aggregated_dataSource.p_data;
  uint32_t sensor_data_byte_idx = p_obj->sensor_dataSource.data_idx;
  uint8_t sensor_data_byte_inc = sizeof(float_t) * SENSOR_DATASOURCE_NUM_OF_AXIS;

  /* Array byte index*/
  uint32_t byte_idx;

  if (sensor_data_byte_idx > 0)
  {
    /* Get number of byte to be copied from sensor data idx to the end of the buffer*/
    uint32_t sensor_byte_to_copied = p_obj->sensor_dataSource.len - sensor_data_byte_idx;

    /* get sensor data buffer pointer*/
    p_sensor_data_float = &p_sensor_data_float[sensor_data_byte_idx / sizeof(float_t)];


    /* Start to copy data from sensor buffer pointer to the end*/
    for (byte_idx = 0u; byte_idx < sensor_byte_to_copied; byte_idx = byte_idx + sensor_data_byte_inc)
    {
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_actuator_data_float++;
    }

    p_sensor_data_float = (float_t *)p_obj->sensor_dataSource.p_data;
    for (byte_idx = 0u; byte_idx < sensor_data_byte_idx; byte_idx = byte_idx + sensor_data_byte_inc)
    {
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_actuator_data_float++;
    }
  }
  else
  {
    p_sensor_data_float = (float_t *)p_obj->sensor_dataSource.p_data;
    for (byte_idx = 0u; byte_idx < p_obj->sensor_dataSource.len; byte_idx = byte_idx + sensor_data_byte_inc)
    {
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_sensor_data_float++;
      *p_aggregated_float++ = *p_actuator_data_float++;
    }
  }
}

inline static void DataAggregationProcessTask_PerformPadding(int16_t *p_buffer, uint32_t starting_ele, uint32_t num_ele)
{
  uint32_t padding_end = starting_ele + num_ele;
  for (uint32_t array_idx = starting_ele; array_idx < padding_end; array_idx++)
  {
    p_buffer[array_idx] = p_buffer[array_idx - 1];
  }
}

static void DataAggregationProcessTask_ArrayDecimation(int16_t *p_array, float_t *p_output, uint32_t len,
                                                       uint8_t decimation, uint8_t axis)
{
  uint32_t decimated_array_len;
  uint32_t decimation_cnt = 0;
  uint32_t next_position = 0;
  uint32_t axis_increment = 0;
  uint8_t next_axis = 0;
  float_t sum = 0;

  decimated_array_len = len / decimation;

  for (uint32_t array_idx = 0; array_idx < decimated_array_len; array_idx += axis)
  {
    next_position = decimation_cnt * decimation * axis;
    next_axis = 0;

    for (uint8_t axis_idx = 0; axis_idx < axis; axis_idx++)
    {
      sum = 0;
      for (uint8_t decimation_idx = 0; decimation_idx < decimation; decimation_idx++)
      {
        sum += p_array[next_position + axis_increment + axis_idx];
        axis_increment += axis;
      }
      p_output[array_idx + next_axis] = sum / decimation;
      next_axis++;
      axis_increment = 0;
    }

    decimation_cnt++;
  }
}

static void DataAggregationProcessTask_AITaskDpuCallback(IDPU2_t *_this, void *p_param)
{
  DataAggregationProcessTask_t *p_obj = (DataAggregationProcessTask_t *)p_param;
  ULONG message = DPU_READY_TO_PROCESS;

  if (TX_SUCCESS != tx_queue_send(&p_obj->in_queue, &message, TX_NO_WAIT))
  {
    // unable to send the message. Signal the error
    sys_error_handler();
  }
}

