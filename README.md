# COIL (Computer Oriented Intermediate Language)

## Overview

COIL is a binary intermediate language designed to bridge the gap between assembly languages and machine code. It provides a unified representation that enables cross-architecture development and sophisticated optimizations without sacrificing low-level control. 

Unlike text-based intermediate representations such as LLVM IR, COIL is a binary format designed for direct consumption by toolchains with minimal parsing overhead. It maintains the explicit hardware control of assembly languages while introducing minimal abstractions that enable powerful optimizations and cross-platform capabilities.

## Design Philosophy

COIL embraces the following core principles:

1. **Binary-First**: COIL is a binary format first and foremost, optimized for processing efficiency rather than human readability.

2. **Minimal Abstraction**: COIL provides only the abstractions necessary to enable cross-platform compatibility and optimization, without hiding the underlying hardware.

3. **Performance-First**: Every feature in COIL is designed with performance as the primary consideration, enabling optimizations that would be difficult to achieve in either high-level languages or pure assembly.

4. **Direct Hardware Access**: COIL provides explicit mechanisms for accessing hardware-specific features while maintaining a consistent programming model.

5. **Cross-Architecture Capability**: Through its configuration system and target switching capability, COIL enables code to be written once and compiled for multiple architectures.

## Key Features

COIL differentiates itself from traditional assembly through several key abstractions:

### 1. Virtual Register System

COIL introduces a virtual register abstraction layer that maps to physical registers according to the target architecture. This system provides:

- Consistent register naming across different architectures
- Architecture-independent register allocation
- Explicit control over register usage when needed

Virtual registers are encoded as simple numeric identifiers in the binary format, with a mapping table that translates them to physical registers specific to each target architecture.

### 2. Variable Management System

COIL implements a lightweight variable system that tracks:

- Variable lifetimes
- Type information
- Memory layout
- Access patterns

This system allows the COIL toolchain to make intelligent decisions about register allocation, spilling, and memory organization while maintaining the performance characteristics of direct register and memory access.

### 3. ABI and Function System

COIL provides explicit support for function declarations, calls, and parameter passing through architecture-specific ABIs. This system ensures:

- Consistent function calling conventions
- Efficient parameter passing
- Interoperability with native code
- Support for cross-architecture function calls

### 4. Target Switching Mechanism

COIL's unique target switching capability allows a single binary to contain code for multiple architectures or processing units. This enables:

- Heterogeneous computing (CPU/GPU/specialized hardware)
- Polyglot binaries compatible with multiple architectures
- Mode transitions (e.g., x86 real mode to protected mode)
- Optimal code selection based on available hardware features

### 5. Configuration-Driven Architecture

COIL uses detailed configuration blocks to specify the capabilities and constraints of target architectures. This system:

- Documents available instruction sets and registers
- Defines memory models and alignment requirements
- Specifies ABI details for parameter passing and returns
- Enables precise targeting of hardware-specific features

## COIL in the Toolchain

COIL occupies a unique position in the development toolchain:

```
High-Level Languages → (COF) COIL → (NOF) Machine Code → Optional COIL Linker
```

COIL can be targeted by:
- Compiler backends as a replacement for assembly generation
- Assembly language compilers
- Hand-written assembly (through a COIL assembler)

## Binary Format

COIL defines two binary formats:

1. **COIL Object Format (COF)**: A rich intermediate format that preserves COIL's abstractions, enabling cross-platform optimization.

2. **Native Object Format (NOF)**: A specialized object format containing native machine code for multiple target architectures, designed for final linking and execution.

## Implementation Requirements

COIL assemblers and linkers must adhere to the "all feature implemented policy," meaning:

- All operations defined in the COIL instruction set must be supported
- Operations not directly supported by hardware must be emulated through inline expansion
- Only operations that require operating system support (e.g., certain atomic operations) may be deferred to runtime libraries

## Documentation Structure

- [Instruction Set Architecture](isa.md) - Core instruction set and encoding
- [Configuration System](conf.md) - Target architecture specification
- [Register System](reg.md) - Virtual register mapping
- [Variable System](vars.md) - Variable management and lifetime tracking
- [ABI System](abi.md) - Function calling conventions and interfaces
- [Object Formats](objects.md) - Binary format specifications
- [Target Switching](target.md) - Cross-architecture execution
- [ISA Extensions](isaext.md) - Instruction Set Extensions utilizing an operating system environment

## Use Cases

COIL is particularly well-suited for:

- **Embedded systems programming**: Where direct hardware control is essential
- **Operating system development**: Including bootloaders and kernels
- **Cross-platform libraries**: That need to run efficiently on multiple architectures
- **High-performance computing**: Where squeezing every ounce of performance matters
- **Heterogeneous computing**: Where code needs to run on both CPUs and accelerators