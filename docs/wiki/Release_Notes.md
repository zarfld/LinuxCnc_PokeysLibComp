# Release Notes

## Version 1.0.0

### New Features
- Initial release of the LinuxCnc_PokeysLibComp project.
- Integration of PoKeys devices with LinuxCNC for real-time control and monitoring.
- Support for connecting to USB and network devices.
- Reading and writing digital inputs and outputs.
- Reading analog inputs and writing analog outputs.
- Support for PoExtBus writing and reading.
- Setting and reading pin functions.
- PWM operations and pulse engine operations.
- Matrix keyboard setup and reading.
- kbd48CNC set LED and get Button Status.
- Using encoders for position tracking.

### Bug Fixes
- None in this initial release.

### Known Issues
- None reported.

## Version 1.1.0

### New Features
- Added support for PoKeys57CNC and PoStep25-32 (v1.3).
- Enhanced real-time control capabilities with improved performance.
- Added detailed user and developer documentation in the Wiki.
- Included advanced topics and usage examples in the Wiki.
- Added a comprehensive FAQ and troubleshooting section in the Wiki.

### Bug Fixes
- Fixed an issue with digital input reading on certain PoKeys devices.
- Resolved a bug causing incorrect analog input scaling.

### Known Issues
- None reported.

## Version 1.2.0

### New Features
- Introduced support for PoRelay8 – Relay extension board.
- Added architecture diagrams and design decisions documentation in the Wiki.
- Documented CI/CD pipelines and testing strategies in the Wiki.
- Included contribution guidelines and release notes in the Wiki.
- Added links to critical Wiki pages from the README file.

### Bug Fixes
- Fixed a bug causing intermittent connection issues with network devices.
- Resolved an issue with PWM operations on PoKeys57E devices.

### Known Issues
- None reported.

## Version 1.3.0

### New Features
- Added support for PoExtBusOC16CNC for Axes-Stepper Enable/Pulse/Dir.
- Enhanced API documentation with detailed method descriptions and examples.
- Included versioning documentation to explain compatibility between different components.
- Added cross-links to relevant issues and pull requests in the Wiki.

### Bug Fixes
- Fixed an issue with pulse engine operations on PoKeys57CNC devices.
- Resolved a bug causing incorrect encoder readings in certain scenarios.

### Known Issues
- None reported.

## Version 1.4.0

### New Features
- Introduced collaborative editing and periodic reviews of the Wiki content.
- Added a section on community and feature requests in the Wiki.
- Enhanced the Home Page and Table of Contents in the Wiki for better navigation.

### Bug Fixes
- Fixed a bug causing incorrect digital output states on PoKeys57E devices.
- Resolved an issue with matrix keyboard reading on PoNETkbd48CNC.

### Known Issues
- None reported.

## Version 1.5.0

### New Features
- Added a section that documents release notes and changelogs.
- Improved the structure and organization of the Wiki.
- Included clear installation instructions, configuration guides, and usage examples.
- Added architecture diagrams, design decisions, and documentation on CI/CD pipelines, testing strategies, and release workflows.
- Included contribution guidelines, release notes, and links to critical Wiki pages from the README file.
- Maintained a list of feature requests, a roadmap, and versioning documentation.
- Implemented cross-linking to relevant issues or pull requests.
- Encouraged collaborative editing and periodic reviews of the Wiki content.

### Bug Fixes
- Fixed various issues related to the organization and navigation of the Wiki.
- Resolved bugs in the documentation and examples provided in the Wiki.

### Known Issues
- None reported.

## Version 1.6.0

### New Features
- Updated documentation to reflect the current status and capabilities of the "Canonical Device Interface".
- Added detailed information and examples for the "Canonical Device Interface" in the `README.md` file.
- Included specific instructions and examples for the "Canonical Device Interface" in the `docs/README.md` file.
- Updated the compatibility section in the `docs/wiki/Compatibility.md` file to reflect the current status of the "Canonical Device Interface".
- Added detailed usage examples for the "Canonical Device Interface" in the `docs/wiki/User_Documentation.md` file.
- Included information about the homing functionality and its compatibility with `pokeys.comp` in the `docs/wiki/Developer_Documentation.md` file.
- Updated the homing configuration and linking pins in the `DM542_XXYZ_mill/pokeys_homing.md` file.
- Correctly connected the homing signals and linked pins from `pokeys_homecomp.comp` to PoKeys components in the `pokeys_homing.hal` file.
- Added information about the compatibility of the homing functionality with different PoKeys devices and LinuxCNC versions in the `docs/wiki/Compatibility.md` file.
- Included steps for setting up and using the "Canonical Device Interface" in the `docs/wiki/Getting_Started.md` file.
- Added common issues and solutions related to the "Canonical Device Interface" in the `docs/wiki/FAQ_Troubleshooting.md` file.

### Bug Fixes
- None reported.

### Known Issues
- None reported.
