# Developer Documentation

Welcome to the Developer Documentation for the **LinuxCnc_PokeysLibComp** project. This section provides information for contributors, including coding standards, architecture details, API documentation, and contribution guidelines.

## Coding Standards

To maintain consistency and readability in the codebase, please follow these coding standards:

### Python

- Follow the PEP 8 style guide for Python code.
- Use meaningful variable and function names.
- Write docstrings for all functions and classes.
- Use type hints where applicable.

### C/C++

- Follow the project's existing code style for C/C++ code.
- Use meaningful variable and function names.
- Write comments to explain complex logic.
- Use consistent indentation and formatting.

## Architecture Details

The **LinuxCnc_PokeysLibComp** project is organized into several components, each responsible for different aspects of the integration between PoKeys devices and LinuxCNC.

### Components

- **pokeys_py**: Contains Python scripts for interfacing with PoKeys devices.
- **pokeys_rt**: Contains real-time components for PoKeys devices.
- **tests**: Contains unit tests, integration tests, and other testing-related files.
- **docs**: Contains project-related documentation.
- **scripts**: Contains scripts for setup, configuration, and auxiliary tasks.

### Diagrams

#### System Architecture

![System Architecture](../diagrams/system_architecture.png)

#### Data Flow

![Data Flow](../diagrams/data_flow.png)

## API Documentation

The following sections provide detailed documentation for the APIs used in the **LinuxCnc_PokeysLibComp** project.

### Digital I/O

#### Functions

- `PK_DigitalIOGet(sPoKeysDevice* device, uint8_t pinID, uint8_t* pinValue)`: Get the value of a digital input pin.
- `PK_DigitalIOSet(sPoKeysDevice* device, uint8_t pinID, uint8_t pinValue)`: Set the value of a digital output pin.

### Analog I/O

#### Functions

- `PK_AnalogIOGet(sPoKeysDevice* device, uint8_t channelID, float* value)`: Get the value of an analog input channel.
- `PK_AnalogIOSet(sPoKeysDevice* device, uint8_t channelID, float value)`: Set the value of an analog output channel.

### Pulse Engine

#### Functions

- `PK_PEv2_PulseEngineMove(sPoKeysDevice* device, uint8_t axisID, int32_t position)`: Move the specified axis to the given position.
- `PK_PEv2_PulseEngineStop(sPoKeysDevice* device, uint8_t axisID)`: Stop the specified axis.

## Contribution Guidelines

We welcome contributions from the community to help improve the project. Please follow these guidelines when contributing:

1. **Fork the Repository**: Start by forking the repository to your GitHub account.
2. **Clone the Repository**: Clone the forked repository to your local machine.
   ```bash
   git clone https://github.com/your-username/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```
3. **Create a Branch**: Create a new branch for your contribution.
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. **Make Changes**: Make your changes to the codebase.
5. **Commit Changes**: Commit your changes with a descriptive commit message.
   ```bash
   git add .
   git commit -m "Add feature: your feature description"
   ```
6. **Push Changes**: Push your changes to your forked repository.
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Create a Pull Request**: Open a pull request from your forked repository to the main repository.

## Design Decisions

### Real-Time Components

The real-time components in the **LinuxCnc_PokeysLibComp** project are designed to ensure low-latency and deterministic behavior. The following design decisions were made to achieve this:

- **Use of Real-Time Threads**: Real-time threads are used to handle time-critical tasks, ensuring that they are executed with minimal jitter.
- **Avoidance of Dynamic Memory Allocation**: Dynamic memory allocation is avoided in real-time components to prevent unpredictable delays.
- **Priority Inversion Handling**: Priority inversion is handled using priority inheritance mechanisms to ensure that high-priority tasks are not blocked by lower-priority tasks.

### Modular Design

The project follows a modular design approach, allowing for easy extension and maintenance. Each component is designed to be self-contained and interacts with other components through well-defined interfaces.

## Keeping the Documentation Up-to-date

To ensure the documentation remains accurate and up-to-date, we recommend scheduling periodic reviews. Contributors are encouraged to update and edit the documentation to reflect the latest changes to the repository.

## CI/CD Pipelines

The **LinuxCnc_PokeysLibComp** project uses continuous integration and continuous deployment (CI/CD) pipelines to automate the build, test, and deployment processes. The following sections provide details on the CI/CD pipelines used in the project.

### Build Pipeline

The build pipeline is responsible for compiling the code and generating the necessary artifacts. It includes the following steps:

1. **Checkout Code**: Clone the repository and checkout the latest code.
2. **Install Dependencies**: Install the required dependencies for building the project.
3. **Compile Code**: Compile the code and generate the necessary artifacts.
4. **Run Unit Tests**: Execute the unit tests to ensure the code is functioning correctly.
5. **Generate Artifacts**: Generate the build artifacts, such as binaries and libraries.

### Test Pipeline

The test pipeline is responsible for running the tests and verifying the functionality of the code. It includes the following steps:

1. **Checkout Code**: Clone the repository and checkout the latest code.
2. **Install Dependencies**: Install the required dependencies for running the tests.
3. **Run Unit Tests**: Execute the unit tests to ensure the code is functioning correctly.
4. **Run Integration Tests**: Execute the integration tests to verify the interaction between different components.
5. **Run Functional Tests**: Execute the functional tests to simulate real-world usage scenarios.
6. **Generate Test Reports**: Generate the test reports and publish them for review.

### Deployment Pipeline

The deployment pipeline is responsible for deploying the code to the target environment. It includes the following steps:

1. **Checkout Code**: Clone the repository and checkout the latest code.
2. **Install Dependencies**: Install the required dependencies for deploying the project.
3. **Deploy Code**: Deploy the code to the target environment.
4. **Run Smoke Tests**: Execute the smoke tests to verify the basic functionality of the deployed code.
5. **Generate Deployment Reports**: Generate the deployment reports and publish them for review.

## Testing Strategies

The **LinuxCnc_PokeysLibComp** project follows a comprehensive testing strategy to ensure the reliability and correctness of the code. The following sections provide details on the different types of tests used in the project.

### Unit Tests

Unit tests are designed to test individual functions and methods within the codebase. They are executed in isolation and do not depend on external components. Unit tests are typically written using a testing framework, such as `pytest` for Python or `Google Test` for C++.

### Integration Tests

Integration tests verify the interaction between different components of the project. They ensure that the components work together as expected and that data flows correctly between them. Integration tests are typically written using a testing framework, such as `pytest` for Python or `Google Test` for C++.

### Functional Tests

Functional tests simulate real-world usage scenarios and verify the functionality of the project from an end-user perspective. They ensure that the project meets the specified requirements and behaves as expected. Functional tests are typically written using a testing framework, such as `pytest` for Python or `Google Test` for C++.

### Performance Tests

Performance tests measure the performance of the project under different conditions. They ensure that the project meets the required performance criteria and does not introduce any performance bottlenecks. Performance tests are typically written using a testing framework, such as `pytest` for Python or `Google Test` for C++.

## Contribution Guidelines

We welcome contributions from the community to help improve the project. Please follow these guidelines when contributing:

1. **Fork the Repository**: Start by forking the repository to your GitHub account.
2. **Clone the Repository**: Clone the forked repository to your local machine.
   ```bash
   git clone https://github.com/your-username/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```
3. **Create a Branch**: Create a new branch for your contribution.
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. **Make Changes**: Make your changes to the codebase.
5. **Commit Changes**: Commit your changes with a descriptive commit message.
   ```bash
   git add .
   git commit -m "Add feature: your feature description"
   ```
6. **Push Changes**: Push your changes to your forked repository.
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Create a Pull Request**: Open a pull request from your forked repository to the main repository.

## Design Decisions

### Real-Time Components

The real-time components in the **LinuxCnc_PokeysLibComp** project are designed to ensure low-latency and deterministic behavior. The following design decisions were made to achieve this:

- **Use of Real-Time Threads**: Real-time threads are used to handle time-critical tasks, ensuring that they are executed with minimal jitter.
- **Avoidance of Dynamic Memory Allocation**: Dynamic memory allocation is avoided in real-time components to prevent unpredictable delays.
- **Priority Inversion Handling**: Priority inversion is handled using priority inheritance mechanisms to ensure that high-priority tasks are not blocked by lower-priority tasks.

### Modular Design

The project follows a modular design approach, allowing for easy extension and maintenance. Each component is designed to be self-contained and interacts with other components through well-defined interfaces.

## Keeping the Documentation Up-to-date

To ensure the documentation remains accurate and up-to-date, we recommend scheduling periodic reviews. Contributors are encouraged to update and edit the documentation to reflect the latest changes to the repository.

## PEv2 Developer Description

### PEv2 Information

#### Pins

- `pokeys.[DevID].PEv2.nrOfAxes`
- `pokeys.[DevID].PEv2.maxPulseFrequency`
- `pokeys.[DevID].PEv2.bufferDepth`
- `pokeys.[DevID].PEv2.slotTiming`
- `pokeys.[DevID].PEv2.digin.Emergency.in`
- `pokeys.[DevID].PEv2.digin.Emergency.in-not`
- `pokeys.[DevID].PEv2.PulseEngineEnabled`
- `pokeys.[DevID].PEv2.PulseGeneratorType`
- `pokeys.[DevID].PEv2.PG_swap_stepdir`
- `pokeys.[DevID].PEv2.PG_extended_io`
- `pokeys.[DevID].PEv2.ChargePumpEnabled`
- `pokeys.[DevID].PEv2.PulseEngineActivated`
- `pokeys.[DevID].PEv2.PulseEngineState`
- `pokeys.[DevID].PEv2.MiscInputStatus`
- `pokeys.[DevID].PEv2.digin.Misc-#.in[8]`
- `pokeys.[DevID].PEv2.digin.Misc-#.in-not[8]`
- `pokeys.[DevID].PEv2.LimitOverride`
- `pokeys.[DevID].PEv2.LimitOverrideSetup`
- `pokeys.[DevID].PEv2.digin.Probed.in`
- `pokeys.[DevID].PEv2.digout.Emergency.out`
- `pokeys.[DevID].PEv2.AxisEnabledMask`
- `pokeys.[DevID].PEv2.AxisEnabledStatesMask`
- `pokeys.[DevID].PEv2.ExternalRelayOutputs`
- `pokeys.[DevID].PEv2.ExternalOCOutputs`
- `pokeys.[DevID].PEv2.digout.ExternalRelay-#.out[4]`
- `pokeys.[DevID].PEv2.digout.ExternalOC-#.out[4]`
- `pokeys.[DevID].PEv2.HomingStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStartMaskSetup`
- `pokeys.[DevID].PEv2.ProbeStatus`
- `pokeys.[DevID].PEv2.ProbeSpeed`
- `pokeys.[DevID].PEv2.BacklashCompensationEnabled`

#### Parameters

- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digout.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### PEv2 Information - For Each Axis

#### Pins

- `pokeys.[DevID].PEv2.[PEv2ID].AxesState[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesConfig[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].AxesSwitchConfig[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMaximum[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].SoftLimitMinimum[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingSpeed[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingReturnSpeed[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomingAlgorithm[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeOffsets[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbePosition[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ProbeMaxPosition[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].CurrentPosition[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].PositionSetup[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].ReferencePositionSpeed[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxSpeed[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MaxAcceleration[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogMultiplier[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogEncoder[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].MPGjogDivider[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.DedicatedInput[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].HomeBackOffDistance[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Error.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Probe.in-not[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnabled.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.AxisEnabled.in[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.LimitOverride.out[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashWidth[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashRegister[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].BacklashAcceleration[8]`

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.SoftLimit.PosMax[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Offset[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitN.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.LimitP.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digout.AxisEnable.invert[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Pin[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.Filter[8]`
- `pokeys.[DevID].PEv2.[PEv2ID].digin.Home.invert[8]`

## Detailed List of Bitmapped Fields in PEv2

### 1. `HomingStartMaskSetup`

- **Type**: `uint32_t`
- **Enumeration**: **None** (Direct Bitmapping)
- **Bit Values**:
  - **Bit 0**: Axis 1
  - **Bit 1**: Axis 2
  - ...
  - **Bit 7**: Axis 8

**Description**: Controls which axes are starting the homing procedure. Each bit represents an axis (1 = homing started, 0 = not started).

### 2. `BacklashCompensationEnabled`

- **Type**: `uint8_t`
- **Enumeration**: **None** (Direct Bitmapping)
- **Bit Values**:
  - **Bit 0**: Axis 1
  - **Bit 1**: Axis 2
  - ...
  - **Bit 7**: Axis 8

**Description**: Indicates whether backlash compensation is enabled for each axis.

### 3. `TriggerIngnoredAxisMask`

- **Type**: `uint8_t`
- **Enumeration**: **None** (Direct Bitmapping)
- **Bit Values**:
  - **Bit 0**: Axis 1
  - **Bit 1**: Axis 2
  - ...
  - **Bit 7**: Axis 8

**Description**: Defines whether the trigger is ignored for each axis (1 = trigger ignored, 0 = trigger active).

### 4. `DedicatedLimitNInputs` and `DedicatedLimitPInputs`

- **Type**: `uint8_t`
- **Enumeration**: **None** (Direct Bitmapping)
- **Bit Values**:
  - **Bit 0**: Axis 1
  - **Bit 1**: Axis 2
  - ...
  - **Bit 7**: Axis 8

**Description**: Bitmapped fields for the limit switch inputs (negative limit for `DedicatedLimitNInputs` and positive limit for `DedicatedLimitPInputs`).

### 5. `SignalConfig[]` (Signal inversion flags)

- **Type**: **Array of Bitmapped Fields**
- **Enumeration**: **`ePK_PEv2_AxisSignalOptions`**
- **Values**:
  - **`PK_ASO_INVERT_STEP`** = `(1 << 0)` – Invert step signal (active-low)
  - **`PK_ASO_INVERT_DIRECTION`** = `(1 << 1)` – Invert direction signal

**Description**: This field stores configuration flags that control signal inversion for each axis (step and direction signals).

### 6. `AxesSwitchConfig[]` (Switch configuration for each axis)

- **Type**: **Array of Bitmapped Fields**
- **Enumeration**: **`ePK_PEv2_AxisSwitchOptions`**
- **Values**:
  - **`PK_ASO_SWITCH_LIMIT_N`** = `(1 << 0)` – Limit- switch
  - **`PK_ASO_SWITCH_LIMIT_P`** = `(1 << 1)` – Limit+ switch
  - **`PK_ASO_SWITCH_HOME`** = `(1 << 2)` – Home switch
  - **`PK_ASO_SWITCH_COMBINED_LN_H`** = `(1 << 3)` – Home switch is shared with Limit- switch
  - **`PK_ASO_SWITCH_COMBINED_LP_H`** = `(1 << 4)` – Home switch is shared with Limit+ switch
  - **`PK_ASO_SWITCH_INVERT_LIMIT_N`** = `(1 << 5)` – Invert limit- switch polarity
  - **`PK_ASO_SWITCH_INVERT_LIMIT_P`** = `(1 << 6)` – Invert limit+ switch polarity
  - **`PK_ASO_SWITCH_INVERT_HOME`** = `(1 << 7)` – Invert home switch polarity

**Description**: This field defines the switch configuration for each axis, with individual bits representing limit and home switches and their polarities.

### 7. `MotionBuffer[]`

- **Type**: **Bitmapped Field**
- **Enumeration**: **None** (Direct Buffering)
- **Bit Values**: Axis-specific motion data is bit-mapped into this buffer.

**Description**: This buffer stores movement data for each axis. It needs to be split based on axis-specific motion commands and feedback.

### 8. `PWMSetup[]` (PWM output configuration)

- **Type**: **Array of PWM Channels**
- **Enumeration**: **None**
- **Values**:
  - Each PWM channel corresponds to an axis and stores its respective configuration.

**Description**: Stores PWM output configurations, which can be synchronized with axes. Each channel is treated separately but might need to be mapped to individual axis parameters.

### 9. `ePK_PEv2_AxisConfig` (Axis configuration flags)

- **Enumeration**: **`ePK_PEv2_AxisConfig`**
- **Values**:
  - **`PK_AC_ENABLED`** = `(1 << 0)` – Axis enabled
  - **`PK_AC_INVERTED`** = `(1 << 1)` – Axis direction inverted
  - **`PK_AC_INTERNAL_PLANNER`** = `(1 << 2)` – Internal motion planner enabled
  - **`PK_AC_POSITION_MODE`** = `(1 << 3)` – Internal motion planner is in position mode
  - **`PK_AC_INVERTED_HOME`** = `(1 << 4)` – Homing direction inverted
  - **`PK_AC_SOFT_LIMIT_ENABLED`** = `(1 << 5)` – Soft limits enabled
  - **`PK_AC_FAST_OUTPUT`** = `(1 << 6)` – Fast output mode
  - **`PK_AC_ENABLED_MASKED`** = `(1 << 7)` – Output enable pin masking enabled

**Description**: These are flags that control axis configuration (enable/disable, inversion, motion planning mode, homing direction, etc.) for each axis.

## Full List of Enumerations in PEv2 that Affect Axis Behavior

1. **`ePK_PEv2_AxisConfig`**: Configuration flags for each axis (enable, inversion, position mode, etc.)
2. **`ePK_PEv2_AxisSwitchOptions`**: Switch options for each axis (limit and home switch configuration, inversion)
3. **`ePK_PEv2_AxisSignalOptions`**: Signal inversion options for each axis (step and direction inversion)

## Conclusion

Many of these bitmapped fields need to be split into individual HAL pins or parameters to map each axis’s status, configuration, and behavior in LinuxCNC. This ensures that each axis operates independently, and its signals are correctly interpreted by the system. The enumeration-based fields provide a clear method for interpreting and configuring these bitmapped values.

## Public Interface of PEv2 in PoKeysLib

The public interface of PEv2 in PoKeysLib is as follows:

### Pulse Engine State

```c
enum ePK_PEState
{
    PK_PEState_peSTOPPED        = 0,           // Pulse engine is stopped
    PK_PEState_peINTERNAL       = 1,           // PEv1: Internal motion controller is in use, PEv2: not used
    PK_PEState_peBUFFER         = 2,           // PEv1: Buffered operation mode is in use, PEv2: not used
    PK_PEState_peRUNNING        = 3,           // Pulse engine is activated

    PK_PEState_peJOGGING        = 10,          // Jogging mode enabled
    PK_PEState_peSTOPPING       = 11,          // Pulse engine is stopping

    PK_PEState_peHOME           = 20,          // All axes are homed
    PK_PEState_peHOMING         = 21,          // Axes homing is in progress

    PK_PEState_pePROBECOMPLETE  = 30,          // All axes are homed
    PK_PEState_pePROBE          = 31,          // Axes probing is in progress
    PK_PEState_pePROBEERROR     = 32,          // Error occurred during probing

    PK_PEState_peHYBRIDPROBE_STOPPING = 40,
    PK_PEState_peHYBRIDPROBE_COMPLETE = 41,

    PK_PEState_peSTOP_LIMIT     = 100,         // Pulse engine stopped due to limit reached
    PK_PEState_peSTOP_EMERGENCY = 101          // Pulse engine stopped due to emergency switch
};
```

### Pulse Engine Axis State

```c
enum ePK_PEAxisState
{
    PK_PEAxisState_axSTOPPED      =  0,        // Axis is stopped
    PK_PEAxisState_axREADY        =  1,        // Axis ready
    PK_PEAxisState_axRUNNING      =  2,        // Axis is running

    PK_PEAxisState_axHOMING_RESETTING = 8,     // Stopping the axis to reset the position counters
    PK_PEAxisState_axHOMING_BACKING_OFF = 9,   // Backing off switch
    PK_PEAxisState_axHOME         =  10,       // Axis is homed
    PK_PEAxisState_axHOMINGSTART  =  11,       // Homing procedure is starting on axis
    PK_PEAxisState_axHOMINGSEARCH =  12,       // Homing procedure first step - going to home
    PK_PEAxisState_axHOMINGBACK   =  13,       // Homing procedure second step - slow homing

    PK_PEAxisState_axPROBED       =  14,       // Probing completed for this axis
    PK_PEAxisState_axPROBESTART   =  15,       // Probing procedure is starting on axis
    PK_PEAxisState_axPROBESEARCH  =  16,       // Probing procedure - probing

    PK_PEAxisState_axERROR        =  20,       // Axis error
    PK_PEAxisState_axLIMIT        =  30        // Axis limit tripped
};
```

### Pulse Engine Axis Configuration

```c
enum ePK_PEv2_AxisConfig
{
    PK_AC_ENABLED            = (1 << 0),       // Axis enabled
    PK_AC_INVERTED           = (1 << 1),       // Axis inverted
    PK_AC_INTERNAL_PLANNER   = (1 << 2),       // Axis uses internal motion planner
    PK_AC_POSITION_MODE      = (1 << 3),       // Internal motion planner for this axis is in position mode
    PK_AC_INVERTED_HOME      = (1 << 4),       // Axis homing direction is inverted
    PK_AC_SOFT_LIMIT_ENABLED = (1 << 5),       // Use soft-limits for this axis
    PK_AC_FAST_OUTPUT        = (1 << 6),       // Fast output mode
    PK_AC_ENABLED_MASKED     = (1 << 7)        // Use output enable pin masking
};
```

### Pulse Engine Axis Switch Options

```c
enum ePK_PEv2_AxisSwitchOptions
{
    PK_ASO_SWITCH_LIMIT_N        = (1 << 0),   // Limit- switch
    PK_ASO_SWITCH_LIMIT_P        = (1 << 1),   // Limit+ switch
    PK_ASO_SWITCH_HOME           = (1 << 2),   // Home switch
    PK_ASO_SWITCH_COMBINED_LN_H  = (1 << 3),   // Home switch is shared with Limit- switch
    PK_ASO_SWITCH_COMBINED_LP_H  = (1 << 4),   // Home switch is shared with Limit+ switch
    PK_ASO_SWITCH_INVERT_LIMIT_N = (1 << 5),   // Invert limit- switch polarity
    PK_ASO_SWITCH_INVERT_LIMIT_P = (1 << 6),   // Invert limit+ switch polarity
    PK_ASO_SWITCH_INVERT_HOME    = (1 << 7)    // Invert home switch polarity
};
```

### Pulse Engine Axis Signal Options

```c
enum ePK_PEv2_AxisSignalOptions
{
    PK_ASO_INVERT_STEP = (1 << 0),             // Invert step signal (active-low)
    PK_ASO_INVERT_DIRECTION = (1 << 1)         // Invert direction signal
};
```

### Pulse Engine v2 Information

```c
typedef struct
{
    uint8_t nrOfAxes;
    uint8_t maxPulseFrequency;
    uint8_t bufferDepth;
    uint8_t slotTiming;

    uint8_t reserved[4];
} sPoKeysPEv2info;
```

### Pulse Engine v2 Structure

```c
typedef struct
{
    sPoKeysPEv2info info;                      // Pulse engine info

    uint8_t         AxesState[8];              // Axis states (bit-mapped) - see ePK_PEAxisState
    uint8_t         AxesConfig[8];             // Axis configuration - see ePK_PEv2_AxisConfig
    uint8_t         AxesSwitchConfig[8];       // Axis switch configuration - see ePK_PEv2_AxisSwitchOptions
    int32_t         CurrentPosition[8];        // Current position
    int32_t         PositionSetup[8];          // Position to be set as current position
    int32_t         ReferencePositionSpeed[8]; // Reference position or speed (position or pulses/s)
    int8_t          InvertAxisEnable[8];       // Invert axis enable signal

    int32_t         SoftLimitMaximum[8];       // Soft limit maximum position
    int32_t         SoftLimitMinimum[8];       // Soft limit minimum position

    uint8_t         HomingSpeed[8];            // Homing speed per axis (in %)
    uint8_t         HomingReturnSpeed[8];      // Homing return speed per axis (in % of the homing speed)

    int32_t         HomeOffsets[8];            // Home position offset
    uint8_t         HomingAlgorithm[8];        // Homing algorithm configuration

    uint8_t         FilterLimitMSwitch[8];     // Digital filter for limit- switch
    uint8_t         FilterLimitPSwitch[8];     // Digital filter for limit+ switch
    uint8_t         FilterHomeSwitch[8];       // Digital filter for home switch

    int32_t         ProbePosition[8];          // Position where probe detected change
    int32_t         ProbeMaxPosition[8];       // Maximum position to travel to until stopping and returning error
    float           MaxSpeed[8];               // Maximum axis speed (in pulses per ms)
    float           MaxAcceleration[8];        // Maximum axis acceleration (in pulses/ms/ms)
    float           MaxDecceleration[8];       // Maximum axis deceleration (in pulses/ms/ms)

    int32_t         MPGjogMultiplier[8];       // MPG jog multiplier value
    uint8_t         MPGjogEncoder[8];          // MPG jog encoder ID
    uint8_t         PinHomeSwitch[8];          // Home switch pin (0 for external dedicated input)
    uint8_t         PinLimitMSwitch[8];        // Limit- switch pin (0 for external dedicated input)
    uint8_t         PinLimitPSwitch[8];        // Limit+ switch pin (0 for external dedicated input)
    uint8_t         AxisEnableOutputPins[8];   // Axis enabled output pin (0 for external dedicated output)
    uint32_t        HomeBackOffDistance[8];    // Back-off distance after homing
    uint16_t        MPGjogDivider[8];          // Divider for the MPG jogging (enhanced encoder resolution)
    uint8_t         FilterProbeInput;          // Filter for the probe input
    uint8_t         reserved[7];               // Motion buffer entries - moved further down...
    uint8_t         AxisSignalOptions[8];      // Axis signal options (invert step or direction)

    float           ReferenceVelocityPV[8];    // Reference velocity in PV mode (between 0.0 and 1.0) as ratio of max. speed
    uint8_t         ReservedSafety[8];         // 8-bytes reserved

    // ------ 64-bit region boundary ------
    uint8_t         PulseEngineEnabled;        // Pulse engine enabled status, also number of enabled axes
    uint8_t         PulseGeneratorType;        // Pulse engine generator type (0: external, 1: internal 3ch)
    uint8_t         ChargePumpEnabled;         // Charge pump output enabled
    uint8_t         EmergencySwitchPolarity;   // Emergency switch polarity (set to 1 to invert)

    uint8_t         PulseEngineActivated;      // Pulse engine activation status
    uint8_t         LimitStatusP;              // Limit+ status (bit-mapped)
    uint8_t         LimitStatusN;              // Limit- status (bit-mapped)
    uint8_t         HomeStatus;                // Home status (bit-mapped)

    uint8_t         ErrorInputStatus;          // Stepper motor driver error inputs status (bit-mapped)
    uint8_t         MiscInputStatus;           // Miscellaneous digital inputs...
    uint8_t         LimitOverride;             // Limit override status
    uint8_t         LimitOverrideSetup;        // Limit override configuration

    // State of pulse engine - see ePoKeysPEState
    uint8_t         PulseEngineState;

    uint8_t         AxisEnabledMask;           // Bit-mapped output enabled mask
    uint8_t         EmergencyInputPin;
    uint8_t         SyncFastOutputsAxisID;     // Enable synced outputs and select pulse engine axis to map the data to (1-based)

    uint8_t         SyncFastOutputsMapping[8]; // Output mapping for the synced outputs

    // ------ 64-bit region boundary ------
    uint8_t         param1;                    // Parameter 1 value
    uint8_t         param2;
    uint8_t         param3;

    uint8_t         AxisEnabledStatesMask;     // Bit-mapped states, where axis enabled and charge pump signals are active
    uint8_t         PulseEngineStateSetup;     // Pulse engine new state configuration

    uint8_t         SoftLimitStatus;           // Bit-mapped soft-limit statuses per axes
    uint8_t         ExternalRelayOutputs;      // External relay outputs
    uint8_t         ExternalOCOutputs;         // External open-collector outputs
    uint8_t         PulseEngineBufferSize;     // Buffer size information...

    uint8_t         motionBufferEntriesAccepted;
    uint8_t         newMotionBufferEntries;

    uint8_t         HomingStartMaskSetup;      // Bit-mapped axes indexes to be homed
    uint8_t         ProbeStartMaskSetup;       // Bit-mapped axis indexes for probing

    uint8_t         ProbeInput;                // Probe input (0:disabled, 1-8:external inputs, 9+ Pin ID-9)
    uint8_t         ProbeInputPolarity;        // Probe input polarity
    uint8_t         ProbeStatus;               // Probe status (probe completion bit-mapped status)

    // ------ 64-bit region boundary ------
    uint8_t         MotionBuffer[448];         // Motion buffer

    // ------ 64-bit region boundary ------
    float           ProbeSpeed;                // Probe speed (ratio of the maximum speed)
    float           reservedf;

    uint32_t        DebugValues[16];

    uint16_t        BacklashWidth[8];          // half of real backlash width
    int16_t         BacklashRegister[8];       // current value of the backlash register
    uint8_t         BacklashAcceleration[8];   // in pulses per ms^2
    uint8_t         BacklashCompensationEnabled;
    uint8_t         BacklashCompensationMaxSpeed;

    uint8_t         reserved_back[2];

    uint8_t         TriggerPreparing;
    uint8_t         TriggerPrepared;
    uint8_t         TriggerPending;
    uint8_t         TriggerActive;

    int32_t         SpindleSpeedEstimate;
    int32_t         SpindlePositionError;
    uint32_t        SpindleRPM;
    uint32_t        spindleIndexCounter;

    uint8_t         DedicatedLimitNInputs;
    uint8_t         DedicatedLimitPInputs;
    uint8_t         DedicatedHomeInputs;
    uint8_t         TriggerIngnoredAxisMask;

    uint32_t        EncoderIndexCount;
    uint32_t        EncoderTicksPerRotation;
    uint32_t        EncoderVelocity;
    uint32_t        reservedEnd;

    uint8_t         InternalDriverStepConfig[4];
    uint8_t         InternalDriverCurrentConfig[4];

} sPoKeysPEv2;
```

### Pulse Engine v2 Commands

```c
// Get status of Pulse engine
POKEYSDECL int32_t PK_PEv2_StatusGet(sPoKeysDevice * device);
// Get additional status data
POKEYSDECL int32_t PK_PEv2_Status2Get(sPoKeysDevice * device);
// Configure (setup) the pulse engine
POKEYSDECL int32_t PK_PEv2_PulseEngineSetup(sPoKeysDevice * device);
// Read additional parameters
POKEYSDECL int32_t PK_PEv2_AdditionalParametersGet(sPoKeysDevice * device);
// Set additional parameters
POKEYSDECL int32_t PK_PEv2_AdditionalParametersSet(sPoKeysDevice * device);
// Retrieve single axis parameters. Axis ID is in param1
POKEYSDECL int32_t PK_PEv2_AxisConfigurationGet(sPoKeysDevice * device);
// Set single axis parameters. Axis ID is in param1
POKEYSDECL int32_t PK_PEv2_AxisConfigurationSet(sPoKeysDevice * device);
// Set positions - param2 is used for bit-mapped axis selection
POKEYSDECL int32_t PK_PEv2_PositionSet(sPoKeysDevice * device);
// Set pulse engine state
POKEYSDECL int32_t PK_PEv2_PulseEngineStateSet(sPoKeysDevice * device);
// Execute the move. Position or speed is specified by the ReferencePositionSpeed
POKEYSDECL int32_t PK_PEv2_PulseEngineMove(sPoKeysDevice * device);
// Execute the move in PV (position-velocity) mode. Position is specified by the ReferencePositionSpeed, relative velocity by ReferenceVelocityPV - requires FW 4.4.7 or newer
POKEYSDECL int32_t PK_PEv2_PulseEngineMovePV(sPoKeysDevice * device);
// Read external outputs state - save them to ExternalRelayOutputs and ExternalOCOutputs
POKEYSDECL int32_t PK_PEv2_ExternalOutputsGet(sPoKeysDevice * device);
// Set external outputs state (from ExternalRelayOutputs and ExternalOCOutputs)
POKEYSDECL int32_t PK_PEv2_ExternalOutputsSet(sPoKeysDevice * device);
// Transfer motion buffer to device. The number of new entries (newMotionBufferEntries) must be specified
// The number of accepted entries is saved to motionBufferEntriesAccepted.
// In addition, pulse engine state is read (PEv2_GetStatus)
POKEYSDECL int32_t PK_PEv2_BufferFill(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_BufferFill_16(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_BufferFillLarge(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_BufferFillLarge_16(sPoKeysDevice * device);
// Clear motion buffer in device
POKEYSDECL int32_t PK_PEv2_BufferClear(sPoKeysDevice * device);
// Reboot pulse engine v2
POKEYSDECL int32_t PK_PEv2_PulseEngineReboot(sPoKeysDevice * device);
// Start the homing procedure. Home offsets must be provided in the HomeOffsets
// Axes to home are selected as bit-mapped HomingStartMaskSetup value
POKEYSDECL int32_t PK_PEv2_HomingStart(sPoKeysDevice * device);
// Finish the homing procedure
POKEYSDECL int32_t PK_PEv2_HomingFinish(sPoKeysDevice * device);
// Star the probing procedure.
// ProbeMaxPosition defines the maximum position in position ticks where probing error will be thrown
// ProbeSpeed defines the probing speed (1 = max speed)
// ProbeInput defines the extenal input (values 1-8) or PoKeys pin (0-based Pin ID + 9)
// ProbeInputPolarity defines the polarity of the probe signal
POKEYSDECL int32_t PK_PEv2_ProbingStart(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ProbingHybridStart(sPoKeysDevice * device);
// Finish the probing procedure. Probe position and status are saved to ProbePosition and ProbeStatus
POKEYSDECL int32_t PK_PEv2_ProbingFinish(sPoKeysDevice * device);
// Same as previous command, except for pulse engine states not being reset to 'STOPPED'
POKEYSDECL int32_t PK_PEv2_ProbingFinishSimple(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_SyncedPWMSetup(sPoKeysDevice * device, uint8_t enabled, uint8_t srcAxis, uint8_t dstPWMChannel);
POKEYSDECL int32_t PK_PEv2_SyncOutputsSetup(sPoKeysDevice * device);

// Get/Set internal motor drivers parameters
POKEYSDECL int32_t PK_PEv2_InternalDriversConfigurationGet(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_InternalDriversConfigurationSet(sPoKeysDevice * device);

POKEYSDECL int32_t PK_PEv2_ThreadingPrepareForTrigger(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingForceTriggerReady(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingTrigger(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingRelease(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingStatusGet(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingCancel(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_ThreadingSetup(sPoKeysDevice * device, uint8_t sensorMode, uint16_t ticksPerRevolution, uint16_t tagetSpindleRPM, uint16_t filterGainSpeed, uint16_t filterGainPosition);

POKEYSDECL int32_t PK_PEv2_BacklashCompensationSettings_Get(sPoKeysDevice * device);
POKEYSDECL int32_t PK_PEv2_BacklashCompensationSettings_Set(sPoKeysDevice * device);
```

## Examples and Usage Information for PEv2 Commands and Enumerations

### Example Usage of PEv2 Commands

#### 1. **PK_PEv2_StatusGet**

```c
sPoKeysDevice device;
int32_t status = PK_PEv2_StatusGet(&device);
if (status == PK_OK) {
    // Process the status information
    uint8_t pulseEngineState = device.PEv2.PulseEngineState;
    // Further processing...
}
```

#### 2. **PK_PEv2_PulseEngineSetup**

```c
sPoKeysDevice device;
device.PEv2.PulseEngineEnabled = 1;
device.PEv2.ChargePumpEnabled = 1;
device.PEv2.PulseGeneratorType = 0;
device.PEv2.PulseEngineBufferSize = 64;
device.PEv2.EmergencySwitchPolarity = 0;
device.PEv2.AxisEnabledStatesMask = 0xFF;

int32_t setupStatus = PK_PEv2_PulseEngineSetup(&device);
if (setupStatus == PK_OK) {
    // Pulse engine setup successful
}
```

#### 3. **PK_PEv2_AxisConfigurationGet**

```c
sPoKeysDevice device;
device.PEv2.param1 = 0; // Axis ID

int32_t configStatus = PK_PEv2_AxisConfigurationGet(&device);
if (configStatus == PK_OK) {
    // Process the axis configuration
    uint8_t axisConfig = device.PEv2.AxesConfig[0];
    // Further processing...
}
```

#### 4. **PK_PEv2_AxisConfigurationSet**

```c
sPoKeysDevice device;
device.PEv2.param1 = 0; // Axis ID
device.PEv2.AxesConfig[0] = PK_AC_ENABLED | PK_AC_SOFT_LIMIT_ENABLED;
device.PEv2.AxesSwitchConfig[0] = PK_ASO_SWITCH_LIMIT_N | PK_ASO_SWITCH_HOME;
device.PEv2.PinHomeSwitch[0] = 1;
device.PEv2.PinLimitMSwitch[0] = 2;
device.PEv2.PinLimitPSwitch[0] = 3;
device.PEv2.HomingSpeed[0] = 50;
device.PEv2.HomingReturnSpeed[0] = 25;
device.PEv2.MPGjogEncoder[0] = 1;
device.PEv2.MaxSpeed[0] = 1000.0f;
device.PEv2.MaxAcceleration[0] = 500.0f;
device.PEv2.MaxDecceleration[0] = 500.0f;
device.PEv2.SoftLimitMinimum[0] = -10000;
device.PEv2.SoftLimitMaximum[0] = 10000;
device.PEv2.MPGjogMultiplier[0] = 1;
device.PEv2.AxisEnableOutputPins[0] = 1;
device.PEv2.InvertAxisEnable[0] = 0;
device.PEv2.FilterLimitMSwitch[0] = 0;
device.PEv2.FilterLimitPSwitch[0] = 0;
device.PEv2.FilterHomeSwitch[0] = 0;
device.PEv2.HomingAlgorithm[0] = 0;
device.PEv2.HomeBackOffDistance[0] = 100;
device.PEv2.MPGjogDivider[0] = 1;
device.PEv2.AxisSignalOptions[0] = PK_ASO_INVERT_STEP;
device.PEv2.FilterProbeInput = 0;

int32_t setConfigStatus = PK_PEv2_AxisConfigurationSet(&device);
if (setConfigStatus == PK_OK) {
    // Axis configuration set successfully
}
```

#### 5. **PK_PEv2_PositionSet**

```c
sPoKeysDevice device;
device.PEv2.param2 = 0x01; // Bit-mapped axis selection (Axis 1)
device.PEv2.PositionSetup[0] = 5000; // Position for Axis 1

int32_t positionSetStatus = PK_PEv2_PositionSet(&device);
if (positionSetStatus == PK_OK) {
    // Position set successfully
}
```

#### 6. **PK_PEv2_PulseEngineStateSet**

```c
sPoKeysDevice device;
device.PEv2.PulseEngineStateSetup = PK_PEState_peRUNNING;
device.PEv2.LimitOverrideSetup = 0;
device.PEv2.AxisEnabledMask = 0xFF;

int32_t stateSetStatus = PK_PEv2_PulseEngineStateSet(&device);
if (stateSetStatus == PK_OK) {
    // Pulse engine state set successfully
}
```

#### 7. **PK_PEv2_PulseEngineMove**

```c
sPoKeysDevice device;
device.PEv2.ReferencePositionSpeed[0] = 10000; // Position for Axis 1

int32_t moveStatus = PK_PEv2_PulseEngineMove(&device);
if (moveStatus == PK_OK) {
    // Move command executed successfully
}
```

#### 8. **PK_PEv2_PulseEngineMovePV**

```c
sPoKeysDevice device;
device.PEv2.param2 = 0x01; // Bit-mapped axis selection (Axis 1)
device.PEv2.ReferencePositionSpeed[0] = 10000; // Position for Axis 1
device.PEv2.ReferenceVelocityPV[0] = 0.5f; // Velocity for Axis 1

int32_t movePVStatus = PK_PEv2_PulseEngineMovePV(&device);
if (movePVStatus == PK_OK) {
    // Move PV command executed successfully
}
```

#### 9. **PK_PEv2_BufferFill**

```c
sPoKeysDevice device;
device.PEv2.newMotionBufferEntries = 10;
memcpy(device.PEv2.MotionBuffer, motionData, sizeof(motionData));

int32_t bufferFillStatus = PK_PEv2_BufferFill(&device);
if (bufferFillStatus == PK_OK) {
    // Buffer filled successfully
}
```

#### 10. **PK_PEv2_BufferClear**

```c
sPoKeysDevice device;

int32_t bufferClearStatus = PK_PEv2_BufferClear(&device);
if (bufferClearStatus == PK_OK) {
    // Buffer cleared successfully
}
```

#### 11. **PK_PEv2_HomingStart**

```c
sPoKeysDevice device;
device.PEv2.HomingStartMaskSetup = 0x01; // Bit-mapped axis selection (Axis 1)
device.PEv2.HomeOffsets[0] = 0; // Home offset for Axis 1

int32_t homingStartStatus = PK_PEv2_HomingStart(&device);
if (homingStartStatus == PK_OK) {
    // Homing procedure started successfully
}
```

#### 12. **PK_PEv2_HomingFinish**

```c
sPoKeysDevice device;
device.PEv2.PulseEngineStateSetup = PK_PEState_peHOME;

int32_t homingFinishStatus = PK_PEv2_HomingFinish(&device);
if (homingFinishStatus == PK_OK) {
    // Homing procedure finished successfully
}
```

#### 13. **PK_PEv2_ProbingStart**

```c
sPoKeysDevice device;
device.PEv2.ProbeStartMaskSetup = 0x01; // Bit-mapped axis selection (Axis 1)
device.PEv2.ProbeMaxPosition[0] = 10000; // Maximum position for Axis 1
device.PEv2.ProbeSpeed = 0.5f; // Probe speed
device.PEv2.ProbeInput = 1; // Probe input
device.PEv2.ProbeInputPolarity = 0; // Probe input polarity

int32_t probingStartStatus = PK_PEv2_ProbingStart(&device);
if (probingStartStatus == PK_OK) {
    // Probing procedure started successfully
}
```

#### 14. **PK_PEv2_ProbingFinish**

```c
sPoKeysDevice device;

int32_t probingFinishStatus = PK_PEv2_ProbingFinish(&device);
if (probingFinishStatus == PK_OK) {
    // Probing procedure finished successfully
    int32_t probePosition = device.PEv2.ProbePosition[0];
    uint8_t probeStatus = device.PEv2.ProbeStatus;
    // Further processing...
}
```

#### 15. **PK_PEv2_BacklashCompensationSettings_Get**

```c
sPoKeysDevice device;

int32_t backlashGetStatus = PK_PEv2_BacklashCompensationSettings_Get(&device);
if (backlashGetStatus == PK_OK) {
    // Backlash compensation settings retrieved successfully
    uint16_t backlashWidth = device.PEv2.BacklashWidth[0];
    int16_t backlashRegister = device.PEv2.BacklashRegister[0];
    uint8_t backlashAcceleration = device.PEv2.BacklashAcceleration[0];
    uint8_t backlashCompensationEnabled = device.PEv2.BacklashCompensationEnabled;
    // Further processing...
}
```

#### 16. **PK_PEv2_BacklashCompensationSettings_Set**

```c
sPoKeysDevice device;
device.PEv2.BacklashWidth[0] = 100;
device.PEv2.BacklashAcceleration[0] = 10;
device.PEv2.BacklashCompensationEnabled = 1;

int32_t backlashSetStatus = PK_PEv2_BacklashCompensationSettings_Set(&device);
if (backlashSetStatus == PK_OK) {
    // Backlash compensation settings set successfully
}
```

#### 17. **PK_PEv2_SyncedPWMSetup**

```c
sPoKeysDevice device;
uint8_t enabled = 1;
uint8_t srcAxis = 0;
uint8_t dstPWMChannel = 1;

int32_t pwmSetupStatus = PK_PEv2_SyncedPWMSetup(&device, enabled, srcAxis, dstPWMChannel);
if (pwmSetupStatus == PK_OK) {
    // Synced PWM setup successful
}
```

### Example Usage of PEv2 Enumerations

#### 1. **ePK_PEState**

```c
sPoKeysDevice device;
device.PEv2.PulseEngineState = PK_PEState_peRUNNING;
```

#### 2. **ePK_PEAxisState**

```c
sPoKeysDevice device;
device.PEv2.AxesState[0] = PK_PEAxisState_axRUNNING;
```

#### 3. **ePK_PEv2_AxisConfig**

```c
sPoKeysDevice device;
device.PEv2.AxesConfig[0] = PK_AC_ENABLED | PK_AC_SOFT_LIMIT_ENABLED;
```

#### 4. **ePK_PEv2_AxisSwitchOptions**

```c
sPoKeysDevice device;
device.PEv2.AxesSwitchConfig[0] = PK_ASO_SWITCH_LIMIT_N | PK_ASO_SWITCH_HOME;
```

#### 5. **ePK_PEv2_AxisSignalOptions**

```c
sPoKeysDevice device;
device.PEv2.AxisSignalOptions[0] = PK_ASO_INVERT_STEP | PK_ASO_INVERT_DIRECTION;
```

## Summary of Key Pointers

### 1. Axis State and Motion

- `device->PEv2.AxesState[]`
- `device->PEv2.CurrentPosition[]`
- `device->PEv2.ReferencePositionSpeed[]`
- `device->PEv2.ReferenceVelocityPV[]`

### 2. Homing and Limits

- `device->PEv2.HomingStartMaskSetup`
- `device->PEv2.AxesSwitchConfig[]`
- `device->PEv2.SignalConfig[]`

### 3. Backlash Compensation

- `device->PEv2.BacklashWidth[]`
- `device->PEv2.BacklashAcceleration[]`
- `device->PEv2.BacklashCompensationEnabled`

### 4. Probing

- `device->PEv2.ProbeStatus`
- `device->PEv2.ProbeSpeed`

### 5. PWM Setup

- `device->PEv2.PWMSetup[]`

## Conclusion

The examples and usage information provided above demonstrate how to use the PEv2 commands and enumerations in the PoKeysLib. By following these examples, developers can effectively integrate and utilize the PEv2 interface in their projects.

