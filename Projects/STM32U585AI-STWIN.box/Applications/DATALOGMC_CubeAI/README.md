## __DATALOGMC_AI Application__

The **FP-IND-DATALOGMC** function pack for **STEVAL-STWINBX1** and **EVLSPIN32G4-ACT** is a powerful integrated toolkit for the next generation of smart
actuators. It is derived from a **FP-SNS-DATALOG2** function pack and it allows the collection of heterogeneous data, combining STWIN.box sensor information with
STSPIN32G4 motor control data and it provides a comprehensive view of the system’s operational conditions. This enables both real-time monitoring and accurate
performance assessment.

EVLSPIN32G4-ACT is designed to drive a variety of three-phase brushless DC motors (not included in the kit) and ready for FOC control algorithms. See UM for
further details and installation guide. The list of supported motors is provided in the motor control SDK documentation (**X-CUBE-MCSDK-6**).

The STEVAL-STWINBX1 communicates with the STSPIN32G4 MCU via serial port (UART), using the motor control protocol (MCPv2). This protocol allows both system configuration and data logging.

FP-IND-DATALOGMC is based on application-level modules (SensorManager, TagManager, DPU digital processing units) that can be reused and extended to build
its custom application. To enable this modularity, the function pack has been built on top of eLooM, an embedded light object-oriented framework for STM32 applications
specifically designed for embedded low power applications powered by STM32. 

The **DATALOGMC_AI** example demonstrates how the NanoEdge AI Solution can be used for motor fault classification through machine learning. The machine learning model is trained using the quadrature current motor data I_q and accelerometer IIS3DWB_ACC data as input, allowing for accurate classification of motor behavior into two states - normal and fault. This example includes a portable mechanical setup that can be replicated with a 3D printer. The demo setup allows for the generation of an asymmetric load disturbance, which affects both the I_q motor current and the IIS3DWB_ACC accelerometer data.

The function pack includes a Python GUI, hsdatalogMC_GUI_AI that communicates with the STEVAL-STWINBX1 via USB allowing the user to control the motor, log sensor and motor data and display the NanoEdgeAI machine learning classification output.

The application can be controlled via Bluetooth using the **STBLESensor app** which lets you manage the board and sensor configurations, start/stop data acquisition on SD card and control data labelling.

This firmware package includes Components Device Drivers, Board Support Package and example applications 
for the STWIN.box platform (STEVAL-STWINBX1).  

### __Keywords__

Datalog, Motor Control, NanoEdgeAI Studio, Machine Learning, Predictive Maintenance, Condition Monitoring, Industry 4.0, Sensor


### __Hardware and Software environment__

- This example runs on STMU5 Devices.

- This example has been tested with STMicroelectronics STEVAL-STWINBX1 evaluation boards and can be easily tailored to any other supported device and development board. 


### __How to use it?__

In order to make the program work, you must do the following:

- Open your preferred toolchain
 
- Rebuild all files and load your image into target memory

- Run the example

