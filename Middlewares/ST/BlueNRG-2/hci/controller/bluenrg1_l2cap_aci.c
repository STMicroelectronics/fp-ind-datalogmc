/**
  ******************************************************************************
  * @file    bluenrg1_l2cap_aci.c
  * @author  AMG - RF Application team
  * @brief   Source file for external uC - BlueNRG-x in network coprocessor mode
  *          (l2cap_aci)
  *          Autogenerated files, do not edit!!
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "ble_types.h"
#include "bluenrg1_l2cap_aci.h"
tBleStatus aci_l2cap_connection_parameter_update_req(uint16_t Connection_Handle,
                                                     uint16_t Conn_Interval_Min,
                                                     uint16_t Conn_Interval_Max,
                                                     uint16_t Slave_latency,
                                                     uint16_t Timeout_Multiplier)
{
  struct hci_request rq;
  uint8_t cmd_buffer[258];
  aci_l2cap_connection_parameter_update_req_cp0 *cp0 = (aci_l2cap_connection_parameter_update_req_cp0 *)(cmd_buffer);
  tBleStatus status = 0;
  uint8_t index_input = 0;
  cp0->Connection_Handle = htob(Connection_Handle, 2);
  index_input += 2;
  cp0->Conn_Interval_Min = htob(Conn_Interval_Min, 2);
  index_input += 2;
  cp0->Conn_Interval_Max = htob(Conn_Interval_Max, 2);
  index_input += 2;
  cp0->Slave_latency = htob(Slave_latency, 2);
  index_input += 2;
  cp0->Timeout_Multiplier = htob(Timeout_Multiplier, 2);
  index_input += 2;
  BLUENRG_memset(&rq, 0, sizeof(rq));
  rq.ogf = 0x3f;
  rq.ocf = 0x181;
  rq.event = 0x0F;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(&rq, FALSE) < 0)
  {
    return BLE_STATUS_TIMEOUT;
  }
  if (status)
  {
    return status;
  }
  return BLE_STATUS_SUCCESS;
}
tBleStatus aci_l2cap_connection_parameter_update_resp(uint16_t Connection_Handle,
                                                      uint16_t Conn_Interval_Min,
                                                      uint16_t Conn_Interval_Max,
                                                      uint16_t Slave_latency,
                                                      uint16_t Timeout_Multiplier,
                                                      uint16_t Minimum_CE_Length,
                                                      uint16_t Maximum_CE_Length,
                                                      uint8_t Identifier,
                                                      uint8_t Accept)
{
  struct hci_request rq;
  uint8_t cmd_buffer[258];
  aci_l2cap_connection_parameter_update_resp_cp0 *cp0 = (aci_l2cap_connection_parameter_update_resp_cp0 *)(cmd_buffer);
  tBleStatus status = 0;
  uint8_t index_input = 0;
  cp0->Connection_Handle = htob(Connection_Handle, 2);
  index_input += 2;
  cp0->Conn_Interval_Min = htob(Conn_Interval_Min, 2);
  index_input += 2;
  cp0->Conn_Interval_Max = htob(Conn_Interval_Max, 2);
  index_input += 2;
  cp0->Slave_latency = htob(Slave_latency, 2);
  index_input += 2;
  cp0->Timeout_Multiplier = htob(Timeout_Multiplier, 2);
  index_input += 2;
  cp0->Minimum_CE_Length = htob(Minimum_CE_Length, 2);
  index_input += 2;
  cp0->Maximum_CE_Length = htob(Maximum_CE_Length, 2);
  index_input += 2;
  cp0->Identifier = htob(Identifier, 1);
  index_input += 1;
  cp0->Accept = htob(Accept, 1);
  index_input += 1;
  BLUENRG_memset(&rq, 0, sizeof(rq));
  rq.ogf = 0x3f;
  rq.ocf = 0x182;
  rq.cparam = cmd_buffer;
  rq.clen = index_input;
  rq.rparam = &status;
  rq.rlen = 1;
  if (hci_send_req(&rq, FALSE) < 0)
  {
    return BLE_STATUS_TIMEOUT;
  }
  if (status)
  {
    return status;
  }
  return BLE_STATUS_SUCCESS;
}
