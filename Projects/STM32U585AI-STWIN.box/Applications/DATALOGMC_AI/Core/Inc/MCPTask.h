/**
 ********************************************************************************
 * @file    MCPTask.h
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

#ifndef ELOOM_AMANAGED_TASK_EX_MCPTASK_H_
#define ELOOM_AMANAGED_TASK_EX_MCPTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "services/AManagedTaskEx.h"
#include "services/AManagedTaskEx_vtbl.h"
#include "events/DataEventSrc.h"
#include "events/DataEventSrc_vtbl.h"
#include "events/IDataEventListener.h"
#include "events/IDataEventListener_vtbl.h"
#include "tx_api.h"

#include "aspep.h"
#include "mcp.h"


/* Task error codes */
/********************/
#ifndef SYS_NO_ERROR_CODE
#define SYS_NO_ERROR_CODE                             0
#endif
#ifndef SYS_MCPTASK_BASE_ERROR_CODE
#define SYS_MCPTASK_CODE                  1
#endif

#define SLOW_TELEMETRY_BUFF_DIM 6u
/*#define SYS_MCPTASK_UNKNOWN_ERROR_CODE  SYS_MCPTASK_BASE_ERROR_CODEE + 1*/

typedef enum
{
  MC_REG_HEATS_TEMP_IDX = 0,
  MC_REG_SPEED_REF_IDX,
  MC_REG_SPEED_MEAS_IDX,
  MC_REG_BUS_VOLTAGE_IDX,
  MC_REG_FAULTS_FLAGS_IDX,
  /* Add other indices as necessary */
  MC_REG_IDX_COUNT /* This should always be the last element */
}MCPTaskSlowTelemetry_idx_t;

/**
 * @Brief: Fast telemetry index structure
 */
typedef enum
{
  MC_REG_I_Q_MEAS_IDX = 0,
  MC_REG_I_D_MEAS_IDX,
  MC_REG_I_Q_REF_IDX,
  MC_REG_I_D_REF_IDX,
  MC_REG_V_Q_IDX,
  MC_REG_V_D_IDX,
  MC_REG_I_A_IDX,
  MC_REG_I_B_IDX,
  MC_REG_V_A_IDX,
  MC_REG_V_B_IDX
}MCPTaskFastTelemetry_idx_t;

/**
 * Create  type name for _MCPTask.
 */
typedef struct _MCPTask MCPTask_t;

/**
 *  MCPTask_t internal structure.
 */
struct _MCPTask
{
  /**
   * Base class object.
   */
  AManagedTaskEx super;

  MCP_Handle_t *MCP_handle;

  bool motor_started;

  bool log_started;

  bool slow_telemetries_buffer_allocated;

  bool enabled_slow_telemetries;

  bool enabled_fast_telemetries;

  uint16_t enabled_slow_telemetries_cnt;

  int32_t motor_speed;

  /**
   * EMData slow telemetry data buffer
   */
  void *p_slow_telemetry_buffer;

  /**
   * EMData slow telemetry
   */
  EMData_t slow_telemetry_data;

  /**
   * EMData slow telemetry
   */
  EMData_t async_telemetry_data;

  /**
   * Slow telemetry buffer
   */
  int32_t p_slow_telemetry_buff[SLOW_TELEMETRY_BUFF_DIM];

  /* Fast telemetry register ID buffer */

  uint16_t *p_asyncRegIDBuff;

  /* Fast telemetry register value buffer */
  uint8_t *p_asyncRegValBuff;

  /**
   * ::IEventSrc interface implementation for this class.
   */
  IEventSrc *p_event_src;

  IEventSrc *p_sync_evt_src;

  uint16_t *p_tx_regID_buff;

  uint8_t *p_tx_regValue_buff;

  TX_TIMER mcp_telemetry_request_timer;

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
  TX_TIMER mcp_fake_fastlemetry_timer;
#endif

  /* MCP Send Queue memory ptr */
  UCHAR *p_mcp_receive_queue_memory;

  /* MCP Send Queue */
  TX_QUEUE mcp_receive_queue;

  /* MCP Send semaphore */
  TX_SEMAPHORE mcp_send_semaphore;

  UCHAR *mcp_send_thread_memory_pointer;

  TX_THREAD mcp_send_thread;

  /* MCP Send commands queue */
  UCHAR *p_mcp_send_queue_memory;

  /* MCP Send Queue */
  TX_QUEUE mcp_send_queue;
};

/* Public API declaration */
/**************************/

/**
 * Allocate an instance of MCPTask_t in the framework heap.
 *
 * @return a pointer to the generic object ::AManagedTaskEx if success,
 *         or NULL if out of memory error occurs.
 */
AManagedTaskEx *MCPTask_Alloc(void);

/**
  * Get the ::IEventSrc interface for the sensor task.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface of the sensor.
  */
IEventSrc *MCPTask_GetEventSrcIF(MCPTask_t *_this);

/**
  * Get the ::IEventSrc interface for async telemetry task.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface to observer/observable async telemetry interface.
  */
IEventSrc *MCPTask_GetAsyncTelemetryEventSrcIF(MCPTask_t *_this);


void MCPTask_SetSlowTelemetryState(MCPTaskSlowTelemetry_idx_t slowTelemetryIdx, bool state);
bool MCPTask_GetSlowTelemetryState(MCPTaskSlowTelemetry_idx_t slowTelemetryIdx);
void MCPTask_SetFastTelemetryState(MCPTaskFastTelemetry_idx_t fastTelemetryIdx, bool state);
bool MCPTask_GetFastTelemetryState(MCPTaskFastTelemetry_idx_t fastTelemetryIdx);
uint32_t MCPTask_GetNumOfEnabledFastTelemetry(void);
uint32_t MCPTask_GetNumOfEnabledSlowTelemetry(void);
bool MCPTask_GetMotorStarted(void);
int32_t MCPTask_GetMotorSpeed(void);
uint8_t MCPTask_SetMotorSpeed(int32_t speed);
uint8_t MCPTask_start_motor(void);
uint8_t MCPTask_stop_motor(void);
uint8_t MCPTask_motor_reset(void);
uint8_t MCPTask_ack_fault(void);

/* Inline functions definition */
/*******************************/


#ifdef __cplusplus
}
#endif

#endif /* ELOOM_AMANAGED_TASK_EX_MCPTASK_H_ */
