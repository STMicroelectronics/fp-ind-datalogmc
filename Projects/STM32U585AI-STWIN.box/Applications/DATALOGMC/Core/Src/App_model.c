/**
  ******************************************************************************
  * @file    App_Model.c
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
  ******************************************************************************
  */

/**
  ******************************************************************************
  * This file has been auto generated from the following Device Template Model:
  * dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC;1
  *
  * Created by: DTDL2PnPL_cGen version 1.2.2
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"
#include <string.h>
#include <stdio.h>
#include "services/SQuery.h"

/* USER includes -------------------------------------------------------------*/
#include "services/sysdebug.h"
#include "STWIN.box_sd.h"
#include "fx_stm32_sd_driver.h"
#include "rtc.h"
#include "MCPTask.h"
#include "Acquisition_Info_PnPL.h"

/* USER private function prototypes ------------------------------------------*/
static uint8_t __stream_control(ILog_Controller_t *ifn, bool status);
static inline void fast_mc_telemetries_configure_usb_dps(void);
static inline void fast_mc_telemetries_configure_sd_dps(void);
static void update_fast_telemetry_state(MCPTaskFastTelemetry_idx_t fast_telemetry_idx, bool value);
static void update_slow_telemetry_state(MCPTaskSlowTelemetry_idx_t slow_telemetry_idx, bool value);

/* USER defines --------------------------------------------------------------*/
#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_DT, level, message)
#define STREAM_TIMESTAMP_DIM  (8u)

/* Fast telemetry definition */
#define FAST_TELEMETRY_APP_MODEL_IDX          1u
#define FAST_TELEMETRY_APP_MODEL_USB_EP       3u
#define FAST_TELEMETRY_APP_MODEL_STREAM_ID    3u
#define FAST_TELEMETRY_APP_MODEL_DIMENSION    (FAST_TELEMETRY_APP_MODEL_USB_DPS / 2)
#define FAST_TELEMETRY_APP_MODEL_SPTS         1
#define FAST_TELEMETRY_ONE_ENABLED            1
#define FAST_TELEMETRY_ONE_ENABLED_USB_DPS    2040
#define FAST_TELEMETRY_TWO_ENABLED            2
#define FAST_TELEMETRY_TWO_ENABLED_USB_DPS    2036
#define FAST_TELEMETRY_THREE_ENABLED          3
#define FAST_TELEMETRY_THREE_ENABLED_USB_DPS  2034
#define FAST_TELEMETRY_FOUR_ENABLED           4
#define FAST_TELEMETRY_FOUR_ENABLED_USB_DPS   2032
#define FAST_TELEMETRY_ODR                    20000U
#define FAST_TELEMETRY_SAMPLE_SIZE            2U
#define FAST_TELEMETRY_MAX_ENABLED            4

/* Slow telemetry definition */
#define SLOW_TELEMETRY_APP_MODEL_IDX          0u
#define SLOW_TELEMETRY_APP_MODEL_USB_EP       4u
#define SLOW_TELEMETRY_APP_MODEL_STREAM_ID    4u
#define SLOW_TELEMETRY_ODR 				            3U
#define SLOW_TELEMETRY_SAMPLE_SIZE 		        4U
#define SLOW_TELEMETRY_MAX_NUM	 		          4U


/* App Model Definition */
AppModel_t app_model;

AppModel_t* getAppModel(void)
{
  return &app_model;
}

/* Device Components APIs ----------------------------------------------------*/

/* Slow Motor Telemetries PnPL Component -------------------------------------*/
static ActuatorModel_t slow_mc_telemetries_model;
static StBleStreamModel_t slow_mc_telemetries_ble_stream_model;

uint8_t slow_mc_telemetries_comp_init(void)
{
  slow_mc_telemetries_model.comp_name = slow_mc_telemetries_get_key();
  slow_mc_telemetries_model.actuatorStatus.is_active = true;
  slow_mc_telemetries_model.stream_params.stream_id = SLOW_TELEMETRY_APP_MODEL_STREAM_ID;
  slow_mc_telemetries_model.stream_params.usb_dps = STREAM_TIMESTAMP_DIM;
  slow_mc_telemetries_model.stream_params.usb_ep = SLOW_TELEMETRY_APP_MODEL_USB_EP;
  slow_mc_telemetries_model.stream_params.spts = 1;
  slow_mc_telemetries_model.id = 0;

  slow_mc_telemetries_set_st_ble_stream__id(1);

  slow_mc_telemetries_set_st_ble_stream__temperature_unit("Celsius");
  slow_mc_telemetries_set_st_ble_stream__temperature_format("int32_t");

  slow_mc_telemetries_set_st_ble_stream__ref_speed_unit("rpm");
  slow_mc_telemetries_set_st_ble_stream__ref_speed_format("int32_t");

  slow_mc_telemetries_set_st_ble_stream__speed_unit("rpm");
  slow_mc_telemetries_set_st_ble_stream__speed_format("int32_t");

  slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit("V");
  slow_mc_telemetries_set_st_ble_stream__bus_voltage_format("int32_t");

  slow_mc_telemetries_set_st_ble_stream__fault_unit("");
  slow_mc_telemetries_set_st_ble_stream__fault_format("int32_t");

  /* Slow telemetry state */
  slow_mc_telemetries_set_st_ble_stream__temperature_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
  slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
  slow_mc_telemetries_set_st_ble_stream__speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
  slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
  slow_mc_telemetries_set_st_ble_stream__fault_enable(slow_mc_telemetries_model.actuatorStatus.is_active);

  //  /* Worst case 4byte for maximum 4 slow telemetries each 300ms */
  slow_mc_telemetries_model.stream_params.bandwidth = SLOW_TELEMETRY_ODR * SLOW_TELEMETRY_SAMPLE_SIZE * SLOW_TELEMETRY_MAX_NUM;
  slow_mc_telemetries_model.stream_params.sd_dps = SLOW_TELEMETRY_ODR * SLOW_TELEMETRY_SAMPLE_SIZE * SLOW_TELEMETRY_MAX_NUM;

  app_model.ac_models[SLOW_TELEMETRY_APP_MODEL_IDX] = &slow_mc_telemetries_model;
  app_model.slow_mc_telemetries_ble_stream_model = &slow_mc_telemetries_ble_stream_model;

  return 0;
}
char* slow_mc_telemetries_get_key(void)
{
  return "slow_mc_telemetries";
}
uint8_t slow_mc_telemetries_get_usb_dps(int32_t *value)
{
  *value = slow_mc_telemetries_model.stream_params.usb_dps;
  return 0;
}
uint8_t slow_mc_telemetries_get_sd_dps(int32_t *value)
{
  *value = slow_mc_telemetries_model.stream_params.sd_dps;
  return 0;
}
uint8_t slow_mc_telemetries_get_enable(bool *value)
{
  *value = slow_mc_telemetries_model.actuatorStatus.is_active;
  return 0;
}
uint8_t slow_mc_telemetries_get_data_type(char **value)
{
  *value = "int32";
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__id(int32_t *value)
{
  /* USER Code */
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_id;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_enable(bool *value)
{
  *value = MCPTask_GetSlowTelemetryState(MC_REG_HEATS_TEMP_IDX);
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_unit(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].unit;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__temperature_format(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].format;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_enable(bool *value)
{
  *value = MCPTask_GetSlowTelemetryState(MC_REG_SPEED_REF_IDX);
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_unit(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].unit;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__ref_speed_format(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].format;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_enable(bool *value)
{
  *value = MCPTask_GetSlowTelemetryState(MC_REG_SPEED_MEAS_IDX);
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_unit(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].unit;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_format(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].format;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_enable(bool *value)
{
  *value =  MCPTask_GetSlowTelemetryState(MC_REG_BUS_VOLTAGE_IDX);
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_unit(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].unit;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_format(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].format;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_enable(bool *value)
{
  *value = MCPTask_GetSlowTelemetryState(MC_REG_FAULTS_FLAGS_IDX);
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_unit(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].unit;
  return 0;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__fault_format(char **value)
{
  *value = slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].format;
  return 0;
}
uint8_t slow_mc_telemetries_get_stream_id(int8_t *value)
{
  *value = slow_mc_telemetries_model.stream_params.stream_id;
  return 0;
}
uint8_t slow_mc_telemetries_get_ep_id(int8_t *value)
{
  *value = slow_mc_telemetries_model.stream_params.usb_ep;
  return 0;
}
uint8_t slow_mc_telemetries_set_enable(bool value)
{
  slow_mc_telemetries_model.actuatorStatus.is_active = value;

  if(!value)
  {
    slow_mc_telemetries_set_st_ble_stream__temperature_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
    slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
    slow_mc_telemetries_set_st_ble_stream__speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
    slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
    slow_mc_telemetries_set_st_ble_stream__fault_enable(slow_mc_telemetries_model.actuatorStatus.is_active);
  }
  else
  {
    /* Get num of enabled fast telemetries */
    if(0 == MCPTask_GetNumOfEnabledSlowTelemetry())
    {
      /* Enable all telemetries in the list */
      slow_mc_telemetries_set_st_ble_stream__temperature_enable(true);
      slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(true);
      slow_mc_telemetries_set_st_ble_stream__speed_enable(true);
      slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(true);
      slow_mc_telemetries_set_st_ble_stream__fault_enable(true);
    }
  }

  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__id(int32_t value)
{
  /* USER Code */
  slow_mc_telemetries_ble_stream_model.st_ble_stream_id = value;
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_enable(bool value)
{
  update_slow_telemetry_state(MC_REG_HEATS_TEMP_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_unit(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_format(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(bool value)
{
  update_slow_telemetry_state(MC_REG_SPEED_REF_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_unit(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_format(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_enable(bool value)
{
  update_slow_telemetry_state(MC_REG_SPEED_MEAS_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_unit(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_format(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(bool value)
{
  update_slow_telemetry_state(MC_REG_BUS_VOLTAGE_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_format(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_enable(bool value)
{
  update_slow_telemetry_state(MC_REG_FAULTS_FLAGS_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_unit(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_format(const char *value)
{
  /* USER Code */
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].format, value);
  return 0;
}

/* Fast Motor Telemetries  PnPL Component ------------------------------------*/
static ActuatorModel_t fast_mc_telemetries_model;

uint8_t fast_mc_telemetries_comp_init(void)
{
  fast_mc_telemetries_model.comp_name               = fast_mc_telemetries_get_key();
  fast_mc_telemetries_model.stream_params.usb_dps    = STREAM_TIMESTAMP_DIM;
  fast_mc_telemetries_model.stream_params.spts       = FAST_TELEMETRY_APP_MODEL_SPTS;
  fast_mc_telemetries_model.stream_params.stream_id  = FAST_TELEMETRY_APP_MODEL_STREAM_ID;
  fast_mc_telemetries_model.stream_params.usb_ep     = FAST_TELEMETRY_APP_MODEL_USB_EP;
  fast_mc_telemetries_model.actuatorStatus.is_active = true;

  /* USER Component initialization code */

  /* Enable fast telemetries*/
  fast_mc_telemetries_set_i_q__enabled(true);
  fast_mc_telemetries_set_i_d__enabled(true);
  fast_mc_telemetries_set_i_q_ref__enabled(true);
  fast_mc_telemetries_set_i_d_ref__enabled(true);

  app_model.ac_models[FAST_TELEMETRY_APP_MODEL_IDX] = &fast_mc_telemetries_model;
  return 0;
}
char* fast_mc_telemetries_get_key(void)
{
  return "fast_mc_telemetries";
}

uint8_t fast_mc_telemetries_get_max_num_telemetries(int32_t *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_usb_dps(int32_t *value)
{
  *value = fast_mc_telemetries_model.stream_params.usb_dps;
  return 0;
}
uint8_t fast_mc_telemetries_get_sd_dps(int32_t *value)
{
  *value = fast_mc_telemetries_model.stream_params.sd_dps;
  return 0;
}
uint8_t fast_mc_telemetries_get_enable(bool *value)
{
  *value = fast_mc_telemetries_model.actuatorStatus.is_active;
  return 0;
}
uint8_t fast_mc_telemetries_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}
uint8_t fast_mc_telemetries_get_dim(int32_t *value)
{
  /* Each fast telemetry is a uint16_t, in total we have
   * @fast_mc_telemetries_model.stream_params.usb_dps/2
   * sample in the buffers
   **/
  *value = (fast_mc_telemetries_model.stream_params.usb_dps-STREAM_TIMESTAMP_DIM )/2;
  return 0;
}
uint8_t fast_mc_telemetries_get_i_q__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_Q_MEAS_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_q__unit(char **value)
{
  *value = "";
  return 0;
}
uint8_t fast_mc_telemetries_get_i_d__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_D_MEAS_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_d__unit(char **value)
{
  *value = "";
  return 0;
}
uint8_t fast_mc_telemetries_get_i_q_ref__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_Q_REF_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_q_ref__unit(char **value)
{
  *value = "";
  return 0;
}
uint8_t fast_mc_telemetries_get_i_d_ref__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_D_REF_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_d_ref__unit(char **value)
{
  *value = "";
  return 0;
}
uint8_t fast_mc_telemetries_get_v_q__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_V_Q_IDX);
  return 0;
}

uint8_t fast_mc_telemetries_get_v_q__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_v_d__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_V_D_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_v_d__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_i_a__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_A_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_a__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_i_b__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_I_B_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_i_b__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_v_a__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_V_A_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_v_a__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_v_b__enabled(bool *value)
{
  *value = MCPTask_GetFastTelemetryState(MC_REG_V_B_IDX);
  return 0;
}
uint8_t fast_mc_telemetries_get_v_b__unit(char **value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_get_stream_id(int8_t *value)
{
  *value = fast_mc_telemetries_model.stream_params.stream_id;
  return 0;
}
uint8_t fast_mc_telemetries_get_ep_id(int8_t *value)
{
  *value = fast_mc_telemetries_model.stream_params.usb_ep;
  return 0;
}
uint8_t fast_mc_telemetries_set_enable(bool value)
{
  fast_mc_telemetries_model.actuatorStatus.is_active = value;
  if(!value)
  {
    fast_mc_telemetries_set_i_d_ref__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_i_q_ref__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_i_d__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_i_q__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_v_q__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_v_d__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_i_a__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_i_b__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_v_a__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
    fast_mc_telemetries_set_v_b__enabled(fast_mc_telemetries_model.actuatorStatus.is_active);
  }
  else
  {
    /* Get num of enabled fast telemetries */
    if(0 == MCPTask_GetNumOfEnabledFastTelemetry())
    {
      /* Enable the first one in the list */
      fast_mc_telemetries_set_i_q__enabled(true);
    }
  }

  return 0;
}
uint8_t fast_mc_telemetries_set_i_q__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_Q_MEAS_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_D_MEAS_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q_ref__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_Q_REF_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q_ref__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d_ref__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_D_REF_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d_ref__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_v_q__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_V_Q_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_q__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_v_d__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_V_D_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_d__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_i_a__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_A_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_a__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_i_b__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_I_B_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_b__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_v_a__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_V_A_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_a__unit(const char *value)
{
  /* USER Code */
  return 0;
}
uint8_t fast_mc_telemetries_set_v_b__enabled(bool value)
{
  update_fast_telemetry_state(MC_REG_V_B_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_b__unit(const char *value)
{
  /* USER Code */
  return 0;
}

//USER CODE:
static inline void fast_mc_telemetries_configure_usb_dps(void)
{
  /* Get number of enabled fast telemetries */
  uint32_t num_of_enabled_tf = MCPTask_GetNumOfEnabledFastTelemetry();

  fast_mc_telemetries_model.stream_params.usb_dps = STREAM_TIMESTAMP_DIM;

  switch(num_of_enabled_tf)
  {
    case FAST_TELEMETRY_ONE_ENABLED:
      {
        fast_mc_telemetries_model.stream_params.usb_dps += FAST_TELEMETRY_ONE_ENABLED_USB_DPS;
      }
      break;

    case FAST_TELEMETRY_TWO_ENABLED:
      {
        fast_mc_telemetries_model.stream_params.usb_dps += FAST_TELEMETRY_TWO_ENABLED_USB_DPS;
      }
      break;

    case FAST_TELEMETRY_THREE_ENABLED:
      {
        fast_mc_telemetries_model.stream_params.usb_dps += FAST_TELEMETRY_THREE_ENABLED_USB_DPS;
      }
      break;

    case FAST_TELEMETRY_FOUR_ENABLED:
      {
        fast_mc_telemetries_model.stream_params.usb_dps += FAST_TELEMETRY_FOUR_ENABLED_USB_DPS;
      }
  }
}

static inline void fast_mc_telemetries_configure_sd_dps(void)
{
  /* Get number of enabled fast telemetries */
  uint32_t num_of_enabled_tf = MCPTask_GetNumOfEnabledFastTelemetry();
//  uint32_t mc_fast_telemetries_odr = FAST_TELEMETRY_ODR * num_of_enabled_tf;

  fast_mc_telemetries_model.stream_params.sd_dps = STREAM_TIMESTAMP_DIM;

  fast_mc_telemetries_model.stream_params.bandwidth = (FAST_TELEMETRY_ODR * FAST_TELEMETRY_SAMPLE_SIZE) * num_of_enabled_tf;

  /* 330ms of sensor data. Access to SD is optimized when buffer dimension is multiple of 512 */
  fast_mc_telemetries_model.stream_params.sd_dps = (uint32_t)(fast_mc_telemetries_model.stream_params.bandwidth*0.33f);
  fast_mc_telemetries_model.stream_params.sd_dps = fast_mc_telemetries_model.stream_params.sd_dps - (fast_mc_telemetries_model.stream_params.sd_dps % 512) + 512;

}

/* IIS3DWB_ACC PnPL Component ------------------------------------------------*/
static SensorModel_t iis3dwb_acc_model;

uint8_t iis3dwb_acc_comp_init(void)
{
  iis3dwb_acc_model.comp_name = iis3dwb_acc_get_key();
  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "iis3dwb", COM_TYPE_ACC);
  iis3dwb_acc_model.id = id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  iis3dwb_acc_model.stream_params.stream_id = -1;
  iis3dwb_acc_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  iis3dwb_acc_set_sensor_annotation(default_notes);
#if (HSD_USE_DUMMY_DATA == 1)
  iis3dwb_acc_set_samples_per_ts(0);
#else
  iis3dwb_acc_set_samples_per_ts(1000);
#endif
  app_model.s_models[id] = &iis3dwb_acc_model;
  __stream_control(NULL, true);
  return 0;
}

char* iis3dwb_acc_get_key(void)
{
  return "iis3dwb_acc";
}

uint8_t iis3dwb_acc_get_odr(int32_t *value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)iis3dwb_acc_model.sensor_status.type.mems.odr;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_fs(int32_t *value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)iis3dwb_acc_model.sensor_status.type.mems.fs;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_enable(bool *value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = iis3dwb_acc_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_samples_per_ts(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_dim(int32_t *value)
{
  *value = 3;
  return 0;
}
uint8_t iis3dwb_acc_get_ioffset(float *value)
{
  *value = iis3dwb_acc_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_measodr(float *value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = iis3dwb_acc_model.sensor_status.type.mems.measured_odr;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_usb_dps(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_sd_dps(int32_t *value)
{
  *value = iis3dwb_acc_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_sensitivity(float *value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = iis3dwb_acc_model.sensor_status.type.mems.sensitivity;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}
uint8_t iis3dwb_acc_get_sensor_annotation(char **value)
{
  uint16_t id = iis3dwb_acc_model.id;
  iis3dwb_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = iis3dwb_acc_model.annotation;
  return 0;
}
uint8_t iis3dwb_acc_get_sensor_category(int32_t *value)
{
  *value = iis3dwb_acc_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_stream_id(int8_t *value)
{
  *value = iis3dwb_acc_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_get_ep_id(int8_t *value)
{
  *value = iis3dwb_acc_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_set_fs(int32_t value)
{
  sys_error_code_t ret = SMSensorSetFS(iis3dwb_acc_model.id, value);
  if(ret == SYS_NO_ERROR_CODE)
  {
    iis3dwb_acc_model.sensor_status.type.mems.fs = value;
    /* USER Code */
  }
  return ret;
}
uint8_t iis3dwb_acc_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if(value)
  {
    ret = SMSensorEnable(iis3dwb_acc_model.id);
  }
  else
  {
    ret = SMSensorDisable(iis3dwb_acc_model.id);
  }
  if(ret == SYS_NO_ERROR_CODE)
  {
    iis3dwb_acc_model.sensor_status.is_active = value;
    /* USER Code */
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t iis3dwb_acc_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 1000;
  if (value >= min_v && value <= max_v)
  {
    iis3dwb_acc_model.stream_params.spts = value;
  }
  /* USER Code */
  return 0;
}
uint8_t iis3dwb_acc_set_sensor_annotation(const char *value)
{
  strcpy(iis3dwb_acc_model.annotation, value);
  /* USER Code */
  return 0;
}

/* ILPS22QS_PRESS PnPL Component ---------------------------------------------*/
static SensorModel_t ilps22qs_press_model;

uint8_t ilps22qs_press_comp_init(void)
{
  ilps22qs_press_model.comp_name = ilps22qs_press_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "ilps22qs", COM_TYPE_PRESS);
  ilps22qs_press_model.id = id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  ilps22qs_press_model.stream_params.stream_id = -1;
  ilps22qs_press_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  ilps22qs_press_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  ilps22qs_press_set_samples_per_ts(0);
#else
  ilps22qs_press_set_samples_per_ts(200);
#endif
  app_model.s_models[id] = &ilps22qs_press_model;
  __stream_control(NULL, true);
  return 0;
}

char *ilps22qs_press_get_key(void)
{
  return "ilps22qs_press";
}

uint8_t ilps22qs_press_get_odr(int32_t *value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)ilps22qs_press_model.sensor_status.type.mems.odr;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_fs(int32_t *value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)ilps22qs_press_model.sensor_status.type.mems.fs;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_enable(bool *value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = ilps22qs_press_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_samples_per_ts(int32_t *value)
{
  *value = ilps22qs_press_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_dim(int32_t *value)
{
  *value = 1;
  return 0;
}
uint8_t ilps22qs_press_get_ioffset(float *value)
{
  *value = ilps22qs_press_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_measodr(float *value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = ilps22qs_press_model.sensor_status.type.mems.measured_odr;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_usb_dps(int32_t *value)
{
  *value = ilps22qs_press_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_sd_dps(int32_t *value)
{
  *value = ilps22qs_press_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_sensitivity(float *value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = ilps22qs_press_model.sensor_status.type.mems.sensitivity;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_data_type(char **value)
{
  *value = "float";
  return 0;
}
uint8_t ilps22qs_press_get_sensor_annotation(char **value)
{
  uint16_t id = ilps22qs_press_model.id;
  ilps22qs_press_model.sensor_status = SMSensorGetStatus(id);
  *value = ilps22qs_press_model.annotation;
  return 0;
}
uint8_t ilps22qs_press_get_sensor_category(int32_t *value)
{
    *value = ilps22qs_press_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_stream_id(int8_t *value)
{
  *value = ilps22qs_press_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_get_ep_id(int8_t *value)
{
  *value = ilps22qs_press_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t ilps22qs_press_set_odr(int32_t value)
{
  sys_error_code_t ret = SMSensorSetODR(ilps22qs_press_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ilps22qs_press_model.sensor_status.type.mems.odr = value;
#if (HSD_USE_DUMMY_DATA != 1)
    ilps22qs_press_set_samples_per_ts((int32_t)value);
#endif
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ilps22qs_press_set_fs(int32_t value)
{
  sys_error_code_t ret = SMSensorSetFS(ilps22qs_press_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ilps22qs_press_model.sensor_status.type.mems.fs = value;
    /* USER Code */
  }
  return ret;
}
uint8_t ilps22qs_press_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if (value)
  {
    ret = SMSensorEnable(ilps22qs_press_model.id);
  }
  else
  {
    ret = SMSensorDisable(ilps22qs_press_model.id);
  }
  if (ret == SYS_NO_ERROR_CODE)
  {
    ilps22qs_press_model.sensor_status.is_active = value;
    /* USER Code */
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ilps22qs_press_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 200;
  if (value >= min_v && value <= max_v)
  {
    ilps22qs_press_model.stream_params.spts = value;
  }
  return 0;
}
uint8_t ilps22qs_press_set_sensor_annotation(const char *value)
{
  strcpy(ilps22qs_press_model.annotation, value);
  return 0;
}

/* IMP23ABSU_MIC PnPL Component ----------------------------------------------*/
static SensorModel_t imp23absu_mic_model;

uint8_t imp23absu_mic_comp_init(void)
{
  imp23absu_mic_model.comp_name = imp23absu_mic_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "imp23absu", COM_TYPE_MIC);
  imp23absu_mic_model.id = id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  imp23absu_mic_model.stream_params.stream_id = -1;
  imp23absu_mic_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  imp23absu_mic_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  imp23absu_mic_set_samples_per_ts(0);
#else
  imp23absu_mic_set_samples_per_ts(1000);
  imp23absu_mic_set_odr(16000);
#endif
  app_model.s_models[id] = &imp23absu_mic_model;
  __stream_control(NULL, true);
  return 0;
}

char* imp23absu_mic_get_key(void)
{
  return "imp23absu_mic";
}

uint8_t imp23absu_mic_get_odr(int32_t *value)
{
  uint16_t id = imp23absu_mic_model.id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  *value = imp23absu_mic_model.sensor_status.type.audio.frequency;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_aop(int32_t *value)
{
  *value = 130;
  return 0;
}
uint8_t imp23absu_mic_get_enable(bool *value)
{
  uint16_t id = imp23absu_mic_model.id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  *value = imp23absu_mic_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_volume(int32_t *value)
{
  uint16_t id = imp23absu_mic_model.id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  *value = imp23absu_mic_model.sensor_status.type.audio.volume;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_resolution(int32_t *value)
{
  uint16_t id = imp23absu_mic_model.id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  *value = imp23absu_mic_model.sensor_status.type.audio.resolution;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_samples_per_ts(int32_t *value)
{
  *value = imp23absu_mic_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_dim(int32_t *value)
{
  *value = 1;
  return 0;
}
uint8_t imp23absu_mic_get_ioffset(float *value)
{
  *value = imp23absu_mic_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_usb_dps(int32_t *value)
{
  *value = imp23absu_mic_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_sd_dps(int32_t *value)
{
  *value = imp23absu_mic_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_sensitivity(float *value)
{
  *value = 0.000030517578125; // 2/(2^imp23absu_mic_model.sensor_status.type.audio.resolution);
  return 0;
}
uint8_t imp23absu_mic_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}
uint8_t imp23absu_mic_get_sensor_annotation(char **value)
{
  uint16_t id = imp23absu_mic_model.id;
  imp23absu_mic_model.sensor_status = SMSensorGetStatus(id);
  *value = imp23absu_mic_model.annotation;
  return 0;
}
uint8_t imp23absu_mic_get_sensor_category(int32_t *value)
{
  *value = imp23absu_mic_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_stream_id(int8_t *value)
{
  *value = imp23absu_mic_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_get_ep_id(int8_t *value)
{
  *value = imp23absu_mic_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_set_odr(int32_t value)
{
  sys_error_code_t ret = SMSensorSetFrequency(imp23absu_mic_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    imp23absu_mic_model.sensor_status.type.audio.frequency = value;
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t imp23absu_mic_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if(value)
  {
    ret = SMSensorEnable(imp23absu_mic_model.id);
  }
  else
  {
    ret = SMSensorDisable(imp23absu_mic_model.id);
  }
  if(ret == SYS_NO_ERROR_CODE)
  {
    imp23absu_mic_model.sensor_status.is_active = value;
    /* USER Code */
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t imp23absu_mic_set_volume(int32_t value)
{
  sys_error_code_t ret = SMSensorSetVolume(imp23absu_mic_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    imp23absu_mic_model.sensor_status.type.audio.volume = value;
    /* USER Code */
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t imp23absu_mic_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 1000;
  if (value >= min_v && value <= max_v)
  {
    imp23absu_mic_model.stream_params.spts = value;
  }
  /* USER Code */
  return 0;
}
uint8_t imp23absu_mic_set_sensor_annotation(const char *value)
{
  strcpy(imp23absu_mic_model.annotation, value);
  /* USER Code */
  return 0;
}

/* ISM330DHCX_ACC PnPL Component ---------------------------------------------*/
static SensorModel_t ism330dhcx_acc_model;

uint8_t ism330dhcx_acc_comp_init(void)
{
  ism330dhcx_acc_model.comp_name = ism330dhcx_acc_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "ism330dhcx", COM_TYPE_ACC);
  ism330dhcx_acc_model.id = id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  ism330dhcx_acc_model.stream_params.stream_id = -1;
  ism330dhcx_acc_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  ism330dhcx_acc_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  ism330dhcx_acc_set_samples_per_ts(0);
#else
  ism330dhcx_acc_set_samples_per_ts(1000);
#endif
  app_model.s_models[id] = &ism330dhcx_acc_model;
  __stream_control(NULL, true);
  return 0;
}

char *ism330dhcx_acc_get_key(void)
{
  return "ism330dhcx_acc";
}

uint8_t ism330dhcx_acc_get_odr(float *value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensor_status.type.mems.odr;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_fs(int32_t *value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)ism330dhcx_acc_model.sensor_status.type.mems.fs;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_enable(bool *value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_samples_per_ts(int32_t *value)
{
  *value = ism330dhcx_acc_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_dim(int32_t *value)
{
  *value = 3;
  return 0;
}
uint8_t ism330dhcx_acc_get_ioffset(float *value)
{
  *value = ism330dhcx_acc_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_measodr(float *value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensor_status.type.mems.measured_odr;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_usb_dps(int32_t *value)
{
  *value = ism330dhcx_acc_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_sd_dps(int32_t *value)
{
  *value = ism330dhcx_acc_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_sensitivity(float *value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.sensor_status.type.mems.sensitivity;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}
uint8_t ism330dhcx_acc_get_sensor_annotation(char **value)
{
  uint16_t id = ism330dhcx_acc_model.id;
  ism330dhcx_acc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_acc_model.annotation;
  return 0;
}
uint8_t ism330dhcx_acc_get_sensor_category(int32_t *value)
{
    *value = ism330dhcx_acc_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_stream_id(int8_t *value)
{
  *value = ism330dhcx_acc_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_get_ep_id(int8_t *value)
{
  *value = ism330dhcx_acc_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_acc_set_odr(float value)
{
  sys_error_code_t ret = SMSensorSetODR(ism330dhcx_acc_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensor_status.type.mems.odr = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
#if (HSD_USE_DUMMY_DATA != 1)
    ism330dhcx_acc_set_samples_per_ts((int32_t)value);
#endif
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ism330dhcx_acc_set_fs(int32_t value)
{
  sys_error_code_t ret = SMSensorSetFS(ism330dhcx_acc_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensor_status.type.mems.fs = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
  }
  return ret;
}
uint8_t ism330dhcx_acc_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if (value)
  {
    ret = SMSensorEnable(ism330dhcx_acc_model.id);
  }
  else
  {
    ret = SMSensorDisable(ism330dhcx_acc_model.id);
  }
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_acc_model.sensor_status.is_active = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ism330dhcx_acc_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 1000;
  if (value >= min_v && value <= max_v)
  {
    ism330dhcx_acc_model.stream_params.spts = value;
    //update spts in sensor manager
  }
  return 0;
}
uint8_t ism330dhcx_acc_set_sensor_annotation(const char *value)
{
  strcpy(ism330dhcx_acc_model.annotation, value);
  return 0;
}

/* ISM330DHCX_GYRO PnPL Component --------------------------------------------*/
static SensorModel_t ism330dhcx_gyro_model;

uint8_t ism330dhcx_gyro_comp_init(void)
{
  ism330dhcx_gyro_model.comp_name = ism330dhcx_gyro_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "ism330dhcx", COM_TYPE_GYRO);
  ism330dhcx_gyro_model.id = id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  ism330dhcx_gyro_model.stream_params.stream_id = -1;
  ism330dhcx_gyro_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  ism330dhcx_gyro_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  ism330dhcx_gyro_set_samples_per_ts(0);
#else
  ism330dhcx_gyro_set_samples_per_ts(1000);
#endif
  app_model.s_models[id] = &ism330dhcx_gyro_model;
  __stream_control(NULL, true);
  return 0;
}

char *ism330dhcx_gyro_get_key(void)
{
  return "ism330dhcx_gyro";
}

uint8_t ism330dhcx_gyro_get_odr(float *value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_gyro_model.sensor_status.type.mems.odr;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_fs(int32_t *value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)ism330dhcx_gyro_model.sensor_status.type.mems.fs;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_enable(bool *value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_gyro_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_samples_per_ts(int32_t *value)
{
  *value = ism330dhcx_gyro_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_dim(int32_t *value)
{
  *value = 3;
  return 0;
}
uint8_t ism330dhcx_gyro_get_ioffset(float *value)
{
  *value = ism330dhcx_gyro_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_measodr(float *value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_gyro_model.sensor_status.type.mems.measured_odr;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_usb_dps(int32_t *value)
{
  *value = ism330dhcx_gyro_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_sd_dps(int32_t *value)
{
  *value = ism330dhcx_gyro_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_sensitivity(float *value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_gyro_model.sensor_status.type.mems.sensitivity;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_data_type(char **value)
{
  *value = "int16";
  return 0;
}
uint8_t ism330dhcx_gyro_get_sensor_annotation(char **value)
{
  uint16_t id = ism330dhcx_gyro_model.id;
  ism330dhcx_gyro_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_gyro_model.annotation;
  return 0;
}
uint8_t ism330dhcx_gyro_get_sensor_category(int32_t *value)
{
    *value = ism330dhcx_gyro_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_stream_id(int8_t *value)
{
  *value = ism330dhcx_gyro_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_get_ep_id(int8_t *value)
{
  *value = ism330dhcx_gyro_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_gyro_set_odr(float value)
{
  sys_error_code_t ret = SMSensorSetODR(ism330dhcx_gyro_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_gyro_model.sensor_status.type.mems.odr = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
#if (HSD_USE_DUMMY_DATA != 1)
    ism330dhcx_gyro_set_samples_per_ts((int32_t)value);
#endif
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ism330dhcx_gyro_set_fs(int32_t value)
{
  sys_error_code_t ret = SMSensorSetFS(ism330dhcx_gyro_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_gyro_model.sensor_status.type.mems.fs = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
  }
  return ret;
}
uint8_t ism330dhcx_gyro_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if (value)
  {
    ret = SMSensorEnable(ism330dhcx_gyro_model.id);
  }
  else
  {
    ret = SMSensorDisable(ism330dhcx_gyro_model.id);
  }
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_gyro_model.sensor_status.is_active = value;
    if (app_model.mlc_ucf_valid == true)
    {
      app_model.mlc_ucf_valid = false;
    }
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ism330dhcx_gyro_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 1000;
  if (value >= min_v && value <= max_v)
  {
    ism330dhcx_gyro_model.stream_params.spts = value;
  }
  return 0;
}
uint8_t ism330dhcx_gyro_set_sensor_annotation(const char *value)
{
  strcpy(ism330dhcx_gyro_model.annotation, value);
  return 0;
}

/* ISM330DHCX_MLC PnPL Component ---------------------------------------------*/
static SensorModel_t ism330dhcx_mlc_model;

uint8_t ism330dhcx_mlc_comp_init(void)
{
  ism330dhcx_mlc_model.comp_name = ism330dhcx_mlc_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "ism330dhcx", COM_TYPE_MLC);
  ism330dhcx_mlc_model.id = id;
  ism330dhcx_mlc_model.sensor_status = SMSensorGetStatus(id);
  ism330dhcx_mlc_model.stream_params.stream_id = -1;
  ism330dhcx_mlc_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  ism330dhcx_mlc_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  ism330dhcx_mlc_model.stream_params.spts = 0;
#else
  ism330dhcx_mlc_model.stream_params.spts = 1;
#endif
  app_model.s_models[id] = &ism330dhcx_mlc_model;
  app_model.mlc_ucf_valid = false;
  __stream_control(NULL, true);
  return 0;
}

char *ism330dhcx_mlc_get_key(void)
{
  return "ism330dhcx_mlc";
}

uint8_t ism330dhcx_mlc_get_enable(bool *value)
{
  uint16_t id = ism330dhcx_mlc_model.id;
  ism330dhcx_mlc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_mlc_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_samples_per_ts(int32_t *value)
{
  *value = ism330dhcx_mlc_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_ucf_status(bool *value)
{
  *value = app_model.mlc_ucf_valid;
  return 0;
}
uint8_t ism330dhcx_mlc_get_dim(int32_t *value)
{
  *value = 9;
  return 0;
}
uint8_t ism330dhcx_mlc_get_ioffset(float *value)
{
  *value = ism330dhcx_mlc_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_data_type(char **value)
{
  *value = "int8";
  return 0;
}
uint8_t ism330dhcx_mlc_get_usb_dps(int32_t *value)
{
  *value = ism330dhcx_mlc_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_sd_dps(int32_t *value)
{
  *value = ism330dhcx_mlc_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_sensor_annotation(char **value)
{
  uint16_t id = ism330dhcx_mlc_model.id;
  ism330dhcx_mlc_model.sensor_status = SMSensorGetStatus(id);
  *value = ism330dhcx_mlc_model.annotation;
  return 0;
}
uint8_t ism330dhcx_mlc_get_sensor_category(int32_t *value)
{
    *value = ism330dhcx_mlc_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_stream_id(int8_t *value)
{
  *value = ism330dhcx_mlc_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_get_ep_id(int8_t *value)
{
  *value = ism330dhcx_mlc_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t ism330dhcx_mlc_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if (app_model.mlc_ucf_valid == true)
  {
  if (value)
  {
    ret = SMSensorEnable(ism330dhcx_mlc_model.id);
  }
  else
  {
    ret = SMSensorDisable(ism330dhcx_mlc_model.id);
  }
  if (ret == SYS_NO_ERROR_CODE)
  {
    ism330dhcx_mlc_model.sensor_status.is_active = value;
  }
  __stream_control(NULL, true);
  }
  return ret;
}
uint8_t ism330dhcx_mlc_set_sensor_annotation(const char *value)
{
  strcpy(ism330dhcx_mlc_model.annotation, value);
  return 0;
}
uint8_t ism330dhcx_mlc_load_file(IIsm330dhcx_Mlc_t *ifn, const char *data, int32_t size)
{
  IIsm330dhcx_Mlc_load_file(ifn, data, size);
  app_model.mlc_ucf_valid = true;
  ism330dhcx_mlc_model.sensor_status.is_active = true;
  __stream_control(NULL, true);
  return 0;
}

/* STTS22H_TEMP PnPL Component -----------------------------------------------*/
static SensorModel_t stts22h_temp_model;

uint8_t stts22h_temp_comp_init(void)
{
  stts22h_temp_model.comp_name = stts22h_temp_get_key();

  SQuery_t querySM;
  SQInit(&querySM, SMGetSensorManager());
  uint16_t id = SQNextByNameAndType(&querySM, "stts22h", COM_TYPE_TEMP);
  stts22h_temp_model.id = id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  stts22h_temp_model.stream_params.stream_id = -1;
  stts22h_temp_model.stream_params.usb_ep = -1;
  char default_notes[SENSOR_NOTES_LEN] = "\0";
  stts22h_temp_set_sensor_annotation(default_notes);
  /* USER Code */
#if (HSD_USE_DUMMY_DATA == 1)
  stts22h_temp_set_samples_per_ts(0);
#else
  stts22h_temp_set_samples_per_ts(200);
#endif
  app_model.s_models[id] = &stts22h_temp_model;
  __stream_control(NULL, true);
    return 0;
}

char* stts22h_temp_get_key(void)
{
  return "stts22h_temp";
}

uint8_t stts22h_temp_get_odr(int32_t *value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)stts22h_temp_model.sensor_status.type.mems.odr;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_fs(int32_t *value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = (int32_t)stts22h_temp_model.sensor_status.type.mems.fs;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_enable(bool *value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = stts22h_temp_model.sensor_status.is_active;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_samples_per_ts(int32_t *value)
{
  *value = stts22h_temp_model.stream_params.spts;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_dim(int32_t *value)
{
  *value = 1;
  return 0;
}
uint8_t stts22h_temp_get_ioffset(float *value)
{
  *value = stts22h_temp_model.stream_params.ioffset;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_measodr(float *value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = stts22h_temp_model.sensor_status.type.mems.measured_odr;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_usb_dps(int32_t *value)
{
  *value = stts22h_temp_model.stream_params.usb_dps;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_sd_dps(int32_t *value)
{
  *value = stts22h_temp_model.stream_params.sd_dps;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_sensitivity(float *value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = stts22h_temp_model.sensor_status.type.mems.sensitivity;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_data_type(char **value)
{
  *value = "float";
  return 0;
}
uint8_t stts22h_temp_get_sensor_annotation(char **value)
{
  uint16_t id = stts22h_temp_model.id;
  stts22h_temp_model.sensor_status = SMSensorGetStatus(id);
  *value = stts22h_temp_model.annotation;
  return 0;
}
uint8_t stts22h_temp_get_sensor_category(int32_t *value)
{
    *value = stts22h_temp_model.sensor_status.isensor_class;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_stream_id(int8_t *value)
{
  *value = stts22h_temp_model.stream_params.stream_id;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_get_ep_id(int8_t *value)
{
  *value = stts22h_temp_model.stream_params.usb_ep;
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_set_odr(int32_t value)
{
  sys_error_code_t ret = SMSensorSetODR(stts22h_temp_model.id, value);
  if (ret == SYS_NO_ERROR_CODE)
  {
    stts22h_temp_model.sensor_status.type.mems.odr = value;
#if (HSD_USE_DUMMY_DATA != 1)
    stts22h_temp_set_samples_per_ts((int32_t)value);
#endif
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t stts22h_temp_set_enable(bool value)
{
  sys_error_code_t ret = 1;
  if(value)
  {
    ret = SMSensorEnable(stts22h_temp_model.id);
  }
  else
  {
    ret = SMSensorDisable(stts22h_temp_model.id);
  }
  if(ret == SYS_NO_ERROR_CODE)
  {
    stts22h_temp_model.sensor_status.is_active = value;
    /* USER Code */
    __stream_control(NULL, true);
  }
  return ret;
}
uint8_t stts22h_temp_set_samples_per_ts(int32_t value)
{
  int32_t min_v = 0;
  int32_t max_v = 200;
  if (value >= min_v && value <= max_v)
  {
    stts22h_temp_model.stream_params.spts = value;
  }
  /* USER Code */
  return 0;
}
uint8_t stts22h_temp_set_sensor_annotation(const char *value)
{
  strcpy(stts22h_temp_model.annotation, value);
  /* USER Code */
  return 0;
}

/* Log Controller PnPL Component ---------------------------------------------*/
uint8_t log_controller_comp_init(void)
{
  app_model.log_controller_model.comp_name = log_controller_get_key();

  app_model.log_controller_model.status = false;
  app_model.log_controller_model.interface = LOG_CTRL_MODE_USB;
  return 0;
}

char* log_controller_get_key(void)
{
  return "log_controller";
}

uint8_t log_controller_get_log_status(bool *value)
{
  *value = app_model.log_controller_model.status;
  return 0;
}
uint8_t log_controller_get_sd_mounted(bool *value)
{
  app_model.log_controller_model.sd_mounted = SD_IsDetected();
  *value = app_model.log_controller_model.sd_mounted;
  return 0;
}
uint8_t log_controller_get_controller_type(int32_t *value)
{
  *value = 0; /* 0 == HSD log controller, 1 == App classifier controller, 2 = generic log controller */
  return 0;
}
uint8_t log_controller_save_config(ILog_Controller_t *ifn)
{
  ILog_Controller_save_config(ifn);
  return 0;
}
uint8_t log_controller_start_log(ILog_Controller_t *ifn, int32_t interface)
{
  app_model.log_controller_model.interface = interface;

  __stream_control(ifn, TRUE);

  //Reset Tag counter
  TMResetTagListCounter();
  ILog_Controller_start_log(ifn, interface);

  RTC_DateTypeDef sdate;
  RTC_TimeTypeDef stime;
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);

  _tm t =
      { .tm_year = sdate.Year + 2000, .tm_mon = sdate.Month - 1, .tm_mday = sdate.Date, .tm_hour = stime.Hours, .tm_min = stime.Minutes, .tm_sec = stime.Seconds };

// WHY THIS -1 (in months) ???
//  struct tm {
//     int tm_sec;         /* seconds,  range 0 to 59          */
//     int tm_min;         /* minutes, range 0 to 59           */
//     int tm_hour;        /* hours, range 0 to 23             */
//     int tm_mday;        /* day of the month, range 1 to 31  */
//     int tm_mon;         /* month, range 0 to 11             */ <------ (-1) months here (0..11), months from RTC (1..12)
//     int tm_year;        /* The number of years since 1900   */
//     int tm_wday;        /* day of the week, range 0 to 6    */
//     int tm_yday;        /* day in the year, range 0 to 365  */
//     int tm_isdst;       /* daylight saving time             */
//  };

  TMSetStartTime(t);
  sprintf(app_model.acquisition_info_model.start_time, "%04d-%02d-%02dT%02d:%02d:%02d", t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

  /* last part not done in sprintf to avoid a warning  */
  app_model.acquisition_info_model.start_time[19] = '.';
  app_model.acquisition_info_model.start_time[20] = '0';
  app_model.acquisition_info_model.start_time[21] = '0';
  app_model.acquisition_info_model.start_time[22] = '0';
  app_model.acquisition_info_model.start_time[23] = 'Z';
  app_model.acquisition_info_model.start_time[24] = '\0';

  return 0;
}
uint8_t log_controller_stop_log(ILog_Controller_t *ifn)
{
  PnPLGenerateAcquisitionUUID(app_model.acquisition_info_model.uuid);
  TMCloseAllOpenedTags();
  ILog_Controller_stop_log(ifn);
  TMCalculateEndTime(app_model.acquisition_info_model.end_time);
  return 0;
}
uint8_t log_controller_set_time(ILog_Controller_t *ifn, const char *datetime)
{
  ILog_Controller_set_time(ifn, datetime);
  return 0;
}
uint8_t log_controller_switch_bank(ILog_Controller_t *ifn)
{
  ILog_Controller_switch_bank(ifn);
  return 0;
}

/* Motor Controller PnPL Component -------------------------------------------*/
uint8_t motor_controller_comp_init(void)
{
  app_model.motor_controller_model.comp_name = motor_controller_get_key();
  /* USER Component initialization code */
  return 0;
}
char* motor_controller_get_key(void)
{
  return "motor_controller";
}
uint8_t motor_controller_get_motor_status(bool *value)
{
  *value = MCPTask_GetMotorStarted();
  return 0;
}
uint8_t motor_controller_get_motor_speed(int32_t *value)
{
  /* USER Code */
  *value = MCPTask_GetMotorSpeed();
  return 0;
}
uint8_t motor_controller_set_motor_speed(int32_t value)
{
  int32_t min = -4000;
  int32_t max = 4000;
  /* USER Code */
  if (value >= min && value <= max)
  {
    MCPTask_SetMotorSpeed(value);
  }
  return 0;
}
uint8_t motor_controller_start_motor(IMotor_Controller_t *ifn)
{
  IMotor_Controller_start_motor(ifn);
  return 0;
}
uint8_t motor_controller_stop_motor(IMotor_Controller_t *ifn)
{
  IMotor_Controller_stop_motor(ifn);
  return 0;
}
uint8_t motor_controller_motor_reset(IMotor_Controller_t *ifn)
{
  IMotor_Controller_motor_reset(ifn);
  return 0;
}

uint8_t motor_controller_ack_fault(IMotor_Controller_t *ifn)
{
  IMotor_Controller_ack_fault(ifn);
  return 0;
}

/* Tags Information PnPL Component -------------------------------------------*/
uint8_t tags_info_comp_init(void)
{
  app_model.tags_info_model.comp_name = tags_info_get_key();

  TMInit(HSD_SW_TAG_CLASS_NUM, HSD_HW_TAG_CLASS_NUM);

  tags_info_set_sw_tag0__enabled(true);
  tags_info_set_sw_tag1__enabled(true);
  tags_info_set_sw_tag2__enabled(true);
  tags_info_set_sw_tag3__enabled(true);
  tags_info_set_sw_tag4__enabled(true);
  tags_info_set_sw_tag0__status(false);
  tags_info_set_sw_tag1__status(false);
  tags_info_set_sw_tag2__status(false);
  tags_info_set_sw_tag3__status(false);
  tags_info_set_sw_tag4__status(false);
    return 0;
}

char* tags_info_get_key(void)
{
  return "tags_info";
}

uint8_t tags_info_get_max_tags_num(int32_t *value)
{
  *value = HSD_MAX_TAGS_NUM;
  return 0;
}
uint8_t tags_info_get_sw_tag0__label(char **value)
{
  *value = TMGetSWTagLabel(0);
  return 0;
}
uint8_t tags_info_get_sw_tag0__enabled(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(0);
  *value = sw_tag_class->enabled;
  return 0;
}
uint8_t tags_info_get_sw_tag0__status(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(0);
  *value = sw_tag_class->status;
  return 0;
}
uint8_t tags_info_get_sw_tag1__label(char **value)
{
  *value = TMGetSWTagLabel(1);
  return 0;
}
uint8_t tags_info_get_sw_tag1__enabled(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(1);
  *value = sw_tag_class->enabled;
  return 0;
}
uint8_t tags_info_get_sw_tag1__status(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(1);
  *value = sw_tag_class->status;
  return 0;
}
uint8_t tags_info_get_sw_tag2__label(char **value)
{
  *value = TMGetSWTagLabel(2);
  return 0;
}
uint8_t tags_info_get_sw_tag2__enabled(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(2);
  *value = sw_tag_class->enabled;
  return 0;
}
uint8_t tags_info_get_sw_tag2__status(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(2);
  *value = sw_tag_class->status;
  return 0;
}
uint8_t tags_info_get_sw_tag3__label(char **value)
{
  *value = TMGetSWTagLabel(3);
  return 0;
}
uint8_t tags_info_get_sw_tag3__enabled(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(3);
  *value = sw_tag_class->enabled;
  return 0;
}
uint8_t tags_info_get_sw_tag3__status(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(3);
  *value = sw_tag_class->status;
  return 0;
}
uint8_t tags_info_get_sw_tag4__label(char **value)
{
  *value = TMGetSWTagLabel(4);
  return 0;
}
uint8_t tags_info_get_sw_tag4__enabled(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(4);
  *value = sw_tag_class->enabled;
  return 0;
}
uint8_t tags_info_get_sw_tag4__status(bool *value)
{
  HSD_SW_Tag_Class_t *sw_tag_class;
  sw_tag_class = TMGetSWTag(4);
  *value = sw_tag_class->status;
  return 0;
}
uint8_t tags_info_set_sw_tag0__label(const char *value)
{
  return TMSetSWTagLabel(value, 0);
}
uint8_t tags_info_set_sw_tag0__enabled(bool value)
{
  return TMEnableSWTag(value, 0);
}
uint8_t tags_info_set_sw_tag0__status(bool value)
{
  bool status;
  log_controller_get_log_status(&status);

  if(status)
  {
    TMSetSWTag(value, 0);
  }
  else
  {
    TMInitSWTagStatus(false, 0);
  }
  return 0;
}
uint8_t tags_info_set_sw_tag1__label(const char *value)
{
  return TMSetSWTagLabel(value, 1);
}
uint8_t tags_info_set_sw_tag1__enabled(bool value)
{
  return TMEnableSWTag(value, 1);
}
uint8_t tags_info_set_sw_tag1__status(bool value)
{
  bool status;
  log_controller_get_log_status(&status);

  if(status)
  {
    TMSetSWTag(value, 1);
  }
  else
  {
    TMInitSWTagStatus(false, 1);
  }
  return 0;
}
uint8_t tags_info_set_sw_tag2__label(const char *value)
{
  return TMSetSWTagLabel(value, 2);
}
uint8_t tags_info_set_sw_tag2__enabled(bool value)
{
  return TMEnableSWTag(value, 2);
}
uint8_t tags_info_set_sw_tag2__status(bool value)
{
  bool status;
  log_controller_get_log_status(&status);

  if(status)
  {
    TMSetSWTag(value, 2);
  }
  else
  {
    TMInitSWTagStatus(false, 2);
  }
  return 0;
}
uint8_t tags_info_set_sw_tag3__label(const char *value)
{
  return TMSetSWTagLabel(value, 3);
}
uint8_t tags_info_set_sw_tag3__enabled(bool value)
{
  return TMEnableSWTag(value, 3);
}
uint8_t tags_info_set_sw_tag3__status(bool value)
{
  bool status;
  log_controller_get_log_status(&status);

  if(status)
  {
    TMSetSWTag(value, 3);
  }
  else
  {
    TMInitSWTagStatus(false, 3);
  }
  return 0;
}
uint8_t tags_info_set_sw_tag4__label(const char *value)
{
  return TMSetSWTagLabel(value, 4);
}
uint8_t tags_info_set_sw_tag4__enabled(bool value)
{
  return TMEnableSWTag(value, 4);
}
uint8_t tags_info_set_sw_tag4__status(bool value)
{
  bool status;
  log_controller_get_log_status(&status);

  if(status)
  {
    TMSetSWTag(value, 4);
  }
  else
  {
    TMInitSWTagStatus(false, 4);
  }
  return 0;
}

/* Acquisition Information PnPL Component ------------------------------------*/
uint8_t acquisition_info_comp_init(void)
{
  app_model.acquisition_info_model.comp_name = acquisition_info_get_key();

  acquisition_info_set_name("STWIN.Box_acquisition\0");
  acquisition_info_set_description("HighSpeedDatalog v2.0.0 on STWIN.Box\0");
  app_model.acquisition_info_model.interface = -1;
  return 0;
}

char* acquisition_info_get_key(void)
{
  return "acquisition_info";
}

uint8_t acquisition_info_get_name(char **value)
{
  *value = app_model.acquisition_info_model.name;
  return 0;
}
uint8_t acquisition_info_get_description(char **value)
{
  *value = app_model.acquisition_info_model.description;
  return 0;
}
uint8_t acquisition_info_get_uuid(char **value)
{
  *value = app_model.acquisition_info_model.uuid;
  return 0;
}
uint8_t acquisition_info_get_start_time(char **value)
{
  *value = app_model.acquisition_info_model.start_time;
  return 0;
}
uint8_t acquisition_info_get_end_time(char **value)
{
  *value = app_model.acquisition_info_model.end_time;
  return 0;
}
uint8_t acquisition_info_get_data_ext(char **value)
{
  *value = ".dat";
  return 0;
}
uint8_t acquisition_info_get_data_fmt(char **value)
{
  *value = "HSD_2.0.0";
  return 0;
}
uint8_t acquisition_info_get_tags(JSON_Value *value)
{
  JSON_Value *tempJSON1;
  JSON_Object *JSON_Tags;
  JSON_Array *JSON_TagsArray;

  JSON_Tags = json_value_get_object(value);

  json_object_set_value(JSON_Tags, "tags", json_value_init_array());
  JSON_TagsArray = json_object_dotget_array(JSON_Tags, "tags");

  int i;

  HSD_Tag_t *tag_list = TMGetTagList();
  uint8_t tag_list_size = TMGetTagListSize();
  if(tag_list_size > 0)
  {
    for(i = 0; i < tag_list_size; i++)
    {
      tempJSON1 = json_value_init_object();
      JSON_Object *tag_object = json_value_get_object(tempJSON1);
      json_object_set_string(tag_object, "l", tag_list[i].label);
      json_object_set_boolean(tag_object, "e", tag_list[i].status);
      json_object_set_string(tag_object, "ta", tag_list[i].abs_timestamp);
      json_array_append_value(JSON_TagsArray, tempJSON1);
    }
  }
  /* no need to free tempJSON1 as it is part of value */
  return 0;
}
uint8_t acquisition_info_get_interface(char **value)
{
  int8_t acq_interface = app_model.acquisition_info_model.interface;
  switch (acq_interface)
  {
    case 0:
      *value = acquisition_info_interface_sd;
      break;
    case 1:
      *value = acquisition_info_interface_usb;
      break;
  }
  return 0;
}
uint8_t acquisition_info_get_schema_version(char **value)
{
  *value = "2.0.0";
  return 0;
}
uint8_t acquisition_info_set_name(const char *value)
{
  if(strlen(value) != 0)
  {
    strcpy(app_model.acquisition_info_model.name, value);
  }
  return 0;
}
uint8_t acquisition_info_set_description(const char *value)
{
  strcpy(app_model.acquisition_info_model.description, value);
  return 0;
}

/* Firmware Information PnPL Component ---------------------------------------*/
uint8_t firmware_info_comp_init(void)
{
  app_model.firmware_info_model.comp_name = firmware_info_get_key();

  char default_alias[DEVICE_ALIAS_LENGTH] = "STWIN_BOX_001";
  firmware_info_set_alias(default_alias);

  char default_mac_address[MAC_ADDRESS_LENGTH] = "00:00:00:00:00:00";
  set_mac_address(default_mac_address);

  return 0;
}

char* firmware_info_get_key(void)
{
  return "firmware_info";
}

uint8_t firmware_info_get_alias(char **value)
{
  *value = app_model.firmware_info_model.alias;
  return 0;
}
uint8_t firmware_info_get_fw_name(char **value)
{
  *value = "FP-IND-DATALOGMC_Datalogmc";
  return 0;
}
uint8_t firmware_info_get_fw_version(char **value)
{
  *value = FW_VERSION_MAJOR "." FW_VERSION_MINOR "." FW_VERSION_PATCH;
  return 0;
}
uint8_t firmware_info_get_part_number(char **value)
{
  *value = "FP-IND-DATALOGMC";
  return 0;
}
uint8_t firmware_info_get_device_url(char **value)
{
  *value = "https://www.st.com/stwinbox";
  return 0;
}
uint8_t firmware_info_get_fw_url(char **value)
{
  *value = "https://www.st.com/stwinbox";
  return 0;
}
uint8_t firmware_info_get_mac_address(char **value)
{
  *value = app_model.firmware_info_model.mac_address;
  return 0;
}
uint8_t firmware_info_set_alias(const char *value)
{
  strcpy(app_model.firmware_info_model.alias, value);
    return 0;
}

uint8_t set_mac_address(const char *value)
{
  strcpy(app_model.firmware_info_model.mac_address, value);
  return 0;
}

/* Device Information PnPL Component -----------------------------------------*/
uint8_t DeviceInformation_comp_init(void)
{
  return 0;
}

char* DeviceInformation_get_key(void)
{
  return "DeviceInformation";
}

uint8_t DeviceInformation_get_manufacturer(char **value)
{
  *value = "STMicroelectronics";
  return 0;
}
uint8_t DeviceInformation_get_model(char **value)
{
  *value = "STEVAL-STWINBX1";
  return 0;
}
uint8_t DeviceInformation_get_swVersion(char **value)
{
  *value = FW_VERSION_MAJOR "." FW_VERSION_MINOR "." FW_VERSION_PATCH;
  return 0;
}
uint8_t DeviceInformation_get_osName(char **value)
{
  *value = "AzureRTOS";
  return 0;
}
uint8_t DeviceInformation_get_processorArchitecture(char **value)
{
  *value = "ARM Cortex-M33";
  return 0;
}
uint8_t DeviceInformation_get_processorManufacturer(char **value)
{
  *value = "STMicroelectronics";
  return 0;
}
uint8_t DeviceInformation_get_totalStorage(float *value)
{
  *value = 0;
  return 0;
}
uint8_t DeviceInformation_get_totalMemory(float *value)
{
  *value = 0;
  return 0;
}

static uint8_t __stream_control(ILog_Controller_t *ifn, bool status)
{
int8_t i;
  if(status) //set stream ids
  {
    int8_t j, st_i = 0;
    uint16_t proposed_fifoWM = 1;
    //sort stream id by bandwidth
    for(i = 0; i < SENSOR_NUMBER; i++)
    {
      if(app_model.s_models[i] != NULL)
      {
        if(app_model.s_models[i]->sensor_status.is_active == true)
        {
          if(app_model.s_models[i]->sensor_status.isensor_class == ISENSOR_CLASS_MEMS)
          {
            app_model.s_models[i]->stream_params.bandwidth = app_model.s_models[i]->sensor_status.type.mems.odr * SMGetnBytesPerSample(i);
          }
          else if(app_model.s_models[i]->sensor_status.isensor_class == ISENSOR_CLASS_AUDIO)
          {
            app_model.s_models[i]->stream_params.bandwidth = app_model.s_models[i]->sensor_status.type.audio.frequency * SMGetnBytesPerSample(i);
          }
          else
          {
            /* TODO: add support for other ISENSOR_CLASS */
          }

          if(app_model.log_controller_model.interface == LOG_CTRL_MODE_SD)
          {
            /* 330ms of sensor data. Access to SD is optimized when buffer dimension is multiple of 512 */
            app_model.s_models[i]->stream_params.sd_dps = (uint32_t)(app_model.s_models[i]->stream_params.bandwidth*0.33f);
            app_model.s_models[i]->stream_params.sd_dps = app_model.s_models[i]->stream_params.sd_dps - (app_model.s_models[i]->stream_params.sd_dps % 512) + 512;

            /*********** IS IT STILL multiple of 512?  **************/
            if (app_model.s_models[i]->stream_params.sd_dps > app_model.s_models[i]->stream_params.bandwidth)
            {
              app_model.s_models[i]->stream_params.sd_dps = (uint32_t)app_model.s_models[i]->stream_params.bandwidth + 8 + 4;  /* 8 = timestamp dimension in bytes; 4 = dataloss protocol */
            }

            proposed_fifoWM = app_model.s_models[i]->stream_params.sd_dps/SMGetnBytesPerSample(i)/2;
            if(proposed_fifoWM==0)
            {
              proposed_fifoWM = 1;
            }
            sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
            if(ret != SYS_NO_ERROR_CODE)
            {

            }
          }
          else if(app_model.log_controller_model.interface == LOG_CTRL_MODE_USB)
          {

            /* in case of slow sensor send 1 sample for each usb packet */
            float low_odr = 0;
            if(app_model.s_models[i]->sensor_status.isensor_class == ISENSOR_CLASS_MEMS)
            {
              low_odr = app_model.s_models[i]->sensor_status.type.mems.odr;
            }
            else if(app_model.s_models[i]->sensor_status.isensor_class == ISENSOR_CLASS_AUDIO)
            {
              low_odr = app_model.s_models[i]->sensor_status.type.audio.frequency;
            }
            else
            {
              /* TODO: add support for other ISENSOR_CLASS */
            }

            if(low_odr <= 20)
            {
              /* When there's a timestamp, more then one packet will be sent */
              app_model.s_models[i]->stream_params.usb_dps = SMGetnBytesPerSample(i) + 8; /* 8 = timestamp dimension in bytes */
              proposed_fifoWM = 1;
              sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
              if(ret != SYS_NO_ERROR_CODE)
              {

              }
            }
            else
            {
              /* 50ms of sensor data; when there's a timestamp packets will be sent fastly */
              app_model.s_models[i]->stream_params.usb_dps = (uint32_t)(app_model.s_models[i]->stream_params.bandwidth*0.05f);
              if(app_model.s_models[i]->stream_params.usb_dps > 7000)
              {
                app_model.s_models[i]->stream_params.usb_dps = 7000; // set a limit to avoid buffer to big
              }
              else if(app_model.s_models[i]->stream_params.usb_dps < SMGetnBytesPerSample(i) + 8)
              {
                /* In case usb_dps is a very low value, verify the setup to send at least 1 sensor data + timestamp */
                app_model.s_models[i]->stream_params.usb_dps = SMGetnBytesPerSample(i) + 8;
              }

              proposed_fifoWM = app_model.s_models[i]->stream_params.usb_dps/SMGetnBytesPerSample(i)/2;
              if(proposed_fifoWM==0)
              {
                proposed_fifoWM = 1;
              }
              sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
              if(ret != SYS_NO_ERROR_CODE)
              {

              }
            }
#ifdef SYS_DEBUG
            SensorDescriptor_t descriptor = SMSensorGetDescription(i);
            float ms = app_model.s_models[i]->stream_params.usb_dps/app_model.s_models[i]->stream_params.bandwidth;
            SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("**** %s, odr: %f, DPS: %d, ms: %f, proposed FIFO WM: %d \r\n", descriptor.p_name,
                                               app_model.s_models[i]->sensor_status.type.mems.odr, app_model.s_models[i]->stream_params.usb_dps, ms, proposed_fifoWM));
#endif
          }

          app_model.s_models[i]->stream_params.stream_id = st_i;
          st_i++;

          if(i > 0)
          {
            j = i - 1;
            while(j >= 0)
            {
              if (app_model.s_models[j] != NULL)
              {
                if(app_model.s_models[j]->stream_params.bandwidth <= app_model.s_models[i]->stream_params.bandwidth)
                {
                  if(app_model.s_models[j]->stream_params.stream_id >=0)
                  {
                    if(app_model.s_models[j]->stream_params.stream_id < app_model.s_models[i]->stream_params.stream_id)
                    {
                      app_model.s_models[i]->stream_params.stream_id = app_model.s_models[j]->stream_params.stream_id;
                    }
                    app_model.s_models[j]->stream_params.stream_id++;
                  }
                }
              }
              j--;
            }
          }
        }
        else
        {
          app_model.s_models[i]->stream_params.bandwidth = 0;
          app_model.s_models[i]->stream_params.stream_id = -1;
        }
      }
    }
    for(i = 0; i < SENSOR_NUMBER; i++)
    {
      if(app_model.s_models[i] != NULL)
      {
        int8_t stream = app_model.s_models[i]->stream_params.stream_id;
        if(stream < N_MAX_EP - 2)
        {
          app_model.s_models[i]->stream_params.usb_ep = stream;
        }
        else
        {
          if (stream >= FAST_TELEMETRY_APP_MODEL_STREAM_ID)
          {
            app_model.s_models[i]->stream_params.stream_id += 2;
          }
          app_model.s_models[i]->stream_params.usb_ep = N_MAX_EP - 1;
        }
      }
    }
//    for(i = 0; i < ALGORITHM_NUMBER; i++)     /* Generated only when algorithms are available */
//    {
//      //TODO
//      if(app_model.a_models[i] != NULL)
//      {
//        app_model.a_models[i]->stream_params.stream_id = i + SENSOR_NUMBER;
//      }
//    }
  }
  else
  {
    for(i = 0; i < SENSOR_NUMBER; i++)
    {
      if(app_model.s_models[i] != NULL)
      {
        app_model.s_models[i]->stream_params.stream_id = -1;
        app_model.s_models[i]->stream_params.usb_ep = -1;
        app_model.s_models[i]->stream_params.bandwidth = 0;
      }
    }
//    for(i = 0; i < ALGORITHM_NUMBER; i++)     /* Generated only when algorithms are available */
//    {
//
//      app_model.a_models[i]->stream_params.stream_id = -1;
//      app_model.a_models[i]->stream_params.usb_ep = -1;
//      app_model.a_models[i]->stream_params.bandwidth = 0;
//    }
  }
    return 0;
}

static void update_fast_telemetry_state(MCPTaskFastTelemetry_idx_t fast_telemetry_idx, bool value)
{
  /* Get current slow telemtry state*/
    bool current_state = MCPTask_GetFastTelemetryState(fast_telemetry_idx);

    if(value)
    {
      if(fast_mc_telemetries_model.active_telemetry_cnt < FAST_TELEMETRY_MAX_ENABLED)
      {
        if(current_state != value)
        {
          /* Enable fast telemetry */
          MCPTask_SetFastTelemetryState(fast_telemetry_idx, value);

          /*Update fast telemetry counter*/
          fast_mc_telemetries_model.active_telemetry_cnt++;

          /* Check upper Level Component*/
          if(!fast_mc_telemetries_model.actuatorStatus.is_active)
          {
            fast_mc_telemetries_model.actuatorStatus.is_active = true;
          }
        }
      }
    }
    else
    {
      if(current_state != value)
      {
        /* Enable fast telemetry */
        MCPTask_SetFastTelemetryState(fast_telemetry_idx, value);

        /*Update fast telemetry counter*/
        fast_mc_telemetries_model.active_telemetry_cnt--;

        /**/
        if(0 == MCPTask_GetNumOfEnabledFastTelemetry())
        {
          fast_mc_telemetries_model.actuatorStatus.is_active = false;
        }
      }
    }

    /*Update USB DPS based on enabled fast telemetry*/
    fast_mc_telemetries_configure_usb_dps();

    /*Update SD DPS based on enabled fast telemetry*/
    fast_mc_telemetries_configure_sd_dps();
}

static void update_slow_telemetry_state(MCPTaskSlowTelemetry_idx_t slow_telemetry_idx, bool value)
{
/* Get current slow telemtry state*/
  bool current_state = MCPTask_GetSlowTelemetryState(slow_telemetry_idx);

  /* Set new state */
  if(current_state != value)
  {
    /* Check new configuration */
    if(value)
    {
      MCPTask_SetSlowTelemetryState(slow_telemetry_idx, value);

      /* Check upper Level Component*/
      if(!slow_mc_telemetries_model.actuatorStatus.is_active)
      {
        slow_mc_telemetries_model.actuatorStatus.is_active = true;
      }
      slow_mc_telemetries_model.stream_params.usb_dps += sizeof(uint32_t);
    }
    else
    {
      MCPTask_SetSlowTelemetryState(slow_telemetry_idx, value);
      slow_mc_telemetries_model.stream_params.usb_dps -= sizeof(uint32_t);

      /**/
      if(0 == MCPTask_GetNumOfEnabledSlowTelemetry())
      {
        slow_mc_telemetries_model.actuatorStatus.is_active = false;
      }
    }
  }

  slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[slow_telemetry_idx].status = value;
}
