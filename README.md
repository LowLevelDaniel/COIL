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
  <a href="#roadmap">
    <img src="https://img.shields.io/badge/status-pre--alpha-orange" alt="Status" />
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
- [Versioning](#versioning)
- [Getting Started](#getting-started)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [Related Projects](#related-projects)
- [About the Low Level Team](#about-the-low-level-team)

## Overview

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
  - `std.md` - Standard library documentation
- `VERSION.md` - Versioning scheme and compatibility guarantees
- `CONTRIBUTING.md` - Contribution guidelines
- `CEL-GRAMMAR.md` - COIL Explanatory Language specification
- `LICENSE` - Project license (Unlicense)

## Key Concepts

### Levels of Abstraction

COIL defines several abstraction levels to accommodate different use cases:

1. **RAW (Level 0)**: No dependencies, suitable for bare metal environments
2. **SSTD (Level 1)**: Simple Standard library, statically linkable
3. **ESTD (Level 2)**: Extended Standard library for OS environments
4. **GSTD (Level 3)**: Graphics Standard library for display systems

### Object Formats

COIL defines two primary object formats:

- **COIL Object Format (COF)**: Output from compilers targeting COIL
- **Native COIL Object Format (NCOF)**: Output from COIL assemblers for linkers

### COIL Explanatory Language (CEL)

CEL is a human-readable representation of COIL binary code, similar to assembly language but with COIL-specific features. It provides a convenient way to write and understand COIL code before binary translation.

## Versioning

COIL follows a strict semantic versioning scheme defined in [VERSION.md](./VERSION.md):

- **MAJOR**: Breaking changes to binary format or core semantics
- **MINOR**: New features with backward compatibility
- **PATCH**: Bug fixes and specification clarifications

Pre-1.0.0 versions are considered unstable and may introduce breaking changes in minor versions.

## Getting Started

> Note: COIL is currently in pre-alpha stage. Implementation tools are under development.

### Learning COIL

1. Start with the [Overview](./coil/OVERVIEW.md) to understand COIL's design philosophy
2. Explore the [Instruction Set Architecture](./coil/isa.md) and [Type System](./coil/type.md)
3. Learn about the [COIL Explanatory Language](./CEL-GRAMMAR.md) for human-readable COIL

## Roadmap to 1.0.0

- [x] Initial specification
- [x] Core design principles documentation
- [ ] Complete ISA definition
- [ ] Finalized type system specification
- [ ] Basic standard library implementation
- [ ] Test suite
- [ ] Example compiler targeting COIL
- [ ] Performance benchmarks against native code

## Contributing

We welcome contributions to the COIL project! Please see our [Contributing Guidelines](./CONTRIBUTING.md) for more information on how to get involved.

The best ways to contribute currently are:
- Reviewing and providing feedback on specifications
- Identifying gaps or inconsistencies in documentation
- Proposing enhancements to the language design
- Working on reference implementations

## Related Projects

> Future complementary tools and libraries will be listed here as they are developed.

## About the Low Level Team

The Low Level Team (LLT) is a collaborative group of open source developers focused on creating new foundations for computing technology. Starting with the COIL language specification, LLT aims to develop innovative approaches to systems programming, operating systems, browsers, graphics libraries, and game engines.

LLT values technical merit over recognition, with the goal of establishing open source control over future computing paradigms. Most team members maintain anonymity to keep focus on the technical contributions rather than individual recognition.

For more information, visit the LLT GitHub organization.