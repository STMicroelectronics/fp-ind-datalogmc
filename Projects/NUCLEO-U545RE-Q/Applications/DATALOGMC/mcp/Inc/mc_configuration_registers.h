
/**
  ******************************************************************************
  * @file    mc_configuration_registers.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file provides the definitions needed to build the project
  *          configuration information registers.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef MC_CONFIGURATION_REGISTERS_H
#define MC_CONFIGURATION_REGISTERS_H

typedef struct
{
  float_t voltage;
  float_t current;
  float_t frequency;
} scaleFlashParams_t;

typedef struct
{
  uint32_t SDKVersion;
  uint8_t MotorNumber;
  uint8_t MCP_Flag;
  uint8_t MCPA_UARTA_LOG;
  uint8_t MCPA_UARTB_LOG;
  uint8_t MCPA_STLNK_LOG;
  uint8_t Padding;
} __attribute__((packed)) GlobalConfig_reg_t;

typedef struct
{
  float_t polePairs;
  float_t ratedFlux;
  float_t rs;
  float_t rsSkinFactor;
  float_t ls;
  float_t ld;
  float_t maxCurrent;
  float_t mass_copper_kg;
  float_t cooling_tau_s;
  char name[24];
} __attribute__((packed)) MotorConfig_reg_t;

typedef  struct
{
  uint32_t maxMechanicalSpeed;
  float_t maxReadableCurrent;
  float_t nominalCurrent;
  uint16_t nominalVoltage;
  uint8_t driveType;
  uint8_t padding;
} __attribute__((packed)) ApplicationConfig_reg_t;

typedef struct
{
  uint8_t primarySensor;
  uint8_t auxiliarySensor;
  uint8_t topology;
  uint8_t FOCRate;
  uint32_t PWMFrequency;
  uint16_t MediumFrequency;
  uint16_t configurationFlag1;
  uint16_t configurationFlag2;
} __attribute__((packed)) FOCFwConfig_reg_t;

#endif /* MC_CONFIGURATION_REGISTERS_H */
/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
