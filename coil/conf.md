# Configuration Format (Version 0.1.0)

The COIL Configuration Format provides a standardized way to describe the capabilities and characteristics of processing units. Instead of using architecture-specific terminology, the configuration format uses abstract feature descriptions that can be mapped to various architectures.

## Configuration Purpose

The COIL configuration serves several critical functions:

1. **Feature Detection**: Identifies what capabilities are available on the target hardware
2. **Optimization Guidance**: Helps the COIL assembler make intelligent optimization decisions
3. **Feature Emulation**: Determines when and how to emulate missing features
4. **ABI Definitions**: Defines Application Binary Interfaces for different systems
5. **Resource Information**: Provides details about available resources (registers, memory, etc.)

## Configuration Format

COIL configurations exist in both binary and text formats:

1. **Binary Format (.coilcfg)**: Compact representation used by COIL assemblers
2. **Text Format (.coilcfg.txt)**: Human-readable/editable format that can be compiled to binary

This specification defines both formats.

## Binary Configuration Format

The binary configuration format is designed for efficient processing by COIL tools.

### Binary Format Header

Each binary configuration file begins with a header:

```c
struct coil_config_header {
    uint32_t magic;         // Magic number: 'CCFG' (0x43434647)
    uint8_t  version_major; // Major version (0 for COIL 0.1.0)
    uint8_t  version_minor; // Minor version (1 for COIL 0.1.0)
    uint8_t  version_patch; // Patch version (0 for COIL 0.1.0)
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

#### Integer Operations (Bits 32-63)

```c
enum cpu_integer_features {
    // Basic operations
    CPU_FEAT_INT_ADD         = (1ULL << 0),  // Integer addition
    CPU_FEAT_INT_SUB         = (1ULL << 1),  // Integer subtraction
    CPU_FEAT_INT_MUL         = (1ULL << 2),  // Integer multiplication
    CPU_FEAT_INT_DIV         = (1ULL << 3),  // Integer division
    CPU_FEAT_INT_REM         = (1ULL << 4),  // Integer remainder
    
    // Extended operations
    CPU_FEAT_INT_POPCNT      = (1ULL << 8),  // Population count
    CPU_FEAT_INT_CLZ         = (1ULL << 9),  // Count leading zeros
    CPU_FEAT_INT_CTZ         = (1ULL << 10), // Count trailing zeros
    CPU_FEAT_INT_BSWAP       = (1ULL << 11), // Byte swap
    CPU_FEAT_INT_ADDC        = (1ULL << 12), // Add with carry
    CPU_FEAT_INT_SUBC        = (1ULL << 13), // Subtract with carry
    CPU_FEAT_INT_MULH        = (1ULL << 14), // Multiplication high half
    CPU_FEAT_INT_MULHSU      = (1ULL << 15), // Signed-unsigned multiplication high
    
    // Bit manipulation
    CPU_FEAT_INT_AND         = (1ULL << 16), // Bitwise AND
    CPU_FEAT_INT_OR          = (1ULL << 17), // Bitwise OR
    CPU_FEAT_INT_XOR         = (1ULL << 18), // Bitwise XOR
    CPU_FEAT_INT_NOT         = (1ULL << 19), // Bitwise NOT
    CPU_FEAT_INT_SHL         = (1ULL << 20), // Shift left logical
    CPU_FEAT_INT_SHR         = (1ULL << 21), // Shift right logical
    CPU_FEAT_INT_SAR         = (1ULL << 22), // Shift right arithmetic
    CPU_FEAT_INT_ROL         = (1ULL << 23), // Rotate left
    CPU_FEAT_INT_ROR         = (1ULL << 24), // Rotate right
    CPU_FEAT_INT_BEXT        = (1ULL << 25), // Bit extraction
    CPU_FEAT_INT_BINS        = (1ULL << 26), // Bit insertion
    
    // Advanced bit manipulation
    CPU_FEAT_INT_BITFIELD    = (1ULL << 28), // Bit field manipulation
    CPU_FEAT_INT_BITPERM     = (1ULL << 29), // Bit permutation
    CPU_FEAT_INT_CRC         = (1ULL << 30), // CRC calculation
    CPU_FEAT_INT_GFMUL       = (1ULL << 31), // Galois Field multiplication
};
```

#### Floating-Point Operations (Bits 64-95)

```c
enum cpu_float_features {
    // Supported formats
    CPU_FEAT_FP16            = (1ULL << 0),  // 16-bit floating point (half precision)
    CPU_FEAT_FP32            = (1ULL << 1),  // 32-bit floating point (single precision)
    CPU_FEAT_FP64            = (1ULL << 2),  // 64-bit floating point (double precision)
    CPU_FEAT_FP80            = (1ULL << 3),  // 80-bit floating point (extended precision)
    CPU_FEAT_FP128           = (1ULL << 4),  // 128-bit floating point (quad precision)
    CPU_FEAT_BFLOAT16        = (1ULL << 5),  // bfloat16 format
    
    // Basic operations
    CPU_FEAT_FP_ADD          = (1ULL << 8),  // Floating-point addition
    CPU_FEAT_FP_SUB          = (1ULL << 9),  // Floating-point subtraction
    CPU_FEAT_FP_MUL          = (1ULL << 10), // Floating-point multiplication
    CPU_FEAT_FP_DIV          = (1ULL << 11), // Floating-point division
    CPU_FEAT_FP_REM          = (1ULL << 12), // Floating-point remainder
    CPU_FEAT_FP_SQRT         = (1ULL << 13), // Square root
    CPU_FEAT_FP_FMA          = (1ULL << 14), // Fused multiply-add
    
    // Extended operations
    CPU_FEAT_FP_EXP          = (1ULL << 16), // Exponential
    CPU_FEAT_FP_LOG          = (1ULL << 17), // Logarithm
    CPU_FEAT_FP_SIN          = (1ULL << 18), // Sine
    CPU_FEAT_FP_COS          = (1ULL << 19), // Cosine
    CPU_FEAT_FP_TAN          = (1ULL << 20), // Tangent
    CPU_FEAT_FP_POW          = (1ULL << 21), // Power
    
    // Rounding modes
    CPU_FEAT_FP_ROUND_NEAR   = (1ULL << 24), // Round to nearest
    CPU_FEAT_FP_ROUND_ZERO   = (1ULL << 25), // Round toward zero
    CPU_FEAT_FP_ROUND_UP     = (1ULL << 26), // Round toward +infinity
    CPU_FEAT_FP_ROUND_DOWN   = (1ULL << 27), // Round toward -infinity
    
    // IEEE compliance
    CPU_FEAT_FP_IEEE754      = (1ULL << 28), // IEEE 754 compliant
    CPU_FEAT_FP_DENORMALS    = (1ULL << 29), // Supports denormal numbers
    CPU_FEAT_FP_NAN          = (1ULL << 30), // Supports NaN values
    CPU_FEAT_FP_INF          = (1ULL << 31), // Supports infinity values
};
```

Additional feature categories follow similar patterns, with bits assigned to specific capabilities.

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

Resource flags:
```c
enum resource_flags {
    RESOURCE_FLAG_SHARED     = (1 << 0),  // Resource is shared between PUs
    RESOURCE_FLAG_EXCLUSIVE  = (1 << 1),  // Resource is exclusive to a single PU
    RESOURCE_FLAG_VIRTUAL    = (1 << 2),  // Resource is virtualized
    RESOURCE_FLAG_RESTRICTED = (1 << 3),  // Resource has usage restrictions
    // 4-15 reserved
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

### Register Mapping Table

The register mapping table defines how arguments and return values are passed according to an ABI:

```c
struct coil_reg_mapping {
    uint16_t arg_index;     // Argument index (0 = return value)
    uint16_t reg_type;      // Register type
    uint32_t reg_index;     // Register index
    uint32_t reg_mask;      // Register mask (for partial registers)
    uint8_t  padding[4];    // Padding to align to 16 bytes
};
```

Register types:
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

### String Table

The string table contains all strings referenced in the configuration file, stored as null-terminated UTF-8 strings. Offsets into the string table are relative to the start of the string table.

## Text Configuration Format

The text configuration format uses a human-readable syntax for creating and editing configurations. The format is similar to INI files but with specialized sections and key-value pairs.

### Text Format Structure

```
; COIL Configuration File
[metadata]
version = 0.1.0
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

## Configuration Tools

COIL provides the following tools for working with configurations:

1. **coilcfg-detect**: Detects system capabilities and generates a configuration
2. **coilcfg-compile**: Compiles a text configuration to binary format
3. **coilcfg-decompile**: Converts a binary configuration to text format
4. **coilcfg-query**: Queries information from a configuration file

## Implementation Requirements

COIL assemblers must:

1. Read and interpret binary configuration files
2. Use configuration information to guide code generation and optimization
3. Emulate missing features when necessary and possible
4. Honor ABI specifications for function calls and system interfaces

## Architecture-Specific Extensions

The configuration format is intentionally architecture-agnostic, focusing on capabilities rather than specific instructions or hardware features. However, architecture-specific extensions may be defined to provide more detailed information about particular hardware.

These extensions should be placed in separate sections with appropriate naming:

```
[arch:x86_64]
sse4.2 = true
avx2 = true
```

## Future Extensions

Future versions of COIL will extend the configuration format to include:

1. More detailed performance metrics and optimization hints
2. Power and thermal constraints
3. More sophisticated feature dependencies
4. Support for heterogeneous systems with multiple types of processing units
5. Dynamic reconfiguration capabilities

These extensions will be defined in later specification versions.