# Converting Existing LinuxCNC HAL and INI Files for Use with `pokeys.comp`, `pokeys_homecomp.comp`, and `kbd48cnc.comp`

This guide provides detailed instructions on converting existing LinuxCNC HAL and INI files for use with `pokeys.comp`, `pokeys_homecomp.comp`, and `kbd48cnc.comp`. Follow the steps below to update your configuration files.

## Step 1: Identify Existing HAL Pins

Identify the existing HAL pins in your configuration files, such as `DM542_XXYZ_mill/custom.hal` and `DM542_XXYZ_mill/custom_postgui.hal`.

## Step 2: Determine Corresponding New Component Pins

Determine the corresponding new component pins by referring to the new component files, such as `pokeys_homecomp.comp`, `kbd48CNC.comp`, and `pokeys_homing.hal`.

## Step 3: Update HAL Configuration Files

Update the HAL configuration files to map the existing pins to the new component pins. For example, in `pokeys_homing.hal`, you can see how the pins are mapped between `pokeys_homecomp` and `pokeys` components.

### Example Mapping

Here is an example of mapping existing HAL pins to new component pins:

**Existing HAL Pin:**
```
net axis-select-z  halui.axis.z.select
```

**New Component Pin:**
```
net axis-select-z  pokeys.kbd48CNC.JOG_X.Button
```

## Step 4: Ensure Correct Connection and Configuration

Ensure that the new component pins are correctly connected and configured in the HAL files. Verify that all necessary connections are made and that the configuration is consistent with the new components.

## Step 5: Update INI Configuration Files

Update the INI configuration files to reflect any changes made in the HAL files. Ensure that the INI settings are consistent with the new components and their configuration.

## Step 6: Configure Pulse Engine (PEv2) in `pokeys.comp`

To configure the Pulse engine (PEv2) in `pokeys.comp`, you need to set the following INI settings in your configuration file:

* `DEVICE_ID`: Set the device ID for the PoKeys device.
* `ApplyIniSettings`: Set to `1` to apply settings parametrized in LinuxCNC.
* `PEv2_ProbeInput`: Set the probe input pin.
* `PEv2_ProbeInputPolarity`: Set the probe input polarity.
* `PEv2_EmergencyInputPin`: Set the emergency input pin.
* `PEv2_EmergencyOutputPin`: Set the emergency output pin.
* `PEv2_PulseGeneratorType`: Set the pulse generator type (0 for external, 1 for 3ch integrated, 2 for external smart).
* `PEv2_PG_swap_stepdir`: Set to `0` to disable swapping of step and direction signals.
* `PEv2_PG_extended_io`: Set to `1` to enable extended I/O.

For each axis, you need to set the following settings:

* `PEv2_AxisEnabled_<n>`: Set to `1` to enable the axis.
* `PEv2_AxesSwitchConfig_<n>`: Set the axis switch configuration.
* `PEv2_FilterLimitMSwitch_<n>`: Set the filter for the limit- switch.
* `PEv2_FilterLimitPSwitch_<n>`: Set the filter for the limit+ switch.
* `PEv2_PinHomeSwitch_<n>`: Set the home switch pin.
* `PEv2_PinLimitMSwitch_<n>`: Set the limit- switch pin.
* `PEv2_PinLimitPSwitch_<n>`: Set the limit+ switch pin.
* `PEv2_InvertLimitN_<n>`: Set to `1` to invert the limit- switch.
* `PEv2_EnableLimitN_<n>`: Set to `1` to enable the limit- switch.
* `PEv2_InvertLimitP_<n>`: Set to `1` to invert the limit+ switch.
* `PEv2_EnableLimitP_<n>`: Set to `1` to enable the limit+ switch.
* `PEv2_InvertHome_<n>`: Set to `1` to invert the home switch.
* `PEv2_EnableHome_<n>`: Set to `1` to enable the home switch.
* `PEv2_HomeOnLimitN_<n>`: Set to `0` to disable homing on the limit- switch.
* `PEv2_HomeOnLimitP_<n>`: Set to `1` to enable homing on the limit+ switch.
* `PEv2_AxisEnableOutputPins_<n>`: Set the axis enable output pins.
* `PEv2_Feedback_Encoder_Id_<n>`: Set the feedback encoder ID.
* `PEv2_InvertAxisEnable_<n>`: Set to `1` to invert the axis enable signal.

These settings can be found in the INI files such as `DM542_XXYZ_mill/Pokeys57CNC_debug_empty.ini`, `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini`, and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini`. Make sure to update your INI file with the appropriate values for your specific configuration.

## Additional Resources

Refer to the following files for more examples and detailed information:

- `DM542_XXYZ_mill/custom.hal`
- `DM542_XXYZ_mill/custom_postgui.hal`
- `pokeys_homecomp.comp`
- `kbd48CNC.comp`
- `pokeys_homing.hal`
- `DM542_XXYZ_mill/Pokeys57CNC_debug_empty.ini`
- `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini`
- `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini`

By following these steps, you can successfully convert your existing LinuxCNC HAL and INI files for use with `pokeys.comp`, `pokeys_homecomp.comp`, and `kbd48cnc.comp`.

## Mapping Existing HAL Pins to New Components

To map existing HAL pins to the new components, follow these steps:

1. Identify the existing HAL pins in your configuration files, such as `DM542_XXYZ_mill/custom.hal` and `DM542_XXYZ_mill/custom_postgui.hal`.
2. Determine the corresponding new component pins by referring to the new component files, such as `pokeys_homecomp.comp`, `kbd48CNC.comp`, and `pokeys_homing.hal`.
3. Update the HAL configuration files to map the existing pins to the new component pins. For example, in `pokeys_homing.hal`, you can see how the pins are mapped between `pokeys_homecomp` and `pokeys` components.
4. Ensure that the new component pins are correctly connected and configured in the HAL files.

### Example Mapping

Here is an example of mapping existing HAL pins to new component pins:

**Existing HAL Pin:**
```
net axis-select-z  halui.axis.z.select
```

**New Component Pin:**
```
net axis-select-z  pokeys.kbd48CNC.JOG_X.Button
```

## Required INI Settings for Pulse Engine (PEv2) in `pokeys.comp`

To configure the Pulse engine (PEv2) in `pokeys.comp`, you need to set the following INI settings in your configuration file:

* `DEVICE_ID`: Set the device ID for the PoKeys device.
* `ApplyIniSettings`: Set to `1` to apply settings parametrized in LinuxCNC.
* `PEv2_ProbeInput`: Set the probe input pin.
* `PEv2_ProbeInputPolarity`: Set the probe input polarity.
* `PEv2_EmergencyInputPin`: Set the emergency input pin.
* `PEv2_EmergencyOutputPin`: Set the emergency output pin.
* `PEv2_PulseGeneratorType`: Set the pulse generator type (0 for external, 1 for 3ch integrated, 2 for external smart).
* `PEv2_PG_swap_stepdir`: Set to `0` to disable swapping of step and direction signals.
* `PEv2_PG_extended_io`: Set to `1` to enable extended I/O.

For each axis, you need to set the following settings:

* `PEv2_AxisEnabled_<n>`: Set to `1` to enable the axis.
* `PEv2_AxesSwitchConfig_<n>`: Set the axis switch configuration.
* `PEv2_FilterLimitMSwitch_<n>`: Set the filter for the limit- switch.
* `PEv2_FilterLimitPSwitch_<n>`: Set the filter for the limit+ switch.
* `PEv2_PinHomeSwitch_<n>`: Set the home switch pin.
* `PEv2_PinLimitMSwitch_<n>`: Set the limit- switch pin.
* `PEv2_PinLimitPSwitch_<n>`: Set the limit+ switch pin.
* `PEv2_InvertLimitN_<n>`: Set to `1` to invert the limit- switch.
* `PEv2_EnableLimitN_<n>`: Set to `1` to enable the limit- switch.
* `PEv2_InvertLimitP_<n>`: Set to `1` to invert the limit+ switch.
* `PEv2_EnableLimitP_<n>`: Set to `1` to enable the limit+ switch.
* `PEv2_InvertHome_<n>`: Set to `1` to invert the home switch.
* `PEv2_EnableHome_<n>`: Set to `1` to enable the home switch.
* `PEv2_HomeOnLimitN_<n>`: Set to `0` to disable homing on the limit- switch.
* `PEv2_HomeOnLimitP_<n>`: Set to `1` to enable homing on the limit+ switch.
* `PEv2_AxisEnableOutputPins_<n>`: Set the axis enable output pins.
* `PEv2_Feedback_Encoder_Id_<n>`: Set the feedback encoder ID.
* `PEv2_InvertAxisEnable_<n>`: Set to `1` to invert the axis enable signal.

These settings can be found in the INI files such as `DM542_XXYZ_mill/Pokeys57CNC_debug_empty.ini`, `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini`, and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini`. Make sure to update your INI file with the appropriate values for your specific configuration.

## Documenting the Purpose of Each Line in HAL Files Related to PEv2

The following lines in the HAL files are related to PEv2 and their purposes are described below:

* In `DM542_XXYZ_mill/pokeys_homing.hal`:
  * `net PulseEngineState <= pokeys.0.PEv2.PulseEngineState`: Connects the Pulse Engine state to the `pokeys` component.
  * `net x-AxesState <= joint.0.PEv2.AxesState`: Connects the X-axis state to the `pokeys` component.
  * `net x2-AxesState <= joint.1.PEv2.AxesState`: Connects the X2-axis state to the `pokeys` component.
  * `net y-AxesState <= joint.2.PEv2.AxesState`: Connects the Y-axis state to the `pokeys` component.
  * `net z-AxesState <= joint.3.PEv2.AxesState`: Connects the Z-axis state to the `pokeys` component.
  * `net x-AxesCommand <= joint.0.PEv2.AxesCommand`: Connects the X-axis command to the `pokeys` component.
  * `net x2-AxesCommand <= joint.1.PEv2.AxesCommand`: Connects the X2-axis command to the `pokeys` component.
  * `net y-AxesCommand <= joint.2.PEv2.AxesCommand`: Connects the Y-axis command to the `pokeys` component.
  * `net z-AxesCommand <= joint.3.PEv2.AxesCommand`: Connects the Z-axis command to the `pokeys` component.
  * `net x-AxesConfig <= joint.0.PEv2.AxesConfig`: Connects the X-axis configuration to the `pokeys` component.
  * `net x2-AxesConfig <= joint.1.PEv2.AxesConfig`: Connects the X2-axis configuration to the `pokeys` component.
  * `net y-AxesConfig <= joint.2.PEv2.AxesConfig`: Connects the Y-axis configuration to the `pokeys` component.
  * `net z-AxesConfig <= joint.3.PEv2.AxesConfig`: Connects the Z-axis configuration to the `pokeys` component.
  * `setp pokeys.0.PEv2.0.home-sequence [JOINT_0]HOME_SEQUENCE`: Sets the home sequence for the X-axis.
  * `setp pokeys.0.PEv2.6.home-sequence [JOINT_1]HOME_SEQUENCE`: Sets the home sequence for the X2-axis.
  * `setp pokeys.0.PEv2.1.home-sequence [JOINT_2]HOME_SEQUENCE`: Sets the home sequence for the Y-axis.
  * `setp pokeys.0.PEv2.2.home-sequence [JOINT_3]HOME_SEQUENCE`: Sets the home sequence for the Z-axis.
  * `net x-homing <= pokeys-homecomp.0.joint.0.homing`: Connects the X-axis homing signal to the `pokeys` component.
  * `net x2-homing <= pokeys-homecomp.0.joint.1.homing`: Connects the X2-axis homing signal to the `pokeys` component.
  * `net y-homing <= pokeys-homecomp.0.joint.2.homing`: Connects the Y-axis homing signal to the `pokeys` component.
  * `net z-homing <= pokeys-homecomp.0.joint.3.homing`: Connects the Z-axis homing signal to the `pokeys` component.
  * `net x-homed <= pokeys-homecomp.0.joint.0.homed`: Connects the X-axis homed signal to the `pokeys` component.
  * `net x2-homed <= pokeys-homecomp.0.joint.1.homed`: Connects the X2-axis homed signal to the `pokeys` component.
  * `net y-homed <= pokeys-homecomp.0.joint.2.homed`: Connects the Y-axis homed signal to the `pokeys` component.
  * `net z-homed <= pokeys-homecomp.0.joint.3.homed`: Connects the Z-axis homed signal to the `pokeys` component.
  * `net x-home-state <= pokeys-homecomp.0.joint.0.home-state`: Connects the X-axis home state to the `pokeys` component.
  * `net x2-home-state <= pokeys-homecomp.0.joint.1.home-state`: Connects the X2-axis home state to the `pokeys` component.
  * `net y-home-state <= pokeys-homecomp.0.joint.2.home-state`: Connects the Y-axis home state to the `pokeys` component.
  * `net z-home-state <= pokeys-homecomp.0.joint.3.home-state`: Connects the Z-axis home state to the `pokeys` component.
  * `net x-index-enable <= pokeys-homecomp.0.joint.0.index-enable`: Connects the X-axis index enable signal to the `pokeys` component.
  * `net x2-index-enable <= pokeys-homecomp.0.joint.1.index-enable`: Connects the X2-axis index enable signal to the `pokeys` component.
  * `net y-index-enable <= pokeys-homecomp.0.joint.2.index-enable`: Connects the Y-axis index enable signal to the `pokeys` component.
  * `net z-index-enable <= pokeys-homecomp.0.joint.3.index-enable`: Connects the Z-axis index enable signal to the `pokeys` component.
  * `net x-PEv2_AxesState <= pokeys-homecomp.0.joint.0.PEv2_AxesState`: Connects the X-axis PEv2 state to the `pokeys` component.
  * `net x2-PEv2_AxesState <= pokeys-homecomp.0.joint.1.PEv2_AxesState`: Connects the X2-axis PEv2 state to the `pokeys` component.
  * `net y-PEv2_AxesState <= pokeys-homecomp.0.joint.2.PEv2_AxesState`: Connects the Y-axis PEv2 state to the `pokeys` component.
  * `net z-PEv2_AxesState <= pokeys-homecomp.0.joint.3.PEv2_AxesState`: Connects the Z-axis PEv2 state to the `pokeys` component.
  * `net x-PEv2_AxesCommand <= pokeys-homecomp.0.joint.0.PEv2_AxesCommand`: Connects the X-axis PEv2 command to the `pokeys` component.
  * `net x2-PEv2_AxesCommand <= pokeys-homecomp.0.joint.1.PEv2_AxesCommand`: Connects the X2-axis PEv2 command to the `pokeys` component.
  * `net y-PEv2_AxesCommand <= pokeys-homecomp.0.joint.2.PEv2_AxesCommand`: Connects the Y-axis PEv2 command to the `pokeys` component.
  * `net z-PEv2_AxesCommand <= pokeys-homecomp.0.joint.3.PEv2_AxesCommand`: Connects the Z-axis PEv2 command to the `pokeys` component.
  * `net x-PEv2_AxesConfig <= pokeys-homecomp.0.joint.0.PEv2_AxesConfig`: Connects the X-axis PEv2 configuration to the `pokeys` component.
  * `net x2-PEv2_AxesConfig <= pokeys-homecomp.0.joint.1.PEv2_AxesConfig`: Connects the X2-axis PEv2 configuration to the `pokeys` component.
  * `net y-PEv2_AxesConfig <= pokeys-homecomp.0.joint.2.PEv2_AxesConfig`: Connects the Y-axis PEv2 configuration to the `pokeys` component.
  * `net z-PEv2_AxesConfig <= pokeys-homecomp.0.joint.3.PEv2_AxesConfig`: Connects the Z-axis PEv2 configuration to the `pokeys` component.
  * `net x-PEv2_AxesSwitchConfig <= pokeys-homecomp.0.joint.0.PEv2_AxesSwitchConfig`: Connects the X-axis PEv2 switch configuration to the `pokeys` component.
  * `net x2-PEv2_AxesSwitchConfig <= pokeys-homecomp.0.joint.1.PEv2_AxesSwitchConfig`: Connects the X2-axis PEv2 switch configuration to the `pokeys` component.
  * `net y-PEv2_AxesSwitchConfig <= pokeys-homecomp.0.joint.2.PEv2_AxesSwitchConfig`: Connects the Y-axis PEv2 switch configuration to the `pokeys` component.
  * `net z-PEv2_AxesSwitchConfig <= pokeys-homecomp.0.joint.3.PEv2_AxesSwitchConfig`: Connects the Z-axis PEv2 switch configuration to the `pokeys` component.
  * `net x-PEv2_HomingSpeed <= pokeys-homecomp.0.joint.0.PEv2_HomingSpeed`: Connects the X-axis PEv2 homing speed to the `pokeys` component.
  * `net x2-PEv2_HomingSpeed <= pokeys-homecomp.0.joint.1.PEv2_HomingSpeed`: Connects the X2-axis PEv2 homing speed to the `pokeys` component.
  * `net y-PEv2_HomingSpeed <= pokeys-homecomp.0.joint.2.PEv2_HomingSpeed`: Connects the Y-axis PEv2 homing speed to the `pokeys` component.
  * `net z-PEv2_HomingSpeed <= pokeys-homecomp.0.joint.3.PEv2_HomingSpeed`: Connects the Z-axis PEv2 homing speed to the `pokeys` component.
  * `net x-PEv2_HomingReturnSpeed <= pokeys-homecomp.0.joint.0.PEv2_HomingReturnSpeed`: Connects the X-axis PEv2 homing return speed to the `pokeys` component.
  * `net x2-PEv2_HomingReturnSpeed <= pokeys-homecomp.0.joint.1.PEv2_HomingReturnSpeed`: Connects the X2-axis PEv2 homing return speed to the `pokeys` component.
  * `net y-PEv2_HomingReturnSpeed <= pokeys-homecomp.0.joint.2.PEv2_HomingReturnSpeed`: Connects the Y-axis PEv2 homing return speed to the `pokeys` component.
  * `net z-PEv2_HomingReturnSpeed <= pokeys-homecomp.0.joint.3.PEv2_HomingReturnSpeed`: Connects the Z-axis PEv2 homing return speed to the `pokeys` component.
  * `net x-PEv2_HomingAlgorithm <= pokeys-homecomp.0.joint.0.PEv2_HomingAlgorithm`: Connects the X-axis PEv2 homing algorithm to the `pokeys` component.
  * `net x2-PEv2_HomingAlgorithm <= pokeys-homecomp.0.joint.1.PEv2_HomingAlgorithm`: Connects the X2-axis PEv2 homing algorithm to the `pokeys` component.
  * `net y-PEv2_HomingAlgorithm <= pokeys-homecomp.0.joint.2.PEv2_HomingAlgorithm`: Connects the Y-axis PEv2 homing algorithm to the `pokeys` component.
  * `net z-PEv2_HomingAlgorithm <= pokeys-homecomp.0.joint.3.PEv2_HomingAlgorithm`: Connects the Z-axis PEv2 homing algorithm to the `pokeys` component.
  * `net x-PEv2_HomeOffsets <= pokeys-homecomp.0.joint.0.PEv2_HomeOffsets`: Connects the X-axis PEv2 home offsets to the `pokeys` component.
  * `net x2-PEv2_HomeOffsets <= pokeys-homecomp.0.joint.1.PEv2_HomeOffsets`: Connects the X2-axis PEv2 home offsets to the `pokeys` component.
  * `net y-PEv2_HomeOffsets <= pokeys-homecomp.0.joint.2.PEv2_HomeOffsets`: Connects the Y-axis PEv2 home offsets to the `pokeys` component.
  * `net z-PEv2_HomeOffsets <= pokeys-homecomp.0.joint.3.PEv2_HomeOffsets`: Connects the Z-axis PEv2 home offsets to the `pokeys` component.
  * `net x-PEv2_HomeBackOffDistance <= pokeys-homecomp.0.joint.0.PEv2_HomeBackOffDistance`: Connects the X-axis PEv2 home back-off distance to the `pokeys` component.
  * `net x2-PEv2_HomeBackOffDistance <= pokeys-homecomp.0.joint.1.PEv2_HomeBackOffDistance`: Connects the X2-axis PEv2 home back-off distance to the `pokeys` component.
  * `net y-PEv2_HomeBackOffDistance <= pokeys-homecomp.0.joint.2.PEv2_HomeBackOffDistance`: Connects the Y-axis PEv2 home back-off distance to the `pokeys` component.
  * `net z-PEv2_HomeBackOffDistance <= pokeys-homecomp.0.joint.3.PEv2_HomeBackOffDistance`: Connects the Z-axis PEv2 home back-off distance to the `pokeys` component.

## Examples of Converting Existing HAL and INI Files

Here are some examples of converting existing HAL and INI files for use with `pokeys.comp`, `pokeys_homecomp.comp`, and `kbd48cnc.comp`.

### Example 1: Converting `DM542_XXYZ_mill/custom.hal`

**Original `custom.hal` File:**
```
# Include your custom HAL commands here
# This file will not be overwritten when you run PNCconf again

net axis-select-z  halui.axis.z.select
```

**Converted `custom.hal` File:**
```
# Include your custom HAL commands here
# This file will not be overwritten when you run PNCconf again

net axis-select-z  pokeys.kbd48CNC.JOG_X.Button
```

### Example 2: Converting `DM542_XXYZ_mill/custom_postgui.hal`

**Original `custom_postgui.hal` File:**
```
# Include your custom_postgui HAL commands here
# This file will not be overwritten when you run PNCconf again

net axis-select-z  halui.axis.z.select
```

**Converted `custom_postgui.hal` File:**
```
# Include your custom_postgui HAL commands here
# This file will not be overwritten when you run PNCconf again

net axis-select-z  pokeys.kbd48CNC.JOG_X.Button
```

### Example 3: Converting `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini`

**Original `Pokeys57CNC_DM542_XXYZ_mill.ini` File:**
```
[POKEYS]
DEVICE_ID=53386
ApplyIniSettings=1
PEv2_ProbeInput=19
PEv2_ProbeInputPolarity=0
PEv2_EmergencyInputPin=52
PEv2_EmergencyOutputPin=0
PEv2_PulseGeneratorType=0
PEv2_PG_swap_stepdir=0
PEv2_PG_extended_io=1
PEv2_AxisEnabled_0=1
PEv2_AxesSwitchConfig_0=115
PEv2_FilterLimitMSwitch_0=0
PEv2_FilterLimitPSwitch_0=0
PEv2_PinHomeSwitch_0=0
PEv2_PinLimitMSwitch_0=0
PEv2_PinLimitPSwitch_0=0
PEv2_InvertLimitN_0=1
PEv2_EnableLimitN_0=1
PEv2_InvertLimitP_0=1
PEv2_EnableLimitP_0=0
PEv2_InvertHome_0=1
PEv2_EnableHome_0=1
PEv2_HomeOnLimitN_0=0
PEv2_HomeOnLimitP_0=1
PEv2_AxisEnableOutputPins_0=0
PEv2_Feedback_Encoder_Id_0=0
PEv2_InvertAxisEnable_0=1
PEv2_AxisEnabled_1=1
PEv2_AxesSwitchConfig_1=133
PEv2_FilterLimitMSwitch_1=0
PEv2_FilterLimitPSwitch_1=0
PEv2_PinHomeSwitch_1=0
PEv2_PinLimitMSwitch_1=0
PEv2_PinLimitPSwitch_1=0
PEv2_InvertLimitN_1=1
PEv2_EnableLimitN_1=1
PEv2_InvertLimitP_1=1
PEv2_EnableLimitP_1=0
PEv2_InvertHome_1=1
PEv2_EnableHome_1=1
PEv2_HomeOnLimitN_1=0
PEv2_HomeOnLimitP_1=0
PEv2_AxisEnableOutputPins_1=0
PEv2_Feedback_Encoder_Id_1=0
PEv2_InvertAxisEnable_1=1
PEv2_AxesSwitchConfig_2=133
PEv2_FilterLimitMSwitch_2=0
PEv2_FilterLimitPSwitch_2=0
PEv2_PinHomeSwitch_2=0
PEv2_PinLimitMSwitch_2=0
PEv2_PinLimitPSwitch_2=0
PEv2_InvertLimitN_2=1
PEv2_EnableLimitN_2=0
PEv2_InvertLimitP_2=0
PEv2_EnableLimitP_2=0
PEv2_InvertHome_2=1
PEv2_EnableHome_2=1
PEv2_HomeOnLimitN_2=0
PEv2_HomeOnLimitP_2=0
PEv2_AxisEnableOutputPins_2=0
PEv2_Feedback_Encoder_Id_2=0
PEv2_InvertAxisEnable_2=1
PEv2_AxesSwitchConfig_6=115
PEv2_FilterLimitMSwitch_6=0
PEv2_FilterLimitPSwitch_6=0
PEv2_PinHomeSwitch_6=0
PEv2_PinLimitMSwitch_6=0
PEv2_PinLimitPSwitch_6=0
PEv2_InvertLimitN_6=1
PEv2_EnableLimitN_6=1
PEv2_InvertLimitP_6=1
PEv2_EnableLimitP_6=1
PEv2_InvertHome_6=1
PEv2_EnableHome_6=1
PEv2_HomeOnLimitN_6=0
PEv2_HomeOnLimitP_6=1
PEv2_AxisEnableOutputPins_6=0
PEv2_Feedback_Encoder_Id_6=0
PEv2_InvertAxisEnable_6=1
```

**Converted `Pokeys57CNC_DM542_XXYZ_mill.ini` File:**
```
[POKEYS]
DEVICE_ID=53386
ApplyIniSettings=1
PEv2_ProbeInput=19
PEv2_ProbeInputPolarity=0
PEv2_EmergencyInputPin=52
PEv2_EmergencyOutputPin=0
PEv2_PulseGeneratorType=0
PEv2_PG_swap_stepdir=0
PEv2_PG_extended_io=1
PEv2_AxisEnabled_0=1
PEv2_AxesSwitchConfig_0=115
PEv2_FilterLimitMSwitch_0=0
PEv2_FilterLimitPSwitch_0=0
PEv2_PinHomeSwitch_0=0
PEv2_PinLimitMSwitch_0=0
PEv2_PinLimitPSwitch_0=0
PEv2_InvertLimitN_0=1
PEv2_EnableLimitN_0=1
PEv2_InvertLimitP_0=1
PEv2_EnableLimitP_0=0
PEv2_InvertHome_0=1
PEv2_EnableHome_0=1
PEv2_HomeOnLimitN_0=0
PEv2_HomeOnLimitP_0=1
PEv2_AxisEnableOutputPins_0=0
PEv2_Feedback_Encoder_Id_0=0
PEv2_InvertAxisEnable_0=1
PEv2_AxisEnabled_1=1
PEv2_AxesSwitchConfig_1=133
PEv2_FilterLimitMSwitch_1=0
PEv2_FilterLimitPSwitch_1=0
PEv2_PinHomeSwitch_1=0
PEv2_PinLimitMSwitch_1=0
PEv2_PinLimitPSwitch_1=0
PEv2_InvertLimitN_1=1
PEv2_EnableLimitN_1=1
PEv2_InvertLimitP_1=1
PEv2_EnableLimitP_1=0
PEv2_InvertHome_1=1
PEv2_EnableHome_1=1
PEv2_HomeOnLimitN_1=0
PEv2_HomeOnLimitP_1=0
PEv2_AxisEnableOutputPins_1=0
PEv2_Feedback_Encoder_Id_1=0
PEv2_InvertAxisEnable_1=1
PEv2_AxesSwitchConfig_2=133
PEv2_FilterLimitMSwitch_2=0
PEv2_FilterLimitPSwitch_2=0
PEv2_PinHomeSwitch_2=0
PEv2_PinLimitMSwitch_2=0
PEv2_PinLimitPSwitch_2=0
PEv2_InvertLimitN_2=1
PEv2_EnableLimitN_2=0
PEv2_InvertLimitP_2=0
PEv2_EnableLimitP_2=0
PEv2_InvertHome_2=1
PEv2_EnableHome_2=1
PEv2_HomeOnLimitN_2=0
PEv2_HomeOnLimitP_2=0
PEv2_AxisEnableOutputPins_2=0
PEv2_Feedback_Encoder_Id_2=0
PEv2_InvertAxisEnable_2=1
PEv2_AxesSwitchConfig_6=115
PEv2_FilterLimitMSwitch_6=0
PEv2_FilterLimitPSwitch_6=0
PEv2_PinHomeSwitch_6=0
PEv2_PinLimitMSwitch_6=0
PEv2_PinLimitPSwitch_6=0
PEv2_InvertLimitN_6=1
PEv2_EnableLimitN_6=1
PEv2_InvertLimitP_6=1
PEv2_EnableLimitP_6=1
PEv2_InvertHome_6=1
PEv2_EnableHome_6=1
PEv2_HomeOnLimitN_6=0
PEv2_HomeOnLimitP_6=1
PEv2_AxisEnableOutputPins_6=0
PEv2_Feedback_Encoder_Id_6=0
PEv2_InvertAxisEnable_6=1
```
