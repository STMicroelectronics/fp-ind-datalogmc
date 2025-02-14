# FP-IND-DATALOGMC Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/fp-ind-datalogmc.svg?color=brightgreen)

The **FP-IND-DATALOGMC** function pack for **STEVAL-STWINBX1** and **EVLSPIN32G4-ACT** is a powerful integrated toolkit for the next generation 
of smart actuators. This toolkit is derived from an **FP-SNS-DATALOG2** function pack and allows the collection of heterogeneous data, 
combining STWIN.box sensor information with STSPIN32G4 motor control data. It provides a comprehensive view of the system’s 
operational conditions, enabling both real-time monitoring and accurate performance assessment.

Version 1.1.0 introduces a further firmware example called **DATALOGMC_AI** that implements motor fault classification based on a 
machine learning solution developed through **NanoEdgeAIStudio**. The machine learning model allows accurate classification of motor 
behavior into two states: good and faulty.

Users can also customize the model by adding their own classes, which can be achieved by incorporating additional data and modifying 
the training process. The package also includes a portable mechanical setup that can be replicated with a 3D printer.

EVLSPIN32G4-ACT is designed to drive a variety of three-phase brushless DC motors (not included in the kit) and is ready for FOC 
control algorithms. See the user manual for further details and installation guide. The list of supported motors is provided in the 
motor control SDK documentation (**X-CUBE-MCSDK-6**).

The function pack is compatible with **[STDATALOG_PYSDK](https://github.com/STMicroelectronics/stdatalog-pysdk)**, a data-centric design and user-friendly Python SDK that can run with hardware 
boards that supply real-time data streams, empowering users with full control of the data acquisition process. STDATALOG_PYSDK 
contains hsdatalogMC_GUI that communicates with the STEVAL-STWINBX1 via USB, allowing the user to control the motor (start/stop 
commands, set velocity) and select a combination of motor control telemetries and sensor data to be logged. The STEVAL-STWINBX1 
communicates with the STSPIN32G4 MCU via serial port (UART), using the motor control protocol (MCPv2). This protocol allows both 
system configuration and data logging.

The application can be controlled via Bluetooth using the STBLESensor app, which lets you manage the board and sensor 
configurations, start/stop data acquisition on the SD card, and control data labeling.

This firmware package includes components device drivers, board support package, and example applications for the STWIN.box 
platform (STEVAL-STWINBX1).

![](_htmresc/FP-IND-DATALOGMC_Software_Architecture.jpg)

Here is the list of references to user documents:

- [DB5152](https://www.st.com/resource/en/data_brief/FP-IND-DATALOGMC.pdf) : STM32Cube function pack for high-speed datalogging of sensors data and motor control telemetries
- [STM32Cube](https://www.st.com/stm32cube): STM32Cube

## Known Limitations

- None

## Development Toolchains and Compilers

-   IAR Embedded Workbench for ARM (EWARM) toolchain V9.20.1
-   RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.38
-   STM32CubeIDE v1.17.0

## Supported Devices and Boards

- [STEVAL-STWINBX1](https://www.st.com/stwinbox)
- [EVLSPIN32G4-ACT](https://www.st.com/en/evaluation-tools/evlspin32g4-act.html)

## Backward Compatibility

- None

## Dependencies

- None
