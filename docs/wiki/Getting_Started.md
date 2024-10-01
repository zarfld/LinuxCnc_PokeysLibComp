# Getting Started

Welcome to the **LinuxCnc_PokeysLibComp** project! This guide will help you get started with the installation, setup, and usage of the project.

## Installation

To install the LinuxCnc_PokeysLibComp project, follow these steps:

1. **Clone the Repository**: Start by cloning the repository to your local machine.
   ```bash
   git clone https://github.com/zarfld/LinuxCnc_PokeysLibComp.git
   cd LinuxCnc_PokeysLibComp
   ```

2. **Run the Installation Script**: Execute the installation script to set up the project.
   ```bash
   sudo sh install.sh
   ```

## Setup

### Hardware Requirements

The project has been tested with the following hardware:

- RaspberryPi4 (for LinuxCNC)
- PoKeys57E
- PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir
- CNC keyboard – PoNETkbd48CNC
- PoRelay8 – Relay extension board
- PoExtBusOC16
- PoKeys57CNC
- PoStep25-32 (v1.3)

### Configuration

1. **Connect the Hardware**: Connect the PoKeys devices to your system according to the hardware requirements.
2. **Configure LinuxCNC**: Update the LinuxCNC configuration files to include the PoKeys components. Refer to the [User Documentation](User_Documentation.md) for detailed configuration steps.

## Usage

### Running LinuxCNC

1. **Launch LinuxCNC**: Start LinuxCNC from the terminal or desktop environment.
   ```bash
   linuxcnc
   ```

2. **Select Configuration**: Choose the appropriate configuration that includes the PoKeys components.

3. **Verify Connections**: Ensure that the PoKeys devices are properly connected and recognized by LinuxCNC.

### Example Usage

Refer to the [User Documentation](User_Documentation.md) for detailed usage examples and practical scenarios.

## Platform-Specific Notes

### Raspberry Pi

- **OS**: The project has been tested on Raspberry Pi OS (formerly Raspbian) with LinuxCNC.
- **Dependencies**: Ensure that all necessary dependencies are installed. Refer to the [Installation](#installation) section for details.

### Other Platforms

- **Compatibility**: The project is designed to be compatible with various Linux distributions. Ensure that the necessary dependencies are installed and configured.

## Dependencies

The project requires the following dependencies:

- LinuxCNC
- PoKeysLib
- Python 3.x
- Additional libraries and tools as specified in the [Installation](#installation) section.

## Troubleshooting

If you encounter any issues during the installation, setup, or usage of the project, refer to the [FAQ/Troubleshooting](FAQ_Troubleshooting.md) section for common problems and solutions.

## Contributing

We welcome contributions from the community to help improve the project. Please refer to the [Contributing](../CONTRIBUTING.md) page for guidelines on how to contribute.

Thank you for being a part of the **LinuxCnc_PokeysLibComp** community!
