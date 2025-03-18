# Contributing to COIL

Thank you for your interest in contributing to the Computer Oriented Intermediate Language (COIL) project. This document outlines the process and standards for contribution to ensure the specification's quality and consistency.

## Code of Conduct

We expect all contributors to:
  - Be respectful and considerate in all communications
  - Accept constructive criticism with grace
  - Focus on what is best for the community and the project
  - Show empathy towards other community members

## Development Environment

### Required Tools
  - Git for version control
  - A markdown editor for documentation
  - For reference implementation contributions:
  - C++17 compatible compiler (preferably GCC 9+ or Clang 10+)
  - CMake 3.15+
  - Python 3.6+ (for test scripts)

### Documentation Standards
  - Use clear, concise language
  - Include examples where appropriate
  - Follow the existing Markdown style
  - Keep technical documentation separate from usage guides
  - Specify version compatibility for all features

## Specification Change Process

Changes to the COIL specification follow a structured process:

1. **Proposal**: Submit a detailed specification change proposal
  - Use the `PROPOSAL-template.md` in the `/proposals` directory
  - Include motivation, detailed description, and examples
  - Identify potentially affected components

2. **Discussion**: Allow at least 2 weeks for community discussion
  - Address feedback and concerns
  - Update the proposal as needed

3. **Review**: Core team reviews the final proposal
  - Evaluate technical merit
  - Check version compatibility
  - Assess implementation difficulty

4. **Acceptance**: If approved, proposal moves to implementation phase
  - Assignment of version number per VERSION.md rules
  - Update to all affected documentation

## Contribution Process

1. **Fork the Repository**
  - Create a personal fork of the repository on GitHub
  - Keep your fork synchronized with the main repository

2. **Create a Feature Branch**
  - Create a branch with a descriptive name:
    - For features: `feature/brief-description`
    - For fixes: `fix/brief-description`
    - For docs: `docs/brief-description`

3. **Make Your Changes**
  - Follow the coding/documentation style of the project
  - Make focused, logical commits with clear messages
  - Include tests or examples when applicable

4. **Documentation**
  - Update relevant documentation
  - Add clarifying comments for complex parts
  - Ensure versioning information is correctly updated

5. **Submit a Pull Request**
  - Provide a clear, descriptive title
  - Reference related issues or discussions
  - Include a detailed description of changes and motivations
  - Complete the pull request template checklist

## Review Process

1. **Initial Review**
  - Automated checks for formatting and basic correctness
  - Maintainer review for scope and approach

2. **Technical Review**
  - Deep analysis of technical correctness
  - Compatibility with existing features
  - Adherence to COIL design principles

3. **Documentation Review**
  - Clarity and completeness of documentation
  - Correctness of examples
  - Version information accuracy

4. **Final Approval**
  - At least two core team approvals required
  - Changes may be requested at any stage
  - Once approved, a maintainer will merge the PR

## Version Control

COIL uses semantic versioning as detailed in [VERSION.md](./VERSION.md):

- Include version updates in your PR when appropriate
- Document which components are affected
- Note any backward compatibility issues

## Implementation Contributions

For contributions to reference implementations:

1. **Code Style**
  - Follow the project's code style guide in `.style-guide.md`
  - Run linting tools before submission

2. **Testing**
  - Include unit tests for new functionality
  - Verify backward compatibility
  - Add to the integration test suite if necessary

3. **Performance**
  - Consider the performance impact of changes
  - Document performance characteristics of new features

## Getting Help

- Join our community discussions on the project's GitHub Discussions
- Ask questions in the relevant issue or PR
- Reach out to maintainers listed in MAINTAINERS.md

Thank you for contributing to COIL. Your efforts help create a powerful, efficient intermediate language representation for the future of computing.