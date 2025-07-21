/**
  ******************************************************************************
  * @file    App_model_Fast_Mc_Telemetries.c
  * @author  SRA
  * @brief   Fast_Mc_Telemetries PnPL Components APIs
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
  * dtmi:dtmi:vespucci:steval_stwinbx1:fpIndDatalogMC_datalog2MC:actuators:fast_mc_telemetries;1
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
static void update_fast_telemetry_state(uint8_t fast_telemetry_idx, bool value);
static inline void fast_mc_telemetries_configure_usb_dps(void);
static inline void fast_mc_telemetries_configure_sd_dps(void);

/* USER defines --------------------------------------------------------------*/
/* Fast telemetry definition */
#define FAST_TELEMETRY_APP_MODEL_IDX          1u
#define FAST_TELEMETRY_APP_MODEL_USB_EP       2u
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

/* Fast Motor Telemetries  PnPL Component ------------------------------------*/
static ActuatorModel_t fast_mc_telemetries_model;

extern AppModel_t app_model;

uint8_t fast_mc_telemetries_comp_init(void)
{
  fast_mc_telemetries_model.comp_name = fast_mc_telemetries_get_key();
  fast_mc_telemetries_model.stream_params.usb_dps = STREAM_TIMESTAMP_DIM;
  fast_mc_telemetries_model.stream_params.spts = FAST_TELEMETRY_APP_MODEL_SPTS;
  fast_mc_telemetries_model.stream_params.stream_id = FAST_TELEMETRY_APP_MODEL_STREAM_ID;
  fast_mc_telemetries_model.stream_params.usb_ep = FAST_TELEMETRY_APP_MODEL_USB_EP;
  fast_mc_telemetries_model.actuatorStatus.is_active = true;

  /* USER Component initialization code */

  /* Enable fast telemetries*/
  fast_mc_telemetries_set_i_q_ref__enabled(true, NULL);
  fast_mc_telemetries_set_i_q__enabled(false, NULL);
  fast_mc_telemetries_set_i_d__enabled(false, NULL);
  fast_mc_telemetries_set_i_d_ref__enabled(false, NULL);

  app_model.ac_models[FAST_TELEMETRY_APP_MODEL_IDX] = &fast_mc_telemetries_model;
  return 0;
}
char *fast_mc_telemetries_get_key(void)
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
  *value = MCPTask_GetNumOfEnabledFastTelemetry();//(fast_mc_telemetries_model.stream_params.usb_dps - STREAM_TIMESTAMP_DIM) / 2;
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
uint8_t fast_mc_telemetries_get_sensitivity__voltage(float_t *value)
{
  *value = MCPTask_GetVoltageScaler();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t fast_mc_telemetries_get_sensitivity__current(float_t *value)
{
  *value = MCPTask_GetCurrentScaler();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t fast_mc_telemetries_get_sensitivity__frequency(float_t *value)
{
  *value = MCPTask_GetFrequencyScaler();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t fast_mc_telemetries_get_samples_per_ts(int32_t *value)
{
  uint32_t sample_per_ts = 0u;
  uint32_t num_enabled_tele = 0u;

  num_enabled_tele = MCPTask_GetNumOfEnabledFastTelemetry();
  switch (num_enabled_tele)
  {
    case 0:
    {
      sample_per_ts = 0;
    }
    break;

    case 1:
    {
      sample_per_ts = FAST_TELEMETRY_ONE_ENABLED_USB_DPS / (sizeof(uint16_t) * 1);
    }
    break;

    case 2:
    {
      sample_per_ts = FAST_TELEMETRY_TWO_ENABLED_USB_DPS / (sizeof(uint16_t) * 2);
    }
    break;

    case 3:
    {
      sample_per_ts = FAST_TELEMETRY_THREE_ENABLED_USB_DPS / (sizeof(uint16_t) * 3);
    }
    break;

    case 4:
    {
      sample_per_ts = FAST_TELEMETRY_FOUR_ENABLED_USB_DPS / (sizeof(uint16_t) * 4);
    }
    break;
  }

  *value = sample_per_ts;

  return PNPL_NO_ERROR_CODE ;
}
uint8_t fast_mc_telemetries_get_odr(int32_t *value)
{
  *value = MCPTask_GetMotorPWM();
  return PNPL_NO_ERROR_CODE ;
}
uint8_t fast_mc_telemetries_get_ioffset(float_t *value)
{
  *value = fast_mc_telemetries_model.stream_params.ioffset;
  return PNPL_NO_ERROR_CODE ;
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
uint8_t fast_mc_telemetries_set_enable(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  fast_mc_telemetries_model.actuatorStatus.is_active = value;
  if (!value)
  {
    fast_mc_telemetries_set_i_d_ref__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_i_q_ref__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_i_d__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_i_q__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_v_q__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_v_d__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_i_a__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_i_b__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_v_a__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
    fast_mc_telemetries_set_v_b__enabled(fast_mc_telemetries_model.actuatorStatus.is_active, NULL);
  }
  else
  {
    /* Get num of enabled fast telemetries */
    if (0 == MCPTask_GetNumOfEnabledFastTelemetry())
    {
      /* Enable the first one in the list */
      fast_mc_telemetries_set_i_q__enabled(true, NULL);
    }
  }

  return 0;
}
uint8_t fast_mc_telemetries_set_i_q__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_Q_MEAS_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_D_MEAS_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q_ref__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_Q_REF_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_q_ref__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d_ref__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_D_REF_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_d_ref__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_v_q__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_V_Q_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_q__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_v_d__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_V_D_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_d__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_i_a__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_A_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_a__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_i_b__enabled(bool value, char **response_message)
{

  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_I_B_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_i_b__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_v_a__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_V_A_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_a__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}
uint8_t fast_mc_telemetries_set_v_b__enabled(bool value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  update_fast_telemetry_state(MC_REG_V_B_IDX, value);
  return 0;
}
uint8_t fast_mc_telemetries_set_v_b__unit(const char *value, char **response_message)
{
  if (response_message != NULL)
  {
    *response_message = "";
  }
  return 0;
}

/* Private Function definition */
static void update_fast_telemetry_state(uint8_t fast_telemetry_idx, bool value)
{
  /* Get current slow telemetry state*/
  bool current_state = MCPTask_GetFastTelemetryState((MCPTaskFastTelemetry_idx_t) fast_telemetry_idx);

  if (value)
  {
    if (fast_mc_telemetries_model.active_telemetry_cnt < FAST_TELEMETRY_MAX_ENABLED)
    {
      if (current_state != value)
      {
        /* Enable fast telemetry */
        MCPTask_SetFastTelemetryState((MCPTaskFastTelemetry_idx_t) fast_telemetry_idx, value);

        /*Update fast telemetry counter*/
        fast_mc_telemetries_model.active_telemetry_cnt++;

        /* Check upper Level Component*/
        if (!fast_mc_telemetries_model.actuatorStatus.is_active)
        {
          fast_mc_telemetries_model.actuatorStatus.is_active = true;
        }
      }
    }
  }
  else
  {
    if (current_state != value)
    {
      /* Enable fast telemetry */
      MCPTask_SetFastTelemetryState((MCPTaskFastTelemetry_idx_t) fast_telemetry_idx, value);

      /*Update fast telemetry counter*/
      fast_mc_telemetries_model.active_telemetry_cnt--;

      /**/
      if (0 == MCPTask_GetNumOfEnabledFastTelemetry())
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

static inline void fast_mc_telemetries_configure_usb_dps(void)
{
  /* Get number of enabled fast telemetries */
  uint32_t num_of_enabled_tf = MCPTask_GetNumOfEnabledFastTelemetry();

  fast_mc_telemetries_model.stream_params.usb_dps = STREAM_TIMESTAMP_DIM;

  switch (num_of_enabled_tf)
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
  fast_mc_telemetries_model.stream_params.sd_dps = (uint32_t)(fast_mc_telemetries_model.stream_params.bandwidth * 0.33f);
  fast_mc_telemetries_model.stream_params.sd_dps = fast_mc_telemetries_model.stream_params.sd_dps - (fast_mc_telemetries_model.stream_params.sd_dps % 512)
                                                   + 512;

}

