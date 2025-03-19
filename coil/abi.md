# Application Binary Interface (ABI) Binary Format (Version 0.1.0)

The COIL ABI (Application Binary Interface) defines binary structures within the COIL Object Format (COF) that specify calling conventions, parameter passing, and register usage. This document details the binary representation of ABI definitions and their application in COIL binaries.

## Binary Structure

ABI definitions in COIL are represented as a series of binary structures that appear within:

1. **Configuration files**: As part of the configuration format
2. **COIL Object files**: Through ABI directives in the binary

## ABI Definition in Configuration Files

Within COIL configuration files (.coilcfg), ABI definitions use the following binary format:

```c
struct coil_abi_entry {
    uint32_t name_offset;   // Offset in string table for ABI name
    uint16_t arg_count;     // Maximum number of arguments
    uint16_t flags;         // ABI flags
    uint32_t mapping_offset; // Offset to register mapping table
    uint32_t mapping_count;  // Number of mappings
    uint8_t  padding[4];     // Padding to align to 16 bytes
};
```

ABI flags are encoded as bit flags:
```c
enum abi_flags {
    ABI_FLAG_VARIADIC      = (1 << 0),  // Supports variadic functions
    ABI_FLAG_PRESERVES_SP  = (1 << 1),  // Preserves stack pointer
    ABI_FLAG_CALLER_CLEANUP = (1 << 2),  // Caller cleans up stack
    ABI_FLAG_USES_RED_ZONE = (1 << 3),  // Uses red zone below stack
    // 4-15 reserved
};
```

### Register Mapping Table

Each ABI includes a register mapping table that defines how arguments and return values are mapped to registers:

```c
struct coil_reg_mapping {
    uint16_t arg_index;     // Argument index (0 = return value)
    uint16_t reg_type;      // Register type
    uint32_t reg_index;     // Register index
    uint32_t reg_mask;      // Register mask (for partial registers)
    uint8_t  padding[4];    // Padding to align to 16 bytes
};
```

Register types are encoded as:
```c
enum reg_type {
    REG_TYPE_GPR   = 0,  // General purpose register
    REG_TYPE_FPR   = 1,  // Floating-point register
    REG_TYPE_VR    = 2,  // Vector register
    REG_TYPE_SPR   = 3,  // Special purpose register
    REG_TYPE_STACK = 4,  // Stack location
    // 5-65535 reserved
};
```

Special argument indexes:
```c
// Special argument indexes
#define ARG_INDEX_RETURN    0x0000  // Return value
#define ARG_INDEX_THIS      0xFFF0  // 'this' pointer in object-oriented calls
#define ARG_INDEX_CONTEXT   0xFFF1  // Context pointer for languages like Go
#define ARG_INDEX_VARARG    0xFFF2  // Start of variadic arguments
#define ARG_INDEX_FLOAT_BASE 0x8000  // Base for floating-point argument indexes
```

## ABI Directive in COIL Binary

Within a COIL binary, ABI definitions are represented using the ABI directive:

```
[0xD6 (DIR_OPCODE_ABI)][0x00 (begin_abi)][length: uint16_t][name_length: uint8_t][name: char[]]
  [0xD6][0x01 (abi_arg)][length: uint16_t][arg_index: uint16_t][reg_type: uint8_t][reg_index: uint8_t]
  ...
[0xD6][0x02 (end_abi)][0x00 0x00]
```

Followed by individual argument mappings and terminated by an end marker.

Example binary representation for a simple ABI:
```
D6 00 07 00 06 6D 79 2D 61 62 69  // Begin ABI "my-abi"
D6 01 04 00 00 00 00 00            // Return value in GPR 0
D6 01 04 00 01 00 01 00            // First argument in GPR 1
D6 01 04 00 02 00 02 00            // Second argument in GPR 2
D6 02 00 00                        // End ABI
```

## ABI Reference in Instructions

COIL instructions can reference ABIs through special opcodes or qualifiers. For function calls, the binary format is:

```
[CALL opcode][ABI qualifier][target operand]
```

Where the ABI qualifier byte encodes:
```c
// For call instructions
#define ABI_QUAL_DEFAULT   0x00  // Default ABI
#define ABI_QUAL_NAMED     0x01  // Named ABI follows (ABI name as operand)
#define ABI_QUAL_INDEX     0x02  // ABI index follows (uint16_t)
```

Example binary for a call using a named ABI:
```
04 01 [target operand] [ABI name operand]
```

For system calls, a similar pattern is used:
```
[SYSC opcode][ABI qualifier][syscall number operand][arguments...]
```

## Standard ABI Binary Representations

COIL defines several standard ABIs with their binary representations:

### System V AMD64 ABI

Used for Linux, Unix, and macOS on x86-64:

```
// Binary encoding in configuration file
struct coil_abi_entry system_v_x64 = {
    .name_offset = 0x10,    // Offset to "system_v_x64" in string table
    .arg_count = 6,         // 6 register arguments
    .flags = 0x05,          // ABI_FLAG_PRESERVES_SP | ABI_FLAG_CALLER_CLEANUP
    .mapping_offset = 0x50, // Offset to mapping table
    .mapping_count = 14,    // 14 mappings (6 integer, 8 floating-point)
    .padding = {0, 0, 0, 0}
};

// First few entries in mapping table
struct coil_reg_mapping system_v_x64_mappings[] = {
    {0x0000, 0, 0, 0xFFFFFFFF}, // Return in RAX (RQ0)
    {0x0001, 0, 4, 0xFFFFFFFF}, // First integer arg in RDI (RQ4)
    {0x0002, 0, 5, 0xFFFFFFFF}, // Second integer arg in RSI (RQ5)
    // ...more mappings...
};
```

### Microsoft x64 ABI

Used for Windows on x86-64:

```
// Binary encoding in configuration file
struct coil_abi_entry windows_x64 = {
    .name_offset = 0x30,    // Offset to "windows_x64" in string table
    .arg_count = 4,         // 4 register arguments
    .flags = 0x09,          // ABI_FLAG_PRESERVES_SP | ABI_FLAG_USES_RED_ZONE
    .mapping_offset = 0x150,// Offset to mapping table
    .mapping_count = 10,    // 10 mappings (4 integer, 4 floating-point, etc.)
    .padding = {0, 0, 0, 0}
};

// First few entries in mapping table
struct coil_reg_mapping windows_x64_mappings[] = {
    {0x0000, 0, 0, 0xFFFFFFFF}, // Return in RAX (RQ0)
    {0x0001, 0, 2, 0xFFFFFFFF}, // First integer arg in RCX (RQ2)
    {0x0002, 0, 3, 0xFFFFFFFF}, // Second integer arg in RDX (RQ3)
    // ...more mappings...
};
```

## ABI Storage in COIL Object Format

In a COIL Object Format (COF) file, ABI definitions are stored in:

1. **Metadata sections**: Dedicated sections for ABI definitions
2. **String table**: ABI names and related strings
3. **Instruction stream**: References to ABIs in call instructions

The metadata section for ABIs has the format:
```
[Section Header (type=COF_SECTION_METADATA, subtype=META_SUBTYPE_ABI)]
[Number of ABIs: uint32_t]
[ABI entry 1]
[ABI entry 2]
...
[ABI entry N]
[Mapping tables...]
```

## ABI and Virtual Register Integration

The ABI system directly integrates with the COIL virtual register system by specifying the mapping between function arguments and virtual registers. The COIL assembler uses this information to:

1. Generate correct register assignments for function calls
2. Preserve necessary registers according to ABI rules
3. Manage stack frame layout for arguments that don't fit in registers
4. Handle return values appropriately

## Binary Example

Complete binary example of an ABI definition in a COIL configuration file:

```
// Offset 0x000: ABI entry for System V x86-64
00 00 00 10  // name_offset = 0x10 (offset to "system_v_x64")
06 00        // arg_count = 6
05 00        // flags = 0x05 (ABI_FLAG_PRESERVES_SP | ABI_FLAG_CALLER_CLEANUP)
50 00 00 00  // mapping_offset = 0x50
0E 00 00 00  // mapping_count = 14
00 00 00 00  // padding

// Offset 0x010: String "system_v_x64\0"
73 79 73 74 65 6D 5F 76 5F 78 36 34 00

// Offset 0x050: Mapping table (first few entries)
00 00        // arg_index = 0 (return value)
00 00        // reg_type = 0 (GPR)
00 00 00 00  // reg_index = 0 (RAX/RQ0)
FF FF FF FF  // reg_mask = 0xFFFFFFFF (full register)
00 00 00 00  // padding

01 00        // arg_index = 1 (first argument)
00 00        // reg_type = 0 (GPR)
04 00 00 00  // reg_index = 4 (RDI/RQ4)
FF FF FF FF  // reg_mask = 0xFFFFFFFF (full register)
00 00 00 00  // padding

// ...more mappings...
```

## COIL Explanatory Language (CEL) Representation

For documentation purposes, ABIs can be represented in COIL Explanatory Language (CEL). However, it's important to remember that CEL is not COIL itself; COIL is always a binary format.

CEL representation of an ABI:
```
abi_def "system_v_x64"
  arg 0, RQ0      # Return value in RAX (RQ0)
  arg 1, RQ4      # First integer argument in RDI (RQ4)
  arg 2, RQ5      # Second integer argument in RSI (RQ5)
  # ...more mappings...
end_abi
```

## Implementation Requirements

COIL assemblers must:

1. Correctly parse and interpret ABI binary structures
2. Apply ABI rules when generating native code for function calls
3. Support all standard ABIs for their target platforms
4. Allow defining custom ABIs through configuration or directives
5. Handle stack management for arguments exceeding register capacity

## Future Extensions

Future versions of COIL will extend the ABI binary format to include:

1. More detailed control over stack frame layout
2. Enhanced support for complex return values
3. Exception handling information
4. Thread-local storage specifications
5. Support for more specialized calling conventions

These extensions will maintain backward compatibility with the format defined here.