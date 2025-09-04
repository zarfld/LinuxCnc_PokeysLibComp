# LinuxCnc_PokeysLibComp

## Overview

The LinuxCnc_PokeysLibComp project integrates PoKeys devices with LinuxCNC, providing control and monitoring capabilities. This repository contains the necessary components, scripts, and documentation to set up and use PoKeys devices with LinuxCNC.

## Directory Structure

The repository is organized into the following directories:

- `pokeys_uspace/`: Contains Source of pokeys userspace component for interfacing with PoKeys devices.
- `pokeys_py/`: prepared for Contains Python scripts for interfacing with PoKeys devices.
- `pokeys_rt/`: Contains Source of pokeys_homecomp real-time component for PoKeys devices.
- `tests/`: Contains unit tests, integration tests, and other testing-related files.
- `docs/`: Contains project-related documentation.
- `scripts/`: Contains scripts for setup, configuration, and auxiliary tasks.

## Installation

To install the LinuxCnc_PokeysLibComp project, follow these steps:

```bash
git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
cd LinuxCnc_PokeysLibComp
sudo sh install.sh
```

## Running the UI Setup Tool - !!! planned feature - Work in progress  - do NOT use!!!

After installation you can launch the graphical configuration utility with:

```bash
python -m pokeys_py
```

Importing the ``pokeys_py`` package no longer performs any device actions or
telemetry initialization. Hardware is only accessed when the setup tool or
``run_ui_setup_tool`` function is invoked.

## Hardware Requirements

The project has been tested with the following hardware:

- RaspberryPi4 (for LinuxCNC)
- PoKeys57E
- PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir
- CNC keyboard – PoNETkbd48CNC
- PoRelay8 – Relay extension board
- PoExtBusOC16
- PoKeys57CNC
- PoStep25-32 (v1.3)

## Library Status

The following features have been implemented and tested:

- [e] Connecting to USB devices
- [x] Connecting to network devices
- [x] Reading device data
- [x] Reading digital inputs
- [x] Writing digital outputs
- [x] Reading analog inputs
- [x] PoExtBus writing
- [x] PoExtBus reading
- [ ] LCD operations
- [ ] Matrix LED
- [x] Setting pin functions
- [x] Reading pin functions
- [ ] Setting pin key codes
- [ ] Reading pin key codes
- [x] PWM operations
- [x] Pulse engine operations
- [i] Matrix keyboard setup
- [i] Matrix keyboard reading
- [x] kbd48CNC set LED
- [x] kbd48CNC get Button Status
- [x] Using encoders
- [ ] I2C operations
- [ ] SPI operation
- [ ] PoIL operations
- [i] 1-wire operations

x ... available
i ... implemented but not tested
! ... implemented but not working yet
p ... planned
e ... known issue

### PoRelay8:

- [x] needs to be connected in Parallel to kbd48CNC with white PoExt Cable OR using Canbus (connecting it using the Red PoExtBus cable to the i2C extender which is delivered with kbd48CNC will not work)
- [x] if it is used together with "non smart" PoExtBus devices (e.g. PoExtBusOC16) use "PoExtBus - Smart" page on Pokeys-SW to set "Sequence ID"
- [ ] inputs not available (yet?)

### PoExtBusOC16

- [x] can be set using pokeys.0.PoExtBus.#.Out-0..7 (all 16outs can be used pokeys.0.PoExtBus.0 and pokeys.0.PoExtBus.1)
- [ ] inputs not available

### PoExtBusOC16CNC

- see "PulseEnginev2"

### kbd48CNC:

- [x] using as PoNet-extension buard attached to Pokeys57E
- [x] using CanBus (6Pin MicroMatch Connector) directly on Pokeys57CNC

### Pokeys57E

- [x] connected using Ethernet
- [x] Read&Set of Digital IO on HalPins pokeys.0.Pins.\*.DigitalValueGet / DigitalValueSet
- [x] Readt of AnalogValue IO on HalPins pokeys.0.Pins.\*.AnalogValue on the pins that support that
- periphals listed above was tested on this device.

changing structure of IOs to "Canonical Device Interface" in work

### Pokeys57CNC

- [x] connected using USB
- [x] connected using Ethernet
- [x] LinuxCNC example config in Work (using the pinsetup as available on PCB)
- [x] Switchng OC Outputs pokeys.0.PEv2.ExternalOCOutput.0..3
- [x] Switchng Relay Outputs (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1) (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1)
- using USB connection seems "faster" than using ethernet - may depend on networksetup (networkswitch vs. direct connection)

### Pokeys57E

- [ ] connected using USB (currently could not connect on newer distributions see issue #231 )

### PulseEnginev2

- [x] Setting Status Running/Stop depending on LinuxCNC state ("machine-on")
- [x] Reading Status of Limit+/- and Home switches
- [x] setting External OC and RelayOutputs - in case that "Extended IO" is available and activated (Pokeys57CNC)
- [x] setting PinConfiguration for Limits, Home, Probe & Emergency switches based in LinuxCNC-INI configuration - not reliable
- [x] parametrizing MaxSpeed, MaxAccelleration, ... based on LinuxCNC configuarion (mm/sec to pulses/sec conversion is being done using JOINT.\*.STEPSCALE)

## HAL Interface

### Pokeys Pin

![image](https://github.com/user-attachments/assets/d0f7cb2d-aeb1-4d77-87f3-09857c2719b9)

Count: 55

#### Parameters

- pokeys.[DevID].Pin.[PinID].PinFunction int PinFunction to be set for corresponding pin see ePK_PinCap

the PinFunction can be set using followin enumeration. Note that e.g. digitalInput or digitalOutput could be directly inverted on pokeys by adding value 128 (PK_PinCap_digitalOutput+PK_PinCap_invertPin or 2 + 128 = 130):

add. Note: for PK_PinCap_analogOutput has not been used so far currently only PWM functionality has been used instead that is what is being used for (pins 18, 20, 21 and 22)

```
      enum ePK_PinCap
      {
          PK_PinCap_pinRestricted  = 0,           // Pin is not used
          PK_PinCap_reserved       = 1,           // --
          PK_PinCap_digitalInput   = 2,           // Digital input
          PK_PinCap_digitalOutput  = 4,           // Digital output
          PK_PinCap_analogInput    = 8,           // Analog input (only on selected pins)
          PK_PinCap_analogOutput   = 16,          // Analog output (only on selected pins)
          PK_PinCap_triggeredInput = 32,          // Triggered input
          PK_PinCap_digitalCounter = 64,          // Digital counter (only on selected pins)
          PK_PinCap_invertPin      = 128          // Invert digital pin polarity (set together with digital input, output or triggered input)
      };
```

### Digital Input

Count: 55

#### Pins

- pokeys.[DevID].digin.[PinID].in bit State of the hardware input
- pokeys.[DevID].digin.[PinID].in-not bit Inverted state of the input.

#### Parameters

- `pokeys.[DevID].digin.[PinID].invert` bit invert Pin on pokeys using PK_PinCap_invertPin on PinFunction

### Digital Output

Count: 55

#### Pins

- `pokeys.[DevID].digout.[PinID].out` bit Value to be written (possibly inverted) to the hardware output

#### Parameters

- `pokeys.[DevID].digout.[PinID].invert` bit invert Pin on pokeys using PK_PinCap_invertPin on PinFunction

### Analog Input

Count: 7

#### Pins

- `pokeys.[DevID].adcin.[AdcId].value-raw` float The hardware reading
- `pokeys.[DevID].adcin.[AdcId].value` float The hardware reading, scaled according to the scale and offset parameters

#### Parameters

- `pokeys.[DevID].adcin.[AdcId].scale` float The input voltage (or current) will be multiplied by scale before being output to value
- `pokeys.[DevID].adcin.[AdcId].offset` float This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied

### Analog Output (PWM)

![image](https://github.com/user-attachments/assets/2cbabe83-ed07-4ff1-9151-ba896d139e8b)

Count: 6

#### Pins

- `pokeys.[DevID].adcout.[AdcId].value`: The value to be written. The actual value output to the hardware will depend on the scale and offset parameters
- `pokeys.[DevID].adcout.[AdcId].enable`: If false, then output 0 to the hardware, regardless of the value pin

#### Parameters

- `pokeys.[DevID].adcout.[AdcId].offset`: This will be added to the value before the hardware is updated
- `pokeys.[DevID].adcout.[AdcId].scale`: This should be set so that an input of 1 on the value pin will cause 1V
- `pokeys.[DevID].adcout.[AdcId].high_limit`: When calculating the value to output to the hardware, if value + offset is greater than high_limit, then high_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].low_limit`: When calculating the value to output to the hardware, if value + offset is less than low_limit, then low_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].max_v`: Max output voltage
- `pokeys.[DevID].adcout.pwm.period`: PWM period, shared among all channels

### Encoder

Count: 29

#### Pins

- pokeys.[DevID].encoder.[EncId].count; S32 Encoder value in counts.
- pokeys.[DevID].encoder.[EncId].position; FLOAT Encoder value in position units (see parameter “scale”).
- ~~pokeys.[DevID].encoder.[EncId].velocity; FLOAT Velocity in position units per second~~
- pokeys.[DevID].encoder.[EncId].reset; BIT When True, force counter to zero
- ~~pokeys.[DevID].encoder.[EncId].index-enable; BIT (bidirectional) When True, reset to zero on next index pulse, and set pin False.~~

#### Parameters

- pokeys.[DevID]. encoder.[EncId].scale FLOAT "The scale factor used to convert counts to position units. It is in “counts per position unit”";

### Real-Time Clock

#### Pins

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

### Pulse Engine

![image](https://github.com/user-attachments/assets/7e5205bd-206e-47c1-8ed5-7084deec5348)

#### Pulse Generator options

![image](https://github.com/user-attachments/assets/11616183-3ac6-4e62-9b90-539d05009d5a)

#### Motion Parameters

![image](https://github.com/user-attachments/assets/e5ecda16-c469-4b2d-9326-4ca26beaaf1a)

#### Emergency Pin

#### Limit Switches

![image](https://github.com/user-attachments/assets/77c3ed81-c1cf-4005-b674-b3e04fdacf3a)

#### Home/Ref switch

![image](https://github.com/user-attachments/assets/8e240b6d-f876-46a8-af00-23ac77b827f3)

#### Axis enable output pin

![image](https://github.com/user-attachments/assets/2df47a81-f1a3-40c2-99a2-010a8b672413)

#### Aux. outputs

![image](https://github.com/user-attachments/assets/3d8df6b2-d5df-4afc-9c14-a8d45ca47309)

This section provides access to external **Relay** and **Open-Collector (OC)** outputs available on PoKeys devices that support Pulse Engine v2 (PEv2).

##### HAL Pins

The following HAL output pins are exposed by the component:

###### Relay Outputs

pokeys.[DevID].PEv2.digout.ExternalRelay-0.out
pokeys.[DevID].PEv2.digout.ExternalRelay-1.out
pokeys.[DevID].PEv2.digout.ExternalRelay-2.out
pokeys.[DevID].PEv2.digout.ExternalRelay-3.out

###### OC Outputs

pokeys.[DevID].PEv2.digout.ExternalOC-0.out
pokeys.[DevID].PEv2.digout.ExternalOC-1.out
pokeys.[DevID].PEv2.digout.ExternalOC-2.out
pokeys.[DevID].PEv2.digout.ExternalOC-3.out

These pins correspond to the checkboxes shown in the _Aux. outputs_ section of the PoKeys configuration utility:

### Relay Outputs

| UI Label | HAL Pin                                          | PoKeys57CNC | PoKeys57CNCpro4x25 | PoKeysCNCaddon |
| -------- | ------------------------------------------------ | ----------- | ------------------ | -------------- |
| 0        | `pokeys.[DevID].PEv2.digout.ExternalRelay-0.out` | SSR1        | FAN Control        | Relay1         |
| 1        | `pokeys.[DevID].PEv2.digout.ExternalRelay-1.out` | SSR2        | Plasma Relay       | Relay2         |
| 2        | `pokeys.[DevID].PEv2.digout.ExternalRelay-2.out` | Relay1      | Relay1             | Relay3         |
| 3        | `pokeys.[DevID].PEv2.digout.ExternalRelay-3.out` | Relay2      | Relay2             | Relay4         |

### Open-Collector (OC) Outputs

| UI Label | HAL Pin                                       | PoKeys57CNC | PoKeys57CNCpro4x25 | PoKeysCNCaddon |
| -------- | --------------------------------------------- | ----------- | ------------------ | -------------- |
| 0        | `pokeys.[DevID].PEv2.digout.ExternalOC-0.out` | OC1         | OC1                | OC1            |
| 1        | `pokeys.[DevID].PEv2.digout.ExternalOC-1.out` | OC2         | OC2                | OC2            |
| 2        | `pokeys.[DevID].PEv2.digout.ExternalOC-2.out` | OC3         | OC3                | OC3            |
| 3        | `pokeys.[DevID].PEv2.digout.ExternalOC-3.out` | OC4         | OC4                | OC4            |

> ℹ️ **Note:** These mappings are automatically applied at runtime based on the detected device type.
> You don't need to change your HAL file — the component ensures correct hardware control behind the scenes.

---

##### Example Usage

You can assign a logical signal to control a relay like this:

```hal
net floodlight-control => pokeys.0.PEv2_digout_ExternalRelay_out[2]
```

#### MPG jog setup

![image](https://github.com/user-attachments/assets/860896a5-b3c9-4c95-8bd4-eb01fc9b2f43)

#### Pulse Engine State

![image](https://github.com/user-attachments/assets/441ab118-b71e-4c6e-b30c-4ac1906a8ee7)

#### Parameters

## Documentation

The documentation provides clear instructions on running tests, setup steps for hardware-dependent tests, and interpreting test results and coverage reports. It is essential to keep the documentation up to date as new features are implemented and tests are added.

By following these instructions, you can ensure the reliability, correctness, and performance of the `pokeys_py` component, making it easier to maintain and develop in the future.

## Contributing

We welcome contributions from the community to help improve the project. Please take a moment to review the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Branch Naming Conventions

To keep the repository organized, please follow these branch naming conventions:

- **Feature Branches**: `feature/{feature-name}`
- **Bug Fix Branches**: `bugfix/{bug-name}`
- **Improvement Branches**: `improvement/{improvement-name}`
- **Hotfix Branches**: `hotfix/{hotfix-name}`

## Creating Separate Branches for Each Feature or Bug Fix

Each new feature or bug fix should have its own branch created from the latest version of the main branch. The branch should focus on one feature or issue to ensure isolated development.

## Conformity with LinuxCNC Guidelines and Canonical Device Interface

This repository aligns with LinuxCNC’s development practices and canonical interfaces. The components have been reviewed and updated to follow LinuxCNC coding guidelines, ensuring that HAL and INI files conform to LinuxCNC’s structure and formatting. Real-time components comply with LinuxCNC’s real-time constraints, and all components follow the canonical interface definitions. The code adheres to LinuxCNC's code style and formatting guidelines, and test cases have been created and run to validate conformity.

## GitHub Discussions

We encourage the use of GitHub Discussions for questions, feature requests, and ideas before creating issues or pull requests. This helps keep the issue tracker focused on actionable items and fosters community engagement.

### Available Categories

We have created the following categories in GitHub Discussions to help organize conversations:

- **General Q&A**: For users to ask questions or seek clarification on how to use the repository.
- **Feature Requests**: For proposing new features or enhancements.
- **Ideas & Brainstorming**: For discussing ideas that are still in the conceptual stage.
- **Show and Tell**: For users to share how they are using the project in their own implementations.
- **Announcements**: For maintainers to share important updates, releases, or roadmaps.
- **Bug Reports**: For reporting bugs and discussing potential fixes.

### Guidelines for Starting a Discussion

When starting a discussion, please follow these guidelines:

1. **Choose the Appropriate Category**: Select the category that best fits your topic to help keep discussions organized.
2. **Provide a Clear Title**: Use a clear and descriptive title for your discussion.
3. **Include Relevant Details**: Provide as much relevant information as possible to help others understand your question, idea, or request.
4. **Be Respectful and Constructive**: Follow the project's Code of Conduct and engage in respectful and constructive conversations.

### When to Use Discussions Instead of Issues or Pull Requests

Use GitHub Discussions for:

- General questions or clarifications about the project.
- Proposing new features or enhancements before creating a feature request issue.
- Sharing ideas and brainstorming with the community.
- Showcasing how you are using the project in your own implementations.
- Reporting bugs and discussing potential fixes before creating a bug report issue.

By using GitHub Discussions effectively, we can build a stronger community around the project and make the repository more welcoming to both new and experienced users.

You can access the GitHub Discussions for this repository [here](https://github.com/zarfld/LinuxCnc_PokeysLibComp/discussions).

## Sponsorship

We appreciate your interest in supporting the LinuxCnc_PokeysLibComp project! Your sponsorship helps us continue to develop and maintain this project. Here are some ways you can sponsor us:

- **GitHub Sponsors**: You can sponsor us directly through GitHub Sponsors. Visit our [GitHub Sponsors page](https://github.com/sponsors/zarfld) to learn more and become a sponsor.
- **PayPal**: Make a one-time donation via PayPal. Visit our [PayPal donation page](https://www.paypal.com/donate/?business=YJFUYJK44JFX6&no_recurring=0&item_name=Supporting+the+LinuxCnc_PokeysLibComp+project%21+Your+sponsorship+helps+us+continue+to+develop+and+maintain+this+project.&currency_code=EUR) to contribute.

For more detailed information on sponsorship tiers and benefits, as well as instructions on how to sponsor through GitHub Sponsors and PayPal, please refer to our [Sponsorship Page](.github/SPONSORS.md).

Thank you for your support!
