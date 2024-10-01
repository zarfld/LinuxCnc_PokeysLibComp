# PoKeys PEv2 Digital Inputs and Parameters

This document provides detailed information about the digital inputs (digins) and parameters of the `pokeys.[DevID].PEv2` component.

## Overview

The `pokeys.[DevID].PEv2` component is designed to interface with PoKeys devices, providing real-time control and monitoring capabilities within LinuxCNC. It supports various types of I/O, including digital inputs, digital outputs, analog inputs, analog outputs, and encoders.

## Digital Inputs (digins)

### Pins

- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Pin`: Home switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Filter`: Digital filter for Home switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert`: Invert Home (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Pin`: Limit- switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Filter`: Digital filter for limit- switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert`: Invert limit- (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Pin`: Limit+ switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Filter`: Digital filter for limit+ switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert`: Invert limit+ (PoKeys functionality)
- `pokeys.[DevID].PEv2.digin.Emergency.Pin`: Emergency switch pin
- `pokeys.[DevID].PEv2.digin.Emergency.invert`: Invert Emergency (PoKeys functionality)
- `pokeys.[DevID].PEv2.digin.Probe.Pin`: Probe switch pin
- `pokeys.[DevID].PEv2.digin.Probe.invert`: Invert Probe (PoKeys functionality)

### Parameters

- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Pin`: Specifies the pin number for the Home switch. Set to 0 for an external dedicated input.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Filter`: Configures the digital filter for the Home switch to debounce the input signal.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert`: Inverts the Home switch input signal if set to TRUE.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Pin`: Specifies the pin number for the Limit- switch. Set to 0 for an external dedicated input.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Filter`: Configures the digital filter for the Limit- switch to debounce the input signal.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert`: Inverts the Limit- switch input signal if set to TRUE.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Pin`: Specifies the pin number for the Limit+ switch. Set to 0 for an external dedicated input.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Filter`: Configures the digital filter for the Limit+ switch to debounce the input signal.
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert`: Inverts the Limit+ switch input signal if set to TRUE.
- `pokeys.[DevID].PEv2.digin.Emergency.Pin`: Specifies the pin number for the Emergency switch.
- `pokeys.[DevID].PEv2.digin.Emergency.invert`: Inverts the Emergency switch input signal if set to TRUE.
- `pokeys.[DevID].PEv2.digin.Probe.Pin`: Specifies the pin number for the Probe switch.
- `pokeys.[DevID].PEv2.digin.Probe.invert`: Inverts the Probe switch input signal if set to TRUE.

## Examples

### Example 1: Configuring Home Switch

To configure the Home switch for a PoKeys device with DevID 1 and PEv2Id 0, set the following parameters:

```bash
setp pokeys.1.PEv2.0.digin.Home.Pin 5
setp pokeys.1.PEv2.0.digin.Home.Filter 10
setp pokeys.1.PEv2.0.digin.Home.invert TRUE
```

### Example 2: Configuring Limit Switches

To configure the Limit- and Limit+ switches for a PoKeys device with DevID 1 and PEv2Id 0, set the following parameters:

```bash
setp pokeys.1.PEv2.0.digin.LimitN.Pin 6
setp pokeys.1.PEv2.0.digin.LimitN.Filter 10
setp pokeys.1.PEv2.0.digin.LimitN.invert FALSE

setp pokeys.1.PEv2.0.digin.LimitP.Pin 7
setp pokeys.1.PEv2.0.digin.LimitP.Filter 10
setp pokeys.1.PEv2.0.digin.LimitP.invert FALSE
```

### Example 3: Configuring Emergency and Probe Switches

To configure the Emergency and Probe switches for a PoKeys device with DevID 1, set the following parameters:

```bash
setp pokeys.1.PEv2.digin.Emergency.Pin 8
setp pokeys.1.PEv2.digin.Emergency.invert TRUE

setp pokeys.1.PEv2.digin.Probe.Pin 9
setp pokeys.1.PEv2.digin.Probe.invert FALSE
```

By following these examples, you can configure the digital inputs and parameters for the `pokeys.[DevID].PEv2` component to suit your specific requirements.
