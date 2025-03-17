# COIL Configuration Format

## Introduction

The COIL Configuration Format defines a comprehensive specification of target architectures, instruction capabilities, ABIs, and system interfaces in a C-structure-inspired syntax. This configuration system serves as the backbone of COIL's cross-architecture and cross-processing unit capabilities, enabling precise and efficient code generation for diverse hardware targets.

## Design Philosophy

The COIL Configuration Format follows these key principles:

1. **Explicit Structure**: Uses C-style structures and enums for clear, explicit configuration
2. **Efficient Representation**: Uses bitfields for feature flags to maximize space efficiency
3. **Hierarchical Organization**: Organizes capabilities in a logical, nested hierarchy
4. **Comprehensive Coverage**: Captures all relevant hardware details for optimal code generation
5. **Programmatic Processing**: Designed for easy parsing and in-memory representation
6. **Self-Documenting**: Structure and naming clarify hardware capabilities and limitations

## Configuration Structure

The COIL configuration consists of C-style structures and enums organized in hierarchical sections:

1. **Version Information**: Configuration version tracking
2. **Core Architecture Definitions**: CPU architecture, word size, endianness
3. **Instruction Capabilities**: Detailed bitfield-based feature flags
4. **Register Definitions**: Register properties, aliases, and mappings
5. **ABI Definitions**: Calling conventions for functions and system calls
6. **Memory System**: Cache configurations and memory models
7. **Target-Specific Extensions**: Specialized capabilities for different hardware

## File Format

COIL configuration files use a C-structure syntax with nested structures and explicit typing:

```c
/**
 * @file coil_config.h
 * @brief COIL configuration for cross-architecture code generation
 * @version 1.0.0
 */

#ifndef COIL_CONFIG_H
#define COIL_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Version information for the configuration structure
 */
typedef struct coil_config_version {
  uint8_t major;                   /**< Major version (incompatible changes) */
  uint8_t minor;                   /**< Minor version (compatible additions) */
  uint8_t patch;                   /**< Patch version (bug fixes) */
} coil_config_version_t;

/**
 * @brief Current version of the COIL configuration structure
 */
#define COIL_CONFIG_VERSION_MAJOR 1
#define COIL_CONFIG_VERSION_MINOR 0
#define COIL_CONFIG_VERSION_PATCH 0

/**
 * @brief Supported CPU architectures
 */
typedef enum coil_cpu_arch {
  COIL_CPU_ARCH_X86,           /**< x86 and x86-64 */
  COIL_CPU_ARCH_ARM,           /**< ARM and ARM64 */
  COIL_CPU_ARCH_RISCV,         /**< RISC-V */
  COIL_CPU_ARCH_WASM,          /**< WebAssembly */
  COIL_CPU_ARCH_GPU_CUDA,      /**< NVIDIA CUDA */
  COIL_CPU_ARCH_GPU_OPENCL,    /**< OpenCL */
  COIL_CPU_ARCH_GPU_VULKAN,    /**< Vulkan Compute */
  COIL_CPU_ARCH_CUSTOM         /**< Custom architecture */
} coil_cpu_arch_t;

/**
 * @brief Register types available in CPUs
 */
typedef enum coil_reg_type {
  COIL_REG_GP,      /**< General purpose registers */
  COIL_REG_FP,      /**< Floating point registers */
  COIL_REG_VEC,     /**< Vector registers */
  COIL_REG_FLAG,    /**< Flag registers */
  COIL_REG_PC,      /**< Program counter */
  COIL_REG_SP,      /**< Stack pointer */
  COIL_REG_FP_PTR,  /**< Frame pointer */
  COIL_REG_LR,      /**< Link register */
  COIL_REG_SPECIAL  /**< Special purpose registers */
} coil_reg_type_t;

/**
 * @brief Endianness types
 */
typedef enum coil_endianness {
  COIL_ENDIAN_LITTLE,          /**< Little-endian byte order */
  COIL_ENDIAN_BIG,             /**< Big-endian byte order */
  COIL_ENDIAN_BI               /**< Bi-endian (configurable) */
} coil_endianness_t;

/**
 * @brief Register descriptor
 */
typedef struct coil_reg {
  coil_reg_type_t type;           /**< Type of register */
  uint16_t index;                 /**< Index of the register within its type */
  char name[16];                  /**< Name of the register (e.g., "rax", "r0") */
  uint16_t size;                  /**< Size of the register in bits */
  bool reserved;                  /**< Whether this register is reserved */
  bool partial_access;            /**< Whether parts of register can be accessed */
  uint32_t partial_sizes;         /**< Bitfield of accessible sub-register sizes */
} coil_reg_t;

/**
 * @brief Register alias descriptor
 */
typedef struct coil_reg_alias {
  char alias[16];                /**< Alias name */
  char target[16];               /**< Target register name */
  uint8_t start_bit;             /**< Starting bit for partial register */
  uint8_t end_bit;               /**< Ending bit for partial register */
} coil_reg_alias_t;

/**
 * @brief Integer instruction capabilities - Part 1
 */
typedef enum coil_instr_int_1 {
  /* Basic arithmetic */
  COIL_INSTR_INT_ADD              = (1ULL << 0),  /**< Addition */
  COIL_INSTR_INT_SUB              = (1ULL << 1),  /**< Subtraction */
  COIL_INSTR_INT_MUL              = (1ULL << 2),  /**< Multiplication */
  COIL_INSTR_INT_DIV              = (1ULL << 3),  /**< Division */
  COIL_INSTR_INT_REM              = (1ULL << 4),  /**< Remainder/Modulo */
  COIL_INSTR_INT_NEG              = (1ULL << 5),  /**< Negation */
  
  /* Bitwise operations */
  COIL_INSTR_INT_AND              = (1ULL << 6),  /**< Bitwise AND */
  COIL_INSTR_INT_OR               = (1ULL << 7),  /**< Bitwise OR */
  COIL_INSTR_INT_XOR              = (1ULL << 8),  /**< Bitwise XOR */
  COIL_INSTR_INT_NOT              = (1ULL << 9),  /**< Bitwise NOT */
  
  /* Shifts and rotates */
  COIL_INSTR_INT_SHL              = (1ULL << 10), /**< Shift left logical */
  COIL_INSTR_INT_SHR              = (1ULL << 11), /**< Shift right logical */
  COIL_INSTR_INT_SAR              = (1ULL << 12), /**< Shift right arithmetic */
  COIL_INSTR_INT_ROL              = (1ULL << 13), /**< Rotate left */
  COIL_INSTR_INT_ROR              = (1ULL << 14), /**< Rotate right */
  
  /* Bit manipulation */
  COIL_INSTR_INT_BIT_TEST         = (1ULL << 15), /**< Test bit */
  COIL_INSTR_INT_BIT_SET          = (1ULL << 16), /**< Set bit */
  COIL_INSTR_INT_BIT_CLR          = (1ULL << 17), /**< Clear bit */
  COIL_INSTR_INT_POPCOUNT         = (1ULL << 18), /**< Population count */
  COIL_INSTR_INT_CLZ              = (1ULL << 19), /**< Count leading zeros */
  COIL_INSTR_INT_CTZ              = (1ULL << 20), /**< Count trailing zeros */
  
  /* Memory operations */
  COIL_INSTR_INT_LOAD             = (1ULL << 21), /**< Load from memory */
  COIL_INSTR_INT_STORE            = (1ULL << 22), /**< Store to memory */
  COIL_INSTR_INT_MOVE             = (1ULL << 23), /**< Move register to register */
  
  /* Advanced operations */
  COIL_INSTR_INT_ADDC             = (1ULL << 24), /**< Add with carry */
  COIL_INSTR_INT_SUBC             = (1ULL << 25), /**< Subtract with carry */
  COIL_INSTR_INT_MULH             = (1ULL << 26), /**< Multiply high */
  COIL_INSTR_INT_MULHU            = (1ULL << 27), /**< Multiply high unsigned */
  COIL_INSTR_INT_MULHSU           = (1ULL << 28), /**< Multiply high signed-unsigned */
  
  /* Comparison */
  COIL_INSTR_INT_CMP              = (1ULL << 29), /**< Compare */
  COIL_INSTR_INT_TEST             = (1ULL << 30), /**< Test (AND without storing) */
  COIL_INSTR_INT_MIN              = (1ULL << 31), /**< Minimum */
  COIL_INSTR_INT_MAX              = (1ULL << 32), /**< Maximum */
  COIL_INSTR_INT_CMOV             = (1ULL << 33), /**< Conditional move */
  
  /* Atomic operations */
  COIL_INSTR_INT_ATOMIC_ADD       = (1ULL << 34), /**< Atomic add */
  COIL_INSTR_INT_ATOMIC_SUB       = (1ULL << 35), /**< Atomic subtract */
  COIL_INSTR_INT_ATOMIC_AND       = (1ULL << 36), /**< Atomic AND */
  COIL_INSTR_INT_ATOMIC_OR        = (1ULL << 37), /**< Atomic OR */
  COIL_INSTR_INT_ATOMIC_XOR       = (1ULL << 38), /**< Atomic XOR */
  COIL_INSTR_INT_ATOMIC_XCHG      = (1ULL << 39), /**< Atomic exchange */
  COIL_INSTR_INT_ATOMIC_CMPXCHG   = (1ULL << 40), /**< Atomic compare-exchange */
  
  /* Stack operations */
  COIL_INSTR_INT_PUSH             = (1ULL << 41), /**< Push to stack */
  COIL_INSTR_INT_POP              = (1ULL << 42), /**< Pop from stack */
  
  /* Miscellaneous */
  COIL_INSTR_INT_BYTE_SWAP        = (1ULL << 43), /**< Byte swap */
} coil_instr_int_1_t;

/**
 * @brief Floating-point instruction capabilities
 */
typedef enum coil_instr_fp {
  /* Basic floating point */
  COIL_INSTR_FP_ADD           = (1ULL << 0),  /**< FP addition */
  COIL_INSTR_FP_SUB           = (1ULL << 1),  /**< FP subtraction */
  COIL_INSTR_FP_MUL           = (1ULL << 2),  /**< FP multiplication */
  COIL_INSTR_FP_DIV           = (1ULL << 3),  /**< FP division */
  COIL_INSTR_FP_NEG           = (1ULL << 4),  /**< FP negation */
  COIL_INSTR_FP_ABS           = (1ULL << 5),  /**< FP absolute value */
  COIL_INSTR_FP_SQRT          = (1ULL << 6),  /**< FP square root */
  
  /* Advanced floating point */
  COIL_INSTR_FP_FMA           = (1ULL << 7),  /**< FP fused multiply-add */
  COIL_INSTR_FP_FMS           = (1ULL << 8),  /**< FP fused multiply-subtract */
  COIL_INSTR_FP_CMP           = (1ULL << 9),  /**< FP compare */
  COIL_INSTR_FP_MIN           = (1ULL << 10), /**< FP minimum */
  COIL_INSTR_FP_MAX           = (1ULL << 11), /**< FP maximum */
  
  /* Conversion */
  COIL_INSTR_FP_INT_TO_FP     = (1ULL << 12), /**< Integer to FP conversion */
  COIL_INSTR_FP_FP_TO_INT     = (1ULL << 13), /**< FP to integer conversion */
  COIL_INSTR_FP_ROUND         = (1ULL << 14), /**< FP rounding */
  COIL_INSTR_FP_TRUNC         = (1ULL << 15), /**< FP truncation */
  COIL_INSTR_FP_CEIL          = (1ULL << 16), /**< FP ceiling */
  COIL_INSTR_FP_FLOOR         = (1ULL << 17), /**< FP floor */
  
  /* Memory operations */
  COIL_INSTR_FP_LOAD          = (1ULL << 18), /**< Load from memory */
  COIL_INSTR_FP_STORE         = (1ULL << 19), /**< Store to memory */
  COIL_INSTR_FP_MOVE          = (1ULL << 20), /**< Move register to register */
} coil_instr_fp_t;

/**
 * @brief SIMD/Vector instruction capabilities
 */
typedef enum coil_instr_vector {
  /* Vector basics */
  COIL_INSTR_VEC_ADD          = (1ULL << 0),  /**< Vector add */
  COIL_INSTR_VEC_SUB          = (1ULL << 1),  /**< Vector subtract */
  COIL_INSTR_VEC_MUL          = (1ULL << 2),  /**< Vector multiply */
  COIL_INSTR_VEC_DIV          = (1ULL << 3),  /**< Vector divide */
  COIL_INSTR_VEC_ABS          = (1ULL << 4),  /**< Vector absolute value */
  COIL_INSTR_VEC_NEG          = (1ULL << 5),  /**< Vector negate */
  
  /* Vector bitwise */
  COIL_INSTR_VEC_AND          = (1ULL << 6),  /**< Vector AND */
  COIL_INSTR_VEC_OR           = (1ULL << 7),  /**< Vector OR */
  COIL_INSTR_VEC_XOR          = (1ULL << 8),  /**< Vector XOR */
  COIL_INSTR_VEC_NOT          = (1ULL << 9),  /**< Vector NOT */
  
  /* Vector advanced arithmetic */
  COIL_INSTR_VEC_FMA          = (1ULL << 10), /**< Vector fused multiply-add */
  COIL_INSTR_VEC_DOT          = (1ULL << 11), /**< Vector dot product */
  
  /* Vector comparison */
  COIL_INSTR_VEC_CMP          = (1ULL << 12), /**< Vector compare */
  COIL_INSTR_VEC_MIN          = (1ULL << 13), /**< Vector minimum */
  COIL_INSTR_VEC_MAX          = (1ULL << 14), /**< Vector maximum */
  
  /* Vector manipulation */
  COIL_INSTR_VEC_SHUFFLE      = (1ULL << 15), /**< Vector shuffle */
  COIL_INSTR_VEC_EXTRACT      = (1ULL << 16), /**< Vector extract element */
  COIL_INSTR_VEC_INSERT       = (1ULL << 17), /**< Vector insert element */
  
  /* Memory operations */
  COIL_INSTR_VEC_LOAD         = (1ULL << 18), /**< Vector load */
  COIL_INSTR_VEC_STORE        = (1ULL << 19), /**< Vector store */
  COIL_INSTR_VEC_GATHER       = (1ULL << 20), /**< Vector gather */
  COIL_INSTR_VEC_SCATTER      = (1ULL << 21), /**< Vector scatter */
} coil_instr_vector_t;

/**
 * @brief Control flow instruction capabilities
 */
typedef enum coil_instr_control_flow {
  /* Basic control flow */
  COIL_INSTR_CF_BRANCH        = (1ULL << 0),  /**< Branch/jump */
  COIL_INSTR_CF_BRANCH_COND   = (1ULL << 1),  /**< Conditional branch */
  COIL_INSTR_CF_CALL          = (1ULL << 2),  /**< Call subroutine */
  COIL_INSTR_CF_RET           = (1ULL << 3),  /**< Return from subroutine */
  COIL_INSTR_CF_JUMP_REG      = (1ULL << 4),  /**< Jump to register */
  
  /* Advanced control flow */
  COIL_INSTR_CF_CMOV          = (1ULL << 5),  /**< Conditional move */
  COIL_INSTR_CF_CSET          = (1ULL << 6),  /**< Conditional set */
  COIL_INSTR_CF_LOOP          = (1ULL << 7),  /**< Loop instructions */
  
  /* System operations */
  COIL_INSTR_CF_SYSCALL       = (1ULL << 8),  /**< System call */
  COIL_INSTR_CF_TRAP          = (1ULL << 9),  /**< Trap/exception */
  COIL_INSTR_CF_BREAK         = (1ULL << 10), /**< Breakpoint */
  
  /* Thread synchronization */
  COIL_INSTR_CF_BARRIER       = (1ULL << 11), /**< Memory barrier */
  COIL_INSTR_CF_ATOMIC        = (1ULL << 12), /**< Atomic operations */
  COIL_INSTR_CF_FENCE         = (1ULL << 13), /**< Memory fence */
} coil_instr_control_flow_t;

/**
 * @brief Memory synchronization capabilities
 */
typedef enum coil_instr_mem_sync {
  COIL_INSTR_MEM_FENCE        = (1ULL << 0),  /**< Memory fence */
  COIL_INSTR_MEM_BARRIER_ACQ  = (1ULL << 1),  /**< Acquire barrier */
  COIL_INSTR_MEM_BARRIER_REL  = (1ULL << 2),  /**< Release barrier */
  COIL_INSTR_MEM_BARRIER_FULL = (1ULL << 3),  /**< Full barrier */
} coil_instr_mem_sync_t;

/**
 * @brief ABI calling convention
 */
typedef struct coil_abi {
  char name[32];                      /**< Name of the ABI */
  
  /* Parameter passing */
  coil_reg_t *int_param_regs;         /**< Integer parameter registers */
  uint8_t int_param_reg_count;        /**< Number of integer parameter registers */
  coil_reg_t *fp_param_regs;          /**< FP parameter registers */
  uint8_t fp_param_reg_count;         /**< Number of FP parameter registers */
  uint8_t stack_alignment;            /**< Stack alignment in bytes */
  
  /* Return values */
  coil_reg_t *int_return_regs;        /**< Integer return registers */
  uint8_t int_return_reg_count;       /**< Number of integer return registers */
  coil_reg_t *fp_return_regs;         /**< FP return registers */
  uint8_t fp_return_reg_count;        /**< Number of FP return registers */
  
  /* Register preservation */
  coil_reg_t *caller_saved;           /**< Caller-saved registers */
  uint8_t caller_saved_count;         /**< Number of caller-saved registers */
  coil_reg_t *callee_saved;           /**< Callee-saved registers */
  uint8_t callee_saved_count;         /**< Number of callee-saved registers */
  
  /* Stack behavior */
  bool stack_grows_down;              /**< Whether stack grows downward */
  uint16_t red_zone_size;             /**< Size of the red zone in bytes */
  bool shadow_space;                  /**< Whether shadow space is used */
  uint16_t shadow_space_size;         /**< Size of shadow space in bytes */
} coil_abi_t;

/**
 * @brief System call interface
 */
typedef struct coil_syscall_abi {
  char name[32];                      /**< Name of the syscall ABI */
  
  coil_reg_t syscall_num_reg;         /**< Register for syscall number */
  coil_reg_t *param_regs;             /**< Parameter registers */
  uint8_t param_reg_count;            /**< Number of parameter registers */
  coil_reg_t return_reg;              /**< Return value register */
  coil_reg_t error_reg;               /**< Error code register */
  char trap_instruction[16];          /**< Instruction used to trigger syscall */
  
  /* Syscall number table */
  struct {
    char name[32];                    /**< Syscall name */
    uint32_t number;                  /**< Syscall number */
  } *syscalls;                        /**< Array of syscall definitions */
  uint16_t syscall_count;             /**< Number of syscall definitions */
} coil_syscall_abi_t;

/**
 * @brief Target architecture description
 */
typedef struct coil_target {
  /* Identification */
  char name[32];                      /**< Target name */
  coil_cpu_arch_t arch;               /**< CPU architecture */
  
  /* Basic properties */
  uint8_t word_size;                  /**< Word size in bits (32/64) */
  coil_endianness_t endianness;       /**< Byte order */
  
  /* Instruction capabilities */
  uint64_t instr_int_1;               /**< Integer instruction capabilities part 1 */
  uint64_t instr_fp;                  /**< Floating-point instruction capabilities */
  uint64_t instr_vector;              /**< SIMD/Vector instruction capabilities */
  uint64_t instr_control_flow;        /**< Control flow instruction capabilities */
  uint64_t instr_mem_sync;            /**< Memory synchronization capabilities */
  
  /* Registers */
  struct {
    coil_reg_t *gp_regs;             /**< General purpose registers */
    uint8_t gp_reg_count;            /**< Number of GP registers */
    coil_reg_t *fp_regs;             /**< Floating point registers */
    uint8_t fp_reg_count;            /**< Number of FP registers */
    coil_reg_t *vec_regs;            /**< Vector registers */
    uint8_t vec_reg_count;           /**< Number of vector registers */
    coil_reg_t *special_regs;        /**< Special registers */
    uint8_t special_reg_count;       /**< Number of special registers */
    
    coil_reg_alias_t *aliases;       /**< Register aliases */
    uint16_t alias_count;            /**< Number of register aliases */
    
    struct {
      char virtual_name[16];         /**< Virtual register name */
      char target_name[16];          /**< Target register name */
    } *virtual_mappings;             /**< Virtual to physical register mappings */
    uint16_t virtual_mapping_count;  /**< Number of virtual mappings */
  } registers;
  
  /* ABI information */
  coil_abi_t *abis;                   /**< Supported ABIs */
  uint8_t abi_count;                  /**< Number of ABIs */
  char default_abi[32];               /**< Default ABI name */
  
  /* System interface */
  coil_syscall_abi_t *syscall_abis;   /**< Syscall ABIs */
  uint8_t syscall_abi_count;          /**< Number of syscall ABIs */
  
  /* Memory system */
  struct {
    uint16_t cache_line_size;         /**< Cache line size in bytes */
    bool unaligned_access;            /**< Whether unaligned access is supported */
    uint8_t min_alignment;            /**< Minimum required alignment in bytes */
  } memory;
  
  /* Extensions */
  struct {
    char name[32];                    /**< Extension name */
    uint64_t features;                /**< Extension-specific features */
  } *extensions;                      /**< Hardware extensions */
  uint8_t extension_count;            /**< Number of extensions */
} coil_target_t;

/**
 * @brief Main COIL configuration structure
 */
typedef struct coil_config {
  /* Version information */
  coil_config_version_t version;      /**< Configuration version */
  
  /* Targets */
  coil_target_t *targets;             /**< Defined targets */
  uint8_t target_count;               /**< Number of targets */
  
  /* Additional metadata */
  char author[64];                    /**< Configuration author */
  char description[256];              /**< Configuration description */
} coil_config_t;

#endif /* COIL_CONFIG_H */
```

## Configuration Instance

Here's what a configuration instance looks like when populated with values:

```c
// Example COIL Configuration for x86_64
coil_config_t config = {
  .version = {
    .major = 1,
    .minor = 0,
    .patch = 0
  },
  
  .author = "COIL Development Team",
  .description = "Standard COIL configuration for x86_64",
  
  .target_count = 1,
  .targets = {
    {
      .name = "x86_64",
      .arch = COIL_CPU_ARCH_X86,
      .word_size = 64,
      .endianness = COIL_ENDIAN_LITTLE,
      
      // Instruction capabilities bitfields
      .instr_int_1 = 
          COIL_INSTR_INT_ADD | 
          COIL_INSTR_INT_SUB | 
          COIL_INSTR_INT_MUL |
          COIL_INSTR_INT_DIV | 
          COIL_INSTR_INT_REM |
          // Many more instruction capability flags...
          COIL_INSTR_INT_PUSH | 
          COIL_INSTR_INT_POP,
          
      .instr_fp = 
          COIL_INSTR_FP_ADD | 
          COIL_INSTR_FP_SUB | 
          COIL_INSTR_FP_MUL |
          COIL_INSTR_FP_DIV | 
          // More FP instructions...
          COIL_INSTR_FP_LOAD | 
          COIL_INSTR_FP_STORE,
          
      .instr_control_flow = 
          COIL_INSTR_CF_BRANCH | 
          COIL_INSTR_CF_BRANCH_COND | 
          COIL_INSTR_CF_CALL |
          COIL_INSTR_CF_RET | 
          COIL_INSTR_CF_SYSCALL,
      
      // Registers
      .registers = {
        .gp_reg_count = 16,
        .gp_regs = {
          { .type = COIL_REG_GP, .index = 0, .name = "rax", .size = 64 },
          { .type = COIL_REG_GP, .index = 1, .name = "rbx", .size = 64 },
          { .type = COIL_REG_GP, .index = 2, .name = "rcx", .size = 64 },
          // More registers...
        },
        
        .fp_reg_count = 16,
        .fp_regs = {
          { .type = COIL_REG_FP, .index = 0, .name = "xmm0", .size = 128 },
          { .type = COIL_REG_FP, .index = 1, .name = "xmm1", .size = 128 },
          // More registers...
        },
        
        .alias_count = 16,
        .aliases = {
          { .alias = "eax", .target = "rax", .start_bit = 0, .end_bit = 31 },
          { .alias = "ax", .target = "rax", .start_bit = 0, .end_bit = 15 },
          { .alias = "al", .target = "rax", .start_bit = 0, .end_bit = 7 },
          // More aliases...
        },
        
        .virtual_mapping_count = 32,
        .virtual_mappings = {
          { .virtual_name = "r0", .target_name = "rax" },
          { .virtual_name = "r1", .target_name = "rcx" },
          // More mappings...
          { .virtual_name = "f0", .target_name = "xmm0" },
          { .virtual_name = "f1", .target_name = "xmm1" },
          // More mappings...
        }
      },
      
      // ABIs
      .abi_count = 2,
      .abis = {
        {
          .name = "system_v",
          .int_param_reg_count = 6,
          .int_param_regs = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" },
          .fp_param_reg_count = 8,
          .fp_param_regs = { "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7" },
          .stack_alignment = 16,
          
          .int_return_reg_count = 1,
          .int_return_regs = { "rax" },
          .fp_return_reg_count = 1,
          .fp_return_regs = { "xmm0" },
          
          .caller_saved_count = 9,
          .caller_saved = { "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11" },
          .callee_saved_count = 6,
          .callee_saved = { "rbx", "rbp", "r12", "r13", "r14", "r15" },
          
          .stack_grows_down = true,
          .red_zone_size = 128,
          .shadow_space = false,
        },
        {
          .name = "win64",
          .int_param_reg_count = 4,
          .int_param_regs = { "rcx", "rdx", "r8", "r9" },
          // Rest of Win64 ABI configuration...
        }
      },
      
      // Default ABI
      .default_abi = "system_v",
      
      // Syscall ABIs
      .syscall_abi_count = 1,
      .syscall_abis = {
        {
          .name = "linux_x86_64",
          .syscall_num_reg = { .name = "rax" },
          .param_reg_count = 6,
          .param_regs = { "rdi", "rsi", "rdx", "r10", "r8", "r9" },
          .return_reg = { .name = "rax" },
          .error_reg = { .name = "rax" },
          .trap_instruction = "syscall",
          
          .syscall_count = 4,
          .syscalls = {
            { .name = "read", .number = 0 },
            { .name = "write", .number = 1 },
            { .name = "open", .number = 2 },
            { .name = "close", .number = 3 }
            // More syscalls...
          }
        }
      },
      
      // Memory system
      .memory = {
        .cache_line_size = 64,
        .unaligned_access = true,
        .min_alignment = 1
      },
      
      // Extensions
      .extension_count = 2,
      .extensions = {
        {
          .name = "avx2",
          .features = 0x01 // Extension-specific feature flags
        },
        {
          .name = "sse4.2",
          .features = 0x02 // Extension-specific feature flags
        }
      }
    }
  }
};
```

## Usage in COIL

The COIL configuration is used by several components of the system:

1. **Assembler**: Uses the configuration to generate correct machine code for each target
   ```c
   // Example assembler usage
   coil_target_t *target = find_target(config, "x86_64");
   if (target->instr_int_1 & COIL_INSTR_INT_ADD) {
     // Generate add instruction for x86_64
   }
   ```

2. **Optimizer**: Uses the configuration to make target-specific optimization decisions
   ```c
   // Example optimizer usage
   if (target->registers.gp_reg_count > 16) {
     // Use register-heavy optimization strategy
   } else {
     // Use memory-based optimization strategy
   }
   ```

3. **Code Generator**: Maps virtual registers to target registers
   ```c
   // Example register mapping
   char *physical_reg = NULL;
   for (int i = 0; i < target->registers.virtual_mapping_count; i++) {
     if (strcmp(target->registers.virtual_mappings[i].virtual_name, "r0") == 0) {
       physical_reg = target->registers.virtual_mappings[i].target_name;
       break;
     }
   }
   ```

4. **ABI Handler**: Ensures proper function call conventions
   ```c
   // Example ABI usage
   coil_abi_t *abi = NULL;
   for (int i = 0; i < target->abi_count; i++) {
     if (strcmp(target->abis[i].name, "system_v") == 0) {
       abi = &target->abis[i];
       break;
     }
   }
   
   // Get parameter register for first argument
   char *param_reg = abi->int_param_regs[0].name;
   ```

5. **Syscall Interface**: Manages system call conventions
   ```c
   // Example syscall usage
   coil_syscall_abi_t *syscall_abi = &target->syscall_abis[0];
   
   // Get syscall number for "write"
   int syscall_num = -1;
   for (int i = 0; i < syscall_abi->syscall_count; i++) {
     if (strcmp(syscall_abi->syscalls[i].name, "write") == 0) {
       syscall_num = syscall_abi->syscalls[i].number;
       break;
     }
   }
   
   // Generate code to put syscall number in appropriate register
   generate_mov_instruction(syscall_abi->syscall_num_reg.name, syscall_num);
   ```

## Serialization Format

The COIL configuration can be serialized to and from configuration files using a structured format:

```
// COIL configuration for x86_64
config {
  version = { major = 1, minor = 0, patch = 0 }
  author = "COIL Development Team"
  description = "Standard COIL configuration for x86_64"
}

// Target definition
target x86_64 {
  arch = COIL_CPU_ARCH_X86
  word_size = 64
  endianness = COIL_ENDIAN_LITTLE
  
  // Instruction capabilities using bit flags
  instr_int_1 = 0xFFFFFFFFFFFFFFFF  // All integer instructions supported
  instr_fp = 0x1FFFFF               // All FP instructions supported
  instr_vector = 0x3FFFFF           // All vector instructions supported
  instr_control_flow = 0x3FFF       // All control flow instructions supported
  instr_mem_sync = 0xF              // All memory sync instructions supported
  
  // Register definitions
  registers {
    // General purpose registers
    gp_regs = [
      { type = COIL_REG_GP, index = 0, name = "rax", size = 64 },
      { type = COIL_REG_GP, index = 1, name = "rbx", size = 64 },
      { type = COIL_REG_GP, index = 2, name = "rcx", size = 64 },
      { type = COIL_REG_GP, index = 3, name = "rdx", size = 64 },
      { type = COIL_REG_GP, index = 4, name = "rsi", size = 64 },
      { type = COIL_REG_GP, index = 5, name = "rdi", size = 64 },
      { type = COIL_REG_GP, index = 6, name = "rbp", size = 64 },
      { type = COIL_REG_GP, index = 7, name = "rsp", size = 64 },
      { type = COIL_REG_GP, index = 8, name = "r8", size = 64 },
      { type = COIL_REG_GP, index = 9, name = "r9", size = 64 },
      { type = COIL_REG_GP, index = 10, name = "r10", size = 64 },
      { type = COIL_REG_GP, index = 11, name = "r11", size = 64 },
      { type = COIL_REG_GP, index = 12, name = "r12", size = 64 },
      { type = COIL_REG_GP, index = 13, name = "r13", size = 64 },
      { type = COIL_REG_GP, index = 14, name = "r14", size = 64 },
      { type = COIL_REG_GP, index = 15, name = "r15", size = 64 }
    ]
    
    // Floating point registers
    fp_regs = [
      { type = COIL_REG_FP, index = 0, name = "xmm0", size = 128 },
      { type = COIL_REG_FP, index = 1, name = "xmm1", size = 128 },
      // More registers...
    ]
    
    // Register aliases
    aliases = [
      { alias = "eax", target = "rax", start_bit = 0, end_bit = 31 },
      { alias = "ax", target = "rax", start_bit = 0, end_bit = 15 },
      { alias = "al", target = "rax", start_bit = 0, end_bit = 7 },
      // More aliases...
    ]
    
    // Virtual register mappings
    virtual_mappings = [
      { virtual_name = "r0", target_name = "rax" },
      { virtual_name = "r1", target_name = "rcx" },
      { virtual_name = "r2", target_name = "rdx" },
      // More mappings...
    ]
  }
  
  // ABI definitions
  abis = [
    {
      name = "system_v"
      int_param_regs = ["rdi", "rsi", "rdx", "rcx", "r8", "r9"]
      fp_param_regs = ["xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"]
      stack_alignment = 16
      
      int_return_regs = ["rax"]
      fp_return_regs = ["xmm0"]
      
      caller_saved = ["rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"]
      callee_saved = ["rbx", "rbp", "r12", "r13", "r14", "r15"]
      
      stack_grows_down = true
      red_zone_size = 128
      shadow_space = false
    },
    {
      name = "win64"
      // Win64 ABI details...
    }
  ]
  
  default_abi = "system_v"
  
  // Syscall ABI definitions
  syscall_abis = [
    {
      name = "linux_x86_64"
      syscall_num_reg = "rax"
      param_regs = ["rdi", "rsi", "rdx", "r10", "r8", "r9"]
      return_reg = "rax"
      error_reg = "rax" 
      trap_instruction = "syscall"
      
      syscalls = [
        { name = "read", number = 0 },
        { name = "write", number = 1 },
        { name = "open", number = 2 },
        { name = "close", number = 3 },
        // More syscalls...
      ]
    }
  ]
  
  // Memory system
  memory {
    cache_line_size = 64
    unaligned_access = true
    min_alignment = 1
  }
  
  // Extensions
  extensions = [
    {
      name = "avx2"
      features = 0x1
    },
    {
      name = "sse4.2"
      features = 0x2
    }
  ]
}
```

## Adding New Targets

To add a new target to the COIL configuration:

1. Define a new target structure in the configuration file
2. Specify its instruction capabilities using bitfields
3. Define its register set with proper mappings to virtual registers
4. Specify ABIs and syscall interfaces
5. Define target-specific extensions

Example of adding an ARM64 target:

```
target arm64 {
  arch = COIL_CPU_ARCH_ARM
  word_size = 64
  endianness = COIL_ENDIAN_LITTLE
  
  // Instruction capabilities
  instr_int_1 = 0xFFFFFFFFFFFFFFF7  // All but a few integer instructions
  instr_fp = 0x1FFFFF               // All FP instructions
  // More capabilities...
  
  // Register set
  registers {
    gp_regs = [
      { type = COIL_REG_GP, index = 0, name = "x0", size = 64 },
      { type = COIL_REG_GP, index = 1, name = "x1", size = 64 },
      // More registers...
    ]
    
    // Virtual register mappings
    virtual_mappings = [
      { virtual_name = "r0", target_name = "x0" },
      { virtual_name = "r1", target_name = "x1" },
      // More mappings...
    ]
  }
  
  // ABI definitions
  abis = [
    {
      name = "aapcs64"
      // AAPCS64 ABI details...
    }
  ]
  
  // More ARM64-specific configuration...
}
```

## Conclusion

The COIL Configuration Format provides a comprehensive, structured approach to defining target architectures, instruction capabilities, ABIs, and system interfaces. By using a C-structure-inspired format with detailed bitfields for capabilities, it enables precise control over code generation while maintaining cross-platform compatibility.

This approach makes it easy to:
1. Add new target architectures
2. Specify detailed hardware capabilities
3. Define complex register mappings
4. Configure multiple ABIs and syscall interfaces
5. Enable target-specific optimizations

The configuration system forms the backbone of COIL's ability to generate efficient code across diverse architectures while maintaining a consistent programming model.


## Key Configuration Components

### Target Definition

The target definition specifies the basic properties of a hardware architecture:

```
target <name> {
    word_size = <bits>        // Natural word size (typically 32 or 64)
    endian = <little|big>     // Endianness
    features = [<feature>, ...] // Supported hardware features
    default_abi = <abi_name>  // Default ABI for this target
}
```

### Register Definition

The register definition specifies the available registers for a target:

```
registers <target_name> {
    general {
        size = <bits>               // Size in bits
        names = [<reg>, ...]        // Available general registers
        aliases { ... }             // Register aliases
        special { ... }             // Special register mappings
    }
    
    floating {
        size = <bits>               // Size in bits
        names = [<reg>, ...]        // Available floating-point registers
        aliases { ... }             // Register aliases
    }
    
    flags {
        names = [<flag>, ...]       // Flag registers or bits
    }
    
    virtual {
        <vreg> = <preg>             // Virtual to physical register mapping
        // More mappings...
    }
}
```

### ABI Definition

The ABI definition specifies how functions are called:

```
abi <name> {
    target = <target_name>           // Target this ABI applies to
    
    parameters {
        integer = [<reg>, ...]       // Registers for integer parameters
        floating = [<reg>, ...]      // Registers for floating-point parameters
        stack_alignment = <bytes>    // Stack alignment requirement
        shadow_space = <bytes>       // Shadow space for register parameters
    }
    
    returns {
        integer = <reg>              // Register for integer return value
        floating = <reg>             // Register for floating-point return value
        large = [<method>, <reg>]    // Method for returning large values
    }
    
    preserved {
        integer = [<reg>, ...]       // Callee-preserved integer registers
        floating = [<reg>, ...]      // Callee-preserved floating-point registers
    }
    
    rules {
        red_zone = <bytes>           // Size of the red zone
        stack_canary = <bool>        // Whether stack canaries are used
        tail_call = <bool>           // Whether tail call optimization is supported
    }
}
```

### Instruction Mapping

The instruction mapping defines how COIL instructions map to native instructions:

```
instructions <target_name> {
    <instruction> {
        <operand_type> {
            encoding = "<encoding>"    // Native instruction encoding
            operands = [<op>, ...]     // Operand types
            flags = [<flag>, ...]      // Affected flags
        }
        
        // Other operand type variants...
    }
    
    // More instructions...
}
```

### Feature Definition

The feature definition specifies hardware features:

```
feature <name> {
    target = <target_name>          // Target this feature applies to
    registers = [<reg>, ...]        // Registers added by this feature
    instructions = [<inst>, ...]    // Instructions added by this feature
}
```

### System Interface

The system interface definition specifies how to interact with the operating system:

```
syscall <name> {
    target = <target_name>          // Target this interface applies to
    number_reg = <reg>              // Register for syscall number
    args = [<reg>, ...]             // Registers for syscall arguments
    result = <reg>                  // Register for syscall result
    error = <expression>            // Error return convention
    trap = <instruction>            // Instruction to invoke syscall
    
    calls {
        <name> = <number>           // Syscall name to number mapping
        // More syscalls...
    }
}
```

## Configuration Usage

The COIL configuration file is used by several components of the COIL toolchain:

1. **COIL Assembler**: Uses the configuration to generate correct machine code for each target
2. **COIL Optimizer**: Uses the configuration to perform target-specific optimizations
3. **COIL Linker**: Uses the configuration to correctly handle ABIs and linking conventions
4. **Debugger**: Uses the configuration to map between COIL and native representations

## Configuration Inheritance and Overrides

COIL configurations support inheritance and overrides to facilitate reuse and customization:

```
config "custom_config" {
    base = "main_config"
    version = "1.0.0-custom"
}

// Extend the x86_64 target with custom settings
target x86_64 {
    extends = "main_config.x86_64"
    features += [avx512f, avx512bw]
}

// Override ABI for specific use case
abi system_v {
    extends = "main_config.system_v"
    rules.red_zone = 0  // Disable red zone for kernel code
}
```

## Configuration Variables

COIL configurations can include variables for more flexible definitions:

```
config "parametric_config" {
    version = "1.0.0"
    vars {
        WORD_SIZE = 64
        VECTOR_SIZE = 256
    }
}

target x86_64 {
    word_size = ${WORD_SIZE}
    // Other settings...
}

registers x86_64 {
    vector {
        size = ${VECTOR_SIZE}
        // Other settings...
    }
}
```

## Multiple Targets in One Configuration

A single COIL configuration file can define multiple targets, enabling multi-architecture projects:

```
config "heterogeneous_config" {
    version = "1.0.0"
    description = "Configuration for heterogeneous CPU/GPU system"
}

target x86_64 { ... }
target cuda_gpu { ... }

// Mappings between targets
target_interface cpu_to_gpu {
    source = x86_64
    destination = cuda_gpu
    
    // Data transfer mechanisms
    transfer {
        method = "memcpy"
        source_prepare = [...]
        destination_prepare = [...]
    }
    
    // Function call mechanisms
    call {
        method = "kernel_launch"
        parameters = [...]
    }
}
```

## Configuration Validation

The COIL toolchain includes a configuration validator that checks for:

1. **Completeness**: Ensures all required information is present
2. **Consistency**: Ensures there are no contradictions or conflicts
3. **Correctness**: Ensures mappings and definitions are technically correct
4. **Compatibility**: Ensures different components work together

## Example: Complete Configuration for x86_64

Here's a more detailed example of an x86_64 configuration:

```
config "x86_64_complete" {
    version = "1.0.0"
    description = "Complete x86_64 configuration for COIL"
}

target x86_64 {
    word_size = 64
    endian = little
    features = [mmx, sse, sse2, sse3, ssse3, sse4.1, sse4.2, avx, avx2, fma]
    default_abi = system_v
}

registers x86_64 {
    general {
        size = 64
        names = [rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15]
        
        aliases {
            // 32-bit aliases
            eax = rax[31:0]
            ebx = rbx[31:0]
            ecx = rcx[31:0]
            edx = rdx[31:0]
            esi = rsi[31:0]
            edi = rdi[31:0]
            ebp = rbp[31:0]
            esp = rsp[31:0]
            r8d = r8[31:0]
            r9d = r9[31:0]
            r10d = r10[31:0]
            r11d = r11[31:0]
            r12d = r12[31:0]
            r13d = r13[31:0]
            r14d = r14[31:0]
            r15d = r15[31:0]
            
            // 16-bit aliases
            ax = rax[15:0]
            bx = rbx[15:0]
            cx = rcx[15:0]
            dx = rdx[15:0]
            si = rsi[15:0]
            di = rdi[15:0]
            bp = rbp[15:0]
            sp = rsp[15:0]
            r8w = r8[15:0]
            r9w = r9[15:0]
            r10w = r10[15:0]
            r11w = r11[15:0]
            r12w = r12[15:0]
            r13w = r13[15:0]
            r14w = r14[15:0]
            r15w = r15[15:0]
            
            // 8-bit aliases
            al = rax[7:0]
            bl = rbx[7:0]
            cl = rcx[7:0]
            dl = rdx[7:0]
            sil = rsi[7:0]
            dil = rdi[7:0]
            bpl = rbp[7:0]
            spl = rsp[7:0]
            r8b = r8[7:0]
            r9b = r9[7:0]
            r10b = r10[7:0]
            r11b = r11[7:0]
            r12b = r12[7:0]
            r13b = r13[7:0]
            r14b = r14[7:0]
            r15b = r15[7:0]
            
            ah = rax[15:8]
            bh = rbx[15:8]
            ch = rcx[15:8]
            dh = rdx[15:8]
        }
        
        special {
            pc = rip
            sp = rsp
            fp = rbp
        }
    }
    
    floating {
        size = 128
        names = [xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15]
        
        aliases {
            // AVX 256-bit registers
            ymm0 = [xmm0, xmm0_hi]
            ymm1 = [xmm1, xmm1_hi]
            ymm2 = [xmm2, xmm2_hi]
            ymm3 = [xmm3, xmm3_hi]
            ymm4 = [xmm4, xmm4_hi]
            ymm5 = [xmm5, xmm5_hi]
            ymm6 = [xmm6, xmm6_hi]
            ymm7 = [xmm7, xmm7_hi]
            ymm8 = [xmm8, xmm8_hi]
            ymm9 = [xmm9, xmm9_hi]
            ymm10 = [xmm10, xmm10_hi]
            ymm11 = [xmm11, xmm11_hi]
            ymm12 = [xmm12, xmm12_hi]
            ymm13 = [xmm13, xmm13_hi]
            ymm14 = [xmm14, xmm14_hi]
            ymm15 = [xmm15, xmm15_hi]
        }
    }
    
    flags {
        names = [cf, pf, af, zf, sf, tf, if, df, of]
        aliases {
            carry = cf
            parity = pf
            adjust = af
            zero = zf
            sign = sf
            trap = tf
            interrupt = if
            direction = df
            overflow = of
        }
    }
    
    virtual {
        // General purpose COIL virtual registers to x86_64 mappings
        r0 = rax
        r1 = rcx
        r2 = rdx
        r3 = rbx
        r4 = rsi
        r5 = rdi
        r6 = r8
        r7 = r9
        r8 = r10
        r9 = r11
        r10 = r12
        r11 = r13
        r12 = r14
        r13 = r15
        r14 = rbp
        r15 = rsp
        
        // Floating point COIL virtual registers to x86_64 mappings
        f0 = xmm0
        f1 = xmm1
        f2 = xmm2
        f3 = xmm3
        f4 = xmm4
        f5 = xmm5
        f6 = xmm6
        f7 = xmm7
        f8 = xmm8
        f9 = xmm9
        f10 = xmm10
        f11 = xmm11
        f12 = xmm12
        f13 = xmm13
        f14 = xmm14
        f15 = xmm15
        
        // Vector COIL virtual registers to x86_64 mappings
        v0 = ymm0
        v1 = ymm1
        v2 = ymm2
        v3 = ymm3
        v4 = ymm4
        v5 = ymm5
        v6 = ymm6
        v7 = ymm7
        v8 = ymm8
        v9 = ymm9
        v10 = ymm10
        v11 = ymm11
        v12 = ymm12
        v13 = ymm13
        v14 = ymm14
        v15 = ymm15
    }
}

// System V ABI for x86_64
abi system_v {
    target = x86_64
    
    parameters {
        integer = [rdi, rsi, rdx, rcx, r8, r9]
        floating = [xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7]
        stack_alignment = 16
        shadow_space = 0
    }
    
    returns {
        integer = rax
        floating = xmm0
        large = [indirect, rdi]
    }
    
    preserved {
        integer = [rbx, rsp, rbp, r12, r13, r14, r15]
        floating = []
    }
    
    rules {
        red_zone = 128
        stack_canary = true
        tail_call = true
        name_mangling = "_<name>"
    }
}

// Windows x64 ABI
abi win64 {
    target = x86_64
    
    parameters {
        integer = [rcx, rdx, r8, r9]
        floating = [xmm0, xmm1, xmm2, xmm3]
        stack_alignment = 16
        shadow_space = 32
    }
    
    returns {
        integer = rax
        floating = xmm0
        large = [indirect, rcx]
    }
    
    preserved {
        integer = [rbx, rsp, rbp, rsi, rdi, r12, r13, r14, r15]
        floating = [xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15]
    }
    
    rules {
        red_zone = 0
        stack_canary = true
        tail_call = false
        name_mangling = "<name>"
    }
}
```

## Conclusion

The COIL Configuration Format provides a powerful, flexible system for defining the properties and behaviors of target architectures, ABIs, and other essential components of the COIL system. By centralizing this information in a structured, maintainable format, COIL enables consistent code generation across diverse hardware platforms while maximizing performance and compatibility.

This configuration system is a key enabler of COIL's cross-architecture and cross-processing unit capabilities, allowing developers to write code once and have it efficiently compiled for any supported target architecture with appropriate optimizations and adaptations.