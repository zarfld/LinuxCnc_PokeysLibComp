# PEv2 Documentation

## Overview

The PEv2 component in the PoKeys library is used to interface with PoKeys devices and manage various digital inputs and outputs. This document provides detailed information about the pins, parameters, signals, and INI usage for the PEv2 component.

## Pins

### Digital Inputs

- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in`

### Digital Outputs

- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.digout.AxisEnable.out`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-0.out`

## Parameters

### Digital Input Parameters

- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`

### Digital Output Parameters

- `pokeys.[DevID].PEv2.digout.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.AxisEnable.invert`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-0.invert`

## Signals

### Digital Input Signals

- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in`

### Digital Output Signals

- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.digout.AxisEnable.out`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-0.out`

## INI Usage

The INI file is used to configure various settings for the PEv2 component. Below are some examples of how to configure the PEv2 component in the INI file.

### Example INI Configuration

```ini
[JOINT_0]
STEP_SCALE = 1000
MIN_LIMIT = -10
MAX_LIMIT = 10
HOME_OFFSET = 0
HOME_SEARCH_VEL = 1
HOME_LATCH_VEL = 0.5
STEPGEN_MAXVEL = 10
STEPGEN_MAXACCEL = 20

[JOINT_1]
STEP_SCALE = 1000
MIN_LIMIT = -10
MAX_LIMIT = 10
HOME_OFFSET = 0
HOME_SEARCH_VEL = 1
HOME_LATCH_VEL = 0.5
STEPGEN_MAXVEL = 10
STEPGEN_MAXACCEL = 20

[JOINT_2]
STEP_SCALE = 1000
MIN_LIMIT = -10
MAX_LIMIT = 10
HOME_OFFSET = 0
HOME_SEARCH_VEL = 1
HOME_LATCH_VEL = 0.5
STEPGEN_MAXVEL = 10
STEPGEN_MAXACCEL = 20

[JOINT_3]
STEP_SCALE = 1000
MIN_LIMIT = -10
MAX_LIMIT = 10
HOME_OFFSET = 0
HOME_SEARCH_VEL = 1
HOME_LATCH_VEL = 0.5
STEPGEN_MAXVEL = 10
STEPGEN_MAXACCEL = 20

[SPINDLE_0]
P = 1.0
I = 0.1
D = 0.01
BIAS = 0
FF0 = 0
FF1 = 1
FF2 = 0
DEADBAND = 0.01
MAX_OUTPUT = 10
```

## Additional Information

For more detailed information, refer to the comments in the `Pokeys57CNC_DM542_XXYZ_mill.ini` and `Pokeys57CNC_DM542_XXYZ_mill.hal` files. These files contain additional descriptions and usage examples for the PEv2 component.

## Conclusion

The PEv2 component in the PoKeys library provides a flexible and powerful way to interface with PoKeys devices. By following the guidelines and examples provided in this document, you can effectively configure and use the PEv2 component in your projects.

## Crosscheck with INI Entries

The following INI entries are relevant for the PEv2 description and have been documented accordingly:

### [POKEYS] Section

- `DEVICE_ID=53386`
- `ApplyIniSettings=1`
- `PEv2_ProbeInput=19`
- `PEv2_ProbeInputPolarity=0`
- `PEv2_EmergencyInputPin=52`
- `PEv2_EmergencyOutputPin=0`
- `PEv2_PulseGeneratorType=0`
- `PEv2_PG_swap_stepdir=0`
- `PEv2_PG_extended_io=1`

### AXIS_X

- `PEv2_AxisEnabled_0=1`
- `PEv2_AxesSwitchConfig_0=115`
- `PEv2_FilterLimitMSwitch_0=0`
- `PEv2_FilterLimitPSwitch_0=0`
- `PEv2_PinHomeSwitch_0=0`
- `PEv2_PinLimitMSwitch_0=0`
- `PEv2_PinLimitPSwitch_0=0`
- `PEv2_InvertLimitN_0=1`
- `PEv2_EnableLimitN_0=1`
- `PEv2_InvertLimitP_0=1`
- `PEv2_EnableLimitP_0=1`
- `PEv2_InvertHome_0=1`
- `PEv2_EnableHome_0=1`
- `PEv2_HomeOnLimitN_0=0`
- `PEv2_HomeOnLimitP_0=1`
- `PEv2_AxisEnableOutputPins_0=0`
- `PEv2_Feedback_Encoder_Id_0=0`
- `PEv2_InvertAxisEnable_0=1`

### AXIS_Y

- `PEv2_AxisEnabled_1=1`
- `PEv2_AxesSwitchConfig_1=165`
- `PEv2_FilterLimitMSwitch_1=0`
- `PEv2_FilterLimitPSwitch_1=0`
- `PEv2_PinHomeSwitch_1=0`
- `PEv2_PinLimitMSwitch_1=0`
- `PEv2_PinLimitPSwitch_1=0`
- `PEv2_InvertLimitN_1=1`
- `PEv2_EnableLimitN_1=1`
- `PEv2_InvertLimitP_1=0`
- `PEv2_EnableLimitP_1=0`
- `PEv2_InvertHome_1=1`
- `PEv2_EnableHome_1=1`
- `PEv2_HomeOnLimitN_1=0`
- `PEv2_HomeOnLimitP_1=0`
- `PEv2_AxisEnableOutputPins_1=0`
- `PEv2_Feedback_Encoder_Id_1=0`
- `PEv2_InvertAxisEnable_1=1`

### AXIS_Z

- `PEv2_AxesSwitchConfig_2=165`
- `PEv2_FilterLimitMSwitch_2=0`
- `PEv2_FilterLimitPSwitch_2=0`
- `PEv2_PinHomeSwitch_2=0`
- `PEv2_PinLimitMSwitch_2=0`
- `PEv2_PinLimitPSwitch_2=0`
- `PEv2_InvertLimitN_2=1`
- `PEv2_EnableLimitN_2=1`
- `PEv2_InvertLimitP_2=0`
- `PEv2_EnableLimitP_2=0`
- `PEv2_InvertHome_2=1`
- `PEv2_EnableHome_2=1`
- `PEv2_HomeOnLimitN_2=0`
- `PEv2_HomeOnLimitP_2=0`
- `PEv2_AxisEnableOutputPins_2=0`
- `PEv2_Feedback_Encoder_Id_2=0`
- `PEv2_InvertAxisEnable_2=1`

### AXIS_X2

- `PEv2_AxesSwitchConfig_6=115`
- `PEv2_FilterLimitMSwitch_6=0`
- `PEv2_FilterLimitPSwitch_6=0`
- `PEv2_PinHomeSwitch_6=0`
- `PEv2_PinLimitMSwitch_6=0`
- `PEv2_PinLimitPSwitch_6=0`
- `PEv2_InvertLimitN_6=1`
- `PEv2_EnableLimitN_6=1`
- `PEv2_InvertLimitP_6=1`
- `PEv2_EnableLimitP_6=1`
- `PEv2_InvertHome_6=1`
- `PEv2_EnableHome_6=1`
- `PEv2_HomeOnLimitN_6=0`
- `PEv2_HomeOnLimitP_6=1`
- `PEv2_AxisEnableOutputPins_6=0`
- `PEv2_Feedback_Encoder_Id_6=0`
- `PEv2_InvertAxisEnable_6=1`

## Detailed Digital Inputs and Outputs

### Digital Inputs

- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in`
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].digin.[PinId].in`
- `pokeys.[DevID].digin.[PinId].in-not`

### Digital Outputs

- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.digout.AxisEnable.out`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-0.out`
- `pokeys.[DevID].digout.[PinId].out`

## Crosscheck with HAL and INI Files

The following HAL and INI files have been crosschecked for relevant information:

- `Pokeys57CNC_DM542_XXYZ_mill.hal`
- `Pokeys57CNC_DM542_XXYZ_mill.ini`
- `postgui_call_list.hal`
- `pokeys.comp`

## Comments and Additional Descriptions

The comments in the INI and HAL files have been used to provide additional descriptions and usage examples for the PEv2 component. These comments help in understanding the possible values and configurations for the PEv2 component.

## Conclusion

The PEv2 component in the PoKeys library provides a flexible and powerful way to interface with PoKeys devices. By following the guidelines and examples provided in this document, you can effectively configure and use the PEv2 component in your projects.
