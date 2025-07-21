/**
  ******************************************************************************
  * @file    App_model_Slow_Mc_Telemetries.c
  * @author  SRA
  * @brief   Slow_Mc_Telemetries PnPL Components APIs
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
  * This file has been auto generated from the following DTDL Component:
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:slow_mc_telemetries;2
  *
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"

/* USER includes -------------------------------------------------------------*/
#include "MCPTask.h"

/* USER private function prototypes ------------------------------------------*/
static void update_slow_telemetry_state(uint8_t slow_telemetry_idx, bool value);

/* USER defines --------------------------------------------------------------*/
/* Slow telemetry definition */
#define SLOW_TELEMETRY_APP_MODEL_IDX          0u
#define SLOW_TELEMETRY_APP_MODEL_USB_EP       4u
#define SLOW_TELEMETRY_APP_MODEL_STREAM_ID    4u
#define SLOW_TELEMETRY_ODR                    3U
#define SLOW_TELEMETRY_SAMPLE_SIZE            4U
#define SLOW_TELEMETRY_MAX_NUM                4U

/* Slow Motor Telemetries PnPL Component -------------------------------------*/
static ActuatorModel_t slow_mc_telemetries_model;

extern AppModel_t app_model;

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

  slow_mc_telemetries_set_st_ble_stream__id(1, NULL);

  slow_mc_telemetries_set_st_ble_stream__temperature_unit("Celsius", NULL);
  slow_mc_telemetries_set_st_ble_stream__temperature_format("int32_t", NULL);

  slow_mc_telemetries_set_st_ble_stream__ref_speed_unit("rpm", NULL);
  slow_mc_telemetries_set_st_ble_stream__ref_speed_format("int32_t", NULL);

  slow_mc_telemetries_set_st_ble_stream__speed_unit("rpm", NULL);
  slow_mc_telemetries_set_st_ble_stream__speed_format("int32_t", NULL);

  slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit("V", NULL);
  slow_mc_telemetries_set_st_ble_stream__bus_voltage_format("int32_t", NULL);

  slow_mc_telemetries_set_st_ble_stream__fault_unit("", NULL);
  slow_mc_telemetries_set_st_ble_stream__fault_format("int32_t", NULL);

  /* Slow telemetry state */
  slow_mc_telemetries_set_st_ble_stream__temperature_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
  slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
  slow_mc_telemetries_set_st_ble_stream__speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
  slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
  slow_mc_telemetries_set_st_ble_stream__fault_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);

  //  /* Worst case 4byte for maximum 4 slow telemetries each 300ms */
  slow_mc_telemetries_model.stream_params.bandwidth = SLOW_TELEMETRY_ODR * SLOW_TELEMETRY_SAMPLE_SIZE * SLOW_TELEMETRY_MAX_NUM;
  slow_mc_telemetries_model.stream_params.sd_dps = SLOW_TELEMETRY_ODR * SLOW_TELEMETRY_SAMPLE_SIZE * SLOW_TELEMETRY_MAX_NUM;

  app_model.ac_models[SLOW_TELEMETRY_APP_MODEL_IDX] = &slow_mc_telemetries_model;
  app_model.slow_mc_telemetries_ble_stream_model = &slow_mc_telemetries_ble_stream_model;

  return 0;
}
char *slow_mc_telemetries_get_key(void)
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
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_max(int32_t *value)
{
  /* USER Code */
  *value = MCPTask_GetMaxSpeed();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__speed_initial_value(int32_t *value)
{
  /* USER Code */
  *value = MCPTask_GetRampSpeed();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t slow_mc_telemetries_get_st_ble_stream__bus_voltage_enable(bool *value)
{
  *value = MCPTask_GetSlowTelemetryState(MC_REG_BUS_VOLTAGE_IDX);
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
uint8_t slow_mc_telemetries_get_dim(int32_t *value)
{
  *value = MCPTask_GetNumOfEnabledSlowTelemetry();
  return PNPL_NO_ERROR_CODE;
}
uint8_t slow_mc_telemetries_get_odr(float_t *value)
{
  *value = MCPTask_GetSlowTelemetryODR();
  return PNPL_NO_ERROR_CODE;

}
uint8_t slow_mc_telemetries_get_ioffset(float_t *value)
{
  *value = slow_mc_telemetries_model.stream_params.ioffset;
  return PNPL_NO_ERROR_CODE;
}
uint8_t slow_mc_telemetries_get_samples_per_ts(int32_t *value)
{
  *value = 1;
  return PNPL_NO_ERROR_CODE;
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
uint8_t slow_mc_telemetries_set_enable(bool value, char **response_message)
{
  slow_mc_telemetries_model.actuatorStatus.is_active = value;

  if (response_message != NULL)
  {
    *response_message = "";
  }

  if (!value)
  {
    slow_mc_telemetries_set_st_ble_stream__temperature_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
    slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
    slow_mc_telemetries_set_st_ble_stream__speed_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
    slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
    slow_mc_telemetries_set_st_ble_stream__fault_enable(slow_mc_telemetries_model.actuatorStatus.is_active, NULL);
  }
  else
  {
    /* Get num of enabled fast telemetries */
    if (0 == MCPTask_GetNumOfEnabledSlowTelemetry())
    {
      /* Enable all telemetries in the list */
      slow_mc_telemetries_set_st_ble_stream__temperature_enable(true, NULL);
      slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(true, NULL);
      slow_mc_telemetries_set_st_ble_stream__speed_enable(true, NULL);
      slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(true, NULL);
      slow_mc_telemetries_set_st_ble_stream__fault_enable(true, NULL);
    }
  }

  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__id(int32_t value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  slow_mc_telemetries_ble_stream_model.st_ble_stream_id = value;
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_slow_telemetry_state(MC_REG_HEATS_TEMP_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__temperature_format(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_HEATS_TEMP_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_slow_telemetry_state(MC_REG_SPEED_REF_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__ref_speed_format(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_REF_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_slow_telemetry_state(MC_REG_SPEED_MEAS_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_format(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_SPEED_MEAS_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_max(int32_t value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return PNPL_NO_ERROR_CODE ;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__speed_initial_value(int32_t value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return PNPL_NO_ERROR_CODE ;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_slow_telemetry_state(MC_REG_BUS_VOLTAGE_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].unit, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__bus_voltage_format(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_BUS_VOLTAGE_IDX].format, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_slow_telemetry_state(MC_REG_FAULTS_FLAGS_IDX, value);
  return 0;
}
uint8_t slow_mc_telemetries_set_st_ble_stream__fault_unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].unit, value);
  return 0;
}

uint8_t slow_mc_telemetries_set_st_ble_stream__fault_format(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  strcpy(slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[MC_REG_FAULTS_FLAGS_IDX].format, value);
  return 0;
}

/* Private function definition */
static void update_slow_telemetry_state(uint8_t slow_telemetry_idx, bool value)
{
  /* Get current slow telemetry state*/
  bool current_state = MCPTask_GetSlowTelemetryState((MCPTaskSlowTelemetry_idx_t) slow_telemetry_idx);

  /* Set new state */
  if (current_state != value)
  {
    /* Check new configuration */
    if (value)
    {
      MCPTask_SetSlowTelemetryState((MCPTaskSlowTelemetry_idx_t) slow_telemetry_idx, value);

      /* Check upper Level Component*/
      if (!slow_mc_telemetries_model.actuatorStatus.is_active)
      {
        slow_mc_telemetries_model.actuatorStatus.is_active = true;
      }
      slow_mc_telemetries_model.stream_params.usb_dps += sizeof(uint32_t);
    }
    else
    {
      MCPTask_SetSlowTelemetryState((MCPTaskSlowTelemetry_idx_t) slow_telemetry_idx, value);
      slow_mc_telemetries_model.stream_params.usb_dps -= sizeof(uint32_t);

      /**/
      if (0 == MCPTask_GetNumOfEnabledSlowTelemetry())
      {
        slow_mc_telemetries_model.actuatorStatus.is_active = false;
      }
    }
  }

  slow_mc_telemetries_ble_stream_model.st_ble_stream_objects[slow_telemetry_idx].status = value;
}

