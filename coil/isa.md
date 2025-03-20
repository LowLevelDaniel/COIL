# Instruction Set Architecture (Version 1.0.0) - REVISED

The COIL Instruction Set Architecture (ISA) defines the complete set of operations available in the COIL binary format. This specification documents the binary encoding, behavior, and implementation requirements for all instructions in version 1.0.0.

## Binary Format Overview

In the COIL binary format, instructions and directives share a common encoding structure but are distinguished by their opcode ranges:

```c
// Opcode ranges
#define OPCODE_RANGE_INSTRUCTION  0x00-0xCF  // Instructions
#define OPCODE_RANGE_DIRECTIVE    0xD0-0xEF  // Directives
#define OPCODE_RANGE_EXTENSION    0xF0-0xFF  // Implementation-specific extensions
```

## Instruction Format

All COIL instructions follow a consistent binary format:

```
[opcode: uint8_t][qualifier: uint8_t][operand_count: uint8_t][operands...]
```

Where:
- **opcode**: An 8-bit value identifying the operation
- **qualifier**: An 8-bit value providing additional information about the operation
- **operand_count**: The number of operands that follow
- **operands**: A variable number of operands whose format depends on the operation

## Operand Format

Operands use a consistent format to represent different types of values:

```
[qualifier: uint8_t][type: uint8_t][data: variable]
```

Where:
- **qualifier**: Specifies how to interpret the operand (immediate, variable, etc.)
- **type**: Specifies the data type of the operand (see type.md)
- **data**: The actual data, with size determined by type and qualifier

The type information in operands is used to determine the specific implementation of the operation (e.g., integer addition vs. floating-point addition).

## Operand Qualifiers

```c
enum operand_qualifier : uint8_t {
    OPQUAL_IMM = 0x01, // Value is an immediate constant in the instruction
    OPQUAL_VAR = 0x02, // Value refers to a variable
    OPQUAL_REG = 0x03, // Value refers to a virtual register
    OPQUAL_MEM = 0x04, // Value refers to a memory address
    OPQUAL_LBL = 0x05, // Value refers to a label
    OPQUAL_STR = 0x06, // Value is a string literal (offset into string table)
    OPQUAL_SYM = 0x07, // Value refers to a symbol (index into symbol table)
    OPQUAL_REL = 0x08, // Value is a relative offset
    // 0x09-0xFF reserved for future qualifiers
};
```

## Instruction Opcodes

COIL instructions are organized into logical categories. Unlike traditional ISAs, COIL operations are type-agnostic, with the actual implementation determined by the operand types.

```c
enum instruction_opcode : uint8_t {
    // No Operation (0x00)
    OP_NOP = 0x00, // No operation

    // Control Flow (0x01 - 0x0F)
    OP_SYMB = 0x01, // Define a symbol
    OP_BR   = 0x02, // Unconditional branch to location
    OP_BRC  = 0x03, // Conditional branch to location
    OP_CALL = 0x04, // Call subroutine
    OP_RET  = 0x05, // Return from subroutine
    OP_INT  = 0x06, // Trigger interrupt
    OP_IRET = 0x07, // Return from interrupt
    OP_WFI  = 0x08, // Wait for interrupt
    OP_SYSC = 0x09, // System call
    OP_WFE  = 0x0A, // Wait for event
    OP_SEV  = 0x0B, // Send event
    OP_TRAP = 0x0C, // Software trap
    // 0x0D-0x0F reserved for future control flow operations

    // Arithmetic Operations (0x10 - 0x1F)
    OP_ADD  = 0x10, // Addition (for any numeric type)
    OP_SUB  = 0x11, // Subtraction (for any numeric type)
    OP_MUL  = 0x12, // Multiplication (for any numeric type)
    OP_DIV  = 0x13, // Division (for any numeric type)
    OP_MOD  = 0x14, // Modulus (for any numeric type)
    OP_NEG  = 0x15, // Negation (for any numeric type)
    OP_ABS  = 0x16, // Absolute value (for any numeric type)
    OP_INC  = 0x17, // Increment (for any numeric type)
    OP_DEC  = 0x18, // Decrement (for any numeric type)
    // 0x19-0x1F reserved for future arithmetic operations

    // Bit Manipulation (0x20 - 0x2F) - Applicable to integer types only
    OP_AND  = 0x20, // Bitwise AND
    OP_OR   = 0x21, // Bitwise OR
    OP_XOR  = 0x22, // Bitwise XOR
    OP_NOT  = 0x23, // Bitwise NOT
    OP_SHL  = 0x24, // Shift left
    OP_SHR  = 0x25, // Shift right logical
    OP_SAR  = 0x26, // Shift right arithmetic
    OP_ROL  = 0x27, // Rotate left
    OP_ROR  = 0x28, // Rotate right
    OP_CLZ  = 0x29, // Count leading zeros
    OP_CTZ  = 0x2A, // Count trailing zeros
    OP_POPC = 0x2B, // Population count
    OP_BSWP = 0x2C, // Byte swap
    OP_BEXT = 0x2D, // Bit extraction
    OP_BINS = 0x2E, // Bit insertion
    OP_BMSK = 0x2F, // Bit mask
    
    // Comparison Operations (0x30 - 0x3F)
    OP_CMP  = 0x30, // Compare values and set flags
    OP_TEST = 0x31, // Test bits (bitwise AND and set flags, for integer types)
    // 0x32-0x3F reserved for future comparison operations
    
    // Data Movement (0x40 - 0x4F)
    OP_MOV   = 0x40, // Move data
    OP_LOAD  = 0x41, // Load from memory
    OP_STORE = 0x42, // Store to memory
    OP_XCHG  = 0x43, // Exchange data
    OP_LEA   = 0x44, // Load effective address
    OP_MOVI  = 0x45, // Move immediate value
    OP_MOVZ  = 0x46, // Move with zero extension
    OP_MOVS  = 0x47, // Move with sign extension
    OP_LDMUL = 0x48, // Load multiple
    OP_STMUL = 0x49, // Store multiple
    OP_LDSTR = 0x4A, // Load string
    OP_STSTR = 0x4B, // Store string
    // 0x4C-0x4F reserved for future data movement operations
    
    // Stack Operations (0x50 - 0x5F)
    OP_PUSH  = 0x50, // Push onto stack
    OP_PUSHA = 0x51, // Push all registers
    OP_PUSHF = 0x52, // Push flags
    OP_POP   = 0x53, // Pop from stack    
    // 0x54-0x5F reserved for future stack operations
    
    // Variable Operations (0x60 - 0x6F)
    OP_VARCR = 0x60, // Create variable
    OP_VARDL = 0x61, // Delete variable
    OP_VARSC = 0x62, // Create variable scope
    OP_VAREND = 0x63, // End variable scope
    // 0x64-0x6F reserved for future variable operations
    
    // Conversion Operations (0x70 - 0x7F)
    OP_TRUNC = 0x70, // Truncate value
    OP_ZEXT  = 0x71, // Zero extend value
    OP_SEXT  = 0x72, // Sign extend value
    OP_FTOI  = 0x73, // Float to integer
    OP_ITOF  = 0x74, // Integer to float
    OP_FTOB  = 0x75, // Float to bits
    OP_BTOF  = 0x76, // Bits to float
    // 0x77-0x7F reserved for future conversion operations
    
    // Directive range: 0xD0-0xEF (defined in dir.md)
    // Extension range: 0xF0-0xFF (implementation-specific)
};
```

## Instruction Qualifiers

Each instruction category uses its own qualifier bit definitions:

### Arithmetic Operation Qualifiers
```c
enum arith_qualifier : uint8_t {
    ARITH_DEFAULT = 0x00,     // Default behavior
    ARITH_SIGNED  = (1 << 0), // Signed operation
    ARITH_SAT     = (1 << 1), // Saturating operation
    ARITH_TRAP    = (1 << 4), // Trap on overflow
    ARITH_APPROX  = (1 << 5), // Allow approximation
    ARITH_UNCHECKED = (1 << 6), // Assume no overflow
    // Bit 7 reserved
};
```

### Branch Condition Qualifiers
```c
enum branch_qualifier : uint8_t {
    BR_ALWAYS = 0x00, // Always branch (unconditional)
    BR_EQ     = 0x01, // Equal / Zero
    BR_NE     = 0x02, // Not equal / Not zero
    BR_LT     = 0x03, // Less than
    BR_LE     = 0x04, // Less than or equal
    BR_GT     = 0x05, // Greater than
    BR_GE     = 0x06, // Greater than or equal
    BR_CARRY  = 0x07, // Carry flag set
    BR_OFLOW  = 0x08, // Overflow flag set
    BR_SIGN   = 0x09, // Sign flag set
    BR_PARITY = 0x0A, // Parity flag set
    BR_NCARRY = 0x0B, // Carry flag not set
    BR_NOFLOW = 0x0C, // Overflow flag not set
    BR_NSIGN  = 0x0D, // Sign flag not set
    BR_NPARITY = 0x0E, // Parity flag not set
    // 0x0F-0xFF reserved for future conditions
};
```

### Memory Operation Qualifiers
```c
enum memory_qualifier : uint8_t {
    MEM_DEFAULT   = 0x00,     // Default memory access
    MEM_VOLATILE  = (1 << 0), // Volatile access
    MEM_ATOMIC    = (1 << 1), // Atomic access
    MEM_ALIGNED   = (1 << 2), // Aligned access
    MEM_UNALIGNED = (1 << 3), // Unaligned access allowed
    MEM_NONTEMPORAL = (1 << 4), // Non-temporal access hint
    MEM_READONLY  = (1 << 5), // Read-only memory hint
    MEM_WRITEONLY = (1 << 6), // Write-only memory hint
    // Bit 7 reserved
};
```

### System Call Qualifiers
```c
enum syscall_qualifier : uint8_t {
    SYSC_DEFAULT = 0x00,      // Default system call
    SYSC_PRIV    = (1 << 0),  // Privileged system call
    SYSC_TRAP    = (1 << 1),  // Use trap instruction
    SYSC_INT     = (1 << 2),  // Use interrupt instruction
    SYSC_FAST    = (1 << 3),  // Use fast system call if available
    SYSC_VIRT    = (1 << 4),  // Virtualization-aware system call
    // Bits 5-7 reserved
};
```

## Type-Driven Operation Selection

A key feature of COIL is that the same operation code applies to different data types. The COIL processor selects the appropriate implementation based on the operand types:

1. When operands are integer types, integer arithmetic is used
2. When operands are floating-point types, floating-point arithmetic is used
3. When operands are vector types, vector operations are used

This avoids the need for separate operation codes for different data types (e.g., ADD vs. FADD), simplifying the ISA while still providing type-specific optimizations.

### Native Processor Mapping

COIL's design aligns with how processors typically work:

1. **Flag-Setting Comparison**: Most CPUs have a single comparison instruction that sets flags based on the result. COIL's `CMP` instruction follows this design.

2. **Conditional Operations**: Rather than having multiple comparison opcodes (CMPEQ, CMPLT, etc.), COIL uses a single `CMP` instruction followed by conditional branches based on flag states, matching typical processor design.

3. **Type-Based Implementation**: The actual implementation of instructions depends on the operand types, allowing the COIL processor to select the appropriate native instructions without requiring separate COIL opcodes.

## Detailed Instruction Specifications

Each instruction is defined below with its binary encoding, operands, behavior, and implementation requirements.

### Arithmetic Instructions

#### ADD - Addition
```
Opcode: 0x10
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x10][arith_qualifier][0x03][dest][src1][src2]
Description: Adds src1 and src2, storing the result in dest.
Implementation:
  - When operands are integer types, integer addition is used
  - When operands are floating-point types, floating-point addition is used
  - When operands are vector types, vector addition is used
  - Must handle signed/unsigned distinction for integer types based on qualifier
  - Should use the most efficient addition instruction available for the type
```

#### SUB - Subtraction
```
Opcode: 0x11
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x11][arith_qualifier][0x03][dest][src1][src2]
Description: Subtracts src2 from src1, storing the result in dest.
Implementation:
  - When operands are integer types, integer subtraction is used
  - When operands are floating-point types, floating-point subtraction is used
  - When operands are vector types, vector subtraction is used
  - Must handle signed/unsigned distinction for integer types based on qualifier
  - Should use the most efficient subtraction instruction available for the type
```

#### MUL - Multiplication
```
Opcode: 0x12
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x12][arith_qualifier][0x03][dest][src1][src2]
Description: Multiplies src1 by src2, storing the result in dest.
Implementation:
  - When operands are integer types, integer multiplication is used
  - When operands are floating-point types, floating-point multiplication is used
  - When operands are vector types, vector multiplication is used
  - Must handle signed/unsigned distinction for integer types based on qualifier
  - Should use the most efficient multiplication instruction available for the type
```

### Comparison Instructions

#### CMP - Compare
```
Opcode: 0x30
Qualifier: 0x00
Operands: [src1: operand_t][src2: operand_t]
Binary Format: [0x30][0x00][0x02][src1][src2]
Description: Compares src1 with src2 and sets flags based on the result.
Implementation:
  - When operands are integer types, integer comparison is used
  - When operands are floating-point types, floating-point comparison is used
  - When operands are vector types, vector comparison is used
  - Must set appropriate flags based on the comparison result
  - Should use the most efficient comparison instruction available for the type
```

#### TEST - Test Bits
```
Opcode: 0x31
Qualifier: 0x00
Operands: [src1: operand_t][src2: operand_t]
Binary Format: [0x31][0x00][0x02][src1][src2]
Description: Performs a bitwise AND of src1 and src2 and sets flags based on the result, without storing the result.
Implementation:
  - Only applicable to integer types
  - Sets the zero flag if the result is zero
  - Sets the sign flag based on the most significant bit of the result
  - Clears the overflow and carry flags
  - Often used for efficiently testing if a specific bit is set
```

### Special Operations

#### SQRT - Square Root
```
Opcode: 0x90
Qualifier: 0x00
Operands: [dest: operand_t][src: operand_t]
Binary Format: [0x90][0x00][0x02][dest][src]
Description: Calculates the square root of src and stores the result in dest.
Implementation:
  - When operand is a floating-point type, floating-point square root is used
  - When operand is an integer type, integer square root is used (possibly with conversion)
  - Should use the most efficient square root instruction available for the type
```

## Implementation Requirements

COIL processors must adhere to the following requirements:

1. **Type-Driven Implementation**: Operations must be implemented based on operand types, not just the opcode. The same opcode (e.g., ADD) must work correctly for different data types.

2. **Instruction Fidelity**: All instructions must be implemented with the semantics defined in this specification.

3. **Feature-Based Implementation**: When a native instruction exists that matches the COIL instruction semantics, it should be used. Otherwise, the processor should generate the equivalent functionality through emulation.

4. **Optimization Freedom**: Processors are free to optimize instruction sequences as long as the observable behavior remains the same.

5. **Error Handling**: Processors should implement architecture-appropriate error handling for exceptional conditions.

6. **ABI Compliance**: Instructions that interact with ABIs must follow the ABI definitions in the configuration.

## Type Resolution Rules

When operands have different types, the COIL processor follows these rules for type resolution:

1. If any operand is floating-point, the operation is performed as floating-point, with integer operands being converted to floating-point.
2. Otherwise, if any operand is unsigned, the operation is performed as unsigned integer.
3. Otherwise, the operation is performed as signed integer.
4. If operands have different widths, they are widened to the widest operand type.

## Verification

A correctly implemented COIL processor should pass a verification suite that tests:

1. Instruction encoding/decoding
2. Semantic correctness for each instruction
3. Proper handling of edge cases
4. ABI compliance
5. Type-driven operation selection

Such verification suites are not part of this specification but are recommended for implementations.

## Binary Compatibility Guarantees

This instruction set specification establishes the following guarantees:

1. All COIL 1.0.0 processors must implement all instructions defined in this specification.
2. Future versions of COIL will maintain backward compatibility with version 1.0.0 instructions.
3. Future COIL processors must be able to process COIL 1.0.0 instructions correctly.

## Extension Mechanism

The opcode range 0xE0-0xFF is reserved for implementation-specific extensions. Extensions must be documented by the processor implementation and should follow the same general format as standard instructions.

## Architecture-Specific Considerations

While COIL is architecture-independent, the following considerations apply when implementing the ISA for specific architectures:

### x86-64
- Efficient use of CISC-style instructions
- Proper handling of condition codes
- Optimization for complex addressing modes

### ARM
- Efficient use of predicated execution
- Proper register allocation for ARM calling conventions
- Handling of ARM-specific memory ordering models

### RISC-V
- Efficient decomposition of complex COIL instructions into RISC-V primitives
- Proper implementation of compressed instruction formats
- Handling of RISC-V's weak memory model