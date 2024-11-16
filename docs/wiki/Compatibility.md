# Compatibility

## PoKeys Devices

The following table outlines the compatibility between different versions of PoKeys devices and the LinuxCnc_PokeysLibComp project:

| PoKeys Device | Supported Versions | Notes |
|---------------|--------------------|-------|
| PoKeys57E     | 4.0, 4.1, 4.2      | Limited support |
| PoKeys57CNC   | 3.5, 3.6, 3.7      | Limited support |
| PoKeys57U     | 2.8, 2.9           | Limited support |
| PoKeys56U     | 1.5, 1.6           | Limited support |

## LinuxCNC

The following table outlines the compatibility between different versions of LinuxCNC and the LinuxCnc_PokeysLibComp project:

| LinuxCNC Version | Supported | Notes |
|------------------|-----------|-------|
| 2.8.x            | Yes       | Limited support |
| 2.9.x            | Yes       | Limited support |
| 2.7.x            | No        | Not supported |    nothing of them is Fully supported yet. this sis still in development no approval of support done!

## Canonical Device Interface

The Canonical Device Interface (CDI) is a standardized interface for interacting with PoKeys devices in the LinuxCnc_PokeysLibComp project. It provides a consistent and unified way to access and control various features of PoKeys devices, ensuring compatibility and ease of use.

### Features

The CDI supports the following features:

- Digital Input and Output
- Analog Input and Output
- Pulse Engine (PEv2) Configuration and Control
- Matrix Keyboard Setup and Reading
- CNC Keyboard (PoNETkbd48CNC) LED and Button Control
- Encoder Reading
- Real-Time Clock

### Usage Examples

#### Digital Input and Output

To read the state of a digital input pin and write a value to a digital output pin using the CDI, follow these examples:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the state of digital input pin 0
pin_value = device.digital_read(0)
print(f"Digital input pin 0 state: {pin_value}")

# Set the state of digital output pin 0 to high
device.digital_write(0, 1)
print("Digital output pin 0 set to high")
```

#### Analog Input and Output

To read the value of an analog input pin and write a value to an analog output pin using the CDI, follow these examples:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the value of analog input pin 0
analog_value = device.analog_read(0)
print(f"Analog input pin 0 value: {analog_value}")

# Set the value of analog output pin 0 to 2.5V
device.analog_write(0, 2.5)
print("Analog output pin 0 set to 2.5V")
```

#### Pulse Engine (PEv2)

To configure the Pulse Engine (PEv2) for motion control and move an axis using the CDI, follow these examples:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Configure the Pulse Engine for axis 0
device.pev2_configure_axis(0, steps_per_unit=200, max_velocity=1000, acceleration=500)
print("Pulse Engine axis 0 configured")

# Move axis 0 to position 10.0
device.pev2_move_axis(0, 10.0)
print("Axis 0 moved to position 10.0")
```

#### Matrix Keyboard

To read the state of a matrix keyboard using the CDI, follow this example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the state of the matrix keyboard
key_state = device.matrix_keyboard_read()
print(f"Matrix keyboard state: {key_state}")
```

#### CNC Keyboard (PoNETkbd48CNC)

To set the state of an LED and read the state of a button on the CNC keyboard using the CDI, follow these examples:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Set the state of LED 0 to on
device.kbd48CNC_set_led(0, 1)
print("LED 0 on the CNC keyboard set to on")

# Read the state of button 0 on the CNC keyboard
button_state = device.kbd48CNC_get_button(0)
print(f"Button 0 on the CNC keyboard state: {button_state}")
```

#### Encoder

To read the value of an encoder using the CDI, follow this example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the value of encoder 0
encoder_value = device.encoder_read(0)
print(f"Encoder 0 value: {encoder_value}")
```

#### Real-Time Clock

To read the real-time clock values using the CDI, follow this example:

```python
import pokeys_py

# Initialize the PoKeys device
device = pokeys_py.PoKeysDevice()

# Read the real-time clock values
rtc_values = device.rtc_read()
print(f"RTC values: {rtc_values}")
```

### Configuration Steps

To configure the CDI for use with PoKeys devices in LinuxCNC, follow these steps:

1. **Connect the Hardware**: Ensure that the PoKeys devices are properly connected to your system according to the hardware requirements.

2. **Update LinuxCNC Configuration Files**: Update the LinuxCNC configuration files to include the PoKeys components. Refer to the following example configuration:

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

3. **Verify Connections**: Ensure that the PoKeys devices are properly connected and recognized by LinuxCNC. Use the `halcmd` command to monitor the state of the inputs and outputs.

By following these steps and examples, you can effectively use the Canonical Device Interface to interact with PoKeys devices in the LinuxCnc_PokeysLibComp project.

## Homing Functionality Compatibility

The following table outlines the compatibility of the homing functionality with different PoKeys devices and LinuxCNC versions:

| PoKeys Device | LinuxCNC Version | Homing Functionality Compatibility | Notes |
|---------------|------------------|------------------------------------|-------|
| PoKeys57E     | 2.8.x, 2.9.x     | Limited support                    | Homing functionality is implemented but not fully tested |
| PoKeys57CNC   | 2.8.x, 2.9.x     | Limited support                    | Homing functionality is implemented but not fully tested |
| PoKeys57U     | 2.8.x, 2.9.x     | Limited support                    | Homing functionality is implemented but not fully tested |
| PoKeys56U     | 2.8.x, 2.9.x     | Limited support                    | Homing functionality is implemented but not fully tested |
