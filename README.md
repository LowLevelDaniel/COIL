# COIL: Compiler Optimization Intermediate Language

## Overview

COIL (Compiler Optimization Intermediate Language) is a binary intermediate representation designed to serve as a universal target for compilers across diverse processing units. COIL operates at the lowest level and can be used without a runtime system, making it suitable for applications like bootloaders and UEFI programming, while still providing capabilities for modern hardware.

## Core Design Principles

- **Binary First**: COIL is designed as a pure binary representation for maximum efficiency
- **Runtime Optional**: No runtime system required for core functionality
- **Universal Target Support**: Seamless targeting of CPUs, GPUs, NPUs, TPUs, FPGAs, ASICs, and more
- **Zero-Cost Abstraction**: Full hardware capability exposure without performance compromises
- **Type and Memory Safety**: Strong type checking and memory safety validation
- **Forward Compatibility**: Extensible design accommodating future hardware innovations
- **Deterministic Execution**: Predictable behavior across implementations
- **Hardware-Specific Optimization**: Direct access to specialized hardware features

## Documentation Structure

COIL documentation is organized into the following files:

### Core Components (No Runtime Required)
- **[object_format.md](object_format.md)**: Binary object file format specification
- **[instruction_set.md](instruction_set.md)**: Comprehensive instruction set documentation
- **[type_system.md](type_system.md)**: Type system specification
- **[memory_model.md](memory_model.md)**: Memory spaces, allocation, and management
- **[processing_units.md](processing_units.md)**: Processing unit classification and capabilities
- **[serialization_format.md](serialization_format.md)**: Binary serialization format specification

### Extended Components (Optional)
- **[execution_model.md](execution_model.md)**: Heterogeneous execution model (optional runtime)
- **[optimization.md](optimization.md)**: Optimization strategies
- **[config_format.md](config_format.md)**: Configuration file format for cross-compilation
- **[debugging.md](debugging.md)**: Debugging support specification
- **[implementation_guide.md](implementation_guide.md)**: Implementation guidelines
- **[runtime_extensions.md](runtime_extensions.md)**: Optional runtime extensions (multi-device support)

## Applications

COIL is designed for:

- **System Programming**: Operating systems, device drivers, firmware, bootloaders
- **High-Performance Computing**: Scientific simulations, real-time data processing
- **Graphics Processing**: Rendering pipelines, video encoding/decoding
- **AI/ML Workloads**: Training and inference across diverse AI accelerators
- **Embedded Systems**: Resource-constrained environments with specialized hardware
- **Heterogeneous Computing**: Applications spanning multiple processing unit types

## HOIL: Human-Oriented Intermediate Language

HOIL (Human-Oriented Intermediate Language) is a planned future extension that will provide a textual representation of COIL for human readability and manual authoring. The current focus remains on COIL as a binary format.

## Implementation Guidelines

### For Compiler Front-ends

When generating COIL from a source language:

1. Map source language types to appropriate COIL types
2. Choose appropriate execution domains for different code regions
3. Allocate variables to appropriate memory spaces
4. Provide rich optimization hints from source analysis
5. Identify code regions for different processing units

### For Compiler Back-ends

When translating COIL to target architecture:

1. Detect available hardware features
2. Map COIL operations to optimal hardware instructions
3. Map COIL memory spaces to hardware memory hierarchy
4. Efficiently allocate virtual registers to physical registers
5. Provide software fallbacks for unsupported features

## Getting Started

For compiler developers looking to implement COIL:

1. Start with the [object_format.md](object_format.md) to understand the binary structure
2. Review [instruction_set.md](instruction_set.md) for the core instruction encodings
3. Implement the [type_system.md](type_system.md) as the foundation
4. Add support for the [memory_model.md](memory_model.md)
5. For multi-device support, integrate the optional [runtime_extensions.md](runtime_extensions.md)