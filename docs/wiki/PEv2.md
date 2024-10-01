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
