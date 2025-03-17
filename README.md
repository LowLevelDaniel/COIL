# COIL - Computer Oriented Intermediate Language

## Overview

COIL (Computer Oriented Intermediate Language) is a cross-architecture intermediate language designed to serve as a versatile bridge between high-level programming languages and machine code. COIL aims to replace traditional assembly language by providing a consistent intermediate representation that can be efficiently translated to native code across different CPU architectures.

## Core Design Philosophy

COIL was designed with the following principles in mind:

1. **Universal Feature Implementation**: All COIL features are guaranteed to work on any supported architecture through native execution or emulation via instruction expansion.

2. **Bare Metal Capability**: COIL operates without dependencies at the bare metal level, with optional OS feature usage through its standard library.

3. **Assembly-Level Control with Abstractions**: COIL maintains the low-level control of assembly while introducing key abstractions like the variable system to simplify development.

4. **Efficient Binary Format**: COIL uses a compact binary format optimized for space efficiency and parsing speed.

5. **Familiar Branching Mechanisms**: COIL preserves assembly-like branching and comparison operations using flags and labels.

## Relationship with HOIL

HOIL (Human Oriented Intermediate Language) is the human-readable counterpart to COIL. HOIL provides a more readable syntax that compiles directly to COIL binary format, allowing developers to write code that closely resembles the underlying COIL structure while being more maintainable.

## Documentation Structure

This specification is organized into the following sections:

1. [Core Principles](core-principles.md) - Detailed explanation of COIL's fundamental design principles
2. [Architecture](architecture.md) - COIL's architectural design and execution model
3. [Instruction Set](instruction-set.md) - Comprehensive documentation of COIL instructions
4. [Binary Format](binary-format.md) - Specification of COIL's binary encoding
5. [HOIL Specification](hoil-specification.md) - Syntax and semantics of the human-readable language
6. [Standard Library](standard-library.md) - Documentation of COIL's standard library
7. [Variable System](variable-system.md) - Explanation of COIL's abstract variable management
8. [Implementation Guide](implementation-guide.md) - Guidelines for implementing COIL assemblers and compilers
9. [Object Format](object-format.md) - Specification of COIL's object file format
10. [Cross-Architecture Behavior](cross-architecture.md) - How COIL handles different CPU architectures

## Target Audience

This specification is intended for:

- Compiler developers implementing COIL as a target
- Assembler developers creating COIL assemblers
- Language designers interested in targeting COIL
- Systems programmers developing at the bare metal level
- Educators and students studying compiler design and low-level programming

## Version

This is version 1.0.0 of the COIL specification.