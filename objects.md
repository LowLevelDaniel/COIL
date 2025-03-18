# COIL Object Formats

## Introduction

COIL employs two distinct object formats to balance the needs of optimization, cross-platform compatibility, and integration with existing tools:

1. **COIL Object Format (COF)**: A rich intermediate format that preserves COIL's abstractions, enabling link-time optimization and cross-platform capabilities.

2. **Native Object Format (NOF)**: A specialized object format containing native machine code for multiple target architectures in a unified binary. This format is designed to work with COIL-compliant linkers and provides superior support for heterogeneous computing environments.

This document specifies the structure and usage of these formats, as well as the processes for converting between them.

## Design Philosophy

The COIL object formats are designed according to several key principles:

1. **Preserving Information**: The COIL Object Format preserves all semantic information necessary for optimization and cross-platform adaptation.

2. **Performance**: Both formats are designed for efficient processing, with minimal overhead for the functionality they provide.

3. **Extensibility**: The formats can be extended to support new features and target architectures without breaking compatibility.

4. **Multiple Targets**: Support for multiple target architectures within a single binary.

5. **Explicit Control**: Provide mechanisms for explicit control over object format features when needed.

## COIL Object Format (COF)

The COIL Object Format is the primary intermediate representation used by COIL tools. It preserves all of COIL's abstractions, including variable management, virtual registers, type information, and target-specific details.

### File Structure

A COIL Object Format file consists of a header followed by a series of sections:

```
+----------------+
| COF Header     |
+----------------+
| Section 1      |
+----------------+
| Section 2      |
+----------------+
| ...            |
+----------------+
| Section N      |
+----------------+
```

### COF Header

```c
/**
 * @brief COIL Object Format header
 */
typedef struct cof_header {
  uint32_t magic;              /**< Magic number ('COIL') */
  uint16_t version_major;      /**< Format version major */
  uint16_t version_minor;      /**< Format version minor */
  uint32_t flags;              /**< Format flags */
  uint32_t target_count;       /**< Number of target architectures */
  uint32_t section_count;      /**< Number of sections */
  uint32_t symbol_count;       /**< Number of symbols */
  uint32_t string_table_size;  /**< Size of string table */
  uint64_t entry_point;        /**< Entry point (if executable) */
  uint64_t timestamp;          /**< Creation timestamp */
  uint8_t  uuid[16];           /**< Unique identifier */
  uint32_t header_size;        /**< Size of the header */
  uint32_t section_table_offset; /**< Offset to section table */
  uint32_t symbol_table_offset;  /**< Offset to symbol table */
  uint32_t string_table_offset;  /**< Offset to string table */
  uint32_t target_table_offset;  /**< Offset to target architecture table */
} cof_header_t;
```

### Target Architecture Table

The target architecture table defines the target architectures supported by the object file:

```c
/**
 * @brief Target architecture entry
 */
typedef struct cof_target {
  uint32_t target_id;          /**< Target identifier */
  uint32_t arch_type;          /**< Architecture type */
  uint32_t features;           /**< Feature flags */
  uint32_t name_offset;        /**< Offset to target name in string table */
  uint32_t config_offset;      /**< Offset to configuration data */
  uint32_t config_size;        /**< Size of configuration data */
} cof_target_t;
```

### Section Table

The section table defines the sections within the object file:

```c
/**
 * @brief Section table entry
 */
typedef struct cof_section {
  uint32_t name_offset;        /**< Offset to section name in string table */
  uint32_t type;               /**< Section type */
  uint32_t flags;              /**< Section flags */
  uint32_t target_id;          /**< Target architecture (0 for generic) */
  uint64_t address;            /**< Virtual address (if applicable) */
  uint64_t size;               /**< Section size */
  uint64_t offset;             /**< File offset to section data */
  uint32_t alignment;          /**< Section alignment */
  uint32_t relocation_count;   /**< Number of relocations */
  uint32_t relocation_offset;  /**< Offset to relocation table */
} cof_section_t;
```

### Section Types

```c
/**
 * @brief Section types
 */
enum cof_section_type {
  COF_SECTION_NULL = 0,        /**< Null section */
  COF_SECTION_CODE = 1,        /**< Executable code */
  COF_SECTION_DATA = 2,        /**< Initialized data */
  COF_SECTION_BSS = 3,         /**< Uninitialized data */
  COF_SECTION_READONLY = 4,    /**< Read-only data */
  COF_SECTION_CONFIG = 5,      /**< Configuration data */
  COF_SECTION_SYMBOL = 6,      /**< Symbol table */
  COF_SECTION_STRING = 7,      /**< String table */
  COF_SECTION_RELOC = 8,       /**< Relocation information */
  COF_SECTION_DEBUG = 9,       /**< Debug information */
  COF_SECTION_TARGET = 10,     /**< Target-specific data */
  COF_SECTION_ABI = 11,        /**< ABI-specific data */
  COF_SECTION_COMMENT = 12,    /**< Comment section */
  COF_SECTION_NOTE = 13,       /**< Note section */
  COF_SECTION_VARIABLE = 14,   /**< Variable information */
  COF_SECTION_TYPE = 15,       /**< Type information */
};
```

### Section Flags

```c
/**
 * @brief Section flags
 */
enum cof_section_flags {
  COF_SECTION_FLAG_NONE = 0x00000000,  /**< No flags */
  COF_SECTION_FLAG_WRITE = 0x00000001, /**< Writable section */
  COF_SECTION_FLAG_EXEC = 0x00000002,  /**< Executable section */
  COF_SECTION_FLAG_ALLOC = 0x00000004, /**< Occupies memory during execution */
  COF_SECTION_FLAG_LOAD = 0x00000008,  /**< Section should be loaded */
  COF_SECTION_FLAG_TLS = 0x00000010,   /**< Thread-local storage */
  COF_SECTION_FLAG_MERGE = 0x00000020, /**< Can be merged */
  COF_SECTION_FLAG_STRINGS = 0x00000040, /**< Contains null-terminated strings */
  COF_SECTION_FLAG_GROUP = 0x00000080, /**< Section is a group */
  COF_SECTION_FLAG_COMPRESSED = 0x00000100, /**< Compressed section */
  COF_SECTION_FLAG_ENCRYPTED = 0x00000200, /**< Encrypted section */
};
```

### Symbol Table

The symbol table contains information about symbols defined or referenced in the object file:

```c
/**
 * @brief Symbol table entry
 */
typedef struct cof_symbol {
  uint32_t name_offset;        /**< Offset to symbol name in string table */
  uint32_t section_index;      /**< Section index (0 for external) */
  uint64_t value;              /**< Symbol value (address or offset) */
  uint64_t size;               /**< Symbol size */
  uint16_t type;               /**< Symbol type */
  uint16_t flags;              /**< Symbol flags */
  uint32_t target_id;          /**< Target architecture (0 for generic) */
} cof_symbol_t;
```

### Symbol Types

```c
/**
 * @brief Symbol types
 */
enum cof_symbol_type {
  COF_SYMBOL_NONE = 0,         /**< Unspecified symbol type */
  COF_SYMBOL_FUNCTION = 1,     /**< Function symbol */
  COF_SYMBOL_DATA = 2,         /**< Data symbol */
  COF_SYMBOL_SECTION = 3,      /**< Section symbol */
  COF_SYMBOL_FILE = 4,         /**< File symbol */
  COF_SYMBOL_COMMON = 5,       /**< Common symbol */
  COF_SYMBOL_TLS = 6,          /**< Thread-local storage symbol */
  COF_SYMBOL_VARIABLE = 7,     /**< Variable symbol */
  COF_SYMBOL_TARGET = 8,       /**< Target-specific symbol */
};
```

### Symbol Flags

```c
/**
 * @brief Symbol flags
 */
enum cof_symbol_flags {
  COF_SYMBOL_FLAG_NONE = 0x0000,     /**< No flags */
  COF_SYMBOL_FLAG_GLOBAL = 0x0001,   /**< Global symbol */
  COF_SYMBOL_FLAG_LOCAL = 0x0002,    /**< Local symbol */
  COF_SYMBOL_FLAG_WEAK = 0x0004,     /**< Weak symbol */
  COF_SYMBOL_FLAG_HIDDEN = 0x0008,   /**< Hidden symbol */
  COF_SYMBOL_FLAG_PROTECTED = 0x0010, /**< Protected symbol */
  COF_SYMBOL_FLAG_UNDEFINED = 0x0020, /**< Undefined symbol */
  COF_SYMBOL_FLAG_EXPORTED = 0x0040, /**< Exported symbol */
  COF_SYMBOL_FLAG_ENTRY = 0x0080,    /**< Entry point symbol */
  COF_SYMBOL_FLAG_CONSTRUCTOR = 0x0100, /**< Constructor function */
  COF_SYMBOL_FLAG_DESTRUCTOR = 0x0200, /**< Destructor function */
};
```

### Relocation Table

The relocation table contains information about symbol references that need to be resolved:

```c
/**
 * @brief Relocation table entry
 */
typedef struct cof_relocation {
  uint64_t offset;             /**< Offset within section */
  uint32_t symbol_index;       /**< Symbol table index */
  uint32_t type;               /**< Relocation type */
  int64_t  addend;             /**< Addend value */
  uint32_t target_id;          /**< Target architecture */
} cof_relocation_t;
```

### Variable Information Section

The variable information section contains detailed information about variables used in the code:

```c
/**
 * @brief Variable information entry
 */
typedef struct cof_variable {
  uint32_t name_offset;        /**< Offset to variable name in string table */
  uint32_t symbol_index;       /**< Symbol table index */
  uint8_t  type;               /**< Data type */
  uint8_t  storage_class;      /**< Storage class */
  uint8_t  qualifiers;         /**< Type qualifiers */
  uint8_t  alignment;          /**< Alignment requirements */
  uint64_t size;               /**< Size in bytes */
  uint32_t scope_begin;        /**< Scope begin offset */
  uint32_t scope_end;          /**< Scope end offset */
  uint32_t initial_value_offset; /**< Offset to initial value data */
  uint32_t type_info_offset;   /**< Offset to additional type information */
} cof_variable_t;
```

### Type Information Section

The type information section contains detailed information about complex types:

```c
/**
 * @brief Type information header
 */
typedef struct cof_type_header {
  uint32_t type_id;            /**< Type identifier */
  uint8_t  basic_type;         /**< Basic type code */
  uint8_t  flags;              /**< Type flags */
  uint16_t reserved;           /**< Reserved (padding) */
  uint32_t name_offset;        /**< Offset to type name in string table */
  uint32_t size;               /**< Type size in bytes */
  uint32_t alignment;          /**< Type alignment requirements */
  uint32_t data_offset;        /**< Offset to type-specific data */
  uint32_t data_size;          /**< Size of type-specific data */
} cof_type_header_t;
```

## Native Object Format (NOF)

The Native Object Format is designed for final delivery of COIL code as executable or library binaries. It contains native machine code for multiple target architectures, with metadata to support target switching and runtime selection.

### File Structure

A Native Object Format file consists of a header followed by a series of sections:

```
+----------------+
| NOF Header     |
+----------------+
| Target Table   |
+----------------+
| Section Table  |
+----------------+
| Section 1 Data |
+----------------+
| Section 2 Data |
+----------------+
| ...            |
+----------------+
| Section N Data |
+----------------+
| Symbol Table   |
+----------------+
| String Table   |
+----------------+
| Relocation Info|
+----------------+
```

### NOF Header

```c
/**
 * @brief Native Object Format header
 */
typedef struct nof_header {
  uint32_t magic;              /**< Magic number ('NOFX') */
  uint16_t version_major;      /**< Format version major */
  uint16_t version_minor;      /**< Format version minor */
  uint32_t flags;              /**< Format flags */
  uint32_t target_count;       /**< Number of target architectures */
  uint32_t section_count;      /**< Number of sections */
  uint32_t symbol_count;       /**< Number of symbols */
  uint32_t string_table_size;  /**< Size of string table */
  uint64_t entry_point;        /**< Entry point (if executable) */
  uint32_t default_target;     /**< Default target architecture */
  uint64_t timestamp;          /**< Creation timestamp */
  uint8_t  uuid[16];           /**< Unique identifier */
  uint32_t header_size;        /**< Size of the header */
  uint32_t target_table_offset; /**< Offset to target table */
  uint32_t section_table_offset; /**< Offset to section table */
  uint32_t symbol_table_offset;  /**< Offset to symbol table */
  uint32_t string_table_offset;  /**< Offset to string table */
  uint32_t relocation_table_offset; /**< Offset to relocation table */
} nof_header_t;
```

### Target Table

The target table defines the target architectures included in the object file:

```c
/**
 * @brief Target architecture entry
 */
typedef struct nof_target {
  uint32_t target_id;          /**< Target identifier */
  uint32_t arch_type;          /**< Architecture type */
  uint32_t features;           /**< Feature flags */
  uint32_t name_offset;        /**< Offset to target name in string table */
  uint32_t metadata_offset;    /**< Offset to target metadata */
  uint32_t metadata_size;      /**< Size of target metadata */
  uint32_t section_count;      /**< Number of sections for this target */
  uint32_t first_section;      /**< Index of first section for this target */
  uint64_t entry_point;        /**< Target-specific entry point */
} nof_target_t;
```

### Section Table

The section table defines the sections within the object file:

```c
/**
 * @brief Section table entry
 */
typedef struct nof_section {
  uint32_t name_offset;        /**< Offset to section name in string table */
  uint32_t type;               /**< Section type */
  uint32_t flags;              /**< Section flags */
  uint32_t target_id;          /**< Target architecture */
  uint64_t address;            /**< Virtual address (if applicable) */
  uint64_t size;               /**< Section size */
  uint64_t offset;             /**< File offset to section data */
  uint32_t alignment;          /**< Section alignment */
  uint32_t relocation_count;   /**< Number of relocations */
  uint32_t relocation_offset;  /**< Offset to relocation table */
} nof_section_t;
```

### Symbol Table

The symbol table contains information about symbols defined or referenced in the object file:

```c
/**
 * @brief Symbol table entry
 */
typedef struct nof_symbol {
  uint32_t name_offset;        /**< Offset to symbol name in string table */
  uint32_t section_index;      /**< Section index (0 for external) */
  uint64_t value;              /**< Symbol value (address or offset) */
  uint64_t size;               /**< Symbol size */
  uint16_t type;               /**< Symbol type */
  uint16_t flags;              /**< Symbol flags */
  uint32_t target_id;          /**< Target architecture (0 for generic) */
} nof_symbol_t;
```

### Target Switch Records

Target switch records provide information about transition points between different target architectures:

```c
/**
 * @brief Target switch record
 */
typedef struct nof_target_switch {
  uint64_t source_address;     /**< Source code address */
  uint32_t source_target;      /**< Source target ID */
  uint64_t destination_address; /**< Destination code address */
  uint32_t destination_target;  /**< Destination target ID */
  uint32_t flags;              /**< Switch flags */
  uint32_t data_mapping_offset; /**< Offset to data mapping information */
  uint32_t data_mapping_count;  /**< Number of data mapping entries */
} nof_target_switch_t;
```

### Data Mapping Records

Data mapping records define how data is translated between different target architectures during a target switch:

```c
/**
 * @brief Data mapping record
 */
typedef struct nof_data_mapping {
  uint32_t source_symbol;      /**< Source symbol index */
  uint32_t destination_symbol; /**< Destination symbol index */
  uint32_t mapping_type;       /**< Mapping type */
  uint32_t mapping_flags;      /**< Mapping flags */
  uint64_t source_offset;      /**< Offset within source symbol */
  uint64_t destination_offset; /**< Offset within destination symbol */
  uint64_t size;               /**< Size of mapped data */
  uint32_t transform_offset;   /**< Offset to transformation function */
} nof_data_mapping_t;
```

## Format Conversion

### COF to NOF Conversion

Converting from COIL Object Format to Native Object Format involves the following steps:

1. **Code Generation**: Translate COIL instructions to native machine code for each target architecture.
2. **Symbol Resolution**: Resolve symbolic references and generate relocation information.
3. **Target Switch Generation**: Generate target switch records and data mapping information.
4. **Header Creation**: Create the NOF header and tables.
5. **Section Population**: Populate sections with native code and data.

### COF Linking

COIL Object Format files can be linked together to produce a single COF file:

1. **Section Merging**: Merge sections of the same type and target.
2. **Symbol Resolution**: Resolve external symbols between input files.
3. **Relocation Processing**: Update relocations to reflect the new merged layout.
4. **Optimization**: Perform link-time optimization based on the merged information.

### NOF Linking

Native Object Format files can be linked together to produce a single NOF file:

1. **Section Merging**: Merge sections of the same type and target.
2. **Symbol Resolution**: Resolve external symbols between input files.
3. **Target Switch Mapping**: Update target switch records to reflect the new layout.
4. **Relocation Processing**: Update relocations to reflect the new merged layout.

## Usage Examples

### Multi-Architecture Library

A library that can run efficiently on multiple architectures:

```
// Build process:
// 1. Compile source to COIL Object Format (COF)
// 2. Link multiple COF files together
// 3. Convert to Native Object Format (NOF)
// 4. Link with platform-specific startup code

// Result:
// - Single library file containing code for multiple architectures
// - Runtime detection of available architecture
// - Automatic selection of optimal implementation
```

### Heterogeneous Computing

A program that executes parts of its code on different processing units:

```
// Build process:
// 1. Compile CPU code to COIL with CPU target
// 2. Compile GPU code to COIL with GPU target
// 3. Link into single COF file with target switches
// 4. Convert to NOF with explicit target switching

// Result:
// - Single binary with both CPU and GPU code
// - Explicit control over data transfer between targets
// - Efficient execution across heterogeneous processing units
```

### Boot Loader

A boot loader that transitions between processor modes:

```
// Build process:
// 1. Compile 16-bit real mode code to COIL
// 2. Compile 32-bit protected mode code to COIL
// 3. Compile 64-bit long mode code to COIL
// 4. Link with explicit target switches
// 5. Generate NOF with target transitions

// Result:
// - Boot loader that switches between processor modes
// - Unified codebase with explicit transition points
// - Efficient handling of mode-specific requirements
```

## Conclusion

The COIL object formats provide a powerful foundation for cross-platform and heterogeneous computing. By preserving COIL's abstractions in the COF format and providing efficient native code delivery in the NOF format, they enable a wide range of use cases from embedded systems to high-performance computing.

The explicit support for multiple target architectures within a single binary, combined with the target switching and data mapping capabilities, makes COIL uniquely suited for modern computing environments where workloads may be distributed across diverse processing units.