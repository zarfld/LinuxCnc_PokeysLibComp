# Contributing to LinuxCnc_PokeysLibComp

Thank you for considering contributing to the LinuxCnc_PokeysLibComp project! We welcome contributions from the community to help improve the project. Please take a moment to review these guidelines before you start contributing.

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [How to Contribute](#how-to-contribute)
3. [Code Style](#code-style)
4. [Testing Requirements](#testing-requirements)
5. [Submitting Pull Requests](#submitting-pull-requests)
6. [Branch Naming Conventions](#branch-naming-conventions)
7. [License](#license)

## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). Please read it to understand the expectations for behavior when contributing to the project.

## How to Contribute

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

## Code Style

Please follow the code style guidelines for the project. Consistent code style helps maintain readability and makes it easier for others to understand your contributions.

- **Python**: Follow the PEP 8 style guide for Python code.
- **C/C++**: Follow the project's existing code style for C/C++ code.

## Testing Requirements

All contributions must include appropriate tests to ensure the reliability and correctness of the code. This includes:

- **Unit Tests**: Test individual functions and methods.
- **Integration Tests**: Test the interaction between different components.
- **Functional Tests**: Simulate real-world usage scenarios.
- **Performance Tests**: Ensure the code meets real-time operation requirements.

Run the tests using `pytest` and ensure that all tests pass before submitting your pull request.

## Submitting Pull Requests

When submitting a pull request, please ensure the following:

1. **Descriptive Title**: Use a descriptive title for your pull request.
2. **Detailed Description**: Provide a detailed description of the changes you made and the purpose of the contribution.
3. **Link to Issue**: If applicable, link to the issue that your pull request addresses.
4. **Review and Feedback**: Be responsive to feedback and make necessary changes based on the review comments.

## Branch Naming Conventions

To keep the repository organized, please follow these branch naming conventions:

- **Feature Branches**: `feature/{feature-name}`
- **Bug Fix Branches**: `bugfix/{bug-name}`
- **Improvement Branches**: `improvement/{improvement-name}`
- **Hotfix Branches**: `hotfix/{hotfix-name}`

## License

By contributing to this project, you agree that your contributions will be licensed under the project's [LICENSE](LICENSE).

## Ensuring Adherence to LinuxCNC Standards

To ensure that all development follows the official LinuxCNC guidelines, including compliance with the "Canonical Device Interface" as described in the HAL-Handbook, please adhere to the following:

1. **Review LinuxCNC Documentation**:
   - Review the official [LinuxCNC Development Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html).
   - Ensure that all development practices, naming conventions, and interfaces conform to the LinuxCNC standards.

2. **Follow LinuxCNC HAL and INI File Conventions**:
   - Align with the structure and format of HAL and INI files as outlined in the LinuxCNC [HAL Guidelines](https://linuxcnc.org/docs/html/hal/basic-hal.html) and [INI Guidelines](https://linuxcnc.org/docs/html/config/ini-config.html).
   - Ensure HAL and INI files for PoKeys components expose necessary parameters, pins, and signals using standardized naming.

3. **Implement Canonical Device Interface**:
   - Ensure that all components comply with the [Canonical Device Interface](https://linuxcnc.org/docs/html/hal/halmodule.html#_canonical_device_interfaces), including:
     - **Digital Inputs/Outputs**: For PoKeys digital I/O, ensure that pins are named following the canonical form (e.g., `pokeys.[DevID].digin.[PinID].in`, `pokeys.[DevID].digout.[PinID].out`).
     - **Analog Inputs/Outputs**: Analog I/O should follow the canonical pattern and include necessary parameters like scaling and offsets.
     - **Motion Control**: PoKeys motion control (PEv2) should map to standard motion control HAL pins such as `pos-cmd`, `vel-cmd`, `amp-enable-out`, etc., using canonical naming.
     - **Counters, PWM, and Other Peripherals**: Ensure each type of peripheral supported by PoKeys maps appropriately to the canonical device interface, following the standardized HAL pin and parameter conventions.

4. **Comply with Real-Time Constraints**:
   - For real-time components (e.g., `pokeys_rt`), ensure compliance with LinuxCNC real-time operation standards.
   - Verify that components avoid practices that would cause real-time jitter or violations (e.g., dynamic memory allocation).

5. **Review and Align with Canonical Interface Definitions**:
   - Ensure that all HAL components, especially for digital/analog I/O, motion control (PEv2), and communication protocols (PoNET), follow the canonical interface definitions as outlined in LinuxCNC's [HAL Component Interface](https://linuxcnc.org/docs/html/hal/halmodule.html).
   - Map all PoKeys-specific functionality appropriately to these interfaces.

6. **Modular Design for Future Proofing**:
   - Follow LinuxCNC's modular design principles. Ensure that each component (`pokeys_rt`, `pokeys_py`) is modular, allowing for future expansions and updates without major refactoring.
   - Leverage abstraction for interfacing with hardware like PoKeys to maintain cleaner code and easier integration.

7. **Adhere to Code Style Guidelines**:
   - Align with LinuxCNC's coding style and practices, especially for C/C++ code in real-time components.
   - Maintain consistent formatting, indentation, and comment style across all files.
   - Review the [LinuxCNC Source Code Formatting Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html#_source_code_formatting) to ensure conformity.

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

## References

- [GitHub Pull Request Reviews](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/reviewing-changes-in-pull-requests)
- [Code Review Best Practices](https://medium.com/@techgirl1908/code-review-best-practices-22cb9f4a63b1)
- [Collaborating with GitHub](https://guides.github.com/introduction/collaborating/)
- [LinuxCNC Development Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html)
- [LinuxCNC HAL Guidelines](https://linuxcnc.org/docs/html/hal/basic-hal.html)
- [LinuxCNC INI Guidelines](https://linuxcnc.org/docs/html/config/ini-config.html)
- [Canonical Device Interface](https://linuxcnc.org/docs/html/hal/halmodule.html#_canonical_device_interfaces)
- [LinuxCNC Source Code Formatting Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html#_source_code_formatting)
