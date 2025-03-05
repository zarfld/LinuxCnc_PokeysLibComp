# LinuxCnc_PokeysLibComp

## Overview

The LinuxCnc_PokeysLibComp project integrates PoKeys devices with LinuxCNC, providing control and monitoring capabilities. This repository contains the necessary components, scripts, and documentation to set up and use PoKeys devices with LinuxCNC.

## Directory Structure

The repository is organized into the following directories:

- `pokeys_uspace/`: Contains Source of pokeys userspace component for interfacing with PoKeys devices.
- `pokeys_py/`: prepared for Contains Python scripts for interfacing with PoKeys devices.
- `pokeys_rt/`: prepared for real-time components for PoKeys devices.
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

- [e] Connecting to USB devices
- [x] Connecting to network devices
- [x] Reading device data
- [x] Reading digital inputs
- [x] Writing digital outputs
- [x] Reading analog inputs
- [x] PoExtBus writing
- [x] PoExtBus reading
- [ ] LCD operations
- [ ] Matrix LED
- [x] Setting pin functions
- [x] Reading pin functions
- [ ] Setting pin key codes
- [ ] Reading pin key codes
- [X] PWM operations
- [x] Pulse engine operations
- [i] Matrix keyboard setup
- [i] Matrix keyboard reading
- [x] kbd48CNC set LED
- [x] kbd48CNC get Button Status
- [x] Using encoders
- [ ] I2C operations
- [ ] SPI operation
- [ ] PoIL operations
- [i] 1-wire operations


x ... available
i ... implemented but not tested
! ... implemented but not working yet
p ... planned
e ... known issue 


### PoRelay8:
- [x] needs to be connected in Parallel to kbd48CNC with white PoExt Cable OR using Canbus (connecting it using the Red PoExtBus cable to the i2C extender which is delivered with kbd48CNC will not work)
- [x] if it is used together with "non smart" PoExtBus devices (e.g. PoExtBusOC16) use "PoExtBus - Smart" page on Pokeys-SW to set "Sequence ID"
- [ ] inputs not available (yet?)

### PoExtBusOC16
- [x] can be set using pokeys.0.PoExtBus.#.Out-0..7  (all 16outs can be used pokeys.0.PoExtBus.0 and pokeys.0.PoExtBus.1)
- [ ] inputs not available

### PoExtBusOC16CNC
- see "PulseEnginev2"

### kbd48CNC:
- [x] using as PoNet-extension buard attached to Pokeys57E
- [x] using CanBus (6Pin MicroMatch Connector) directly on Pokeys57CNC

### Pokeys57E
- [x] connected using Ethernet
- [x] Read&Set of Digital IO on HalPins pokeys.0.Pins.*.DigitalValueGet / DigitalValueSet
- [x] Readt of AnalogValue IO on HalPins pokeys.0.Pins.*.AnalogValue on the pins that support that
- periphals listed above was tested on this device.

changing structure of IOs to "Canonical Device Interface" in work

### Pokeys57CNC
- [x] connected using USB
- [x] connected using Ethernet
- [x] LinuxCNC example config in Work (using the pinsetup as available on PCB)
- [x] Switchng OC Outputs  pokeys.0.PEv2.ExternalOCOutput.0..3
- [x] Switchng Relay Outputs (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1) (SSR1 & 2 -> pokeys.0.PEv2.ExternalRelayOutput.0 & 1)
- using USB connection seems "faster" than using ethernet - may depend on networksetup (networkswitch vs. direct connection)

### Pokeys57E
- [ ] connected using USB  (currently could not connect on newer distributions see issue #231 )

### PulseEnginev2 
- [x] Setting Status Running/Stop depending on LinuxCNC state ("machine-on")
- [x] Reading Status of Limit+/- and Home switches
- [x] setting External OC and RelayOutputs - in case that "Extended IO" is available and activated (Pokeys57CNC)
- [x] setting PinConfiguration for Limits, Home, Probe & Emergency switches based in LinuxCNC-INI configuration - not reliable
- [x] parametrizing MaxSpeed, MaxAccelleration, ... based on LinuxCNC configuarion (mm/sec to pulses/sec conversion is being done using JOINT.*.STEPSCALE)


## HAL Interface

### Pokeys Pin 
![image](https://github.com/user-attachments/assets/d0f7cb2d-aeb1-4d77-87f3-09857c2719b9)


Count: 55

#### Parameters

- pokeys.[DevID].Pin.[PinID].PinFunction			  int 		PinFunction to be set for corresponding pin see ePK_PinCap

the PinFunction can be set using followin enumeration. Note that e.g. digitalInput or digitalOutput could be directly inverted on pokeys by adding value 128 (PK_PinCap_digitalOutput+PK_PinCap_invertPin or 2 + 128 = 130):

add. Note: for PK_PinCap_analogOutput has not been used so far currently only PWM functionality has been used instead that is what is being used for (pins 18, 20, 21 and 22)

```
      enum ePK_PinCap
      {
          PK_PinCap_pinRestricted  = 0,           // Pin is not used
          PK_PinCap_reserved       = 1,           // --
          PK_PinCap_digitalInput   = 2,           // Digital input
          PK_PinCap_digitalOutput  = 4,           // Digital output
          PK_PinCap_analogInput    = 8,           // Analog input (only on selected pins)
          PK_PinCap_analogOutput   = 16,          // Analog output (only on selected pins)
          PK_PinCap_triggeredInput = 32,          // Triggered input
          PK_PinCap_digitalCounter = 64,          // Digital counter (only on selected pins)
          PK_PinCap_invertPin      = 128          // Invert digital pin polarity (set together with digital input, output or triggered input)
      };
```

### Digital Input

Count: 55

#### Pins

- pokeys.[DevID].digin.[PinID].in			  bit		State of the hardware input
- pokeys.[DevID].digin.[PinID].in-not		bit		Inverted state of the input.

#### Parameters

- `pokeys.[DevID].digin.[PinID].invert`  bit     invert Pin on pokeys using PK_PinCap_invertPin on PinFunction

### Digital Output

Count: 55

#### Pins

- `pokeys.[DevID].digout.[PinID].out`  bit     Value to be written (possibly inverted) to the hardware output

#### Parameters

- `pokeys.[DevID].digout.[PinID].invert`  bit     invert Pin on pokeys using PK_PinCap_invertPin on PinFunction

### Analog Input

Count: 7

#### Pins

- `pokeys.[DevID].adcin.[AdcId].value-raw`			float    The hardware reading 
- `pokeys.[DevID].adcin.[AdcId].value`				  float    The hardware reading, scaled according to the scale and offset parameters

#### Parameters

- `pokeys.[DevID].adcin.[AdcId].scale`				  float    The input voltage (or current) will be multiplied by scale before being output to value
- `pokeys.[DevID].adcin.[AdcId].offset`				  float    This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied

### Analog Output (PWM)

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

- pokeys.[DevID].encoder.[EncId].count;			S32	Encoder value in counts.
- pokeys.[DevID].encoder.[EncId].position;		FLOAT Encoder value in position units (see parameter “scale”).
- ~~pokeys.[DevID].encoder.[EncId].velocity;		FLOAT Velocity in position units per second~~
- pokeys.[DevID].encoder.[EncId].reset;			BIT	When True, force counter to zero
- ~~pokeys.[DevID].encoder.[EncId].index-enable;	BIT	(bidirectional) When True, reset to zero on next index pulse, and set pin False.~~

#### Parameters

- pokeys.[DevID]. encoder.[EncId].scale	FLOAT	"The scale factor used to convert counts to position units. It is in “counts per position unit”";

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

## CI Job for Labeling Issues

### Overview

A new CI job has been added to automatically review issues in the repository that are not yet labeled and assign the correct label based on the issue content. This helps maintainers and contributors identify tasks efficiently.

### Configuration

The CI job is configured using GitHub Actions. The workflow file is named `label_issues.yml` and is located in the `.github/workflows/` directory.

### Triggering the CI Job

The CI job is triggered automatically in the following scenarios:

- When a new issue is created.
- When an existing issue is updated (e.g., issue title or body is modified).
- Periodically (e.g., once per day) to review unlabeled issues and assign labels if any were missed.

### Label Assignment

The CI job uses a Python script named `label_issues.py` to analyze the issue content and assign labels based on keywords or an NLP model. The script uses the `requests` library to interact with the GitHub API.

### Confidence Threshold

The CI job implements a confidence threshold to avoid incorrect labeling. If the confidence level is high (e.g., over 80%), the label is applied automatically. If the confidence level is low, the issue is flagged for manual review by maintainers with a comment like:

> "This issue could not be confidently labeled. Please review manually."

### Manual Override

Maintainers can manually override the label if the CI job incorrectly assigns one. The CI job also applies a `needs-review` label for issues where it is unsure about the appropriate label.

### Excluding Issues with Existing Labels

The CI job excludes issues that already have one or more labels. It does not remove or change existing labels unless flagged by a maintainer.

### Logging and Reporting

The CI job logs its actions, such as which issues were labeled and which issues it flagged for manual review. Optionally, it can send a daily or weekly report summarizing its actions to maintainers.

### Error Handling

In case the CI job fails or cannot process an issue (e.g., due to parsing errors), it flags the issue with a `needs-review` label and provides a log or error report.

### Examples of Automatically Assigned Labels

The following are examples of labels that can be automatically assigned by the CI job based on issue content:

- `good first issue`: For issues that include terms like "beginner", "easy", "first-time", "simple", or "docs".
- `documentation`: For issues that include terms related to documentation, like "README", "docs", "documentation".
- `help wanted`: For issues that contain requests for help.
- `bug`: For issues that include terms like "bug", "error", "issue".
- `enhancement`: For issues that include terms like "feature", "enhancement", "improvement".

By implementing this CI job, the repository ensures that issues are well-organized and labeled appropriately, improving issue management and making it easier for contributors to navigate the repository.

## Swapping Between Real and Mocked `pokeyslib`

To facilitate testing and development, you can easily swap between the real `pokeyslib` and the mocked `pokeyslib`. This allows you to run tests without the need for physical PoKeys devices.

### Using the Mocked `pokeyslib`

1. **Set the Environment Variable**: Set the `CI` environment variable to `true` to enable the use of the mocked `pokeyslib`.

```bash
export CI=true
```

2. **Run the Tests**: Execute the tests as usual using `pytest`.

```bash
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

### Using the Real `pokeyslib`

1. **Unset the Environment Variable**: Unset the `CI` environment variable or set it to `false` to use the real `pokeyslib`.

```bash
unset CI
# or
export CI=false
```

2. **Run the Tests**: Execute the tests as usual using `pytest`.

```bash
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

### Example Test Configuration

Here is an example of how to configure and run tests with the mocked `pokeyslib`:

```bash
export CI=true
pytest test_counter.py
```

This will run the `test_counter.py` tests using the mocked `pokeyslib`, allowing you to validate the behavior of the `pokeys_py` component without the need for physical hardware.

By following these instructions, you can easily swap between the real and mocked `pokeyslib`, enabling comprehensive testing and development of the `pokeys_py` component.

### Mocked `pokeyslib` Design

#### 1. **Function Mocks**

Each function provided by the real `pokeyslib` will have a mocked version with the **exact same signature**. The mocked function will simulate the expected hardware behavior.

- **Example**:

Real `pokeyslib.h`:

```c
POKEYSDECL int32_t PK_DigitalIOGet(sPoKeysDevice* device, uint8_t pinID, uint8_t* pinValue);
```

Mocked Version:

```cpp
int32_t PK_DigitalIOGet(sPoKeysDevice* device, uint8_t pinID, uint8_t* pinValue) {
    *pinValue = 1;  // Simulate the pin being high
    return PK_OK;
}
```

All other functions in the library, such as `PK_DigitalIOSet`, `PK_AnalogIOGet`, `PK_PEv2_PulseEngineMove`, etc., will follow the same approach, mimicking the real behavior of the functions.

#### 2. **Structures and Enums**

All the structures and enums defined in the original `pokeyslib.h` need to be replicated exactly in the mock to ensure **binary compatibility** when linking against components.

- **Example**:

Real `pokeyslib.h`:

```c
typedef struct sPoKeysDevice_Info {
    char deviceName[32];
    uint32_t serialNumber;
    uint88 firmwareVersionMajor;
    uint8_t firmwareVersionMinor;
    uint8_t numberOfPins;
} sPoKeysDevice_Info;
```

Mocked `pokeyslib.h` (identical to the real header):

```cpp
typedef struct sPoKeysDevice_Info {
    char deviceName[32];
    uint32_t serialNumber;
    uint88 firmwareVersionMajor;
    uint8_t firmwareVersionMinor;
    uint8_t numberOfPins;
} sPoKeysDevice_Info;
```

#### 3. **State Management**

The mocked library will maintain **internal state** to simulate real device behavior across function calls. This is particularly important for functions like `PK_DigitalIOSet` and `PK_DigitalIOGet`, where setting an output pin should affect subsequent input reads.

- A **state map** will track the value of pins, counters, PWM channels, and any other device attributes.
- **Logging** should be available to track the internal state during tests.

#### 4. **Configuration and Setup**

The mock library must allow for **custom configuration** of the device state prior to tests. This includes setting up the initial state of pins, analog values, and other peripheral settings.

- For example, a test might configure the mock device to have pin 0 as **high** before executing a series of tests.

```cpp
void setup_mock_device() {
    device_state.pins[0] = 1;  // Simulate pin 0 being high
    device_state.pins[1] = 0;  // Simulate pin 1 being low
}
```

### Mock Library Integration

1. **Linking the Mock Library**:
    - During testing, the **mocked `pokeyslib`** will be linked into the test environment.
    - During production builds, the **real `pokeyslib`** will be linked.

    **CMake Example**:
    ```cmake
    if(BUILD_TESTS)
        target_link_libraries(my_tests PRIVATE mocked_pokeyslib)
    else()
        target_link_libraries(my_project PRIVATE real_pokeyslib)
    endif()
    ```

2. **Replacing the Real Library**:
    - The CI pipeline or build system will determine whether to link against the **real** or **mocked** version of `pokeyslib`.

### Testing with the Mock

- The **mocked `pokeyslib`** should work seamlessly with existing unit tests and integration tests.
- **Test components** (e.g., LinuxCNC HAL files) should use the mock for testing to simulate **device interactions**.

```cpp
TEST(MockPoKeysTest, TestDigitalIO) {
    sPoKeysDevice* device = CreateMockDevice();
    uint8_t pinValue = 0;

    PK_DigitalIOGet(device, 0, &pinValue);
    ASSERT_EQ(pinValue, 1);  // Check that the mock returns the expected value
}
```

- Additionally, the mock can generate **logs** of all the interactions to validate the sequence of events and simulate edge cases like connection loss, invalid pin values, etc.

### Steps for Implementation

1. **Identify all Functions and Structures**:
   - Review `pokeyslib.h` to identify all the functions, structures, and enums that need to be mocked.

2. **Implement Mocked Functions**:
   - Implement each function in the mock library, ensuring that the signature matches the original.

3. **State Management**:
   - Implement internal state management for pins, counters, PWM, and other device settings to simulate real device behavior.

4. **Testing**:
   - Write unit tests that validate the mock by simulating real-world scenarios and comparing the mock behavior with expected outcomes.

5. **Documentation**:
   - Provide clear documentation on how to swap between the real and mocked `pokeyslib` for testing and production.

### Conclusion

This **mocked `pokeyslib`** will allow for full **unit testing** and **CI pipeline testing** without the need for physical devices. By maintaining the same interface as the real library, no changes to existing components will be necessary, ensuring seamless integration.

## Wiki

We have a comprehensive Wiki to help you get started with the project and find detailed documentation. The Wiki is structured and organized according to best practices, making it easy to navigate and find the information you need.

### Key Sections

- [Home Page](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Home): Overview of the repository, key features, and links to important pages.
- [Table of Contents](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Table_of_Contents): Navigate through different sections of the Wiki.
- [Getting Started](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Getting_Started): Guides for installation, setup, and usage.
- [User Documentation](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/User_Documentation): Detailed guides for how end-users can utilize the repository or software.
- [Developer Documentation](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Developer_Documentation): Information for contributors, such as coding standards, architecture details, API documentation, and contribution guidelines.
- [Advanced Topics](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Advanced_Topics): Cover advanced features, integrations, or use cases.
- [FAQ/Troubleshooting](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/FAQ_Troubleshooting): Frequently asked questions and common troubleshooting issues.
- [Release Notes](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Release_Notes): Document release notes and changelogs.
- [Feature Requests](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Feature_Requests): Maintain a list of feature requests and a roadmap for the project.
- [Versioning](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Versioning): Explain compatibility between different components (e.g., PoKeys, LinuxCNC).
- [Issues and Pull Requests](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Issues_Pull_Requests): Cross-link to relevant issues or pull requests.

### Contributing to the Wiki

We encourage contributions to the Wiki to keep it up to date and comprehensive. If you would like to contribute to the Wiki, please follow these guidelines:

1. **Fork the Repository**: Start by forking the repository to your GitHub account.
2. **Clone the Repository**: Clone the forked repository to your local machine.
   ```bash
   git clone https://github.com/your-username/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```
3. **Create a Branch**: Create a new branch for your Wiki contribution.
   ```bash
   git checkout -b wiki/your-wiki-contribution
   ```
4. **Make Changes**: Make your changes to the Wiki pages.
5. **Commit Changes**: Commit your changes with a descriptive commit message.
   ```bash
   git add .
   git commit -m "Update Wiki: your contribution description"
   ```
6. **Push Changes**: Push your changes to your forked repository.
   ```bash
   git push origin wiki/your-wiki-contribution
   ```
7. **Create a Pull Request**: Open a pull request from your forked repository to the main repository.

By following these steps, you can contribute to the Wiki and help improve the documentation for the project.

## PoKeys Device Compatibility

| PoKeys Device | Supported Versions | Notes |
|---------------|--------------------|-------|
| PoKeys57E     | 4.0, 4.1, 4.2      | Limited support |
| PoKeys57CNC   | 3.5, 3.6, 3.7      | Limited support |
| PoKeys57U     | 2.8, 2.9           | Limited support |
| PoKeys56U     | 1.5, 1.6           | Limited support |

## LinuxCNC Compatibility

The following table outlines the compatibility between different versions of LinuxCNC and the LinuxCnc_PokeysLibComp project:

| LinuxCNC Version | Supported | Notes |
|------------------|-----------|-------|
| 2.8.x            | Yes       | Limited support |
| 2.9.x            | Yes       | Limited support |
| 2.7.x            | No        | Not supported |    nothing of them is Fully supported yet. this sis still in development no approval of support done!

## GitHub Discussions

We encourage the use of GitHub Discussions for questions, feature requests, and ideas before creating issues or pull requests. This helps keep the issue tracker focused on actionable items and fosters community engagement.

### Available Categories

We have created the following categories in GitHub Discussions to help organize conversations:

- **General Q&A**: For users to ask questions or seek clarification on how to use the repository.
- **Feature Requests**: For proposing new features or enhancements.
- **Ideas & Brainstorming**: For discussing ideas that are still in the conceptual stage.
- **Show and Tell**: For users to share how they are using the project in their own implementations.
- **Announcements**: For maintainers to share important updates, releases, or roadmaps.
- **Bug Reports**: For reporting bugs and discussing potential fixes.

### Guidelines for Starting a Discussion

When starting a discussion, please follow these guidelines:

1. **Choose the Appropriate Category**: Select the category that best fits your topic to help keep discussions organized.
2. **Provide a Clear Title**: Use a clear and descriptive title for your discussion.
3. **Include Relevant Details**: Provide as much relevant information as possible to help others understand your question, idea, or request.
4. **Be Respectful and Constructive**: Follow the project's Code of Conduct and engage in respectful and constructive conversations.

### When to Use Discussions Instead of Issues or Pull Requests

Use GitHub Discussions for:

- General questions or clarifications about the project.
- Proposing new features or enhancements before creating a feature request issue.
- Sharing ideas and brainstorming with the community.
- Showcasing how you are using the project in your own implementations.
- Reporting bugs and discussing potential fixes before creating a bug report issue.

By using GitHub Discussions effectively, we can build a stronger community around the project and make the repository more welcoming to both new and experienced users.

You can access the GitHub Discussions for this repository [here](https://github.com/zarfld/LinuxCnc_PokeysLibComp/discussions).

## Sponsorship

We appreciate your interest in supporting the LinuxCnc_PokeysLibComp project! Your sponsorship helps us continue to develop and maintain this project. Here are some ways you can sponsor us:

- **GitHub Sponsors**: You can sponsor us directly through GitHub Sponsors. Visit our [GitHub Sponsors page](https://github.com/sponsors/zarfld) to learn more and become a sponsor.
- **PayPal**: Make a one-time donation via PayPal. Visit our [PayPal donation page](https://www.paypal.com/donate/?business=YJFUYJK44JFX6&no_recurring=0&item_name=Supporting+the+LinuxCnc_PokeysLibComp+project%21+Your+sponsorship+helps+us+continue+to+develop+and+maintain+this+project.&currency_code=EUR) to contribute.

For more detailed information on sponsorship tiers and benefits, as well as instructions on how to sponsor through GitHub Sponsors and PayPal, please refer to our [Sponsorship Page](.github/SPONSORS.md).

Thank you for your support!

## Setting Up Two-Factor Authentication (2FA) for GitHub Repositories

To set up two-factor authentication (2FA) for GitHub repositories, follow these steps:

### Enable 2FA for Your GitHub Account

1. Go to your GitHub account settings.
2. Navigate to the "Security" section.
3. Click on "Enable two-factor authentication".
4. Follow the prompts to set up 2FA using either a mobile app or SMS.

### Require 2FA for All Collaborators

1. Go to the repository's settings.
2. Navigate to the "Manage access" section.
3. Click on "Require two-factor authentication for all collaborators".
4. Confirm the changes.

### Verify 2FA Setup

1. Ensure that all collaborators have enabled 2FA on their GitHub accounts.
2. Check the repository's access settings to confirm that 2FA is required for all collaborators.

By following these steps, you can enhance the security of your GitHub repository by requiring two-factor authentication for all collaborators. For more detailed instructions, refer to the GitHub documentation on setting up 2FA.

## Enabling Dependabot Security Alerts and Automatic Updates

To enable Dependabot security alerts and automatic updates for your GitHub repository, follow these steps:

1. Go to the repository's settings.
2. Navigate to the "Security & analysis" section.
3. Enable "Dependabot alerts" and "Dependabot security updates".

Dependabot will now monitor your repository for security vulnerabilities and automatically create pull requests to update dependencies when vulnerabilities are found.

## Setting Up CodeQL for Code Scanning

To set up CodeQL for code scanning in your GitHub repository, follow these steps:

1. Go to the repository's settings.
2. Navigate to the "Security & analysis" section.
3. Enable "Code scanning" and select "CodeQL" as the analysis tool.
4. Configure the CodeQL workflow to run on push and pull request events.

CodeQL will now analyze your code for security vulnerabilities and provide detailed reports on potential issues.

## Enabling Secret Scanning

To enable secret scanning for your GitHub repository, follow these steps:

1. Go to the repository's settings.
2. Navigate to the "Security & analysis" section.
3. Enable "Secret scanning".

Secret scanning will now monitor your repository for exposed secrets, such as API keys and passwords, and alert you if any are found.

## Limiting Access Control and Enforcing Least Privilege

To limit access control and enforce the principle of least privilege for your GitHub repository, follow these steps:

1. Go to the repository's settings.
2. Navigate to the "Manage access" section.
3. Review the permissions of all collaborators and teams.
4. Assign the minimum necessary permissions to each collaborator and team based on their role and responsibilities.
5. Regularly review and update permissions to ensure they remain appropriate.

By limiting access control and enforcing least privilege, you can reduce the risk of unauthorized access and protect your repository from potential security threats.

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

## Documentation

For detailed instructions on converting existing LinuxCNC HAL and INI files for use with `pokeys.comp`, `pokeys_homecomp.comp`, and `kbd48cnc.comp`, refer to the [Converting HAL and INI Files](docs/wiki/Converting_HAL_and_INI_Files.md) documentation.
