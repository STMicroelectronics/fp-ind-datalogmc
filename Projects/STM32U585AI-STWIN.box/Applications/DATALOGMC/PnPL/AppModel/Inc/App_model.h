/**
  ******************************************************************************
  * @file    App_model.h
  * @author  SRA
  * @brief   App Application Model and PnPL Components APIs
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
  ****************************************************************\**************
  */

/**
  ******************************************************************************
  * This file has been auto generated from the following Device Template Model:
  * dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC_AI;1
  *
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

/**
  ******************************************************************************
  * Component APIs *************************************************************
  * - Component init function
  *    <comp_name>_comp_init(void)
  * - Component get_key function
  *    <comp_name>_get_key(void)
  * - Component GET/SET Properties APIs ****************************************
  *  - GET Functions
  *    uint8_t <comp_name>_get_<prop_name>(prop_type *value)
  *      if prop_type == char --> (char **value)
  *  - SET Functions
  *    uint8_t <comp_name>_set_<prop_name>(prop_type value)
  *      if prop_type == char --> (char *value)
  *  - Component COMMAND Reaction Functions
  *      uint8_t <comp_name>_<command_name>(
  *                     field1_type field1_name, field2_type field2_name, ...,
  *                     fieldN_type fieldN_name);
  *  - Component TELEMETRY Send Functions
  *      uint8_t <comp_name>_create_telemetry(tel1_type tel1_name,
  *                     tel2_type tel2_name, ..., telN_type telN_name,
  *                     char **telemetry, uint32_t *size)
  ******************************************************************************
  */

#ifndef APP_MODEL_H_
#define APP_MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "PnPLCompManager.h"
#include "SensorManager.h"
#include "parson.h"
#include "App_model_Slow_Mc_Telemetries.h"
#include "App_model_Fast_Mc_Telemetries.h"
#include "App_model_Iis3dwb_Acc.h"
#include "App_model_Ilps22qs_Press.h"
#include "App_model_Imp23absu_Mic.h"
#include "App_model_Ism330dhcx_Acc.h"
#include "App_model_Ism330dhcx_Gyro.h"
#include "App_model_Stts22h_Temp.h"
#include "App_model_Log_Controller.h"
#include "App_model_Motor_Controller.h"
#include "App_model_Tags_Info.h"
#include "App_model_Acquisition_Info.h"
#include "App_model_Firmware_Info.h"
#include "App_model_Deviceinformation.h"

/* USER includes -------------------------------------------------------------*/
#include "services/sysdebug.h"

/* USER definition -----------------------------------------------------------*/
#define COMP_TYPE_SENSOR          0x00
#define COMP_TYPE_ALGORITHM       0x01
#define COMP_TYPE_OTHER           0x02
#define COMP_TYPE_ACTUATOR        0x03

#define LOG_CTRL_MODE_SD          0x00
#define LOG_CTRL_MODE_USB         0x01
#define LOG_CTRL_MODE_BLE         0x02

#define OTHER_COMP_NUMBER         6
#define ACTUATOR_NUMBER           2
#define SENSOR_NUMBER             7
#define ST_BLE_STREAM_OBJECT_NUM  5

#define STREAM_TIMESTAMP_DIM  8u

#define N_MAX_EP 5
#define FAST_TELEMETRY_APP_MODEL_STREAM_ID    3u

#define FW_VERSION_MAJOR    "2"
#define FW_VERSION_MINOR    "1"
#define FW_VERSION_PATCH    "0"

typedef struct _StreamParams_t
{
  int8_t stream_id;
  int8_t usb_ep;
  uint16_t spts;
  uint32_t sd_dps;
  uint32_t usb_dps;
  uint32_t ble_dps;
  uint32_t serial_dps;
  float_t ioffset;
  float_t bandwidth;
  /* Stream Parameters Model USER code */
} StreamParams_t;

#define SENSOR_NOTES_LEN 20U
typedef struct _SensorModel_t
{
  /* E.g. IIS3DWB Component is a sensor (look @ schema field)
  so, its model has the following structure */
  uint8_t id;
  char *comp_name;
  StreamParams_t stream_params;
  SensorStatus_t *sensor_status;
  char annotation[SENSOR_NOTES_LEN];
  /* Sensor Components Model USER code */
} SensorModel_t;

typedef struct _AlgorithmModel_t
{
  /* E.g. FFT_Algo Component is an algorithm (look @ schema field)
     so, its model has the following structure */
  uint8_t id;
  char *comp_name;
  bool enable;
  StreamParams_t stream_params;
  /* Algorithm Components Model USER code */
} AlgorithmModel_t;

typedef struct _ActuatorStatus_t
{
  /* Specifies if the actuator is active or not. */
  boolean_t is_active;
} ActuatorStatus_t;

typedef struct _ActuatorModel_t
{
  /* E.g. Motor Telemetries Component is an actuator (look @ schema field)
  so, its model has the following structure */
  uint8_t id;
  char *comp_name;
  StreamParams_t stream_params;
  ActuatorStatus_t actuatorStatus;
  /* Actuator Components Model USER code */
  uint8_t active_telemetry_cnt;
} ActuatorModel_t;

#define UNIT_LEN 20U
#define FORMAT_LEN 10U
typedef struct _StBleStreamObjectModel_t
{
  /* Component that describe the data format to be sent via BLE */
  bool status;
  char format[FORMAT_LEN];
  /* ST BLE Stream property Model USER code */
  char unit[UNIT_LEN];
} StBleStreamObjectModel_t;

typedef struct _StBleStreamModel_t
{
  /* Component that enable the data stream via BLE */
  int32_t st_ble_stream_id;
  StBleStreamObjectModel_t st_ble_stream_objects[ST_BLE_STREAM_OBJECT_NUM];
} StBleStreamModel_t;

typedef struct _AppModel_t
{
  SensorModel_t *s_models[SENSOR_NUMBER];
  ActuatorModel_t *ac_models[ACTUATOR_NUMBER];
  StBleStreamModel_t *slow_mc_telemetries_ble_stream_model;
  LogControllerModel_t log_controller_model;
  MotorControllerModel_t motor_controller_model;
  TagsInfoModel_t tags_info_model;
  AcquisitionInfoModel_t acquisition_info_model;
  FirmwareInfoModel_t firmware_info_model;
  /* Insert here your custom App Model code */
  bool mlc_ucf_valid;
} AppModel_t;

uint8_t __stream_control(bool status);

AppModel_t *getAppModel(void);
uint8_t getNextActuatorId(uint16_t *id);

uint8_t addSensorToAppModel(uint16_t id, SensorModel_t *model);

uint8_t addAlgorithmToAppModel(uint16_t id, AlgorithmModel_t *model);

uint8_t addActuatorToAppModel(uint16_t id, ActuatorModel_t *model);

uint8_t set_mac_address(const char *value);

#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_H_ */
