# COIL Target Switching System

## Introduction

One of COIL's most powerful and unique features is its target switching capability. This system allows developers to create code that can seamlessly transition between different hardware architectures, instruction sets, and execution environments within a single codebase. The target switching system is what enables COIL to excel at heterogeneous computing, polyglot binaries, and mode transitions in systems programming.

## Design Philosophy

The target switching system is designed with several key principles in mind:

1. **Transparent Transitions**: Make architecture transitions as seamless as possible for the developer.
2. **Explicit Control**: Give programmers explicit control over when and how target switching occurs.
3. **Flexible Integration**: Allow for flexible integration with existing code and tools for each target.
4. **Performance-Focused**: Minimize overhead for cross-target calls and data transfers.
5. **Static Analysis Friendly**: Enable comprehensive static analysis across architecture boundaries.
6. **Binary Representation**: Provide efficient binary encoding of target switches.

## Target Configuration

Each target in COIL represents a specific hardware architecture, instruction set, or execution environment. Targets are defined in the configuration file and specified in the Configuration Table of the COIL binary format.

### Target Definition Structure

```c
/**
 * @brief Target definition structure
 */
typedef struct coil_target_def {
  uint32_t target_id;           /**< Unique target identifier */
  uint8_t  pu_class;            /**< Processing unit class (CPU, GPU, etc.) */
  uint8_t  arch;                /**< Architecture identifier */
  uint8_t  word_size;           /**< Word size in bits (8, 16, 32, 64) */
  uint8_t  endianness;          /**< Byte ordering (little, big) */
  uint32_t features;            /**< Feature flags */
  uint32_t extensions;          /**< Extension flags */
  uint32_t abi_id;              /**< Default ABI identifier */
  uint32_t config_offset;       /**< Offset to detailed configuration */
  uint32_t config_size;         /**< Size of configuration data */
  uint32_t name_offset;         /**< Offset to target name in string table */
  uint32_t description_offset;  /**< Offset to description in string table */
} coil_target_def_t;
```

### Processing Unit Classes

```c
/**
 * @brief Processing unit classes
 */
enum coil_pu_class {
  COIL_PU_CPU = 0,    /**< Central Processing Unit */
  COIL_PU_GPU = 1,    /**< Graphics Processing Unit */
  COIL_PU_DSP = 2,    /**< Digital Signal Processor */
  COIL_PU_NPU = 3,    /**< Neural Processing Unit */
  COIL_PU_TPU = 4,    /**< Tensor Processing Unit */
  COIL_PU_FPGA = 5,   /**< Field-Programmable Gate Array */
  COIL_PU_ASIC = 6,   /**< Application-Specific Integrated Circuit */
};
```

### Architectures

```c
/**
 * @brief CPU architectures
 */
enum coil_cpu_arch {
  COIL_CPU_ARCH_X86,            /**< x86 and x86-64 */
  COIL_CPU_ARCH_ARM,            /**< ARM and ARM64 */
  COIL_CPU_ARCH_RISCV,          /**< RISC-V */
  COIL_CPU_ARCH_POWERPC,        /**< PowerPC and POWER */
  COIL_CPU_ARCH_MIPS,           /**< MIPS */
  COIL_CPU_ARCH_SPARC,          /**< SPARC */
  COIL_CPU_ARCH_WASM,           /**< WebAssembly */
  COIL_CPU_ARCH_AVR,            /**< 8-bit AVR microcontroller */
  COIL_CPU_ARCH_MSP430,         /**< 16-bit MSP430 microcontroller */
  /* Additional architectures as needed */
};
```

## Target Switching

Target switching in COIL is accomplished through explicit switch points in the code. These switch points define where execution transitions from one target architecture to another, and how data is mapped between the different environments.

### Switch Point Definition

```c
/**
 * @brief Target switch point definition
 */
typedef struct coil_switch_point {
  uint32_t source_target;       /**< Source target ID */
  uint32_t destination_target;  /**< Destination target ID */
  uint32_t switch_type;         /**< Switch type (call, jump, etc.) */
  uint32_t switch_flags;        /**< Switch flags */
  uint32_t entry_symbol;        /**< Entry point symbol in destination target */
  uint32_t return_symbol;       /**< Return point symbol in source target */
  uint32_t data_mapping_count;  /**< Number of data mapping entries */
  uint32_t data_mapping_offset; /**< Offset to data mapping table */
} coil_switch_point_t;
```

### Switch Types

```c
/**
 * @brief Target switch types
 */
enum coil_switch_type {
  COIL_SWITCH_CALL = 0,      /**< Function call with return */
  COIL_SWITCH_JUMP = 1,      /**< One-way jump (no return) */
  COIL_SWITCH_MODE = 2,      /**< Processor mode switch */
  COIL_SWITCH_CONTEXT = 3,   /**< Execution context switch */
  COIL_SWITCH_INTERRUPT = 4, /**< Interrupt handler switch */
  COIL_SWITCH_EXCEPTION = 5, /**< Exception handler switch */
};
```

### Switch Flags

```c
/**
 * @brief Target switch flags
 */
enum coil_switch_flags {
  COIL_SWITCH_FLAG_NONE = 0x00000000,      /**< No flags */
  COIL_SWITCH_FLAG_SAVE_STATE = 0x00000001, /**< Save full processor state */
  COIL_SWITCH_FLAG_ASYNC = 0x00000002,     /**< Asynchronous switch */
  COIL_SWITCH_FLAG_CONDITIONAL = 0x00000004, /**< Conditional switch */
  COIL_SWITCH_FLAG_LAZY_MAP = 0x00000008,  /**< Lazy data mapping */
  COIL_SWITCH_FLAG_NO_RETURN = 0x00000010, /**< No return expected */
  COIL_SWITCH_FLAG_CRITICAL = 0x00000020,  /**< Critical section (uninterruptible) */
  COIL_SWITCH_FLAG_CHECK_SUPPORT = 0x00000040, /**< Check target support at runtime */
  COIL_SWITCH_FLAG_FALLBACK = 0x00000080,  /**< Fallback available */
};
```

### Data Mapping

Data mapping defines how data is transferred between different target architectures during a target switch:

```c
/**
 * @brief Data mapping entry
 */
typedef struct coil_data_mapping {
  uint32_t source_symbol;      /**< Symbol in source target */
  uint32_t destination_symbol; /**< Symbol in destination target */
  uint32_t mapping_type;       /**< Type of mapping */
  uint32_t mapping_flags;      /**< Mapping flags */
  uint32_t transform_routine;  /**< Custom transform routine (if needed) */
  uint64_t source_offset;      /**< Offset within source symbol */
  uint64_t destination_offset; /**< Offset within destination symbol */
  uint64_t size;               /**< Size of mapped data */
} coil_data_mapping_t;
```

### Mapping Types

```c
/**
 * @brief Data mapping types
 */
enum coil_mapping_type {
  COIL_MAP_DIRECT = 0,       /**< Direct copy */
  COIL_MAP_TRANSFORM = 1,    /**< Transform during copy */
  COIL_MAP_REFERENCE = 2,    /**< Pass by reference */
  COIL_MAP_SHARED = 3,       /**< Shared memory */
  COIL_MAP_STREAM = 4,       /**< Stream data */
  COIL_MAP_ASYNC = 5,        /**< Asynchronous transfer */
  COIL_MAP_DMA = 6,          /**< DMA transfer */
  COIL_MAP_CUSTOM = 7,       /**< Custom mapping */
};
```

### Mapping Flags

```c
/**
 * @brief Data mapping flags
 */
enum coil_mapping_flags {
  COIL_MAP_FLAG_NONE = 0x00000000,     /**< No flags */
  COIL_MAP_FLAG_READ_ONLY = 0x00000001, /**< Read-only mapping */
  COIL_MAP_FLAG_WRITE_ONLY = 0x00000002, /**< Write-only mapping */
  COIL_MAP_FLAG_READ_WRITE = 0x00000003, /**< Read-write mapping */
  COIL_MAP_FLAG_VOLATILE = 0x00000004, /**< Volatile data */
  COIL_MAP_FLAG_CACHE = 0x00000008,    /**< Cache data */
  COIL_MAP_FLAG_FLUSH = 0x00000010,    /**< Flush caches */
  COIL_MAP_FLAG_INVALIDATE = 0x00000020, /**< Invalidate caches */
  COIL_MAP_FLAG_COHERENT = 0x00000040, /**< Maintain coherence */
  COIL_MAP_FLAG_NO_CONVERT = 0x00000080, /**< Skip endian conversion */
  COIL_MAP_FLAG_NO_ALIGN = 0x00000100, /**< Skip alignment */
  COIL_MAP_FLAG_BARRIER = 0x00000200,  /**< Memory barrier */
};
```

## Binary Representation

Target switches are encoded in the COIL binary format as special instructions:

```c
/**
 * @brief Target switch instruction
 */
typedef struct coil_switch_instr {
  uint8_t  opcode;             /**< Opcode (COIL_OP_CF_SWITCH) */
  uint8_t  operand_count;      /**< Number of operands */
  uint16_t ext_data_size;      /**< Extended data size */
  uint32_t switch_point_id;    /**< Switch point identifier */
  uint32_t condition;          /**< Condition code (if conditional) */
  uint8_t  operands[];         /**< Operands (parameters) */
} coil_switch_instr_t;
```

## Implementation Mechanisms

COIL supports several mechanisms for implementing target switches:

### 1. Direct Mode Transition

For architectures that support multiple modes (e.g., x86 real mode to protected mode), the switch instruction directly modifies the processor state to change modes.

### 2. Function Call Interface

For switching between different processing units (e.g., CPU to GPU), the switch uses a function call interface defined by the runtime environment.

### 3. Processor Context Switch

For multi-core and multi-processor systems, the switch may involve a full processor context switch to migrate execution.

### 4. Runtime Dispatch

For polyglot binaries, the switch may involve runtime dispatch to select the appropriate implementation based on the available hardware.

### 5. Stub Generation

The COIL assembler generates appropriate stub code for each target switch point to handle the transition seamlessly.

## Use Cases

### 1. CPU/GPU Heterogeneous Computing

```
// CPU code (target_id = 1)
void process_data(float* data, int size) {
  // Prepare data
  ...
  
  // Switch to GPU for computation
  #switch target=2 entry=gpu_process_data
  // Control returns here after GPU processing
  
  // Post-process data on CPU
  ...
}

// GPU code (target_id = 2)
void gpu_process_data(float* data, int size) {
  // Process data on GPU
  ...
  
  // Return to CPU
  #return
}
```

### 2. x86 Boot Loader Mode Transitions

```
// 16-bit real mode code (target_id = 3)
void boot_entry() {
  // Initial setup in real mode
  ...
  
  // Switch to 32-bit protected mode
  #switch target=4 entry=protected_mode_entry mode
  
  // Never returns here
}

// 32-bit protected mode code (target_id = 4)
void protected_mode_entry() {
  // Protected mode initialization
  ...
  
  // Switch to 64-bit long mode
  #switch target=5 entry=long_mode_entry mode
  
  // Never returns here
}

// 64-bit long mode code (target_id = 5)
void long_mode_entry() {
  // Long mode initialization
  ...
  
  // Jump to kernel
  jump_to_kernel();
}
```

### 3. Multi-Architecture Library

```
// Generic entry point
void sort_array(int* data, int size) {
  // Determine best implementation based on available hardware
  #switch target=auto entry=sort_array_impl
  
  // Control returns here after sorting
}

// x86 AVX2 implementation (target_id = 6)
void sort_array_impl(int* data, int size) {
  // AVX2-optimized sorting algorithm
  ...
  
  #return
}

// ARM NEON implementation (target_id = 7)
void sort_array_impl(int* data, int size) {
  // NEON-optimized sorting algorithm
  ...
  
  #return
}

// Fallback implementation (target_id = 0)
void sort_array_impl(int* data, int size) {
  // Generic sorting algorithm
  ...
  
  #return
}
```

## Target Selection Mechanisms

COIL provides several mechanisms for selecting the appropriate target at runtime:

### 1. Static Selection

The target is determined at compile time and encoded directly in the binary.

### 2. Dynamic Selection

The target is selected at runtime based on available hardware features:

```c
/**
 * @brief Target selection descriptor
 */
typedef struct coil_target_selector {
  uint32_t selector_type;      /**< Selection mechanism */
  uint32_t default_target;     /**< Default target if no match */
  uint32_t entry_points[16];   /**< Entry points for each potential target */
  uint32_t condition_count;    /**< Number of selection conditions */
  uint32_t condition_offset;   /**< Offset to selection conditions */
} coil_target_selector_t;
```

### 3. Runtime Detection

For polyglot binaries, COIL includes runtime detection code to identify available hardware features:

```c
/**
 * @brief Runtime detection structure
 */
typedef struct coil_runtime_detector {
  uint32_t detector_type;      /**< Detection mechanism */
  uint32_t feature_mask;       /**< Required feature mask */
  uint32_t detector_offset;    /**< Offset to detector code */
  uint32_t detector_size;      /**< Size of detector code */
} coil_runtime_detector_t;
```

## Data Marshaling

When switching between targets, data often needs to be transformed to accommodate different architectures. COIL provides built-in support for common transformations:

### 1. Endian Conversion

```c
/**
 * @brief Endian conversion options
 */
enum coil_endian_conversion {
  COIL_ENDIAN_NONE = 0,        /**< No conversion */
  COIL_ENDIAN_SWAP = 1,        /**< Byte swap */
  COIL_ENDIAN_BSWAP16 = 2,     /**< 16-bit byte swap */
  COIL_ENDIAN_BSWAP32 = 3,     /**< 32-bit byte swap */
  COIL_ENDIAN_BSWAP64 = 4,     /**< 64-bit byte swap */
};
```

### 2. Pointer Adjustment

```c
/**
 * @brief Pointer adjustment options
 */
enum coil_ptr_adjustment {
  COIL_PTR_NONE = 0,           /**< No adjustment */
  COIL_PTR_REBASE = 1,         /**< Rebase pointers */
  COIL_PTR_TRANSLATE = 2,      /**< Translate addresses */
  COIL_PTR_SERIALIZE = 3,      /**< Serialize to handle non-pointer systems */
};
```

### 3. Data Layout Transformation

```c
/**
 * @brief Data layout transformation
 */
enum coil_layout_transform {
  COIL_LAYOUT_NONE = 0,        /**< No transformation */
  COIL_LAYOUT_PACK = 1,        /**< Pack structure */
  COIL_LAYOUT_UNPACK = 2,      /**< Unpack structure */
  COIL_LAYOUT_ALIGN = 3,       /**< Align to target requirements */
  COIL_LAYOUT_CUSTOM = 4,      /**< Custom transformation */
};
```

## Conclusion

COIL's target switching system provides a powerful mechanism for creating code that can seamlessly transition between different hardware architectures, instruction sets, and execution environments. This capability enables developers to write highly optimized code for heterogeneous computing systems, create polyglot binaries that run efficiently on multiple architectures, and handle mode transitions in systems programming.

By making architecture transitions explicit yet straightforward, COIL gives programmers fine-grained control over where and how their code executes, while abstracting away much of the complexity associated with cross-architecture development. This balance of control and abstraction is key to COIL's value proposition as a low-level intermediate language for modern computing systems.