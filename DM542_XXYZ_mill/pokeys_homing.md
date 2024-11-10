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
