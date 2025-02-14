/**
  ******************************************************************************
  * @file    VL53L8CXTask.h
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
#ifndef VL53L8CXTASK_H_
#define VL53L8CXTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "ABusIF.h"
#include "events/DataEventSrc.h"
#include "events/DataEventSrc_vtbl.h"
#include "ISensorRanging.h"
#include "ISensorRanging_vtbl.h"
#include "mx.h"
#include "vl53l8cx.h"


/**
  * Create  type name for _VL53L8CXTask.
  */
typedef struct _VL53L8CXTask VL53L8CXTask;

/**
  *  VL53L8CXTask internal structure.
  */
struct _VL53L8CXTask
{
  /**
    * Base class object.
    */
  AManagedTaskEx super;

  // Task variables should be added here.

  /**
    * IRQ GPIO configuration parameters.
    */
  const MX_GPIOParams_t *pIRQConfig;

  /**
    * SPI CS GPIO configuration parameters.
    */
  const MX_GPIOParams_t *pCSConfig;

  /**
    * I2C BS GPIO configuration parameters.
    */
  const MX_GPIOParams_t *pBSConfig;

  /**
    * Bus IF object used to connect the sensor task to the specific bus.
    */
  ABusIF *p_sensor_bus_if;

  VL53L8CX_Configuration tof_driver_if;

  /**
    * Implements the  ISensor interface.
    */
  ISensorRanging_t sensor_if;

  /**
    * Specifies sensor capabilities.
    */
  const SensorDescriptor_t *sensor_descriptor;

  /**
    * Specifies sensor configuration.
    */
  SensorStatus_t sensor_status;

  EMData_t data;
  /**
    * Specifies the sensor ID for the time-of-flight subsensor.
    */
  uint8_t id;

  /**
    * Synchronization object used to send command to the task.
    */
  TX_QUEUE in_queue;

  /**
    * Buffer to store the data read from the sensor
    */
#ifdef TOF_EXTENDED
  uint32_t p_sensor_data_buff[VL53L8CX_MAX_NB_ZONES][8];
#else
  uint32_t p_sensor_data_buff[VL53L8CX_MAX_NB_ZONES][2];
#endif

  /**
    * ::IEventSrc interface implementation for this class.
    */
  IEventSrc *p_event_src;

  /**
    * Software timer used to generate the read command
    */
  TX_TIMER read_timer;

  /**
    * Timer period used to schedule the read command
    */
  ULONG vl53l8cx_task_cfg_timer_period_ms;

  /**
    * Used to update the instantaneous ODR.
    */
  double prev_timestamp;
};

// Public API declaration
//***********************

/**
  * Get the ISourceObservable interface for the accelerometer.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the generic object ::ISourceObservable if success,
  * or NULL if out of memory error occurs.
  */
ISourceObservable *VL53L8CXTaskGetTofSensorIF(VL53L8CXTask *_this);

/**
  * Allocate an instance.
  *
  * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO connected to the VL53L8CX sensor and configured in EXTI mode.
  *        If it is NULL then the sensor is configured in polling mode.
  * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the SPI CS Pin.
  * @param pBSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the I2C BS Pin.
  * @return a pointer to the generic object ::AManagedTaskEx if success,
  * or NULL if out of memory error occurs.
  */
AManagedTaskEx *VL53L8CXTaskAlloc(const void *pIRQConfig, const void *pCSConfig, const void *pBSConfig);

/**
  * Call the default ::VL53L8CXTaskAlloc and then it overwrite sensor name
  *
  * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO connected to the VL53L8CX sensor and configured in EXTI mode.
  *        If it is NULL then the sensor is configured in polling mode.
  * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the SPI CS Pin.
  * @param pBSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the I2C BS Pin.
  * @return a pointer to the generic object ::AManagedTaskEx if success,
  * or NULL if out of memory error occurs.
  */
AManagedTaskEx *VL53L8CXTaskAllocSetName(const void *pIRQConfig, const void *pCSConfig, const void *pBSConfig,
                                         const char *p_name);

/**
  * Allocate an instance of ::VL53L8CXTask in a memory block specified by the application.
  * The size of the memory block must be greater or equal to `sizeof(VL53L8CXTask)`.
  * This allocator allows the application to avoid the dynamic allocation.
  *
  * \code
  * VL53L8CXTask sensor_task;
  * VL53L8CXTaskStaticAlloc(&sensor_task);
  * \endcode
  *
  * @param p_mem_block [IN] specify a memory block allocated by the application.
  *        The size of the memory block must be greater or equal to `sizeof(VL53L8CXTask)`.
  * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO connected to the VL53L8CX sensor and configured in EXTI mode.
  *        If it is NULL then the sensor is configured in polling mode.
  * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the SPI CS Pin.
  * @param pBSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the I2C BS Pin.
  * @return a pointer to the generic object ::AManagedTaskEx_t if success,
  * or NULL if out of memory error occurs.
  */
AManagedTaskEx *VL53L8CXTaskStaticAlloc(void *p_mem_block, const void *pIRQConfig, const void *pCSConfig,
                                        const void *pBSConfig);

/**
  * Call the default ::VL53L8CXTaskAlloc and then it overwrite sensor name
  *
  * \code
  * VL53L8CXTask sensor_task;
  * VL53L8CXTaskStaticAlloc(&sensor_task);
  * \endcode
  *
  * @param p_mem_block [IN] specify a memory block allocated by the application.
  *        The size of the memory block must be greater or equal to `sizeof(VL53L8CXTask)`.
  * @param pIRQConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO connected to the VL53L8CX sensor and configured in EXTI mode.
  *        If it is NULL then the sensor is configured in polling mode.
  * @param pCSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the SPI CS Pin.
  * @param pBSConfig [IN] specifies a ::MX_GPIOParams_t instance declared in the mx.h file.
  *        It must be a GPIO identifying the I2C BS Pin.
  * @return a pointer to the generic object ::AManagedTaskEx_t if success,
  * or NULL if out of memory error occurs.
  */
AManagedTaskEx *VL53L8CXTaskStaticAllocSetName(void *p_mem_block, const void *pIRQConfig, const void *pCSConfig,
                                               const void *pBSConfig,
                                               const char *p_name);

/**
  * Get the Bus interface for the sensor task.
  *
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the Bus interface of the sensor.
  */
ABusIF *VL53L8CXTaskGetSensorIF(VL53L8CXTask *_this);

/**
  * Get the ::IEventSrc interface for the sensor task.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface of the sensor.
  */
IEventSrc *VL53L8CXTaskGetEventSrcIF(VL53L8CXTask *_this);

/**
  * IRQ callback
  */
void VL53L8CXTask_EXTI_Callback(uint16_t nPin);

// Inline functions definition
// ***************************

#ifdef __cplusplus
}
#endif

#endif /* VL53L8CXTASK_H_ */
