# User Documentation

Welcome to the User Documentation section of the **LinuxCnc_PokeysLibComp** project Wiki! This section provides detailed guides for how end-users can utilize the repository or software. It includes usage examples and practical scenarios to help you get started.

## Overview

The **LinuxCnc_PokeysLibComp** project integrates PoKeys devices with LinuxCNC, providing real-time control and monitoring capabilities. This repository contains the necessary components, scripts, and documentation to set up and use PoKeys devices with LinuxCNC.

## Getting Started

### Installation

To install the LinuxCnc_PokeysLibComp project, follow these steps:

1. **Clone the Repository**: Start by cloning the repository to your local machine.
   ```bash
   git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```

2. **Run the Install Script**: Execute the install script to set up the project.
   ```bash
   sudo sh install.sh
   ```

### Hardware Requirements

The project has been tested with the following hardware:

- RaspberryPi4 (for LinuxCNC)
- PoKeys57E
- PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir
- CNC keyboard – PoNETkbd48CNC
- PoRelay8 – Relay extension board
- PoExtBusOC16
- PoKeys57CNC
- PoStep25-32 (v1.3)

## Usage Examples

### Digital Input and Output

#### Reading Digital Inputs

To read the state of a digital input pin, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the state of digital input pin 0
pin_value = device.digital_read(0)
print(f"Digital input pin 0 state: {pin_value}")
```

#### Writing Digital Outputs

To write a value to a digital output pin, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Set the state of digital output pin 0 to high
device.digital_write(0, 1)
print("Digital output pin 0 set to high")
```

### Analog Input and Output

#### Reading Analog Inputs

To read the value of an analog input pin, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the value of analog input pin 0
analog_value = device.analog_read(0)
print(f"Analog input pin 0 value: {analog_value}")
```

#### Writing Analog Outputs

To write a value to an analog output pin, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Set the value of analog output pin 0 to 2.5V
device.analog_write(0, 2.5)
print("Analog output pin 0 set to 2.5V")
```

### Pulse Engine (PEv2)

#### Configuring the Pulse Engine

To configure the Pulse Engine (PEv2) for motion control, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Configure the Pulse Engine for axis 0
device.pev2_configure_axis(0, steps_per_unit=200, max_velocity=1000, acceleration=500)
print("Pulse Engine axis 0 configured")
```

#### Moving an Axis

To move an axis using the Pulse Engine, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Move axis 0 to position 10.0
device.pev2_move_axis(0, 10.0)
print("Axis 0 moved to position 10.0")
```

### Matrix Keyboard

#### Reading Matrix Keyboard

To read the state of a matrix keyboard, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the state of the matrix keyboard
key_state = device.matrix_keyboard_read()
print(f"Matrix keyboard state: {key_state}")
```

### CNC Keyboard (PoNETkbd48CNC)

#### Setting LED State

To set the state of an LED on the CNC keyboard, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Set the state of LED 0 to on
device.kbd48CNC_set_led(0, 1)
print("LED 0 on the CNC keyboard set to on")
```

#### Reading Button State

To read the state of a button on the CNC keyboard, use the following example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the state of button 0 on the CNC keyboard
button_state = device.kbd48CNC_get_button(0)
print(f"Button 0 on the CNC keyboard state: {button_state}")
```

## Configuration Steps for PoKeys Devices with LinuxCNC

### Step 1: Connect the Hardware

Ensure that the PoKeys devices are properly connected to your system according to the hardware requirements.

### Step 2: Update LinuxCNC Configuration Files

Update the LinuxCNC configuration files to include the PoKeys components. Refer to the following example configuration:

```ini
[HAL]
# Load PoKeys HAL component
loadrt pokeys

# Add PoKeys device
addf pokeys.0 servo-thread

# Configure digital inputs and outputs
net input-0 <= pokeys.0.digin.0.in
net output-1 => pokeys.0.digout.1.out

# Configure analog inputs and outputs
net analog-input-0 <= pokeys.0.adcin.0.value
net analog-output-1 => pokeys.0.adcout.1.value
```

### Step 3: Verify Connections

Ensure that the PoKeys devices are properly connected and recognized by LinuxCNC. Use the `halcmd` command to monitor the state of the inputs and outputs.

## Troubleshooting Steps for Common Issues

### Issue 1: The PoKeys device is not detected by LinuxCNC

**Solution**:
1. Ensure that the PoKeys device is properly connected to the RaspberryPi4 or other supported hardware.
2. Verify that the necessary drivers and dependencies are installed.
3. Check the device connection using the `lsusb` command to ensure it is recognized by the system.
4. Restart the LinuxCNC service and check the logs for any error messages.

### Issue 2: Digital inputs/outputs are not functioning as expected

**Solution**:
1. Verify the pin configuration in the HAL file to ensure the correct pins are assigned for digital inputs/outputs.
2. Check the wiring and connections to the PoKeys device to ensure they are secure.
3. Use the `halcmd` command to monitor the state of the digital inputs/outputs and verify their functionality.
4. Refer to the [User Documentation](User_Documentation.md) section for detailed guides on configuring digital inputs/outputs.

### Issue 3: Analog inputs/outputs are not providing accurate readings

**Solution**:
1. Verify the pin configuration in the HAL file to ensure the correct pins are assigned for analog inputs/outputs.
2. Check the wiring and connections to the PoKeys device to ensure they are secure.
3. Use the `halcmd` command to monitor the state of the analog inputs/outputs and verify their functionality.
4. Refer to the [User Documentation](User_Documentation.md) section for detailed guides on configuring analog inputs/outputs.

## Conclusion

This User Documentation section provides detailed guides and usage examples to help you get started with the **LinuxCnc_PokeysLibComp** project. If you have any questions or need further assistance, please refer to the [FAQ/Troubleshooting](FAQ_Troubleshooting.md) section or reach out to the community for support.
