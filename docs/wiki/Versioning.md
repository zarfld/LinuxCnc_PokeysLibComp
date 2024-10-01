# Versioning

## Overview

This page provides information on versioning and compatibility between different components of the LinuxCnc_PokeysLibComp project, including PoKeys devices and LinuxCNC.

## Compatibility

### PoKeys Devices

The following table outlines the compatibility between different versions of PoKeys devices and the LinuxCnc_PokeysLibComp project:

| PoKeys Device | Supported Versions | Notes |
|---------------|--------------------|-------|
| PoKeys57E     | 4.0, 4.1, 4.2      | Fully supported |
| PoKeys57CNC   | 3.5, 3.6, 3.7      | Fully supported |
| PoKeys57U     | 2.8, 2.9           | Limited support |
| PoKeys56U     | 1.5, 1.6           | Limited support |

### LinuxCNC

The following table outlines the compatibility between different versions of LinuxCNC and the LinuxCnc_PokeysLibComp project:

| LinuxCNC Version | Supported | Notes |
|------------------|-----------|-------|
| 2.8.x            | Yes       | Fully supported |
| 2.9.x            | Yes       | Fully supported |
| 2.7.x            | No        | Not supported |

## Versioning Strategy

The LinuxCnc_PokeysLibComp project follows semantic versioning. The version number is in the format `MAJOR.MINOR.PATCH`, where:

- `MAJOR`: Incremented for incompatible API changes.
- `MINOR`: Incremented for new functionality in a backward-compatible manner.
- `PATCH`: Incremented for backward-compatible bug fixes.

## Release Notes

For detailed release notes and changelogs, please refer to the [Release Notes](Release_Notes.md) page.

## Older Versions

Older versions of the documentation are maintained for past releases. You can find the documentation for previous versions in the following links:

- [Version 1.0.0](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Version-1.0.0)
- [Version 1.1.0](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Version-1.1.0)
- [Version 1.2.0](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Version-1.2.0)
- [Version 1.3.0](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Version-1.3.0)
- [Version 1.4.0](https://github.com/zarfld/LinuxCnc_PokeysLibComp/wiki/Version-1.4.0)

## Updating Documentation

When a major update occurs, ensure that the documentation in the Wiki is updated to reflect the changes. If necessary, keep older versions of the documentation for past releases.

## Contribution

We welcome contributions to the documentation. If you would like to contribute, please refer to the [Contributing](../CONTRIBUTING.md) page for guidelines on how to contribute to the Wiki and the project.
