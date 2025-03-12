# Introduction to COIL and HOIL

## COIL: Compiler Optimization Intermediate Language

COIL is a binary intermediate representation designed as a universal target for compilers across diverse processing units. It serves as a high-performance replacement for traditional assembly language, providing a consistent target that works across CPUs, GPUs, NPUs, TPUs, FPGAs, and other specialized hardware.

### Key Philosophy

- **Binary First**: COIL is designed as an efficient binary representation rather than a textual format
- **Zero Runtime Dependencies**: Core functionality operates without any runtime system
- **Universal Target Support**: Works consistently across diverse processing units
- **Bare-Metal Ready**: Functions in environments without operating systems
- **Clear Separation of Concerns**: Core functionality vs. optional runtime extensions

### Core vs. Runtime

COIL is organized into two distinct components:

#### Core COIL
- **Self-contained** with zero external dependencies
- **Bare-metal compatible** for low-level programming
- **Statically resolved** at compile/link time
- **Perfect for:** firmware, bootloaders, OS kernels, device drivers

#### Runtime Extensions
- **Optional** extensions for more complex environments
- **OS-dependent** features for multi-device coordination
- **Standard library** functionality similar to libc
- **Perfect for:** heterogeneous computing, high-performance applications

## HOIL: Human Oriented Intermediate Language

HOIL is the human-readable representation of COIL. While COIL is designed as a binary format for efficiency, HOIL provides a textual syntax that is easier for humans to read, write, and understand. HOIL can be directly translated to and from COIL binary representation.

### Relationship Between COIL and HOIL

```
+----------------+       +----------------+
|  Source Code   |       |  HOIL Code     |
| (C, C++, etc.) | ----> | (Human-readable|
+----------------+       |  COIL)         |
         |               +----------------+
         v                       |
+----------------+               |
|  Compiler      |               |
|  Frontends     |               |
+----------------+               |
         |                       |
         v                       v
+----------------+       +----------------+
|  COIL Binary   | <---> |  COIL Binary   |
| (Compiler      |       | (from HOIL     |
|  generated)    |       |  assembler)    |
+----------------+       +----------------+
         |                       |
         v                       v
+----------------+       +----------------+
|  COIL          |       |  Operating     |
|  Assemblers    | ----> |  System        |
| (Target-       |       |  (optional     |
|  specific)     |       |  runtime)      |
+----------------+       +----------------+
         |                       |
         v                       v
+----------------+       +----------------+
| Native Code    |       | Final          |
| for Target     | ----> | Execution      |
+----------------+       +----------------+
```

## Ecosystem Roles

### Compiler Developers
Compiler developers create frontends that translate source languages into COIL binary format. They focus on language semantics, optimizations, and generating correct COIL code, but don't need to worry about the details of target architectures.

### Assembler Creators
Assembler creators develop tools that translate COIL binary code into native code for specific target architectures. They focus on efficiently mapping COIL instructions to hardware capabilities.

### HOIL Programmers
Some developers may write code directly in HOIL for specialized use cases where direct control over the intermediate representation is beneficial.

## Target Applications

COIL is designed for:

- **System Programming:** Operating systems, device drivers, firmware, bootloaders
- **High-Performance Computing:** Scientific simulations, real-time data processing
- **Graphics Processing:** Rendering pipelines, video encoding/decoding
- **AI/ML Workloads:** Training and inference across diverse AI accelerators
- **Embedded Systems:** Resource-constrained environments with specialized hardware
- **Heterogeneous Computing:** Applications spanning multiple processing unit types