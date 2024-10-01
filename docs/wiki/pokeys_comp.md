# PoKeys Component Documentation

This document provides detailed information about the `pokeys.comp` component, including descriptions of pins and parameters.

## Overview

The `pokeys.comp` component is designed to interface with PoKeys devices, providing real-time control and monitoring capabilities within LinuxCNC. It supports various types of I/O, including digital inputs, digital outputs, analog inputs, analog outputs, and encoders.

Note that `pokeys.comp` is a UserSpace component rather than a realtime component.

## Digital Input

### Pins

- `pokeys.[DevID].digin.[PinID].in`: State of the hardware input
- `pokeys.[DevID].digin.[PinID].in-not`: Inverted state of the input

### Parameters

- `pokeys.[DevID].digin.[PinID].invert`: If TRUE, in is inverted before reading from the hardware

## Digital Output

### Pins

- `pokeys.[DevID].digout.[PinID].out`: Value to be written (possibly inverted) to the hardware output

### Parameters

- `pokeys.[DevID].digout.[PinID].invert`: If TRUE, out is inverted before writing to the hardware

## Analog Input

### Pins

- `pokeys.[DevID].adcin.[AdcId].value-raw`: The hardware reading
- `pokeys.[DevID].adcin.[AdcId].value`: The hardware reading, scaled according to the scale and offset parameters

### Parameters

- `pokeys.[DevID].adcin.[AdcId].scale`: The input voltage (or current) will be multiplied by scale before being output to value
- `pokeys.[DevID].adcin.[AdcId].offset`: This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied

## Analog Output

### Pins

- `pokeys.[DevID].adcout.[AdcId].value`: The value to be written. The actual value output to the hardware will depend on the scale and offset parameters
- `pokeys.[DevID].adcout.[AdcId].enable`: If false, then output 0 to the hardware, regardless of the value pin

### Parameters

- `pokeys.[DevID].adcout.[AdcId].offset`: This will be added to the value before the hardware is updated
- `pokeys.[DevID].adcout.[AdcId].scale`: This should be set so that an input of 1 on the value pin will cause 1V
- `pokeys.[DevID].adcout.[AdcId].high_limit`: When calculating the value to output to the hardware, if value + offset is greater than high_limit, then high_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].low_limit`: When calculating the value to output to the hardware, if value + offset is less than low_limit, then low_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].max_v`: Max output voltage
- `pokeys.[DevID].adcout.pwm.period`: PWM period, shared among all channels

## Encoder

### Pins

- `pokeys.[DevID].encoder.[EncId].count`: Encoder value in counts
- `pokeys.[DevID].encoder.[EncId].position`: Encoder value in position units (see parameter “scale”)
- `pokeys.[DevID].encoder.[EncId].reset`: When True, force counter to zero

### Parameters

- `pokeys.[DevID].encoder.[EncId].scale`: The scale factor used to convert counts to position units. It is in “counts per position unit”

## Real-Time Clock

### Pins

- `pokeys.[DevID].rtc.sec`: Second
- `pokeys.[DevID].rtc.min`: Minute
- `pokeys.[DevID].rtc.hour`: Hour
- `pokeys.[DevID].rtc.dow`: Day of week
- `pokeys.[DevID].rtc.dom`: Day of month
- `pokeys.[DevID].rtc.doy`: Day of year
- `pokeys.[DevID].rtc.month`: Month
- `pokeys.[DevID].rtc.year`: Year
- `pokeys.[DevID].rtc.loop_frequ`: Actual loop frequency of pokeys.comp updated after rtc.sec changed
- `pokeys.[DevID].rtc.loop_frequ_demand`: Demand value for loop frequency (if 0, default of 10Hz will be used)

## Additional Pins

- `pokeys.[DevID].digin.[PinId].in`: State of the hardware input
- `pokeys.[DevID].digin.[PinId].in-not`: Inverted state of the input
- `pokeys.[DevID].digout.[PinId].out`: Value to be written (possibly inverted) to the hardware output
- `pokeys.[DevID].digout.[PinId].invert`: If TRUE, out is inverted before writing to the hardware

## PEv2 Digital Input

### Pins

- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.in`: Home switch input
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.in`: Limit- switch input
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.in`: Limit+ switch input
- `pokeys.[DevID].PEv2.digin.Emergency.in`: Emergency switch input
- `pokeys.[DevID].PEv2.digin.Probe.in`: Probe input

### Parameters

- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Pin`: Home switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Filter`: Digital filter for Home switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert`: Invert Home (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Pin`: Limit- switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Filter`: Digital filter for limit- switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert`: Invert limit- (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Pin`: Limit+ switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Filter`: Digital filter for limit+ switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert`: Invert limit+ (PoKeys functionality)
- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

## kbd48CNC

### Pins

- `pokeys.[DevID].kbd48CNC.[KeyID].Button`: State of the keyboard button
- `pokeys.[DevID].kbd48CNC.[KeyID].LED`: State of the keyboard LED

### Parameters

- `pokeys.[DevID].kbd48CNC.[KeyID].Button.invert`: If TRUE, Button is inverted before reading from the hardware
- `pokeys.[DevID].kbd48CNC.[KeyID].LED.invert`: If TRUE, LED is inverted before writing to the hardware
