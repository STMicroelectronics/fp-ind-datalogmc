/**
  ******************************************************************************
  * @file    App.c
  * @author  SRA
  * @brief   Define the Application main entry points
  *
  * The framework `weak` function are redefined in this file and they link
  * the application specific code with the framework.
  *
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

#include "services/sysdebug.h"
#include "services/ApplicationContext.h"
#include "AppPowerModeHelper.h"

#include "HardwareDetection.h"
#include "stts22h_reg.h"

#include "mx.h"

#include "UtilTask.h"
#include "SPIBusTask.h"
#include "I2CBusTask.h"
#include "IIS3DWBTask.h"
#include "ILPS22QSTask.h"
#include "IMP23ABSUTask.h"
#include "ISM330DHCXTask.h"
#include "STTS22HTask.h"

#include "DatalogAppTask.h"
#include "MCPTask.h"
#include "DataAggregationProcessTask.h"

#include "PnPLCompManager.h"
#include "Slow_Mc_Telemetries_PnPL.h"
#include "Fast_Mc_Telemetries_PnPL.h"
#include "Iis3dwb_Acc_PnPL.h"
#include "Stts22h_Temp_PnPL.h"
#include "Log_Controller_PnPL.h"
#include "Motor_Controller_PnPL.h"
#include "Tags_Info_PnPL.h"
#include "Acquisition_Info_PnPL.h"
#include "Firmware_Info_PnPL.h"
#include "Deviceinformation_PnPL.h"
#include "Ai_Motor_Classifier_PnPL.h"

#include "parson.h"

static uint8_t BOARD_ID = BOARD_ID_BOXA;
static uint8_t FW_ID = USB_FW_ID_DATALOG2_BOXA;

static IPnPLComponent_t *pSlow_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pFast_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pIis3dwb_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pStts22h_Temp_PnPLObj = NULL;
static IPnPLComponent_t *pLog_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pMotor_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pTags_Info_PnPLObj = NULL;
static IPnPLComponent_t *pAcquisition_Info_PnPLObj = NULL;
static IPnPLComponent_t *pFirmware_Info_PnPLObj = NULL;
static IPnPLComponent_t *pDeviceinformation_PnPLObj = NULL;
static IPnPLComponent_t *pAI_Motor_Classifier_PnPLObj = NULL;

/**
  * Utility task object.
  */
static AManagedTaskEx *sUtilObj = NULL;

/**
  * Bus task object.
  */
static AManagedTaskEx *sSPI2BusObj = NULL;
static AManagedTaskEx *sI2C2BusObj = NULL;
static AManagedTaskEx *sI2C3BusObj = NULL;

/**
  * Sensor task object.
  */
static AManagedTaskEx *sIIS3DWBObj = NULL;
static AManagedTaskEx *sSTTS22HObj = NULL;

/**
  * DatalogApp
  */
static AManagedTaskEx *sDatalogAppObj = NULL;

/**
 * Motor Control Protocol scheduler task
 */
static AManagedTaskEx *sMCPObj = NULL;

/**
  * Data Aggregation and Process Task
  */
static AManagedTaskEx *sDataAggregationProcessTaskObj = NULL;

/**
 *  Flags for external Add-on
 */
static boolean_t sExtIis3dwb = FALSE;
static boolean_t sExtStts22h = FALSE;

/* eLooM framework entry points definition */
/*******************************************/

sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext)
{
  assert_param(pAppContext);
  sys_error_code_t res = SYS_NO_ERROR_CODE;


  uint8_t stts22h_address;

  /* Workaround to set malloc/free function even if BLE Init fails */
  json_set_allocation_functions(SysAlloc, SysFree);

  /* Check the HW configuration to load the tasks accordingly */
  sExtIis3dwb = HardwareDetection_Check_Ext_IIS3DWB();
  sExtStts22h = HardwareDetection_Check_Ext_STTS22H(&stts22h_address);

  PnPLSetAllocationFunctions(SysAlloc, SysFree);

  /* Allocate the task objects */
  sUtilObj = UtilTaskAlloc(&MX_GPIO_LED1InitParams, &MX_GPIO_LED2InitParams);
  sDatalogAppObj = DatalogAppTaskAlloc();

  sMCPObj = MCPTask_Alloc();
  sDataAggregationProcessTaskObj = DataAggregationProcessTask_Alloc();

  sI2C2BusObj = I2CBusTaskAlloc(&MX_I2C2InitParams);
  sSPI2BusObj = SPIBusTaskAlloc(&MX_SPI2InitParams);

  if(sExtIis3dwb)
  {
    /* Use the external IIS3DWB */
  sIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_INT1_EXTERNAL_DWBInitParams, &MX_GPIO_CS_EXTERNALInitParams);
  }
  else
  {
    /* Use the onboard IIS3DWB */
    sIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_INT1_DWBInitParams, &MX_GPIO_CS_DWBInitParams);
  }

  if(sExtStts22h)
  {
    /* Use the external STTS22H on I2C3 */
    sSTTS22HObj = STTS22HTaskAlloc(NULL, NULL, stts22h_address);
    sI2C3BusObj = I2CBusTaskAlloc(&MX_I2C3InitParams);
  }
  else
  {
    /* Use the onboard STTS22H (address low) */
    sSTTS22HObj = STTS22HTaskAlloc(NULL, NULL, STTS22H_I2C_ADD_L);
  }


  /* Add the task object to the context. */

  res = ACAddTask(pAppContext, (AManagedTask *) sMCPObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sUtilObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sI2C2BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sI2C3BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sSPI2BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sIIS3DWBObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sSTTS22HObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sDatalogAppObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sDataAggregationProcessTaskObj);

  /* PnPL Components Allocation */
  pSlow_Mc_Telemetries_PnPLObj = Slow_Mc_Telemetries_PnPLAlloc();
  pFast_Mc_Telemetries_PnPLObj = Fast_Mc_Telemetries_PnPLAlloc();
  pIis3dwb_Acc_PnPLObj = Iis3dwb_Acc_PnPLAlloc();
  pStts22h_Temp_PnPLObj = Stts22h_Temp_PnPLAlloc();
  pLog_Controller_PnPLObj = Log_Controller_PnPLAlloc();
  pMotor_Controller_PnPLObj = Motor_Controller_PnPLAlloc();
  pTags_Info_PnPLObj = Tags_Info_PnPLAlloc();
  pAcquisition_Info_PnPLObj = Acquisition_Info_PnPLAlloc();
  pFirmware_Info_PnPLObj = Firmware_Info_PnPLAlloc();
  pDeviceinformation_PnPLObj = Deviceinformation_PnPLAlloc();
  pAI_Motor_Classifier_PnPLObj = Ai_Motor_Classifier_PnPLAlloc();

  PnPLSetBOARDID(BOARD_ID);
  PnPLSetFWID(FW_ID);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* Re-map the state machine of the Integrated tasks */

  //SensorManagerStateMachineRemap(spAppPMState2SMPMStateMap);

  /* Connect the sensor task to the bus. */
  if(sI2C3BusObj)
  {
    /* Use I2C3 for External STTS22H */
    I2CBusTaskConnectDevice((I2CBusTask *) sI2C3BusObj, (I2CBusIF *)STTS22HTaskGetSensorIF((STTS22HTask *) sSTTS22HObj));
  }
  else
  {
    /* Use I2C2 for Internal STTS22H */
    I2CBusTaskConnectDevice((I2CBusTask *) sI2C2BusObj, (I2CBusIF *)STTS22HTaskGetSensorIF((STTS22HTask *) sSTTS22HObj));
  }

  if(sIIS3DWBObj)
  {
    SPIBusTaskConnectDevice((SPIBusTask *) sSPI2BusObj, (SPIBusIF *)IIS3DWBTaskGetSensorIF((IIS3DWBTask *) sIIS3DWBObj));
  }

  /* Connect the Sensor events */
  IEventListener *DatalogAppListener = DatalogAppTask_GetEventListenerIF((DatalogAppTask *) sDatalogAppObj);

  /* Connect MCP events to DatalogApp Task */
  IEventListener *DatalogActuatorListener = DatalogAppTask_GetActuatorEventListenerIF((DatalogAppTask *) sDatalogAppObj);
  IEventSrcAddEventListener(MCPTask_GetEventSrcIF((MCPTask_t *) sMCPObj), DatalogActuatorListener);

  /* Get Data Aggregation and process Event listener interface */
  IEventListener *DAPSensorEvtLstener = DataAggregationProcessTask_GetSensorEventListenerIF((DataAggregationProcessTask_t*) sDataAggregationProcessTaskObj);
  IEventListener *DAPActuatorEvtLstener = DataAggregationProcessTask_GetActuatorEventListenerIF((DataAggregationProcessTask_t*) sDataAggregationProcessTaskObj);

  /* Connect Data Aggregation and process Event listener interface to data sources */
  IEventSrcAddEventListener(MCPTask_GetEventSrcIF((MCPTask_t *) sMCPObj), DAPActuatorEvtLstener);

  if(sIIS3DWBObj)
  {
    IEventSrcAddEventListener(IIS3DWBTaskGetEventSrcIF((IIS3DWBTask *) sIIS3DWBObj), DatalogAppListener);
    IEventSrcAddEventListener(IIS3DWBTaskGetEventSrcIF((IIS3DWBTask *) sIIS3DWBObj), DAPSensorEvtLstener);
  }

  IEventSrcAddEventListener(STTS22HTaskGetTempEventSrcIF((STTS22HTask *) sSTTS22HObj), DatalogAppListener);

  /* Connect NEAIDPU to DatalogAppListener*/
  IEventListener *p_classifierEvtListener = DatalogAppTask_GetAIClassifierEventListenerIF((DatalogAppTask*) sDatalogAppObj);
  IDPU2_t *p_classifierDPU = DataAggregationProcessTask_GetClassifierDPU((DataAggregationProcessTask_t*) sDataAggregationProcessTaskObj);
  IEventSrc *p_evt_src;
  p_evt_src = ADPU2_GetEventSrcIF((ADPU2_t*) p_classifierDPU);
  IEventSrcAddEventListener(p_evt_src, p_classifierEvtListener);

  /* Init&Add PnPL Components */
  Slow_Mc_Telemetries_PnPLInit(pSlow_Mc_Telemetries_PnPLObj);
  Fast_Mc_Telemetries_PnPLInit(pFast_Mc_Telemetries_PnPLObj);
  Iis3dwb_Acc_PnPLInit(pIis3dwb_Acc_PnPLObj);
  Stts22h_Temp_PnPLInit(pStts22h_Temp_PnPLObj);
  Log_Controller_PnPLInit(pLog_Controller_PnPLObj);
  Motor_Controller_PnPLInit(pMotor_Controller_PnPLObj);
  Tags_Info_PnPLInit(pTags_Info_PnPLObj);
  Acquisition_Info_PnPLInit(pAcquisition_Info_PnPLObj);
  Firmware_Info_PnPLInit(pFirmware_Info_PnPLObj);
  Deviceinformation_PnPLInit(pDeviceinformation_PnPLObj);
  Ai_Motor_Classifier_PnPLInit(pAI_Motor_Classifier_PnPLObj);

  return SYS_NO_ERROR_CODE;
}

IAppPowerModeHelper *SysGetPowerModeHelper(void)
{
  // Install the application power mode helper.
  static IAppPowerModeHelper *s_pxPowerModeHelper = NULL;
  if (s_pxPowerModeHelper == NULL)
  {
    s_pxPowerModeHelper = AppPowerModeHelperAlloc();
  }

  return s_pxPowerModeHelper;
}

/* Private function definition */
/*******************************/

void EXT_INT1_EXTI_Callback(uint16_t nPin)
{
  if (sIIS3DWBObj)
  {
    IIS3DWBTask_EXTI_Callback(nPin);
  }
}
