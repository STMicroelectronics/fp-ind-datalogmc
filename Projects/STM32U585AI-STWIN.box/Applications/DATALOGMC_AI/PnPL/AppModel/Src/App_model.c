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
  * dtmi:vespucci:steval_stwinbx1:FP_IND_DATALOGMC_DatalogMC_AI;1
  *
  * Created by: DTDL2PnPL_cGen version 2.0.0
  *
  * WARNING! All changes made to this file will be lost if this is regenerated
  ******************************************************************************
  */

#include "App_model.h"
#include <string.h>
#include <stdio.h>
/* USER includes -------------------------------------------------------------*/

/* USER defines --------------------------------------------------------------*/
uint8_t algorithms_cnt = 0;
uint8_t actuators_cnt = 0;
#define SYS_DEBUGF(level, message)                SYS_DEBUGF3(SYS_DBG_DT, level, message)
/* USER private function prototypes ------------------------------------------*/

AppModel_t app_model;

AppModel_t *getAppModel(void)
{
  return &app_model;
}

uint8_t getNextAlgorithmId(uint16_t *id)
{
  if (algorithms_cnt < ALGORITHM_NUMBER)
  {
    *id = algorithms_cnt;
    algorithms_cnt++;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t getNextActuatorId(uint16_t *id)
{
  if (actuators_cnt < ACTUATOR_NUMBER)
  {
    *id = actuators_cnt;
    actuators_cnt++;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t addSensorToAppModel(uint16_t id, SensorModel_t *model)
{
  if (id < SENSOR_NUMBER)
  {
    app_model.s_models[id] = model;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t addAlgorithmToAppModel(uint16_t id, AlgorithmModel_t *model)
{
  if (id < ALGORITHM_NUMBER)
  {
    app_model.a_models[id] = model;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t addActuatorToAppModel(uint16_t id, ActuatorModel_t *model)
{
  if (id < ACTUATOR_NUMBER)
  {
    app_model.ac_models[id] = model;
    return PNPL_NO_ERROR_CODE;
  }
  return PNPL_BASE_ERROR_CODE;
}

uint8_t __stream_control(bool status)
{
  int8_t i;
  if (status) //set stream ids
  {
    int8_t j, st_i = 0;
    uint16_t proposed_fifoWM = 1;
    //sort stream id by bandwidth
    for (i = 0; i < SENSOR_NUMBER; i++)
    {
      if (app_model.s_models[i] != NULL)
      {
        if (app_model.s_models[i]->sensor_status->is_active == true)
        {
          if (app_model.s_models[i]->sensor_status->isensor_class == ISENSOR_CLASS_MEMS)
          {
            app_model.s_models[i]->stream_params.bandwidth = app_model.s_models[i]->sensor_status->type.mems.odr * SMGetnBytesPerSample(i);
          }
          else if (app_model.s_models[i]->sensor_status->isensor_class == ISENSOR_CLASS_AUDIO)
          {
            app_model.s_models[i]->stream_params.bandwidth = app_model.s_models[i]->sensor_status->type.audio.frequency * SMGetnBytesPerSample(i);
          }
          else
          {
            /* TODO: add support for other ISENSOR_CLASS */
          }

          if (app_model.log_controller_model.interface == LOG_CTRL_MODE_SD)
          {
            /* 330ms of sensor data. Access to SD is optimized when buffer dimension is multiple of 512 */
            app_model.s_models[i]->stream_params.sd_dps = (uint32_t)(app_model.s_models[i]->stream_params.bandwidth * 0.33f);
            app_model.s_models[i]->stream_params.sd_dps = app_model.s_models[i]->stream_params.sd_dps - (app_model.s_models[i]->stream_params.sd_dps % 512) + 512;

            /*********** IS IT STILL multiple of 512?  **************/
            if (app_model.s_models[i]->stream_params.sd_dps > app_model.s_models[i]->stream_params.bandwidth)
            {
              app_model.s_models[i]->stream_params.sd_dps = (uint32_t)app_model.s_models[i]->stream_params.bandwidth + 8 + 4;  /* 8 = timestamp dimension in bytes; 4 = dataloss protocol */
            }

            proposed_fifoWM = app_model.s_models[i]->stream_params.sd_dps / SMGetnBytesPerSample(i) / 2;
            if (proposed_fifoWM == 0)
            {
              proposed_fifoWM = 1;
            }
            sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
            if (ret != SYS_NO_ERROR_CODE)
            {

            }
          }
          else if (app_model.log_controller_model.interface == LOG_CTRL_MODE_USB)
          {

            /* in case of slow sensor send 1 sample for each usb packet */
            float_t low_odr = 0;
            if (app_model.s_models[i]->sensor_status->isensor_class == ISENSOR_CLASS_MEMS)
            {
              low_odr = app_model.s_models[i]->sensor_status->type.mems.odr;
            }
            else if (app_model.s_models[i]->sensor_status->isensor_class == ISENSOR_CLASS_AUDIO)
            {
              low_odr = app_model.s_models[i]->sensor_status->type.audio.frequency;
            }
            else
            {
              /* TODO: add support for other ISENSOR_CLASS */
            }

            if (low_odr <= 20)
            {
              /* When there's a timestamp, more then one packet will be sent */
              app_model.s_models[i]->stream_params.usb_dps = SMGetnBytesPerSample(i) + 8; /* 8 = timestamp dimension in bytes */
              proposed_fifoWM = 1;
              sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
              if (ret != SYS_NO_ERROR_CODE)
              {

              }
            }
            else
            {
              /* 50ms of sensor data; when there's a timestamp packets will be sent fastly */
              app_model.s_models[i]->stream_params.usb_dps = (uint32_t)(app_model.s_models[i]->stream_params.bandwidth * 0.05f);
              if (app_model.s_models[i]->stream_params.usb_dps > 7000)
              {
                app_model.s_models[i]->stream_params.usb_dps = 7000; // set a limit to avoid buffer to big
              }
              else if (app_model.s_models[i]->stream_params.usb_dps < SMGetnBytesPerSample(i) + 8)
              {
                /* In case usb_dps is a very low value, verify the setup to send at least 1 sensor data + timestamp */
                app_model.s_models[i]->stream_params.usb_dps = SMGetnBytesPerSample(i) + 8;
              }

              proposed_fifoWM = app_model.s_models[i]->stream_params.usb_dps / SMGetnBytesPerSample(i) / 2;
              if (proposed_fifoWM == 0)
              {
                proposed_fifoWM = 1;
              }
              sys_error_code_t ret = SMSensorSetFifoWM(i, proposed_fifoWM);
              if (ret != SYS_NO_ERROR_CODE)
              {

              }
            }
#ifdef SYS_DEBUG
            SensorDescriptor_t descriptor = SMSensorGetDescription(i);
            float_t ms = app_model.s_models[i]->stream_params.usb_dps / app_model.s_models[i]->stream_params.bandwidth;
            SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("**** %s, odr: %f, DPS: %d, ms: %f, proposed FIFO WM: %d \r\n", descriptor.p_name,
                                               app_model.s_models[i]->sensor_status->type.mems.odr, app_model.s_models[i]->stream_params.usb_dps, ms,
                                               proposed_fifoWM));
#endif
          }

          app_model.s_models[i]->stream_params.stream_id = st_i;
          st_i++;

          if (i > 0)
          {
            j = i - 1;
            while (j >= 0)
            {
              if (app_model.s_models[j] != NULL)
              {
                if (app_model.s_models[j]->stream_params.bandwidth <= app_model.s_models[i]->stream_params.bandwidth)
                {
                  if (app_model.s_models[j]->stream_params.stream_id >= 0)
                  {
                    if (app_model.s_models[j]->stream_params.stream_id < app_model.s_models[i]->stream_params.stream_id)
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
    for (i = 0; i < SENSOR_NUMBER; i++)
    {
      if (app_model.s_models[i] != NULL)
      {
        int8_t stream = app_model.s_models[i]->stream_params.stream_id;
        if (stream < N_MAX_EP - 2)
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
    for (i = 0; i < SENSOR_NUMBER; i++)
    {
      if (app_model.s_models[i] != NULL)
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

/* USER functions ------------------------------------------------------------*/
uint8_t set_mac_address(const char *value)
{
  strcpy(app_model.firmware_info_model.mac_address, value);
  return PNPL_NO_ERROR_CODE;
}
