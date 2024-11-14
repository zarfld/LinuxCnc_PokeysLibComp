# PoKeys Component Documentation

This document provides detailed information about the `pokeys.comp` component, including descriptions of pins and parameters.

## Overview

The `pokeys.comp` component is designed to interface with PoKeys devices, providing real-time control and monitoring capabilities within LinuxCNC. It supports various types of I/O, including digital inputs, digital outputs, analog inputs, analog outputs, and encoders.

Note that `pokeys.comp` is a UserSpace component rather than a realtime component.

## Digital Input

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

## PoKeys device information

### Pins
- `pokeys.[DevID].devSerial`   // devSerial used in PK_ConnectToDeviceWSerial or PK_ConnectToDeviceWSerial_UDP
- `pokeys.[DevID].alive`          // status bit it is blinking on/off when operating
- `pokeys.[DevID].connected`  //gets true when connected
- `pokeys.[DevID].err`               // gets True oin Errors
- `pokeys.[DevID].info.PinCount`                        // Number of pins, physically on the device
- `pokeys.[DevID].info.PWMCount`                        // Number of pins that support PWM output
- `pokeys.[DevID].info.BasicEncoderCount`               // Number of basic encoders
- `pokeys.[DevID].info.EncodersCount`                   // Number of encoder slots available
- `pokeys.[DevID].info.FastEncoders`                    // Number of fast encoders supported
- `pokeys.[DevID].info.UltraFastEncoders`               // Number of available ultra fast encoders
- `pokeys.[DevID].info.PWMinternalFrequency`             // Main PWM peripheral clock
- `pokeys.[DevID].info.AnalogInputs`                    // Number of available analog inputs
- `pokeys.[DevID].info.KeyMapping`                      // Device supports key mapping (acts as a USB keyboard)
- `pokeys.[DevID].info.TriggeredKeyMapping`             // Device supports triggered key mapping
- `pokeys.[DevID].info.KeyRepeatDelay`                  // Device supports user customizable key repeat rates and delays
- `pokeys.[DevID].info.DigitalCounters`                 // Device supports digital counters
- `pokeys.[DevID].info.JoystickButtonAxisMapping`       // Device supports mapping of joystick buttons
- `pokeys.[DevID].info.JoystickAnalogToDigitalMapping`  // Device supports mapping of analog inputs to digital keys
- `pokeys.[DevID].info.Macros`                          // Device supports customizable macro sequences
- `pokeys.[DevID].info.MatrixKeyboard`                  // Device supports matrix keyboard
- `pokeys.[DevID].info.MatrixKeyboardTriggeredMapping`  // Device supports matrix keyboard triggered key mapping
- `pokeys.[DevID].info.LCD`                             // Device supports alphanumeric LCD display
- `pokeys.[DevID].info.MatrixLED`                       // Device supports matrix LED display
- `pokeys.[DevID].info.ConnectionSignal`                // Device supports connection signal output
- `pokeys.[DevID].info.PoExtBus`                        // Device supports PoExtBus digital outputs
- `pokeys.[DevID].info.PoNET`                           // Device supports PoNET bus devices
- `pokeys.[DevID].info.AnalogFiltering`                 // Device supports analog inputs low-pass digital filtering
- `pokeys.[DevID].info.InitOutputsStart`                // Device supports initializing outputs at startup
- `pokeys.[DevID].info.protI2C`                         // Device supports I2C bus (master)
- `pokeys.[DevID].info.prot1wire`                       // Device supports 1-wire bus (master)
- `pokeys.[DevID].info.AdditionalOptions`               // Device supports additional options with activation keys
- `pokeys.[DevID].info.LoadStatus`                      // Device supports reporting load status
- `pokeys.[DevID].info.CustomDeviceName`                // Device supports specifying custom device names
- `pokeys.[DevID].info.PoTLog27support`                 // Device supports PoTLog27 firmware
- `pokeys.[DevID].info.SensorList`                      // Device supports sensor lists
- `pokeys.[DevID].info.WebInterface`                    // Device supports web interface
- `pokeys.[DevID].info.FailSafeSettings`                // Device supports fail-safe mode
- `pokeys.[DevID].info.JoystickHATswitch`               // Device supports joystick HAT switch mapping
- `pokeys.[DevID].info.PulseEngine`                     // Device supports Pulse engine
- `pokeys.[DevID].info.PulseEnginev2`                   // Device supports Pulse engine v2
- `pokeys.[DevID].info.EasySensors`                     // Device supports EasySensors

## Homing Use Case

### Interaction Between `pokeys_homecomp.comp` and `pokeys.comp`

`pokeys_homecomp.comp` interacts with `pokeys.comp` by linking homing signals and pins to the PoKeys components. Here are the key points of interaction:

* `pokeys_homecomp.comp` defines various pins and signals related to the homing process, including `PEv2_AxesState` and `PEv2_AxesCommand`.
* The homing signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
* The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components.
* The `DM542_XXYZ_mill/pokeys_homing.md` file provides a detailed description of the homing configuration and linking pins.
* When homing is triggered, `pokeys_homecomp.comp` sets the `joint.#.PEv2.AxesCommand` pin, which is then received by `pokeys.comp` at `pokeys.0.PEv2.#.AxesCommand`.
* The status of PEv2 in `pokeyslib` is set on `pokeys.0.PEv2.#.AxesState` by `pokeys.comp`, and it is received on `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`.
* `pokeys_homecomp.comp` checks the status and represents them at `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state`.

This setup ensures that the homing command is correctly propagated from `pokeys_homecomp.comp` to `pokeys.comp`, and finally to the PoKeys library to execute the homing procedure. The status of the pulse engine axes is also correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

### Roles of `PEv2_AxesState` and `PEv2_AxesCommand`

The `PEv2_AxesState` and `PEv2_AxesCommand` play crucial roles in the homing process of the PoKeys device. Here is an explanation of their roles:

* `PEv2_AxesState`: This pin represents the state of the pulse engine axes. It is used to monitor the current state of each axis during the homing process. The possible values for `PEv2_AxesState` include:
  * `PK_PEAxisState_axHOMING_RESETTING`: Stopping the axis to reset the position counters
  * `PK_PEAxisState_axHOMING_BACKING_OFF`: Backing off switch
  * `PK_PEAxisState_axHOMINGSTART`: Homing procedure is starting on axis
  * `PK_PEAxisState_axHOMINGSEARCH`: Homing procedure first step - going to home
  * `PK_PEAxisState_axHOMINGBACK`: Homing procedure second step - slow homing
  * `PK_PEAxisState_axHOME`: Axis is homed
  * These values are used to manage the homing process and ensure that the pulse engine axes are correctly controlled during the homing routine.

* `PEv2_AxesCommand`: This pin is used to send commands to the pulse engine axes during the homing process. The possible values for `PEv2_AxesCommand` include:
  * `PK_PEAxisCommand_axIDLE`: Axis in IDLE state
  * `PK_PEAxisCommand_axHOMINGSTART`: Start Homing procedure
  * `PK_PEAxisCommand_axHOMINGCANCEL`: Cancel Homing procedure
  * These values are used to control the state of the pulse engine axes and initiate or cancel the homing process.

The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for these pins, linking them from `pokeys_homecomp.comp` to the PoKeys components. This setup ensures that the homing command is correctly propagated from `pokeys_homecomp.comp` to `pokeys.comp`, and finally to the PoKeys library to execute the homing procedure. The status of the pulse engine axes is also correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

### Propagation of Homing Signals

The homing signals in the system are propagated through a series of connections and interactions between various components. Here is an overview of how the homing signals are propagated:

* The `pokeys_homecomp.comp` component defines various pins and signals related to the homing process, including `PEv2_AxesState` and `PEv2_AxesCommand`.
* The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components.
* When homing is triggered, `pokeys_homecomp.comp` sets the `joint.#.PEv2.AxesCommand` pin, which is then received by `pokeys.comp` at `pokeys.0.PEv2.#.AxesCommand`.
* The status of PEv2 in `pokeyslib` is set on `pokeys.0.PEv2.#.AxesState` by `pokeys.comp`, and it is received on `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`.
* `pokeys_homecomp.comp` checks the status and represents them at `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state`.
* The `DM542_XXYZ_mill/pokeys_homing.md` file provides a detailed description of the homing configuration and linking pins.

This setup ensures that the homing command is correctly propagated from `pokeys_homecomp.comp` to `pokeys.comp`, and finally to the PoKeys library to execute the homing procedure. The status of the pulse engine axes is also correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

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
