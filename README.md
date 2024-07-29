# FP-IND-DATALOGMC Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/fp-ind-datalogmc.svg?color=brightgreen)

The **FP-IND-DATALOGMC** function pack for **STEVAL-STWINBX1** and
**EVLSPIN32G4-ACT** is a powerful integrated toolkit for the next generation of smart
actuators.

This toolkit is derived from a FP-SNS-DATALOG2 function pack and it allows the
collection of heterogeneous data, combining STWIN.box sensor information with
STSPIN32G4 motor control data and it provides a comprehensive view of the
system’s operational conditions. This enables both real-time monitoring and accurate
performance assessment.

The v1.1.0 introduces a further firmware example called **DATALOGMC_AI** that
implements a motor fault classification based on a machine learning solution
developed through **NanoEdgeAIStudio**. The machine learning model allows an
accurate classification of motor behavior into two states: good and faulty.

Users can also customize the model by adding their own classes, which can be
achieved by incorporating additional data and modifying the training process.
The package also includes a portable mechanical setup that can be replicated with a
3D printer.

EVLSPIN32G4-ACT is designed to drive a variety of three-phase brushless DC
motors (not included in the kit) and ready for FOC control algorithms. The list of
supported motors is provided in the motor control SDK documentation (X-CUBEMCSDK-
6) and installation details are available in the Quick Start guide.

The FP-IND-DATALOGMC, thanks to its data-centric design and user-friendly Python
SDK, may run with hardware boards that supply real-time data streams and motor
telemetries, empowering users with full control of the data acquisition process.

The included firmware is compatible with the STBLESensor app, which lets you
manage: the board, motor, and sensor configurations; start/stop data acquisition on
SD card, and control data labeling. Sensor data and motor telemetries can also be
stored onto a microSD™ card.  

![](_htmresc/FP-IND-DATALOGMC_Software_Architecture.jpg)

Here is the list of references to user documents:

- [DB5152](https://www.st.com/resource/en/data_brief/FP-IND-DATALOGMC.pdf) : STM32Cube function pack for high-speed datalogging of sensors data and motor control telemetries
- [STM32Cube](https://www.st.com/stm32cube): STM32Cube

## Known Limitations

- None

## Development Toolchains and Compilers

-   IAR Embedded Workbench for ARM (EWARM) toolchain V9.20.1
-   RealView Microcontroller Development Kit (MDK-ARM) toolchain V5.38
-   STM32CubeIDE v1.15.1

## Supported Devices and Boards

- [STEVAL-STWINBX1](https://www.st.com/stwinbox)
- [EVLSPIN32G4-ACT](https://www.st.com/en/evaluation-tools/evlspin32g4-act.html)

## Backward Compatibility

- None

## Dependencies

- None
