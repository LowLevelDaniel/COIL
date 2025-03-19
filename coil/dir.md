# Directives Binary Format (Version 1.0.0)

COIL directives are binary structures within the COIL Object Format (COF) that control aspects of the assembly process. Unlike instructions, directives do not translate directly to executable operations but instead guide the COIL processor in processing and transforming the binary code.

**IMPORTANT NOTE:** This document is part of the COIL specification documentation. It does not contain implementation code, but rather describes how the directive binary format should function when implemented.

## Binary Structure

Directives in COIL binary format follow a general structure:

```
[directive_opcode: uint8_t][qualifier: uint8_t][length: uint16_t][payload: variable]
```

Where:
- **directive_opcode**: A byte value identifying the directive (values 0xD0-0xDF reserved for directives)
- **qualifier**: A byte providing additional information about the directive
- **length**: A 16-bit value indicating the length of the payload in bytes
- **payload**: Variable-length data specific to the directive

## Directive Opcodes

COIL reserves the following opcode ranges:

```c
// Directive Opcodes (0xD0-0xDF)
enum directive_opcode : uint8_t {
    DIR_OPCODE_VERSION   = 0xD0, // Version specification
    DIR_OPCODE_TARGET    = 0xD1, // Target architecture
    DIR_OPCODE_SECTION   = 0xD2, // Section definition
    DIR_OPCODE_SYMBOL    = 0xD3, // Symbol definition
    DIR_OPCODE_ALIGN     = 0xD4, // Alignment directive
    DIR_OPCODE_DATA      = 0xD5, // Data definition
    DIR_OPCODE_ABI       = 0xD6, // ABI definition
    DIR_OPCODE_FEATURE   = 0xD7, // Feature control
    DIR_OPCODE_OPTIMIZE  = 0xD8, // Optimization control
    DIR_OPCODE_CONDITION = 0xD9, // Conditional assembly
    DIR_OPCODE_MACRO     = 0xDA, // Macro definition
    DIR_OPCODE_INCLUDE   = 0xDB, // Include other COIL binary
    // 0xDC-0xDF reserved for future standard directives
};
```

## Directive Qualifiers

Each directive type has specific qualifier values:

```c
// Section directive qualifiers
enum section_qualifier : uint8_t {
    SECTION_TEXT    = 0x01, // Text (code) section
    SECTION_DATA    = 0x02, // Data section
    SECTION_RODATA  = 0x03, // Read-only data section
    SECTION_BSS     = 0x04, // Uninitialized data section
    SECTION_CUSTOM  = 0xFF, // Custom section (name in payload)
};

// Symbol directive qualifiers
enum symbol_qualifier : uint8_t {
    SYMBOL_LOCAL    = 0x01, // Local symbol
    SYMBOL_GLOBAL   = 0x02, // Global symbol
    SYMBOL_WEAK     = 0x03, // Weak symbol
    SYMBOL_EXTERN   = 0x04, // External symbol reference
};

// Data directive qualifiers (specifies data type)
enum data_qualifier : uint8_t {
    DATA_BYTE    = 0x01, // 8-bit values
    DATA_WORD    = 0x02, // 16-bit values
    DATA_LONG    = 0x03, // 32-bit values
    DATA_QUAD    = 0x04, // 64-bit values
    DATA_FLOAT   = 0x05, // 32-bit float values
    DATA_DOUBLE  = 0x06, // 64-bit float values
    DATA_STRING  = 0x07, // Null-terminated string
    DATA_BYTES   = 0x08, // Raw byte sequence
};
```

## Detailed Directive Formats

### Version Directive

Specifies the COIL version targeted by the binary.

Binary format:
```
[0xD0 (DIR_OPCODE_VERSION)][0x00 (reserved)][0x03 0x00 (length)][major: uint8_t][minor: uint8_t][patch: uint8_t]
```

Example (for version 1.0.0):
```
D0 00 03 00 01 00 00
```

CEL representation (for documentation):
```
.version 1.0.0
```

### Target Directive

Specifies the target architecture.

Binary format:
```
[0xD1 (DIR_OPCODE_TARGET)][0x00 (reserved)][length: uint16_t][target_id: uint16_t]
```

Target IDs:
```c
enum target_id : uint16_t {
    TARGET_ANY     = 0x0000, // Generic, architecture-independent
    TARGET_X86     = 0x0001, // x86 (32-bit)
    TARGET_X86_64  = 0x0002, // x86-64 (64-bit)
    TARGET_ARM     = 0x0003, // ARM (32-bit)
    TARGET_ARM64   = 0x0004, // ARM64 (64-bit)
    TARGET_RISCV32 = 0x0005, // RISC-V (32-bit)
    TARGET_RISCV64 = 0x0006, // RISC-V (64-bit)
    // 0x0007-0xFFFF reserved for future architectures
};
```

Example (for x86-64):
```
D1 00 02 00 02 00
```

CEL representation:
```
.target x86-64
```

### Section Directive

Defines a section in the COIL binary.

Binary format:
```
[0xD2 (DIR_OPCODE_SECTION)][qualifier: uint8_t][length: uint16_t][section_data]
```

For custom sections:
```
[0xD2][0xFF (SECTION_CUSTOM)][length: uint16_t][name_length: uint8_t][name: char[])[flags: uint8_t]
```

Example (for text section):
```
D2 01 01 00 01
```

CEL representation:
```
.section .text, "x"
```

### Symbol Directive

Defines a symbol in the COIL binary.

Binary format:
```
[0xD3 (DIR_OPCODE_SYMBOL)][qualifier: uint8_t][length: uint16_t][name_length: uint8_t][name: char[]][value: uint64_t (optional)]
```

Example (global symbol "main"):
```
D3 02 05 00 04 6D 61 69 6E
```

CEL representation:
```
.global main
```

### Data Directive

Defines data in the current section.

Binary format:
```
[0xD5 (DIR_OPCODE_DATA)][data_type: uint8_t][length: uint16_t][data_payload]
```

Example (bytes 0x12, 0x34, 0x56):
```
D5 01 03 00 12 34 56
```

CEL representation:
```
.byte 0x12, 0x34, 0x56
```

### ABI Definition Directive

Defines an Application Binary Interface.

Binary format:
```
[0xD6 (DIR_OPCODE_ABI)][0x00 (begin_abi)][length: uint16_t][name_length: uint8_t][name: char[]]
  [0xD6][0x01 (abi_arg)][length: uint16_t][arg_index: uint8_t][reg_type: uint8_t][reg_index: uint8_t]
  ...
[0xD6][0x02 (end_abi)][0x00 0x00]
```

Example (simple ABI with 2 argument mappings):
```
D6 00 07 00 06 6D 79 2D 61 62 69 // Start ABI "my-abi"
D6 01 03 00 00 00 00 // Return value in register 0
D6 01 03 00 01 00 01 // First argument in register 1
D6 02 00 00 // End ABI
```

CEL representation:
```
.abi_def "my-abi"
  .arg 0, RQ0  // Return value
  .arg 1, RQ1  // First argument
.end_abi
```

### Feature Control Directive

Enables or disables architecture-specific features.

Binary format:
```
[0xD7 (DIR_OPCODE_FEATURE)][feature_state: uint8_t][length: uint16_t][feature_id: uint16_t]
```

Where feature_state is 0x00 for disable, 0x01 for enable.

Example (enable AVX2, feature ID 0x0102):
```
D7 01 02 00 02 01
```

CEL representation:
```
.feature avx2, on
```

### Optimization Control Directive

Sets the optimization level for the processor.

Binary format:
```
[0xD8 (DIR_OPCODE_OPTIMIZE)][0x00 (reserved)][0x01 0x00 (length)][level: uint8_t]
```

Example (optimization level 2):
```
D8 00 01 00 02
```

CEL representation:
```
.optimize 2
```

## Directives in the COIL Object Format

Within a COIL object file (.cof), directives are stored in:

1. **Header information**: Certain directives (like version and target) affect the COF header
2. **Special sections**: Some directives create or modify sections
3. **Code sections**: Directives that affect code generation are interleaved with instructions
4. **Metadata sections**: Complex directives like ABI definitions may be stored in dedicated metadata sections

## Relationship to Instructions

In the COIL binary format, directives and instructions are distinguished by their opcode ranges:

1. **Instructions**: Use opcode range 0x00-0xCF
2. **Directives**: Use opcode range 0xD0-0xDF
3. **Extension area**: 0xE0-0xFF reserved for implementation-specific extensions

This clear separation allows the COIL processor to identify and process directives differently from instructions during the binary translation process.

## Implementation Requirements

COIL processors must:

1. Recognize all standard directive binary formats
2. Process directives correctly according to their defined semantics
3. Validate directive payloads and report appropriate errors
4. Apply directive effects appropriately during the assembly process
5. Preserve directive information where needed in debugging sections of NCOF

## COIL Explanatory Language (CEL) Representation

While the above describes the actual binary representation of directives in COIL, the COIL Explanatory Language (CEL) provides a human-readable way to document these directives. CEL uses a syntax with leading period (`.`) for directives.

It's important to understand that CEL is not COIL itself, but rather a representation used for documentation and explanation. COIL is always a binary format, and directives in COIL are encoded as binary structures as described above.

## Version 1.0.0 Directives Scope

The 1.0.0 release of COIL includes:
- Complete binary format for all core directives
- Full specification of directive qualifiers and parameters
- Integration with the COIL Object Format
- Documentation of CEL representation for human readability

## Future Extensions

Future versions of COIL will extend the directive binary format to include:

1. More sophisticated conditional assembly directives (Version 2.0.0)
2. Enhanced debugging directives (Version 2.0.0)
3. Module and namespace management (Version 2.0.0)
4. Interface declarations (Version 2.0.0)
5. More powerful optimization controls (Version 2.0.0)

These extensions will be defined in later specification versions and will maintain backward compatibility where possible.