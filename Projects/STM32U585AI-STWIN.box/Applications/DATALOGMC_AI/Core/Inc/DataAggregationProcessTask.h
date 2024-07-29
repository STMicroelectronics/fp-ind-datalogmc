/**
 ********************************************************************************
 * @file    DataAggregationProcessTask.h
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

#ifndef ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_H_
#define ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "events/DataEventSrc.h"
#include "events/DataEventSrc_vtbl.h"
#include "events/IDataEventListener.h"
#include "events/IDataEventListener_vtbl.h"

#include "NeaiNccDPU.h"
#include "DefDataBuilder.h"
#include "DefDataBuilder_vtbl.h"

#include "tx_api.h"


/* Task error codes */
/********************/
#ifndef SYS_NO_ERROR_CODE
#define SYS_NO_ERROR_CODE                             0
#endif
#ifndef SYS_DATAAGGREGATIONPROCESSTASK_BASE_ERROR_CODE
#define SYS_DATAAGGREGATIONPROCESSTASK_CODE                  1
#endif
/*#define SYS_DATAAGGREGATIONPROCESSTASK_UNKNOWN_ERROR_CODE  SYS_DATAAGGREGATIONPROCESSTASK_BASE_ERROR_CODEE + 1*/

/**
 *  DataSource_t internal structure.
 */
typedef struct _DataSource
{
  /* pointer to data buffer */
  void *p_data;

  /* data buffer length in byte*/
  uint32_t len;

  /* Data buffer byte index.
   * Used as position during data copy 
   */
  uint32_t data_idx;
}DataSource_t;


/**
 * Create  type name for _DataAggregationProcessTask.
 */
typedef struct _DataAggregationProcessTask DataAggregationProcessTask_t;

/**
 *  DataAggregationProcessTask_t internal structure.
 */
struct _DataAggregationProcessTask {
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  /* Incoming message queue */
  TX_QUEUE in_queue;

  /* Incoming task message */
  ULONG message;

  void *owner;

  /* Flag used to synchronize aggregation and process */
  bool DPUprocessOnDoing;

  /**
   * Digital processing Unit specialized for Neai nCC classification algorithm.
   */
  IDPU2_t *p_AImotorCtrlDPU;

  /**
   * pointer to Neai DPU input buffer
   */
  void *p_neai_dpu_in_buff;

  /**
   * pointer to Neai DPU output buffer
   */
  void *p_neai_dpu_out_buff;

  /**
   * Data builder object to connect the DPU to teh sensor.
   */
  DefDataBuilder_t neai_data_builder;

  /**
   * EMData aggregated data
   */
  EMData_t aggregated_EMData;


  /* IEventSrc interface implementation for this class. */
  IEventSrc *p_event_src;

  /* Specifies the data aggregation ID for the ISourceObservable. */
  uint8_t data_aggregated_id;

  /* Implements the accelerometer ISensor interface.*/
  ISourceObservable neai_dpu_input_if;

  /* Task variables should be added here. */
  DataSource_t sensor_dataSource;

  DataSource_t actuator_dataSource;

  DataSource_t aggregated_dataSource;

  DataSource_t sensor_dataSource_dump;

  DataSource_t actuator_dataSource_dump;

  /* Sensor Data Source Event Listener */
  IDataEventListener_t sensorDataSourceListener;

  /* Actuator Data Source Event Listener */
  IDataEventListener_t actuatorDataSourceListener;
};


/* Public API declaration */
/**************************/

/**
 * Allocate an instance of DataAggregationProcessTask_t in the framework heap.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 *         or NULL if out of memory error occurs.
 */
AManagedTaskEx *DataAggregationProcessTask_Alloc();

IEventListener *DataAggregationProcessTask_GetSensorEventListenerIF(DataAggregationProcessTask_t *_this);
IEventListener *DataAggregationProcessTask_GetActuatorEventListenerIF(DataAggregationProcessTask_t *_this);
IEventSrc *DataAggregationProcessTask_GetEventSrcIF(DataAggregationProcessTask_t *_this);
IDPU2_t *DataAggregationProcessTask_GetClassifierDPU(DataAggregationProcessTask_t *_this);
/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* ELOOM_AMANAGED_TASK_EX_DATAAGGREGATIONPROCESSTASK_H_ */
