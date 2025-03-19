# COIL Terminology Glossary (Version 1.0.0)

This glossary clarifies key terminology used in the COIL ecosystem to ensure consistent understanding among contributors and implementers.

**IMPORTANT NOTE:** This glossary is part of the COIL specification documentation. It describes concepts that would be implemented in actual COIL tools, but this repository contains only documentation, not implementation code.

## Core Concepts and Components

### COIL (Computer Oriented Intermediate Language)
An intermediate representation designed to sit between high-level programming languages and native machine code. COIL provides architecture independence while maintaining low-level control comparable to assembly language. It is a binary format processed by computers, not humans.

### COIL Processor
A program that translates binary COIL Object Format (COF) into Native COIL Object Format (NCOF). The COIL processor performs optimizations, register allocation, and architecture-specific code generation. It does not read assembly language text; it processes binary COIL code and generates native code for specific target architectures.

### CEL (COIL Explanatory Language)
A human-readable text representation of COIL binary code, similar to assembly language. CEL is primarily used for documentation, examples, and explanations in the specification. It is not a programming language but a representation format to help humans understand COIL code.

### COF (COIL Object Format)
The binary format output by compilers targeting COIL. COF files contain COIL instructions, data, and metadata that are architecture-independent. These files serve as input to COIL processors.

### NCOF (Native COIL Object Format)
The binary format output by COIL processors. NCOF files contain architecture-specific native code that can be processed by platform-specific linkers. These may include specialized formats for different processing units (CPU, GPU, etc.).

### Linker (COIL-compatible)
A program that combines one or more NCOF files to produce an executable binary. The linker resolves symbols, performs address relocation, and integrates the code with the appropriate runtime environment.

## Abstraction Levels

### RAW (Level 0)
COIL code with no dependencies or library requirements. This level is designed for bare metal environments and embedded systems where size and direct hardware control are critical. This is the only level fully defined in version 1.0.0 of the specification.

### SSTD (Level 1 - Simple Standard)
COIL code using the Simple Standard library, which can be statically linked. Suitable for environments with minimal operating system support but where some standard functionality is needed. This level will be fully defined in version 2.0.0.

### ESTD (Level 2 - Extended Standard)
COIL code using the Extended Standard library, which typically requires dynamic linking and a full operating system environment. Provides comprehensive functionality for application development. This level will be fully defined in version 2.0.0.

### GSTD (Level 3 - Graphics Standard)
COIL code using the Graphics Standard library, which requires display capabilities and extensive operating system support. Designed for applications with graphical user interfaces. This level will be fully defined in version 2.0.0.

## File Formats and Extensions

### .coil / .cof
COIL Object Format file, output from compilers targeting COIL.

### .ncoil / .ncof
Native COIL Object Format file, output from COIL processors.

### .coilcfg
Binary COIL configuration file used by COIL processors.

### .coilcfg.txt
Text format of COIL configuration for human editing.

## Implementation Components

### Virtual Register System
A platform-independent abstraction of CPU registers that allows COIL code to reference registers in a consistent way across different architectures. The processor maps these virtual registers to physical registers or stack locations.

### Configuration Format
A standardized description of processing unit capabilities and characteristics, used by COIL processors to make optimization decisions and determine how to implement features that may not be directly supported by the target hardware.

### Directives
Special instructions for the COIL processor that control compilation, section creation, linkage, and other aspects of code generation that don't directly translate to machine instructions.

### ISA (Instruction Set Architecture)
The defined set of operations in COIL, including their binary encoding, operand format, and behavior. This is what makes up the actual COIL code.

### Type System
The defined data types in COIL, including their representation, conversion rules, and operations. The type system enables type checking and optimization by the COIL processor.

### ABI (Application Binary Interface)
The binary interface that defines how function calls operate, how parameters are passed, and how the system interacts with COIL code. ABIs in COIL are defined in a platform-independent way but map to platform-specific conventions.

## Version 1.0.0 Scope

Version 1.0.0 of the COIL specification covers:
- Core ISA and binary encoding
- Primitive type system
- Virtual register system
- Basic directives
- Object formats (COF and NCOF)
- Configuration format
- ABI specification

Features planned for future versions (like composite types, arrays, and standard libraries) are mentioned in the specification but will be fully defined in subsequent versions.

## Development Roles

### Compiler Developer
Creates compilers that translate high-level languages to COIL Object Format (COF).

### COIL Processor Developer
Creates the tools that translate COF to NCOF for specific target architectures.

### Linker Developer
Creates the tools that combine NCOF files into executable binaries.

### Library Developer
Creates standard libraries (SSTD, ESTD, GSTD) that can be used by COIL code (future versions).

### Specification Contributor
Contributes to the COIL specification documentation by suggesting improvements, identifying issues, or helping to define future features.