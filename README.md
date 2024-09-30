# LinuxCnc_PokeysLibComp

## Overview

The LinuxCnc_PokeysLibComp project integrates PoKeys devices with LinuxCNC, providing real-time control and monitoring capabilities. This repository contains the necessary components, scripts, and documentation to set up and use PoKeys devices with LinuxCNC.

## Directory Structure

The repository is organized into the following directories:

- `pokeys_py/`: Contains Python scripts for interfacing with PoKeys devices.
- `pokeys_rt/`: Contains real-time components for PoKeys devices.
- `tests/`: Contains unit tests, integration tests, and other testing-related files.
- `docs/`: Contains project-related documentation.
- `scripts/`: Contains scripts for setup, configuration, and auxiliary tasks.

## Installation

To install the LinuxCnc_PokeysLibComp project, follow these steps:

```bash
git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
cd LinuxCnc_PokeysLibComp
sudo sh install.sh
```

## Hardware Requirements

The project has been tested with the following hardware:

- RaspberryPi4 (for LinuxCNC)
- PoKeys57E
- PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir
- CNC keyboard – PoNETkbd48CNC
- PoRelay8 – Relay extension board
- PoExtBusOC16
- PoKeys57CNC
- PoStep25-32 (v1.3)

## Library Status

The following features have been implemented and tested:

- Connecting to USB devices
- Connecting to network devices
- Reading device data
- Reading digital inputs
- Writing digital outputs
- Reading analog inputs
- PoExtBus writing
- PoExtBus reading
- Setting pin functions
- Reading pin functions
- PWM operations
- Pulse engine operations
- Matrix keyboard setup
- Matrix keyboard reading
- kbd48CNC set LED
- kbd48CNC get Button Status
- Using encoders

## HAL Interface

### Digital Input

Count: 55

#### Pins

- `pokeys.[DevID].digin.[PinID].in`: State of the hardware input
- `pokeys.[DevID].digin.[PinID].in-not`: Inverted state of the input

#### Parameters

- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Pin`: Home switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.Filter`: Digital filter for Home switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.Home.invert`: Invert Home (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Pin`: Limit- switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.Filter`: Digital filter for limit- switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitN.invert`: Invert limit- (PoKeys functionality)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Pin`: Limit+ switch pin (0 for external dedicated input)
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.Filter`: Digital filter for limit+ switch
- `pokeys.[DevID].PEv2.[PEv2Id].digin.LimitP.invert`: Invert limit+ (PoKeys functionality)
- `pokeys.[DevID].PEv2.digin.Emergency.Pin`
- `pokeys.[DevID].PEv2.digin.Emergency.invert`
- `pokeys.[DevID].PEv2.digin.Probe.Pin`
- `pokeys.[DevID].PEv2.digin.Probe.invert`

### Digital Output

Count: 55

#### Pins

- `pokeys.[DevID].digout.[PinID].out`: Value to be written (possibly inverted) to the hardware output

#### Parameters

- `pokeys.[DevID].digout.[PinID].invert`: If TRUE, out is inverted before writing to the hardware

### Analog Input

Count: 7

#### Pins

- `pokeys.[DevID].adcin.[AdcId].value-raw`: The hardware reading
- `pokeys.[DevID].adcin.[AdcId].value`: The hardware reading, scaled according to the scale and offset parameters

#### Parameters

- `pokeys.[DevID].adcin.[AdcId].scale`: The input voltage (or current) will be multiplied by scale before being output to value
- `pokeys.[DevID].adcin.[AdcId].offset`: This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied

### Analog Output

Count: 6

#### Pins

- `pokeys.[DevID].adcout.[AdcId].value`: The value to be written. The actual value output to the hardware will depend on the scale and offset parameters
- `pokeys.[DevID].adcout.[AdcId].enable`: If false, then output 0 to the hardware, regardless of the value pin

#### Parameters

- `pokeys.[DevID].adcout.[AdcId].offset`: This will be added to the value before the hardware is updated
- `pokeys.[DevID].adcout.[AdcId].scale`: This should be set so that an input of 1 on the value pin will cause 1V
- `pokeys.[DevID].adcout.[AdcId].high_limit`: When calculating the value to output to the hardware, if value + offset is greater than high_limit, then high_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].low_limit`: When calculating the value to output to the hardware, if value + offset is less than low_limit, then low_limit will be used instead
- `pokeys.[DevID].adcout.[AdcId].max_v`: Max output voltage
- `pokeys.[DevID].adcout.pwm.period`: PWM period, shared among all channels

### Encoder

Count: 29

#### Pins

- `pokeys.[DevID].encoder.[EncId].count`: Encoder value in counts
- `pokeys.[DevID].encoder.[EncId].position`: Encoder value in position units (see parameter “scale”)
- `pokeys.[DevID].encoder.[EncId].reset`: When True, force counter to zero

#### Parameters

- `pokeys.[DevID].encoder.[EncId].scale`: The scale factor used to convert counts to position units. It is in “counts per position unit”

### Real-Time Clock

#### Pins

- `pokeys.[DevID].rtc.sec`: Second
- `pokeys.[DevID].rtc.min`: Minute
- `pokeys.[DevID].rtc.hour`: Hour
- `pokeys.[DevID].rtc.dow`: Day of week
- `pokeys.[DevID].rtc.dom`: Day of month
- `pokeys.[DevID].rtc.doy`: Day of year
- `pokeys.[DevID].rtc.month`: Month
- `pokeys.[DevID].rtc.year`: Year
- `pokeys.[DevID].rtc.loop_frequ`: Actual loop frequency of pokeys.comp updated after rtc.sec changed
- `pokeys.[DevID].rtc.loop_frequ_demand`: Demand value for loop frequency (if 0, default of 10Hz will be used)

## Running Tests

To ensure the reliability and correctness of the `pokeys_py` component, a comprehensive test suite has been developed. The tests cover various functionalities, including digital I/O, analog I/O, PEv2, counters, PWM outputs, and peripheral devices. The tests are categorized into unit tests, integration tests, functional tests, and performance tests.

### Prerequisites

Before running the tests, make sure you have the following prerequisites installed:

- Python 3.x
- `pytest` testing framework
- `unittest.mock` for mocking PoKeys device responses

You can install `pytest` using pip:

```bash
pip install pytest
```

### Running Unit Tests

Unit tests are designed to test individual functions and methods within the `pokeys_py` component. To run the unit tests, navigate to the `tests` directory and execute the following command:

```bash
pytest test_analog_io.py
pytest test_counter.py
pytest test_digital_io.py
pytest test_pev2_motion_control.py
pytest test_ponet.py
pytest test_pwm.py
```

### Running Integration Tests

Integration tests verify the interaction between the `pokeys_py` component and the PoKeys hardware and LinuxCNC. To run the integration tests, execute the following command:

```bash
pytest test_integration.py
```

### Running Functional Tests

Functional tests simulate real-world usage scenarios for the `pokeys_py` component. To run the functional tests, execute the following command:

```bash
pytest test_functional.py
```

### Running Performance Tests

Performance tests ensure that the `pokeys_py` component meets real-time operation requirements. To run the performance tests, execute the following command:

```bash
pytest test_performance.py
```

### Interpreting Test Results

After running the tests, `pytest` will display the test results in the terminal. The results will indicate the number of tests passed, failed, or skipped. Additionally, you can generate a coverage report to identify untested code.

To generate a coverage report, use the following command:

```bash
pytest --cov=pokeys_py --cov-report=html
```

The coverage report will be generated in the `htmlcov` directory. Open the `index.html` file in a web browser to view the detailed coverage report.

### Hardware-Dependent Tests

Some tests may require physical hardware to run. For hardware-dependent tests, make sure the PoKeys device is connected and properly configured. Follow the setup steps provided in the documentation to ensure the hardware is ready for testing.

### Continuous Integration (CI)

To integrate the tests into an automated testing pipeline, set up continuous integration (CI) to run the tests on every commit or pull request. This ensures that the code is continuously tested and any issues are detected early in the development process.

### Running Tests with Mocks or Simulators

To run tests with mocks or simulators, follow these steps:

1. Set the environment variable `CI` to `true` to enable the use of mocks in the CI environment.
2. Run the tests as usual using `pytest`.

For example:

```bash
export CI=true
pytest test_analog_io.py
pytest test_counter.py
pytest test_digital_io.py
pytest test_pev2_motion_control.py
pytest test_ponet.py
pytest test_pwm.py
pytest test_integration.py
pytest test_functional.py
pytest test_performance.py
```

### Setting Environment Variables

To toggle between real and mock devices, set the `CI` environment variable accordingly:

- For running tests with real hardware, unset the `CI` environment variable or set it to `false`:

```bash
unset CI
# or
export CI=false
```

- For running tests with mocks or simulators, set the `CI` environment variable to `true`:

```bash
export CI=true
```

## Documentation

The documentation provides clear instructions on running tests, setup steps for hardware-dependent tests, and interpreting test results and coverage reports. It is essential to keep the documentation up to date as new features are implemented and tests are added.

By following these instructions, you can ensure the reliability, correctness, and performance of the `pokeys_py` component, making it easier to maintain and develop in the future.

## Configuring GitHub Actions Runners

To ensure that the CI workflow runs smoothly, it is important to configure GitHub Actions runners properly. This section provides instructions for setting up self-hosted runners and verifying the required permissions and access for runners.

### Setting Up Self-Hosted Runners

1. **Create a Runner**: Follow the instructions in the GitHub documentation to create a self-hosted runner. You can find the documentation [here](https://docs.github.com/en/actions/hosting-your-own-runners/adding-self-hosted-runners).

2. **Install the Runner**: Download and install the runner on your machine. Make sure to follow the installation steps provided in the documentation.

3. **Configure the Runner**: Configure the runner by running the `config.sh` script. Provide the necessary details such as the repository URL, runner name, and labels.

4. **Start the Runner**: Start the runner by running the `run.sh` script. The runner should now be online and ready to pick up jobs.

### Verifying Permissions and Access

1. **Repository Access**: Ensure that the runner has access to the repository. This can be done by adding the runner to the repository settings under the "Actions" tab.

2. **Permissions**: Verify that the runner has the necessary permissions to access the repository. This includes read and write permissions for the repository.

3. **Runner Logs**: Check the runner logs for any errors or issues that might be preventing the job from being picked up. The logs can be found in the `runner/logs` directory.

4. **Self-Hosted Runners**: If using self-hosted runners, ensure they are online and properly configured. This includes verifying the network connectivity and ensuring that the runner is not blocked by any firewall or security settings.

5. **GitHub-Hosted Runners**: If using GitHub-hosted runners, verify that there are no restrictions or limitations on the number of concurrent jobs. This can be done by checking the GitHub Actions settings in the repository.

By following these steps, you can ensure that the GitHub Actions runners are properly configured and have the necessary permissions and access to run the CI workflow successfully.

## Code Review Guidelines

Code reviews are an essential part of our development process. They help ensure code quality, maintainability, and foster collaboration within the team. The following guidelines outline the expectations for code reviews:

1. **Code Style**: Adhere to the project's coding standards and style guidelines. Consistent code style makes the codebase easier to read and maintain.
2. **Documentation**: Ensure that code is well-documented. This includes inline comments, function/method documentation, and any necessary external documentation.
3. **Testing**: Verify that the code includes appropriate tests. This includes unit tests, integration tests, and any other relevant tests.
4. **Performance**: Consider the performance implications of the code. Ensure that the code is efficient and does not introduce unnecessary performance bottlenecks.
5. **Security**: Review the code for potential security vulnerabilities. Ensure that the code follows best practices for security.

For detailed guidelines, please refer to the [Code Review Guidelines](CODE_REVIEW_GUIDELINES.md).

## Contributing

We welcome contributions from the community to help improve the project. Please take a moment to review the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Branch Naming Conventions

To keep the repository organized, please follow these branch naming conventions:

- **Feature Branches**: `feature/{feature-name}`
- **Bug Fix Branches**: `bugfix/{bug-name}`
- **Improvement Branches**: `improvement/{improvement-name}`
- **Hotfix Branches**: `hotfix/{hotfix-name}`

## Creating Separate Branches for Each Feature or Bug Fix

Each new feature or bug fix should have its own branch created from the latest version of the main branch. The branch should focus on one feature or issue to ensure isolated development.

## Conformity with LinuxCNC Guidelines and Canonical Device Interface

This repository aligns with LinuxCNC’s development practices and canonical interfaces. The components have been reviewed and updated to follow LinuxCNC coding guidelines, ensuring that HAL and INI files conform to LinuxCNC’s structure and formatting. Real-time components comply with LinuxCNC’s real-time constraints, and all components follow the canonical interface definitions. The code adheres to LinuxCNC's code style and formatting guidelines, and test cases have been created and run to validate conformity.

## Using Docker for Testing

To simplify the testing process and ensure consistency across different environments, we have integrated Docker into our testing workflow. This allows you to run tests inside a Docker container, providing an isolated and reproducible environment.

### Building the Docker Image

To build the Docker image for testing, follow these steps:

1. Ensure you have Docker installed on your machine. You can download and install Docker from [here](https://www.docker.com/get-started).

2. Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
cd LinuxCnc_PokeysLibComp
```

3. Build the Docker image using the provided `Dockerfile`:

```bash
docker build -t linuxcnc-pokeyslibcomp .
```

### Running Tests Inside the Docker Container

Once the Docker image is built, you can run tests inside the Docker container. This ensures that tests are executed in a consistent environment, regardless of the host system.

To run tests inside the Docker container, use the following command:

```bash
docker run --rm linuxcnc-pokeyslibcomp
```

This command will start a Docker container from the `linuxcnc-pokeyslibcomp` image and execute the tests inside the container. The `--rm` flag ensures that the container is removed after the tests are completed.

### Benefits of Using Docker for Testing

- **Consistency**: Every test run in the same environment, reducing variability across different CI environments.
- **Simplicity**: The Docker image provides a pre-configured LinuxCNC environment, which minimizes setup complexity in the CI pipeline.
- **Isolation**: Running tests inside Docker ensures no interference with other system processes or environment variables.
- **Cross-platform Compatibility**: The Docker image can be used across different platforms, making local testing and development easier for contributors.

By using Docker for testing, you can ensure that tests are executed in a consistent and isolated environment, reducing the chances of environment-related issues and making it easier to reproduce and debug test failures.

## Using the `create_images.sh` Script

The `create_images.sh` script automates the build and image creation process for Raspberry Pi 4 Bullseye, Bookworm, and amd64 hybrid. Follow these steps to use the script:

1. Ensure you have the necessary dependencies installed for cross-compiling for ARM and natively compiling for amd64.

2. Run the `create_images.sh` script:

```bash
chmod +x create_images.sh
./create_images.sh
```

The script will build and create images for Raspberry Pi 4 Bullseye, Bookworm, and amd64 hybrid, following the structure and format of existing LinuxCNC releases. It will also test and validate the generated images.

## Downloading Generated Images

The generated images are stored in the repository's releases or a suitable cloud storage platform. You can download the images from the following links:

- [Raspberry Pi 4 Bullseye Image](#)
- [Raspberry Pi 4 Bookworm Image](#)
- [amd64 Hybrid Image](#)

## Note on GitHub Actions Workflow

In the GitHub Actions workflow file, ensure that the `runs-on` key is correctly specified. The correct syntax for the `runs-on` key should be `runs-on: ${{ matrix.os }}` instead of `runs-on: [self-hosted, ${{ matrix.os }}]`.

## Supported Platforms and LinuxCNC Versions

The following platforms and LinuxCNC versions are supported:

- **Raspberry Pi 4**:
  - Bullseye
  - Bookworm
- **amd64 Hybrid Images**:
  - LinuxCNC 2.9.2
  - LinuxCNC 2.9.3
  - Latest available release

## Replicating CI Environments Locally

To replicate the CI environments locally, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```

2. **Set Up the Environment**:
   - Install the necessary dependencies:
     ```bash
     sudo apt-get update
     sudo apt-get install -y qemu qemu-user-static debootstrap
     ```

3. **Run the CI Pipeline Locally**:
   - Execute the CI pipeline script:
     ```bash
     ./create_images.sh
     ```

4. **Test and Validate the Generated Images**:
   - Use QEMU to test and validate the generated images:
     ```bash
     sudo qemu-system-aarch64 -M raspi3 -kernel rpi4_bullseye/boot/vmlinuz-*-arm64 -initrd rpi4_bullseye/boot/initrd.img-*-arm64 -append "root=/dev/sda2" -hda rpi4_bullseye.img -nographic
     ```

By following these steps, you can replicate the CI environments locally and ensure that the generated images are functioning correctly.

## Labels for Issues and Pull Requests

To make it easier for new contributors to get started, we use specific labels to identify issues and pull requests that are suitable for beginners. These labels help contributors find tasks that match their experience level and ensure a smooth onboarding process.

### Labels

- `good first issue`: For issues that are simple, well-documented, and require minimal background knowledge.
- `beginner friendly`: For issues that require basic knowledge of the project but are ideal for newcomers.
- `help wanted`: For issues where assistance is needed, often broader in scope but still accessible.
- `documentation`: For tasks that involve contributing to documentation (which can be a good entry point for new contributors).
- `low-hanging fruit`: For smaller, isolated issues that are quick to solve.

### Using Labels

When creating or reviewing issues and pull requests, apply the appropriate labels to help new contributors find suitable tasks. Here are some guidelines:

- **Tagging Issues**: When creating new issues, apply the above labels if they are suitable for beginners or contributors with less experience in the project.
- **Tagging Pull Requests**: Apply labels to pull requests that address beginner-friendly issues or are written by new contributors to encourage review and feedback.

### Encouraging New Contributors

We encourage new contributors to look for issues with these labels. If you are a first-time contributor, start by exploring the issues labeled `good first issue`, `beginner friendly`, or `documentation`. These tasks are designed to help you get started with the project and build your confidence as a contributor.
