/**
  ******************************************************************************
  * @file    App_model_Tags_Info.h
  * @author  SRA
  * @brief   Tags_Info PnPL Components APIs
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
  * dtmi:vespucci:other:tags_info;2
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

#ifndef APP_MODEL_TAGS_INFO_H_
#define APP_MODEL_TAGS_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* USER includes -------------------------------------------------------------*/
#define HSD_SW_TAG_CLASS_NUM 5U
#define HSD_HW_TAG_CLASS_NUM 5U

typedef struct _TagsInfoModel_t
{
  char *comp_name;
  /* TagsInfo Component Model USER code */
} TagsInfoModel_t;

/* Tags Information PnPL Component -------------------------------------------- */
uint8_t tags_info_comp_init(void);
char *tags_info_get_key(void);
uint8_t tags_info_get_max_tags_num(int32_t *value);
uint8_t tags_info_get_sw_tag0__label(char **value);
uint8_t tags_info_get_sw_tag0__enabled(bool *value);
uint8_t tags_info_get_sw_tag0__status(bool *value);
uint8_t tags_info_get_sw_tag1__label(char **value);
uint8_t tags_info_get_sw_tag1__enabled(bool *value);
uint8_t tags_info_get_sw_tag1__status(bool *value);
uint8_t tags_info_get_sw_tag2__label(char **value);
uint8_t tags_info_get_sw_tag2__enabled(bool *value);
uint8_t tags_info_get_sw_tag2__status(bool *value);
uint8_t tags_info_get_sw_tag3__label(char **value);
uint8_t tags_info_get_sw_tag3__enabled(bool *value);
uint8_t tags_info_get_sw_tag3__status(bool *value);
uint8_t tags_info_get_sw_tag4__label(char **value);
uint8_t tags_info_get_sw_tag4__enabled(bool *value);
uint8_t tags_info_get_sw_tag4__status(bool *value);
uint8_t tags_info_set_sw_tag0__label(const char *value);
uint8_t tags_info_set_sw_tag0__enabled(bool value);
uint8_t tags_info_set_sw_tag0__status(bool value);
uint8_t tags_info_set_sw_tag1__label(const char *value);
uint8_t tags_info_set_sw_tag1__enabled(bool value);
uint8_t tags_info_set_sw_tag1__status(bool value);
uint8_t tags_info_set_sw_tag2__label(const char *value);
uint8_t tags_info_set_sw_tag2__enabled(bool value);
uint8_t tags_info_set_sw_tag2__status(bool value);
uint8_t tags_info_set_sw_tag3__label(const char *value);
uint8_t tags_info_set_sw_tag3__enabled(bool value);
uint8_t tags_info_set_sw_tag3__status(bool value);
uint8_t tags_info_set_sw_tag4__label(const char *value);
uint8_t tags_info_set_sw_tag4__enabled(bool value);
uint8_t tags_info_set_sw_tag4__status(bool value);



#ifdef __cplusplus
}
#endif

#endif /* APP_MODEL_TAGS_INFO_H_ */