# COIL Architecture Overview

This document provides a high-level overview of the COIL architecture, showing how the various components interact and explaining the design principles that guided its development.

## Architectural Layers

COIL's architecture is designed with distinct layers that separate concerns:

```
+--------------------------------------------------+
|                                                  |
|                  Applications                    |
|                                                  |
+--------------------------------------------------+
                       |
+--------------------------------------------------+
|                                                  |
|               Source Languages                   |
|     (C, C++, Rust, Python, Domain-Specific)     |
|                                                  |
+--------------------------------------------------+
                       |
+--------------------------------------------------+
|                                                  |
|               Language Frontends                 |
|       (Compilers targeting COIL binary)         |
|                                                  |
+--------------------------------------------------+
                       |
+--------------------------------------------------+
|                                                  |
|                  COIL/HOIL IR                   |
|    (Core Binary Format or Human-Readable Text)   |
|                                                  |
+--------------------------------------------------+
                       |
+--------------------------------------------------+
|                                                  |
|               COIL Assemblers                    |
|    (Target-specific translators to native code)  |
|                                                  |
+--------------------------------------------------+
                       |
+--------------------------------------------------+
|                                                  |
|               Processing Units                   |
|   (CPUs, GPUs, NPUs, TPUs, FPGAs, Custom HW)    |
|                                                  |
+--------------------------------------------------+
```

## Key Architectural Components

### 1. Core Components (No Runtime Dependencies)

The core components operate without any runtime dependencies and are suitable for bare-metal environments:

- **Type System**: Comprehensive type system resolved at compile time
- **Memory Model**: Direct hardware memory access with static management
- **Instruction Set**: Binary encoding for operations with no runtime overhead
- **Processing Unit Model**: Abstract model of computational devices
- **Serialization Format**: Efficient binary encoding for COIL modules
- **Optimization Framework**: Compile-time optimization techniques

### 2. Optional Runtime Extensions

Runtime extensions provide additional capabilities for environments with operating system support:

- **Execution Model Extensions**: Multi-threading, device coordination
- **Device Management**: Discovery and selection of processing units
- **Memory Management**: Advanced memory operations across devices
- **Debugging Support**: Runtime debugging capabilities (when available)

### 3. Compilation Pipeline

```
Source Code → Parser → AST → IR Generation → Analysis → Transformation → COIL Binary → Assembler → Native Code
```

Each stage in this pipeline can be implemented independently:

1. **Frontend Compiler**: Translates source code to COIL binary
2. **COIL Assembler**: Translates COIL binary to native code

## Design Principles

### 1. Separation of Concerns

COIL maintains a strict separation between:

- **What**: The operations to be performed (defined by COIL)
- **How**: The implementation of these operations (handled by assemblers)
- **Where**: The target processing unit (configured for assemblers)

This separation allows compiler developers to focus on translating high-level constructs to COIL operations without worrying about the specific details of target architectures.

### 2. Binary-First Design

COIL is designed as a binary format first, with a human-readable representation (HOIL) as a secondary concern. This ensures:

- **Efficiency**: Compact representation with minimal overhead
- **Direct Translation**: Straightforward mapping to native code
- **Interoperability**: Consistent representation across tools

### 3. Zero Runtime Dependencies (Core)

Core COIL functionality operates without any runtime system, ensuring it can be used in:

- Bootloaders
- Firmware
- Operating system kernels
- Device drivers
- Embedded systems

### 4. Clear Target Specification

COIL provides mechanisms for specifying target requirements and capabilities:

- Target architecture identification
- Required features and extensions
- Memory model constraints
- Processing unit characteristics

Assemblers use this information to generate appropriate native code.

### 5. Extensibility

COIL's architecture is designed to be extended without breaking existing code:

- New instruction types can be added
- Additional processing unit types can be supported
- Extended features can be introduced gradually
- Optional runtime components are clearly separated

## Processing Unit Model

COIL's processing unit model abstracts the capabilities of diverse hardware:

- **Classification**: Categorizes processing units by type and capabilities
- **Feature Detection**: Identifies available hardware features
- **Memory Hierarchy**: Maps to hardware memory models
- **Compute Model**: Describes how code executes on the hardware

This abstraction allows COIL to target:

- Traditional CPUs
- Graphics Processing Units (GPUs)
- Neural Processing Units (NPUs)
- Tensor Processing Units (TPUs)
- Field-Programmable Gate Arrays (FPGAs)
- Digital Signal Processors (DSPs)
- Custom accelerators and emerging hardware

## Communication Flow

### Compiler to COIL Binary

1. Compiler analyzes source code
2. Performs language-specific optimizations
3. Maps language constructs to COIL operations
4. Encodes in COIL binary format with target requirements

### COIL Binary to Assembler

1. Assembler decodes COIL binary
2. Verifies compatibility with target
3. Maps COIL operations to native instructions
4. Applies target-specific optimizations
5. Generates native binary code

## Concurrency and Parallelism

COIL supports different models of concurrency and parallelism:

- **Core Support**: Basic vector operations and synchronization primitives
- **Runtime Extensions**: Advanced multi-device coordination and task scheduling

This layered approach ensures that basic parallel operations work in bare-metal environments, while more sophisticated approaches are available when a runtime system is present.