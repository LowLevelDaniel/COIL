# Instruction Set Architecture (Version 0.1.0)

The COIL Instruction Set Architecture (ISA) defines a complete set of operations that can be mapped to native instructions on target architectures. The COIL ISA is designed for simplicity, consistency, and efficient mapping to multiple architecture targets while maintaining precise control over execution.

## Instruction Format

All COIL instructions follow a uniform binary format:

```
[opcode: uint8_t][qualifier: uint8_t][operands...]
```

Where:
- **opcode**: An 8-bit value identifying the operation
- **qualifier**: An 8-bit value providing additional information about the operation
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

## Operand Qualifiers

```c
enum operand_qualifier : uint8_t {
  COIL_OPQUAL_IMM = (1 << 0), // Value is an immediate constant in the instruction
  COIL_OPQUAL_VAR = (1 << 1), // Value refers to a variable
  COIL_OPQUAL_REG = (1 << 2), // Value refers to a virtual register
  COIL_OPQUAL_MEM = (1 << 3), // Value refers to a memory address
  COIL_OPQUAL_LBL = (1 << 4), // Value refers to a label
  // Bits 5-7 reserved for future use
}
```

## Instruction Qualifiers
check type.md

## Opcodes

COIL opcodes are organized into logical categories:

```c
enum opcode : uint8_t {
  // No Operation (0x00)
  OP_NOP = 0x00, // No operation
  
  // Control Flow (0x01 - 0x0F)
  OP_CF_SYMB = 0x01, // Define a symbol
  OP_CF_BR   = 0x02, // Unconditional branch to location
  OP_CF_BRC  = 0x03, // Conditional branch to location
  OP_CF_CALL = 0x04, // Call subroutine
  OP_CF_RET  = 0x05, // Return from subroutine
  OP_CF_INT  = 0x06, // Trigger interrupt
  OP_CF_IRET = 0x07, // Return from interrupt
  OP_CF_WFI  = 0x08, // Wait for interrupt
  OP_CF_SYSC = 0x09, // System call
  OP_CF_WFE  = 0x0A, // Wait for event
  OP_CF_SEV  = 0x0B, // Send event
  // 0x0C - 0x0F reserved for future control flow operations
  
  // Arithmetic Operations (0x10 - 0x1F)
  OP_ARITH_ADD = 0x10, // Addition
  OP_ARITH_SUB = 0x11, // Subtraction
  OP_ARITH_MUL = 0x12, // Multiplication
  OP_ARITH_DIV = 0x13, // Division
  OP_ARITH_MOD = 0x14, // Modulus
  OP_ARITH_NEG = 0x15, // Negation
  OP_ARITH_ABS = 0x16, // Absolute value
  OP_ARITH_INC = 0x17, // Increment
  OP_ARITH_DEC = 0x18, // Decrement
  // 0x19 - 0x1F reserved for future arithmetic operations
  
  // Bit Manipulation (0x20 - 0x2F)
  OP_BIT_AND  = 0x20, // Bitwise AND
  OP_BIT_OR   = 0x21, // Bitwise OR
  OP_BIT_XOR  = 0x22, // Bitwise XOR
  OP_BIT_NOT  = 0x23, // Bitwise NOT
  OP_BIT_SHL  = 0x24, // Shift left
  OP_BIT_SHR  = 0x25, // Shift right logical
  OP_BIT_SAR  = 0x26, // Shift right arithmetic
  OP_BIT_ROL  = 0x27, // Rotate left
  OP_BIT_ROR  = 0x28, // Rotate right
  OP_BIT_CLZ  = 0x29, // Count leading zeroes
  OP_BIT_CTZ  = 0x2A, // Count trailing zeroes
  OP_BIT_POPC = 0x2B, // Population count
  // 0x2C - 0x2F reserved for future bit operations
  
  // Comparison Operations (0x30 - 0x3F)
  OP_CMP_CMP  = 0x30, // Compare instruction (with flags)
  OP_CMP_TEST = 0x31, // Test bits
  // 0x32 - 0x3F reserved for future comparison operations
  
  // Data Movement (0x40 - 0x4F)
  OP_MOV_MOV   = 0x40, // Move data
  OP_MOV_LOAD  = 0x41, // Load from memory
  OP_MOV_STORE = 0x42, // Store to memory
  OP_MOV_XCHG  = 0x43, // Exchange data
  // 0x44 - 0x4F reserved for future data movement operations
  
  // Stack Operations (0x50 - 0x5F)
  OP_STK_PUSH  = 0x50, // Push onto stack (operand can be of type state and then choose all registers, all flags, all registers and flags etc...)
  OP_STK_POP   = 0x51, // Pop from stack (determine from type)
  // 0x54 - 0x5F reserved for future stack operations
  
  // Variable Operations (0x60 - 0x6F)
  OP_VAR_CREATE = 0x60, // Create variable
  OP_VAR_DELETE = 0x61, // Delete variable
  OP_VAR_SCOPE  = 0x62, // Create variable scope
  OP_VAR_END    = 0x63, // End variable scope
  // 0x64 - 0x6F reserved for future variable operations
  
  // Conversion Operations (0x70 - 0x7F)
  OP_CONV_TRUNC = 0x70, // Truncate value
  OP_CONV_EXT   = 0x71, // Extend value
  OP_CONV_FTOI  = 0x72, // Float to integer
  OP_CONV_ITOF  = 0x73, // Integer to float
  OP_CONV_FTOB  = 0x74, // Float to bits
  OP_CONV_BTOF  = 0x75, // Bits to float
  // 0x76 - 0x7F reserved for future conversion operations

  // 0x80 - 0xEF reserved for future instruction categories
  
  // Assembler-specific Extensions (0xF0 - 0xFF)
  // This range is reserved for COIL assembler-specific instructions
  // These should not be used in portable code
  // 0xF0 - 0xFF
}
```

## Instruction Specifications

Each instruction is defined below with its operand format, behavior, and binary encoding. Note that COIL abstracts away many architecture-specific details, allowing the assembler to choose the most efficient native implementation.

### No Operation

**NOP** - No Operation
```
Opcode: 0x00
Operands: None
Binary Format: [0x00][0x00]
Description: Performs no operation. This may be optimized out by the assembler or used for alignment.
```

### Control Flow Operations

**SYMB** - Define Symbol
```
Opcode: 0x01
Operands: [symbol_name: string]
Binary Format: [0x01][0x00][string length: uint8_t][symbol_name: string]
Description: Defines a symbol at the current location in the code.
```

**BR** - Unconditional Branch
```
Opcode: 0x02
Operands: [target: operand_t]
Binary Format: [0x02][0x00][target]
Description: Branches to the specified target unconditionally.
Example: BR label // Branch to label
```

**BRC** - Conditional Branch
```
Opcode: 0x03
Operands: [condition: qualifier][target: operand_t]
Binary Format: [0x03][condition][target]
Description: Branches to the specified target if the condition is met.
Example: BRC.EQ label // Branch to label if equal flag is set
```

**CALL** - Call Subroutine
```
Opcode: 0x04
Operands: [target: operand_t]
Binary Format: [0x04][qualifier][target]
Description: Calls the subroutine at target, saving the return address.
Example: CALL function // Call function
```

**RET** - Return from Subroutine
```
Opcode: 0x05
Operands: None
Binary Format: [0x05][0x00]
Description: Returns from a subroutine to the saved return address.
Example: RET // Return from subroutine
```

### Arithmetic Operations

**ADD** - Addition
```
Opcode: 0x10
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x10][qualifier][dest][src1][src2] or [0x10][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Adds src1 and src2, storing the result in dest.
Example: ADD r0, r1, r2  // r0 = r1 + r2
         ADD r0, r0, r1  // r0 += r1 (in-place form)
```

**SUB** - Subtraction
```
Opcode: 0x11
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x11][qualifier][dest][src1][src2] or [0x11][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Subtracts src2 from src1, storing the result in dest.
Example: SUB r0, r1, r2  // r0 = r1 - r2
         SUB r0, r0, r1  // r0 -= r1 (in-place form)
```

**MUL** - Multiplication
```
Opcode: 0x12
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x12][qualifier][dest][src1][src2] or [0x12][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Multiplies src1 and src2, storing the result in dest.
Example: MUL r0, r1, r2  // r0 = r1 * r2
         MUL r0, r0, r1  // r0 *= r1 (in-place form)
```

**DIV** - Division
```
Opcode: 0x13
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x13][qualifier][dest][src1][src2] or [0x13][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Divides src1 by src2, storing the result in dest.
Example: DIV r0, r1, r2  // r0 = r1 / r2
         DIV r0, r0, r1  // r0 /= r1 (in-place form)
```

### Bit Manipulation Operations

**AND** - Bitwise AND
```
Opcode: 0x20
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x20][qualifier][dest][src1][src2] or [0x20][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Performs bitwise AND of src1 and src2, storing the result in dest.
Example: AND r0, r1, r2  // r0 = r1 & r2
         AND r0, r0, r1  // r0 &= r1 (in-place form)
```

**OR** - Bitwise OR
```
Opcode: 0x21
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x21][qualifier][dest][src1][src2] or [0x21][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Performs bitwise OR of src1 and src2, storing the result in dest.
Example: OR r0, r1, r2  // r0 = r1 | r2
         OR r0, r0, r1  // r0 |= r1 (in-place form)
```

**XOR** - Bitwise XOR
```
Opcode: 0x22
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Alternative (in-place): [dest/src1: operand_t][src2: operand_t]
Binary Format: [0x22][qualifier][dest][src1][src2] or [0x22][qualifier|COIL_ARITH_IN_PLACE][dest/src1][src2]
Description: Performs bitwise XOR of src1 and src2, storing the result in dest.
Example: XOR r0, r1, r2  // r0 = r1 ^ r2
         XOR r0, r0, r1  // r0 ^= r1 (in-place form)
```

### Data Movement Operations

**MOV** - Move Data
```
Opcode: 0x40
Operands: [dest: operand_t][src: operand_t]
Binary Format: [0x40][qualifier][dest][src]
Description: Copies the value from src to dest.
Example: MOV r0, r1  // r0 = r1
         MOV r0, 42  // r0 = 42
```

**LOAD** - Load from Memory
```
Opcode: 0x41
Operands: [dest: operand_t][address: operand_t]
Binary Format: [0x41][qualifier][dest][address]
Description: Loads a value from memory at the specified address into dest.
Example: LOAD r0, [r1]      // r0 = *r1
         LOAD r0, [r1+4]   // r0 = *(r1+4)
```

**STORE** - Store to Memory
```
Opcode: 0x42
Operands: [value: operand_t][address: operand_t]
Binary Format: [0x42][qualifier][value][address]
Description: Stores a value to memory at the specified address.
Example: STORE r0, [r1]     // *r1 = r0
         STORE r0, [r1+4]  // *(r1+4) = r0
```

### Stack Operations

**PUSH** - Push onto Stack
```
Opcode: 0x50
Operands: [value: operand_t]
Binary Format: [0x50][qualifier][value]
Description: Pushes a value onto the stack.
Example: PUSH r0  // Push r0 onto the stack
```

**POP** - Pop from Stack
```
Opcode: 0x51
Operands: [dest: operand_t]
Binary Format: [0x51][qualifier][dest]
Description: Pops a value from the stack into dest.
Example: POP r0  // Pop into r0 from the stack
```

### Variable Operations

**CREATE** - Create Variable
```
Opcode: 0x60
Operands: [name: string][type: operand_t][initial_value: operand_t (optional)]
Binary Format: [0x60][qualifier][string length: uint8_t][name: string][type][initial_value (optional)]
Description: Creates a variable with the specified name and type, optionally with an initial value.
Example: CREATE "counter", i32, 0  // int32_t counter = 0;
```

**DELETE** - Delete Variable
```
Opcode: 0x61
Operands: [name: string]
Binary Format: [0x61][qualifier][string length: uint8_t][name: string]
Description: Deletes the specified variable, freeing its resources.
Example: DELETE "counter"  // Delete the counter variable
```

**SCOPE** - Create Variable Scope
```
Opcode: 0x62
Operands: None
Binary Format: [0x62][qualifier]
Description: Creates a new variable scope. Variables created after this point will be limited to this scope.
Example: SCOPE  // Begin a new variable scope
```

**END** - End Variable Scope
```
Opcode: 0x63
Operands: None
Binary Format: [0x63][qualifier]
Description: Ends the current variable scope, freeing all variables created within it.
Example: END  // End the current variable scope
```

## Instruction Encoding Examples

This section provides examples of how instructions are encoded in binary.

### Example 1: ADD r0, r1, r2 (Add registers)

```
// Assuming r0, r1, r2 are virtual registers of type int32
[0x10]                  // Opcode: ADD
[0x00]                  // Qualifier: Default (not in-place)
[COIL_OPQUAL_REG|0x04]  // Dest qualifier: Register, type int32
[0x00]                  // Register number: r0
[COIL_OPQUAL_REG|0x04]  // Src1 qualifier: Register, type int32
[0x01]                  // Register number: r1
[COIL_OPQUAL_REG|0x04]  // Src2 qualifier: Register, type int32
[0x02]                  // Register number: r2
```

### Example 2: MOV r0, 42 (Move immediate to register)

```
// Assuming r0 is a virtual register of type int32
[0x40]                    // Opcode: MOV
[0x00]                    // Qualifier: Default
[COIL_OPQUAL_REG|0x04]    // Dest qualifier: Register, type int32
[0x00]                    // Register number: r0
[COIL_OPQUAL_IMM|0x04]    // Src qualifier: Immediate, type int32
[0x2A, 0x00, 0x00, 0x00]  // Value: 42 (little-endian int32)
```

### Example 3: BRC.EQ label (Branch if equal)

```
// Assuming "label" is a defined symbol
[0x03]                      // Opcode: BRC
[COIL_BR_EQ]                // Qualifier: Branch if equal
[COIL_OPQUAL_LBL|0x00]      // Target qualifier: Label
[0x0A]                      // Label name length: 10
[0x6C, 0x61, 0x62, 0x65, 0x6C]  // "label" in ASCII
```

## Optimization Considerations

COIL assemblers are encouraged to optimize instruction sequences when possible, as long as the semantics are preserved. Common optimizations include:

1. Eliminating redundant moves
2. Combining sequential operations on the same operands
3. Replacing complex operations with simpler equivalents
4. Using architecture-specific instructions when available

However, optimizations must not change the behavior of the program, particularly regarding side effects and exception handling.

## Implementation Requirements

COIL assemblers must implement all instructions defined in this specification for version 0.1.0. Behavior must be consistent across different implementations, even if the underlying native code generation differs.

## Instruction Set Extensions

Future versions of COIL may include additional instructions for:

1. Vector/SIMD operations
2. Atomic operations
3. Hardware-specific features
4. Advanced control flow constructs

These will be defined in later specification versions.