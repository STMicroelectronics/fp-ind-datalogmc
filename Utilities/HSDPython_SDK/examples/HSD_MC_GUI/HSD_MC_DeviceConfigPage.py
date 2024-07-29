# *****************************************************************************
#  * @file    DeviceConfigPage.py
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

from PySide6.QtCore import Slot
from HSD_MC_GUI.Widgets.HSD_MC_ClassifierOutputWidget import HSD_MC_ClassifierOutputWidget
from HSD_MC_GUI.Widgets.HSD_MC_FastTelemetriesPlotWidget import HSD_MC_FastTelemetriesPlotWidget
from HSD_MC_GUI.Widgets.HSD_MC_ControlWidget import HSD_MC_ControlWidget
from HSD_MC_GUI.Widgets.HSD_MC_SlowTelemetriesPlotWidget import HSD_MC_SlowTelemetriesPlotWidget
from HSD_MC_GUI.Widgets.HSD_MC_LogControlWidget import HSD_MC_LogControlWidget
from st_hsdatalog.HSD_GUI.Widgets.HSDComponentWidget import HSDComponentWidget

from st_hsdatalog.HSD_GUI.HSD_DeviceConfigPage import HSD_DeviceConfigPage
from st_dtdl_gui.STDTDL_Controller import ComponentType
import st_pnpl.DTDL.dtdl_utils as DTDLUtils

import st_hsdatalog.HSD_utils.logger as logger
log = logger.setup_applevel_logger(is_debug = False, file_name= "app_debug.log")

class HSD_MC_DeviceConfigPage(HSD_DeviceConfigPage):
    
    def __init__(self, page_widget, controller):
        super().__init__(page_widget, controller)
        self.slow_mc_telemetries = []
        self.fast_mc_telemetries = []
    
    @Slot(str, dict)
    def s_component_found(self, comp_name, comp_interface):
        super().s_component_found(comp_name, comp_interface)
        #Replace the Motor Controller ComponentWidget
        if comp_name == "motor_controller":
            self.controller.remove_component_config_widget(comp_name)
            self.motor_control_widget = HSD_MC_ControlWidget(self.controller, comp_contents=comp_interface.contents, parent=self.widget_header)
            self.controller.add_component_config_widget(self.motor_control_widget)
            self.add_header_widget(self.motor_control_widget)
            self.controller.fill_component_status(comp_name)
        if comp_name == "log_controller":
            self.controller.remove_component_config_widget(comp_name)
            c_status = self.controller.get_component_status(comp_name)
            if "controller_type" in c_status["log_controller"]:
                self.log_control_widget = HSD_MC_LogControlWidget(self.controller, comp_contents=comp_interface.contents, parent=self.widget_header)
                self.controller.set_rtc_time()
                self.controller.add_component_config_widget(self.log_control_widget)
                self.add_header_widget(self.log_control_widget)
                self.controller.fill_component_status(comp_name) 

    @Slot(str, dict)
    def s_actuator_component_found(self, comp_name, comp_interface):
        comp_display_name = comp_interface.display_name if isinstance(comp_interface.display_name, str) else comp_interface.display_name.en
        super().s_actuator_component_found(comp_name, comp_interface)

        comp_status = self.controller.get_component_status(comp_name)
        plot_params = self.controller.get_plot_params(comp_name, ComponentType.ACTUATOR, comp_interface, comp_status)

        if comp_name == DTDLUtils.MC_SLOW_TELEMETRY_COMP_NAME:
            actuator_plot_widget = HSD_MC_SlowTelemetriesPlotWidget(self.controller, comp_name, comp_display_name, plot_params, 30, p_id=self.graph_id, parent=self.plots_widget)
        
        elif comp_name == DTDLUtils.MC_FAST_TELEMETRY_COMP_NAME:
            actuator_plot_widget = HSD_MC_FastTelemetriesPlotWidget(self.controller, comp_name, comp_display_name, plot_params, 30, p_id=self.graph_id, parent=self.plots_widget)
            
        self.controller.add_plot_widget(actuator_plot_widget)
        self.plots_widget.layout().addWidget(actuator_plot_widget)
        actuator_plot_widget.setVisible(plot_params.enabled)
        
        self.graph_id +=1
    
    @Slot(str, dict)
    def s_algorithm_component_found(self, comp_name, comp_interface):
        if comp_name == "ai_motor_classifier":
            comp_display_name = comp_interface.display_name if isinstance(comp_interface.display_name, str) else comp_interface.display_name.en
            
            alg_config_widget = HSDComponentWidget(self.controller, comp_name, comp_display_name, ComponentType.ALGORITHM, comp_interface.contents, self.comp_id, self.device_config_widget)
            self.comp_id += 1
            self.controller.add_component_config_widget(alg_config_widget)
            self.device_config_widget.layout().addWidget(alg_config_widget)
        
            comp_display_name = comp_interface.display_name if isinstance(comp_interface.display_name, str) else comp_interface.display_name.en
            out_classes = self.controller.get_output_classes()
            ai_tool = self.controller.get_ai_classifier_tool()
            alg_plot_widget = HSD_MC_ClassifierOutputWidget(self.controller, comp_name, comp_display_name, out_classes=out_classes, ai_tool=ai_tool, p_id=self.graph_id, parent=self.plots_widget)
            self.graph_id +=1
            self.controller.add_plot_widget(alg_plot_widget)
            self.plots_widget.layout().addWidget(alg_plot_widget)
            alg_plot_widget.setVisible(True)
        self.controller.fill_component_status(comp_name)
