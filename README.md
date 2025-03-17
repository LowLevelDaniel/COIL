# COIL (Computer Oriented Intermediate Language)

## Overview

COIL is a low-level intermediate language designed to replace traditional assembly languages while remaining close to machine code. It provides a unified representation that enables cross-architecture development and sophisticated optimizations without sacrificing performance or direct hardware control.

## Design Philosophy

COIL embraces the following core principles:

1. **Minimal Abstraction**: COIL provides only the abstractions necessary to enable cross-platform compatibility and optimization, without hiding the underlying hardware.

2. **Performance-First**: Every feature in COIL is designed with performance as the primary consideration, enabling optimizations that would be difficult to achieve in either high-level languages or pure assembly.

3. **Programmer Flexibility**: COIL allows programmers to work at a level of abstraction appropriate for their task, dropping down to raw assembly when needed while benefiting from COIL's abstractions elsewhere.

4. **Cross-Architecture Compatibility**: Through its configuration system and target switching capability, COIL enables code to be written once and run on multiple architectures.

## Key Features

COIL differentiates itself from traditional assembly through several key abstractions:

### 1. Variable Management System

COIL implements a variable system with types, alleviating the programmer from the burden of manual register and stack management. This allows the COIL optimizer to make intelligent decisions about register allocation and stack usage based on variable lifetimes, access patterns, and the capabilities of the target architecture.

### 2. ABI and Function System

Functions in COIL are first-class citizens with well-defined Application Binary Interfaces (ABIs) as specified in the configuration file. This enables seamless interoperability between functions compiled from different source languages or for different target architectures. The ABI system handles parameter passing, stack setup, and return value management automatically.

### 3. Standard Library

COIL includes a standard library that provides a unified interface for interacting with operating systems and hardware. This abstraction layer enables COIL code to run on bare metal for embedded systems and bootloaders, while also supporting applications that run on top of operating systems.

### 4. Target System Switching

The configuration-driven target system allows switching the target architecture during assembly. This enables:
- Generation of heterogeneous code (e.g., CPU and GPU code in the same program)
- Creation of polyglot binaries that can run on multiple architectures
- Support for mode transitions (e.g., transitioning from 16-bit real mode to 32-bit protected mode in an x86 bootloader)

### 5. Virtual Register System

COIL provides a virtual register abstraction using common names (R1, F1, V1, etc.) that map to physical registers according to the target configuration. This simplifies cross-platform development while maintaining the performance benefits of register-based computation.

## COIL in the Toolchain

COIL occupies a unique position in the development toolchain:

```
High-Level Languages → COIL → Machine Code
```

COIL eliminates the need for target-specific assembly languages, providing a unified representation that can be directly translated to machine code for any supported architecture. It can be targeted by compiler frontends as an intermediate representation, or written directly by programmers who need low-level control with cross-platform capabilities.

## Object Formats

COIL utilizes two distinct object formats:

1. **COIL Object Format**: A rich intermediate format that preserves COIL's abstractions, enabling link-time optimization and cross-platform compatibility.

2. **Native Object Format**: A traditional object format (e.g., ELF, PE, Mach-O) containing native machine code, ready for final linking and execution.

The COIL toolchain includes translators to convert between these formats as appropriate for the target environment.

## Use Cases

COIL is particularly well-suited for:

- **Embedded systems programming**: Where direct hardware control is essential
- **Operating system development**: Including bootloaders and kernels
- **Cross-platform libraries**: That need to run efficiently on multiple architectures
- **High-performance computing**: Where squeezing every ounce of performance matters
- **Heterogeneous computing**: Where code needs to run on both CPUs and accelerators (GPUs, FPGAs, etc.)