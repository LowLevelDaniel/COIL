# COIL Binary Specification

## 1. Overview

The Compiler Optimization Intermediate Language (COIL) is a binary intermediate representation designed to serve as a universal target for compilers across diverse processing units. COIL provides a consistent, architecture-independent format that enables compilers to focus on language semantics and optimizations without needing detailed knowledge of target architectures.

COIL is designed with these core principles:
- **Binary-first design** for efficiency and direct hardware mapping
- **Zero runtime dependencies** for core functionality
- **Architecture independence** allowing portability across diverse hardware
- **Bare-metal compatibility** for system-level programming
- **Clear separation** between core functionality and optional extensions

## 2. Core Components

### 2.1 Type System

COIL's type system is designed to be comprehensive yet efficient, with all types resolved at compile time.

#### Type Encoding

All COIL types are encoded in a compact 32-bit representation:
```
[category:4][width:8][qualifiers:8][attributes:12]
```

#### Basic Types

| Type Category | Encoding | Description |
|---------------|----------|-------------|
| Void | 0x00000000 | Represents no value |
| Boolean | 0x01000001 | True/false value (1 bit) |
| Integer | 0x02WWQAAA | Signed or unsigned integers |
| Float | 0x03WWQAAA | Floating-point numbers |
| Pointer | 0x04WWQAAA | Memory address pointers |
| Vector | 0x05WWQAAA | SIMD vector types |
| Array | 0x06WWQAAA | Fixed-size arrays |
| Structure | 0x07WWQAAA | Composite data structures |
| Function | 0x08WWQAAA | Function types |

Where:
- WW = Width (8 bits) - Size in bits or elements
- Q = Qualifiers (8 bits) - signed/unsigned, const, volatile, etc.
- AAA = Attributes (12 bits) - Type-specific attributes

#### Type Qualifiers

| Qualifier | Value | Description |
|-----------|-------|-------------|
| UNSIGNED | 0x01 | Unsigned integer type |
| CONST | 0x02 | Constant value |
| VOLATILE | 0x04 | Volatile memory access |
| RESTRICT | 0x08 | Non-aliased pointer |
| ATOMIC | 0x10 | Atomic access |

#### Common Predefined Types

| Type Name | Encoding | Description |
|-----------|----------|-------------|
| VOID | 0x00000000 | Void type |
| BOOL | 0x01000001 | Boolean (1 bit) |
| INT8 | 0x02080000 | 8-bit signed integer |
| UINT8 | 0x02080100 | 8-bit unsigned integer |
| INT16 | 0x02100000 | 16-bit signed integer |
| UINT16 | 0x02100100 | 16-bit unsigned integer |
| INT32 | 0x02200000 | 32-bit signed integer |
| UINT32 | 0x02200100 | 32-bit unsigned integer |
| INT64 | 0x02400000 | 64-bit signed integer |
| UINT64 | 0x02400100 | 64-bit unsigned integer |
| FLOAT16 | 0x03100000 | 16-bit floating-point |
| FLOAT32 | 0x03200000 | 32-bit floating-point |
| FLOAT64 | 0x03400000 | 64-bit floating-point |
| PTR | 0x04400000 | Generic pointer |

### 2.2 Memory Model

COIL's memory model provides a consistent view of memory across diverse architectures.

#### Memory Spaces

| Space | Description |
|-------|-------------|
| Global | Process-wide memory |
| Local | Thread-local memory |
| Shared | Memory shared between work items |
| Constant | Read-only memory |
| Private | Function-local memory |

#### Memory Access Instructions

| Instruction | Description |
|-------------|-------------|
| LOAD | Load from memory |
| STORE | Store to memory |
| ATOMIC_OP | Atomic memory operation |
| FENCE | Memory barrier |
| LEA | Load effective address |

#### Memory Ordering

COIL supports various memory ordering models:
- **Relaxed**: No ordering constraints
- **Acquire**: Acquire ordering semantics
- **Release**: Release ordering semantics
- **AcqRel**: Acquire-release ordering semantics
- **SeqCst**: Sequentially consistent ordering

### 2.3 Instruction Set

COIL provides a comprehensive set of architecture-independent instructions.

#### Instruction Categories

| Category | Description |
|----------|-------------|
| Arithmetic | Basic arithmetic operations |
| Logical | Bitwise and logical operations |
| Comparison | Value comparison operations |
| Control Flow | Branches, jumps, and calls |
| Memory | Load, store, and memory management |
| Conversion | Type conversion operations |
| Vector | SIMD vector operations |
| Atomic | Atomic operations |
| Special | Architecture-specific operations |

#### Instruction Format

Each instruction in COIL binary is encoded as:
```
[opcode:8][flags:8][operand count:8][destination:8][operands...]
```

Where:
- opcode: Instruction type
- flags: Instruction-specific flags
- operand count: Number of source operands
- destination: Destination register/value
- operands: Source operands

#### Core Arithmetic Instructions

| Instruction | Opcode | Description |
|-------------|--------|-------------|
| ADD | 0x01 | Addition |
| SUB | 0x02 | Subtraction |
| MUL | 0x03 | Multiplication |
| DIV | 0x04 | Division |
| REM | 0x05 | Remainder |
| NEG | 0x06 | Negation |
| ABS | 0x07 | Absolute value |
| MIN | 0x08 | Minimum |
| MAX | 0x09 | Maximum |
| FMA | 0x0A | Fused multiply-add |

### 2.4 Control Flow

COIL uses a block-based control flow model with explicit branches.

#### Basic Blocks

A basic block is a sequence of instructions with:
- One entry point (no branches to the middle)
- One exit point (at the end)
- No branches except at the end

#### Branch Instructions

| Instruction | Description |
|-------------|-------------|
| BR | Unconditional branch |
| BR_COND | Conditional branch |
| SWITCH | Multi-way branch |
| CALL | Function call |
| RET | Function return |

#### Example Flow Control

```
ENTRY:
    cond = CMP_LT a, b;
    BR cond, THEN, ELSE;

THEN:
    result = ADD a, 1;
    BR ALWAYS, END;

ELSE:
    result = SUB b, 1;
    BR ALWAYS, END;

END:
    RET result;
```

### 2.5 Processing Unit Model

COIL abstracts the capabilities of diverse hardware through a processing unit model.

#### Device Classes

| Class | Description |
|-------|-------------|
| CPU | General-purpose central processing unit |
| GPU | Graphics processing unit |
| NPU | Neural processing unit |
| TPU | Tensor processing unit |
| DSP | Digital signal processor |
| FPGA | Field-programmable gate array |

#### Device Features

Features can be:
- **Core**: Available on all processing units
- **Class-specific**: Available on specific device classes
- **Optional**: May or may not be available on a specific device

#### Target Specification

COIL code can specify target requirements:

```
TARGET {
  required_features = ["feature1", "feature2"]
  preferred_features = ["feature3"]
  device_class = "CPU"
}
```

## 3. Binary Format

### 3.1 Module Structure

A COIL binary module contains:
- Header
- Section table
- Type section
- Function section
- Global section
- Constant section
- Code section
- Relocation section
- Metadata section

### 3.2 File Header

```
[magic:4][version:4][section_count:4][flags:4]
```

Where:
- magic: "COIL" in ASCII (0x434F494C)
- version: Version information (major.minor.patch)
- section_count: Number of sections in the module
- flags: Module-specific flags

### 3.3 Section Table

Each section entry contains:
```
[section_type:4][offset:4][size:4]
```

Where:
- section_type: Type of section
- offset: Byte offset from start of file
- size: Size of section in bytes

### 3.4 Type Section

Contains all type definitions used in the module.

### 3.5 Function Section

Contains function declarations with signatures.

### 3.6 Global Section

Contains global variable declarations.

### 3.7 Constant Section

Contains constant data values.

### 3.8 Code Section

Contains function implementations with instructions.

### 3.9 Relocation Section

Contains relocation information for linking.

### 3.10 Metadata Section

Contains optional metadata including:
- Debug information
- Optimization hints
- Source code mapping
- Custom annotations

## 4. Implementation Strategy

The current implementation strategy for COIL follows these guidelines:

### 4.1 Binary Format Implementation

1. Define a compact binary encoding that minimizes size while maintaining readability when decoded
2. Use little-endian encoding for all multi-byte values
3. Align sections on 4-byte boundaries for efficient access
4. Use variable-length encoding for values that can vary widely in size
5. Prioritize ease of parsing and generation over absolute minimum size

### 4.2 Type System Implementation

1. Use a 32-bit encoded type system for efficient storage and manipulation
2. Implement type compatibility checks at compile time
3. Support type extension through metadata for language-specific features
4. Provide mechanisms for target-specific type sizing and alignment

### 4.3 Instruction Set Implementation

1. Define a core set of instructions that map efficiently to common hardware
2. Provide extension mechanisms for specialized instructions
3. Implement instruction encoding that balances compactness with ease of parsing
4. Include optimization hints in the instruction encoding

### 4.4 Cross-Platform Compatibility

1. Define clear platform-independent semantics for all operations
2. Provide fallback implementations for optional features
3. Support target-specific specialization for performance-critical operations
4. Maintain consistent behavior across diverse hardware

## 5. Future Directions

COIL is designed to evolve with future hardware and software needs:

### 5.1 Upcoming Features

1. **Enhanced Vector Support**: Expanded vector operations for advanced SIMD capabilities
2. **Hardware Accelerator Integration**: Better support for specialized accelerators like AI hardware
3. **Memory Model Extensions**: Advanced memory models for new hardware architectures
4. **Fine-grained Concurrency**: Improved support for concurrent execution models
5. **Advanced Optimization Hints**: More detailed optimization directives

### 5.2 Standardization Efforts

1. Formal specification development
2. Reference implementation creation
3. Validation suite development
4. Ecosystem tool development

### 5.3 Hardware Evolution Support

COIL is designed to adapt to emerging hardware trends:
1. **Quantum Computing Extensions**: Preparing for quantum computing integration
2. **Neuromorphic Computing Support**: Extensions for brain-inspired computing architectures
3. **Novel Memory Hierarchies**: Support for new memory technologies and organizations
4. **Heterogeneous Integration**: Better integration across diverse processing units

### 5.4 Language Evolution

1. Support for new programming language features
2. Enhanced debug information for modern development tools
3. Better support for dynamic language features
4. Extended safety and security features

## 6. Conclusion

COIL provides a robust, architecture-independent binary intermediate representation that enables efficient compilation across diverse processing units. By clearly separating concerns between language semantics and target-specific details, COIL allows compiler developers to focus on language features while assembler developers focus on hardware optimization.

The specification will continue to evolve to support emerging hardware architectures and programming models while maintaining backwards compatibility and the core principles of zero runtime dependencies and bare-metal compatibility.