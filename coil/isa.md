# Instruction Set Architecture

The COIL Instruction Set Architecture (ISA) defines operations that can be mapped to native instructions on target architectures. The COIL ISA is designed for simplicity, consistency, and efficient mapping to multiple CPU architectures while maintaining precise control over execution.

## Binary Format Distinction

In the COIL binary format, both instructions and directives are represented as binary structures, but they are distinguished by their opcode values:

```c
// Opcode ranges
#define OPCODE_RANGE_INSTRUCTION  0x00-0xCF  // Instructions
#define OPCODE_RANGE_DIRECTIVE    0xD0-0xDF  // Directives
#define OPCODE_RANGE_EXTENSION    0xE0-0xFF  // Implementation-specific extensions
```

This separation allows the COIL processor to identify and process each element appropriately.

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
See type.md for details

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
  OP_STK_PUSH  = 0x50, // Push onto stack
  OP_STK_POP   = 0x51, // Pop from stack
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
  
  // Processor-specific Extensions (0xF0 - 0xFF)
  // This range is reserved for COIL processor-specific instructions
  // These should not be used in portable code
  // 0xF0 - 0xFF
}
```

## Instruction and Directive Coexistence in COF

In a COIL Object Format (COF) file, instructions and directives appear in specific contexts:

1. **File/Module Level**: Certain directives (like version and target) apply to the entire module
2. **Section Level**: Section directives define and control different sections
3. **Code Stream**: Instructions appear in code sections, with interspersed directives affecting the code generation

The binary layout of a code section might look like:

```
[Section header]
[Directive 1]       // e.g., alignment directive
[Instruction 1]
[Instruction 2]
[Directive 2]       // e.g., optimization hint
[Instruction 3]
...
```

## Processing Sequence

When a COIL processor processes a COIL binary, it:

1. Interprets the file header and global directives
2. Processes each section according to its type
3. For code sections:
   a. Processes each element based on its opcode range
   b. For instructions: translates to native code
   c. For directives: applies the specified behavior to the processing
4. Applies optimizations as specified by directives
5. Generates the final NCOF output

## Instruction Specifications

Each instruction is defined below with its operand format, behavior, and binary encoding. Note that COIL abstracts away many architecture-specific details, allowing the processor to choose the most efficient native implementation.

### No Operation

**NOP** - No Operation
```
Opcode: 0x00
Operands: None
Binary Format: [0x00][0x00]
Description: Performs no operation. This may be optimized out by the processor or used for alignment.
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
Example (in CEL): BR label // Branch to label
```

**BRC** - Conditional Branch
```
Opcode: 0x03
Operands: [condition: qualifier][target: operand_t]
Binary Format: [0x03][condition][target]
Description: Branches to the specified target if the condition is met.
Example (in CEL): BRC.EQ label // Branch to label if equal flag is set
```

[Additional instructions omitted for brevity]

## Implementation Requirements

COIL processors must:

1. Correctly parse and interpret both instruction and directive binary formats
2. Process directives and instructions in the correct sequence
3. Apply directive effects appropriately to the processing
4. Maintain the semantic distinction between directives (processing-time) and instructions (runtime)
5. Generate appropriate NCOF code reflecting both instruction translations and directive effects

## Future Extensions

Although version 0.1.0 focuses primarily on CPU architectures, future versions will introduce a mechanism for handling different processing units through directive-based context switching. This will allow the same COIL binary format to represent code for different types of processing units (GPU, TPU, etc.) by changing how the processor interprets subsequent instructions.

This approach will maintain architecture independence while acknowledging that different processing unit types have fundamentally different execution models.
