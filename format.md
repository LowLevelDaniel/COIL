# COIL Binary Format

This document specifies the binary encoding for COIL, designed for space efficiency and parsing optimization.

## 1. File Structure

A COIL binary file consists of the following sections, in order:

1. **Header**: File identification and version information
2. **Metadata**: Module information and compilation details
3. **Type Section**: Definitions of types used in the module
4. **Global Section**: Global variable declarations
5. **Import Section**: Imported modules and symbols
6. **Export Section**: Exported symbols
7. **Procedure Section**: Procedure definitions
8. **Data Section**: Initialized data
9. **Relocation Section**: Relocation information for linking
10. **Debug Section**: Optional debugging information

## 2. Section Details

### 2.1 Header

The header is a fixed-size section that identifies the file as a COIL binary:

```
+---------------+---------------+---------------+---------------+
| 'C' (8 bits)  | 'O' (8 bits)  | 'I' (8 bits)  | 'L' (8 bits)  |
+---------------+---------------+---------------+---------------+
| Version Major | Version Minor | Version Patch | Flags (8 bits)|
+---------------+---------------+---------------+---------------+
```

- **Signature**: The ASCII string "COIL" (0x434F494C)
- **Version**: Three 8-bit fields for major, minor, and patch version
- **Flags**: 8 bits of file-level flags
  - Bit 0: Debug information present
  - Bit 1: Big-endian encoding (default is little-endian)
  - Bit 2: Contains relocatable code
  - Bit 3: Contains executable code
  - Bits 4-7: Reserved for future use

### 2.2 Metadata

The metadata section contains variable-length information about the module:

```
+----------------------+----------------------+
| Metadata Count (16b) | Reserved (16 bits)   |
+----------------------+----------------------+
| Key 1 Length (16b)   | Value 1 Length (16b) |
+----------------------+----------------------+
| Key 1 Data (variable)                       |
+--------------------------------------------->
| Value 1 Data (variable)                     |
+--------------------------------------------->
| Key 2 Length (16b)   | Value 2 Length (16b) |
+----------------------+----------------------+
| ...                                         |
```

Common metadata keys include:
- "name": Module name
- "compiler": Compiler identification
- "created": Timestamp
- "target": Target architecture
- "optimize": Optimization level

### 2.3 Type Section

The type section defines all types used in the module:

```
+--------------------+--------------------+
| Type Count (32b)   | Reserved (32 bits) |
+--------------------+--------------------+
| Type 1 ID (32b)    | Type 1 Kind (8b)   |
+--------------------+--------------------+
| Type 1 Data (variable)                  |
+------------------------------------------+
| Type 2 ID (32b)    | Type 2 Kind (8b)   |
+--------------------+--------------------+
| ...                                     |
```

Type kinds include:
- 0: Void
- 1: Integer
- 2: Floating-point
- 3: Vector
- 4: Pointer
- 5: Array
- 6: Structure
- 7: Function

Each type kind has a specific data format for its additional information.

### 2.4 Global Section

The global section declares global variables:

```
+--------------------+--------------------+
| Global Count (32b) | Reserved (32 bits) |
+--------------------+--------------------+
| Global 1 ID (32b)  | Type ID (32b)      |
+--------------------+--------------------+
| Flags (8b)         | Initial Value Offset (32b) |
+--------------------+--------------------+
| Global 2 ID (32b)  | Type ID (32b)      |
+--------------------+--------------------+
| ...                                     |
```

Global flags include:
- Bit 0: Mutable
- Bit 1: External
- Bit 2: Thread-local
- Bits 3-7: Reserved

The initial value offset points to the data section, or is 0 if the global is uninitialized.

### 2.5 Import Section

The import section specifies imported symbols:

```
+--------------------+--------------------+
| Import Count (32b) | Reserved (32 bits) |
+--------------------+--------------------+
| Module Name Length (16b) | Name Length (16b) |
+--------------------+--------------------+
| Module Name (variable)                  |
+------------------------------------------+
| Symbol Name (variable)                  |
+------------------------------------------+
| Symbol Kind (8b)    | Symbol ID (32b)   |
+--------------------+--------------------+
| ...                                     |
```

Symbol kinds include:
- 0: Global variable
- 1: Procedure
- 2: Memory
- 3: Table

### 2.6 Export Section

The export section specifies exported symbols:

```
+--------------------+--------------------+
| Export Count (32b) | Reserved (32 bits) |
+--------------------+--------------------+
| Name Length (16b)  | Symbol Kind (8b)   |
+--------------------+--------------------+
| Symbol ID (32b)    | Name (variable)    |
+--------------------+--------------------+
| ...                                     |
```

Symbol kinds are the same as in the import section.

### 2.7 Procedure Section

The procedure section contains the code for procedures:

```
+------------------------+------------------------+
| Procedure Count (32b)  | Reserved (32 bits)     |
+------------------------+------------------------+
| Procedure 1 ID (32b)   | Type ID (32b)          |
+------------------------+------------------------+
| Flags (8b)             | Local Count (16b)      |
+------------------------+------------------------+
| Code Size (32b)        | Local Types (variable) |
+------------------------+------------------------+
| Code (variable)                                 |
+---------------------------------------------------+
| Procedure 2 ID (32b)   | ...                    |
+------------------------+------------------------+
| ...                                             |
```

Procedure flags include:
- Bit 0: External
- Bit 1: NoReturn
- Bit 2: Pure
- Bit 3: Inline hint
- Bit 4: NoInline hint
- Bits 5-7: Reserved

The local types are a sequence of type IDs for local variables.

### 2.8 Data Section

The data section contains initialized data:

```
+--------------------+--------------------+
| Data Count (32b)   | Reserved (32 bits) |
+--------------------+--------------------+
| Segment 1 Size (32b) | Alignment (8b)   |
+--------------------+--------------------+
| Flags (8b)         | Reserved (16b)     |
+--------------------+--------------------+
| Data (variable)                         |
+------------------------------------------+
| Segment 2 Size (32b) | ...              |
+--------------------+--------------------+
| ...                                     |
```

Data segment flags include:
- Bit 0: Read
- Bit 1: Write
- Bit 2: Execute
- Bit 3: Shared
- Bits 4-7: Reserved

### 2.9 Relocation Section

The relocation section contains information for resolving addresses during linking:

```
+------------------------+------------------------+
| Relocation Count (32b) | Reserved (32 bits)     |
+------------------------+------------------------+
| Relocation 1 Offset (32b) | Type (8b)          |
+------------------------+------------------------+
| Symbol ID (32b)        | Addend (32b)          |
+------------------------+------------------------+
| Relocation 2 Offset (32b) | ...                |
+------------------------+------------------------+
| ...                                             |
```

Relocation types include:
- 0: Absolute 32-bit
- 1: Absolute 64-bit
- 2: Relative 32-bit
- 3: Relative 64-bit
- 4: PC-relative 32-bit
- 5: PC-relative 64-bit

### 2.10 Debug Section

The debug section contains optional debugging information:

```
+--------------------+--------------------+
| Debug Info Size (32b) | Reserved (32b)  |
+--------------------+--------------------+
| Debug Info Format (8b) | Reserved (24b) |
+--------------------+--------------------+
| Debug Info (variable)                   |
+------------------------------------------+
```

Debug info format can be:
- 0: DWARF
- 1: COIL Debug Format
- 2-255: Reserved

## 3. Instruction Encoding

Instructions within the code section of procedures are encoded as described in the [Instruction Set](instruction-set.md) document. Here's a summary:

```
+---------------+----------------+----------------+------------------+
| Primary       | Extended       | Type & Operand | Operand Data     |
| Opcode (8b)   | Opcode (0-8b)  | Info (8-16b)   | (Variable)       |
+---------------+----------------+----------------+------------------+
```

### 3.1 Primary Opcode Ranges

The primary opcode space is divided into ranges for different instruction categories:

- 0x00-0x1F: Control flow operations
- 0x20-0x3F: Memory operations
- 0x40-0x5F: Integer arithmetic operations
- 0x60-0x7F: Floating-point operations
- 0x80-0x9F: Vector operations
- 0xA0-0xBF: Conversion operations
- 0xC0-0xDF: Synchronization operations
- 0xE0-0xFF: Special and extended operations

### 3.2 Extended Opcode

The extended opcode provides additional information about the instruction:

- For arithmetic operations, it specifies the exact operation (add, subtract, etc.)
- For memory operations, it specifies the access type (load, store, etc.)
- For control flow, it specifies the branch condition or jump type

### 3.3 Type and Operand Information

This field encodes the data types and operand kinds:

- Bits 0-2: Operand count
- Bits 3-5: Base type (integer, float, vector, etc.)
- Bits 6-9: Type width code (8-bit, 16-bit, 32-bit, etc.)
- Bits 10-15: Type modifiers and additional information

### 3.4 Operand Data

Operands are encoded based on their kind:

1. **Register Variable**:
   ```
   +---------------+
   | 00 | Reg ID   |
   +---------------+
   ```

2. **Stack Variable**:
   ```
   +---------------+---------------+
   | 01 | Reserved | Stack Offset  |
   +---------------+---------------+
   ```

3. **Immediate Value**:
   ```
   +---------------+---------------------+
   | 10 | Size     | Immediate Data      |
   +---------------+---------------------+
   ```

4. **Symbol Reference**:
   ```
   +---------------+---------------+
   | 11 | Kind     | Symbol ID     |
   +---------------+---------------+
   ```

## 4. Variable-Length Encoding

COIL uses variable-length encoding for integer values to save space:

- Values 0-127: Single byte encoding (0xxxxxxx)
- Values 128-16383: Two byte encoding (10xxxxxx xxxxxxxx)
- Values 16384-2097151: Three byte encoding (110xxxxx xxxxxxxx xxxxxxxx)
- Values 2097152-268435455: Four byte encoding (1110xxxx xxxxxxxx xxxxxxxx xxxxxxxx)
- Larger values: Five+ byte encoding (1111xxxx... followed by 4+ bytes)

## 5. String Encoding

Strings are encoded with a length prefix followed by UTF-8 encoded bytes:

```
+--------------------+---------------------------+
| Length (VLE)       | UTF-8 String Data         |
+--------------------+---------------------------+
```

## 6. Alignment

- The header is aligned to 8 bytes
- Each section begins on a 4-byte boundary
- Multi-byte values are aligned according to their natural alignment unless specified otherwise
- Instructions begin on byte boundaries without special alignment

## 7. Endianness

By default, COIL binaries use little-endian encoding for multi-byte values. The endianness can be changed using the header flag.

## 8. Extensibility

The COIL binary format includes several provisions for future extensions:

- Reserved bits in section headers
- Extended opcode space for additional instructions
- Version information in the header
- Format ID fields for alternative encoding of debug information

Implementers should ignore unrecognized fields or values to maintain forward compatibility.