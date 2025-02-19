# DM542_XXYZ_mill/pokeys_homing.md

## Description of `DM542_XXYZ_mill/pokeys_homing.hal`

The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components. It includes the following key sections:

- **PulseEngineState**: Connects the state of the pulse engine.
- **AxesState**: Connects the state of the axes.
- **AxesCommand**: Connects the command signals for the axes.
- **AxesConfig**: Connects the configuration signals for the axes.
- **HomeSequence**: Sets the home sequence for each axis.
- **Homing Signals**: Links the homing signals from `pokeys_homecomp.comp` to PoKeys components.
- **Homed Signals**: Links the homed signals from `pokeys_homecomp.comp` to PoKeys components.
- **Home State**: Links the home state signals from `pokeys_homecomp.comp` to PoKeys components.
- **Index Enable**: Links the index enable signals from `pokeys_homecomp.comp` to PoKeys components.
- **PEv2 Signals**: Links various PEv2 signals from `pokeys_homecomp.comp` to PoKeys components.

## Relation to `pokeys_homecomp.comp`

The `pokeys_homecomp.comp` component provides pin outs and signal linking for the PoKeys homing configuration. It includes the following key sections:

- **Pin Outs**: Defines the pin outs for the homing module, including `devSerial`, `PulseEngineState`, `homing`, `homed`, `home_state`, `index_enable`, `PEv2_AxesState`, `PEv2_AxesCommand`, `PEv2_AxesConfig`, `PEv2_AxesSwitchConfig`, `PEv2_HomingSpeed`, `PEv2_HomingReturnSpeed`, `PEv2_HomingAlgorithm`, `PEv2_HomeOffsets`, and `PEv2_HomeBackOffDistance`.
- **Signal Linking**: Links the signals from the homing module to the PoKeys components.

## Relation to `pokeys.comp`

The `pokeys.comp` component provides real-time control and monitoring capabilities within LinuxCNC. It supports various types of I/O, including digital inputs, digital outputs, analog inputs, analog outputs, and encoders. The `pokeys_homecomp.comp` component interacts with `pokeys.comp` by linking the homing signals and pins to the PoKeys components.

## Signals and Pins Sent Between `pokeys_homecomp.comp` and `pokeys.comp`

The following signals and pins are sent between `pokeys_homecomp.comp` and `pokeys.comp`:

- **Homing Signals**: The homing signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
- **Homed Signals**: The homed signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
- **Home State**: The home state signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
- **Index Enable**: The index enable signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
- **PEv2 Signals**: Various PEv2 signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.

## Homing Configuration and Linking Pins

The homing configuration and linking pins are defined in the `DM542_XXYZ_mill/pokeys_homing.hal` file. The key sections include:

- **PulseEngineState**: Connects the state of the pulse engine.
- **AxesState**: Connects the state of the axes.
- **AxesCommand**: Connects the command signals for the axes.
- **AxesConfig**: Connects the configuration signals for the axes.
- **HomeSequence**: Sets the home sequence for each axis.
- **Homing Signals**: Links the homing signals from `pokeys_homecomp.comp` to PoKeys components.
- **Homed Signals**: Links the homed signals from `pokeys_homecomp.comp` to PoKeys components.
- **Home State**: Links the home state signals from `pokeys_homecomp.comp` to PoKeys components.
- **Index Enable**: Links the index enable signals from `pokeys_homecomp.comp` to PoKeys components.
- **PEv2 Signals**: Links various PEv2 signals from `pokeys_homecomp.comp` to PoKeys components.

## Use Case of Homing for a Single Axis/Joint

When homing is triggered in the PoKeys setup, the `pokeys_homecomp.comp` component sets the `joint.#.PEv2.AxesCommand` pin, which is then received by the `pokeys.comp` component at `pokeys.0.PEv2.#.AxesCommand`. The `pokeys.comp` component subsequently triggers the homing process at the PoKeys library (`pokeyslib`).

- The `pokeys_homecomp.comp` component defines various pins and signals related to the homing process, including `PEv2_AxesCommand` which is used to send commands to the pulse engine.
- When homing is initiated, `pokeys_homecomp.comp` sets the `joint.#.PEv2.AxesCommand` pin to the appropriate value to start the homing procedure.
- The `pokeys_homing.hal` file links the `joint.#.PEv2.AxesCommand` pin from `pokeys_homecomp.comp` to the corresponding pin in `pokeys.comp` (`pokeys.0.PEv2.#.AxesCommand`).
- The `pokeys.comp` component receives the command on `pokeys.0.PEv2.#.AxesCommand` and processes it to trigger the homing routine at the PoKeys library (`pokeyslib`).

This setup ensures that the homing command is correctly propagated from the `pokeys_homecomp.comp` component to the `pokeys.comp` component, and finally to the PoKeys library to execute the homing procedure.

## Propagation of Homing Commands and Status Updates

When the status of PEv2 in `pokeyslib` is set on `pokeys.0.PEv2.#.AxesState` by `pokeys.comp`, it is received on `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`. `pokeys_homecomp.comp` then checks the status and represents them at `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state`.

- The `pokeys_homecomp.comp` component defines various pins and signals related to the homing process, including `PEv2_AxesState`, which is used to receive the status of the pulse engine axes.
- The `pokeys_homing.hal` file links the `joint.#.PEv2.AxesState` pin from `pokeys_homecomp.comp` to the corresponding pin in `pokeys.comp` (`pokeys.0.PEv2.#.AxesState`).
- The `pokeys.comp` component sets the status of PEv2 on `pokeys.0.PEv2.#.AxesState`, which is then received by `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`.
- `pokeys_homecomp.comp` checks the status of the pulse engine axes and updates the corresponding pins `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state` to represent the current state of the homing process.
- This setup ensures that the status of the pulse engine axes is correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

## Possible Values for `joint.#.PEv2.AxesCommand` During Homing

The possible values for `joint.#.PEv2.AxesCommand` are defined in the `pokeys_homecomp.comp` file. These values are used to control the state of the pulse engine axes during the homing process. The possible values are:

- `PK_PEAxisCommand_axIDLE`: Axis in IDLE state
- `PK_PEAxisCommand_axHOMINGSTART`: Start Homing procedure
- `PK_PEAxisCommand_axHOMINGCANCEL`: Cancel Homing procedure

These values are used to manage the homing process and ensure that the pulse engine axes are correctly controlled during the homing routine. The `pokeys_homecomp.comp` file defines these values and uses them to set the `joint.#.PEv2.AxesCommand` pin accordingly.

## Possible Values for `joint.#.PEv2.AxesState` During Homing

The specific values set for `joint.#.PEv2.AxesState` during homing are defined in the `pokeys_homecomp.comp` file. These values represent the state of the pulse engine axis during the homing process. The possible values are:

- `PK_PEAxisState_axHOMING_RESETTING`: Stopping the axis to reset the position counters
- `PK_PEAxisState_axHOMING_BACKING_OFF`: Backing off switch
- `PK_PEAxisState_axHOMINGSTART`: Homing procedure is starting on axis
- `PK_PEAxisState_axHOMINGSEARCH`: Homing procedure first step - going to home
- `PK_PEAxisState_axHOMINGBACK`: Homing procedure second step - slow homing
- `PK_PEAxisState_axHOME`: Axis is homed

These values are used to manage the homing process and ensure that the pulse engine axes are correctly controlled during the homing routine. The `pokeys_homecomp.comp` file defines these values and uses them to set the `joint.#.PEv2.AxesState` pin accordingly.

## Detailed Interaction Between `pokeys_homecomp.comp` and `pokeys.comp`

`pokeys_homecomp.comp` interacts with `pokeys.comp` by linking homing signals and pins to the PoKeys components. Here are the key points of interaction:

* `pokeys_homecomp.comp` defines various pins and signals related to the homing process, including `PEv2_AxesState` and `PEv2_AxesCommand`.
* The homing signals from `pokeys_homecomp.comp` are linked to the PoKeys components in `pokeys.comp`.
* The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components.
* The `DM542_XXYZ_mill/pokeys_homing.md` file provides a detailed description of the homing configuration and linking pins.
* When homing is triggered, `pokeys_homecomp.comp` sets the `joint.#.PEv2.AxesCommand` pin, which is then received by `pokeys.comp` at `pokeys.0.PEv2.#.AxesCommand`.
* The status of PEv2 in `pokeyslib` is set on `pokeys.0.PEv2.#.AxesState` by `pokeys.comp`, and it is received on `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`.
* `pokeys_homecomp.comp` checks the status and represents them at `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state`.

This setup ensures that the homing command is correctly propagated from `pokeys_homecomp.comp` to `pokeys.comp`, and finally to the PoKeys library to execute the homing procedure. The status of the pulse engine axes is also correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

## Roles of `PEv2_AxesState` and `PEv2_AxesCommand`

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

## Propagation of Homing Signals

The homing signals in the system are propagated through a series of connections and interactions between various components. Here is an overview of how the homing signals are propagated:

* The `pokeys_homecomp.comp` component defines various pins and signals related to the homing process, including `PEv2_AxesState` and `PEv2_AxesCommand`.
* The `DM542_XXYZ_mill/pokeys_homing.hal` file defines the connections for homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components.
* When homing is triggered, `pokeys_homecomp.comp` sets the `joint.#.PEv2.AxesCommand` pin, which is then received by `pokeys.comp` at `pokeys.0.PEv2.#.AxesCommand`.
* The status of PEv2 in `pokeyslib` is set on `pokeys.0.PEv2.#.AxesState` by `pokeys.comp`, and it is received on `pokeys_homecomp.comp` at `joint.#.PEv2.AxesState`.
* `pokeys_homecomp.comp` checks the status and represents them at `joint.#.homing`, `joint.#.homed`, and `joint.#.home-state`.
* The `DM542_XXYZ_mill/pokeys_homing.md` file provides a detailed description of the homing configuration and linking pins.

This setup ensures that the homing command is correctly propagated from `pokeys_homecomp.comp` to `pokeys.comp`, and finally to the PoKeys library to execute the homing procedure. The status of the pulse engine axes is also correctly propagated from `pokeys.comp` to `pokeys_homecomp.comp`, allowing `pokeys_homecomp.comp` to manage the homing process and update the relevant status pins accordingly.

## Configuration Steps for Homing Functionality

To configure the homing functionality in the `LinuxCnc_PokeysLibComp` project, follow these steps:

1. **Update the `DM542_XXYZ_mill/pokeys_homing.hal` file**: Define the connections for homing signals and link pins from `pokeys_homecomp.comp` to PoKeys components. Ensure that the homing signals, homed signals, home state signals, and index enable signals are correctly linked.

2. **Set the home sequence for each axis**: Use the `setp` command in the `DM542_XXYZ_mill/pokeys_homing.hal` file to set the home sequence for each axis. For example:
   ```plaintext
   setp pokeys.0.PEv2.0.home-sequence [JOINT_0]HOME_SEQUENCE
   setp pokeys.0.PEv2.6.home-sequence [JOINT_1]HOME_SEQUENCE
   setp pokeys.0.PEv2.1.home-sequence [JOINT_2]HOME_SEQUENCE
   setp pokeys.0.PEv2.2.home-sequence [JOINT_3]HOME_SEQUENCE
   ```

3. **Update the `DM542_XXYZ_mill/pokeys_homing.md` file**: Provide a detailed description of the homing configuration and linking pins. Ensure that the documentation accurately describes the homing configuration and the interaction between `pokeys_homecomp.comp` and `pokeys.comp`.

4. **Verify and document the interaction between `pokeys_homecomp.comp` and `pokeys.comp`**: Ensure that the homing signals and pins are correctly linked to the PoKeys components. Confirm that the `pokeys_homing.hal` file correctly connects the homing signals and links pins from `pokeys_homecomp.comp` to PoKeys components.

5. **Test the homing functionality**: Test the homing functionality with the supported PoKeys devices and document any limitations or issues. Update the `docs/wiki/Compatibility.md` file to include information about the compatibility of the homing functionality with different PoKeys devices and LinuxCNC versions.

6. **Ensure that the `pokeys_homecomp.comp` file defines the necessary pins and signals**: Include `PEv2_AxesState`, `PEv2_AxesCommand`, `PEv2_AxesConfig`, `PEv2_AxesSwitchConfig`, `PEv2_HomingSpeed`, `PEv2_HomingReturnSpeed`, `PEv2_HomingAlgorithm`, `PEv2_HomeOffsets`, and `PEv2_HomeBackOffDistance`.

7. **Update the `README.md` file**: Include information about the homing functionality and its configuration steps. Provide clear instructions on how to use the homing functionality, including any necessary configuration steps and examples.
