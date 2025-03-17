# COIL Object Format

This document specifies the COIL Object Format (COF), which is used for COIL-compliant linkers to perform cross-architecture optimizations.

## 1. Overview

The COIL Object Format is distinct from the COIL binary format. While the COIL binary format represents the intermediate language itself, the COF is designed to store native code generated from COIL, along with metadata that enables advanced link-time optimizations.

COF serves two primary purposes:
1. **Linking Native Code**: Combining multiple compiled COIL modules into a single executable
2. **Cross-Processing Unit Optimization**: Enabling optimizations across different processing units and code sections

## 2. File Structure

A COF file consists of the following sections:

1. **Header**: File identification and format information
2. **Section Table**: Information about file sections
3. **Symbol Table**: Definitions and references to symbols
4. **Relocation Table**: Instructions for adjusting addresses during linking
5. **COIL Metadata**: Original COIL information for optimization
6. **Native Code Sections**: Compiled native code
7. **Data Sections**: Initialized and uninitialized data
8. **Debug Sections**: Optional debugging information

## 3. Section Details

### 3.1 Header

The COF header identifies the file and provides version information:

```
+---------------+---------------+---------------+---------------+
| 'C' (8 bits)  | 'O' (8 bits)  | 'F' (8 bits)  | Version (8b)  |
+---------------+---------------+---------------+---------------+
| Flags (16 bits)              | Section Count (16 bits)        |
+---------------+---------------+---------------+---------------+
| Symbol Count (32 bits)                                       |
+---------------+---------------+---------------+---------------+
| Timestamp (64 bits)                                          |
|                                                              |
+---------------+---------------+---------------+---------------+
| Target Architecture Type (32 bits)                           |
+---------------+---------------+---------------+---------------+
| Target Architecture Version (32 bits)                        |
+---------------+---------------+---------------+---------------+
```

Field descriptions:
- **Signature**: The ASCII string "COF" followed by a version byte
- **Flags**: Various file-level flags
  - Bit 0: Contains executable code
  - Bit 1: Contains debug information
  - Bit 2: Big-endian encoding
  - Bit 3: Position-independent code
  - Bits 4-15: Reserved
- **Section Count**: Number of sections in the file
- **Symbol Count**: Number of symbols in the file
- **Timestamp**: Creation time in Unix timestamp format
- **Target Architecture Type**: Enum value corresponding to `config_cpu_arch_t`
- **Target Architecture Version**: Version identifier for the target architecture

### 3.2 Section Table

The section table follows the header and lists all sections in the file:

```
+---------------+---------------+---------------+---------------+
| Section 1 Type (8 bits)       | Flags (16 bits)              |
+---------------+---------------+---------------+---------------+
| Section 1 Name Offset (32 bits)                             |
+---------------+---------------+---------------+---------------+
| Section 1 Offset (64 bits)                                  |
|                                                              |
+---------------+---------------+---------------+---------------+
| Section 1 Size (64 bits)                                    |
|                                                              |
+---------------+---------------+---------------+---------------+
| Section 1 Alignment (8 bits)  | Reserved (24 bits)           |
+---------------+---------------+---------------+---------------+
| Section 2 Type ...                                           |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

Section types include:
- 0: Null (unused section)
- 1: Native Code
- 2: Initialized Data
- 3: Uninitialized Data (BSS)
- 4: Symbol Table
- 5: String Table
- 6: Relocation Table
- 7: COIL Metadata
- 8: Debug Information
- 9-255: Reserved

Section flags include:
- Bit 0: Readable
- Bit 1: Writable
- Bit 2: Executable
- Bit 3: Memory-resident
- Bit 4: Memory-initialized
- Bit 5: Position-independent
- Bits 6-15: Reserved

### 3.3 Symbol Table

The symbol table contains information about all symbols defined or referenced in the object file:

```
+---------------+---------------+---------------+---------------+
| Symbol 1 Name Offset (32 bits)                              |
+---------------+---------------+---------------+---------------+
| Symbol 1 Section Index (16 bits) | Type (8 bits) | Flags (8b) |
+---------------+---------------+---------------+---------------+
| Symbol 1 Value (64 bits)                                    |
|                                                              |
+---------------+---------------+---------------+---------------+
| Symbol 1 Size (32 bits)        | Reserved (32 bits)          |
+---------------+---------------+---------------+---------------+
| Symbol 2 Name Offset ...                                    |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

Symbol types include:
- 0: Undefined
- 1: Global
- 2: Local
- 3: Weak
- 4: Function
- 5: Data
- 6: Section
- 7: File

Symbol flags include:
- Bit 0: External
- Bit 1: Defined
- Bit 2: Debug
- Bit 3: Weak
- Bit 4: Hidden
- Bit 5: Protected
- Bit 6: Exported
- Bit 7: Thread-local

### 3.4 Relocation Table

The relocation table contains entries that instruct the linker how to modify addresses during linking:

```
+---------------+---------------+---------------+---------------+
| Relocation 1 Section Index (16 bits) | Type (16 bits)        |
+---------------+---------------+---------------+---------------+
| Relocation 1 Offset (64 bits)                               |
|                                                              |
+---------------+---------------+---------------+---------------+
| Relocation 1 Symbol Index (32 bits) | Add Symbol (32 bits)   |
+---------------+---------------+---------------+---------------+
| Relocation 1 Addend (64 bits)                               |
|                                                              |
+---------------+---------------+---------------+---------------+
| Relocation 2 Section Index ...                              |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

Relocation types are architecture-specific, with common types including:
- 1: Absolute 32-bit
- 2: Absolute 64-bit
- 3: PC-relative 32-bit
- 4: PC-relative 64-bit
- 5: GOT 32-bit
- 6: GOT 64-bit
- 7: PLT 32-bit
- 8: PLT 64-bit

### 3.5 COIL Metadata

The COIL metadata section contains information from the original COIL binary that can be used for link-time optimizations:

```
+---------------+---------------+---------------+---------------+
| Metadata Version (32 bits)    | Entry Count (32 bits)         |
+---------------+---------------+---------------+---------------+
| Entry 1 Type (16 bits)        | Entry 1 Size (16 bits)        |
+---------------+---------------+---------------+---------------+
| Entry 1 Data (variable size)                                 |
+---------------+---------------+---------------+---------------+
| Entry 2 Type ...                                             |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

Metadata entry types include:
1. **Procedure Mapping**: Maps COIL procedures to native code
2. **Instruction Mapping**: Maps COIL instructions to native instructions
3. **Variable Mapping**: Maps COIL variables to registers or memory
4. **Optimization Hints**: Information that can guide link-time optimizations
5. **Cross-Unit References**: References to other processing units
6. **Architecture Capabilities**: CPU capabilities used by the code
7. **Alternative Implementations**: Different implementations for the same function

### 3.6 Native Code Sections

Native code sections contain the compiled machine code for the target architecture. The format of these sections is architecture-specific, but the COF header includes information about the target architecture to help interpret them.

### 3.7 Data Sections

Data sections contain initialized or uninitialized data. Initialized data is stored directly in the file, while uninitialized data (BSS) only requires size information.

### 3.8 Debug Sections

Debug sections contain information for debugging, including:
- Source file mappings
- Line number information
- Type information
- Variable locations
- Stack unwinding information

## 4. Cross-Architecture Linking

One of the primary features of COF is its support for cross-architecture linking. This allows code compiled for different architectures or processing units to be linked together with appropriate interface code generated automatically.

### 4.1 Architecture Compatibility Table

COF files can include an architecture compatibility table that specifies which architectures can interact with the code:

```
+---------------+---------------+---------------+---------------+
| Compatible Architecture Count (32 bits)                      |
+---------------+---------------+---------------+---------------+
| Architecture 1 Type (32 bits)                               |
+---------------+---------------+---------------+---------------+
| Architecture 1 Min Version (32 bits) | Max Version (32 bits) |
+---------------+---------------+---------------+---------------+
| Architecture 2 Type ...                                      |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

### 4.2 Cross-Unit Interface Descriptors

When code needs to interact with another processing unit, it includes interface descriptors that specify:
- The calling convention
- Parameter passing mechanism
- Return value handling
- Memory synchronization requirements
- Privilege level transitions

```
+---------------+---------------+---------------+---------------+
| Interface Descriptor Count (32 bits)                         |
+---------------+---------------+---------------+---------------+
| Descriptor 1 Source Symbol Index (32 bits)                  |
+---------------+---------------+---------------+---------------+
| Descriptor 1 Target Architecture (32 bits)                  |
+---------------+---------------+---------------+---------------+
| Descriptor 1 Parameter Count (16 bits) | Flags (16 bits)     |
+---------------+---------------+---------------+---------------+
| Descriptor 1 Parameter Types (variable size)                |
+---------------+---------------+---------------+---------------+
| Descriptor 1 Return Type (32 bits)                          |
+---------------+---------------+---------------+---------------+
| Descriptor 2 Source Symbol Index ...                        |
+---------------+---------------+---------------+---------------+
| ...                                                          |
```

## 5. Link-Time Optimization

COF is designed to enable advanced link-time optimizations:

### 5.1 Procedure Inlining

The linker can inline procedures across module boundaries based on:
- Procedure size
- Calling frequency
- Performance impact
- Cross-architecture compatibility

### 5.2 Specialization

The linker can specialize procedures for specific architectures or usage patterns:
- Constant propagation across module boundaries
- Architecture-specific implementations
- Context-dependent optimizations

### 5.3 Dead Code Elimination

The linker can eliminate unused code across module boundaries:
- Unreachable procedures
- Unused global variables
- Unused parameters
- Unused return values

### 5.4 Code Layout Optimization

The linker can optimize the layout of code for better cache behavior:
- Placing frequently called procedures together
- Aligning code for optimal instruction fetch
- Separating hot and cold code paths

## 6. Object File Utility Functions

COF files can be manipulated using the following utility functions, which are typically provided as part of the COIL toolchain:

### 6.1 Object Inspection

```
cof-inspect file.o               # Display basic information
cof-inspect --symbols file.o     # List symbols
cof-inspect --relocations file.o # List relocations
cof-inspect --metadata file.o    # Display COIL metadata
```

### 6.2 Object Manipulation

```
cof-extract --section=.text file.o > text.bin  # Extract a section
cof-strip --debug file.o -o file-stripped.o    # Remove debug information
cof-rewrite --set-arch=x86_64 file.o -o new.o  # Change target architecture
```

### 6.3 Object Linking

```
cof-link file1.o file2.o -o output  # Link objects into an executable
cof-link --shared file.o -o lib.so  # Create a shared library
cof-link --optimize=3 file.o -o opt # Link with optimizations
```

## 7. Cross-Processing Unit Considerations

### 7.1 CPU-GPU Interaction

When linking code that interacts between a CPU and GPU:

1. The COF file includes both CPU and GPU native code sections
2. Interface descriptors specify how data is transferred between the processors
3. The linker generates appropriate memory copy and synchronization code
4. Address space translations are automatically handled

### 7.2 Heterogeneous Multi-Core

For heterogeneous multi-core systems (e.g., big.LITTLE):

1. The COF file may include multiple implementations of the same procedure
2. The linker selects the appropriate implementation based on the target core
3. Interface descriptors handle differences in calling conventions
4. Memory coherence is ensured through appropriate synchronization

### 7.3 Accelerator Integration

For specialized accelerators:

1. The COF file includes code for both the main CPU and the accelerator
2. Data transfer operations are automatically generated
3. Power management considerations are encoded as hints
4. Fallback implementations are provided for systems without the accelerator

## 8. Version Control and Compatibility

### 8.1 Versioning Scheme

COF uses a versioning scheme that ensures backward compatibility:

- **Major Version**: Incremented for incompatible changes
- **Minor Version**: Incremented for backward-compatible additions
- **Patch Version**: Incremented for backward-compatible bug fixes

### 8.2 Compatibility Guarantees

- Linkers must be able to read COF files with the same major version number
- Newer linkers should be able to read older COF files
- Unrecognized sections or attributes should be preserved or ignored

### 8.3 Extension Mechanism

COF includes a mechanism for vendor-specific extensions:

```
+---------------+---------------+---------------+---------------+
| Extension ID (128 bits)                                      |
|                                                              |
|                                                              |
|                                                              |
+---------------+---------------+---------------+---------------+
| Extension Data Size (64 bits)                               |
|                                                              |
+---------------+---------------+---------------+---------------+
| Extension Data (variable size)                              |
+---------------+---------------+---------------+---------------+
```

The Extension ID is typically a UUID that identifies the vendor and extension type.