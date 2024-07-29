/**
  ******************************************************************************
  * @file    register_interface.h
  * @author  SRA
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef REGISTER_INTERFACE_H
#define REGISTER_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*
   MCP_ID definition :
   | Element Identifier 10 bits  |  Type  | Motor #|
   |                             |        |        |
   |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|

   Type definition :
   0	Data Segment End
   1	8-bit data
   2	16-bit data
   3	32-bit data
   4	Character string
   5	Raw Structure
   6	Flag
   7	Data Segment Begin
*/

#define ELT_IDENTIFIER_POS 6
#define TYPE_POS 3
#define TYPE_MASK 0x38
#define MOTOR_MASK 0x7
#define REG_MASK 0xFFF8

#define MOTOR_ID_ONE	 (0x01)
#define MOTOR_ID_GLOBAL  (0x00)

#define TYPE_DATA_8BIT    (1 << TYPE_POS)
#define TYPE_DATA_16BIT   (2 << TYPE_POS)
#define TYPE_DATA_32BIT   (3 << TYPE_POS)
#define TYPE_DATA_STRING  (4 << TYPE_POS)
#define TYPE_DATA_RAW     (5 << TYPE_POS)
#define TYPE_DATA_FLAG    (6 << TYPE_POS)

/* TYPE_DATA_8BIT registers definition */
#define  MC_REG_STATUS                 (((1  << ELT_IDENTIFIER_POS) | TYPE_DATA_8BIT ) | MOTOR_ID_ONE)


/* TYPE_DATA_16BIT registers definition */
#define  MC_REG_SPEED_KP                 (((2U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_SPEED_KI                 (((3U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_SPEED_KD                 (((4U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KP                   (((6U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KI                   (((7U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KD                   (((8U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_D_KP                   (((10U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_D_KI                   (((11U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_D_KD                   (((12U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_C1                (((13U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_C2                (((14U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_C1             (((15U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_C2             (((16U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_KI                (((17U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_KP                (((18U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_KP                (((19U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_KI                (((20U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_BUS               (((21U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_BUS_VOLTAGE              (((22U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_HEATS_TEMP               (((23U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_DAC_OUT1                 (((25U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_GLOBAL)
#define  MC_REG_DAC_OUT2                 (((26U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_GLOBAL)
#define  MC_REG_DAC_OUT3                 (((27 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_BUS_MEAS          (((30U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_A                      (((31U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_B                      (((32U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_ALPHA_MEAS             (((33U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_BETA_MEAS              (((34U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_Q_MEAS                 (((35U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_D_MEAS                 (((36U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_Q_REF                  (((37U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_I_D_REF                  (((38U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_V_Q                      (((39U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_V_D                      (((40U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_V_ALPHA                  (((41U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_V_BETA                   (((42U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_ENCODER_EL_ANGLE         (((43 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_ENCODER_SPEED            (((44 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_EL_ANGLE          (((45U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_ROT_SPEED         (((46U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_I_ALPHA           (((47U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_I_BETA            (((48U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_BEMF_ALPHA        (((49U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_BEMF_BETA         (((50U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_EL_ANGLE       (((51 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_ROT_SPEED      (((52 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_I_ALPHA        (((53 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_I_BETA         (((54 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_BEMF_ALPHA     (((55 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOCORDIC_BEMF_BETA      (((56 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_DAC_USER1                (((57U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_DAC_USER2                (((58U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_HALL_EL_ANGLE            (((59 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_HALL_SPEED               (((60 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_FF_VQ                    (((62U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FF_VD                    (((63U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FF_VQ_PIOUT              (((64U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FF_VD_PIOUT              (((65U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_DCBUS_REF            (((66U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_DCBUS_MEAS           (((67U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_ACBUS_FREQ           (((68U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_ACBUS_RMS            (((69U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KP                 (((70U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KI                 (((71U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KD                 (((72U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KP                 (((73U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KI                 (((74U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KD                 (((75U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PFC_STARTUP_DURATION     (((76U << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_PWM_FREQUENCY         (((77 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KP              (((78 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KI              (((79 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KD              (((80 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_SPEED_KP_DIV             (((81 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_SPEED_KI_DIV             (((82 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_SPEED_KD_DIV             (((83 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_D_KP_DIV               (((84 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_D_KI_DIV               (((85 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_D_KD_DIV               (((86 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KP_DIV               (((87 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KI_DIV               (((88 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_I_Q_KD_DIV               (((89 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KP_DIV          (((90 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KI_DIV          (((91 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_POSITION_KD_DIV          (((92 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KP_DIV             (((93 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KI_DIV             (((94 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_I_KD_DIV             (((95 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KP_DIV             (((96 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KI_DIV             (((97 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_PFC_V_KD_DIV             (((98 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_KI_DIV            (((99 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT)  | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_KP_DIV            (((100 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_KP_DIV            (((101 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FLUXWK_KI_DIV            (((102 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_STARTUP_CURRENT_REF      (((105 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_PULSE_VALUE              (((106 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_FOC_VQREF                (((107 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_ZCR                 (((108 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_U                   (((109 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_V                   (((110 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_W                   (((111 << ELT_IDENTIFIER_POS) | TYPE_DATA_16BIT) | MOTOR_ID_ONE)
#define  MC_REG_OVERVOLTAGETHRESHOLD     (((112U << ELT_IDENTIFIER_POS)| TYPE_DATA_16BIT )| MOTOR_ID_ONE)
#define  MC_REG_UNDERVOLTAGETHRESHOLD    (((113U << ELT_IDENTIFIER_POS)| TYPE_DATA_16BIT )| MOTOR_ID_ONE)

/* TYPE_DATA_32BIT registers definition */
#define  MC_REG_FAULTS_FLAGS             (((0 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SPEED_MEAS               (((1 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SPEED_REF                (((2 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_STOPLL_EST_BEMF          (((3 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_STOPLL_OBS_BEMF          (((4 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_STOCORDIC_EST_BEMF       (((5 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_STOCORDIC_OBS_BEMF       (((6 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_FF_1Q                    (((7 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_FF_1D                    (((8 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_FF_2                     (((9 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)/* To check shifted by >> 16*/
#define  MC_REG_PFC_FAULTS               (((40 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_CURRENT_POSITION         (((41 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_RS                    (((91 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_LS                    (((92 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_KE                    (((93 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_VBUS                  (((94 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_MEAS_NOMINALSPEED     (((95 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_CURRENT               (((96 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_SPDBANDWIDTH          (((97 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_LDLQRATIO             (((98 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_NOMINAL_SPEED         (((99 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) | MOTOR_ID_ONE)
#define  MC_REG_SC_CURRBANDWIDTH         (((100 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_SC_J                     (((101 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_SC_F                     (((102 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_SC_MAX_CURRENT           (((103 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_SC_STARTUP_SPEED         (((104 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_SC_STARTUP_ACC           (((105 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)
#define  MC_REG_RESISTOR_OFFSET          (((116 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT)| MOTOR_ID_ONE)

#define  MC_REG_MOTOR_POWER              (((109 << ELT_IDENTIFIER_POS) | TYPE_DATA_32BIT) |MOTOR_ID_ONE)

#define  MC_REG_FW_NAME                  (((0U << ELT_IDENTIFIER_POS) | TYPE_DATA_STRING) | MOTOR_ID_GLOBAL)
#define  MC_REG_CTRL_STAGE_NAME          (((1U << ELT_IDENTIFIER_POS) | TYPE_DATA_STRING) | MOTOR_ID_ONE)
#define  MC_REG_PWR_STAGE_NAME           (((2U << ELT_IDENTIFIER_POS) | TYPE_DATA_STRING) | MOTOR_ID_GLOBAL)
#define  MC_REG_MOTOR_NAME               (((3U << ELT_IDENTIFIER_POS) | TYPE_DATA_STRING) | MOTOR_ID_ONE)

#define  MC_REG_GLOBAL_CONFIG            (((0U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_GLOBAL)
#define  MC_REG_MOTOR_CONFIG             (((1U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_APPLICATION_CONFIG       (((2U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_FOCFW_CONFIG             (((3U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_SCALE_CONFIG             (((4U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_SPEED_RAMP               (((6U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_TORQUE_RAMP              (((7U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_REVUP_DATA               (((8U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)  /* Configure all steps*/
#define  MC_REG_CURRENT_REF              (((13U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW)| MOTOR_ID_ONE)
#define  MC_REG_POSITION_RAMP            (((14U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW)| MOTOR_ID_ONE)
#define  MC_REG_ASYNC_UARTA              (((20U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_ASYNC_UARTB              (((21U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_ASYNC_STLNK              (((22U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_HT_HEW_PINS              (((28U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_HT_CONNECTED_PINS        (((29U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_HT_PHASE_SHIFT           (((30U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_ADC_CONFIG          (((31U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)
#define  MC_REG_BEMF_ONTIME_ADC_CONFIG   (((32U << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW) | MOTOR_ID_ONE)

/* TYPE_DATA_RAW registers definition */
#define  MC_REG_GLOBAL_CFG            (((0  << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW ) | MOTOR_ID_ONE)
#define  MC_REG_MOTOR_CFG             (((1  << ELT_IDENTIFIER_POS) | TYPE_DATA_RAW ) | MOTOR_ID_ONE)

uint8_t RI_SetRegCommandParser (MCP_Handle_t * pHandle);
uint8_t RI_GetRegCommandParser (MCP_Handle_t * pHandle);
uint8_t RI_GetPtrReg (uint16_t dataID, void ** dataPtr);
uint8_t RI_GetIDSize (uint16_t ID);

#ifdef __cplusplus
}
#endif

#endif /*REGISTER_INTERFACE_H*/
