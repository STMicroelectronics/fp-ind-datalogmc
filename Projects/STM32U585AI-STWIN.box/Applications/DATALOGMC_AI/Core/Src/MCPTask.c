/**
 ********************************************************************************
 * @file    MCPTask.c
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

/* Includes ------------------------------------------------------------------*/
#include "MCPTask.h"
#include "MCPTask_vtbl.h"
#include "services/sysmem.h"
#include "services/sysdebug.h"
#include "services/SysTimestamp.h"
#include "register_interface.h"


/* Define ---------------------------------------------------------------------*/
#ifndef MCPTASK_CFG_STACK_DEPTH
#define MCPTASK_CFG_STACK_DEPTH        (TX_MINIMUM_STACK*20)
#endif

#ifndef MCPTASK_CFG_PRIORITY
#define MCPTASK_CFG_PRIORITY           (6)
#endif

#define DEFAULT_MCP_SEND_QUEUE_SIZE                20

#define SYS_DEBUGF(level, message)                 SYS_DEBUGF3(SYS_DBG_MCPTASK, level, message)

/* MCP telemetry timer request time in ms */
#define MCP_TELEMETRY_REQUEST_TIMER_PERIOD_MS      300U

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
#define MCP_FAKE_FAST_TELEMETRY 51U
#endif

#define MCP_SEND_CFG_STACK_DEPTH                  (TX_MINIMUM_STACK*15)
#define MCP_FAST_TELEMETRY_EMDATA_TAG 1u

/* Slow telemetry array dimension */
#define SLOW_TELEMETRY_ARRAY_DIM  5u

/*Fast telemetry buffer dimension */
#define ONE_FAST_TELEMTRY_BUFF_DIM    2046u
#define TWO_FAST_TELEMTRY_BUFF_DIM    2042u
#define THREE_FAST_TELEMTRY_BUFF_DIM  2040u
#define FOUR_FAST_TELEMTRY_BUFF_DIM   2038u


#define MCP_TX_REG_ID_BUFF_DIM  512
#define MCP_TX_REG_VALUE_BUFF_DIM 512

#define MCPTASK_ASYNC_REGID_BUFF_SIZE 64u
#define MCPTASK_ASYNC_REGVAL_BUFF_SIZE 64u



/**
 * @anchor fault_codes
 * @name Fault codes
 * The symbols below define the codes associated to the faults that the
 * Motor Control subsystem can raise.
 */
#define  MCP_NO_ERROR     ((uint16_t)0x0000) /**< @brief No error.*/
#define  MCP_NO_FAULTS    ((uint16_t)0x0000) /**< @brief No error.*/
#define  MCP_DURATION     ((uint16_t)0x0001) /**< @brief Error: FOC rate to high.*/
#define  MCP_OVER_VOLT    ((uint16_t)0x0002) /**< @brief Error: Software over voltage.*/
#define  MCP_UNDER_VOLT   ((uint16_t)0x0004) /**< @brief Error: Software under voltage.*/
#define  MCP_OVER_TEMP    ((uint16_t)0x0008) /**< @brief Error: Software over temperature.*/
#define  MCP_START_UP     ((uint16_t)0x0010) /**< @brief Error: Startup failed.*/
#define  MCP_SPEED_FDBK   ((uint16_t)0x0020) /**< @brief Error: Speed feedback.*/
#define  MCP_BREAK_IN     ((uint16_t)0x0040) /**< @brief Error: Emergency input (Over current).*/
#define  MCP_SW_ERROR     ((uint16_t)0x0080) /**< @brief Software Error.*/

#define  MCP_NO_ERROR_VAL     (0u) /**< @brief No error.*/
#define  MCP_NO_FAULTS_VAL    (0u) /**< @brief No error.*/
#define  MCP_DURATION_VAL     (1u) /**< @brief Error: FOC rate to high.*/
#define  MCP_OVER_VOLT_VAL    (2u) /**< @brief Error: Software over voltage.*/
#define  MCP_UNDER_VOLT_VAL   (3u) /**< @brief Error: Software under voltage.*/
#define  MCP_OVER_TEMP_VAL    (4u) /**< @brief Error: Software over temperature.*/
#define  MCP_START_UP_VAL     (5u) /**< @brief Error: Startup failed.*/
#define  MCP_SPEED_FDBK_VAL   (6u) /**< @brief Error: Speed feedback.*/
#define  MCP_BREAK_IN_VAL     (7u) /**< @brief Error: Emergency input (Over current).*/
#define  MCP_SW_ERROR_VAL     (8u) /**< @brief Software Error.*/

#ifdef SYS_ENABLE_BLE_FAKE_SLOW_TELE
#define MCPTASK_MAX_FAKE_SLOW_TELEMETRY_VAL 9u
#define MCPTASK_REG_HEATS_TEMP_INIT_VAL   1u
#define MCPTASK_REG_SPEED_REF_INIT_VAL    2u
#define MCPTASK_REG_SPEED_MEAS_INIT_VAL   3u
#define MCPTASK_REG_BUS_VOLTAGE_INIT_VAL  4u
#define MCPTASK_REG_FAULTS_FLAGS_INIT_VAL 0u
#endif

/* Private member function declaration ---------------------------------------------------------------------*/

/**
 * Class object declaration.
 */
typedef struct _MCPTaskClass
{
  /**
   * MCPTask class virtual table.
   */
  AManagedTaskEx_vtbl vtbl;

  /**
   * UtilTask class (PM_STATE, ExecuteStepFunc) map. The map is implemented with an array and
   * the key is the index. Number of items of this array must be equal to the number of PM state
   * of the application. If the managed task does nothing in a PM state, then set to NULL the
   * relative entry in the map.
   */
  pExecuteStepFunc_t p_pm_state2func_map[];

} MCPTaskClass_t;


/**
 * @brief Enumeration of the command IDs for the MCP task send function
 */
typedef enum
{
  MCPTASK_CONNECT_CMD = 0,             /**< Connect command */
  MCPTASK_DISCONNECT_CMD,              /**< Disconnect command */
  MCPTASK_START_CMD,                   /**< Start command */
  MCPTASK_STOP_CMD,                    /**< Stop command */
  MCPTASK_ACK_FAULT_CMD,               /**< Acknowledge fault command */
  MCPTASK_SET_SPEED_CMD,               /**< Set speed command */
  MCPTASK_RESET_CMD,                   /**< Reset command */
  MCPTASK_ENABLE_SLOW_TELEMETRY_CMD,   /**< Enable slow telemetry command */
  MCPTASK_DISABLE_SLOW_TELEMETRY_CMD,  /**< Disable slow telemetry command */
  MCPTASK_ENABLE_FAST_TELEMETRY_CMD,   /**< Enable fast telemetry command */
  MCPTASK_DISABLE_FAST_TELEMETRY_CMD,  /**< Disable fast telemetry command */
  MCPTASK_TELEMETRY_REQUEST_CMD,       /**< Telemetry request command */
  MCPTASK_NEW_PACKET_AVAILABLE,        /**< New packet available command */
  MCPTASK_MESSAGE_ID_FORCE_STEP        /**< Message ID force step command */
}MCPTask_send_cmd_id_t;

/**
 * @brief Structure for MCP task commands
 */
typedef struct
{
  uint32_t cmd_id;  /**< Command ID */
  int32_t val;      /**< Value */
}MCPTask_cmd_t;

/**
 * @brief Structure for MCP task slow telemetry
 */
typedef struct
{
  uint16_t reg_id;  /**< Register ID */
  bool enable;      /**< Enable flag */
  int32_t value;    /**< Value */
}MCPTask_Slow_telemetry_t;

/**
 * MCP Fast telemetry configuration structure
 */
typedef struct
{
  uint16_t reg_id;
  bool enable;
}MCPTask_Fast_telemetry_t;

/* Private Function declaration ---------------------------------------------------------------------*/
static sys_error_code_t MCPTask_ExecuteStepState1(AManagedTask *_this);
static void MCPTask_Scheduler(MCP_Handle_t *p_MCP_handle);
static sys_error_code_t MCPTask_SendTelemetryRequest(MCPTask_t* _this);
static void MCPTask_SlowTelemetrySave(uint16_t dataID, uint8_t * size, uint8_t * rxData);
static void MCPTask_SendAsyncsRequest(void);
static void MCPTask_DisableAsyncsRequest(void);
static void MCPTask_EnableFastTelemetry(MCPTask_t *_this);
static void MCPTask_DisableFastTelemetry(MCPTask_t *_this);
static void MCPTask_SendThread(ULONG thread_input);
static void MCPTask_SlowTelemetryUpdateEMDataBuffer(uint32_t *emDataBuffer, MCPTask_Slow_telemetry_t *slowTelemetryArray, uint16_t slowTelemetryArrayLen);
static void MCPTask_SlowTelemetrySendEMData(void);
static void MCPTask_SlowTelemetryGetDataElement(uint8_t *rxData, uint16_t *rxLength);
static MCPTaskSlowTelemetry_idx_t MCPTask_SlowTelemetryConvertTokenToIdx(uint16_t dataID);
static int32_t MCPTask_SlowTelemetryExtractData(uint16_t dataID, uint8_t *size, uint8_t *rxData);
static void MCPTask_SlowTelemetrySaveIntoArray(MCPTask_Slow_telemetry_t *p_SlowTelemetryArray, MCPTaskSlowTelemetry_idx_t idx, int32_t telemetry_data);

/* Callback Function declaration ---------------------------------------------------------------------*/
static void MCPTask_send_complete_cb(void);
static void MCPTask_after_sending_cb(void);
static void MCPTask_receive_new_packet_cb(void);
static void MCPTask_OnDeliveredTelemetryPacketCb (uint8_t CallbackID, uint8_t MCPResponse, uint8_t * rxData, uint16_t * rxLength);
static void MCPTask_saveAsyncTelemetry_cb(uint8_t * rxAsyncData, uint32_t rxLength);
static void MCPTask_connection_completed_cb(bool state);
static void MCPTask_RequestTelemtryTimerCb(ULONG timer);
static void MCPTask_delay_cb(uint8_t delay);
#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
static void MCPTask_FakeFastTelemtryTimerCb(ULONG timer);
#endif

/* Inline Function declaration ---------------------------------------------------------------------*/
static inline sys_error_code_t MCPTask_postReportToFront(MCPTask_t *_this, MCPTask_cmd_t *pReport);
static inline int32_t MCPTask_convertHexFaultToIntIdx(int32_t hex_fault);

/* Module objects instance */
/***************************/

/**
 * The class object.
 */
static const MCPTaskClass_t sTheClass = {
    /* class virtual table */
    {
        MCPTask_vtblHardwareInit,
        MCPTask_vtblOnCreateTask,
        MCPTask_vtblDoEnterPowerMode,
        MCPTask_vtblHandleError,
        MCPTask_vtblOnEnterTaskControlLoop,
        MCPTask_vtblForceExecuteStep,
        MCPTask_vtblOnEnterPowerMode
    },

    /* class (PM_STATE, ExecuteStepFunc) map */
    {
        MCPTask_ExecuteStepState1,
        NULL,
        MCPTask_ExecuteStepState1
    }
};

/**
 * @Brief Slow Telemetry configuration array
 */
#ifdef SYS_ENABLE_BLE_FAKE_SLOW_TELE

static uint32_t MCPTask_Slow_telemetry_Init_val[SLOW_TELEMETRY_ARRAY_DIM] = {
                                                                               MCPTASK_REG_HEATS_TEMP_INIT_VAL,
                                                                               MCPTASK_REG_SPEED_REF_INIT_VAL,
                                                                               MCPTASK_REG_SPEED_MEAS_INIT_VAL,
                                                                               MCPTASK_REG_BUS_VOLTAGE_INIT_VAL,
                                                                               MCPTASK_REG_FAULTS_FLAGS_INIT_VAL
                                                                             };

static MCPTask_Slow_telemetry_t MCPTask_Slow_telemetry[SLOW_TELEMETRY_ARRAY_DIM] = {
                                                                                      {MC_REG_HEATS_TEMP,   false, MCPTASK_REG_HEATS_TEMP_INIT_VAL  },
                                                                                      {MC_REG_SPEED_REF,    false, MCPTASK_REG_SPEED_REF_INIT_VAL   },
                                                                                      {MC_REG_SPEED_MEAS,   false, MCPTASK_REG_SPEED_MEAS_INIT_VAL  },
                                                                                      {MC_REG_BUS_VOLTAGE,  false, MCPTASK_REG_BUS_VOLTAGE_INIT_VAL },
                                                                                      {MC_REG_FAULTS_FLAGS, false, MCPTASK_REG_FAULTS_FLAGS_INIT_VAL}
                                                                                   };
#else
static MCPTask_Slow_telemetry_t MCPTask_Slow_telemetry[SLOW_TELEMETRY_ARRAY_DIM] = {
                                                                                    {MC_REG_HEATS_TEMP,   false, 0u},
                                                                                    {MC_REG_SPEED_REF,    false, 0u},
                                                                                    {MC_REG_SPEED_MEAS,   false, 0u},
                                                                                    {MC_REG_BUS_VOLTAGE,  false, 0u},
                                                                                    {MC_REG_FAULTS_FLAGS, false, 0u}
                                                                                   };
#endif
/**
 * @Brief Fast Telemetry configuration array
 */
static MCPTask_Fast_telemetry_t MCPTask_Fast_telemetry[] = {
                                                            {MC_REG_I_Q_MEAS,  false},
                                                            {MC_REG_I_D_MEAS, false},
                                                            {MC_REG_I_Q_REF, false},
                                                            {MC_REG_I_D_REF,  false},
                                                            {MC_REG_V_Q,      false},
                                                            {MC_REG_V_D,      false},
                                                            {MC_REG_I_A,      false},
                                                            {MC_REG_I_B,      false},
                                                            {MC_REG_V_ALPHA,  false},
                                                            {MC_REG_V_BETA,   false}
                                                           };

/**
 * @Brief Fast Telemetry buffer dimension based on number of enabled fast telemetries
 */
static uint32_t MCPTask_Fast_telemetry_receive_buff_dim[] = { ONE_FAST_TELEMTRY_BUFF_DIM,
                                                              TWO_FAST_TELEMTRY_BUFF_DIM,
                                                              THREE_FAST_TELEMTRY_BUFF_DIM,
                                                              FOUR_FAST_TELEMTRY_BUFF_DIM
                                                            };


#if defined(DEBUG) || defined (SYS_DEBUG)
#define sTaskObj                                  MCP_TaskObj
#endif

/* Private Variable */
/********************/
/**
 * The only instance of the task object.
 */
static MCPTask_t sTaskObj;


/* Public API definition */
/*************************/

AManagedTaskEx *MCPTask_Alloc(void)
{
  /* Initialize the super class */
  AMTInitEx(&sTaskObj.super);

  sTaskObj.super.vptr = &sTheClass.vtbl;

  sTaskObj.motor_started = false;
  sTaskObj.motor_speed = 1440;

  return (AManagedTaskEx*) &sTaskObj;
}

/**
  * Get the ::IEventSrc interface for the sensor task.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface of the sensor.
  */
IEventSrc *MCPTask_GetEventSrcIF(MCPTask_t *_this)
{
  assert_param(_this != NULL);
  return (IEventSrc*) _this->p_event_src;
}

/**
  * Get the ::IEventSrc interface for MCPTask Asynch telemetry.
  * @param _this [IN] specifies a pointer to a task object.
  * @return a pointer to the ::IEventSrc interface of the asynch telemetry.
  */
IEventSrc* MCPTask_GetAsyncTelemetryEventSrcIF(MCPTask_t *_this)
{
  assert_param(_this != NULL);
  return (IEventSrc*) _this->p_sync_evt_src;
}

/**
  * Get the motor started info .
  * @return True motor started false otherwise.
  */
bool MCPTask_GetMotorStarted(void)
{
  return sTaskObj.motor_started;
}

/**
  * Get the motor speed value.
  * @return speed value.
  */
int32_t MCPTask_GetMotorSpeed(void)
{
  return sTaskObj.motor_speed;
}

uint8_t MCPTask_SetMotorSpeed(int32_t speed)
{
  MCPTask_cmd_t send_msg = {0};

  /* Prepare massage*/
  send_msg.cmd_id = MCPTASK_SET_SPEED_CMD;
  send_msg.val = speed;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
  return 0;
}

/**
 * @brief Enable or Disable a slow telemetry of MCPTask_Slow_telemetry array.
 *
 * @param slowTelemetryIdx Slow telemetry Index.
 * @param state True Enable
 *              False Disable
 */
void MCPTask_SetSlowTelemetryState(MCPTaskSlowTelemetry_idx_t slowTelemetryIdx, bool state)
{
  MCPTask_Slow_telemetry[slowTelemetryIdx].enable = state;
}

/**
 * @brief Get slow telemetry state: enable or disable.
 *
 * @param[in] slowTelemetryIdx Slow telemetry Index.
 * @retval state True: Enable
 *               False: Disable
 */
bool MCPTask_GetSlowTelemetryState(MCPTaskSlowTelemetry_idx_t slowTelemetryIdx)
{
  return MCPTask_Slow_telemetry[slowTelemetryIdx].enable;
}

/**
 * @brief Enable or Disable a Fast telemetry of MCPTask_Slow_telemetry array.
 *
 * @param slowTelemetryIdx Fast telemetry Index.
 * @param state True Enable
 *              False Disable
 */
void MCPTask_SetFastTelemetryState(MCPTaskFastTelemetry_idx_t fastTelemetryIdx, bool state)
{
  MCPTask_Fast_telemetry[fastTelemetryIdx].enable = state;
}

/**
 * @brief Get Fast telemetry state: enable or disable.
 *
 * @param[in] slowTelemetryIdx Fast telemetry Index.
 * @retval state True: Enable
 *               False: Disable
 */
bool MCPTask_GetFastTelemetryState(MCPTaskFastTelemetry_idx_t fastTelemetryIdx)
{
  return MCPTask_Fast_telemetry[fastTelemetryIdx].enable;
}

/**
 * @brief Get the number of enabled fast telemetry
 *
 * This function returns the number of enabled fast telemetry
 *
 * @return uint32_t The number of enabled fast telemetry
 */
uint32_t MCPTask_GetNumOfEnabledFastTelemetry(void)
{
  uint32_t num_of_enabled_ft = 0;

  /* Get Fast telemetry array len */
  uint16_t fast_telemetry_array_len = sizeof(MCPTask_Fast_telemetry) / sizeof(MCPTask_Fast_telemetry_t);

  for(uint16_t fast_telemetry_idx = 0u; fast_telemetry_idx < fast_telemetry_array_len; fast_telemetry_idx++)
  {
    if(MCPTask_Fast_telemetry[fast_telemetry_idx].enable)
    {
      num_of_enabled_ft++;
    }
  }
  return num_of_enabled_ft;
}

/**
 * @brief Get the number of enabled slow telemetry
 *
 * This function returns the number of enabled slow telemetry
 *
 * @return uint32_t The number of enabled slow telemetry
 */
uint32_t MCPTask_GetNumOfEnabledSlowTelemetry(void)
{
  uint32_t num_of_enabled_st = 0;

  /* Get Slow telemetry array len */
  uint16_t slow_telemetry_array_len = sizeof(MCPTask_Slow_telemetry) / sizeof(MCPTask_Slow_telemetry_t);

  for(uint16_t slow_telemetry_idx = 0u; slow_telemetry_idx < slow_telemetry_array_len; slow_telemetry_idx++)
  {
    if(MCPTask_Slow_telemetry[slow_telemetry_idx].enable)
    {
      num_of_enabled_st++;
    }
  }
  return num_of_enabled_st;
}

/* AManagedTask virtual functions definition */
/*********************************************/

/**
 * @brief Initializes the hardware for the MCP task
 *
 * This function initializes the GPDMA1 and USART3 hardware for the MCP task.
 *
 * @param[in] _this Pointer to the MCP task object
 * @param[in] p_params Pointer to the initialization parameters (not used)
 *
 * @return sys_error_code_t The result of the initialization
 */
sys_error_code_t MCPTask_vtblHardwareInit(AManagedTask *_this, void *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  MCPTask_t *p_obj = (MCPTask_t*)_this;*/

  return res;
}

sys_error_code_t MCPTask_vtblOnCreateTask(AManagedTask *_this, tx_entry_function_t *p_task_code, CHAR **p_name,
VOID **p_stack_start,
                                          ULONG *p_stack_size, UINT *p_priority, UINT *p_preempt_threshold, ULONG *p_timeSlice, ULONG *p_auto_start,
                                          ULONG *p_params)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MCPTask_t *p_obj = (MCPTask_t*) _this;

  p_obj->p_event_src = DataEventSrcAlloc();
  if(p_obj->p_event_src == NULL)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
  }

  IEventSrcInit(p_obj->p_event_src);

  p_obj->p_sync_evt_src = DataEventSrcAlloc();
  if(p_obj->p_sync_evt_src == NULL)
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
  }

  IEventSrcInit(p_obj->p_sync_evt_src);

  /* Initialize MCP ASPEP Protocol */
  MCP_Init(&p_obj->MCP_handle);


  /* Alloc MCP Task buffers */
  p_obj->p_tx_regID_buff = (uint16_t*) SysAlloc(sizeof(uint16_t) * MCP_TX_REG_ID_BUFF_DIM);

  if(NULL == p_obj->p_tx_regID_buff)
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  p_obj->p_tx_regValue_buff = (uint8_t*) SysAlloc(sizeof(uint8_t) * MCP_TX_REG_VALUE_BUFF_DIM);

  if(NULL == p_obj->p_tx_regValue_buff)
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* Alloc MCP Task async buffers */
  p_obj->p_asyncRegIDBuff = (uint16_t*) SysAlloc(sizeof(uint16_t) * MCPTASK_ASYNC_REGID_BUFF_SIZE);
  if(NULL == p_obj->p_asyncRegIDBuff)
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  p_obj->p_asyncRegValBuff = (uint8_t*) SysAlloc(sizeof(uint8_t) * MCPTASK_ASYNC_REGVAL_BUFF_SIZE);
  if(NULL == p_obj->p_asyncRegValBuff)
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* Allocate MCP Thread for transmission side */
  p_obj->mcp_send_thread_memory_pointer = (UCHAR*) SysAlloc(MCP_SEND_CFG_STACK_DEPTH);
  if(p_obj->mcp_send_thread_memory_pointer == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* Create the MCP send thread.  */
  tx_thread_create(&p_obj->mcp_send_thread, "MCP Send Thread", MCPTask_SendThread, (ULONG )p_obj, p_obj->mcp_send_thread_memory_pointer, MCP_SEND_CFG_STACK_DEPTH,
                   MCPTASK_CFG_PRIORITY, MCPTASK_CFG_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);


  /* Allocate the MCP send message queue. */
  p_obj->p_mcp_send_queue_memory = (UCHAR*) SysAlloc(DEFAULT_MCP_SEND_QUEUE_SIZE * sizeof(MCPTask_cmd_t));
  if(p_obj->p_mcp_send_queue_memory == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* initialize MCP send message queue */
  if(TX_SUCCESS != tx_queue_create(&p_obj->mcp_send_queue, "MCP Send queue", sizeof(MCPTask_cmd_t) / 4, p_obj->p_mcp_send_queue_memory,
                         DEFAULT_MCP_SEND_QUEUE_SIZE * sizeof(MCPTask_cmd_t)))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_BASE_INIT_TASK_ERROR_CODE);
    res = SYS_BASE_INIT_TASK_ERROR_CODE;
  }

  /* create the MCP telemetry request timer */
  if(TX_SUCCESS
      != tx_timer_create(&p_obj->mcp_telemetry_request_timer, "MCP_TLM_T", MCPTask_RequestTelemtryTimerCb, (ULONG)TX_NULL,
                         AMT_MS_TO_TICKS(MCP_TELEMETRY_REQUEST_TIMER_PERIOD_MS), AMT_MS_TO_TICKS(MCP_TELEMETRY_REQUEST_TIMER_PERIOD_MS), TX_NO_ACTIVATE))
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
  /* create the MCP telemetry request timer */
  if(TX_SUCCESS
      != tx_timer_create(&p_obj->mcp_fake_fastlemetry_timer, "MCP_FAKE_FAST_TELE", MCPTask_FakeFastTelemtryTimerCb, (ULONG)TX_NULL,
                         AMT_MS_TO_TICKS(MCP_FAKE_FAST_TELEMETRY), AMT_MS_TO_TICKS(MCP_FAKE_FAST_TELEMETRY), TX_NO_ACTIVATE))
  {
    res = SYS_MCP_TASK_INIT_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }
#endif
  /* Allocate the MCP send message queue. */
  p_obj->p_mcp_receive_queue_memory = (UCHAR*) SysAlloc(DEFAULT_MCP_SEND_QUEUE_SIZE * sizeof(MCPTask_cmd_t));
  if(p_obj->p_mcp_receive_queue_memory == NULL)
  {
    res = SYS_TASK_HEAP_OUT_OF_MEMORY_ERROR_CODE;
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(res);
    return res;
  }

  /* initialize MCP send message queue */
  if(TX_SUCCESS != tx_queue_create(&p_obj->mcp_receive_queue, "MCP Receive queue", sizeof(MCPTask_cmd_t)/4, p_obj->p_mcp_receive_queue_memory, DEFAULT_MCP_SEND_QUEUE_SIZE * sizeof(MCPTask_cmd_t)))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_BASE_INIT_TASK_ERROR_CODE);
    res = SYS_BASE_INIT_TASK_ERROR_CODE;
  }

  /* initialize MCP send semaphore */
  if(TX_SUCCESS != tx_semaphore_create(&p_obj->mcp_send_semaphore, "mcp_send_semaphore", 0))
  {
    SYS_SET_SERVICE_LEVEL_ERROR_CODE(SYS_OUT_OF_MEMORY_ERROR_CODE);
    res = SYS_OUT_OF_MEMORY_ERROR_CODE;
  }

  /* set the (PM_STATE, ExecuteStepFunc) map from the class object.  */
  _this->m_pfPMState2FuncMap = sTheClass.p_pm_state2func_map;

  *p_task_code = AMTExRun;
  *p_name = "MCPTASK";
  *p_stack_start = NULL; /* allocate the task stack in the system memory pool.*/
  *p_stack_size = MCPTASK_CFG_STACK_DEPTH;
  *p_params = (ULONG) _this;
  *p_priority = MCPTASK_CFG_PRIORITY;
  *p_preempt_threshold = MCPTASK_CFG_PRIORITY;
  *p_timeSlice = TX_NO_TIME_SLICE;
  *p_auto_start = TX_AUTO_START;

  return res;
}

sys_error_code_t MCPTask_vtblDoEnterPowerMode(AManagedTask *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MCPTask_t *p_obj = (MCPTask_t*)_this;

  if(active_power_mode == E_POWER_MODE_STATE1 && new_power_mode == E_POWER_MODE_SENSORS_ACTIVE)
  {
    if(p_obj->motor_started)
    {
      p_obj->enabled_slow_telemetries_cnt = MCPTask_GetNumOfEnabledSlowTelemetry();

      if(p_obj->enabled_slow_telemetries_cnt)
      {
        /* On start log allocate the memory buffer used for slow telemetries*/
        p_obj->p_slow_telemetry_buffer = SysAlloc(p_obj->enabled_slow_telemetries_cnt * sizeof(uint32_t));

        /* Start timer for synchronous telemetries */
        tx_timer_activate(&p_obj->mcp_telemetry_request_timer);

        p_obj->enabled_slow_telemetries = true;
      }

      if(MCPTask_GetNumOfEnabledFastTelemetry())
      {
        MCPTask_EnableFastTelemetry(NULL);

        p_obj->enabled_fast_telemetries = true;
      }
    }

    p_obj->log_started = true;
  }
  else if(E_POWER_MODE_STATE1 == new_power_mode)
  {
    p_obj->log_started = false;

    if(p_obj->enabled_slow_telemetries)
    {
      p_obj->enabled_slow_telemetries = false;

      /* Stop timer for synchronous telemetry */
      tx_timer_deactivate(&p_obj->mcp_telemetry_request_timer);

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
      tx_timer_deactivate(&p_obj->mcp_fake_fastlemetry_timer);
#endif

      if(p_obj->slow_telemetries_buffer_allocated)
      {
        /* On stop log deallocate the memory buffer used for slow telemetries*/
        SysFree(p_obj->p_slow_telemetry_buffer);
        p_obj->slow_telemetries_buffer_allocated = false;
      }
    }

    if(p_obj->enabled_fast_telemetries)
    {
      p_obj->enabled_fast_telemetries = false;
      MCPTask_DisableFastTelemetry(NULL);
    }
  }

  return res;
}

sys_error_code_t MCPTask_vtblHandleError(AManagedTask *_this, SysEvent error)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  /*MCPTask_t *p_obj = (MCPTask_t*)_this;*/

  return res;
}

sys_error_code_t MCPTask_vtblOnEnterTaskControlLoop(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MCPTask_t *p_obj = (MCPTask_t*)_this;


  /* Register MCP callback */
  MCP_RegisterDeliveredSyncPacketCb(p_obj->MCP_handle, MCPTask_OnDeliveredTelemetryPacketCb);

  MCP_RegisterDeliveredAsyncPacketCb(p_obj->MCP_handle, MCPTask_saveAsyncTelemetry_cb);

  /* Register MCP connect evt callback */
  MCP_RegisterConnectCompletedCb(p_obj->MCP_handle, MCPTask_connection_completed_cb);

  /* Register ASPEP after sending cb */
  ASPEP_RegisterTransmitting_cb(MCPTask_after_sending_cb);

  /* Register ASPEP send complete cb */
  ASPEP_RegisterTxComplete_cb( MCPTask_send_complete_cb);

  /* Register ASPEP new packet available cb */
  ASPEP_RegisterNewPacket_cb( MCPTask_receive_new_packet_cb);

  /* Register ASPEP delay function*/
  ASPEP_RegisterDelay_cb( MCPTask_delay_cb);

  SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("MCPTask: start.\r\n"));

  return res;
}

/* AManagedTaskEx virtual functions definition */
/***********************************************/

sys_error_code_t MCPTask_vtblForceExecuteStep(AManagedTaskEx *_this, EPowerMode active_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
  MCPTask_t *p_obj = (MCPTask_t*) _this;

  MCPTask_cmd_t xReport = {0};
  xReport.cmd_id = MCPTASK_MESSAGE_ID_FORCE_STEP;

  if((active_power_mode == E_POWER_MODE_STATE1) || (active_power_mode == E_POWER_MODE_SENSORS_ACTIVE))
  {
    if(AMTExIsTaskInactive(_this))
    {
      res = MCPTask_postReportToFront(p_obj, (MCPTask_cmd_t*) &xReport);
    }
  }
  else
  {
    UINT state;
    if(TX_SUCCESS == tx_thread_info_get(&_this->m_xTaskHandle, TX_NULL, &state, TX_NULL, TX_NULL, TX_NULL, TX_NULL,
    TX_NULL,TX_NULL))
    {
      if(state == TX_SUSPENDED)
      {
        tx_thread_resume(&_this->m_xTaskHandle);
      }
    }
  }

  return res;
}

sys_error_code_t MCPTask_vtblOnEnterPowerMode(AManagedTaskEx *_this, const EPowerMode active_power_mode, const EPowerMode new_power_mode)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;
/*  MCPTask_t *p_obj = (MCPTask_t*)_this;*/

  return res;
}

/*Private function definition*/
/*****************************/
/**
 * Execute one step of the task control loop while the system is in STATE1 state.
 *
 * @param _this [IN] specifies a pointer to a task object.
 * @return SYS_NO_EROR_CODE if success, a task specific error code otherwise.
 */
static sys_error_code_t MCPTask_ExecuteStepState1(AManagedTask *_this)
{
  assert_param(_this != NULL);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  MCPTask_t *p_obj = (MCPTask_t*) _this;
  MCPTask_cmd_t send_cmd = {0};

  AMTExSetInactiveState((AManagedTaskEx*) _this, TRUE);
  if(TX_SUCCESS == tx_queue_receive(&p_obj->mcp_receive_queue, &send_cmd, TX_WAIT_FOREVER))
  {
    AMTExSetInactiveState((AManagedTaskEx*) _this, FALSE);
    switch(send_cmd.cmd_id)
    {
      case MCPTASK_MESSAGE_ID_FORCE_STEP:
        {
          /* do nothing. I need only to resume the task. */
          __NOP();
        }
        break;

      case MCPTASK_NEW_PACKET_AVAILABLE:
        {
          MCPTask_Scheduler(sTaskObj.MCP_handle);
        }

      default:
        break;
    }
  }

  return res;
}

/**
 * @Brief Motor control protocol Send message thread
 */
static void MCPTask_SendThread(ULONG thread_input)
{

  MCPTask_t *p_obj = (MCPTask_t*) thread_input;
  MCPTask_cmd_t send_cmd = {0};

  while(1)
  {
    if(TX_SUCCESS == tx_queue_receive(&p_obj->mcp_send_queue, &send_cmd, TX_WAIT_FOREVER))
    {
      switch(send_cmd.cmd_id)
      {
        case MCPTASK_CONNECT_CMD:
          {
            MCP_ConnectionRequest();
          }
          break;

        case MCPTASK_DISCONNECT_CMD:
        {
          MCP_DisconnectionRequest(p_obj->MCP_handle);
        }
        break;

        case MCPTASK_START_CMD:
          {
#ifndef SYS_ENABLE_BLE_FAKE_SLOW_TELE
            MCP_StartMotorRequest(p_obj->MCP_handle);
#endif

            /* If log already started and slow or fast telemetries are enabled
             * allocated and initialized memory and software timer for slow telemetry
             * and enable fast telemetry
             *
             **/
            if(p_obj->log_started)
            {
              p_obj->enabled_slow_telemetries_cnt = MCPTask_GetNumOfEnabledSlowTelemetry();

              if(p_obj->enabled_slow_telemetries_cnt)
              {
                if(!p_obj->slow_telemetries_buffer_allocated)
                {
                  p_obj->slow_telemetries_buffer_allocated = true;
                  /* On start log allocate the memory buffer used for slow telemetries*/
                  p_obj->p_slow_telemetry_buffer = SysAlloc(p_obj->enabled_slow_telemetries_cnt * sizeof(uint32_t));
                }

                /* Start timer for synchronous telemetries */
                tx_timer_activate(&p_obj->mcp_telemetry_request_timer);

                p_obj->enabled_slow_telemetries = true;

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
                tx_timer_activate(&p_obj->mcp_fake_fastlemetry_timer);
#endif
              }
#ifndef SYS_ENABLE_BLE_FAKE_SLOW_TELE
              if(MCPTask_GetNumOfEnabledFastTelemetry())
              {
                MCPTask_EnableFastTelemetry(NULL);

                p_obj->enabled_fast_telemetries = true;
              }
#endif
            }
            p_obj->motor_started = true;
          }
          break;

        case MCPTASK_ENABLE_FAST_TELEMETRY_CMD:
          {
            MCPTask_SendAsyncsRequest();
          }
          break;

        case MCPTASK_DISABLE_FAST_TELEMETRY_CMD:
          {
            MCPTask_DisableAsyncsRequest();
          }
          break;

        case MCPTASK_STOP_CMD:
          {
#ifdef SYS_ENABLE_BLE_FAKE_SLOW_TELE
            if(p_obj->log_started)
            {

              /* Get Slow telemetry array len */
              uint16_t slow_telemetry_array_len = sizeof(MCPTask_Slow_telemetry) / sizeof(MCPTask_Slow_telemetry_t);
              /* Get number of enabled slow telemetries */
              for(uint16_t slow_telemetry_idx = 0u; slow_telemetry_idx < slow_telemetry_array_len; slow_telemetry_idx++)
              {
                /*Reset Counter*/
                MCPTask_Slow_telemetry[slow_telemetry_idx].value = MCPTask_Slow_telemetry_Init_val[slow_telemetry_idx];
              }
            }
#else
            MCP_StopMotorRequest(p_obj->MCP_handle);
#endif
            /* Stop timer for synchronous telemetry */
            tx_timer_deactivate(&p_obj->mcp_telemetry_request_timer);

            p_obj->motor_started = false;
          }
          break;

        case MCPTASK_TELEMETRY_REQUEST_CMD:
          {

#ifdef SYS_ENABLE_BLE_FAKE_SLOW_TELE
            uint32_t *p_emdata_buff = (uint32_t*) sTaskObj.p_slow_telemetry_buffer;

            /* Get Slow telemetry array len */
            uint16_t slow_telemetry_array_len = sizeof(MCPTask_Slow_telemetry) / sizeof(MCPTask_Slow_telemetry_t);

            /* Get number of enabled slow telemetries */
            for(uint16_t slow_telemetry_idx = 0u; slow_telemetry_idx < slow_telemetry_array_len; slow_telemetry_idx++)
            {
              if(MCPTask_Slow_telemetry[slow_telemetry_idx].enable)
              {
                if(slow_telemetry_idx == MC_REG_FAULTS_FLAGS_IDX)
                {
                  MCPTask_Slow_telemetry[slow_telemetry_idx].value = 0;
                }
                *p_emdata_buff = MCPTask_Slow_telemetry[slow_telemetry_idx].value;
                /*Reset Counter*/
                if(++MCPTask_Slow_telemetry[slow_telemetry_idx].value >= MCPTASK_MAX_FAKE_SLOW_TELEMETRY_VAL)
                {
                  MCPTask_Slow_telemetry[slow_telemetry_idx].value = MCPTask_Slow_telemetry_Init_val[slow_telemetry_idx];
                }
                p_emdata_buff++;
              }
            }

            /* Send here EMData to the linked observer */
            /* Initialize slow telemetry EMData */
            if(MCPTask_GetNumOfEnabledSlowTelemetry() > 0)
            {
              EMD_1dInit(&sTaskObj.slow_telemetry_data, (uint8_t*) sTaskObj.p_slow_telemetry_buffer,
                          E_EM_UINT32, sTaskObj.enabled_slow_telemetries_cnt);

              DataEvent_t evt;
              double timestamp = SysTsGetTimestampF(SysGetTimestampSrv());
              DataEventInit((IEvent*) &evt, sTaskObj.p_event_src, &sTaskObj.slow_telemetry_data, timestamp, 0);
              IEventSrcSendEvent(sTaskObj.p_event_src, (IEvent*) &evt, NULL);
            }

#else
            MCPTask_SendTelemetryRequest(p_obj);
#endif
          }
          break;

        case MCPTASK_SET_SPEED_CMD:
          {
            p_obj->motor_speed = send_cmd.val;
            uint16_t speedReg = MC_REG_SPEED_REF;
            MCP_SetRegister(sTaskObj.MCP_handle, 1, &speedReg, (uint8_t*) &send_cmd.val);
          }
          break;

        case MCPTASK_ACK_FAULT_CMD:
          {
            MCP_SendAckFault(sTaskObj.MCP_handle);
          }
          break;

        default:
          break;
      }
    }

    tx_thread_sleep(AMT_MS_TO_TICKS(4));
  }
}

/**
 * @Brief Motor control protocol scheduler
 */
static void MCPTask_Scheduler(MCP_Handle_t *p_MCP_handle)
{
  MCP_ReceiveAndEventProcess(p_MCP_handle);
}

/**
 *
 * @brief  Send telemetry request to get motor status information.
 *         This function sends a telemetry request to get motor status information
 *         such as motor speed, fault flags, currents, heats, and bus voltage.
 * @param  _this Pointer to the instance of MCPTask_t.
 * @return sys_error_code_t Returns the system error code if any occurred.
 *         SYS_NO_ERROR_CODE if successful.
*/
#ifndef SYS_ENABLE_BLE_FAKE_SLOW_TELE
static sys_error_code_t MCPTask_SendTelemetryRequest(MCPTask_t *_this)
{
  sys_error_code_t retval = SYS_NO_ERROR_CODE;
  uint8_t NumberReg = 0u;
  uint16_t *RegID = (uint16_t*) _this->p_tx_regID_buff;

  /* Get Slow telemetry array len */
  uint16_t slow_telemetry_array_len = sizeof(MCPTask_Slow_telemetry)/sizeof(MCPTask_Slow_telemetry_t);

  for(uint16_t slow_telemetry_idx = 0u; slow_telemetry_idx < slow_telemetry_array_len; slow_telemetry_idx++)
  {
    if(MCPTask_Slow_telemetry[slow_telemetry_idx].enable)
    {
      *RegID = MCPTask_Slow_telemetry[slow_telemetry_idx].reg_id;
      RegID += 1;
      NumberReg++;
    }
  }

  (void)MCP_GetRegister(_this->MCP_handle, NumberReg, _this->p_tx_regID_buff);

  return retval;
}
#endif


/**
 * @brief Sends a command to enable fast telemetry to Motor Control Processor task.
 *
 * @param _this A pointer to the Motor Control Processor task instance.
 */
static void MCPTask_EnableFastTelemetry(MCPTask_t *_this)
{
  MCPTask_cmd_t send_msg = {0};
  send_msg.cmd_id = MCPTASK_ENABLE_FAST_TELEMETRY_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
}

/**
 * @brief Sends a command to enable fast telemetry to Motor Control Processor task.
 *
 * @param _this A pointer to the Motor Control Processor task instance.
 */
static void MCPTask_DisableFastTelemetry(MCPTask_t *_this)
{
  MCPTask_cmd_t send_msg = {0};
  send_msg.cmd_id = MCPTASK_DISABLE_FAST_TELEMETRY_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
}

/* Callback definition */
/***********************/

/**
 * @brief Callback function for the telemetry request timer. It sends a message to the MCP queue to request telemetry data.
 * @param timer The timer ID.
 * @note This function is called by the system timer service and should not be called directly.
 */
static void MCPTask_RequestTelemtryTimerCb(ULONG timer)
{
  ULONG send_msg = MCPTASK_TELEMETRY_REQUEST_CMD;
  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
}

#ifdef SYS_ENABLE_FAKE_AI_CLASSIFIER
uint16_t fake_fast_telemetry[1020];
static void MCPTask_FakeFastTelemtryTimerCb(ULONG timer)
{
  /* Get timestamp */
  double timestamp = SysTsGetTimestampF(SysGetTimestampSrv());

  /* Initialize Fast Async telemetry EMData */
  DataEvent_t evt;
  EMD_1dInit(&sTaskObj.async_telemetry_data, (uint8_t*)fake_fast_telemetry, E_EM_UINT16, 1020);
  DataEventInit((IEvent*) &evt, sTaskObj.p_event_src, &sTaskObj.async_telemetry_data, timestamp, MCP_FAST_TELEMETRY_EMDATA_TAG);

  /* Send EMData to the linked observer */
  IEventSrcSendEvent(sTaskObj.p_event_src, (IEvent*) &evt, NULL);
}
#endif

static void MCPTask_OnDeliveredTelemetryPacketCb(uint8_t CallbackID, uint8_t MCPResponse, uint8_t *rxData, uint16_t *rxLength)
{
  if(MCPResponse != MCP_CMD_OK)
  {
    /* Handle non-okay responses here, if necessary. */
    return;
  }

  switch(CallbackID)
  {
    case SET_DATA_ELEMENT:
      /* Handle SET_DATA_ELEMENT case. */
      break;

    case START_MOTOR:
      /* Handle START_MOTOR case. */
      break;

    case STOP_MOTOR:
      /* Handle STOP_MOTOR case. */
      break;

    case GET_DATA_ELEMENT:
      MCPTask_SlowTelemetryGetDataElement(rxData, rxLength);
      MCPTask_SlowTelemetryUpdateEMDataBuffer((uint32_t*) sTaskObj.p_slow_telemetry_buffer, MCPTask_Slow_telemetry, SLOW_TELEMETRY_ARRAY_DIM);
      MCPTask_SlowTelemetrySendEMData();
      break;
  }
}

/**
 * @brief Processes received data elements and saves telemetry information.
 *
 * This function iterates over the received data, identified by `rxData` and `rxLength`,
 * and processes each data element by saving the telemetry information associated with it.
 *
 * @param[in,out] rxData Pointer to the buffer containing the received data elements.
 * @param[in,out] rxLength Pointer to the variable holding the length of the received data.
 *               This length is decremented by the size of each processed data element.
 *
 * @warning The function does not perform bounds checking on the `rxData` buffer beyond the initial
 *          length check. It is the caller's responsibility to ensure that the buffer is large enough
 *          to contain all the data elements to be processed.
 */
static void MCPTask_SlowTelemetryGetDataElement(uint8_t *rxData, uint16_t *rxLength)
{
  uint8_t size = 0U;
  uint16_t dataID;
  uint16_t *txData = sTaskObj.p_tx_regID_buff;

  while(*rxLength > 1)
  {
    dataID = *txData;
    MCPTask_SlowTelemetrySave(dataID, &size, rxData);

    txData++;
    *rxLength -= size;
    rxData += size;
  }
}

/**
 * @brief Updates the EM data buffer with the values from the slow telemetry data structure.
 *
 * This function iterates through the slow telemetry data structure and updates the EM data buffer
 * with the values of enabled telemetry data points.
 *
 * @param[in,out] emDataBuffer Pointer to the EM data buffer to be updated.
 * @param[in] telemetryValue The value of the telemetry data to update in the buffer.
 * @param[in] slowTelemetryArray Pointer to the array of slow telemetry data.
 * @param[in] slowTelemetryArrayLen Length of the slow telemetry data array.
 */
static void MCPTask_SlowTelemetryUpdateEMDataBuffer(uint32_t *emDataBuffer, MCPTask_Slow_telemetry_t *slowTelemetryArray, uint16_t slowTelemetryArrayLen)
{
  for(uint16_t idx = 0; idx < slowTelemetryArrayLen; idx++)
  {
    if(slowTelemetryArray[idx].enable)
    {
      *emDataBuffer++ = slowTelemetryArray[idx].value;
    }
  }
}

/**
 * @brief Sends the slow telemetry data as an event.
 *
 * This function initializes the slow telemetry data structure with the current telemetry buffer,
 * creates a data event with a timestamp, and sends the event through the event source.
 *
 * @note This function relies on several external functions such as EMD_1dInit, SysTsGetTimestampF,
 *       DataEventInit, and IEventSrcSendEvent to perform its operations.
 *
 * @warning The function assumes that the telemetry buffer and the event source have been properly
 *          initialized before calling this function.
 */
static void MCPTask_SlowTelemetrySendEMData(void)
{
    EMD_1dInit(&sTaskObj.slow_telemetry_data, (uint8_t *)sTaskObj.p_slow_telemetry_buffer, E_EM_UINT32, sTaskObj.enabled_slow_telemetries_cnt);

    DataEvent_t evt;
    double timestamp = SysTsGetTimestampF(SysGetTimestampSrv());
    DataEventInit((IEvent *)&evt, sTaskObj.p_event_src, &sTaskObj.slow_telemetry_data, timestamp, 0);
    IEventSrcSendEvent(sTaskObj.p_event_src, (IEvent *)&evt, NULL);
}

static MCPTaskSlowTelemetry_idx_t MCPTask_SlowTelemetryConvertTokenToIdx(uint16_t dataID)
{
  switch(dataID)
  {
    case MC_REG_SPEED_REF:
      return MC_REG_SPEED_REF_IDX;
    case MC_REG_SPEED_MEAS:
      return MC_REG_SPEED_MEAS_IDX;
    case MC_REG_HEATS_TEMP:
      return MC_REG_HEATS_TEMP_IDX;
    case MC_REG_BUS_VOLTAGE:
      return MC_REG_BUS_VOLTAGE_IDX;
    case MC_REG_FAULTS_FLAGS:
      return MC_REG_FAULTS_FLAGS_IDX;
    default:
      // Handle unknown dataID if needed, or return an invalid index
      return MC_REG_IDX_COUNT;
  }
}

/**
 * @brief Saves telemetry data by extracting and assigning it to the slow telemetry structure.
 *
 * This function serves as a high-level interface that coordinates the extraction of telemetry data
 * from a raw buffer and its assignment to the correct place in the @MCPTask_Slow_telemetry array.
 * It uses the MCPTask_ExtractTelemetryData and MCPTask_AssignTelemetryValue functions to perform its tasks.
 *
 * @param[in] dataID The identifier for the data, which includes the type ID.
 * @param[out] size Pointer to a variable where the size of the data will be stored.
 * @param[in] rxData Pointer to the buffer containing the raw data to be interpreted.
 */
static void MCPTask_SlowTelemetrySave(uint16_t dataID, uint8_t *size, uint8_t *rxData)
{
  int32_t slow_telemetry_data;

  /* Extract telemetry data and determine the size */
  slow_telemetry_data = MCPTask_SlowTelemetryExtractData(dataID, size, rxData);

  /* Convert token to index */
  MCPTaskSlowTelemetry_idx_t idx = MCPTask_SlowTelemetryConvertTokenToIdx(dataID);

  /* Assign the telemetry value based on the index */
  MCPTask_SlowTelemetrySaveIntoArray(MCPTask_Slow_telemetry, idx, slow_telemetry_data);
}

/**
 * @brief Extracts telemetry data from the received buffer and determines its size.
 *
 * This function interprets the received data based on its type ID, which is extracted from the data ID.
 * It then casts the raw data to the correct data type and determines the size of the data.
 *
 * @param[in] dataID The identifier for the data, which includes the type ID.
 * @param[out] size Pointer to a variable where the size of the data will be stored.
 * @param[in] rxData Pointer to the buffer containing the raw data to be interpreted.
 * @return The extracted telemetry data as an int32_t.
 */
static int32_t MCPTask_SlowTelemetryExtractData(uint16_t dataID, uint8_t *size, uint8_t *rxData)
{
  uint8_t typeID = dataID & TYPE_MASK;
  int32_t telemetry_data = 0;

  switch(typeID)
  {
    case TYPE_DATA_8BIT:
      telemetry_data = (*(uint8_t*) rxData);
      *size = sizeof(uint8_t);
      break;
    case TYPE_DATA_16BIT:
      telemetry_data = (*(uint16_t*) rxData);
      *size = sizeof(uint16_t);
      break;
    case TYPE_DATA_32BIT:
      telemetry_data = (*(uint32_t*) rxData);
      *size = sizeof(uint32_t);
      break;
    default:
      *size = 0; /* Set to 0 if type is not recognized */
      break;
  }

  return telemetry_data;
}

/**
 * @brief Processes fault data and updates the motor state if necessary.
 *
 * This function takes the raw telemetry fault data, extracts the current and previous fault values,
 * converts them to internal indices, and updates the motor state if a fault is present.
 *
 * @param[in] telemetry_data The raw telemetry data containing fault information.
 * @param[out] current_fault_idx Pointer to a variable where the current fault index will be stored.
 * @param[out] prev_fault_idx Pointer to a variable where the previous fault index will be stored.
 * @return None.
 */
static inline void MCPTask_ProcessFaultData(int32_t telemetry_data, int32_t *current_fault_idx, int32_t *prev_fault_idx)
{
  int32_t fault = telemetry_data;
  int32_t current_fault = (fault >> 16) & 0xFFFF;
  int32_t prev_fault = fault & 0xFFFF;

  *current_fault_idx = MCPTask_convertHexFaultToIntIdx(current_fault);
  *prev_fault_idx = MCPTask_convertHexFaultToIntIdx(prev_fault);

  // If fault present, set motor in stopped state
  if(MCP_NO_ERROR != *prev_fault_idx)
  {
    sTaskObj.motor_started = false;
  }
}

/**
 * @brief Assigns the extracted telemetry data to the appropriate index in the telemetry structure.
 *
 * This function takes the telemetry data and assigns it to the correct index in the provided slow telemetry
 * structure. For fault flags, it also processes the data to extract current and previous faults
 * and sets the motor state accordingly.
 *
 * @param[in] p_SlowTelemetryArray Pointer to the slow telemetry structure where the data should be assigned.
 * @param[in] idx The index in the slow telemetry structure where the data should be assigned.
 * @param[in] telemetry_data The telemetry data to be assigned to the structure.
 */
static void MCPTask_SlowTelemetrySaveIntoArray(MCPTask_Slow_telemetry_t *p_SlowTelemetryArray, MCPTaskSlowTelemetry_idx_t idx, int32_t telemetry_data)
{
  switch(idx)
  {
    case MC_REG_BUS_VOLTAGE_IDX:
    case MC_REG_HEATS_TEMP_IDX:
    case MC_REG_SPEED_MEAS_IDX:
    case MC_REG_SPEED_REF_IDX:
      {
        p_SlowTelemetryArray[idx].value = telemetry_data;
      }
      break;
    case MC_REG_FAULTS_FLAGS_IDX:
      {
        int32_t current_fault_idx, prev_fault_idx;
        MCPTask_ProcessFaultData(telemetry_data, &current_fault_idx, &prev_fault_idx);
        p_SlowTelemetryArray[idx].value = prev_fault_idx;
      }
      break;
    default:
      /* Handle unknown index if needed */
      break;
  }
}

/**
 * @brief Posts a message report to the front of an MCP task queue.
 *
 * @param _this A pointer to the MCP task object.
 * @param pReport A pointer to the report to be posted to the front of the MCP task queue.
 *
 * @return Returns a system error code indicating the success or failure of the operation.
 */
static inline sys_error_code_t MCPTask_postReportToFront(MCPTask_t *_this, MCPTask_cmd_t *pReport)
{
  assert_param(_this != NULL);
  assert_param(pReport);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  if(SYS_IS_CALLED_FROM_ISR())
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->mcp_receive_queue, pReport, TX_NO_WAIT))
    {
      res = SYS_MCP_TASK_MSG_LOST_ERROR_CODE;
    }
  }
  else
  {
    if(TX_SUCCESS != tx_queue_front_send(&_this->mcp_receive_queue, pReport, AMT_MS_TO_TICKS(100)))
    {
      res = SYS_MCP_TASK_MSG_LOST_ERROR_CODE;
    }
  }

  return res;
}

/**
 * @brief Converts a hexadecimal fault code to its corresponding integer index.
 *
 * This function takes a hexadecimal fault code and returns its corresponding integer index according to the
 * following mapping:
 *
 * | Hexadecimal Fault Code | Integer Index |
 * |------------------------|---------------|
 * | MCP_NO_ERROR           | 0             |
 * | MCP_DURATION           | 1             |
 * | MCP_OVER_VOLT          | 2             |
 * | MCP_UNDER_VOLT         | 3             |
 * | MCP_OVER_TEMP          | 4             |
 * | MCP_START_UP           | 5             |
 * | MCP_SPEED_FDBK         | 6             |
 * | MCP_BREAK_IN           | 7             |
 * | MCP_SW_ERROR           | 8             |
 *
 * @param hex_fault The hexadecimal fault code to convert.
 * @return The corresponding integer index.
 */
static inline int32_t MCPTask_convertHexFaultToIntIdx(int32_t hex_fault)
{
  switch(hex_fault)
  {
    case MCP_NO_ERROR:
      return MCP_NO_ERROR_VAL;
    case MCP_DURATION:
      return MCP_DURATION_VAL;
    case MCP_OVER_VOLT:
      return MCP_OVER_VOLT_VAL;
    case MCP_UNDER_VOLT:
      return MCP_UNDER_VOLT_VAL;
    case MCP_OVER_TEMP:
      return MCP_OVER_TEMP_VAL;
    case MCP_START_UP:
      return MCP_START_UP_VAL;
    case MCP_SPEED_FDBK:
      return MCP_SPEED_FDBK_VAL;
    case MCP_BREAK_IN:
      return MCP_BREAK_IN_VAL;
    case MCP_SW_ERROR:
      return MCP_SW_ERROR_VAL;
    default:
      return -1;
  }
}

/**
 * @Brief Sends asynchronous requests for Fast telemetry.
 *
 * This function collects the registration IDs (RegID) of enabled Fast telemetry items
 * and sends the asynchronous requests using MCPA_SetAsyncs function.
 */
static void MCPTask_SendAsyncsRequest(void)
{
  uint16_t *RegID = sTaskObj.p_asyncRegIDBuff;
  uint8_t HFNum = 0;

  /* Get Fast telemetry array len */
  uint16_t fast_telemetry_array_len = sizeof(MCPTask_Fast_telemetry) / sizeof(MCPTask_Fast_telemetry_t);

  for(uint16_t fast_telemetry_idx = 0u; fast_telemetry_idx < fast_telemetry_array_len; fast_telemetry_idx++)
  {
    if(MCPTask_Fast_telemetry[fast_telemetry_idx].enable)
    {
      /* Store the RegID of enabled Fast telemetry item */
      *RegID = MCPTask_Fast_telemetry[fast_telemetry_idx].reg_id;
      RegID += 1;
      HFNum++;
    }
  }

  /* Send asynchronous requests using MCPA_SetAsyncs function */
  MCP_SetAsyncs(sTaskObj.MCP_handle, sTaskObj.p_asyncRegIDBuff, sTaskObj.p_asyncRegValBuff, HFNum);
}


static void MCPTask_DisableAsyncsRequest(void)
{
  /* Send asynchronous requests using MCPA_SetAsyncs function */
  MCP_DisableAsyncs(sTaskObj.MCP_handle, sTaskObj.p_asyncRegValBuff);
}

/* IMotor_Controller_vtbl virtual functions */
/********************************************/
uint8_t MCPTask_connect_vtbl(void)
{
  return 0;
}

uint8_t MCPTask_start_motor(void)
{
  MCPTask_cmd_t send_msg = {0};

#ifndef SYS_ENABLE_BLE_FAKE_SLOW_TELE
  send_msg.cmd_id = MCPTASK_CONNECT_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
#endif
  /* Prepare massage*/
  send_msg.cmd_id = MCPTASK_START_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
  return 0;
}

uint8_t MCPTask_stop_motor(void)
{
  MCPTask_cmd_t send_msg = {0};

  /* Prepare massage*/
  send_msg.cmd_id = MCPTASK_STOP_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }

  send_msg.cmd_id = MCPTASK_DISCONNECT_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }


  return 0;
}


uint8_t MCPTask_motor_reset(void)
{
  MCPTask_cmd_t send_msg = {0};

  send_msg.cmd_id = MCPTASK_RESET_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
  return 0;
}

uint8_t MCPTask_ack_fault(void)
{
  MCPTask_cmd_t send_msg = {0};

  /* Prepare massage*/
  send_msg.cmd_id = MCPTASK_ACK_FAULT_CMD;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_send_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the message. Signal the error */
    sys_error_handler();
  }
  return 0;
}

/* MCP callback definition */
/***************************/

/**
 * @brief Callback function called upon completion of a connection attempt
 *
 * @param state A boolean value indicating the current state of the connection attempt:
 *              true for successful connection and false for disconnection.
 *
 * @return This function does not return any value.
 */
static void MCPTask_connection_completed_cb(bool state)
{
  if(state)
  {
    /* slave connected */
  }
  else
  {
    /* slave disconnected */
  }
}

/**
 * @brief Callback function for saving asynchronous telemetry.
 *
 * @param rxAsync    Pointer to the received asynchronous data.
 * @param rxLength   Length of the received data.
 */
static void MCPTask_saveAsyncTelemetry_cb(uint8_t *rxAsync, uint32_t rxLength)
{

  /* Get timestamp */
  double timestamp = SysTsGetTimestampF(SysGetTimestampSrv());
  uint8_t ft_num_enabled = MCPTask_GetNumOfEnabledFastTelemetry();
  uint32_t ft_expected_buff_dim = MCPTask_Fast_telemetry_receive_buff_dim[ft_num_enabled - 1];

  if(rxLength == ft_expected_buff_dim)
  {
    /* Get number of elements */
    uint32_t elements_num = rxLength / sizeof(uint16_t);

    /**
     *
     * +--------------+-----------------------+----------+
     * |              |                       |          |
     * |  TIMESTAMP   |         PAYLOAD       |   MARK   |
     * |   (4 byte)   |                       | (2 byte) |
     * +--------------+-----------------------+----------+
     *
     *  Note: Timestamp and mark are not transmitted to python App.
     *  elements_num-3: each element is an uint16_t,
     *                  elements_num-6 byte (TIMESTAMP + MARK)
     **/

    DataEvent_t evt;
    /* Initialize Fast Async telemetry EMData */
    EMD_1dInit(&sTaskObj.async_telemetry_data, (uint8_t*) &rxAsync[4], E_EM_UINT16, elements_num - 3);
    DataEventInit((IEvent*) &evt, sTaskObj.p_event_src, &sTaskObj.async_telemetry_data, timestamp, MCP_FAST_TELEMETRY_EMDATA_TAG);

    /* Send EMData to the linked observer */
    IEventSrcSendEvent(sTaskObj.p_event_src, (IEvent*) &evt, NULL);
  }
}

/**
 * @brief Callback function called upon completion of a data send operation.
 *        This function is a callback function that is called when a data send operation initiated
 *        by the MCPTask has completed.
 *        It is responsible for releasing the MCPTask send semaphore,
 *        which allows other tasks to send data.
 *
 * @param None.
 *
 * @return This function does not return any value.
 */
static void MCPTask_send_complete_cb(void)
{
  tx_semaphore_put(&sTaskObj.mcp_send_semaphore);
}

/**
 * @brief Callback function called after sending procedure
 *
 * @param None.
 *
 * @return This function does not return any value.
 */
static void MCPTask_after_sending_cb(void)
{
  tx_semaphore_get(&sTaskObj.mcp_send_semaphore, TX_WAIT_FOREVER);
}

/**
 * @brief Callback function for receiving a new packet in the MCP task.
 *
 * This function creates a new command message to notify the MCP task that a new
 * packet is available. If the message cannot be sent, the function
 * calls the sys_error_handler function to signal an error.
 */
static void MCPTask_receive_new_packet_cb(void)
{
  MCPTask_cmd_t send_msg = {0};

  send_msg.cmd_id = MCPTASK_NEW_PACKET_AVAILABLE;

  if(TX_SUCCESS != tx_queue_send(&sTaskObj.mcp_receive_queue, &send_msg, TX_NO_WAIT))
  {
    /* unable to send the report. Signal the error */
    sys_error_handler();
  }
}

/**
 * @Brief Callback function for adding delay between header and payload transmission.
 *
 * @param delay   Delay duration in milliseconds.
 */
static void MCPTask_delay_cb(uint8_t delay)
{
  tx_thread_sleep(AMT_MS_TO_TICKS(delay));
}
