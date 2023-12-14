# *****************************************************************************
#  * @file    Controller.py
#  * @author  SRA
# ******************************************************************************
# * @attention
# *
# * Copyright (c) 2022 STMicroelectronics.
# * All rights reserved.
# *
# * This software is licensed under terms that can be found in the LICENSE file
# * in the root directory of this software component.
# * If no LICENSE file comes with this software, it is provided AS-IS.
# *
# *
# ******************************************************************************
#

import os
import time
from threading import Event

from enum import Enum

from PySide6.QtCore import Signal
from st_dtdl_gui.Utils.DataClass import LinesPlotParams, MCTelemetriesPlotParams, PlotCheckBoxParams, PlotGaugeParams, PlotLabelParams, PlotLevelParams
from st_hsdatalog.HSD.utils.type_conversion import TypeConversion
from st_hsdatalog.HSD_GUI.HSD_Controller import HSD_Controller
import st_pnpl.DTDL.dtdl_utils as DTDLUtils

from st_pnpl.PnPLCmd import PnPLCMDManager

from st_hsdatalog.HSD_link.HSDLink_v1 import HSDLink_v1
from st_dtdl_gui.Utils.DataReader import DataReader
from st_dtdl_gui.STDTDL_Controller import ComponentType

import st_hsdatalog.HSD_utils.logger as logger
log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class FastTelemetryStateEnum(Enum):
    MCP_FT_DISABLE = 0
    MCP_FT_ENABLE = 1
     

class HSD_MC_Controller(HSD_Controller):
    
    sig_is_motor_started = Signal(bool, int)
    sig_motor_fault_raised = Signal()
    sig_motor_fault_acked = Signal()
    
    def __init__(self, parent=None):
        super().__init__(parent)

        # @mcp_is_connected saves the connection state between 
        # mcp master and slave
        self.mcp_is_connected = False

        # @is_motor_started saves motor state
        self.is_motor_started = False

        self.mcp_fast_telemetries_state = FastTelemetryStateEnum.MCP_FT_DISABLE

        self.mc_comp_name = "motor_controller"
        self.mc_start_cmd_name = "start_motor"
        self.mc_stop_cmd_name = "stop_motor"
        self.mc_ack_fault_cmd_name = "ack_fault"
        self.mc_motor_speed_prop_name = "motor_speed"
        self.mc_speed_req_name = "speed"

    def get_description_string(content):
        if content.description is not None:
            return content.description if isinstance(content.description, str) else content.description.en
        return None

    def stop_log(self, interface=1):
        if self.is_logging == True:
            self.hsd_link.stop_log(self.device_id)
            if type(self.hsd_link) == HSDLink_v1:
                self.hsd_link.save_json_device_file(self.device_id)
                self.hsd_link.save_json_acq_info_file(self.device_id)
            else:
                #TODO put here a "File saving..." loading window!
                self.hsd_link.save_json_acq_info_file(self.device_id)
                time.sleep(0.5)
                self.hsd_link.save_json_device_file(self.device_id)
                if self.ispu_output_format is not None:
                    self.save_json_ispu_output_format_file()
            self.sig_logging.emit(False, interface)
            self.is_logging = False

    def start_plots(self):        
        for s in self.plot_widgets:
            s_plot = self.plot_widgets[s]
            
            if type(self.hsd_link) == HSDLink_v1:
                    sensor_data_file_path = os.path.join(self.hsd_link.get_acquisition_folder(),(str(s_plot.comp_name) + ".dat"))
                    sensor_data_file = open(sensor_data_file_path, "wb+")
                    self.sensor_data_files.append(sensor_data_file)
                    stopFlag = Event()
                    self.threads_stop_flags.append(stopFlag)

                    dimensions = s_plot.n_curves
                    sample_size = s_plot.sample_size
                    spts = s_plot.spts
                    data_format = s_plot.data_format
                    
                    dr = DataReader(self, s_plot.comp_name, spts, dimensions, sample_size, data_format)
                    self.data_readers.append(dr)
                    
                    thread = self.SensorAcquisitionThread_test_v1(stopFlag, self.hsd_link, dr, self.device_id, s_plot.s_id, s_plot.ss_id, s_plot.comp_name, sensor_data_file)
                    thread.start()
                    self.sensors_threads.append(thread)
            else:
                c_status = self.hsd_link.get_component_status(self.device_id, s_plot.comp_name)
                if c_status is not None:
                    c_status_value = c_status[s_plot.comp_name]
                    
                    c_enable = c_status_value.get("enable")
                    
                    if c_enable == True:
                        sensor_data_file_path = os.path.join(self.hsd_link.get_acquisition_folder(),(str(s_plot.comp_name) + ".dat"))
                        sensor_data_file = open(sensor_data_file_path, "wb+")
                        self.sensor_data_files.append(sensor_data_file)
                        stopFlag = Event()
                        self.threads_stop_flags.append(stopFlag)

                        if "sensitivity" in c_status_value:
                            sensitivity = c_status_value["sensitivity"]
                        else:
                            sensitivity = 1
                        
                        sample_size = TypeConversion.check_type_length(c_status_value["data_type"])
                        data_format = TypeConversion.get_format_char(c_status_value["data_type"])
                        usb_dps = c_status_value["usb_dps"]

                        interleaved_data = True
                        if c_status_value["c_type"] == ComponentType.SENSOR.value:
                            spts = c_status_value.get("samples_per_ts")
                            dimensions = c_status_value["dim"]
                            
                        elif c_status_value["c_type"] == ComponentType.ALGORITHM.value:
                            spts = 0
                            dimensions = c_status_value["dim"]
                            if "algorithm_type" in c_status_value:
                                algorithm_type = c_status_value["algorithm_type"]
                            if algorithm_type == 0 or algorithm_type == 1:
                                interleaved_data = False
                        
                        elif c_status_value["c_type"] == ComponentType.ACTUATOR.value:
                            spts = 1
                            dimensions = 0

                            if s_plot.comp_name == DTDLUtils.MC_SLOW_TELEMETRY_COMP_NAME:
                                # Get slow telemetries content:
                                slow_telemetries_contents = c_status[DTDLUtils.MC_SLOW_TELEMETRY_COMP_NAME]
                                # Get slow telemetries sub properties
                                slow_telemetries_sub_properties = slow_telemetries_contents[DTDLUtils.ST_BLE_STREAM]
                                for t in slow_telemetries_sub_properties:
                                    if t != 'id':
                                    # Check enable condition
                                        t_enabled = slow_telemetries_sub_properties[t].get("enable")
                                        if t_enabled:
                                            self.plot_widgets[s_plot.comp_name].plots_params.plots_params_dict[t].enabled = t_enabled
                                            if t_enabled:
                                                dimensions += 1
                            
                            elif s_plot.comp_name == DTDLUtils.MC_FAST_TELEMETRY_COMP_NAME:
                                fast_telemetries_contents = [ftc for ftc in self.components_dtdl[s_plot.comp_name].contents if HSD_MC_Controller.get_description_string(ftc) == DTDLUtils.MC_FAST_TELEMETRY_STRING]
                                for t in fast_telemetries_contents:
                                    t_display_name = t.display_name if isinstance(t.display_name, str) else t.display_name.en
                                    t_enabled = c_status_value[t.name].get("enabled")
                                    self.plot_widgets[s_plot.comp_name].plots_params.plots_params_dict[t_display_name].enabled = t_enabled

                                dimensions = c_status_value["dim"]
                                interleaved_data = False

                        if "_ispu" in s_plot.comp_name:
                            data_format = "b"
                            dimensions = 64
                            sample_size = 1

                        dr = DataReader(self, s_plot.comp_name, spts, dimensions, sample_size, data_format, sensitivity, interleaved_data)
                        self.data_readers.append(dr)
                    
                        thread = self.SensorAcquisitionThread(stopFlag, self.hsd_link, dr, self.device_id, s_plot.comp_name, sensor_data_file, usb_dps)
                        thread.start()
                        self.sensors_threads.append(thread)
    
    
    def start_motor(self, motor_id = 0):
        # Send Start motor cmd
        self.send_command(PnPLCMDManager.create_command_cmd(self.mc_comp_name, self.mc_start_cmd_name))
        #Emit signal
        self.sig_is_motor_started.emit(True, motor_id)
    
    def stop_motor(self, motor_id = 0):
        #Send stop motor message
        res = self.send_command(PnPLCMDManager.create_command_cmd(self.mc_comp_name, self.mc_stop_cmd_name))
        #Emit signal
        self.sig_is_motor_started.emit(False, motor_id)
        return res
    
    def ack_fault(self, motor_id = 0):
        res = self.send_command(PnPLCMDManager.create_command_cmd(self.mc_comp_name, self.mc_ack_fault_cmd_name))
        time.sleep(0.7)
        stop_res = self.stop_motor()
        if res is not None and stop_res is not None:
            self.sig_motor_fault_acked.emit()
        
    def set_motor_speed(self, value, motor_id = 0):
        self.send_command(PnPLCMDManager.create_set_property_cmd(self.mc_comp_name, self.mc_motor_speed_prop_name, value))
    
    def get_plot_params(self, comp_name, comp_type, comp_interface, comp_status):
        ret = super().get_plot_params(comp_name, comp_type, comp_interface, comp_status)
        if comp_type.name == ComponentType.ACTUATOR.name:
            plot_params_dict = {}
            comp_enabled = comp_status[comp_name].get("enable")
            if comp_name == DTDLUtils.MC_SLOW_TELEMETRY_COMP_NAME:
                st_ble_stream_components = comp_status[DTDLUtils.MC_SLOW_TELEMETRY_COMP_NAME].get(DTDLUtils.ST_BLE_STREAM)
                if st_ble_stream_components is not None:
                    for c in st_ble_stream_components.keys():
                        if c == "temperature":
                            t_enabled = st_ble_stream_components[c].get("enable")
                            t_unit  = st_ble_stream_components[c].get("unit")
                            plot_params_dict["temperature"] = PlotLabelParams("temperature", t_enabled, 0, 0, t_unit) # label
                        elif c == "ref_speed":
                            t_enabled = st_ble_stream_components[c].get("enable")
                            t_unit  = st_ble_stream_components[c].get("unit")
                            plot_params_dict["ref_speed"] = PlotLabelParams("ref_speed", t_enabled, 0, 0, t_unit) # label
                        elif c == "bus_voltage":
                            t_enabled = st_ble_stream_components[c].get("enable")
                            t_unit  = st_ble_stream_components[c].get("unit")
                            plot_params_dict["bus_voltage"] = PlotLabelParams("bus_voltage", t_enabled, 0, 0, t_unit) # label
                        elif c == "speed":
                            t_enabled = st_ble_stream_components[c].get("enable")
                            t_unit  = st_ble_stream_components[c].get("unit")
                            plot_params_dict["speed"] = PlotGaugeParams("speed", t_enabled, -4000, 4000, t_unit)
                        elif c == "fault":
                            t_enabled = st_ble_stream_components[c].get("enable")
                            plot_params_dict["fault"] = PlotCheckBoxParams("fault", t_enabled, ['No Error', 'FOC Duration', 'Over Voltage', 'Under Voltage', 'Over Heat', 'Start Up failure', 'Speed Feedback', 'Over Current', 'Software Error' ]) # label #TODO ENUM in DTDL
            elif comp_name == DTDLUtils.MC_FAST_TELEMETRY_COMP_NAME:
                contents = comp_interface.contents
                description = None
                for c in contents:
                    if c.description is not None:
                        description = c.description if isinstance(c.description, str) else c.description.en
                        display_name = c.display_name if isinstance(c.display_name, str) else c.display_name.en
                        t_root_key = list(comp_status.keys())[0]
                        if description == DTDLUtils.MC_FAST_TELEMETRY_STRING:
                            if c.name in comp_status[t_root_key]:
                                tele_status = comp_status[t_root_key][c.name]
                                t_enabled = tele_status[DTDLUtils.ENABLED_STRING]
                                t_unit = tele_status[DTDLUtils.UNIT_STRING]
                                plot_params_dict[display_name] = LinesPlotParams(c.name, t_enabled, 1, t_unit)
            ret = MCTelemetriesPlotParams(comp_name, comp_enabled, plot_params_dict)
        return ret