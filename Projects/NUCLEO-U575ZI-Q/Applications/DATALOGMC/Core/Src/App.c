/**
  ******************************************************************************
  * @file    App.c
  * @author  SRA
  * @brief   Define the Application main entry points
  *
  *
  * This file is the main entry point for the user code.
  *
  * The framework `weak` functions are redefined in this file and they link
  * the application specific code with the framework:
  * - SysLoadApplicationContext(): it is the first application defined function
  *   called by the framework. Here we define all managed tasks. A managed task
  *   implements one or more application specific feature.
  * - SysOnStartApplication(): this function is called by the framework
  *   when the system is initialized (all managed task objects have been
  *   initialized), and before the INIT task release the control. Here we
  *   link the application objects according to the application design.
  *
  * The execution time  between the two above functions is called
  * *system initialization*. During this period only the INIT task is running.
  *
  * Each managed task will be activated in turn to initialize its hardware
  * resources, if any - MyTask_vtblHardwareInit() - and its software
  * resources - MyTask_vtblOnCreateTask().
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "AppPowerModeHelper.h"

#include "mx.h"

#include "UtilTask.h"

#include "DatalogAppTask.h"
#include "App_model.h"

#include "PnPLCompManager.h"

#include "Slow_Mc_Telemetries_PnPL.h"
#include "Fast_Mc_Telemetries_PnPL.h"
#include "Motor_Controller_PnPL.h"
#include "Automode_PnPL.h"
#include "Log_Controller_PnPL.h"
#include "Tags_Info_PnPL.h"
#include "Acquisition_Info_PnPL.h"
#include "Firmware_Info_PnPL.h"
#include "Deviceinformation_PnPL.h"
#include "MCPTask.h"


static IPnPLComponent_t *pAutomode_PnPLObj = NULL;
static IPnPLComponent_t *pLog_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pTags_Info_PnPLObj = NULL;
static IPnPLComponent_t *pAcquisition_Info_PnPLObj = NULL;
static IPnPLComponent_t *pFirmware_Info_PnPLObj = NULL;
static IPnPLComponent_t *pDeviceinformation_PnPLObj = NULL;
static IPnPLComponent_t *pMotor_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pSlow_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pFast_Mc_Telemetries_PnPLObj = NULL;

/**
  * Utility task object.
  */
static AManagedTaskEx *sUtilObj = NULL;

/**
  * DatalogApp
  */
static AManagedTaskEx *sDatalogAppObj = NULL;

/**
  * Motor Control Protocol scheduler task
  */
static AManagedTaskEx *sMCPObj = NULL;

/**
  * Pnpl mutex definition for thread safe purpose
  */
static TX_MUTEX pnpl_mutex;

/**
  * Private function declaration
  */
static void PnPL_lock_fp(void);
static void PnPL_unlock_fp(void);

/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t res = SYS_NO_ERROR_CODE;

  /* PnPL thread safe mutex creation */
  tx_mutex_create(&pnpl_mutex, "PnPL Mutex", TX_INHERIT);

  /* PnPL thread safe function registration */
  PnPL_SetLockUnlockCallbacks(PnPL_lock_fp, PnPL_unlock_fp);

  PnPLSetAllocationFunctions(SysAlloc, SysFree);

  /************ Allocate task objects ************/
  sUtilObj = UtilTaskAlloc(&MX_GPIO_LEDBlueInitParams, &MX_GPIO_UBInitParams);
  sDatalogAppObj = DatalogAppTaskAlloc();
  sMCPObj = MCPTask_Alloc();


  /************ Add the task object to the context ************/
  res = ACAddTask(pAppContext, (AManagedTask *) sUtilObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sDatalogAppObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sMCPObj);


  pLog_Controller_PnPLObj = Log_Controller_PnPLAlloc();
  pTags_Info_PnPLObj = Tags_Info_PnPLAlloc();
  pAcquisition_Info_PnPLObj = Acquisition_Info_PnPLAlloc();
  pFirmware_Info_PnPLObj = Firmware_Info_PnPLAlloc();
  pDeviceinformation_PnPLObj = Deviceinformation_PnPLAlloc();
  pAutomode_PnPLObj = Automode_PnPLAlloc();
  pSlow_Mc_Telemetries_PnPLObj = Slow_Mc_Telemetries_PnPLAlloc();
  pFast_Mc_Telemetries_PnPLObj = Fast_Mc_Telemetries_PnPLAlloc();
  pMotor_Controller_PnPLObj = Motor_Controller_PnPLAlloc();

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /************ Connect the sensor task to the bus ************/


  /************ Connect the Sensor events to the DatalogAppTask ************/

  /* Connect MCP events to DatalogApp Task */
  IEventListener *DatalogActuatorListener = DatalogAppTask_GetActuatorEventListenerIF((DatalogAppTask *) sDatalogAppObj);
  IEventSrcAddEventListener(MCPTask_GetSlowTelemetriesEventSrcIF((MCPTask_t *) sMCPObj), DatalogActuatorListener);
  IEventSrcAddEventListener(MCPTask_GetAsyncTelemetriesEventSrcIF((MCPTask_t *) sMCPObj), DatalogActuatorListener);

  /************ Other PnPL Components ************/
  Log_Controller_PnPLInit(pLog_Controller_PnPLObj);
  Tags_Info_PnPLInit(pTags_Info_PnPLObj);
  Acquisition_Info_PnPLInit(pAcquisition_Info_PnPLObj);
  Firmware_Info_PnPLInit(pFirmware_Info_PnPLObj);
  Deviceinformation_PnPLInit(pDeviceinformation_PnPLObj);
  Automode_PnPLInit(pAutomode_PnPLObj);
  Slow_Mc_Telemetries_PnPLInit(pSlow_Mc_Telemetries_PnPLObj);
  Fast_Mc_Telemetries_PnPLInit(pFast_Mc_Telemetries_PnPLObj);
  Motor_Controller_PnPLInit(pMotor_Controller_PnPLObj);

  return SYS_NO_ERROR_CODE;
}

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  /* Install the application power mode helper. */
  static IAppPowerModeHelper *s_pxPowerModeHelper = NULL;
  if (s_pxPowerModeHelper == NULL)
  {
    s_pxPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return s_pxPowerModeHelper;
}


static void PnPL_lock_fp(void)
{
  tx_mutex_get(&pnpl_mutex, TX_NO_WAIT);
}

static void PnPL_unlock_fp(void)
{
  tx_mutex_put(&pnpl_mutex);
}
