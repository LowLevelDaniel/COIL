# COIL: Compiler Optimization Intermediate Language

COIL is a binary intermediate representation designed as a universal target for compilers across diverse processing units. It serves as a high-performance replacement for traditional assembly language.

## Key Features

- **Binary First:** Designed as an efficient binary representation
- **Zero Runtime Dependencies:** Core functionality works without any runtime system
- **Universal Target Support:** Works across CPUs, GPUs, NPUs, TPUs, FPGAs, and more
- **Bare-Metal Ready:** Perfect for bootloaders, firmware, and UEFI applications
- **Cross-Platform:** Consistent behavior across all supported platforms
- **Future-Proof:** Extensible design for emerging hardware

## Core vs Runtime

COIL is organized into two distinct components:

### Core COIL
- **Self-contained** with zero external dependencies
- **Bare-metal compatible** for low-level programming
- **Statically resolved** at compile/link time
- **Perfect for:** firmware, bootloaders, OS kernels, device drivers

### Runtime Extensions
- **Optional** extensions for more complex environments
- **OS-dependent** features for multi-device coordination
- **Standard library** functionality similar to libc
- **Perfect for:** heterogeneous computing, high-performance applications

## Documentation Structure

- [`core/`](core/README.md): Core features (no dependencies)
- [`runtime/`](runtime/README.md): Runtime extensions (OS dependent)
- [`examples/`](examples/README.md): Example applications

## Target Applications

COIL is designed for:

- **System Programming:** Operating systems, device drivers, firmware, bootloaders
- **High-Performance Computing:** Scientific simulations, real-time data processing
- **Graphics Processing:** Rendering pipelines, video encoding/decoding
- **AI/ML Workloads:** Training and inference across diverse AI accelerators
- **Embedded Systems:** Resource-constrained environments with specialized hardware
- **Heterogeneous Computing:** Applications spanning multiple processing unit types

## Getting Started

See the [Core Features Overview](core/README.md) to understand COIL's basic capabilities.

For using COIL in more complex environments with operating system support, see the [Runtime Extensions](runtime/README.md).