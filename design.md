# COIL Design Rationale

This document explains the design decisions behind the Computer Oriented Intermediate Language (COIL).

## Motivation

Modern compilers typically generate architecture-specific assembly or machine code as their final output. This approach presents several challenges:

1. **Architectural dependencies**: Each target architecture requires a specialized backend
2. **Limited abstraction**: Assembly languages provide minimal abstraction, making optimization difficult
3. **Hardware feature fragmentation**: Different hardware supports different feature sets, complicating code generation
4. **Difficult cross-compilation**: Targeting multiple architectures simultaneously is complex

COIL addresses these challenges by providing a consistent intermediate representation that abstracts hardware-specific details while maintaining the power and flexibility of assembly.

## Core Design Principles

### 1. Maintain Assembly-Like Control Flow

COIL preserves the familiar control flow mechanisms of assembly:
- Labels and symbols for program addresses
- Direct jumps and branches
- Conditional execution

This approach maintains the power and flexibility that make assembly valuable while adding abstractions that make cross-platform development more manageable.

### 2. Abstract Register and Stack Management

One of the main challenges in cross-architectural code generation is dealing with different register sets and stack management conventions. COIL addresses this by introducing a value system that abstracts these details:

- **COIL Symbol**: An alias for a program memory address
- **COIL Immediate**: A direct value used without storage
- **COIL Variable**: An abstraction for either a register or stack position
- **COIL Operational**: A variable hinted to be stored in a register for frequent use
- **COIL State**: An abstraction for processor state (flags, etc.)

This abstraction allows COIL to target vastly different architectures without changing the intermediate representation.

### 3. Scope-Based Lifetime Management

COIL introduces explicit scopes to help with optimizations and variable lifetime management. Scopes provide clear boundaries for resource allocation and deallocation, making it easier to optimize register usage and stack management.

### 4. All Features Are Native

A fundamental principle of COIL is that all operations - including SIMD, atomics, cryptography, and privileged operations - are native COIL features. This means:

- COIL provides a consistent programming interface regardless of the underlying hardware
- Every COIL feature is available on all targets, even if some require emulation
- Programmers never need to check if a feature is available or write architecture-specific code

This approach completely abstracts away hardware feature fragmentation and enables true write-once-compile-anywhere code.

The implementation of these features may differ based on the target hardware:
- On hardware with native support, operations map directly to hardware instructions
- On hardware without native support, operations are emulated using available instructions

This implementation detail is entirely transparent to the programmer using COIL.

### 5. Computer-Oriented Design

COIL is explicitly designed to be generated and consumed by computers rather than humans. While it remains human-readable for debugging purposes, its primary audience is compiler and interpreter software. This focus allows COIL to prioritize consistency and machine-friendliness over human ergonomics.

## Target Implementation Strategy

COIL's implementation strategy for handling different target capabilities involves:

1. **Feature Map**: For each COIL operation, maintain a map of implementation functions
2. **Native Implementations**: When a target supports a feature natively, use the hardware instructions
3. **Emulated Implementations**: When a target does not support a feature, use the emulation implementation
4. **Runtime Selection**: The compiler chooses the appropriate implementation at compile time

This approach enables COIL to support all features on all targets while still taking advantage of hardware acceleration when available.

## Extensibility

COIL is designed to be extensible in several ways:

1. **New types**: The type system can be extended to support new data types
2. **New operations**: The instruction set can be extended to support new operations
3. **New targets**: New architectures and processing units can be added
4. **New implementation strategies**: Alternative implementations for operations can be added

This extensibility ensures that COIL can adapt to new hardware features and architectural innovations without changing its core design principles.