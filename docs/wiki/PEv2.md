# PEv2 Documentation

## Overview

The PEv2 component in the PoKeys library is used to interface with PoKeys devices and manage various digital inputs and outputs. This document provides detailed information about the pins, parameters, signals, and INI usage for the PEv2 component.

## Pins

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

## Interactions with pokeyslib in pokeys.comp

The following interactions with `pokeyslib` in `pokeys.comp` are related to PEv2:

- `pokeyslib.PE2_SetDigitalInput(pokeys.[DevID], pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in)`
- `pokeyslib.PE2_SetDigitalInput(pokeys.[DevID], pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in)`
- `pokeyslib.PE2_SetDigitalInput(pokeys.[DevID], pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in)`
- `pokeyslib.PE2_SetDigitalInput(pokeys.[DevID], pokeys.[DevID].PEv2.digin.Emergency.in)`
- `pokeyslib.PE2_SetDigitalOutput(pokeys.[DevID], pokeys.[DevID].PEv2.digout.Emergency.out)`
- `pokeyslib.PE2_SetDigitalOutput(pokeys.[DevID], pokeys.[DevID].PEv2.digout.AxisEnable.out)`
- `pokeyslib.PE2_SetDigitalOutput(pokeys.[DevID], pokeys.[DevID].PEv2.digout.ExternalRelay-0.out)`

## Conclusion

The PEv2 component in the PoKeys library provides a flexible and powerful way to interface with PoKeys devices. By following the guidelines and examples provided in this document, you can effectively configure and use the PEv2 component in your projects.

## PEv2 Information

### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration [8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition [8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup [8]`
- `pokeys.[DevID].PEv2.[PE
