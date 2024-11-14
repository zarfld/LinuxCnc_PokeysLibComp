# Machine Description

## Overview

The machine is a 3-axis milling machine with an additional X2 axis, making it a 4-axis machine in total. It uses a Pokeys57CNC controller and four DM542 drivers for the XYZ and X2 axes.

## Limit Switches

The limit switches are normally closed and are connected to the "PoOptoIn" which is a 16-channel industrial digital input isolator. The configuration files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini` show that the limit switches are set to be inverted, which means they are normally closed.

### Wiring

The limit switches for each axis are connected as follows:
- X axis (Joint 0) uses "Ax1 Home" and "Ax1 Limit -"
- X2 axis (Joint 1) uses "Ax6 Home" and "Ax6 Limit -"
- Y axis (Joint 2) uses "Ax2 Home" and "Ax2 Limit -"
- Z axis (Joint 3) uses "Ax3 Home" (with "Ax3 Limit -" reserved and not yet connected)

The "Ax* Limit +" is not connected on any axis.

### Configuration

The `PEv2_EnableLimitP_*` parameters are configured to enable the positive limit switches for AXIS_X and AXIS_X2, but not for AXIS_Y and AXIS_Z. This configuration can be found in the files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini`.

The `PEv2_AxesSwitchConfig_*` parameters are set according to the hardware setup and the desired behavior of the limit and home switches. These parameters are configured using bitwise OR combinations of various options, such as `PK_ASO_SWITCH_LIMIT_N`, `PK_ASO_SWITCH_LIMIT_P`, `PK_ASO_SWITCH_HOME`, and others.

### Limit Switches Used

The limit switches used in this setup are NJK-5002C.

### Documentation

The limit switch connection parameterization is documented correctly in the repository.

## Additional Information

The configuration files `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` and `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_homingdeb.ini` provide detailed information about the limit switches and their configuration. The file `DM542_XXYZ_mill/postgui_call_list.hal` sets the `PEv2_AxesSwitchConfig_*` parameters for each axis, ensuring consistency with the wiring and connections of the limit and home switches to the PoOptoIn.

## Signal Flow

The signal flow for the `DM542_XXYZ_mill` configuration is as follows:

* The `DM542_XXYZ_mill/kbd48CNC.hal` file defines the connections for the kbd48CNC keyboard, mapping various buttons to HALUI signals and other components.
* The `pokeys_homing.hal` file connects miscellaneous signals and defines the homing configuration for the axes, including linking pins from `pokeys_homecomp.comp` to PoKeys components.
* The `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.hal` file sets up the machine configuration, including loading necessary components, defining external input and output signals, and configuring the axes and spindle.
* The `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.ini` file provides the machine's INI configuration, including settings for the display, task, trajectory, and axes.
* The `DM542_XXYZ_mill/Pokeys57CNC_DM542_XXYZ_mill.md` file describes the machine, including the limit switches, wiring, and configuration details.
* The `DM542_XXYZ_mill/Pokeys57E_DM542_XXYZ_mill.hal` file is similar to the `Pokeys57CNC_DM542_XXYZ_mill.hal` file but is configured for the Pokeys57E device.
* The `DM542_XXYZ_mill/Pokeys57E_DM542_XXYZ_mill.ini` file provides the INI configuration for the Pokeys57E device.
* The `DM542_XXYZ_mill/postgui_call_list.hal` file loads additional HAL files and sets parameters for the PoKeys device.
* The `pokeys_homing.hal` file connects miscellaneous signals and defines the homing configuration for the axes, including linking pins from `pokeys_homecomp.comp` to PoKeys components.

These files collectively define the signal flow for the `DM542_XXYZ_mill` configuration, including the connections for the kbd48CNC keyboard, homing configuration, machine setup, and device-specific settings. The signal flow details are spread across these files, ensuring proper communication and control of the machine's components.

## HAL Pins

The HAL pins for the `DM542_XXYZ_mill` configuration are as follows:

* The `kbd48CNC.hal` file defines the connections for the kbd48CNC keyboard, mapping various buttons to HALUI signals and other components. For example, `net pokeys.kbd48CNC.CYCLE_START.Button` maps to `halui.program.run`, and `net pokeys.kbd48CNC.CYCLE_START.LED` maps to `halui.program.is-running`.
* The `pokeys_homing.hal` file connects miscellaneous signals and defines the homing configuration for the axes, including linking pins from `pokeys_homecomp.comp` to PoKeys components. For example, `net x-homing` maps to `pokeys.0.PEv2.0.homing`.
* The `Pokeys57CNC_DM542_XXYZ_mill.hal` file sets up the machine configuration, including loading necessary components, defining external input and output signals, and configuring the axes and spindle. For example, `net min-x` maps to `pokeys.0.PEv2.0.digin.LimitN.in`.
* The `Pokeys57E_DM542_XXYZ_mill.hal` file is similar to the `Pokeys57CNC_DM542_XXYZ_mill.hal` file but is configured for the Pokeys57E device. For example, `net min-x` maps to `pokeys.0.PEv2.0.digin.LimitN.in`.
* The `postgui_call_list.hal` file loads additional HAL files and sets parameters for the PoKeys device. For example, `setp pokeys.0.PEv2.digin.Probe.Pin` maps to `[POKEYS]PEv2_ProbeInput`.

These mappings ensure proper communication and control of the machine's components, including the limit switches, homing configuration, and emergency handling. The HAL pins are connected to the corresponding hardware pins based on the configuration files, ensuring the correct operation of the machine.

## Hardware Pins

The hardware pins for the `DM542_XXYZ_mill` configuration are as follows:

* The `kbd48CNC.hal` file defines the connections for the kbd48CNC keyboard, mapping various buttons to HALUI signals and other components. For example, `net pokeys.kbd48CNC.CYCLE_START.Button` maps to `halui.program.run`, and `net pokeys.kbd48CNC.CYCLE_START.LED` maps to `halui.program.is-running`.
* The `pokeys_homing.hal` file connects miscellaneous signals and defines the homing configuration for the axes, including linking pins from `pokeys_homecomp.comp` to PoKeys components. For example, `net x-homing` maps to `pokeys.0.PEv2.0.homing`.
* The `Pokeys57CNC_DM542_XXYZ_mill.hal` file sets up the machine configuration, including loading necessary components, defining external input and output signals, and configuring the axes and spindle. For example, `net min-x` maps to `pokeys.0.PEv2.0.digin.LimitN.in`.
* The `Pokeys57E_DM542_XXYZ_mill.hal` file is similar to the `Pokeys57CNC_DM542_XXYZ_mill.hal` file but is configured for the Pokeys57E device. For example, `net min-x` maps to `pokeys.0.PEv2.0.digin.LimitN.in`.
* The `postgui_call_list.hal` file loads additional HAL files and sets parameters for the PoKeys device. For example, `setp pokeys.0.PEv2.digin.Probe.Pin` maps to `[POKEYS]PEv2_ProbeInput`.

These mappings ensure proper communication and control of the machine's components, including the limit switches, homing configuration, and emergency handling. The HAL pins are connected to the corresponding hardware pins based on the configuration files, ensuring the correct operation of the machine.

## Optional Parametrization Options

The optional parametrization options for the `DM542_XXYZ_mill` configuration are as follows:

* The `postgui_call_list.hal` settings affect the machine by configuring various parameters and enabling specific functionalities for the PoKeys device and its interaction with the machine. Here are the key effects:
  * The file sources `custom_postgui.hal` to include any custom post-GUI HAL commands.
  * It sets the device ID for the PoKeys device using the `DEVICE_ID` parameter from the INI file.
  * It configures the probe input pin and its polarity using the `PEv2_ProbeInput` and `PEv2_ProbeInputPolarity` parameters.
  * It sets the emergency input and output pins using the `PEv2_EmergencyInputPin` and `PEv2_EmergencyOutputPin` parameters.
  * It configures the pulse generator type using the `PEv2_PulseGeneratorType` parameter.
  * It sets the axis switch configuration for each axis using the `PEv2_AxesSwitchConfig_*` parameters.
  * It enables and configures the limit and home switches for each axis, including setting the invert and filter options.
  * It sets the home sequence for each axis using the `home-sequence` parameter.
  * It links the homing signals from the `pokeys_homecomp.comp` component to the PoKeys components.
  * It applies the INI settings to the PoKeys device using the `ApplyIniSettings` parameter.
  * It sets up the real-time clock (RTC) synchronization by linking the `pokeys.rtc.sec` signal.

These settings collectively ensure proper communication and control of the machine's components, including the limit switches, homing configuration, and emergency handling. The `postgui_call_list.hal` file plays a crucial role in finalizing the machine's configuration after the GUI has been loaded.

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
