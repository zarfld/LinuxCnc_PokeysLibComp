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
