# COIL Object Format Specification

This document specifies the COIL object file format, which is designed to store compiled COIL code in a format that can be processed by the UnifyLink project to produce native executables and libraries.

## Overview

The COIL object format is designed with the following goals:

1. **Efficiency**: Compact representation of compiled COIL code
2. **Flexibility**: Support for various architectures and extensions
3. **Extensibility**: Ability to add new features without breaking compatibility
4. **Self-contained**: Includes all necessary information for linking

## File Structure

A COIL object file consists of the following components:

1. **File Header**: Contains basic information about the object file
2. **Section Headers**: Define the sections in the file
3. **Section Data**: The actual content of each section
4. **Symbol Table**: Defines symbols in the object file
5. **String Table**: Contains null-terminated strings referenced by other parts of the file
6. **Relocation Tables**: Contains relocation information for sections

### File Header

The file header is a fixed-size structure at the beginning of the file:

```c
typedef struct __attribute__((packed)) {
  uint32_t magic;          // Magic number (COIL_OBJECT_MAGIC = "COIL")
  uint16_t version;        // Object format version
  uint16_t arch;           // Target architecture
  uint16_t proc;           // Target processor
  uint16_t section_count;  // Number of sections
  uint32_t symtab_offset;  // Offset to symbol table
  uint32_t symtab_size;    // Size of symbol table
  uint32_t strtab_offset;  // Offset to string table
  uint32_t strtab_size;    // Size of string table
  uint32_t flags;          // Object file flags
} coil_object_header_t;
```

### Section Headers

Each section has a header that defines its properties:

```c
typedef struct __attribute__((packed)) {
  uint32_t name_offset;    // Offset to section name in string table
  uint32_t type;           // Section type
  uint32_t flags;          // Section flags
  uint32_t offset;         // Section offset in file
  uint32_t size;           // Section size
  uint32_t addr;           // Section address
  uint32_t align;          // Section alignment
} coil_section_header_t;
```

### Section Types

COIL defines several standard section types:

- `COIL_SECTION_TEXT`: Contains executable code
- `COIL_SECTION_DATA`: Contains initialized data
- `COIL_SECTION_BSS`: Contains uninitialized data (not stored in file)
- `COIL_SECTION_RODATA`: Contains read-only data
- `COIL_SECTION_SYMTAB`: Contains symbol table
- `COIL_SECTION_RELOC`: Contains relocation information
- `COIL_SECTION_DEBUG`: Contains debug information
- `COIL_SECTION_CUSTOM`: Custom section for user-defined purposes

### Section Flags

Sections can have various flags:

- `COIL_SECTION_FLAG_ALLOC`: Section should be allocated in memory
- `COIL_SECTION_FLAG_WRITE`: Section can be written to
- `COIL_SECTION_FLAG_EXEC`: Section contains executable code
- `COIL_SECTION_FLAG_MERGE`: Section can be merged with other sections
- `COIL_SECTION_FLAG_STRINGS`: Section contains null-terminated strings
- `COIL_SECTION_FLAG_INFO`: Section contains information, not program data

### Symbol Table

The symbol table contains entries for each symbol in the object file:

```c
typedef struct __attribute__((packed)) {
  uint32_t name_offset;    // Offset to symbol name in string table
  uint32_t value;          // Symbol value
  uint32_t size;           // Symbol size
  uint8_t section;         // Symbol section index
  uint8_t type;            // Symbol type
  uint16_t flags;          // Symbol flags
} coil_symbol_t;
```

### Symbol Types

COIL defines several symbol types:

- `COIL_SYMBOL_NONE`: Undefined symbol
- `COIL_SYMBOL_OBJECT`: Data object
- `COIL_SYMBOL_FUNC`: Function
- `COIL_SYMBOL_SECTION`: Section
- `COIL_SYMBOL_FILE`: Source file
- `COIL_SYMBOL_COMMON`: Common block

### Symbol Flags

Symbols can have various flags:

- `COIL_SYMBOL_FLAG_GLOBAL`: Symbol is visible outside the object file
- `COIL_SYMBOL_FLAG_WEAK`: Symbol can be overridden by another symbol
- `COIL_SYMBOL_FLAG_HIDDEN`: Symbol is not visible outside the object file
- `COIL_SYMBOL_FLAG_UNDEFINED`: Symbol is undefined and must be resolved

### Relocation Tables

Relocation tables contain entries for each relocation in the object file:

```c
typedef struct __attribute__((packed)) {
  uint32_t offset;         // Relocation offset
  uint32_t symbol;         // Symbol index
  uint32_t addend;         // Relocation addend
  uint8_t type;            // Relocation type
  uint8_t flags;           // Relocation flags
} coil_relocation_t;
```

### Relocation Types

Relocation types are architecture-specific and define how addresses are calculated. Some common types include:

- `COIL_RELOC_ABS32`: 32-bit absolute address
- `COIL_RELOC_REL32`: 32-bit relative address
- `COIL_RELOC_ABS64`: 64-bit absolute address
- `COIL_RELOC_REL64`: 64-bit relative address

Architecture-specific relocation types should be defined in the range `COIL_RELOC_ARCH_SPECIFIC_BEGIN` to `COIL_RELOC_ARCH_SPECIFIC_END`.

## Binary Format Details

### Endianness

The COIL object format uses little-endian byte order for all multi-byte values.

### Alignment

- The file header is aligned to a 4-byte boundary
- Section headers are aligned to a 4-byte boundary
- Section data alignment is specified in the section header
- The symbol table is aligned to a 4-byte boundary
- The string table has no alignment requirements

### Padding

Padding may be inserted between sections to achieve the required alignment. Padding bytes should be set to zero.

## Example Object File Layout

Here's an example of a simple COIL object file layout:

1. **File Header**
   - Magic number: "COIL" (`0x434F494C`)
   - Version: 1
   - Architecture: COIL_ARCH_X86_64
   - Processor: COIL_PROC_CPU
   - Section count: 5
   - Symbol table offset, size
   - String table offset, size
   - Flags: 0

2. **Section Headers**
   - Text section header
   - Data section header
   - BSS section header
   - Symbol table section header
   - String table section header

3. **Text Section Data**
   - Machine code for functions

4. **Data Section Data**
   - Initialized data

5. **Symbol Table**
   - Symbol entries

6. **String Table**
   - Null-terminated strings

## Object Format Optimization

The COIL object format includes several optimizations:

1. **Compact Representation**
   - Fixed-size headers for easy parsing
   - Minimal padding
   - Use of offsets instead of pointers

2. **Efficient String Handling**
   - Single string table for all strings
   - String deduplication

3. **Flexible Section Layout**
   - Sections can be ordered arbitrarily
   - Section alignment can be specified per section

## Conversion to Native Formats

The UnifyLink project is responsible for converting COIL object files to native formats like ELF, ELF64, PE32, or PE. This process involves:

1. **Section Mapping**
   - Mapping COIL sections to native sections
   - Handling section flags and alignment

2. **Symbol Conversion**
   - Converting COIL symbols to native symbols
   - Handling symbol visibility and scope

3. **Relocation Processing**
   - Converting COIL relocations to native relocations
   - Calculating final addresses

4. **Output Generation**
   - Creating the native object file, executable, or library

## Binary Compatibility

To ensure binary compatibility across different versions of COIL:

1. **Versioning**: The object format includes a version field
2. **Extension Mechanism**: Custom sections can be used for extensions
3. **Forward Compatibility**: Unknown sections should be preserved

## Future Extensions

The COIL object format is designed to be extensible. Future versions may include:

1. **Improved Debug Information**
   - DWARF-compatible debug sections
   - Source line mapping

2. **Enhanced Symbol Information**
   - Type information
   - More detailed visibility control

3. **Security Features**
   - Signature sections
   - Content verification

4. **Performance Optimizations**
   - Precomputed hash tables
   - Incremental linking support