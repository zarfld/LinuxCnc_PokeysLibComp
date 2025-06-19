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

## Getting Help

For further details consult the PDF manuals in the `used_Manuals` directory. They contain wiring diagrams, additional specifications and complete feature lists for each board.
