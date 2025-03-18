# COIL Binary Encoding Reference

## Introduction

The COIL Binary Encoding Reference provides detailed specifications for the exact binary representation of COIL instructions, operands, and other data structures. This document serves as the authoritative reference for implementers of COIL toolchains, ensuring consistent encoding and decoding across different implementations.

## Instruction Encoding

### Basic Instruction Format

Every COIL instruction follows a consistent encoding format:

```
+-------------+----------------+----------------+------------------+-------------------+
| Opcode      | Operand Count  | Ext Data Size  | Operands         | Extended Data     |
| (1 byte)    | (1 byte)       | (2 bytes)      | (variable)       | (variable)        |
+-------------+----------------+----------------+------------------+-------------------+
```

- **Opcode** (1 byte): Encodes the instruction category and operation
- **Operand Count** (1 byte): Number of operands that follow
- **Ext Data Size** (2 bytes, little-endian): Size of extended data in bytes
- **Operands** (variable): Sequence of encoded operands
- **Extended Data** (variable): Optional additional data for the instruction

### Opcode Encoding

The opcode byte encodes both the instruction category and the specific operation:

```
+---------------+-------------------+
| Category      | Operation         |
| (3 bits)      | (5 bits)          |
+---------------+-------------------+
  7 6 5           4 3 2 1 0
```

The 3 most significant bits (7-5) encode the instruction category, while the 5 least significant bits (4-0) encode the specific operation within that category.

#### Category Encodings

```
000 - Control Flow (CF)
001 - Memory Operations (MEM)
010 - Arithmetic Operations (MATH)
011 - Bit Manipulation (BIT)
100 - Vector Operations (VEC)
101 - Atomic Operations (ATM)
110 - Variable Management (VAR)
111 - Frame Management (FRAME)
```

#### Operation Encodings (Examples for Control Flow)

```
00000 - BR     (Unconditional branch)
00001 - BRC    (Conditional branch)
00010 - CALL   (Function call)
00011 - RET    (Return from function)
00100 - INT    (Software interrupt)
00101 - IRET   (Return from interrupt)
00110 - HLT    (Halt execution)
00111 - SYSC   (System call)
01000 - TRAP   (Trap to exception handler)
```

Full operation encodings for all categories are provided in the appendix.

## Operand Encoding

Each operand begins with a type byte that encodes the operand class and type-specific details, followed by operand-specific data:

```
+-------------+--------------------+
| Type Byte   | Operand-specific   |
| (1 byte)    | Data (variable)    |
+-------------+--------------------+
```

### Operand Type Byte

The operand type byte encodes both the operand class and type-specific details:

```
+---------------+-------------------+
| Class         | Type Details      |
| (2 bits)      | (6 bits)          |
+---------------+-------------------+
  7 6             5 4 3 2 1 0
```

#### Operand Class Encodings

```
00 - Register Operand
01 - Immediate Value
10 - Memory Operand
11 - Variable Reference
```

### Register Operand Encoding

Register operands are encoded with 3 bytes:

```
+-------------+-------------+-------------+
| Type Byte   | Register ID | Flags       |
| (1 byte)    | (1 byte)    | (1 byte)    |
+-------------+-------------+-------------+
```

- **Type Byte**: `00` (class) followed by register type details
- **Register ID**: Virtual register identifier
- **Flags**: Additional flags for register usage

Example for a general-purpose register R0:
```
00000000 00000000 00000000
   |        |        |
   |        |        +-- No flags
   |        +----------- R0 (register ID 0)
   +-------------------- Register operand, general purpose
```

### Immediate Operand Encoding

Immediate operands begin with a type byte followed by the value in the appropriate format:

```
+-------------+-------------------+
| Type Byte   | Immediate Value   |
| (1 byte)    | (variable)        |
+-------------+-------------------+
```

For example, a 32-bit integer immediate value of 42:
```
01000010 00101010 00000000 00000000 00000000
   |        |
   |        +---------------------- 42 in little-endian
   +------------------------------- Immediate operand, 32-bit integer
```

### Memory Operand Encoding

Memory operands vary in encoding based on the addressing mode:

```
+-------------+----------------+
| Type Byte   | Address Data   |
| (1 byte)    | (variable)     |
+-------------+----------------+
```

For example, a register indirect memory operand [R0]:
```
10000001 00000000 00000000
   |        |        |
   |        |        +-- No flags
   |        +----------- R0 (register ID 0)
   +-------------------- Memory operand, register indirect
```

### Variable Operand Encoding

Variable operands are encoded with a type byte followed by the variable ID:

```
+-------------+-------------+
| Type Byte   | Variable ID |
| (1 byte)    | (1 byte)    |
+-------------+-------------+
```

For example, a variable reference to $0:
```
11000000 00000000
   |        |
   |        +----------- Variable ID 0
   +-------------------- Variable operand, direct reference
```

## Detailed Examples

### Example 1: MATH ADD R0, R1, 42

This instruction adds the immediate value 42 to register R1 and stores the result in R0.

```
01000000  # Opcode: MATH ADD (010 category, 00000 operation)
00000011  # Operand count: 3
00000000  # Extended data size: 0 (no extended data)
00000000  # 

# Operand 1: Register R0
00000000  # Type: Register operand, general purpose
00000000  # Register ID: 0 (R0)
00000000  # Flags: None

# Operand 2: Register R1
00000000  # Type: Register operand, general purpose
00000001  # Register ID: 1 (R1)
00000000  # Flags: None

# Operand 3: Immediate value 42
01000010  # Type: Immediate operand, 32-bit integer
00101010  # Value: 42 (little-endian)
00000000
00000000
00000000
```

### Example 2: CF BRC EQ label

This instruction performs a conditional branch to the specified label if the equal condition is true.

```
00000001  # Opcode: CF BRC (000 category, 00001 operation)
00000001  # Operand count: 1
00000002  # Extended data size: 2 bytes
00000000

# Operand 1: Symbol reference to label
01000110  # Type: Immediate operand, symbol reference
10110100  # Symbol ID (little-endian)
00101100
00000000
00000000

# Extended data
00000000  # Condition code: 0 (EQ)
00000000  # Branch prediction: 0 (no prediction)
```

### Example 3: VAR DECL $0 : int32 = 10

This instruction declares a variable with ID 0, type int32, and initial value 10.

```
11000000  # Opcode: VAR DECL (110 category, 00000 operation)
00000001  # Operand count: 1
00000006  # Extended data size: 6 bytes
00000000

# Operand 1: Variable reference
11000000  # Type: Variable operand, direct reference
00000000  # Variable ID: 0

# Extended data
00000010  # Type: int32 (type ID 2)
00000000
00001010  # Initial value: 10 (little-endian)
00000000
00000000
00000000
```

## COIL Object Format (COF) Encoding

The COIL Object Format begins with a header followed by a series of tables and section data:

```
+----------------+
| COF Header     |
+----------------+
| Target Table   |
+----------------+
| Section Table  |
+----------------+
| Symbol Table   |
+----------------+
| String Table   |
+----------------+
| Section Data   |
+----------------+
| Relocation Info|
+----------------+
| Variable Info  |
+----------------+
| Type Info      |
+----------------+
```

### COF Header Encoding

The COF header is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Magic Number   | Version        | Flags          | Target Count   |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Section Count  | Symbol Count   | String Size    | Entry Point    |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (8 bytes)      |
+----------------+----------------+----------------+----------------+
| Timestamp      | UUID           | Header Size    | Table Offsets  |
| (8 bytes)      | (16 bytes)     | (4 bytes)      | (16 bytes)     |
+----------------+----------------+----------------+----------------+
```

- **Magic Number**: ASCII string "COIL" (0x434F494C)
- **Version**: Major version (2 bytes) and minor version (2 bytes)
- **Flags**: 32-bit flags field
- **Target Count**: Number of target architecture entries
- **Section Count**: Number of sections
- **Symbol Count**: Number of symbols
- **String Size**: Size of the string table in bytes
- **Entry Point**: Entry point address (if executable)
- **Timestamp**: Creation timestamp (seconds since Unix epoch)
- **UUID**: Unique identifier for the file
- **Header Size**: Size of the header in bytes
- **Table Offsets**: Offsets to various tables (section, symbol, string, target)

### Section Table Entry Encoding

Each section table entry is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Name Offset    | Type           | Flags          | Target ID      |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Address        | Size           | Offset         | Alignment      |
| (8 bytes)      | (8 bytes)      | (8 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Reloc Count    | Reloc Offset   |
| (4 bytes)      | (4 bytes)      |
+----------------+----------------+
```

### Symbol Table Entry Encoding

Each symbol table entry is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Name Offset    | Section Index  | Value          | Size           |
| (4 bytes)      | (4 bytes)      | (8 bytes)      | (8 bytes)      |
+----------------+----------------+----------------+----------------+
| Type           | Flags          | Target ID      |
| (2 bytes)      | (2 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+
```

### String Table Encoding

The string table is a continuous sequence of null-terminated strings. Offsets into the string table are from the beginning of the table.

```
+----------------+----------------+----------------+----------------+
| String 1       | String 2       | ...            | String N       |
| (variable)     | (variable)     |                | (variable)     |
+----------------+----------------+----------------+----------------+
```

## Native Object Format (NOF) Encoding

The Native Object Format is similar to COF but contains additional information for target switching and data mapping:

```
+----------------+
| NOF Header     |
+----------------+
| Target Table   |
+----------------+
| Section Table  |
+----------------+
| Symbol Table   |
+----------------+
| String Table   |
+----------------+
| Target Switch  |
| Table          |
+----------------+
| Section Data   |
+----------------+
| Relocation Info|
+----------------+
| Data Mapping   |
| Table          |
+----------------+
```

### NOF Header Encoding

The NOF header is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Magic Number   | Version        | Flags          | Target Count   |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Section Count  | Symbol Count   | String Size    | Entry Point    |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (8 bytes)      |
+----------------+----------------+----------------+----------------+
| Default Target | Timestamp      | UUID           | Header Size    |
| (4 bytes)      | (8 bytes)      | (16 bytes)     | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Table Offsets  |
| (32 bytes)     |
+----------------+
```

### Target Switch Entry Encoding

Each target switch entry is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Source Address | Source Target  | Destination    | Destination    |
| (8 bytes)      | (4 bytes)      | Address        | Target         |
|                |                | (8 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Flags          | Data Mapping   | Data Mapping   |
| (4 bytes)      | Offset         | Count          |
|                | (4 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+
```

### Data Mapping Entry Encoding

Each data mapping entry is encoded as follows:

```
+----------------+----------------+----------------+----------------+
| Source Symbol  | Destination    | Mapping Type   | Mapping Flags  |
| (4 bytes)      | Symbol         | (4 bytes)      | (4 bytes)      |
|                | (4 bytes)      |                |                |
+----------------+----------------+----------------+----------------+
| Source Offset  | Destination    | Size           | Transform      |
| (8 bytes)      | Offset         | (8 bytes)      | Offset         |
|                | (8 bytes)      |                | (4 bytes)      |
+----------------+----------------+----------------+----------------+
```

## Variable Information Encoding

Variable information is encoded in the variable information section:

```
+----------------+----------------+----------------+----------------+
| Variable ID    | Storage Class  | Type ID        | Flags          |
| (1 byte)       | (1 byte)       | (2 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Lifetime Start | Lifetime End   | Initial Value  | Initial Value  |
| (4 bytes)      | (4 bytes)      | Size           | Data           |
|                |                | (4 bytes)      | (variable)     |
+----------------+----------------+----------------+----------------+
```

## Type Information Encoding

Type information is encoded in the type information section:

```
+----------------+----------------+----------------+----------------+
| Type ID        | Basic Type     | Flags          | Name Offset    |
| (4 bytes)      | (1 byte)       | (1 byte)       | (4 bytes)      |
+----------------+----------------+----------------+----------------+
| Size           | Alignment      | Data Offset    | Data Size      |
| (4 bytes)      | (4 bytes)      | (4 bytes)      | (4 bytes)      |
+----------------+----------------+----------------+----------------+
```

## ABI Definition Encoding

ABI definitions are encoded in the ABI section:

```
+----------------+----------------+----------------+----------------+
| ABI ID         | Name Offset    | Param Model    | Return Model   |
| (4 bytes)      | (4 bytes)      | (1 byte)       | (1 byte)       |
+----------------+----------------+----------------+----------------+
| Stack Align    | Stack Direction| Int Param Regs | FP Param Regs  |
| (1 byte)       | (1 byte)       | (8 bytes)      | (8 bytes)      |
+----------------+----------------+----------------+----------------+
| Vec Param Regs | Int Return Regs| FP Return Regs | Vec Return Regs|
| (8 bytes)      | (2 bytes)      | (2 bytes)      | (2 bytes)      |
+----------------+----------------+----------------+----------------+
| Caller Regs    | Callee Regs    | Name Mangling  | Additional     |
| (4 bytes)      | (4 bytes)      | (1 byte)       | Flags          |
|                |                |                | (3 bytes)      |
+----------------+----------------+----------------+----------------+
```

## Implementation Notes

### Endianness

All multi-byte values in the COIL binary format are stored in little-endian order unless explicitly specified otherwise. This applies to:

- Immediate values
- Addresses
- Offsets
- Sizes
- Counts
- Flags

### Alignment

To ensure efficient memory access, certain data structures are aligned on natural boundaries:

- 64-bit values are aligned on 8-byte boundaries
- 32-bit values are aligned on 4-byte boundaries
- 16-bit values are aligned on 2-byte boundaries

Section data may have additional alignment requirements specified in the section table.

### Padding

Padding bytes (value 0x00) are inserted as needed to maintain proper alignment. Padding should be ignored when reading the binary format.

## Validation Rules

When validating a COIL binary file, the following rules should be applied:

1. **Magic Number**: The magic number must be "COIL" (0x434F494C) for COF files and "NOFX" (0x4E4F4658) for NOF files.

2. **Version Compatibility**: The major version must be compatible with the implementation.

3. **Offset Validity**: All offsets must point to valid locations within the file.

4. **Table Consistency**: The counts and sizes in the header must match the actual tables.

5. **String References**: All string offsets must point to valid null-terminated strings within the string table.

6. **Symbol References**: All symbol references must refer to valid entries in the symbol table.

7. **Section References**: All section references must refer to valid entries in the section table.

8. **Target References**: All target references must refer to valid entries in the target table.

9. **Instruction Validity**: All instructions must have valid opcodes and operand counts.

10. **Operand Validity**: All operands must have valid type bytes and properly encoded data.

## Appendix A: Complete Opcode Encodings

### Control Flow (CF) Operations

```
00000000 - BR     (Unconditional branch)
00000001 - BRC    (Conditional branch)
00000010 - CALL   (Function call)
00000011 - RET    (Return from function)
00000100 - INT    (Software interrupt)
00000101 - IRET   (Return from interrupt)
00000110 - HLT    (Halt execution)
00000111 - SYSC   (System call)
00001000 - TRAP   (Trap to exception handler)
00001001 - WFE    (Wait for event)
00001010 - SEV    (Send event)
00001011 - FENCE  (Memory fence)
00001100 - YIELD  (Yield execution)
00001101 - SWITCH (Switch target architecture)
00001110 - NOP    (No operation)
```

### Memory Operations (MEM)

```
00100000 - MOV      (Move data)
00100001 - PUSH     (Push to stack)
00100010 - POP      (Pop from stack)
00100011 - LOAD     (Load from memory)
00100100 - STORE    (Store to memory)
00100101 - PREFETCH (Prefetch memory)
00100110 - EXCHANGE (Exchange values)
00100111 - COMPARE  (Compare and update flags)
00101000 - TEST     (Test and update flags)
00101001 - FILL     (Fill memory block)
00101010 - COPY     (Copy memory block)
00101011 - ZERO     (Zero memory block)
00101100 - PUSH_STATE (Save register state)
00101101 - POP_STATE  (Restore register state)
00101110 - OUT      (Output to I/O port)
00101111 - IN       (Input from I/O port)
00110000 - LGDT     (Load global descriptor table)
```

### Arithmetic Operations (MATH)

```
01000000 - ADD    (Addition)
01000001 - SUB    (Subtraction)
01000010 - MUL    (Multiplication)
01000011 - DIV    (Division)
01000100 - MOD    (Modulo)
01000101 - NEG    (Negation)
01000110 - INC    (Increment)
01000111 - DEC    (Decrement)
01001000 - ABS    (Absolute value)
01001001 - SQRT   (Square root)
01001010 - MIN    (Minimum)
01001011 - MAX    (Maximum)
01001100 - FMA    (Fused multiply-add)
01001101 - ROUND  (Round)
01001110 - FLOOR  (Floor)
01001111 - CEIL   (Ceiling)
01010000 - TRUNC  (Truncate)
```

### Bit Manipulation (BIT)

```
01100000 - AND    (Bitwise AND)
01100001 - OR     (Bitwise OR)
01100010 - XOR    (Bitwise XOR)
01100011 - NOT    (Bitwise NOT)
01100100 - ANDN   (Bitwise AND-NOT)
01100101 - ORN    (Bitwise OR-NOT)
01100110 - XNOR   (Bitwise XNOR)
01100111 - SHL    (Shift left logical)
01101000 - SHR    (Shift right logical)
01101001 - SAR    (Shift arithmetic right)
01101010 - ROL    (Rotate left)
01101011 - ROR    (Rotate right)
01101100 - RCL    (Rotate left through carry)
01101101 - RCR    (Rotate right through carry)
01101110 - BSWAP  (Byte swap)
01101111 - BITREV (Bit reverse)
01110000 - CLZ    (Count leading zeros)
01110001 - CTZ    (Count trailing zeros)
01110010 - POPCNT (Population count)
01110011 - PARITY (Parity)
01110100 - EXTRACT (Bit field extract)
01110101 - INSERT (Bit field insert)
01110110 - SET    (Set bit)
01110111 - CLR    (Clear bit)
01111000 - TST    (Test bit)
01111001 - TGL    (Toggle bit)
01111010 - CMP    (Compare values and set flags)
```

### Vector Operations (VEC)

```
10000000 - MOV      (Vector move)
10000001 - ADD      (Vector add)
10000010 - SUB      (Vector subtract)
10000011 - MUL      (Vector multiply)
10000100 - DIV      (Vector divide)
10000101 - MIN      (Vector minimum)
10000110 - MAX      (Vector maximum)
10000111 - AND      (Vector bitwise AND)
10001000 - OR       (Vector bitwise OR)
10001001 - XOR      (Vector bitwise XOR)
10001010 - NOT      (Vector bitwise NOT)
10001011 - SHL      (Vector shift left)
10001100 - SHR      (Vector shift right)
10001101 - CMP      (Vector compare)
10001110 - ABS      (Vector absolute value)
10001111 - SQRT     (Vector square root)
10010000 - FMA      (Vector fused multiply-add)
10010001 - ROUND    (Vector round)
10010010 - INSERT   (Insert element into vector)
10010011 - EXTRACT  (Extract element from vector)
10010100 - SHUFFLE  (Shuffle vector elements)
10010101 - BROADCAST (Broadcast scalar to vector)
10010110 - PACK     (Pack elements with saturation)
10010111 - UNPACK   (Unpack elements)
10011000 - DOT      (Vector dot product)
10011001 - HADD     (Horizontal addition)
10011010 - HSUB     (Horizontal subtraction)
10011011 - GATHER   (Gather elements)
10011100 - SCATTER  (Scatter elements)
10011101 - BLEND    (Blend vectors based on mask)
10011110 - COMPRESS (Compress vector using mask)
10011111 - EXPAND   (Expand vector using mask)
10100000 - ZERO     (Zero vector)
```

### Atomic Operations (ATM)

```
10100000 - ADD        (Atomic add)
10100001 - SUB        (Atomic subtract)
10100010 - AND        (Atomic AND)
10100011 - OR         (Atomic OR)
10100100 - XOR        (Atomic XOR)
10100101 - NAND       (Atomic NAND)
10100110 - XCHG       (Atomic exchange)
10100111 - CAS        (Atomic compare-and-swap)
10101000 - FETCH_ADD  (Atomic fetch and add)
10101001 - FETCH_SUB  (Atomic fetch and subtract)
10101010 - FETCH_AND  (Atomic fetch and AND)
10101011 - FETCH_OR   (Atomic fetch and OR)
10101100 - FETCH_XOR  (Atomic fetch and XOR)
10101101 - FETCH_NAND (Atomic fetch and NAND)
```

### Variable Management (VAR)

```
11000000 - DECL   (Variable declaration)
11000001 - PMT    (Make variable permanent)
11000010 - DMT    (Destroy variable)
11000011 - DLT    (Delete variable)
11000100 - ALIAS  (Create variable alias)
```

### Frame Management (FRAME)

```
11100000 - ENTER  (Enter function frame)
11100001 - LEAVE  (Leave function frame)
11100010 - SAVE   (Save registers)
11100011 - REST   (Restore registers)
```

## Appendix B: Binary Format Structure Tables

### COF Binary Format Structure

| Component       | Offset           | Size              | Description                           |
|-----------------|------------------|-------------------|---------------------------------------|
| Header          | 0                | 64 bytes          | File header with metadata             |
| Target Table    | header.target_offset | target_count * 24 bytes | Target architecture definitions |
| Section Table   | header.section_offset | section_count * 40 bytes | Section definitions          |
| Symbol Table    | header.symbol_offset | symbol_count * 32 bytes | Symbol definitions           |
| String Table    | header.string_offset | string_table_size | Null-terminated strings          |
| Section Data    | Variable         | Variable          | Actual section contents              |
| Relocation Info | Variable         | Variable          | Relocation records                   |
| Variable Info   | Variable         | Variable          | Variable metadata                    |
| Type Info       | Variable         | Variable          | Type definitions                      |

### NOF Binary Format Structure

| Component       | Offset           | Size              | Description                           |
|-----------------|------------------|-------------------|---------------------------------------|
| Header          | 0                | 88 bytes          | File header with metadata             |
| Target Table    | header.target_offset | target_count * 36 bytes | Target architecture definitions |
| Section Table   | header.section_offset | section_count * 48 bytes | Section definitions          |
| Symbol Table    | header.symbol_offset | symbol_count * 32 bytes | Symbol definitions           |
| String Table    | header.string_offset | string_table_size | Null-terminated strings          |
| Target Switch Table | header.switch_offset | switch_count * 28 bytes | Target switch definitions   |
| Section Data    | Variable         | Variable          | Actual section contents              |
| Relocation Info | Variable         | Variable          | Relocation records                   |
| Data Mapping Table | header.mapping_offset | mapping_count * 44 bytes | Data mapping definitions  |

## Appendix C: Full Binary Example

Here is a complete encoding example for a simple "Hello, World!" program:

```
# COF Header (64 bytes)
434F494C  # Magic "COIL"
00010000  # Version 1.0
00000000  # Flags
00000001  # 1 target
00000002  # 2 sections
00000003  # 3 symbols
00000020  # String table size (32 bytes)
0000000000000000  # Entry point
0000000060C25F12  # Timestamp
00000000000000000000000000000000  # UUID
00000040  # Header size (64 bytes)
00000098  # Section table offset
000000C0  # Symbol table offset
000000E0  # String table offset
00000060  # Target table offset

# Target Table (24 bytes)
00000001  # Target ID
00000001  # Architecture type (x86-64)
0000000F  # Features
00000000  # Name offset in string table
00000060  # Configuration offset
00000020  # Configuration size

# Section Table (80 bytes)
# Section 1 (.text)
00000000  # Name offset (".text")
00000001  # Type (code)
00000001  # Flags (EXEC)
00000001  # Target ID
0000000000000000  # Address
0000000000000140  # Size
0000000000000100  # Offset
00000004  # Alignment
00000000  # No relocations
00000000  # Relocation offset

# Section 2 (.data)
00000006  # Name offset (".data")
00000002  # Type (data)
00000001  # Flags (WRITE)
00000001  # Target ID
0000000000000000  # Address
0000000000000019  # Size
0000000000000240  # Offset
00000004  # Alignment
00000000  # No relocations
00000000  # Relocation offset

# Symbol Table (96 bytes)
# Symbol 1 (.text)
00000000  # Name offset (".text")
00000000  # Section index
0000000000000000  # Value
0000000000000140  # Size
0003  # Type (section)
0000  # Flags
00000001  # Target ID

# Symbol 2 (.data)
00000006  # Name offset (".data")
00000001  # Section index
0000000000000000  # Value
0000000000000019  # Size
0003  # Type (section)
0000  # Flags
00000001  # Target ID

# Symbol 3 (main)
0000000C  # Name offset ("main")
00000000  # Section index
0000000000000000  # Value
0000000000000026  # Size
0001  # Type (function)
0001  # Flags (global)
00000001  # Target ID

# String Table (32 bytes)
2E74657874005F646174610066756E6374696F6E00
# ".text", ".data", "function" as null-terminated strings

# Section Data (.text) - Simple instructions
# DIR HINT main FUNC GLOBAL
# DIR LABEL main
#   FRAME ENTER
#   CF SYSC abi-linux-x86_64 (0x01, 1, hw_str, hw_len) -> ($0)
#   VAR DLT $0
#   FRAME LEAVE
#   CF SYSC abi-linux-x86_64 (0x3C, 0) -> ()
# DIR HINT main ENDFUNC

# Followed by .data section with "Hello, World!" string
```

This example illustrates a complete binary encoding for a simple COIL program that outputs "Hello, World!" using system calls.

## Conclusion

The COIL Binary Encoding Reference provides a detailed specification of the binary representation of COIL instructions, operands, and data structures. This document serves as the definitive reference for implementers of COIL toolchains, ensuring interoperability and correctness across different implementations.

By following this specification, developers can create tools that correctly encode and decode COIL binaries, enabling the development of a consistent and robust COIL ecosystem.