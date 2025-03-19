# Instruction Set and Directive Integration in the COIL Binary Format (Version 0.1.0)

This document clarifies the relationship between instructions and directives in the COIL binary format. Both instructions and directives are encoded as binary structures, but they serve different purposes and are processed differently by COIL assemblers.

## Binary Format Distinction

In the COIL binary format, both instructions and directives are represented as binary structures, but they are distinguished by their opcode values:

```c
// Opcode ranges
#define OPCODE_RANGE_INSTRUCTION  0x00-0xCF  // Instructions
#define OPCODE_RANGE_DIRECTIVE    0xD0-0xDF  // Directives
#define OPCODE_RANGE_EXTENSION    0xE0-0xFF  // Implementation-specific extensions
```

This clear separation allows the COIL assembler to identify and process each element appropriately.

## General Binary Structure

The general binary structure for both instructions and directives follows a similar pattern:

```
[opcode: uint8_t][qualifier: uint8_t][additional data...]
```

However, the interpretation of the additional data differs:

1. **Instructions**: Follow the format defined in the ISA specification with operands encoded according to their types
2. **Directives**: Include a length field followed by directive-specific payload

## Binary Format for Instructions

As defined in the ISA specification, instructions use the following format:

```
[opcode: uint8_t][qualifier: uint8_t][operands...]
```

Example binary encoding for `ADD RQ0, RQ1, RQ2`:
```
10 00                    // ADD opcode (0x10) with default qualifier (0x00)
04 00 00                 // First operand: Register RQ0 (type 0x04, index 0x00)
04 00 01                 // Second operand: Register RQ1 (type 0x04, index 0x01)
04 00 02                 // Third operand: Register RQ2 (type 0x04, index 0x02)
```

## Binary Format for Directives

Directives follow this format:

```
[directive_opcode: uint8_t][qualifier: uint8_t][length: uint16_t][payload: variable]
```

Example binary encoding for a version directive specifying version 0.1.0:
```
D0 00 03 00 00 01 00     // Directive opcode 0xD0, qualifier 0x00, length 3 bytes,
                         // payload {major: 0x00, minor: 0x01, patch: 0x00}
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

When a COIL assembler processes a COIL binary, it:

1. Interprets the file header and global directives
2. Processes each section according to its type
3. For code sections:
   a. Processes each element based on its opcode range
   b. For instructions: translates to native code
   c. For directives: applies the specified behavior to the assembly process
4. Applies optimizations as specified by directives
5. Generates the final NCOF output

## Specific Integration Points

### 1. Sections and Segment Management

The section directive (0xD2) in binary format creates and switches between sections:

```
D2 01 01 00 01     // Create/switch to text section with execute flag
```

Instructions that follow are placed in this section:

```
40 00 04 00 00 ...  // MOV instruction placed in the text section
```

### 2. Symbol Definitions and References

Symbol directives define symbols that instructions can reference:

```
D3 02 05 00 04 6D 61 69 6E  // Define global symbol "main"
```

Instructions can then reference these symbols:

```
02 00 0F 00 04 6D 61 69 6E  // Branch to symbol "main"
```

### 3. ABI and Function Calls

ABI directives define calling conventions:

```
D6 00 07 00 06 6D 79 2D 61 62 69  // Begin ABI "my-abi"
D6 01 04 00 00 00 00 00            // Return value in GPR 0
// ...more mappings...
D6 02 00 00                        // End ABI
```

Call instructions can reference these ABIs:

```
04 01 [target operand] [ABI name operand]  // Call using specified ABI
```

### 4. Data Definition and Access

Data directives place data in the binary:

```
D5 01 03 00 12 34 56  // Define three bytes: 0x12, 0x34, 0x56
```

Instructions can access this data through addressing:

```
41 00 [dest operand] [address operand]  // Load data from address
```

## Binary Example with Mixed Instructions and Directives

Here's an example of a small COIL binary segment with both instructions and directives:

```
// Module-level directive: COIL version 0.1.0
D0 00 03 00 00 01 00

// Target directive: x86-64
D1 00 02 00 02 00

// Section directive: text section
D2 01 01 00 01

// Symbol directive: define global "main"
D3 02 05 00 04 6D 61 69 6E

// Instructions (MOV RQ0, 42)
40 00 04 00 00 01 00 2A 00 00 00 00 00 00 00

// Alignment directive: 16-byte boundary
D4 00 02 00 10 00

// More instructions (ADD RQ1, RQ0, 58)
10 00 04 00 01 04 00 00 01 00 3A 00 00 00 00 00 00 00

// Return instruction
05 00
```

## Implementation Requirements

COIL assemblers must:

1. Correctly parse and interpret both instruction and directive binary formats
2. Process directives and instructions in the correct sequence
3. Apply directive effects appropriately to the assembly process
4. Maintain the semantic distinction between directives (assembly-time) and instructions (runtime)
5. Generate appropriate NCOF code reflecting both instruction translations and directive effects

## COIL Explanatory Language (CEL) Representation

For documentation purposes, both instructions and directives can be represented in COIL Explanatory Language (CEL). In CEL, directives are conventionally prefixed with a period (`.`) to distinguish them from instructions.

Example CEL representation (not COIL binary format):
```
.version 0.1.0
.target x86-64
.section .text
.global main
main:
  MOV RQ0, 42
  .align 16
  ADD RQ1, RQ0, 58
  RET
```

It's important to remember that CEL is not COIL itself; COIL is always a binary format, and the actual encoding follows the binary formats described above.

## Future Extensions

As COIL evolves, the integration between instructions and directives will be enhanced while maintaining the clear binary format distinction:

1. More sophisticated conditional assembly through specialized directive opcodes
2. Enhanced optimization control with more detailed directive qualifiers
3. Extended instruction set with corresponding directives for new capabilities
4. Advanced metaprogramming through specialized directive types
5. Enhanced debugging support with integrated instruction and directive information

These enhancements will preserve the binary structure defined here while extending the capabilities of the COIL binary format.