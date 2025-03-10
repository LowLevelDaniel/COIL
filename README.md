# COIL - Computer Oriented Intermediate Language

COIL (Computer Oriented Intermediate Language) is a fast, space-efficient, and extensible language designed to serve as an assembly language replacement for compilers and interpreters.

## Overview

COIL bridges the gap between high-level languages and assembly by providing a consistent abstraction layer that works across different architectures and processing units. It maintains the power and flexibility of assembly while adding important abstractions that make cross-platform development more manageable.

Key features of COIL include:
- **Cross-architectural compatibility**: Works across different CPU architectures (x86, ARM, RISC-V, etc.)
- **Cross-processing unit compatibility**: Works across different processing units (CPU, GPU, DSP, NPU, etc.)
- **Assembly-like control flow**: Maintains familiar assembly concepts like labels, jumps, and branches (utilizing cmp and jg etc...)
- **Value system abstraction**: Replaces direct register/stack management with abstract value categories
- **Scope-based lifetime management**: Manages variable lifetimes through scopes
- **Universal feature availability**: All features (64 bit integer, FPU, SIMD, atomics, crypto, etc.) are available on all targets
- **Custom object format**: Optimized format for COIL code that is converted to native formats via UnifyLink

## Core Philosophy

COIL's core design philosophy includes several key principles:

1. **Uniform Feature Availability**: All COIL features (including SIMD, atomics, crypto operations, etc.) are native parts of the language and available on all targets. These are not "extensions" but fundamental COIL capabilities.

2. **Implementation Transparency**: The fact that some operations might be executed natively on certain hardware while being emulated on others is completely transparent to users of COIL. The same COIL code works consistently across all targets.

3. **Computer-Oriented Design**: COIL is explicitly designed to be generated and consumed by compilers and interpreters, not humans. This focus allows it to prioritize consistency and machine-friendliness.

## Project Status

COIL is currently in early development. This repository contains the foundational header files and documentation for the project.

## Documentation

For detailed information about COIL, please refer to the following documentation:

- [Design Rationale](docs/design.md): Explains the design decisions behind COIL
- [Value System](docs/value_system.md): Details the COIL value system
- [Implementation Guide](docs/implementation_guide.md): Guide for implementing COIL
- [Object Format](docs/object_format.md): Specification of the COIL object format
- [Build System](docs/build_system.md): Guide for using the meson build system with COIL

## Building

COIL uses the meson build system. To build the project:

```bash
# Set up the build directory
meson setup build

# Compile the project
meson compile -C build

# Run tests
meson test -C build
```

## License

This project is licensed under [LICENSE PLACEHOLDER] - see the LICENSE file for details.