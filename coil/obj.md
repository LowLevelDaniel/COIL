# Object Format Specification (Version 1.0.0)

This document defines the binary formats used for representing COIL code: the COIL Object Format (COF) and the Native COIL Object Format (NCOF). These formats enable architecture-independent intermediate code and architecture-specific native code to be represented consistently across different platforms.

## COIL Object Format (COF)

The COIL Object Format is an architecture-independent binary representation of COIL code. It is designed to be compact, portable, and straightforward to process.

### Binary Structure

A COF file consists of a header followed by a series of sections:

```
[COF Header]
[Section 1]
[Section 2]
...
[Section N]
```

#### COF Header

The COF header contains metadata about the COIL binary:

```c
struct cof_header {
    uint32_t magic;         // Magic number: 'COIL' (0x434F494C)
    uint8_t  version_major; // Major version (1 for COIL 1.0.0)
    uint8_t  version_minor; // Minor version (0 for COIL 1.0.0)
    uint8_t  version_patch; // Patch version (0 for COIL 1.0.0)
    uint8_t  flags;         // Global flags
    uint16_t target;        // Target architecture (see target_id enum)
    uint16_t section_count; // Number of sections
    uint32_t entrypoint;    // Offset to entrypoint (0 if none)
    uint32_t str_tab_off;   // Offset to string table
    uint32_t str_tab_size;  // Size of string table
    uint32_t sym_tab_off;   // Offset to symbol table
    uint32_t sym_tab_size;  // Size of symbol table
    uint8_t  padding[8];    // Padding to align to 32 bytes
};
```

Header flags:
```c
enum cof_flags {
    COF_FLAG_EXECUTABLE   = (1 << 0), // Contains an entrypoint
    COF_FLAG_LINKABLE     = (1 << 1), // Contains external references
    COF_FLAG_POSITION_IND = (1 << 2), // Position-independent code
    COF_FLAG_CONTAINS_DBG = (1 << 3), // Contains debug information
    // 4-7 reserved
};
```

#### Section Header

Each section begins with a header:

```c
struct cof_section_header {
    uint32_t name_offset;   // Offset into string table for section name
    uint32_t type;          // Section type
    uint32_t flags;         // Section flags
    uint32_t offset;        // Offset from start of file to section data
    uint32_t size;          // Size of section in bytes
    uint32_t link;          // Index of associated section (if any)
    uint32_t info;          // Additional section information
    uint32_t alignment;     // Section alignment (power of 2)
    uint32_t entsize;       // Size of entries (for table sections)
};
```

Section types:
```c
enum section_type {
    COF_SECTION_NULL      = 0,  // Unused section
    COF_SECTION_CODE      = 1,  // Executable code
    COF_SECTION_DATA      = 2,  // Initialized data
    COF_SECTION_BSS       = 3,  // Uninitialized data (Block Started by Symbol)
    COF_SECTION_SYMTAB    = 4,  // Symbol table
    COF_SECTION_STRTAB    = 5,  // String table
    COF_SECTION_RELA      = 6,  // Relocation entries with addends
    COF_SECTION_REL       = 7,  // Relocation entries without addends
    COF_SECTION_METADATA  = 8,  // Metadata (e.g., ABI definitions)
    COF_SECTION_COMMENT   = 9,  // Comment or descriptive information
    COF_SECTION_DIRECTIVE = 10, // Directives for the assembler
    // 11-65535 available for extensions
};
```

Section flags:
```c
enum section_flags {
    COF_SEC_FLAG_WRITE    = (1 << 0),  // Writable section
    COF_SEC_FLAG_EXEC     = (1 << 1),  // Executable section
    COF_SEC_FLAG_ALLOC    = (1 << 2),  // Section occupies memory during execution
    COF_SEC_FLAG_MERGE    = (1 << 3),  // Section can be merged
    COF_SEC_FLAG_STRINGS  = (1 << 4),  // Section contains null-terminated strings
    COF_SEC_FLAG_GROUP    = (1 << 5),  // Section is a member of a group
    COF_SEC_FLAG_TLS      = (1 << 6),  // Thread-local storage section
    COF_SEC_FLAG_COMPRESS = (1 << 7),  // Compressed section
    // 8-31 reserved
};
```

#### Symbol Table

The symbol table contains entries for all symbols defined or referenced in the COIL binary:

```c
struct cof_symbol {
    uint32_t name_offset;   // Offset into string table for symbol name
    uint32_t value;         // Symbol value (interpretation depends on type)
    uint32_t size;          // Size associated with symbol
    uint8_t  type;          // Symbol type
    uint8_t  binding;       // Symbol binding (scope)
    uint8_t  visibility;    // Symbol visibility
    uint8_t  section_index; // Index of section containing this symbol
};
```

Symbol types:
```c
enum symbol_type {
    SYM_TYPE_NOTYPE  = 0,  // No type specified
    SYM_TYPE_OBJECT  = 1,  // Data object (variable, array, etc.)
    SYM_TYPE_FUNC    = 2,  // Function or executable code
    SYM_TYPE_SECTION = 3,  // Section symbol
    SYM_TYPE_FILE    = 4,  // Source file name
    // 5-15 reserved
};
```

Symbol bindings:
```c
enum symbol_binding {
    SYM_BIND_LOCAL  = 0,  // Local symbol, not visible outside object file
    SYM_BIND_GLOBAL = 1,  // Global symbol, visible to all object files
    SYM_BIND_WEAK   = 2,  // Global scope but with lower precedence
    // 3-15 reserved
};
```

Symbol visibility:
```c
enum symbol_visibility {
    SYM_VIS_DEFAULT   = 0,  // Default visibility rules
    SYM_VIS_HIDDEN    = 1,  // Symbol not visible to other components
    SYM_VIS_PROTECTED = 2,  // Symbol visible but not preemptable
    // 3-15 reserved
};
```

#### Relocation Entries

Relocation entries specify locations in the code or data that need to be adjusted when linking:

```c
struct cof_rel {
    uint32_t offset;        // Location to be relocated
    uint32_t symbol_index;  // Symbol table index
    uint8_t  type;          // Relocation type
    uint8_t  padding[3];    // Padding to align to 8 bytes
};

struct cof_rela {
    uint32_t offset;        // Location to be relocated
    uint32_t symbol_index;  // Symbol table index
    uint8_t  type;          // Relocation type
    uint8_t  padding[3];    // Padding to align to 8 bytes
    int32_t  addend;        // Constant addend for relocation value
};
```

Relocation types are architecture-specific and defined in the target architecture's configuration.

#### String Table

The string table contains null-terminated strings referenced by offsets in other sections. The first byte of the string table (index 0) must be a null byte.

#### Code Section Content

Code sections contain a stream of COIL instructions and directives, encoded according to the format defined in the ISA specification (isa.md).

### Structure of a Minimal COF File

A minimal COF file must contain:

1. COF header
2. String table section
3. At least one code or data section

Example layout of a minimal COF file:
```
[COF Header]
[String Table Section Header]
[String Table Data]
[Code Section Header]
[Code Section Data]
```

## Native COIL Object Format (NCOF)

The Native COIL Object Format represents architecture-specific code generated by COIL processors from COF input. NCOF files are designed to be compatible with platform-specific linkers and loaders.

### Binary Structure

NCOF follows a similar structure to COF but contains native code:

```
[NCOF Header]
[Section 1]
[Section 2]
...
[Section N]
```

#### NCOF Header

```c
struct ncof_header {
    uint32_t magic;         // Magic number: 'NCOF' (0x4E434F46)
    uint8_t  version_major; // Major version (1 for COIL 1.0.0)
    uint8_t  version_minor; // Minor version (0 for COIL 1.0.0)
    uint8_t  version_patch; // Patch version (0 for COIL 1.0.0)
    uint8_t  flags;         // Global flags
    uint16_t arch;          // Target architecture ID
    uint16_t arch_variant;  // Architecture variant/subtype
    uint16_t section_count; // Number of sections
    uint16_t padding;       // Padding to align
    uint32_t entrypoint;    // Offset to entrypoint (0 if none)
    uint32_t str_tab_off;   // Offset to string table
    uint32_t str_tab_size;  // Size of string table
    uint32_t sym_tab_off;   // Offset to symbol table
    uint32_t sym_tab_size;  // Size of symbol table
    uint8_t  padding2[8];   // Padding to align to 32 bytes
};
```

The section structure is similar to COF, with additional architecture-specific information as needed.

### Architecture-Specific Sections

NCOF may contain architecture-specific sections for:

1. Platform-specific metadata
2. Native code mappings
3. Special linkage information
4. Processing unit specific extensions

## Format Extensions

Both COF and NCOF formats support extensions via:

1. Custom section types (user-defined section types starting at 0x8000)
2. Custom metadata sections
3. Additional header flags

Extensions must be documented in processor-specific documentation.

## Binary Compatibility Guarantees

The binary formats defined in this specification are guaranteed to be compatible across all conforming COIL implementations. Version 1.0.0 establishes the following guarantees:

1. All COIL processors must accept valid COF files that conform to this specification
2. Future versions of COIL will maintain backward compatibility with version 1.0.0 COF files
3. NCOF format compatibility is architecture-specific and dependent on the target platform's requirements

## Implementation Requirements

COIL processors must:

1. Correctly parse and interpret all standard sections in COF files
2. Generate valid NCOF files for supported target architectures
3. Handle relocation entries appropriately
4. Maintain symbol visibility rules
5. Process code and data sections according to the COIL ISA specification

## File Extensions

1. `.cof` - COIL Object Format file
2. `.ncof` - Native COIL Object Format file
3. `.coil` - Alternative extension for COF files

## Multi-Architecture Support

To support heterogeneous computing environments with multiple processing unit types, COF files may contain multiple code sections targeting different architectures. The COIL processor is responsible for selecting and processing the appropriate sections based on the target architecture specified at compilation time.

This multi-architecture support is achieved through:

1. Architecture-specific section flags
2. Target directives within the code
3. Separate code sections for different processing unit types

## Binary Tool Requirements

A complete COIL toolchain should include the following binary utilities:

1. **coil-objdump**: Display information about COF/NCOF files
2. **coil-nm**: List symbols from COF/NCOF files
3. **coil-readelf**: Display COIL object file information
4. **coil-strip**: Remove symbols and sections from COF/NCOF files
5. **coil-ar**: Create, modify, and extract from COIL archives

These tools are not part of the core specification but are recommended for a complete implementation.