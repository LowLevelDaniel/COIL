# Configuration Format (Version 1.0.0)

The COIL Configuration Format provides a standardized way to describe the capabilities and characteristics of processing units. Instead of using architecture-specific terminology, the configuration format uses abstract feature descriptions that can be mapped to various architectures.

**IMPORTANT NOTE:** This document is part of the COIL specification documentation. It does not contain implementation code, but rather describes how the configuration format should function when implemented.

## Configuration Purpose

The COIL configuration serves several critical functions:

1. **Feature Detection**: Identifies what capabilities are available on the target hardware
2. **Optimization Guidance**: Helps the COIL processor make intelligent optimization decisions
3. **Feature Emulation**: Determines when and how to emulate missing features
4. **ABI Definitions**: Defines Application Binary Interfaces for different systems
5. **Resource Information**: Provides details about available resources (registers, memory, etc.)

## Configuration Format

COIL configurations exist in both binary and text formats:

1. **Binary Format (.coilcfg)**: Compact representation used by COIL processors
2. **Text Format (.coilcfg.txt)**: Human-readable/editable format that can be compiled to binary

This specification defines both formats.

## Binary Configuration Format

The binary configuration format is designed for efficient processing by COIL tools.

### Binary Format Header

Each binary configuration file begins with a header:

```c
struct coil_config_header {
    uint32_t magic;         // Magic number: 'CCFG' (0x43434647)
    uint8_t  version_major; // Major version (1 for COIL 1.0.0)
    uint8_t  version_minor; // Minor version (0 for COIL 1.0.0)
    uint8_t  version_patch; // Patch version (0 for COIL 1.0.0)
    uint8_t  flags;         // Configuration flags
    uint32_t pu_count;      // Number of processing units defined
    uint64_t timestamp;     // Creation timestamp (UNIX time)
    uint8_t  padding[12];   // Padding to align to 32 bytes
};
```

Configuration flags:
```c
enum config_flags {
    CONFIG_FLAG_SYSTEM_DEFAULT = (1 << 0), // This is the system default configuration
    CONFIG_FLAG_USER_MODIFIED  = (1 << 1), // Configuration has been modified by user
    CONFIG_FLAG_AUTODETECTED   = (1 << 2), // Features were autodetected from hardware
    CONFIG_FLAG_COMPRESSED     = (1 << 3), // Configuration data is compressed
    // 4-7 reserved
};
```

### Processing Unit Entry

Each processing unit in the configuration is described by an entry:

```c
struct coil_pu_entry {
    uint16_t type;          // Processing unit type
    uint16_t index;         // Index of this processing unit
    uint32_t name_offset;   // Offset in string table for PU name
    uint32_t vendor_offset; // Offset in string table for vendor name
    uint32_t model_offset;  // Offset in string table for model name
    uint32_t feature_offset; // Offset to feature bitmap
    uint32_t feature_size;   // Size of feature bitmap in bytes
    uint32_t resource_offset; // Offset to resource table
    uint32_t resource_count;  // Number of resource entries
    uint32_t abi_offset;     // Offset to ABI table
    uint32_t abi_count;      // Number of ABI entries
    uint8_t  padding[4];     // Padding to align to 8 bytes
};
```

Processing unit types:
```c
enum pu_type {
    PU_TYPE_CPU    = 0,  // Central Processing Unit
    PU_TYPE_GPU    = 1,  // Graphics Processing Unit
    PU_TYPE_TPU    = 2,  // Tensor Processing Unit
    PU_TYPE_FPGA   = 3,  // Field-Programmable Gate Array
    PU_TYPE_DSP    = 4,  // Digital Signal Processor
    PU_TYPE_CUSTOM = 5,  // Custom/specialized processing unit
    // 6-65535 reserved
};
```

### Feature Bitmap

The feature bitmap uses a bitfield approach to represent available hardware features in a compact form. Each feature is represented by a bit position in a contiguous bitmap. The specific interpretation of bits depends on the processing unit type.

For CPU features (PU_TYPE_CPU), the bitmap is organized into functional categories:

```
[Core Features (32 bits)]
[Integer Operations (32 bits)]
[Floating-Point Operations (32 bits)]
[SIMD/Vector Operations (32 bits)]
[Memory Operations (32 bits)]
[Atomic Operations (32 bits)]
[Security Features (32 bits)]
[Miscellaneous Features (32 bits)]
[Reserved (variable)]
```

#### Core CPU Features (Bits 0-31)

```c
enum cpu_core_features {
    // Word sizes
    CPU_FEAT_8BIT_SUPPORT    = (1ULL << 0),  // Supports 8-bit operations
    CPU_FEAT_16BIT_SUPPORT   = (1ULL << 1),  // Supports 16-bit operations
    CPU_FEAT_32BIT_SUPPORT   = (1ULL << 2),  // Supports 32-bit operations
    CPU_FEAT_64BIT_SUPPORT   = (1ULL << 3),  // Supports 64-bit operations
    CPU_FEAT_128BIT_SUPPORT  = (1ULL << 4),  // Supports 128-bit operations
    
    // Addressing
    CPU_FEAT_PHYSICAL_32BIT  = (1ULL << 8),  // 32-bit physical addressing
    CPU_FEAT_PHYSICAL_36BIT  = (1ULL << 9),  // 36-bit physical addressing
    CPU_FEAT_PHYSICAL_40BIT  = (1ULL << 10), // 40-bit physical addressing
    CPU_FEAT_PHYSICAL_48BIT  = (1ULL << 11), // 48-bit physical addressing
    CPU_FEAT_PHYSICAL_52BIT  = (1ULL << 12), // 52-bit physical addressing
    CPU_FEAT_PHYSICAL_64BIT  = (1ULL << 13), // 64-bit physical addressing
    
    CPU_FEAT_VIRTUAL_32BIT   = (1ULL << 16), // 32-bit virtual addressing
    CPU_FEAT_VIRTUAL_48BIT   = (1ULL << 17), // 48-bit virtual addressing
    CPU_FEAT_VIRTUAL_57BIT   = (1ULL << 18), // 57-bit virtual addressing
    CPU_FEAT_VIRTUAL_64BIT   = (1ULL << 19), // 64-bit virtual addressing
    
    // Core capabilities
    CPU_FEAT_BRANCH_PRED     = (1ULL << 24), // Has branch prediction
    CPU_FEAT_SPECULATIVE_EXEC = (1ULL << 25), // Has speculative execution
    CPU_FEAT_OUT_OF_ORDER    = (1ULL << 26), // Supports out-of-order execution
    CPU_FEAT_MULTITHREAD     = (1ULL << 27), // Supports multithreading
    CPU_FEAT_MULTICORE       = (1ULL << 28), // Multiple cores per CPU
    CPU_FEAT_MULTIPROCESSOR  = (1ULL << 29), // Multiple CPUs in system
    CPU_FEAT_NUMA            = (1ULL << 30), // Non-uniform memory access
    CPU_FEAT_CACHE_COHERENT  = (1ULL << 31), // Cache coherency supported
};
```

Similar bit definitions exist for other feature categories (integer operations, floating-point operations, etc.) as detailed in the full specification.

### Resource Table

The resource table provides information about available resources, such as registers, memory, and execution units:

```c
struct coil_resource_entry {
    uint16_t type;          // Resource type
    uint16_t flags;         // Resource flags
    uint32_t name_offset;   // Offset in string table for resource name
    uint64_t count;         // Count of this resource (e.g., number of registers)
    uint64_t size;          // Size of this resource (if applicable)
    uint8_t  padding[8];    // Padding to align to 32 bytes
};
```

Resource types:
```c
enum resource_type {
    RESOURCE_TYPE_GPR       = 0,  // General purpose registers
    RESOURCE_TYPE_FPR       = 1,  // Floating-point registers
    RESOURCE_TYPE_VR        = 2,  // Vector registers
    RESOURCE_TYPE_SPR       = 3,  // Special purpose registers
    RESOURCE_TYPE_CACHE_L1  = 4,  // L1 cache
    RESOURCE_TYPE_CACHE_L2  = 5,  // L2 cache
    RESOURCE_TYPE_CACHE_L3  = 6,  // L3 cache
    RESOURCE_TYPE_EXEC_UNIT = 7,  // Execution units
    RESOURCE_TYPE_THREAD    = 8,  // Hardware threads
    RESOURCE_TYPE_CORE      = 9,  // CPU cores
    RESOURCE_TYPE_MEMORY    = 10, // Main memory
    // 11-65535 reserved
};
```

### ABI Table

The ABI table defines Application Binary Interfaces for the processing unit:

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

ABI flags:
```c
enum abi_flags {
    ABI_FLAG_VARIADIC    = (1 << 0),  // Supports variadic functions
    ABI_FLAG_PRESERVES_SP = (1 << 1),  // Preserves stack pointer
    ABI_FLAG_CALLER_CLEANUP = (1 << 2),  // Caller cleans up stack
    ABI_FLAG_USES_RED_ZONE = (1 << 3),  // Uses red zone below stack
    // 4-15 reserved
};
```

## Text Configuration Format

The text configuration format uses a human-readable syntax for creating and editing configurations. The format is similar to INI files but with specialized sections and key-value pairs.

### Text Format Structure

```
; COIL Configuration File
[metadata]
version = 1.0.0
timestamp = 1614556800
flags = system_default, autodetected

[cpu:0]
name = Generic x86-64 CPU
vendor = Generic
model = x86-64 Compatible
features = core_features, integer, floating_point, simd, memory, atomic

[core_features]
8bit_support = true
16bit_support = true
32bit_support = true
64bit_support = true
physical_48bit = true
virtual_48bit = true
branch_pred = true
out_of_order = true
multithread = false
multicore = true
cache_coherent = true

[integer]
int_add = true
int_sub = true
int_mul = true
int_div = true
int_rem = true
int_popcnt = true
int_clz = true
int_ctz = true
int_and = true
int_or = true
int_xor = true
int_not = true
int_shl = true
int_shr = true
int_sar = true

[floating_point]
fp32 = true
fp64 = true
fp_add = true
fp_sub = true
fp_mul = true
fp_div = true
fp_sqrt = true
fp_fma = true
fp_ieee754 = true
fp_denormals = true
fp_nan = true
fp_inf = true

[resources]
gpr = 16, 64bit
fpr = 16, 128bit
cache_l1 = 32KiB
cache_l2 = 256KiB
cache_l3 = 8MiB
thread = 1
core = 4
memory = 16GiB

[abi:linux-x64-syscall]
arg_count = 6
flags = preserves_sp

[linux-x64-syscall.mapping]
0 = gpr:0  ; Return value in RAX
1 = gpr:4  ; First argument in RDI (virtual register RQ4)
2 = gpr:5  ; Second argument in RSI (virtual register RQ5)
3 = gpr:2  ; Third argument in RDX (virtual register RQ2)
4 = gpr:8  ; Fourth argument in R10 (virtual register RQ8)
5 = gpr:9  ; Fifth argument in R8 (virtual register RQ9)
6 = gpr:10 ; Sixth argument in R9 (virtual register RQ10)
```

### Text Format Conventions

1. Comments begin with semicolons (`;`) and continue to the end of the line
2. Section names are enclosed in square brackets (`[section]`)
3. Key-value pairs use an equals sign (`key = value`)
4. Boolean values can be `true`/`false`, `yes`/`no`, or `1`/`0`
5. Numeric values can use suffixes (KiB, MiB, GiB) for sizes
6. References to other sections use the section name
7. Multiple values can be separated by commas
8. Subsections use dot notation (`section.subsection`)

## Version 1.0.0 Configuration Format Scope

The 1.0.0 release of COIL includes:
- Complete configuration format for CPU architectures
- Support for x86-64 and ARM64 platforms
- Full feature bitmap definitions
- Resource and ABI table specifications
- Both binary and text format specifications

## Future Extensions

Future versions of COIL will extend the configuration format to include:

1. More detailed performance metrics and optimization hints (Version 2.0.0)
2. Power and thermal constraints (Version 2.0.0)
3. More sophisticated feature dependencies (Version 2.0.0)
4. Support for heterogeneous systems with multiple types of processing units (Version 3.0.0)
5. Dynamic reconfiguration capabilities (Version 3.0.0)

These extensions will be defined in later specification versions.