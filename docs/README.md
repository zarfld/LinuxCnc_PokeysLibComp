# Documentation Directory

This directory contains all project-related documentation, including the main README, user guides, and API references.

## Contents

- `README.md`: High-level overview of the repository, with clear instructions on how to set up, run, and contribute to the project.
- `user_guide.md`: Detailed user guide for setting up and using the project.
- `api_reference.md`: API reference documentation for the project's components.
- `diagrams/`: Directory containing diagrams and visual aids for the project.

## High-Level Overview

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

## Using the `create_images.sh` Script

The `create_images.sh` script automates the build and image creation process for Raspberry Pi 4 Bullseye, Bookworm, and amd64 hybrid. Follow these steps to use the script:

1. Ensure you have the necessary dependencies installed for cross-compiling for ARM and natively compiling for amd64.

2. Run the `create_images.sh` script:

```bash
chmod +x create_images.sh
./create_images.sh
```

The script will build and create images for Raspberry Pi 4 Bullseye, Bookworm, and amd64 hybrid, following the structure and format of existing LinuxCNC releases. It will also test and validate the generated images.

## Testing and Validating Generated Images

After running the `create_images.sh` script, it is important to test and validate the generated images to ensure they boot correctly and that LinuxCNC runs as expected on each platform. Follow these steps to test and validate the images:

1. **Boot the Image**: Write the generated image to an SD card (for Raspberry Pi) or a USB drive (for amd64 hybrid) and boot the device.

2. **Verify Boot**: Ensure that the device boots successfully and reaches the LinuxCNC interface.

3. **Run LinuxCNC**: Launch LinuxCNC and verify that it runs without errors.

4. **Check PoKeys Integration**: Perform basic configuration checks for PoKeys integration and ensure that the expected LinuxCNC configuration is applied.

5. **Report Issues**: If any issues are encountered during testing, report them in the repository's issue tracker with detailed information.

## Uploading Generated Images

The generated images should be uploaded to the repository's releases or a suitable cloud storage platform for distribution. Follow these steps to upload the images:

1. **Create a Release**: Create a new release in the GitHub repository.

2. **Upload Images**: Upload the generated images to the release.

3. **Provide Download Links**: Include download links for the images in the release notes or related documentation.

## Wiki Structure and Organization

The Wiki is structured and organized according to best practices, making it easy to navigate and find the information you need. The key sections of the Wiki include:

- **Home Page**: A comprehensive entry point with an overview of the repository, key features, and links to important pages.
- **Table of Contents**: Helps users easily navigate through different sections of the Wiki.
- **Modular Sections**: Organized content into sections like Getting Started, User Documentation, Developer Documentation, Advanced Topics, FAQ/Troubleshooting.

## Links to Wiki Pages

Here are some critical Wiki pages that you can refer to for detailed documentation:

- [Home Page](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Home)
- [Table of Contents](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Table_of_Contents)
- [Getting Started](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Getting_Started)
- [User Documentation](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/User_Documentation)
- [Developer Documentation](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Developer_Documentation)
- [Advanced Topics](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Advanced_Topics)
- [FAQ/Troubleshooting](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/FAQ_Troubleshooting)
- [Release Notes](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Release_Notes)
- [Feature Requests](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Feature_Requests)
- [Versioning](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Versioning)
- [Issues and Pull Requests](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Issues_Pull_Requests)

## Contributing to the Wiki

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
