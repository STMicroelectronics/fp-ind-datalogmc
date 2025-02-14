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

#include "PnPLCompManager.h"
#include "Slow_Mc_Telemetries_PnPL.h"
#include "Fast_Mc_Telemetries_PnPL.h"
#include "Iis3dwb_Acc_PnPL.h"
#include "Ilps22qs_Press_PnPL.h"
#include "Imp23absu_Mic_PnPL.h"
#include "Ism330dhcx_Acc_PnPL.h"
#include "Ism330dhcx_Gyro_PnPL.h"
#include "Stts22h_Temp_PnPL.h"
#include "Log_Controller_PnPL.h"
#include "Motor_Controller_PnPL.h"
#include "Tags_Info_PnPL.h"
#include "Acquisition_Info_PnPL.h"
#include "Firmware_Info_PnPL.h"
#include "Deviceinformation_PnPL.h"

#include "parson.h"

static uint8_t BOARD_ID = BOARD_ID_BOXA;
static uint8_t FW_ID = USB_FW_ID_DATALOG2_BOXA;

static IPnPLComponent_t *pSlow_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pFast_Mc_Telemetries_PnPLObj = NULL;
static IPnPLComponent_t *pIis3dwb_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pIlps22qs_Press_PnPLObj = NULL;
static IPnPLComponent_t *pImp23absu_Mic_PnPLObj = NULL;
static IPnPLComponent_t *pIsm330dhcx_Acc_PnPLObj = NULL;
static IPnPLComponent_t *pIsm330dhcx_Gyro_PnPLObj = NULL;
//static IPnPLComponent_t *pIsm330dhcx_Mlc_PnPLObj = NULL;
static IPnPLComponent_t *pStts22h_Temp_PnPLObj = NULL;
static IPnPLComponent_t *pLog_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pMotor_Controller_PnPLObj = NULL;
static IPnPLComponent_t *pTags_Info_PnPLObj = NULL;
static IPnPLComponent_t *pAcquisition_Info_PnPLObj = NULL;
static IPnPLComponent_t *pFirmware_Info_PnPLObj = NULL;
static IPnPLComponent_t *pDeviceinformation_PnPLObj = NULL;

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
static AManagedTaskEx *sILPS22QSObj = NULL;
static AManagedTaskEx *sIMP23ABSUObj = NULL;
static AManagedTaskEx *sISM330DHCXObj = NULL;
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
  hwd_st25dv_version st25dv_version;

  PnPLSetAllocationFunctions(SysAlloc, SysFree);

  /* Check the HW configuration to load the tasks accordingly */
  sExtIis3dwb = HardwareDetection_Check_Ext_IIS3DWB();
  sExtStts22h = HardwareDetection_Check_Ext_STTS22H(&stts22h_address);

  /* Check NFC chip version */
  st25dv_version = HardwareDetection_Check_ST25DV();
  if (st25dv_version == ST25DV64KC)
  {
    BOARD_ID = BOARD_ID_BOXB;
    FW_ID = USB_FW_ID_DATALOG2_BOXB;
  }

  /* Allocate the task objects */
  sUtilObj = UtilTaskAlloc(&MX_GPIO_LED1InitParams, &MX_GPIO_LED2InitParams);
  sDatalogAppObj = DatalogAppTaskAlloc();

  sMCPObj = MCPTask_Alloc();

  sI2C2BusObj = I2CBusTaskAlloc(&MX_I2C2InitParams);
  sSPI2BusObj = SPIBusTaskAlloc(&MX_SPI2InitParams);

  if (sExtIis3dwb)
  {
    /* Use the external IIS3DWB */
    sIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_INT1_EXTERNAL_DWBInitParams, &MX_GPIO_CS_EXTERNALInitParams);
  }
  else
  {
    /* Use the onboard IIS3DWB */
    sIIS3DWBObj = IIS3DWBTaskAlloc(&MX_GPIO_INT1_DWBInitParams, &MX_GPIO_CS_DWBInitParams);
  }

  if (sExtStts22h)
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

  sILPS22QSObj = ILPS22QSTaskAlloc(NULL, NULL);
  sIMP23ABSUObj = IMP23ABSUTaskAlloc(&MX_MDF1InitParams, &MX_ADC1InitParams);
  sISM330DHCXObj = ISM330DHCXTaskAlloc(&MX_GPIO_INT1_DHCXInitParams, &MX_GPIO_INT2_DHCXInitParams, &MX_GPIO_CS_DHCXInitParams);

  /* Add the task object to the context. */
  res = ACAddTask(pAppContext, (AManagedTask *) sMCPObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sUtilObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sI2C2BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sI2C3BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sSPI2BusObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sIIS3DWBObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sILPS22QSObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sIMP23ABSUObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sISM330DHCXObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sSTTS22HObj);
  res = ACAddTask(pAppContext, (AManagedTask *) sDatalogAppObj);


  /* PnPL Components Allocation */
  pSlow_Mc_Telemetries_PnPLObj = Slow_Mc_Telemetries_PnPLAlloc();
  pFast_Mc_Telemetries_PnPLObj = Fast_Mc_Telemetries_PnPLAlloc();
  pIis3dwb_Acc_PnPLObj = Iis3dwb_Acc_PnPLAlloc();
  pIlps22qs_Press_PnPLObj = Ilps22qs_Press_PnPLAlloc();
  pImp23absu_Mic_PnPLObj = Imp23absu_Mic_PnPLAlloc();
  pIsm330dhcx_Acc_PnPLObj = Ism330dhcx_Acc_PnPLAlloc();
  pIsm330dhcx_Gyro_PnPLObj = Ism330dhcx_Gyro_PnPLAlloc();
  pStts22h_Temp_PnPLObj = Stts22h_Temp_PnPLAlloc();
  pLog_Controller_PnPLObj = Log_Controller_PnPLAlloc();
  pMotor_Controller_PnPLObj = Motor_Controller_PnPLAlloc();
  pTags_Info_PnPLObj = Tags_Info_PnPLAlloc();
  pAcquisition_Info_PnPLObj = Acquisition_Info_PnPLAlloc();
  pFirmware_Info_PnPLObj = Firmware_Info_PnPLAlloc();
  pDeviceinformation_PnPLObj = Deviceinformation_PnPLAlloc();

  PnPLSetBOARDID(BOARD_ID);
  PnPLSetFWID(FW_ID);

  return res;
}

sys_error_code_t SysOnStartApplication(ApplicationContext *pAppContext)
{
  UNUSED(pAppContext);

  /* Connect the sensor task to the bus. */
  if (sI2C3BusObj)
  {
    /* Use I2C3 for External STTS22H */
    I2CBusTaskConnectDevice((I2CBusTask *) sI2C3BusObj, (I2CBusIF *)STTS22HTaskGetSensorIF((STTS22HTask *) sSTTS22HObj));
  }
  else
  {
    /* Use I2C2 for Internal STTS22H */
    I2CBusTaskConnectDevice((I2CBusTask *) sI2C2BusObj, (I2CBusIF *)STTS22HTaskGetSensorIF((STTS22HTask *) sSTTS22HObj));
  }

  if (sIIS3DWBObj)
  {
    SPIBusTaskConnectDevice((SPIBusTask *) sSPI2BusObj, (SPIBusIF *)IIS3DWBTaskGetSensorIF((IIS3DWBTask *) sIIS3DWBObj));
  }

  I2CBusTaskConnectDevice((I2CBusTask *) sI2C2BusObj, (I2CBusIF *)ILPS22QSTaskGetSensorIF((ILPS22QSTask *) sILPS22QSObj));
  SPIBusTaskConnectDevice((SPIBusTask *) sSPI2BusObj,
                          (SPIBusIF *)ISM330DHCXTaskGetSensorIF((ISM330DHCXTask *) sISM330DHCXObj));

  /* Connect the Sensor events */
  IEventListener *DatalogAppListener = DatalogAppTask_GetEventListenerIF((DatalogAppTask *) sDatalogAppObj);
  IEventSrcAddEventListener(ILPS22QSTaskGetPressEventSrcIF((ILPS22QSTask *) sILPS22QSObj), DatalogAppListener);
  IEventSrcAddEventListener(IMP23ABSUTaskGetEventSrcIF((IMP23ABSUTask *) sIMP23ABSUObj), DatalogAppListener);
  IEventSrcAddEventListener(ISM330DHCXTaskGetAccEventSrcIF((ISM330DHCXTask *) sISM330DHCXObj), DatalogAppListener);
  IEventSrcAddEventListener(ISM330DHCXTaskGetGyroEventSrcIF((ISM330DHCXTask *) sISM330DHCXObj), DatalogAppListener);
  IEventSrcAddEventListener(STTS22HTaskGetTempEventSrcIF((STTS22HTask *) sSTTS22HObj), DatalogAppListener);

  /* Connect MCP events to DatalogApp Task */
  IEventListener *DatalogActuatorListener = DatalogAppTask_GetActuatorEventListenerIF((DatalogAppTask *) sDatalogAppObj);
  IEventSrcAddEventListener(MCPTask_GetSlowTelemetriesEventSrcIF((MCPTask_t *) sMCPObj), DatalogActuatorListener);
  IEventSrcAddEventListener(MCPTask_GetAsyncTelemetriesEventSrcIF((MCPTask_t *) sMCPObj), DatalogActuatorListener);

  if (sIIS3DWBObj)
  {
    IEventSrcAddEventListener(IIS3DWBTaskGetEventSrcIF((IIS3DWBTask *) sIIS3DWBObj), DatalogAppListener);
  }

  /* Init&Add PnPL Components */
  Slow_Mc_Telemetries_PnPLInit(pSlow_Mc_Telemetries_PnPLObj);
  Fast_Mc_Telemetries_PnPLInit(pFast_Mc_Telemetries_PnPLObj);
  Iis3dwb_Acc_PnPLInit(pIis3dwb_Acc_PnPLObj);
  Ilps22qs_Press_PnPLInit(pIlps22qs_Press_PnPLObj);
  Imp23absu_Mic_PnPLInit(pImp23absu_Mic_PnPLObj);
  Ism330dhcx_Acc_PnPLInit(pIsm330dhcx_Acc_PnPLObj);
  Ism330dhcx_Gyro_PnPLInit(pIsm330dhcx_Gyro_PnPLObj);
  Stts22h_Temp_PnPLInit(pStts22h_Temp_PnPLObj);
  Log_Controller_PnPLInit(pLog_Controller_PnPLObj);
  Motor_Controller_PnPLInit(pMotor_Controller_PnPLObj);
  Tags_Info_PnPLInit(pTags_Info_PnPLObj);
  Acquisition_Info_PnPLInit(pAcquisition_Info_PnPLObj);
  Firmware_Info_PnPLInit(pFirmware_Info_PnPLObj);
  Deviceinformation_PnPLInit(pDeviceinformation_PnPLObj);

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

  else

  {
    //ISM330ISTask_EXTI_Callback(nPin);
  }

}
