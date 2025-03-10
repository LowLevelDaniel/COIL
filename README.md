# COIL - Computer Oriented Intermediate Language

COIL (Computer Oriented Intermediate Language) is a fast, space-efficient, and extensible language designed to serve as an assembly language replacement for compilers and interpreters.

## Overview

COIL bridges the gap between high-level languages and assembly by providing a consistent abstraction layer that works across different architectures and processing units. It maintains the power and flexibility of assembly while adding important abstractions that make cross-platform development more manageable.

Key features of COIL include:
- **Cross-architectural compatibility**: Works across different Processing Unit architectures
- **Cross-processing unit compatibility**: Works across different processing units (CPU, GPU, DSP, NPU, etc.)
- **Assembly-like control flow**: Maintains familiar assembly concepts like labels, jumps, and branches
- **Value system abstraction**: Replaces direct register/stack management with abstract value categories
- **Scope-based lifetime management**: Manages variable lifetimes through scopes
- **Universal feature availability**: All features (64 bit integer, FPU, SIMD, atomics, crypto, etc.) are available on all targets
- **Custom object Format**: Optimized format for space efficient COIL code.
- **Custom optimized object format**: Optimized format for machine code

## Core Philosophy

COIL's core design philosophy includes several key principles:

1. **Uniform Feature Availability**: Every COIL opcode will be able to be implemented on ever COIL target even if there isn't a direct native opcode translation.

2. **Computer-Oriented Design**: COIL is explicitly designed to be generated and consumed by compilers and interpreters, not humans. This focus allows it to prioritize consistency and machine-friendliness.

## Project Status

COIL is currently in early development. This repository contains the formatting header files and documentation for the project.
There is also the starts of a COIL compiler that will be separated later on

## Documentation

For detailed information about COIL, please refer to the following documentation:

- [Design Rationale](docs/design.md): Explains the design decisions behind COIL

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
[License](./LICENSE.md)