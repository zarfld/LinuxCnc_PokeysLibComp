# Code Review Guidelines

## Expectations for Code Reviews

Code reviews are an essential part of our development process. They help ensure code quality, maintainability, and foster collaboration within the team. The following guidelines outline the expectations for code reviews:

1. **Code Style**: Adhere to the project's coding standards and style guidelines. Consistent code style makes the codebase easier to read and maintain.
2. **Documentation**: Ensure that code is well-documented. This includes inline comments, function/method documentation, and any necessary external documentation.
3. **Testing**: Verify that the code includes appropriate tests. This includes unit tests, integration tests, and any other relevant tests.
4. **Performance**: Consider the performance implications of the code. Ensure that the code is efficient and does not introduce unnecessary performance bottlenecks.
5. **Security**: Review the code for potential security vulnerabilities. Ensure that the code follows best practices for security.

## Acceptable Pull Request (PR) Size and Complexity

To facilitate efficient code reviews, it is important to keep pull requests (PRs) manageable in size and complexity. The following guidelines outline what constitutes an acceptable PR size and complexity:

1. **PR Size**: Aim to keep PRs small and focused. A PR should ideally address a single issue or feature. Large PRs can be difficult to review and may introduce more bugs.
2. **PR Complexity**: Avoid introducing complex changes in a single PR. If a change is complex, consider breaking it down into smaller, more manageable PRs.

## Handling Conflicting Code or Suggestions

During code reviews, it is common to encounter conflicting code or suggestions. The following guidelines outline how to handle such situations:

1. **Conflicting Code**: If you encounter conflicting code, discuss the issue with the author and other relevant team members. Collaborate to find a solution that works for everyone.
2. **Suggestions**: Provide constructive feedback and suggestions for improvements. Be respectful and considerate in your feedback. Remember that the goal is to improve the code, not to criticize the author.

## Code Review Process

The following steps outline the code review process:

1. **Assign Reviewers**: Ensure that every PR has at least one assigned reviewer. Rotate reviewers to foster team-wide code familiarity.
2. **Automate Code Review Checks**: Integrate automated tools (e.g., linters, style checkers, and static analysis tools) into the CI/CD pipeline. Ensure code adheres to defined guidelines before requiring human review.
3. **Conduct Thorough Code Reviews**: Reviewers should focus on code correctness, adherence to coding standards and best practices, proper documentation, performance considerations, security implications, and proper test coverage.
4. **Collaborate on Changes**: Use inline comments in pull requests to suggest improvements. Encourage collaboration between the author and reviewer(s) to resolve issues.
5. **Set Clear Approval Criteria**: Define the minimum number of approvals required for a PR to be merged. Use GitHub’s protection rules to enforce these criteria.
6. **Facilitate Discussions and Knowledge Sharing**: Leverage the code review process to discuss architectural decisions, design patterns, or implementation strategies. Create a space for team members to share knowledge and learn from each other’s feedback.
7. **Track Review Progress**: Use GitHub’s tracking tools (e.g., issues, projects) to monitor the progress of code reviews. Ensure PRs are not left in review for too long without feedback or action.
8. **Merge Strategy**: Once all comments are resolved and the PR is approved, merge the changes following the team’s agreed-upon strategy (e.g., squash merge, rebase and merge).
9. **Post-Merge Collaboration**: After merging, encourage the team to continue monitoring the changes in the main branch for any issues or regressions. Encourage further discussion if the review process brought up important architectural or design questions.

## Branch Naming Conventions

To keep the repository organized, please follow these branch naming conventions:

- **Feature Branches**: `feature/{feature-name}`
- **Bug Fix Branches**: `bugfix/{bug-name}`
- **Improvement Branches**: `improvement/{improvement-name}`
- **Hotfix Branches**: `hotfix/{hotfix-name}`

## Creating Separate Branches for Each Feature or Bug Fix

Each new feature or bug fix should have its own branch created from the latest version of the main branch. The branch should focus on one feature or issue to ensure isolated development.

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

## Using GitHub Discussions

We encourage the use of GitHub Discussions for architectural decisions, design patterns, or implementation strategies. This helps keep the issue tracker focused on actionable items and fosters community engagement.

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

## References

- [GitHub Pull Request Reviews](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/reviewing-changes-in-pull-requests)
- [Code Review Best Practices](https://medium.com/@techgirl1908/code-review-best-practices-22cb9f4a63b1)
- [Collaborating with GitHub](https://guides.github.com/introduction/collaborating/)
- [LinuxCNC Development Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html)
- [LinuxCNC HAL Guidelines](https://linuxcnc.org/docs/html/hal/basic-hal.html)
- [LinuxCNC INI Guidelines](https://linuxcnc.org/docs/html/config/ini-config.html)
- [Canonical Device Interface](https://linuxcnc.org/docs/html/hal/halmodule.html#_canonical_device_interfaces)
- [LinuxCNC Source Code Formatting Guidelines](https://linuxcnc.org/docs/devel/html/code/code-notes.html#_source_code_formatting)
