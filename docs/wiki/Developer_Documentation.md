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

Thank you for being a part of the **LinuxCnc_PokeysLibComp** community!
