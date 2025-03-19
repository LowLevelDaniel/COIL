# Computer Oriented Intermediate Language (COIL)

<div align="center">
  <strong>An optimal intermediate representation sitting just above native binaries</strong>
</div>
<div align="center">
  A specification for a binary language format combining low-level control with architecture independence
</div>

<br />

<div align="center">
  <!-- Status -->
  <a href="#current-status">
    <img src="https://img.shields.io/badge/status-1.0.0-green" alt="Status" />
  </a>
  <!-- License -->
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/license-Unlicense-blue" alt="Unlicense" />
  </a>
</div>

<div align="center">
  <sub>Created by the Low Level Team (LLT)</sub>
</div>

## Table of Contents
- [Overview](#overview)
- [Core Benefits](#core-benefits)
- [Project Structure](#project-structure)
- [Key Concepts](#key-concepts)
- [Current Status](#current-status)
- [Getting Started](#getting-started)
- [Contributing](#contributing)
- [About the Low Level Team](#about-the-low-level-team)

## Overview

**IMPORTANT: THIS REPOSITORY CONTAINS ONLY THE SPECIFICATION DOCUMENTATION.** This is not an implementation of COIL. The repository contains purely markdown documentation describing the COIL standard. It does not include any implementation code, test suites, or development tools.

COIL is a binary language format designed to serve as an optimal intermediate representation between high-level languages and native machine code. Unlike traditional bytecode formats that require runtime interpretation, COIL is designed for direct translation to native code while preserving architecture independence and optimization opportunities.

COIL aims to provide a unified target for language implementers while giving low-level system programmers the control they need without forcing them to commit to a specific architecture.

## Core Benefits

- **Direct Mapping to Native Code**: COIL instructions have clear, predictable mappings to native instructions while remaining architecture-independent
- **Minimal Abstraction Overhead**: Designed to introduce virtually zero overhead compared to hand-written assembly
- **Rich Type System**: Supports both simple fixed-width types and complex composite types while maintaining low-level control
- **Explicit Optimization Opportunities**: Provides clear signals to optimizers for architecture-specific optimizations
- **No Runtime Dependencies**: COIL binaries can be compiled to native code without requiring runtime support libraries
- **Architecture Independence**: Write once, compile for any supported target architecture
- **Extensibility**: Designed to evolve with new processor architectures and instruction sets

## Project Structure

- `/coil/` - Core specifications and documentation
  - `OVERVIEW.md` - High-level conceptual overview
  - `isa.md` - Instruction set architecture specification
  - `type.md` - Type system specification
  - `reg.md` - Virtual register system specification
  - `dir.md` - Assembler directives documentation
  - `obj.md` - Object format specifications (COF and NCOF)
  - `conf.md` - Configuration format specification
  - `abi.md` - Application Binary Interface specification
  - `std.md` - Standard library documentation (future versions)
- `VERSION.md` - Versioning scheme and compatibility guarantees
- `CONTRIBUTING.md` - Contribution guidelines
- `GLOSSARY.md` - Terminology reference
- `CEL-GRAMMAR.md` - COIL Explanatory Language specification
- `ROADMAP.md` - Development trajectory and milestones
- `LICENSE` - Project license (Unlicense)

## Key Concepts

### Levels of Abstraction

COIL defines several abstraction levels to accommodate different use cases:

1. **RAW (Level 0)**: No dependencies, suitable for bare metal environments

Future versions will define additional abstraction levels:
2. **SSTD (Level 1)**: Simple Standard library (planned for v2.0.0)
3. **ESTD (Level 2)**: Extended Standard library for OS environments (planned for v2.0.0)
4. **GSTD (Level 3)**: Graphics Standard library for display systems (planned for v2.0.0)

### Object Formats

COIL defines two primary object formats:

- **COIL Object Format (COF)**: Output from compilers targeting COIL
- **Native COIL Object Format (NCOF)**: Output from COIL processors for linkers

### COIL Explanatory Language (CEL)

CEL is a human-readable representation of COIL binary code, similar to assembly language but with COIL-specific features. It provides a convenient way to document and understand COIL code in the specification. Note that CEL is not a programming language that gets compiled, but a documentation tool used for explaining the COIL binary format.

## Current Status

COIL is now at version 1.0.0, representing the first stable release with backward compatibility guarantees. This version includes:

- Complete specification of the core instruction set
- Finalized binary formats (COF and NCOF)
- Virtual register system for x86-64 and ARM architectures
- Primitive type system (integers, floating-point)
- ABI integration for common platforms
- Consistent instruction encoding format

**Important note:** Version 1.0.0 intentionally focuses on the core specification only. More advanced features such as composite types, arrays, and standard libraries are planned for future versions (see ROADMAP.md).

## Getting Started

As this repository contains only specification documentation, the best way to learn about COIL is to read the documentation in the following order:

1. Start with the [Overview](./coil/OVERVIEW.md) to understand COIL's design philosophy
2. Explore the [Instruction Set Architecture](./coil/isa.md) and [Type System](./coil/type.md)
3. Learn about the [COIL Explanatory Language](./CEL-GRAMMAR.md) for understanding COIL examples
4. Review the [Object Format](./coil/obj.md) to understand how COIL is represented in binaries
5. Check the [Virtual Register System](./coil/reg.md) to understand cross-platform register access

If you're interested in implementing COIL tools, the specification provides all the details needed to create:
- Compilers that generate COIL code from high-level languages
- COIL processors that translate COIL to native code
- Linkers that handle NCOF files
- Binary utilities for working with COIL files

## Contributing

We welcome contributions to the COIL specification! Please see our [Contributing Guidelines](./CONTRIBUTING.md) for more information on how to get involved.

The best ways to contribute to the documentation are:
- Reviewing and providing feedback on specifications
- Identifying gaps or inconsistencies in documentation
- Proposing enhancements to the language design
- Improving clarity and completeness of documentation

Note that contributions should focus on improving the specification documentation. Implementation code, tools, or test suites should be developed as separate projects.

## About the Low Level Team

The Low Level Team (LLT) is a collaborative group of open source developers focused on creating new foundations for computing technology. Starting with the COIL language specification, LLT aims to develop innovative approaches to systems programming, operating systems, browsers, graphics libraries, and game engines.

LLT values technical merit over recognition, with the goal of establishing open source control over future computing paradigms. Most team members maintain anonymity to keep focus on the technical contributions rather than individual recognition.

For more information, visit the LLT GitHub organization.