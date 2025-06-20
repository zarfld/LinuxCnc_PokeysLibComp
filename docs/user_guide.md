# User Guide

This guide provides a quick overview of the PoKeys hardware used with **LinuxCnc_PokeysLibComp**. Information here was summarized from the vendor manuals located under `used_Manuals/`.

## Safety and Usage Notes

The PoLabs manuals emphasize several key safety guidelines:

- Information in the manuals may change without notice.
- PoLabs assumes no liability for patent or copyright issues arising from use of their products.
- Users are responsible for proper integration and fail‑safe design when building CNC equipment.
- Always consult the manuals for detailed specifications and wiring limits.

## Supported PoKeys Devices

The project primarily targets the PoKeys57 series of devices. The comparison chart in the manuals highlights differences between models:

| Model      | Digital I/O | Analog Inputs | Encoders | PWM Outputs |
|------------|-------------|---------------|----------|-------------|
| PoKeys57U  | 55          | 7 × 12‑bit    | up to 25 | 6           |
| PoKeys57E  | 55          | 7 × 12‑bit    | up to 25 | 6           |
| PoKeys57CNC| see pinout  | see pinout    | up to 14 | 4           |

Additional features include matrix keyboard support (up to 16×8), joystick emulation, and Pulse Engine v2 for step/dir motion control.

## Communication Interfaces

According to the protocol specification:

- **USB** devices use the standard HID driver with vendor ID `0x1DC3` and product ID `0x1001`.
- **Ethernet** devices communicate over TCP/UDP port `20055`. Device discovery uses broadcast UDP packets that return the user ID, serial number and IP address.
- Bulk communication is available on PoKeys57 firmware via WinUSB/libusb.

When multiple devices are present, use the serial number or user ID to select the desired unit.

## PoExtBus Overview

PoExtBus is a five‑pin expansion bus for connecting simple peripherals based on
74HC595 shift registers. PoKeys56 and PoKeys57 devices share the PoExtBus and
PoNET interfaces on a single connector. Only three signal wires are required to
control up to 80 outputs and slave boards can be daisy‑chained — up to ten
PoExtBusRE boards or five PoExtBusOC16 boards. Boards equipped with a PoExtension
connector must be linked with a 10‑pin ribbon cable, but only one of the
`PoExtBusIn` or `PoExtensionIn` connectors should be used at a time.

## Expansion Boards

### PoRelay8

An eight‑channel relay board that can be daisy‑chained with up to ten boards. It offers improved noise immunity and can connect over PoExtBus or CAN. Each channel exposes common, normally‑open and normally‑closed contacts.

### PoSensors

This module combines six sensors accessible via I2C:

- MMA7660 accelerometer
- MCP3425 16‑bit ADC
- BH1750 light sensor
- LM75BD temperature sensor
- SHT21 humidity and temperature sensor

Use the PoKeys configuration software to scan and add connected sensors.

### PoExtBusOC16CNC

Provides up to eight step‑and‑direction signal pairs to drive external motor drivers. It connects to a PoKeys device via a ribbon cable and supports clamp diodes for transient suppression. Refer to the user manual for wiring details and absolute maximum ratings.

### PoExtBusOC16

A 16‑channel open collector output board with integrated clamp diodes and
status LEDs. Up to five boards may be cascaded over PoExtBus. Each output can
sink up to **400&nbsp;mA** at a supply voltage of **40&nbsp;V**. Connect the power
supply positive to the **COM** terminal and the load between COM and the
`ocA`–`ocP` pins.

### PoExtBusRE

Relay expansion board with eight electromechanical relays. The board exposes
`Vin`, `NO` and `NC` contacts for each relay along with a `VCCre` power input.
A green power LED and eight red LEDs indicate relay state. Up to ten boards can
be chained. Contact ratings are **7&nbsp;A at 240&nbsp;VAC** or **10&nbsp;A at 125&nbsp;VAC / 28&nbsp;VDC**.
For parallel-input operation remove IC1 and drive the header with 3.3–5&nbsp;V
signals.

### PoNET kbd48CNC v2

PoNET kbd48CNC v2 is an upgraded 48‑key keyboard for PoKeys controllers. Each key includes a red LED backlight and the unit has a light sensor for adjusting brightness. Two status LEDs show
- **Solid on** – connected
- **Slow blink** – no PoKeys link
- **Fast blink** – bootloader
- **Off** – disabled

Key specifications from the manual:

| Spec | Value |
|------|------|
| Dimensions | 295 × 105 × 4 mm |
| Power supply | 5 V ±10 % |
| Max current | 500 mA |
| Keys/LEDs | 48 arranged in three 4×4 groups |
| Interface | PoNET via adapter (up to 3 m cable) or CAN on PoKeys57CNC v2 |

A PoNET_I2C extender board and several cables are included in the package. Firmware 6 or newer allows direct CAN bus connection—set the rear switch to position 2 and enable **PoCAN** in the configuration software. In matrix‑keyboard mode the keys can be mapped via the *Matrix Keyboard* menu. Default Mach4 mappings cover common actions like cycle start, feed hold, spindle control and jog speed adjustment.
## Pulse Engine v2

PoKeys Pulse Engine v2 provides step/dir motion control on PoKeys56U, PoKeys56E
and PoKeys57E devices. It works in several modes:

- **Integrated generator** – up to 3 axes at 25 kHz
- **External generator** – up to 8 axes at 125 kHz using the PoKeysCNCaddon or a
  custom 74HCT595 board

The 10‑pin motor connector exposes axis enable, direction, step and error
signals. Limit and home switch inputs are normally closed and should include
pull‑ups. An emergency switch on pin 52 stops pulse generation.

Additional features include three relay outputs, four open‑collector outputs
(80 V/50 mA), a 0–10 V PWM‑derived output and an optional 5 kHz charge‑pump on
pin 53. Soft limits, MPG jogging and operating states such as **Stopped**,
**Error**, **Homing**, **Probing** and **Running** are supported.

## PoKeys57CNC Highlights

### Connector Overview
- External **6–26&nbsp;V** supply is required for stable operation. A jumper selects between USB power (for testing) and the external supply.
- The board exposes connectors for SSR outputs, relays, galvanically isolated open collectors and a 0–10&nbsp;V analog output.
- Dedicated headers are provided for an LCD module, pendant, encoders, analog inputs and eight step/dir motor drivers.
- Limit and home switches per axis are available along with additional inputs for an emergency stop.

### E-stop Wiring
The manual notes that the emergency switch may be wired either to the 4‑pin E‑stop connector or to the pendant connector. When using the pendant connector the `NeST` jumper must remain in place. If only the dedicated connector is used, move the jumper to bridge pins&nbsp;4 and&nbsp;6 on the pendant header.

### Failsafe Configuration
A failsafe timeout can be configured so that digital outputs, PoExtBus devices and PWM channels revert to a defined state if communication is lost. The Pulse engine automatically enters emergency mode once the timeout expires.

### Network Security and Web Interface
Network‑enabled devices support password protection with three access levels: *full access*, *read‑only* and *full lock*. A simple web dashboard is enabled by default and can be disabled or configured to allow anonymous access or output control.

## Getting Help

For further details consult the PDF manuals in the `used_Manuals` directory. They contain wiring diagrams, additional specifications and complete feature lists for each board.
