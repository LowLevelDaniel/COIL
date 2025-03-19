# Contributing to the COIL Specification

Thank you for your interest in contributing to the Computer Oriented Intermediate Language (COIL) project. This document outlines the process and standards for contribution to ensure the specification's quality and consistency.

**IMPORTANT NOTE:** The COIL repository contains only specification documentation, not implementation code. Contributions should focus on improving the specification itself, not on providing implementations, test suites, or tools.

## Code of Conduct

We expect all contributors to:
  - Be respectful and considerate in all communications
  - Accept constructive criticism with grace
  - Focus on what is best for the community and the project
  - Show empathy towards other community members

## Documentation Standards

### Required Tools
  - Git for version control
  - A markdown editor for documentation

### Documentation Standards
  - Use clear, concise language
  - Include examples where appropriate
  - Follow the existing Markdown style
  - Keep technical documentation separate from usage guides
  - Specify version compatibility for all features
  - Always clarify that the specification is documentation-only and not an implementation

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
  - Follow the documentation style of the project
  - Make focused, logical commits with clear messages
  - Include examples when applicable

4. **Documentation**
  - Update relevant documentation
  - Add clarifying comments for complex parts
  - Ensure versioning information is correctly updated
  - Always maintain clarity about the documentation-only nature of the repository

5. **Submit a Pull Request**
  - Provide a clear, descriptive title
  - Reference related issues or discussions
  - Include a detailed description of changes and motivations
  - Complete the pull request template checklist

## Review Process

1. **Initial Review**
  - Checks for documentation formatting and basic correctness
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

## Types of Contributions

### Documentation Improvements
- Clarification of existing specification details
- Better examples to illustrate concepts
- Correction of errors or inconsistencies
- Improved organization or structure

### Specification Enhancements
- New features that extend the capabilities of COIL
- Refinements to existing features
- Performance optimization suggestions
- Cross-platform compatibility improvements

### Use Case Development
- Documentation of how COIL can be used in specific domains
- Examples of how COIL could address particular challenges
- Comparison with other intermediate representations

## Implementation Guidelines

While this repository does not contain implementation code, contributors can suggest implementation guidance that would be useful for those creating COIL tools:

- Architecture-specific mapping suggestions
- Optimization strategies
- Resource utilization considerations
- Platform compatibility notes

Such guidance should be clearly marked as advisory rather than normative specification.

## Getting Help

- Join our community discussions on the project's GitHub Discussions
- Ask questions in the relevant issue or PR
- Reach out to maintainers listed in MAINTAINERS.md

Thank you for contributing to the COIL specification. Your efforts help create a powerful, efficient intermediate language representation for the future of computing.