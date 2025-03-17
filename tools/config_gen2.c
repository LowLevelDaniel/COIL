/**
* @file cpu_config.h
* @brief Comprehensive CPU configuration for cross-architecture code generation
*
* This file defines structures that describe the capabilities and
* characteristics of target CPU architectures, using bitfields for 
* efficient storage. The system is designed to represent hardware 
* features to inform code generation decisions without relying on
* architecture-specific extension flags.
* 
* @version 1.1.0
*/

#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

/**
* @brief Version information for the configuration structure
* Used to ensure compatibility between different versions of the library
*/
typedef struct config_version {
  uint8_t major;                   /**< Major version (incompatible API changes) */
  uint8_t minor;                   /**< Minor version (backwards-compatible functionality) */
  uint8_t patch;                   /**< Patch version (backwards-compatible bug fixes) */
} config_version_t;

/**
* @brief Current version of the CPU configuration structure
*/
#define CONFIG_VERSION_MAJOR 1
#define CONFIG_VERSION_MINOR 1
#define CONFIG_VERSION_PATCH 0

/**
* @brief Supported CPU architectures
*/
typedef enum config_cpu_arch {
  CONFIG_CPU_ARCH_X86,            /**< x86 and x86-64 */
  CONFIG_CPU_ARCH_ARM,            /**< ARM and ARM64 */
  CONFIG_CPU_ARCH_RISCV,          /**< RISC-V */
  CONFIG_CPU_ARCH_POWERPC,        /**< PowerPC and POWER */
  CONFIG_CPU_ARCH_MIPS,           /**< MIPS */
  CONFIG_CPU_ARCH_SPARC,          /**< SPARC */
  CONFIG_CPU_ARCH_WASM,           /**< WebAssembly */
  CONFIG_CPU_ARCH_AVR,            /**< 8-bit AVR microcontroller */
  CONFIG_CPU_ARCH_MSP430,         /**< 16-bit MSP430 microcontroller */
  CONFIG_CPU_ARCH_S390X,          /**< IBM System Z (s390x) */
  CONFIG_CPU_ARCH_XTENSA,         /**< Xtensa (ESP32, etc.) */
  CONFIG_CPU_ARCH_ALPHA,          /**< DEC Alpha */
  CONFIG_CPU_ARCH_ITANIUM,        /**< Intel Itanium (IA-64) */
  CONFIG_CPU_ARCH_PARISC,         /**< HP PA-RISC */
  CONFIG_CPU_ARCH_HEXAGON,        /**< Qualcomm Hexagon */
  CONFIG_CPU_ARCH_CUSTOM          /**< Custom/proprietary architecture */
} config_cpu_arch_t;

/**
* @brief Register types available in CPUs
*/
typedef enum config_cpu_reg_type {
  CONFIG_CPU_REG_GP,      /**< General purpose registers */
  CONFIG_CPU_REG_FP,      /**< Floating point registers */
  CONFIG_CPU_REG_VEC,     /**< Vector registers */
  CONFIG_CPU_REG_FLAG,    /**< Flag registers */
  CONFIG_CPU_REG_SEG,     /**< Segment registers */
  CONFIG_CPU_REG_PC,      /**< Program counter */
  CONFIG_CPU_REG_SP,      /**< Stack pointer */
  CONFIG_CPU_REG_LR,      /**< Link register */
  CONFIG_CPU_REG_PRED,    /**< Predicate registers */
  CONFIG_CPU_REG_SPECIAL, /**< Special-purpose registers */
  CONFIG_CPU_REG_SYSTEM,  /**< System/control registers */
  CONFIG_CPU_REG_DEBUG,   /**< Debug registers */
  CONFIG_CPU_REG_MASK,    /**< Mask registers (for vector operations) */
  CONFIG_CPU_REG_ACC      /**< Accumulator registers */
} config_cpu_reg_type_t;

/**
* @brief Register descriptor with performance characteristics
*/
typedef struct config_cpu_reg {
  config_cpu_reg_type_t type;    /**< Type of register */
  uint32_t index;                /**< Index of the register within its type */
  char name[16];                 /**< Name of the register (e.g., "rax", "r0") */
  uint32_t size;                 /**< Size of the register in bits */
  uint32_t aliases;              /**< Bitfield of register aliases */
  bool reserved;                 /**< Whether this register is reserved */
  bool partial_access;           /**< Whether parts of register can be accessed */
  uint32_t partial_sizes;        /**< Bitfield of accessible sub-register sizes */
  uint16_t read_latency;         /**< Read latency in cycles (0 = unknown) */
  uint16_t write_latency;        /**< Write latency in cycles (0 = unknown) */
} config_cpu_reg_t;

/**
* @brief Cache replacement policies
*/
typedef enum config_cache_replacement_policy {
  CONFIG_CACHE_REPLACE_LRU,     /**< Least Recently Used */
  CONFIG_CACHE_REPLACE_FIFO,    /**< First In First Out */
  CONFIG_CACHE_REPLACE_RANDOM,  /**< Random replacement */
  CONFIG_CACHE_REPLACE_NRU,     /**< Not Recently Used */
  CONFIG_CACHE_REPLACE_PLRU,    /**< Pseudo-LRU */
  CONFIG_CACHE_REPLACE_MRU,     /**< Most Recently Used */
  CONFIG_CACHE_REPLACE_CUSTOM   /**< Custom policy */
} config_cache_replacement_policy_t;

/**
* @brief Cache write policies
*/
typedef enum config_cache_write_policy {
  CONFIG_CACHE_WRITE_BACK,       /**< Write-back caching */
  CONFIG_CACHE_WRITE_THROUGH,    /**< Write-through caching */
  CONFIG_CACHE_WRITE_AROUND,     /**< Write-around caching */
  CONFIG_CACHE_WRITE_ALLOCATE,   /**< Write-allocate policy */
  CONFIG_CACHE_WRITE_NO_ALLOCATE /**< No-write-allocate policy */
} config_cache_write_policy_t;

/**
* @brief Endianness types
*/
typedef enum config_endianness {
  CONFIG_ENDIAN_LITTLE,         /**< Little-endian byte order */
  CONFIG_ENDIAN_BIG,            /**< Big-endian byte order */
  CONFIG_ENDIAN_BI,             /**< Bi-endian (configurable) */
  CONFIG_ENDIAN_MIXED           /**< Mixed endianness */
} config_endianness_t;

/**
* @brief Memory models
*/
typedef enum config_memory_model {
  CONFIG_MEM_MODEL_RELAXED,     /**< Relaxed memory model */
  CONFIG_MEM_MODEL_SEQUENTIAL,  /**< Sequential consistency */
  CONFIG_MEM_MODEL_TSO,         /**< Total Store Order */
  CONFIG_MEM_MODEL_PSO,         /**< Partial Store Order */
  CONFIG_MEM_MODEL_RMO,         /**< Relaxed Memory Order */
  CONFIG_MEM_MODEL_WMO,         /**< Weak Memory Order */
  CONFIG_MEM_MODEL_RELEASE,     /**< Release consistency */
  CONFIG_MEM_MODEL_ACQUIRE,     /**< Acquire consistency */
  CONFIG_MEM_MODEL_RLSE_CONS,   /**< Release-acquire consistency */
  CONFIG_MEM_MODEL_WEAK_RLSE,   /**< Weak release consistency */
  CONFIG_MEM_MODEL_CUSTOM       /**< Custom/architecture-specific */
} config_memory_model_t;

/**
* @brief Privilege levels
*/
typedef enum config_privilege_level {
  CONFIG_PRIV_USER,             /**< User/application level */
  CONFIG_PRIV_SUPERVISOR,       /**< Supervisor mode */
  CONFIG_PRIV_HYPERVISOR,       /**< Hypervisor mode */
  CONFIG_PRIV_MACHINE,          /**< Machine/kernel mode */
  CONFIG_PRIV_SECURE_MONITOR,   /**< Secure monitor mode */
  CONFIG_PRIV_CUSTOM            /**< Custom privilege level */
} config_privilege_level_t;

/**************************************************************************
* Instruction capability bitfields - Part 1 (basic functionality)
**************************************************************************/

/**
* @brief Integer instruction capabilities - Part 1
*/
typedef enum config_instr_int_1 {
  /* Basic arithmetic */
  CONFIG_INSTR_INT_ADD              = (1ULL << 0),  /**< Addition */
  CONFIG_INSTR_INT_SUB              = (1ULL << 1),  /**< Subtraction */
  CONFIG_INSTR_INT_MUL              = (1ULL << 2),  /**< Multiplication */
  CONFIG_INSTR_INT_DIV              = (1ULL << 3),  /**< Division */
  CONFIG_INSTR_INT_REM              = (1ULL << 4),  /**< Remainder/Modulo */
  CONFIG_INSTR_INT_NEG              = (1ULL << 5),  /**< Negation */
  CONFIG_INSTR_INT_ABS              = (1ULL << 6),  /**< Absolute value */
  CONFIG_INSTR_INT_MIN              = (1ULL << 7),  /**< Minimum */
  CONFIG_INSTR_INT_MAX              = (1ULL << 8),  /**< Maximum */
  
  /* Extended precision */
  CONFIG_INSTR_INT_MULADD           = (1ULL << 9),  /**< Multiply-add */
  CONFIG_INSTR_INT_MULSUB           = (1ULL << 10), /**< Multiply-subtract */
  CONFIG_INSTR_INT_MADD             = (1ULL << 11), /**< Multiply-accumulate */
  CONFIG_INSTR_INT_MSUB             = (1ULL << 12), /**< Multiply-subtract-accumulate */
  CONFIG_INSTR_INT_ADDC             = (1ULL << 13), /**< Add with carry */
  CONFIG_INSTR_INT_SUBC             = (1ULL << 14), /**< Subtract with carry/borrow */
  CONFIG_INSTR_INT_MULH             = (1ULL << 15), /**< Multiply high */
  
  /* Bitwise operations */
  CONFIG_INSTR_INT_AND              = (1ULL << 16), /**< Bitwise AND */
  CONFIG_INSTR_INT_OR               = (1ULL << 17), /**< Bitwise OR */
  CONFIG_INSTR_INT_XOR              = (1ULL << 18), /**< Bitwise XOR */
  CONFIG_INSTR_INT_NOT              = (1ULL << 19), /**< Bitwise NOT */
  CONFIG_INSTR_INT_ANDN             = (1ULL << 20), /**< AND with complement */
  CONFIG_INSTR_INT_ORN              = (1ULL << 21), /**< OR with complement */
  CONFIG_INSTR_INT_XNOR             = (1ULL << 22), /**< Exclusive NOR */
  
  /* Shifts and rotates */
  CONFIG_INSTR_INT_SHL              = (1ULL << 23), /**< Shift left logical */
  CONFIG_INSTR_INT_SHR              = (1ULL << 24), /**< Shift right logical */
  CONFIG_INSTR_INT_SAR              = (1ULL << 25), /**< Shift right arithmetic */
  CONFIG_INSTR_INT_ROL              = (1ULL << 26), /**< Rotate left */
  CONFIG_INSTR_INT_ROR              = (1ULL << 27), /**< Rotate right */
  
  /* Bit manipulation */
  CONFIG_INSTR_INT_BIT_SET          = (1ULL << 28), /**< Set bit */
  CONFIG_INSTR_INT_BIT_CLR          = (1ULL << 29), /**< Clear bit */
  CONFIG_INSTR_INT_BIT_TOGGLE       = (1ULL << 30), /**< Toggle bit */
  CONFIG_INSTR_INT_BIT_TEST         = (1ULL << 31), /**< Test bit */
  CONFIG_INSTR_INT_BIT_EXTRACT      = (1ULL << 32), /**< Extract bits */
  CONFIG_INSTR_INT_BIT_INSERT       = (1ULL << 33), /**< Insert bits */
  CONFIG_INSTR_INT_POPCOUNT         = (1ULL << 34), /**< Population count */
  CONFIG_INSTR_INT_CLZ              = (1ULL << 35), /**< Count leading zeros */
  CONFIG_INSTR_INT_CTZ              = (1ULL << 36), /**< Count trailing zeros */
  CONFIG_INSTR_INT_BYTE_SWAP        = (1ULL << 37), /**< Byte swap */
  
  /* Saturating arithmetic */
  CONFIG_INSTR_INT_SADD             = (1ULL << 38), /**< Saturating add */
  CONFIG_INSTR_INT_SSUB             = (1ULL << 39), /**< Saturating subtract */
  CONFIG_INSTR_INT_SMUL             = (1ULL << 40), /**< Saturating multiply */
  
  /* Signed/unsigned operations */
  CONFIG_INSTR_INT_SIGNED_DIV       = (1ULL << 41), /**< Signed division */
  CONFIG_INSTR_INT_UNSIGNED_DIV     = (1ULL << 42), /**< Unsigned division */
  CONFIG_INSTR_INT_SIGNED_REM       = (1ULL << 43), /**< Signed remainder */
  CONFIG_INSTR_INT_UNSIGNED_REM     = (1ULL << 44), /**< Unsigned remainder */
  CONFIG_INSTR_INT_SIGNED_COMPARE   = (1ULL << 45), /**< Signed comparison */
  CONFIG_INSTR_INT_UNSIGNED_COMPARE = (1ULL << 46), /**< Unsigned comparison */
  
  /* Memory operations */
  CONFIG_INSTR_INT_LOAD             = (1ULL << 47), /**< Load from memory */
  CONFIG_INSTR_INT_STORE            = (1ULL << 48), /**< Store to memory */
  CONFIG_INSTR_INT_PREFETCH         = (1ULL << 49), /**< Prefetch */
  CONFIG_INSTR_INT_MOVE             = (1ULL << 50), /**< Move register to register */
  CONFIG_INSTR_INT_LOAD_IMM         = (1ULL << 51), /**< Load immediate value */
  CONFIG_INSTR_INT_EXCHANGE         = (1ULL << 52), /**< Exchange/swap values */
  CONFIG_INSTR_INT_PUSH             = (1ULL << 53), /**< Push to stack */
  CONFIG_INSTR_INT_POP              = (1ULL << 54), /**< Pop from stack */
  
  /* Atomic operations */
  CONFIG_INSTR_INT_ATOMIC_ADD       = (1ULL << 55), /**< Atomic add */
  CONFIG_INSTR_INT_ATOMIC_SUB       = (1ULL << 56), /**< Atomic subtract */
  CONFIG_INSTR_INT_ATOMIC_AND       = (1ULL << 57), /**< Atomic AND */
  CONFIG_INSTR_INT_ATOMIC_OR        = (1ULL << 58), /**< Atomic OR */
  CONFIG_INSTR_INT_ATOMIC_XOR       = (1ULL << 59), /**< Atomic XOR */
  CONFIG_INSTR_INT_ATOMIC_XCHG      = (1ULL << 60), /**< Atomic exchange */
  CONFIG_INSTR_INT_ATOMIC_CMPXCHG   = (1ULL << 61), /**< Atomic compare-exchange */
  CONFIG_INSTR_INT_ATOMIC_FETCH_ADD = (1ULL << 62), /**< Atomic fetch-and-add */
  CONFIG_INSTR_INT_RANDOM           = (1ULL << 63), /**< Random integer generation */
} config_instr_int_1_t;

/**
* @brief Integer instruction capabilities - Part 2 (extended functionality)
*/
typedef enum config_instr_int_2 {
  /* Advanced atomic operations */
  CONFIG_INSTR_INT_ATOMIC_FETCH_SUB = (1ULL << 0),  /**< Atomic fetch-and-subtract */
  CONFIG_INSTR_INT_ATOMIC_FETCH_AND = (1ULL << 1),  /**< Atomic fetch-and-AND */
  CONFIG_INSTR_INT_ATOMIC_FETCH_OR  = (1ULL << 2),  /**< Atomic fetch-and-OR */
  CONFIG_INSTR_INT_ATOMIC_FETCH_XOR = (1ULL << 3),  /**< Atomic fetch-and-XOR */
  CONFIG_INSTR_INT_ATOMIC_FETCH_NAND = (1ULL << 4), /**< Atomic fetch-and-NAND */
  CONFIG_INSTR_INT_ATOMIC_MIN       = (1ULL << 5),  /**< Atomic minimum */
  CONFIG_INSTR_INT_ATOMIC_MAX       = (1ULL << 6),  /**< Atomic maximum */
  CONFIG_INSTR_INT_ATOMIC_INC       = (1ULL << 7),  /**< Atomic increment */
  CONFIG_INSTR_INT_ATOMIC_DEC       = (1ULL << 8),  /**< Atomic decrement */
  
  /* Memory ordering and atomicity */
  CONFIG_INSTR_INT_MEMBAR_ACQ       = (1ULL << 9),  /**< Memory barrier - acquire */
  CONFIG_INSTR_INT_MEMBAR_REL       = (1ULL << 10), /**< Memory barrier - release */
  CONFIG_INSTR_INT_MEMBAR_ACQREL    = (1ULL << 11), /**< Memory barrier - acquire-release */
  CONFIG_INSTR_INT_MEMBAR_SEQ_CST   = (1ULL << 12), /**< Memory barrier - sequential consistency */
  
  /* Advanced bit manipulation */
  CONFIG_INSTR_INT_BIT_FIELD_EXTRACT = (1ULL << 13), /**< Extract contiguous bit field */
  CONFIG_INSTR_INT_BIT_FIELD_INSERT = (1ULL << 14), /**< Insert contiguous bit field */
  CONFIG_INSTR_INT_BIT_REVERSE      = (1ULL << 15), /**< Reverse bit order */
  CONFIG_INSTR_INT_BYTE_REVERSE     = (1ULL << 16), /**< Reverse byte order */
  CONFIG_INSTR_INT_PARITY           = (1ULL << 17), /**< Calculate parity */
  
  /* Advanced arithmetic */
  CONFIG_INSTR_INT_SQRT             = (1ULL << 18), /**< Integer square root */
  CONFIG_INSTR_INT_POPCNT_ADD       = (1ULL << 19), /**< Add population count */
  CONFIG_INSTR_INT_CARRY_LESS_MUL   = (1ULL << 20), /**< Carry-less multiplication */
  CONFIG_INSTR_INT_CHECKSUM         = (1ULL << 21), /**< Checksum calculation */
  CONFIG_INSTR_INT_CRC              = (1ULL << 22), /**< Cyclic redundancy check */
  
  /* Specialized memory access */
  CONFIG_INSTR_INT_UNALIGNED_LOAD   = (1ULL << 23), /**< Unaligned memory load */
  CONFIG_INSTR_INT_UNALIGNED_STORE  = (1ULL << 24), /**< Unaligned memory store */
  CONFIG_INSTR_INT_NON_TEMPORAL_LOAD = (1ULL << 25), /**< Non-temporal load */
  CONFIG_INSTR_INT_NON_TEMPORAL_STORE = (1ULL << 26), /**< Non-temporal store */
  CONFIG_INSTR_INT_LOCKED_LOAD      = (1ULL << 27), /**< Locked load */
  CONFIG_INSTR_INT_LOCKED_STORE     = (1ULL << 28), /**< Locked store */
  
  /* Advanced execution control */
  CONFIG_INSTR_INT_PREDICATED_EXEC  = (1ULL << 29), /**< Predicated execution */
  CONFIG_INSTR_INT_CMOV             = (1ULL << 30), /**< Conditional move */
  CONFIG_INSTR_INT_SELECT           = (1ULL << 31), /**< Value selection based on condition */
  
  /* Miscellaneous */
  CONFIG_INSTR_INT_BSWAP16          = (1ULL << 32), /**< 16-bit byte swap */
  CONFIG_INSTR_INT_BSWAP32          = (1ULL << 33), /**< 32-bit byte swap */
  CONFIG_INSTR_INT_BSWAP64          = (1ULL << 34), /**< 64-bit byte swap */
  CONFIG_INSTR_INT_BSWAP128         = (1ULL << 35), /**< 128-bit byte swap */
  
  /* Future expansion - reserved */
} config_instr_int_2_t;

/**
* @brief Floating-point instruction capabilities - Part 1
*/
typedef enum config_instr_fp_1 {
  /* Basic floating point */
  CONFIG_INSTR_FP_ADD           = (1ULL << 0),  /**< FP addition */
  CONFIG_INSTR_FP_SUB           = (1ULL << 1),  /**< FP subtraction */
  CONFIG_INSTR_FP_MUL           = (1ULL << 2),  /**< FP multiplication */
  CONFIG_INSTR_FP_DIV           = (1ULL << 3),  /**< FP division */
  CONFIG_INSTR_FP_REM           = (1ULL << 4),  /**< FP remainder */
  CONFIG_INSTR_FP_NEG           = (1ULL << 5),  /**< FP negation */
  CONFIG_INSTR_FP_ABS           = (1ULL << 6),  /**< FP absolute value */
  CONFIG_INSTR_FP_SQRT          = (1ULL << 7),  /**< FP square root */
  CONFIG_INSTR_FP_RSQRT         = (1ULL << 8),  /**< FP reciprocal square root */
  CONFIG_INSTR_FP_RECIP         = (1ULL << 9),  /**< FP reciprocal */
  
  /* Advanced floating point */
  CONFIG_INSTR_FP_FMA           = (1ULL << 10), /**< FP fused multiply-add */
  CONFIG_INSTR_FP_FMS           = (1ULL << 11), /**< FP fused multiply-subtract */
  CONFIG_INSTR_FP_FMADD         = (1ULL << 12), /**< FP fused multiply-add-add */
  CONFIG_INSTR_FP_FMSUB         = (1ULL << 13), /**< FP fused multiply-sub-add */
  CONFIG_INSTR_FP_CMP           = (1ULL << 14), /**< FP compare */
  CONFIG_INSTR_FP_MIN           = (1ULL << 15), /**< FP minimum */
  CONFIG_INSTR_FP_MAX           = (1ULL << 16), /**< FP maximum */
  
  /* Transcendental functions */
  CONFIG_INSTR_FP_SIN           = (1ULL << 17), /**< FP sine */
  CONFIG_INSTR_FP_COS           = (1ULL << 18), /**< FP cosine */
  CONFIG_INSTR_FP_TAN           = (1ULL << 19), /**< FP tangent */
  CONFIG_INSTR_FP_EXP           = (1ULL << 20), /**< FP exponential */
  CONFIG_INSTR_FP_LOG           = (1ULL << 21), /**< FP logarithm */
  CONFIG_INSTR_FP_LOG2          = (1ULL << 22), /**< FP base-2 logarithm */
  CONFIG_INSTR_FP_LOG10         = (1ULL << 23), /**< FP base-10 logarithm */
  
  /* Conversion */
  CONFIG_INSTR_FP_INT_TO_FP     = (1ULL << 24), /**< Integer to FP conversion */
  CONFIG_INSTR_FP_FP_TO_INT     = (1ULL << 25), /**< FP to integer conversion */
  CONFIG_INSTR_FP_ROUND         = (1ULL << 26), /**< FP rounding */
  CONFIG_INSTR_FP_CEIL          = (1ULL << 27), /**< FP ceiling */
  CONFIG_INSTR_FP_FLOOR         = (1ULL << 28), /**< FP floor */
  CONFIG_INSTR_FP_TRUNC         = (1ULL << 29), /**< FP truncation */
  CONFIG_INSTR_FP_FP_TO_FP      = (1ULL << 30), /**< FP precision conversion */
  
  /* Special values */
  CONFIG_INSTR_FP_ISNAN         = (1ULL << 31), /**< Test for NaN */
  CONFIG_INSTR_FP_ISINF         = (1ULL << 32), /**< Test for infinity */
  CONFIG_INSTR_FP_ISFINITE      = (1ULL << 33), /**< Test for finite value */
  CONFIG_INSTR_FP_CLASSIFY      = (1ULL << 34), /**< Classify FP value */
  CONFIG_INSTR_FP_COPYSIGN      = (1ULL << 35), /**< Copy sign */
  
  /* Memory operations */
  CONFIG_INSTR_FP_LOAD          = (1ULL << 36), /**< Load from memory */
  CONFIG_INSTR_FP_STORE         = (1ULL << 37), /**< Store to memory */
  CONFIG_INSTR_FP_PREFETCH      = (1ULL << 38), /**< Prefetch */
  CONFIG_INSTR_FP_MOVE          = (1ULL << 39), /**< Move register to register */
  CONFIG_INSTR_FP_LOAD_IMM      = (1ULL << 40), /**< Load immediate value */
  CONFIG_INSTR_FP_EXCHANGE      = (1ULL << 41), /**< Exchange/swap values */
  
  /* Atomic operations (future-proofing for architectures that might support FP atomics) */
  CONFIG_INSTR_FP_ATOMIC_ADD    = (1ULL << 42), /**< Atomic add */
  CONFIG_INSTR_FP_ATOMIC_SUB    = (1ULL << 43), /**< Atomic subtract */
  CONFIG_INSTR_FP_ATOMIC_MIN    = (1ULL << 44), /**< Atomic minimum */
  CONFIG_INSTR_FP_ATOMIC_MAX    = (1ULL << 45), /**< Atomic maximum */
  CONFIG_INSTR_FP_ATOMIC_XCHG   = (1ULL << 46), /**< Atomic exchange */
  CONFIG_INSTR_FP_ATOMIC_CAS    = (1ULL << 47), /**< Atomic compare-and-swap */
  
  /* Random number generation */
  CONFIG_INSTR_FP_RANDOM        = (1ULL << 48), /**< Random floating-point generation */
} config_instr_fp_1_t;

/**
* @brief Floating-point instruction capabilities - Part 2
*/
typedef enum config_instr_fp_2 {
  /* Special operations */
  CONFIG_INSTR_FP_FUSED_OP      = (1ULL << 0),  /**< Fused operations beyond FMA */
  CONFIG_INSTR_FP_FAST_MATH     = (1ULL << 1),  /**< Fast math mode available */
  CONFIG_INSTR_FP_SUBNORMAL     = (1ULL << 2),  /**< Subnormal number support */
  CONFIG_INSTR_FP_EXCEPTIONS    = (1ULL << 3),  /**< FP exceptions */
  CONFIG_INSTR_FP_ROUND_MODES   = (1ULL << 4),  /**< Multiple rounding modes */
  
  /* Advanced transcendental functions */
  CONFIG_INSTR_FP_ASIN          = (1ULL << 5),  /**< Arc sine */
  CONFIG_INSTR_FP_ACOS          = (1ULL << 6),  /**< Arc cosine */
  CONFIG_INSTR_FP_ATAN          = (1ULL << 7),  /**< Arc tangent */
  CONFIG_INSTR_FP_ATAN2         = (1ULL << 8),  /**< Two-argument arc tangent */
  CONFIG_INSTR_FP_SINH          = (1ULL << 9),  /**< Hyperbolic sine */
  CONFIG_INSTR_FP_COSH          = (1ULL << 10), /**< Hyperbolic cosine */
  CONFIG_INSTR_FP_TANH          = (1ULL << 11), /**< Hyperbolic tangent */
  CONFIG_INSTR_FP_ASINH         = (1ULL << 12), /**< Inverse hyperbolic sine */
  CONFIG_INSTR_FP_ACOSH         = (1ULL << 13), /**< Inverse hyperbolic cosine */
  CONFIG_INSTR_FP_ATANH         = (1ULL << 14), /**< Inverse hyperbolic tangent */
  CONFIG_INSTR_FP_POW           = (1ULL << 15), /**< Power function */
  CONFIG_INSTR_FP_CBRT          = (1ULL << 16), /**< Cube root */
  
  /* Extended precision operations */
  CONFIG_INSTR_FP_ACCUM         = (1ULL << 17), /**< Accumulation with extended precision */
  CONFIG_INSTR_FP_DOT_PRODUCT   = (1ULL << 18), /**< Dot product */
  CONFIG_INSTR_FP_SUM           = (1ULL << 19), /**< Sum reduction */
  
  /* Special memory access */
  CONFIG_INSTR_FP_UNALIGNED_LOAD = (1ULL << 20), /**< Unaligned memory load */
  CONFIG_INSTR_FP_UNALIGNED_STORE = (1ULL << 21), /**< Unaligned memory store */
  CONFIG_INSTR_FP_NON_TEMPORAL_LOAD = (1ULL << 22), /**< Non-temporal load */
  CONFIG_INSTR_FP_NON_TEMPORAL_STORE = (1ULL << 23), /**< Non-temporal store */
  
  /* Alternative formats */
  CONFIG_INSTR_FP_ALT_ENCODING  = (1ULL << 24), /**< Alternative encoding (e.g., bfloat16) */
  
  /* Future expansion - reserved */
} config_instr_fp_2_t;

/**
* @brief SIMD/Vector instruction capabilities - Part 1
*/
typedef enum config_instr_vector_1 {
  /* Vector basics */
  CONFIG_INSTR_VEC_ADD          = (1ULL << 0),  /**< Vector add */
  CONFIG_INSTR_VEC_SUB          = (1ULL << 1),  /**< Vector subtract */
  CONFIG_INSTR_VEC_MUL          = (1ULL << 2),  /**< Vector multiply */
  CONFIG_INSTR_VEC_DIV          = (1ULL << 3),  /**< Vector divide */
  CONFIG_INSTR_VEC_ABS          = (1ULL << 4),  /**< Vector absolute value */
  CONFIG_INSTR_VEC_NEG          = (1ULL << 5),  /**< Vector negate */
  
  /* Vector bitwise */
  CONFIG_INSTR_VEC_AND          = (1ULL << 6),  /**< Vector AND */
  CONFIG_INSTR_VEC_OR           = (1ULL << 7),  /**< Vector OR */
  CONFIG_INSTR_VEC_XOR          = (1ULL << 8),  /**< Vector XOR */
  CONFIG_INSTR_VEC_NOT          = (1ULL << 9),  /**< Vector NOT */
  CONFIG_INSTR_VEC_SHIFT        = (1ULL << 10), /**< Vector shifts */
  
  /* Vector advanced arithmetic */
  CONFIG_INSTR_VEC_FMA          = (1ULL << 11), /**< Vector fused multiply-add */
  CONFIG_INSTR_VEC_MAC          = (1ULL << 12), /**< Vector multiply-accumulate */
  CONFIG_INSTR_VEC_DOT          = (1ULL << 13), /**< Vector dot product */
  CONFIG_INSTR_VEC_MADD         = (1ULL << 14), /**< Vector multiply-add */
  CONFIG_INSTR_VEC_MSUB         = (1ULL << 15), /**< Vector multiply-subtract */
  CONFIG_INSTR_VEC_SAD          = (1ULL << 16), /**< Vector sum of abs differences */
  
  /* Vector comparison */
  CONFIG_INSTR_VEC_CMP          = (1ULL << 17), /**< Vector compare */
  CONFIG_INSTR_VEC_MIN          = (1ULL << 18), /**< Vector minimum */
  CONFIG_INSTR_VEC_MAX          = (1ULL << 19), /**< Vector maximum */
  CONFIG_INSTR_VEC_MASK         = (1ULL << 20), /**< Vector mask operations */
  
  /* Vector manipulation */
  CONFIG_INSTR_VEC_SHUFFLE      = (1ULL << 21), /**< Vector shuffle */
  CONFIG_INSTR_VEC_PERMUTE      = (1ULL << 22), /**< Vector permute */
  CONFIG_INSTR_VEC_BLEND        = (1ULL << 23), /**< Vector blend */
  CONFIG_INSTR_VEC_COMPRESS     = (1ULL << 24), /**< Vector compress */
  CONFIG_INSTR_VEC_EXPAND       = (1ULL << 25), /**< Vector expand */
  CONFIG_INSTR_VEC_EXTRACT      = (1ULL << 26), /**< Vector extract element */
  CONFIG_INSTR_VEC_INSERT       = (1ULL << 27), /**< Vector insert element */
  
  /* Vector reduction */
  CONFIG_INSTR_VEC_REDUCE_ADD   = (1ULL << 28), /**< Vector reduce add */
  CONFIG_INSTR_VEC_REDUCE_MUL   = (1ULL << 29), /**< Vector reduce multiply */
  CONFIG_INSTR_VEC_REDUCE_MIN   = (1ULL << 30), /**< Vector reduce minimum */
  CONFIG_INSTR_VEC_REDUCE_MAX   = (1ULL << 31), /**< Vector reduce maximum */
  CONFIG_INSTR_VEC_REDUCE_AND   = (1ULL << 32), /**< Vector reduce AND */
  CONFIG_INSTR_VEC_REDUCE_OR    = (1ULL << 33), /**< Vector reduce OR */
  CONFIG_INSTR_VEC_REDUCE_XOR   = (1ULL << 34), /**< Vector reduce XOR */
  
  /* Vector masking */
  CONFIG_INSTR_VEC_MASKED_OP    = (1ULL << 35), /**< Vector masked operations */
  CONFIG_INSTR_VEC_BLEND_MASK   = (1ULL << 36), /**< Vector blend with mask */
  
  /* Memory operations */
  CONFIG_INSTR_VEC_LOAD         = (1ULL << 37), /**< Vector load */
  CONFIG_INSTR_VEC_STORE        = (1ULL << 38), /**< Vector store */
  CONFIG_INSTR_VEC_GATHER       = (1ULL << 39), /**< Vector gather */
  CONFIG_INSTR_VEC_SCATTER      = (1ULL << 40), /**< Vector scatter */
  CONFIG_INSTR_VEC_PREFETCH     = (1ULL << 41), /**< Vector prefetch */
  CONFIG_INSTR_VEC_MOVE         = (1ULL << 42), /**< Vector move */
  CONFIG_INSTR_VEC_MASKED_LOAD  = (1ULL << 43), /**< Vector masked load */
  CONFIG_INSTR_VEC_MASKED_STORE = (1ULL << 44), /**< Vector masked store */
  CONFIG_INSTR_VEC_NON_TEMPORAL = (1ULL << 45), /**< Non-temporal vector memory access */
  
  /* Atomic operations */
  CONFIG_INSTR_VEC_ATOMIC_ADD   = (1ULL << 46), /**< Vector atomic add */
  CONFIG_INSTR_VEC_ATOMIC_SUB   = (1ULL << 47), /**< Vector atomic subtract */
  CONFIG_INSTR_VEC_ATOMIC_AND   = (1ULL << 48), /**< Vector atomic AND */
  CONFIG_INSTR_VEC_ATOMIC_OR    = (1ULL << 49), /**< Vector atomic OR */
  CONFIG_INSTR_VEC_ATOMIC_XOR   = (1ULL << 50), /**< Vector atomic XOR */
  CONFIG_INSTR_VEC_ATOMIC_MIN   = (1ULL << 51), /**< Vector atomic minimum */
  CONFIG_INSTR_VEC_ATOMIC_MAX   = (1ULL << 52), /**< Vector atomic maximum */
  CONFIG_INSTR_VEC_ATOMIC_XCHG  = (1ULL << 53), /**< Vector atomic exchange */
  
  /* Crypto operations */
  CONFIG_INSTR_VEC_CRYPTO_AES   = (1ULL << 54), /**< AES encryption/decryption */
  CONFIG_INSTR_VEC_CRYPTO_SHA   = (1ULL << 55), /**< SHA hashing */
  CONFIG_INSTR_VEC_CRYPTO_SM4   = (1ULL << 56), /**< SM4 block cipher */
  CONFIG_INSTR_VEC_CRYPTO_PMULL = (1ULL << 57), /**< Polynomial multiply */
  CONFIG_INSTR_VEC_CRYPTO_CRC32 = (1ULL << 58), /**< CRC-32 checksum */
  
  /* Random number generation */
  CONFIG_INSTR_VEC_RANDOM       = (1ULL << 59), /**< Vector random number generation */
} config_instr_vector_1_t;

/**
* @brief SIMD/Vector instruction capabilities - Part 2
*/
typedef enum config_instr_vector_2 {
  /* Advanced vector operations */
  CONFIG_INSTR_VEC_SCALABLE     = (1ULL << 0),  /**< Scalable vector length support */
  CONFIG_INSTR_VEC_PRED_LOOP    = (1ULL << 1),  /**< Predicated loop vectorization */
  CONFIG_INSTR_VEC_MASK_REG     = (1ULL << 2),  /**< Dedicated mask registers */
  CONFIG_INSTR_VEC_STRIP_MINE   = (1ULL << 3),  /**< Automatic strip mining */
  CONFIG_INSTR_VEC_SEGMENT_LOAD = (1ULL << 4),  /**< Segmented vector loads */
  CONFIG_INSTR_VEC_SEGMENT_STORE = (1ULL << 5), /**< Segmented vector stores */
  
  /* Advanced data movement */
  CONFIG_INSTR_VEC_TBL_LOOKUP   = (1ULL << 6),  /**< Table lookup */
  CONFIG_INSTR_VEC_COMPRESS_STORE = (1ULL << 7), /**< Compressed store */
  CONFIG_INSTR_VEC_EXPAND_LOAD   = (1ULL << 8), /**< Expanded load */
  CONFIG_INSTR_VEC_SLIDE        = (1ULL << 9),  /**< Vector slide (up/down) */
  CONFIG_INSTR_VEC_SPLICE       = (1ULL << 10), /**< Vector splice */
  
  /* Advanced arithmetic */
  CONFIG_INSTR_VEC_WIDEN        = (1ULL << 11), /**< Widening operations */
  CONFIG_INSTR_VEC_NARROW       = (1ULL << 12), /**< Narrowing operations */
  CONFIG_INSTR_VEC_SATURATION   = (1ULL << 13), /**< Saturating arithmetic */
  CONFIG_INSTR_VEC_ROUNDING     = (1ULL << 14), /**< Rounding operations */
  CONFIG_INSTR_VEC_SQRT         = (1ULL << 15), /**< Vector square root */
  CONFIG_INSTR_VEC_RECIP_EST    = (1ULL << 16), /**< Reciprocal estimate */
  CONFIG_INSTR_VEC_RSQRT_EST    = (1ULL << 17), /**< Reciprocal square root estimate */
  
  /* Complex number support */
  CONFIG_INSTR_VEC_COMPLEX_MUL  = (1ULL << 18), /**< Complex multiplication */
  CONFIG_INSTR_VEC_COMPLEX_ADD  = (1ULL << 19), /**< Complex addition */
  
  /* Matrix operations */
  CONFIG_INSTR_VEC_OUTER_PRODUCT = (1ULL << 20), /**< Outer product */
  CONFIG_INSTR_VEC_MATRIX_MUL   = (1ULL << 21), /**< Matrix multiplication */
  CONFIG_INSTR_VEC_MATRIX_ADD   = (1ULL << 22), /**< Matrix addition */
  
  /* Cross-lane operations */
  CONFIG_INSTR_VEC_CROSS_LANE   = (1ULL << 23), /**< Cross-lane operations */
  CONFIG_INSTR_VEC_LANE_SHUFFLE = (1ULL << 24), /**< Lane shuffling */
  
  /* Future expansion - reserved */
} config_instr_vector_2_t;

/**
* @brief Memory synchronization and barrier capabilities
* These operations are generally type-independent
*/
typedef enum config_instr_mem_sync {
  /* Memory ordering */
  CONFIG_INSTR_MEM_FENCE            = (1ULL << 0),  /**< Memory fence/barrier */
  CONFIG_INSTR_MEM_ACQUIRE_BARRIER  = (1ULL << 1),  /**< Acquire barrier */
  CONFIG_INSTR_MEM_RELEASE_BARRIER  = (1ULL << 2),  /**< Release barrier */
  CONFIG_INSTR_MEM_DMB              = (1ULL << 3),  /**< Data memory barrier */
  CONFIG_INSTR_MEM_DSB              = (1ULL << 4),  /**< Data synchronization barrier */
  CONFIG_INSTR_MEM_ISB              = (1ULL << 5),  /**< Instruction synchronization barrier */
  CONFIG_INSTR_MEM_SPECULATION_BAR  = (1ULL << 6),  /**< Speculation barrier */
  CONFIG_INSTR_MEM_SYNC_ALL         = (1ULL << 7),  /**< Synchronize all memory accesses */
  CONFIG_INSTR_MEM_SYNC_PROCESS     = (1ULL << 8),  /**< Synchronize process memory */
  CONFIG_INSTR_MEM_SYNC_THREAD      = (1ULL << 9),  /**< Synchronize thread memory */
  CONFIG_INSTR_MEM_SYNC_ACQUIRE     = (1ULL << 10), /**< Acquire semantics */
  CONFIG_INSTR_MEM_SYNC_RELEASE     = (1ULL << 11), /**< Release semantics */
  CONFIG_INSTR_MEM_SYNC_ACQREL      = (1ULL << 12), /**< Acquire-release semantics */
  CONFIG_INSTR_MEM_SYNC_CONSUME     = (1ULL << 13), /**< Consume semantics */
  CONFIG_INSTR_MEM_SYNC_SC          = (1ULL << 14), /**< Sequential consistency */
  CONFIG_INSTR_MEM_SYNC_LWSYNC      = (1ULL << 15), /**< Lightweight sync (PowerPC) */
  CONFIG_INSTR_MEM_SYNC_HWSYNC      = (1ULL << 16), /**< Hardware sync (PowerPC) */
  CONFIG_INSTR_MEM_SYNC_ISYNC       = (1ULL << 17), /**< Instruction sync (PowerPC) */
  CONFIG_INSTR_MEM_SYNC_TBEGIN      = (1ULL << 18), /**< Transaction begin */
  CONFIG_INSTR_MEM_SYNC_TEND        = (1ULL << 19), /**< Transaction end */
  CONFIG_INSTR_MEM_SYNC_TABORT      = (1ULL << 20), /**< Transaction abort */
} config_instr_mem_sync_t;

/**
* @brief Control flow instruction capabilities - Part 1
* These are not type-specific
*/
typedef enum config_instr_control_flow_1 {
  /* Basic control flow */
  CONFIG_INSTR_CF_BRANCH           = (1ULL << 0),  /**< Branch/jump */
  CONFIG_INSTR_CF_BRANCH_COND      = (1ULL << 1),  /**< Conditional branch */
  CONFIG_INSTR_CF_CALL             = (1ULL << 2),  /**< Call subroutine */
  CONFIG_INSTR_CF_RET              = (1ULL << 3),  /**< Return from subroutine */
  CONFIG_INSTR_CF_JUMP_REG         = (1ULL << 4),  /**< Jump to register */
  CONFIG_INSTR_CF_BRANCH_INDIRECT  = (1ULL << 5),  /**< Indirect branch */
  CONFIG_INSTR_CF_BRANCH_PREDICT   = (1ULL << 6),  /**< Branch prediction hints */
  
  /* Advanced control flow */
  CONFIG_INSTR_CF_LOOP             = (1ULL << 7),  /**< Loop instructions */
  CONFIG_INSTR_CF_CONDITIONAL_MOVE = (1ULL << 8),  /**< Conditional move */
  CONFIG_INSTR_CF_CONDITIONAL_SET  = (1ULL << 9),  /**< Conditional set */
  CONFIG_INSTR_CF_CONDITIONAL_SEL  = (1ULL << 10), /**< Conditional select */
  CONFIG_INSTR_CF_ZEROING_REG      = (1ULL << 11), /**< Zero-idiom recognition */
  CONFIG_INSTR_CF_BRANCH_REG       = (1ULL << 12), /**< Branch to register */
  CONFIG_INSTR_CF_JUMP_TABLE       = (1ULL << 13), /**< Jump table support */
  CONFIG_INSTR_CF_COMPUTED_GOTO    = (1ULL << 14), /**< Computed goto */
  CONFIG_INSTR_CF_PRED_EXECUTION   = (1ULL << 15), /**< Predicated execution */
  CONFIG_INSTR_CF_GUARD            = (1ULL << 16), /**< Guarded execution */
  
  /* Exception handling */
  CONFIG_INSTR_CF_TRAP             = (1ULL << 17), /**< Trap instruction */
  CONFIG_INSTR_CF_SYSCALL          = (1ULL << 18), /**< System call */
  CONFIG_INSTR_CF_BREAK            = (1ULL << 19), /**< Breakpoint */
  CONFIG_INSTR_CF_EXCEPTION        = (1ULL << 20), /**< Exception generation */
  
  /* Thread synchronization */
  CONFIG_INSTR_CF_FUTEX            = (1ULL << 21), /**< Fast user-space mutex */
  CONFIG_INSTR_CF_MONITOR          = (1ULL << 22), /**< Monitor memory */
  CONFIG_INSTR_CF_MWAIT            = (1ULL << 23), /**< Wait for monitor event */
  CONFIG_INSTR_CF_YIELD            = (1ULL << 24), /**< Yield to other thread */
  CONFIG_INSTR_CF_PAUSE            = (1ULL << 25), /**< Spin-wait hint */
  CONFIG_INSTR_CF_WFE              = (1ULL << 26), /**< Wait for event */
  CONFIG_INSTR_CF_SEV              = (1ULL << 27), /**< Send event */
  CONFIG_INSTR_CF_WFI              = (1ULL << 28), /**< Wait for interrupt */
  
  /* Advanced branching */
  CONFIG_INSTR_CF_BRANCH_LIKELY    = (1ULL << 29), /**< Branch likely hint */
  CONFIG_INSTR_CF_BRANCH_FAR       = (1ULL << 30), /**< Far branch */
  CONFIG_INSTR_CF_BRANCH_LINK      = (1ULL << 31), /**< Branch and link */
  CONFIG_INSTR_CF_BRANCH_COUNT     = (1ULL << 32), /**< Branch and decrement */
  CONFIG_INSTR_CF_BRANCH_EXCHANGE  = (1ULL << 33), /**< Branch and exchange execution state */
  
  /* Privilege operations */
  CONFIG_INSTR_CF_PRIV_CHANGE      = (1ULL << 34), /**< Change privilege level */
  CONFIG_INSTR_CF_SYSTEM_REG       = (1ULL << 35), /**< System register access */
  CONFIG_INSTR_CF_HALT             = (1ULL << 36), /**< Halt processor */
  CONFIG_INSTR_CF_SLEEP            = (1ULL << 37), /**< Enter sleep mode */
  CONFIG_INSTR_CF_CACHE_OP         = (1ULL << 38), /**< Cache operation */
  CONFIG_INSTR_CF_TLB_OP           = (1ULL << 39), /**< TLB operation */
  
  /* Execution domain operations */
  CONFIG_INSTR_CF_SMC              = (1ULL << 40), /**< Secure monitor call */
  CONFIG_INSTR_CF_HVC              = (1ULL << 41), /**< Hypervisor call */
  CONFIG_INSTR_CF_VMM              = (1ULL << 42), /**< Virtual machine management */
  
  /* Continuation */
  CONFIG_INSTR_CF_SETJMP           = (1ULL << 43), /**< Save execution context */
  CONFIG_INSTR_CF_LONGJMP          = (1ULL << 44), /**< Restore execution context */
} config_instr_control_flow_1_t;

/**
* @brief Control flow instruction capabilities - Part 2
*/
typedef enum config_instr_control_flow_2 {
  /* Advanced branching features */
  CONFIG_INSTR_CF_LOOP_BUFFER      = (1ULL << 0),  /**< Hardware loop buffer */
  CONFIG_INSTR_CF_BRANCH_FUSION    = (1ULL << 1),  /**< Branch fusion */
  CONFIG_INSTR_CF_MACRO_OP_FUSION  = (1ULL << 2),  /**< Macro-op fusion */
  CONFIG_INSTR_CF_CONST_PROP       = (1ULL << 3),  /**< Constant propagation */
  CONFIG_INSTR_CF_GUARD_CHECK      = (1ULL << 4),  /**< Guard checking */
  
  /* Error handling */
  CONFIG_INSTR_CF_SOFT_ERROR       = (1ULL << 5),  /**< Soft error detection */
  CONFIG_INSTR_CF_ERROR_RECOVERY   = (1ULL << 6),  /**< Error recovery instructions */
  CONFIG_INSTR_CF_FAULT_COLLECT    = (1ULL << 7),  /**< Fault collection */
  
  /* Transactional memory */
  CONFIG_INSTR_CF_HTM_BEGIN        = (1ULL << 8),  /**< Hardware transaction begin */
  CONFIG_INSTR_CF_HTM_END          = (1ULL << 9),  /**< Hardware transaction end */
  CONFIG_INSTR_CF_HTM_ABORT        = (1ULL << 10), /**< Hardware transaction abort */
  CONFIG_INSTR_CF_HTM_TEST         = (1ULL << 11), /**< Hardware transaction test */
  
  /* Virtualization support */
  CONFIG_INSTR_CF_VMEXIT           = (1ULL << 12), /**< VM exit handling */
  CONFIG_INSTR_CF_VMENTER          = (1ULL << 13), /**< VM entry */
  CONFIG_INSTR_CF_VMCALL           = (1ULL << 14), /**< VM call */
  
  /* Future expansion - reserved */
} config_instr_control_flow_2_t;

/**
* @brief Security and cryptographic features - Part 1
*/
typedef enum config_security_features_1 {
  /* Hardware security features */
  CONFIG_SEC_FEAT_ASLR             = (1ULL << 0),  /**< Address space layout randomization */
  CONFIG_SEC_FEAT_DEP              = (1ULL << 1),  /**< Data execution prevention */
  CONFIG_SEC_FEAT_SMEP             = (1ULL << 2),  /**< Supervisor mode execution prevention */
  CONFIG_SEC_FEAT_SMAP             = (1ULL << 3),  /**< Supervisor mode access prevention */
  CONFIG_SEC_FEAT_POINTER_AUTH     = (1ULL << 4),  /**< Pointer authentication */
  CONFIG_SEC_FEAT_BTI              = (1ULL << 5),  /**< Branch target identification */
  CONFIG_SEC_FEAT_SHADOW_STACK     = (1ULL << 6),  /**< Shadow stack */
  
  /* Side-channel mitigations */
  CONFIG_SEC_FEAT_SPECTRE_V1       = (1ULL << 7),  /**< Spectre v1 mitigation */
  CONFIG_SEC_FEAT_SPECTRE_V2       = (1ULL << 8),  /**< Spectre v2 mitigation */
  CONFIG_SEC_FEAT_MELTDOWN         = (1ULL << 9),  /**< Meltdown mitigation */
  CONFIG_SEC_FEAT_MDS              = (1ULL << 10), /**< Microarchitectural data sampling mitigation */
  CONFIG_SEC_FEAT_L1TF             = (1ULL << 11), /**< L1 terminal fault mitigation */
  CONFIG_SEC_FEAT_CONSTANT_TIME    = (1ULL << 12), /**< Constant-time primitives */
  
  /* Secure execution */
  CONFIG_SEC_FEAT_SGX              = (1ULL << 13), /**< Software guard extensions */
  CONFIG_SEC_FEAT_SEV              = (1ULL << 14), /**< Secure encrypted virtualization */
  CONFIG_SEC_FEAT_TDX              = (1ULL << 15), /**< Trust domain extensions */
  CONFIG_SEC_FEAT_TME              = (1ULL << 16), /**< Total memory encryption */
  CONFIG_SEC_FEAT_TZ               = (1ULL << 17), /**< TrustZone */
  CONFIG_SEC_FEAT_PMP              = (1ULL << 18), /**< Physical memory protection */
  
  /* Key management */
  CONFIG_SEC_FEAT_KEY_STORAGE      = (1ULL << 19), /**< Secure key storage */
  CONFIG_SEC_FEAT_KEY_GENERATION   = (1ULL << 20), /**< Secure key generation */
  CONFIG_SEC_FEAT_KEY_WRAPPING     = (1ULL << 21), /**< Key wrapping support */
  
  /* Memory protection */
  CONFIG_SEC_FEAT_MEM_TAGGING      = (1ULL << 22), /**< Memory tagging */
  CONFIG_SEC_FEAT_MEM_ENCRYPTION   = (1ULL << 23), /**< Memory encryption */
  CONFIG_SEC_FEAT_ROWHAMMER_PROT   = (1ULL << 24), /**< Rowhammer protection */
  
  /* Hardware cryptographic acceleration features */
  CONFIG_SEC_FEAT_CRYPTO_ENGINE    = (1ULL << 25), /**< Dedicated crypto engine */
  CONFIG_SEC_FEAT_TRNG             = (1ULL << 26), /**< True random number generator */
  CONFIG_SEC_FEAT_KDF_ACCEL        = (1ULL << 27), /**< Key derivation function acceleration */
  CONFIG_SEC_FEAT_PKE_ACCEL        = (1ULL << 28), /**< Public key encryption acceleration */
  
  /* Advanced security features */
  CONFIG_SEC_FEAT_CFI              = (1ULL << 29), /**< Control flow integrity */
  CONFIG_SEC_FEAT_CET              = (1ULL << 30), /**< Control-flow enforcement technology */
  CONFIG_SEC_FEAT_IBT              = (1ULL << 31), /**< Indirect branch tracking */
  CONFIG_SEC_FEAT_SHSTK            = (1ULL << 32), /**< Shadow stack */
  CONFIG_SEC_FEAT_TSX_CTRL         = (1ULL << 33), /**< TSX transaction control */
  CONFIG_SEC_FEAT_USER_MEM_MARK    = (1ULL << 34), /**< User memory marking */
  CONFIG_SEC_FEAT_MTE              = (1ULL << 35), /**< Memory tagging extension */
  CONFIG_SEC_FEAT_BTF              = (1ULL << 36), /**< Branch target filtering */
  CONFIG_SEC_FEAT_PACGA            = (1ULL << 37), /**< Pointer authentication code for generic authentication */
} config_security_features_1_t;

/**
* @brief Security and cryptographic features - Part 2
*/
typedef enum config_security_features_2 {
  /* Additional mitigations */
  CONFIG_SEC_FEAT_IBPB             = (1ULL << 0),  /**< Indirect branch predictor barrier */
  CONFIG_SEC_FEAT_IBRS             = (1ULL << 1),  /**< Indirect branch restricted speculation */
  CONFIG_SEC_FEAT_STIBP            = (1ULL << 2),  /**< Single thread indirect branch predictors */
  CONFIG_SEC_FEAT_SSBD             = (1ULL << 3),  /**< Speculative store bypass disable */
  
  /* Platform security */
  CONFIG_SEC_FEAT_SECURE_BOOT      = (1ULL << 4),  /**< Secure boot */
  CONFIG_SEC_FEAT_TPM              = (1ULL << 5),  /**< Trusted platform module */
  CONFIG_SEC_FEAT_HSM              = (1ULL << 6),  /**< Hardware security module */
  CONFIG_SEC_FEAT_SECURE_ENCLAVE   = (1ULL << 7),  /**< Secure enclave */
  
  /* Multi-tenancy features */
  CONFIG_SEC_FEAT_VM_ISOLATION     = (1ULL << 8),  /**< VM isolation */
  CONFIG_SEC_FEAT_VIRT_SECURE_MEM  = (1ULL << 9),  /**< Virtualized secure memory */
  CONFIG_SEC_FEAT_NESTED_VIRT_SEC  = (1ULL << 10), /**< Nested virtualization security */
  
  /* Future expansion - reserved */
} config_security_features_2_t;

/**
* @brief Cache descriptor with performance metrics
*/
typedef struct config_cpu_cache {
  uint32_t size;                      /**< Cache size in bytes */
  uint32_t line_size;                 /**< Cache line size in bytes */
  uint32_t associativity;             /**< Cache associativity */
  bool shared;                        /**< Whether cache is shared between cores */
  config_cache_replacement_policy_t replacement_policy; /**< Replacement policy */
  config_cache_write_policy_t write_policy;            /**< Write policy */
  uint32_t latency;                   /**< Access latency in cycles */
  uint32_t bandwidth;                 /**< Bandwidth in bytes per cycle */
  bool inclusive;                     /**< Whether cache is inclusive */
  bool exclusive;                     /**< Whether cache is exclusive */
  uint32_t num_banks;                 /**< Number of cache banks */
  uint32_t num_ports;                 /**< Number of ports */
  uint32_t mshr_entries;              /**< Miss status holding register entries */
} config_cpu_cache_t;

/**
* @brief CPU power state (C-state or P-state)
*/
typedef struct config_cpu_power_state {
  uint32_t id;                        /**< State identifier */
  uint32_t latency;                   /**< Transition latency in cycles */
  uint32_t power;                     /**< Power consumption in milliwatts */
  uint32_t exit_latency;              /**< Exit latency in cycles */
  char name[32];                      /**< State name */
} config_cpu_power_state_t;

/**
* @brief Pipeline stage description
*/
typedef struct config_cpu_pipeline_stage {
  char name[32];                      /**< Stage name */
  uint32_t latency;                   /**< Stage latency in cycles */
  uint32_t throughput;                /**< Stage throughput (instructions per cycle) */
  bool stalls;                        /**< Whether stage can stall */
  bool bypassed;                      /**< Whether stage can be bypassed */
} config_cpu_pipeline_stage_t;

/**
* @brief Memory system capabilities
*/
typedef enum config_mem_system {
  CONFIG_MEM_COHERENT          = (1ULL << 0),  /**< Cache coherence */
  CONFIG_MEM_INCLUSIVE         = (1ULL << 1),  /**< Inclusive caches */
  CONFIG_MEM_EXCLUSIVE         = (1ULL << 2),  /**< Exclusive caches */
  CONFIG_MEM_WRITE_COMBINING   = (1ULL << 3),  /**< Write combining */
  CONFIG_MEM_WRITE_BUFFERING   = (1ULL << 4),  /**< Write buffering */
  CONFIG_MEM_READ_PREFETCH     = (1ULL << 5),  /**< Read prefetching */
  CONFIG_MEM_SPECULATIVE_READ  = (1ULL << 6),  /**< Speculative reads */
  CONFIG_MEM_NON_TEMPORAL      = (1ULL << 7),  /**< Non-temporal access */
  CONFIG_MEM_UNCACHEABLE       = (1ULL << 8),  /**< Uncacheable memory regions */
  CONFIG_MEM_WRITE_PROTECT     = (1ULL << 9),  /**< Write protection */
  CONFIG_MEM_ECC               = (1ULL << 10), /**< Error correcting code */
  CONFIG_MEM_PARITY            = (1ULL << 11), /**< Parity checking */
  CONFIG_MEM_SCRATCHPAD        = (1ULL << 12), /**< Scratchpad memory */
  CONFIG_MEM_WATCHPOINTS       = (1ULL << 13), /**< Memory watchpoints */
  CONFIG_MEM_TAGGED            = (1ULL << 14), /**< Tagged memory */
  CONFIG_MEM_PERSISTENT        = (1ULL << 15), /**< Persistent memory support */
  CONFIG_MEM_NVMEM             = (1ULL << 16), /**< Non-volatile memory support */
  CONFIG_MEM_WEAK_ORDERING     = (1ULL << 17), /**< Weak ordering memory model */
  CONFIG_MEM_STRONG_ORDERING   = (1ULL << 18), /**< Strong ordering memory model */
  CONFIG_MEM_NONSPEC_LOAD      = (1ULL << 19), /**< Non-speculative loads */
} config_mem_system_t;

/**
* @brief Virtual memory configuration
*/
typedef struct config_cpu_virtual_memory {
  bool enabled;                       /**< Whether virtual memory is enabled */
  uint32_t address_bits;              /**< Virtual address bits */
  uint32_t physical_address_bits;     /**< Physical address bits */
  uint32_t page_table_levels;         /**< Levels in the page table */
  uint32_t *page_sizes;               /**< Supported page sizes in bytes */
  uint32_t page_size_count;           /**< Number of supported page sizes */
  bool tagged_memory;                 /**< Tagged memory support */
  uint32_t tag_bits;                  /**< Bits for memory tagging */
  uint32_t tlb_entries;               /**< Total TLB entries */
  uint32_t itlb_entries;              /**< Instruction TLB entries */
  uint32_t dtlb_entries;              /**< Data TLB entries */
  bool shared_tlb;                    /**< Shared TLB */
  uint32_t tlb_levels;                /**< TLB hierarchy levels */
  uint32_t tlb_associativity;         /**< TLB associativity */
  uint32_t asid_bits;                 /**< Address space ID bits */
  bool asid_supported;                /**< Address space ID support */
  uint32_t max_asids;                 /**< Maximum number of ASIDs */
} config_cpu_virtual_memory_t;

/**
* @brief Security features
*/
typedef enum config_security {
  CONFIG_SEC_NX_BIT            = (1ULL << 0),  /**< Non-executable memory */
  CONFIG_SEC_SMEP              = (1ULL << 1),  /**< Supervisor mode execution prevention */
  CONFIG_SEC_SMAP              = (1ULL << 2),  /**< Supervisor mode access prevention */
  CONFIG_SEC_PMP               = (1ULL << 3),  /**< Physical memory protection */
  CONFIG_SEC_MPU               = (1ULL << 4),  /**< Memory protection unit */
  CONFIG_SEC_ASLR              = (1ULL << 5),  /**< Address space layout randomization */
  CONFIG_SEC_KASLR             = (1ULL << 6),  /**< Kernel ASLR */
  CONFIG_SEC_POINTER_AUTH      = (1ULL << 7),  /**< Pointer authentication */
  CONFIG_SEC_BTI               = (1ULL << 8),  /**< Branch target identification */
  CONFIG_SEC_CFI               = (1ULL << 9),  /**< Control flow integrity */
  CONFIG_SEC_SHADOW_STACK      = (1ULL << 10), /**< Shadow stack */
  CONFIG_SEC_SECURE_BOOT       = (1ULL << 11), /**< Secure boot */
  CONFIG_SEC_SGX               = (1ULL << 12), /**< Software guard extensions */
  CONFIG_SEC_SEV               = (1ULL << 13), /**< Secure encrypted virtualization */
  CONFIG_SEC_TME               = (1ULL << 14), /**< Total memory encryption */
  CONFIG_SEC_TZ                = (1ULL << 15), /**< TrustZone */
  CONFIG_SEC_MTE               = (1ULL << 16), /**< Memory tagging extension */
  CONFIG_SEC_PAC               = (1ULL << 17), /**< Pointer authentication codes */
  CONFIG_SEC_CET               = (1ULL << 18), /**< Control-flow enforcement technology */
} config_security_t;

/**
* @brief Virtualization features
*/
typedef enum config_virtualization {
  CONFIG_VIRT_HW_ASSIST        = (1ULL << 0),  /**< Hardware virtualization support */
  CONFIG_VIRT_NESTED           = (1ULL << 1),  /**< Nested virtualization */
  CONFIG_VIRT_EPT              = (1ULL << 2),  /**< Extended page tables */
  CONFIG_VIRT_NPT              = (1ULL << 3),  /**< Nested page tables */
  CONFIG_VIRT_SMMU             = (1ULL << 4),  /**< System MMU */
  CONFIG_VIRT_IOMMU            = (1ULL << 5),  /**< I/O MMU */
  CONFIG_VIRT_VIRTUAL_APIC     = (1ULL << 6),  /**< Virtual APIC */
  CONFIG_VIRT_VIRTUAL_TIMER    = (1ULL << 7),  /**< Virtual timer */
  CONFIG_VIRT_SECURE_VIRT      = (1ULL << 8),  /**< Secure virtualization */
  CONFIG_VIRT_MVICS            = (1ULL << 9),  /**< Multi-core virtualization */
  CONFIG_VIRT_VSID             = (1ULL << 10), /**< Virtual segment ID */
  CONFIG_VIRT_TLB_FLUSH_VIRT   = (1ULL << 11), /**< Virtual TLB flush operations */
  CONFIG_VIRT_GUEST_DEBUG      = (1ULL << 12), /**< Guest debugging */
  CONFIG_VIRT_MEMORY_ENCRYPT   = (1ULL << 13), /**< Memory encryption for VMs */
  CONFIG_VIRT_VIRT_EXCEPT      = (1ULL << 14), /**< Virtualized exceptions */
} config_virtualization_t;

/**
* @brief Debug features
*/
typedef enum config_debug {
  CONFIG_DBG_HARDWARE_BREAK    = (1ULL << 0),  /**< Hardware breakpoints */
  CONFIG_DBG_HARDWARE_WATCH    = (1ULL << 1),  /**< Hardware watchpoints */
  CONFIG_DBG_SINGLE_STEP       = (1ULL << 2),  /**< Single-step execution */
  CONFIG_DBG_BRANCH_TRACE      = (1ULL << 3),  /**< Branch tracing */
  CONFIG_DBG_EXECUTION_TRACE   = (1ULL << 4),  /**< Execution tracing */
  CONFIG_DBG_PERF_MONITOR      = (1ULL << 5),  /**< Performance monitoring */
  CONFIG_DBG_JTAG              = (1ULL << 6),  /**< JTAG debug interface */
  CONFIG_DBG_SWD               = (1ULL << 7),  /**< Serial wire debug */
  CONFIG_DBG_ETM               = (1ULL << 8),  /**< Embedded trace macrocell */
  CONFIG_DBG_ITM               = (1ULL << 9),  /**< Instrumentation trace macrocell */
  CONFIG_DBG_PTM               = (1ULL << 10), /**< Program trace macrocell */
  CONFIG_DBG_LBR               = (1ULL << 11), /**< Last branch record */
  CONFIG_DBG_PMU               = (1ULL << 12), /**< Performance monitoring unit */
  CONFIG_DBG_OCD               = (1ULL << 13), /**< On-chip debug */
  CONFIG_DBG_CORESIGHT         = (1ULL << 14), /**< CoreSight debug */
  CONFIG_DBG_MEMORY_PROFILE    = (1ULL << 15), /**< Memory profiling */
  CONFIG_DBG_CACHE_PROFILE     = (1ULL << 16), /**< Cache profiling */
} config_debug_t;

/**
* @brief Hardware type descriptor for integer types
*/
typedef struct config_hw_int_type {
  uint32_t bitsize;                   /**< Size in bits */
  uint64_t capabilities_1;            /**< Integer instruction capabilities (part 1) bitmap */
  uint64_t capabilities_2;            /**< Integer instruction capabilities (part 2) bitmap */
  config_cpu_reg_type_t reg_type;     /**< Register type used */
  uint32_t alignment;                 /**< Natural alignment in bytes, 0 for no alignment requirement */
  bool has_carry_flag;                /**< Whether operations set carry flag */
  bool has_overflow_flag;             /**< Whether operations set overflow flag */
  
  /* Performance data */
  uint16_t latency;                   /**< Typical instruction latency in cycles */
  uint16_t throughput;                /**< Typical throughput (instructions per cycle) */
  uint16_t min_latency;               /**< Minimum instruction latency */
  uint16_t max_latency;               /**< Maximum instruction latency */
} config_hw_int_type_t;

/**
* @brief Hardware type descriptor for floating-point types
*/
typedef struct config_hw_fp_type {
  uint32_t bitsize;                   /**< Size in bits */
  uint64_t capabilities_1;            /**< Floating-point instruction capabilities (part 1) bitmap */
  uint64_t capabilities_2;            /**< Floating-point instruction capabilities (part 2) bitmap */
  config_cpu_reg_type_t reg_type;     /**< Register type used */
  uint32_t alignment;                 /**< Natural alignment in bytes, 0 for no alignment requirement */
  uint8_t precision;                  /**< Effective bits of precision */
  uint8_t exponent_bits;              /**< Number of exponent bits */
  uint8_t mantissa_bits;              /**< Number of mantissa bits */
  bool denormals_as_zero;             /**< Whether denormals are treated as zero */
  bool flush_to_zero;                 /**< Whether results are flushed to zero */
  bool ieee_compliant;                /**< Whether fully IEEE 754 compliant */
  
  /* Performance data */
  uint16_t add_latency;               /**< Addition latency in cycles */
  uint16_t mul_latency;               /**< Multiplication latency in cycles */
  uint16_t div_latency;               /**< Division latency in cycles */
  uint16_t sqrt_latency;              /**< Square root latency in cycles */
  uint16_t fma_latency;               /**< Fused multiply-add latency in cycles */
} config_hw_fp_type_t;

/**
* @brief Hardware type descriptor for vector types
*/
typedef struct config_hw_vec_type {
  uint32_t bitsize;                   /**< Size in bits */
  uint64_t capabilities_1;            /**< Vector instruction capabilities (part 1) bitmap */
  uint64_t capabilities_2;            /**< Vector instruction capabilities (part 2) bitmap */
  config_cpu_reg_type_t reg_type;     /**< Register type used */
  uint32_t min_elements;              /**< Minimum vector length (elements) */
  uint32_t max_elements;              /**< Maximum vector length (elements) */
  bool scalable;                      /**< Whether vector length is scalable */
  uint32_t alignment;                 /**< Natural alignment in bytes, 0 for no alignment requirement */
  uint16_t supported_element_sizes;   /**< Bitfield of supported element sizes */
  bool predicated_execution;          /**< Whether predicated execution is supported */
  bool masked_operations;             /**< Whether masked operations are supported */
  
  /* Performance data */
  uint16_t load_latency;              /**< Vector load latency in cycles */
  uint16_t store_latency;             /**< Vector store latency in cycles */
  uint16_t op_latency;                /**< Vector operation latency in cycles */
  uint16_t lane_throughput;           /**< Operations per cycle per lane */
} config_hw_vec_type_t;

/**
* @brief Execution resources
*/
typedef struct config_cpu_execution {
  uint32_t int_units;                 /**< Integer execution units */
  uint32_t fp_units;                  /**< Floating point execution units */
  uint32_t vector_units;              /**< Vector execution units */
  uint32_t load_units;                /**< Load units */
  uint32_t store_units;               /**< Store units */
  uint32_t branch_units;              /**< Branch units */
  uint32_t crypto_units;              /**< Cryptography units */
  
  uint32_t dispatch_width;            /**< Instructions per cycle dispatch */
  uint32_t issue_width;               /**< Instructions per cycle issue */
  uint32_t retire_width;              /**< Instructions per cycle retirement */
  uint32_t fetch_width;               /**< Instructions per cycle fetch */
  uint32_t decode_width;              /**< Instructions per cycle decode */
  
  uint32_t reservation_stations;      /**< Number of reservation stations */
  uint32_t reorder_buffer_size;       /**< Size of the reorder buffer */
  uint32_t load_queue_size;           /**< Size of the load queue */
  uint32_t store_queue_size;          /**< Size of the store queue */
  
  bool smt;                           /**< Simultaneous multithreading */
  uint8_t smt_threads;                /**< Number of SMT threads per core */
  bool out_of_order;                  /**< Out-of-order execution */
  bool speculative;                   /**< Speculative execution */
  bool superscalar;                   /**< Superscalar execution */
  bool register_renaming;             /**< Register renaming */
  bool gshare_predictor;              /**< Global share branch predictor */
  bool tournament_predictor;          /**< Tournament branch predictor */
  uint32_t branch_pred_entries;       /**< Branch predictor entries */
  bool branch_target_buffer;          /**< Branch target buffer */
  uint32_t btb_entries;               /**< Branch target buffer entries */
  bool return_stack_buffer;           /**< Return stack buffer */
  uint32_t rsb_entries;               /**< Return stack buffer entries */
  
  /* Pipeline details */
  uint32_t pipeline_stages;           /**< Number of pipeline stages */
  config_cpu_pipeline_stage_t *pipeline; /**< Pipeline stage descriptions */
} config_cpu_execution_t;

/**
* @brief Register file configuration
*/
typedef struct config_cpu_registers {
  uint32_t gpr_count;                 /**< General purpose registers */
  uint32_t gpr_width;                 /**< General purpose register width in bits */
  uint32_t fpr_count;                 /**< Floating point registers */
  uint32_t fpr_width;                 /**< Floating point register width in bits */
  uint32_t vector_count;              /**< Vector registers */
  uint32_t vector_width;              /**< Vector register width in bits */
  uint32_t pred_count;                /**< Predicate registers */
  uint32_t pred_width;                /**< Predicate register width in bits */
  uint32_t flag_count;                /**< Flag registers */
  uint32_t flag_width;                /**< Flag register width in bits */
  uint32_t special_count;             /**< Special-purpose registers */
  uint32_t special_width;             /**< Special register width in bits */
  uint32_t rename_count;              /**< Physical register count (for renaming) */
  uint32_t arch_reg_count;            /**< Architectural register count */
} config_cpu_registers_t;

/**
* @brief ABI calling convention descriptor
*/
typedef struct config_cpu_abi {
  char name[64];                      /**< Name of the calling convention */
  
  uint32_t stack_alignment;           /**< Stack alignment in bytes */
  config_cpu_reg_t *param_int_regs;   /**< Integer registers for parameters */
  uint32_t param_int_reg_count;       /**< Number of integer parameter registers */
  config_cpu_reg_t *param_fp_regs;    /**< FP registers for parameters */
  uint32_t param_fp_reg_count;        /**< Number of FP parameter registers */
  config_cpu_reg_t *param_vec_regs;   /**< Vector registers for parameters */
  uint32_t param_vec_reg_count;       /**< Number of vector parameter registers */
  
  bool stack_growth_down;             /**< Stack grows down if true */
  uint32_t red_zone;                  /**< Red zone size in bytes */
  
  config_cpu_reg_t *return_int_regs;  /**< Integer registers for return values */
  uint32_t return_int_reg_count;      /**< Number of integer return registers */
  config_cpu_reg_t *return_fp_regs;   /**< FP registers for return values */
  uint32_t return_fp_reg_count;       /**< Number of FP return registers */
  config_cpu_reg_t *return_vec_regs;  /**< Vector registers for return values */
  uint32_t return_vec_reg_count;      /**< Number of vector return registers */
  
  bool stack_return;                  /**< Return large values on stack */
  
  config_cpu_reg_t *caller_saved;     /**< Caller-saved registers */
  uint32_t caller_saved_count;        /**< Number of caller-saved registers */
  config_cpu_reg_t *callee_saved;     /**< Callee-saved registers */
  uint32_t callee_saved_count;        /**< Number of callee-saved registers */
  
  bool shadow_space;                  /**< Shadow space on stack */
  uint32_t shadow_space_size;         /**< Size of shadow space in bytes */
  
  uint32_t struct_return_size_threshold; /**< Threshold size for struct returns in bytes */
  uint32_t var_args_reg_save_area;    /**< Size of register save area for varargs */
} config_cpu_abi_t;

/**
* @brief Core cluster configuration
*/
typedef struct config_cpu_cluster {
  uint32_t core_count;                /**< Number of cores in this cluster */
  bool shared_l2;                     /**< Whether L2 cache is shared in cluster */
  bool shared_l3;                     /**< Whether L3 cache is shared in cluster */
  bool heterogeneous;                 /**< Whether cores are heterogeneous */
  char type[32];                      /**< Type of cores (e.g., "big", "little") */
  uint32_t max_freq;                  /**< Maximum frequency in MHz */
  uint32_t tdp;                       /**< Thermal design power in milliwatts */
} config_cpu_cluster_t;

/**
* @brief Power management features
*/
typedef struct config_cpu_power_mgmt {
  uint64_t features;                  /**< Power management features bitfield */
  
  uint32_t tdp;                       /**< Thermal design power in milliwatts */
  uint32_t base_freq;                 /**< Base frequency in MHz */
  uint32_t max_freq;                  /**< Maximum frequency in MHz */
  uint32_t min_freq;                  /**< Minimum frequency in MHz */
  
  config_cpu_power_state_t *c_states; /**< Supported C-states */
  uint32_t c_state_count;             /**< Number of supported C-states */
  config_cpu_power_state_t *p_states; /**< Supported P-states */
  uint32_t p_state_count;             /**< Number of supported P-states */
  
  bool dvfs;                          /**< Dynamic voltage and frequency scaling */
  bool avs;                           /**< Adaptive voltage scaling */
  bool turbo;                         /**< Turbo/boost support */
  bool power_gating;                  /**< Power gating support */
  bool clock_gating;                  /**< Clock gating support */
  bool hibernate;                     /**< Hibernate support */
  bool deep_sleep;                    /**< Deep sleep support */
  
  uint32_t power_domains;             /**< Number of power domains */
  uint32_t clock_domains;             /**< Number of clock domains */
} config_cpu_power_mgmt_t;

/**
* @brief Main CPU configuration structure with versioning and extended capabilities
*/
typedef struct config_cpu {
  /* Version information */
  config_version_t version;           /**< Structure version information */
  
  /* Basic identification */
  config_cpu_arch_t arch;             /**< CPU architecture */
  char ident[64];                     /**< CPU identifier */
  char product[64];                   /**< Product name */
  char vendor[64];                    /**< Vendor name */
  uint32_t generation;                /**< Generation or family */
  uint32_t year;                      /**< Release year */
  char microarch[64];                 /**< Microarchitecture name */
  uint32_t arch_version;              /**< Architecture version */
  uint32_t impl_version;              /**< Implementation version */
  char isa_string[256];               /**< ISA string representation */
  
  /* Hardware type capabilities */
  struct {
    /* Hardware representation of various data types */
    config_hw_int_type_t int8;      /**< 8-bit integer */
    config_hw_int_type_t int16;     /**< 16-bit integer */
    config_hw_int_type_t int32;     /**< 32-bit integer */
    config_hw_int_type_t int64;     /**< 64-bit integer */
    config_hw_int_type_t int128;    /**< 128-bit integer (if supported) */
    
    config_hw_fp_type_t fp16;       /**< 16-bit FP (IEEE 754 half-precision) */
    config_hw_fp_type_t bfp16;      /**< 16-bit brain floating point */
    config_hw_fp_type_t fp32;       /**< 32-bit FP (IEEE 754 single-precision) */
    config_hw_fp_type_t fp64;       /**< 64-bit FP (IEEE 754 double-precision) */
    config_hw_fp_type_t fp80;       /**< 80-bit extended precision */
    config_hw_fp_type_t fp128;      /**< 128-bit FP (IEEE 754 quadruple-precision) */
    
    config_hw_vec_type_t vec128;    /**< 128-bit vector */
    config_hw_vec_type_t vec256;    /**< 256-bit vector */
    config_hw_vec_type_t vec512;    /**< 512-bit vector */
    config_hw_vec_type_t vec_scalable; /**< Scalable vector */
    
    /* Type aliases - pointers to the hardware-native types */
    config_hw_int_type_t *defint;   /**< Default integer type */
    config_hw_fp_type_t *deffloat;  /**< Default floating point type */
    config_hw_vec_type_t *defvec;   /**< Default vector type */
  } hw_types;
  
  /* Architecture-wide capabilities (not tied to specific data types) */
  struct {
    uint64_t control_flow_1;        /**< Control flow capabilities bitmap (part 1) */
    uint64_t control_flow_2;        /**< Control flow capabilities bitmap (part 2) */
    uint64_t mem_sync;              /**< Memory synchronization capabilities bitmap */
    uint64_t security_1;            /**< Security capabilities bitmap (part 1) */
    uint64_t security_2;            /**< Security capabilities bitmap (part 2) */
    uint64_t debug;                 /**< Debug capabilities bitmap */
  } arch_caps;
  
  /* Memory subsystem */
  struct {
    /* Cache hierarchy */
    config_cpu_cache_t l1d;         /**< L1 data cache */
    config_cpu_cache_t l1i;         /**< L1 instruction cache */
    config_cpu_cache_t l2;          /**< L2 cache */
    config_cpu_cache_t l3;          /**< L3 cache */
    config_cpu_cache_t l4;          /**< L4 cache (if applicable) */
    
    /* Memory features */
    uint64_t features;              /**< Memory system feature bitfield */
    
    /* Memory organization */
    uint32_t page_size;             /**< Default page size in bytes */
    uint32_t cacheline_size;        /**< Cache line size in bytes */
    bool strict_align;              /**< Strict alignment required */
    uint32_t min_align;             /**< Minimum alignment in bytes */
    config_endianness_t endianness; /**< Byte order */
    config_memory_model_t memory_model; /**< Memory consistency model */
    
    /* Virtual memory */
    config_cpu_virtual_memory_t virtual_memory; /**< Virtual memory configuration */
    
    /* Prefetchers */
    bool hw_prefetcher;             /**< Hardware prefetcher available */
    uint32_t prefetch_streams;      /**< Number of prefetch streams */
    uint32_t prefetch_distance;     /**< Prefetch distance */
    bool stride_prefetcher;         /**< Stride prefetcher */
    bool adjacent_prefetcher;       /**< Adjacent line prefetcher */
    bool spatial_prefetcher;        /**< Spatial prefetcher */
    bool temporal_prefetcher;       /**< Temporal prefetcher */
  } memory;
  
  /* Execution resources */
  struct {
    /* Thread resources */
    uint32_t hw_threads;            /**< Hardware threads per core */
    uint32_t cores;                 /**< Number of cores */
    uint32_t min_cores;             /**< Minimum cores (for configurable CPUs) */
    uint32_t max_cores;             /**< Maximum cores (for configurable CPUs) */
    uint32_t clusters;              /**< Number of core clusters */
    config_cpu_cluster_t *cluster_config; /**< Cluster configuration */
    uint32_t dies;                  /**< Number of dies */
    uint32_t sockets;               /**< Number of sockets */
    
    config_cpu_execution_t execution; /**< Execution units and capabilities */
    config_cpu_registers_t registers; /**< Register file configuration */
  } resources;
  
  /* ABI definitions */
  struct {
    config_cpu_abi_t *abis;         /**< Supported ABIs */
    uint32_t abi_count;             /**< Number of supported ABIs */
    char default_abi[64];           /**< Default ABI name */
  } abi;
  
  /* Virtualization features */
  struct {
      uint64_t virtualization;        /**< Virtualization features bitfield */
  } platform;
  
  /* Power management */
  config_cpu_power_mgmt_t power;
} config_cpu_t;

#endif /* CPU_CONFIG_H */