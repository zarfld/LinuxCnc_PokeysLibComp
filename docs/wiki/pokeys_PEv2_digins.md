# PoKeys PEv2 Digital Inputs and Parameters

This document provides detailed information about the digital inputs (digins) and parameters of the `pokeys.[DevID].PEv2` component.

## Overview

The `pokeys.[DevID].PEv2` component is designed to interface with PoKeys devices, providing real-time control and monitoring capabilities within LinuxCNC. It supports various types of I/O, including digital inputs, digital outputs, analog inputs, analog outputs, and encoders.

## Digital Inputs (digins)

### Pins

- `pokeys.[DevID].digin.[PinID].in`: State of the hardware input
- `pokeys.[DevID].digin.[PinID].in-not`: Inverted state of the input
- `pokeys.[DevID].PoExtBus.[PoExtBusId]digin.[PinID].in`: State of the PoExtBus input (currently prepared for future Firmware)
- `pokeys.[DevID].PoExtBus.[PoExtBusId]digin.[PinID].in-not`: Inverted state of the PoExtBus input
- `pokeys.[DevID].PEv2.digin.Emergency.in`: Emergency input for PulseEnginev2
- `pokeys.[DevID].PEv2.digin.Probe.in`: Probe input for PulseEnginev2
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Probe.in`: Probe input for each Axis of PulseEnginev2
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in`: State of the Home input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in-not`: Inverted state of the Home input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in`: State of the Limit- input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in-not`: Inverted state of the Limit- input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in`: State of the Limit+ input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in-not`: Inverted state of the Limit+ input for Joint[PEv2Id]
- `pokeys.[DevID].PEv2.[PEv2Id].digin.AxisEnabled.in`: Axis enabled input for each Axis of PulseEnginev2
- `pokeys.[DevID].kbd48CNC.[ButtonId].Button`: State of the button on kbd48CNC (True if Button pressed)

### Parameters

- `pokeys.[DevID].digin.[PinID].invert`: If TRUE, in is inverted before reading from the hardware
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

## Digital Outputs (digouts)

### Pins

- `pokeys.[DevID].PEv2.digout.*`
  - Description: Digital output pins for the PEv2 component.
  - Example: `pokeys.0.PEv2.digout.Emergency.out`

### Parameters

- `pokeys.[DevID].PEv2.digout.*.invert`
  - Description: Invert the digital output signal.
  - Example: `pokeys.0.PEv2.digout.Emergency.invert`

### Signals

- `pokeys.[DevID].PEv2.digout.*.out`
  - Description: Digital output signal.
  - Example: `pokeys.0.PEv2.digout.Emergency.out`

## Conclusion

The PEv2 component in the PoKeys library provides a flexible and powerful way to interface with PoKeys devices. By following the guidelines and examples provided in this document, you can effectively configure and use the PEv2 component in your projects.
