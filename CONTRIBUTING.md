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

Thank you for your contributions! We appreciate your help in making LinuxCnc_PokeysLibComp better for everyone.
