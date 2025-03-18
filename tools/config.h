/**
* @file config.h
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

#ifndef COIL_CONFIG_H
#define COIL_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

/**
* @brief Version information for the configuration structure
* Used to ensure compatibility between different versions of the library
*/
typedef struct coil_version {
  uint8_t major;                   /**< Major version (incompatible API changes) */
  uint8_t minor;                   /**< Minor version (backwards-compatible functionality) */
  uint8_t patch;                   /**< Patch version (backwards-compatible bug fixes) */
} coil_version_t;

/**
* @brief Current version of the CPU configuration structure
*/
#define COIL_VERSION_MAJOR 1
#define COIL_VERSION_MINOR 1
#define COIL_VERSION_PATCH 0

/**
* @brief Generic Binary Processing Unit Type
* For future extension to support different processing unit types
*/
typedef enum coil_pu_type {
  COIL_PU_CPU,                     /**< Central Processing Unit */
  COIL_PU_GPU,                     /**< Graphics Processing Unit */
  COIL_PU_DSP,                     /**< Digital Signal Processor */
  COIL_PU_NPU,                     /**< Neural Processing Unit */
  COIL_PU_TPU,                     /**< Tensor Processing Unit */
  COIL_PU_FPGA,                    /**< Field-Programmable Gate Array */
  COIL_PU_ASIC,                    /**< Application-Specific Integrated Circuit */
} coil_pu_type_t;

/**
* @brief Supported CPU architectures
*/
typedef enum coil_cpu_arch {
  COIL_CPU_ARCH_X86,            /**< x86 and x86-64 */
  COIL_CPU_ARCH_ARM,            /**< ARM and ARM64 */
  COIL_CPU_ARCH_RISCV,          /**< RISC-V */
  COIL_CPU_ARCH_POWERPC,        /**< PowerPC and POWER */
  COIL_CPU_ARCH_MIPS,           /**< MIPS */
  COIL_CPU_ARCH_SPARC,          /**< SPARC */
  COIL_CPU_ARCH_WASM,           /**< WebAssembly */
  COIL_CPU_ARCH_AVR,            /**< 8-bit AVR microcontroller */
  COIL_CPU_ARCH_MSP430,         /**< 16-bit MSP430 microcontroller */
  COIL_CPU_ARCH_S390X,          /**< IBM System Z (s390x) */
  COIL_CPU_ARCH_XTENSA,         /**< Xtensa (ESP32, etc.) */
  COIL_CPU_ARCH_ALPHA,          /**< DEC Alpha */
  COIL_CPU_ARCH_ITANIUM,        /**< Intel Itanium (IA-64) */
  COIL_CPU_ARCH_PARISC,         /**< HP PA-RISC */
  COIL_CPU_ARCH_HEXAGON         /**< Qualcomm Hexagon */
} coil_cpu_arch_t;

/**
* @brief Register types available in CPUs
*/
typedef enum coil_cpu_reg : uint8_t {
  ISA_CPU_REG_GP = 0x01,      /**< General purpose registers */
  ISA_CPU_REG_FP = 0x02,      /**< Floating point registers */
  ISA_CPU_REG_VEC = 0x03,     /**< Vector registers */
  ISA_CPU_REG_FLAG = 0x04,    /**< Flag registers */
  ISA_CPU_REG_SEG = 0x05,     /**< Segment registers */
  ISA_CPU_REG_PC = 0x06,      /**< Program counter */
  ISA_CPU_REG_SP = 0x07,      /**< Stack pointer */
  ISA_CPU_REG_LR = 0x08,      /**< Link register */
  ISA_CPU_REG_PRED = 0x09,    /**< Predicate registers */
  ISA_CPU_REG_SPECIAL = 0x0A, /**< Special-purpose registers */
  ISA_CPU_REG_SYSTEM = 0x0B,  /**< System/control registers */
  ISA_CPU_REG_DEBUG = 0x0C,   /**< Debug registers */
  ISA_CPU_REG_MASK = 0x0D,    /**< Mask registers (for vector operations) */
  ISA_CPU_REG_ACC = 0x0E      /**< Accumulator registers */
} coil_cpu_reg_t;

/**
* @brief All Types Instructions Capabilities
* Represents fundamental operations applicable to all data types at assembly level
*/
typedef enum coil_type_instr {
  // Basic Arithmetic
  ISA_TYPE_ADD = (1 << 0),        /**< Addition */
  ISA_TYPE_SUB = (1 << 1),        /**< Subtraction */
  ISA_TYPE_MUL = (1 << 2),        /**< Multiplication */
  ISA_TYPE_DIV = (1 << 3),        /**< Division */
  ISA_TYPE_REM = (1 << 4),        /**< Remainder (modulo) */
  ISA_TYPE_NEG = (1 << 5),        /**< Negation */

  // Flag Operations
  ISA_TYPE_CMP  = (1 << 6),       /**< Compare (subtract without change) */
  ISA_TYPE_TEST = (1 << 7),       /**< Test (and without change) */

  // Memory Operations
  ISA_TYPE_MOV = (1 << 8),        /**< Move/copy operations */
  ISA_TYPE_PUSH = (1 << 9),       /**< Push to stack */
  ISA_TYPE_POP = (1 << 10),       /**< Pop from stack */
  ISA_TYPE_LOAD = (1 << 11),      /**< Load from memory */
  ISA_TYPE_STORE = (1 << 12),     /**< Store to memory */
  ISA_TYPE_PREFETCH = (1 << 13),  /**< Prefetch memory */
  ISA_TYPE_EXCHANGE = (1 << 14),  /**< Exchange/swap values */
  
  // Logical Operations
  ISA_TYPE_AND = (1 << 15),       /**< Bitwise AND */
  ISA_TYPE_OR = (1 << 16),        /**< Bitwise OR */
  ISA_TYPE_XOR = (1 << 17),       /**< Bitwise XOR */
  ISA_TYPE_NOT = (1 << 18),       /**< Bitwise NOT */
  
  // Shifts and Rotations
  ISA_TYPE_SHL = (1 << 19),       /**< Shift left logical */
  ISA_TYPE_SHR = (1 << 20),       /**< Shift right logical */
  ISA_TYPE_SAR = (1 << 21),       /**< Shift arithmetic right (sign preserving) */
  ISA_TYPE_ROL = (1 << 22),       /**< Rotate left */
  ISA_TYPE_ROR = (1 << 23),       /**< Rotate right */
  ISA_TYPE_RCL = (1 << 24),       /**< Rotate left through carry */
  ISA_TYPE_RCR = (1 << 25),       /**< Rotate right through carry */
  
  // Atomic Operations
  ISA_TYPE_ATOMIC_ADD = (1 << 26), /**< Atomic addition */
  ISA_TYPE_ATOMIC_SUB = (1 << 27), /**< Atomic subtraction */
  ISA_TYPE_ATOMIC_AND = (1 << 28), /**< Atomic AND */
  ISA_TYPE_ATOMIC_OR = (1 << 29),  /**< Atomic OR */
  ISA_TYPE_ATOMIC_XOR = (1 << 30), /**< Atomic XOR */
  ISA_TYPE_ATOMIC_XCHG = (1ULL << 31), /**< Atomic exchange */
  ISA_TYPE_ATOMIC_CAS = (1ULL << 32), /**< Atomic compare-and-swap */
  
  // Bit Manipulation (applicable to any binary representation)
  ISA_TYPE_BIT_SET = (1ULL << 33),  /**< Set bit */
  ISA_TYPE_BIT_CLR = (1ULL << 34),  /**< Clear bit */
  ISA_TYPE_BIT_TST = (1ULL << 35),  /**< Test bit */
  ISA_TYPE_BIT_TGL = (1ULL << 36),  /**< Toggle bit */
  
  // Extension/Truncation
  ISA_TYPE_SIGN_EXTEND = (1ULL << 37), /**< Sign extension */
  ISA_TYPE_ZERO_EXTEND = (1ULL << 38), /**< Zero extension */
  ISA_TYPE_TRUNCATE = (1ULL << 39),    /**< Truncation */
  
  // Miscellaneous
  ISA_TYPE_ABS = (1ULL << 40),    /**< Absolute value */
  ISA_TYPE_MIN = (1ULL << 41),    /**< Minimum of two values */
  ISA_TYPE_MAX = (1ULL << 42),    /**< Maximum of two values */
  ISA_TYPE_ZERO = (1ULL << 43),   /**< Zero/clear operation */
  
  // Extension operations (register pair operations)
  ISA_TYPE_WIDE_MUL = (1ULL << 44), /**< Wide multiplication (double width result) */
  ISA_TYPE_WIDE_DIV = (1ULL << 45), /**< Wide division (double width dividend) */
} coil_type_instr_t;

/**
* @brief Integer Specific Instructions
* Operations applicable only to integer types at assembly level
*/
typedef enum coil_int_instr {
  // Extended Arithmetic
  ISA_INT_ADDC = (1 << 0),        /**< Add with carry */
  ISA_INT_SUBB = (1 << 1),        /**< Subtract with borrow */
  ISA_INT_IMUL = (1 << 2),        /**< Signed multiplication */
  ISA_INT_IDIV = (1 << 3),        /**< Signed division */
  ISA_INT_IREM = (1 << 4),        /**< Signed remainder */
  
  // Bit Counting
  ISA_INT_CLZ = (1 << 5),         /**< Count leading zeros */
  ISA_INT_CTZ = (1 << 6),         /**< Count trailing zeros */
  ISA_INT_POPCNT = (1 << 7),      /**< Population count (count set bits) */
  ISA_INT_PARITY = (1 << 8),      /**< Parity check */
  
  // Endian Operations
  ISA_INT_BSWAP = (1 << 9),       /**< Byte swap (endian conversion) */
  ISA_INT_BITREV = (1 << 10),     /**< Bit reverse */
  
  // Bit Field Operations
  ISA_INT_BIT_FIELD_EXTRACT = (1 << 11), /**< Extract bit field */
  ISA_INT_BIT_FIELD_INSERT = (1 << 12),  /**< Insert bit field */
  
  // Extended Operations
  ISA_INT_MULH = (1 << 13),       /**< Return high part of multiplication */
  ISA_INT_MULHU = (1 << 14),      /**< Return high part of unsigned multiplication */
  ISA_INT_MULHS = (1 << 15),      /**< Return high part of signed multiplication */
  
  // Saturation Operations
  ISA_INT_ADDS = (1 << 16),       /**< Saturating addition */
  ISA_INT_SUBS = (1 << 17),       /**< Saturating subtraction */
  ISA_INT_MULS = (1 << 18),       /**< Saturating multiplication */
  
  // BCD Operations
  ISA_INT_BCD_ADJUST = (1 << 19), /**< BCD adjustment */
  ISA_INT_DAA = (1 << 20),        /**< Decimal adjust after addition */
  ISA_INT_DAS = (1 << 21),        /**< Decimal adjust after subtraction */
  
  // Fixed-Point Operations
  ISA_INT_FIXED_MUL = (1 << 22),  /**< Fixed-point multiply */
  ISA_INT_FIXED_DIV = (1 << 23),  /**< Fixed-point divide */
} coil_int_instr_t;

/**
* @brief Float Specific Instructions
* Operations applicable only to floating-point types at assembly level
*/
typedef enum coil_fp_instr {
  // Status Flags & Classification
  ISA_FP_STATUS_FLAGS = (1 << 0),  /**< Support for floating-point status flags */
  ISA_FP_CLASSIFY = (1 << 1),      /**< Classify floating point number */
  
  // Rounding Control
  ISA_FP_ROUNDING_CONTROL = (1 << 2), /**< Support for rounding mode control */
  
  // Precision Control
  ISA_FP_PRECISION_CONVERT = (1 << 3), /**< Convert between floating-point precisions */
  ISA_FP_INT_TO_FP = (1 << 4),     /**< Convert integer to floating-point */
  ISA_FP_FP_TO_INT = (1 << 5),     /**< Convert floating-point to integer */
  
  // Basic Operations
  ISA_FP_SQRT = (1 << 6),         /**< Square root */
  ISA_FP_RCP = (1 << 7),          /**< Reciprocal */
  ISA_FP_RSQRT = (1 << 8),        /**< Reciprocal square root */
  
  // Fused Operations
  ISA_FP_FMA = (1 << 9),          /**< Fused multiply-add */
  ISA_FP_FMS = (1 << 10),         /**< Fused multiply-subtract */
  ISA_FP_FNMA = (1 << 11),        /**< Fused negate-multiply-add */
  ISA_FP_FNMS = (1 << 12),        /**< Fused negate-multiply-subtract */
  
  // Exact Rounding Ops
  ISA_FP_ROUND = (1 << 13),       /**< Round to integer */
  ISA_FP_TRUNC = (1 << 14),       /**< Truncate to integer (round toward zero) */
  ISA_FP_FLOOR = (1 << 15),       /**< Floor (round down) */
  ISA_FP_CEIL = (1 << 16),        /**< Ceiling (round up) */
  
  // Extended Ops with Hardware Acceleration
  ISA_FP_SIN = (1 << 17),         /**< Sine */
  ISA_FP_COS = (1 << 18),         /**< Cosine */
  ISA_FP_TAN = (1 << 19),         /**< Tangent */
  ISA_FP_LOG = (1 << 20),         /**< Logarithm */
  ISA_FP_LOG2 = (1 << 21),        /**< Base-2 logarithm */
  ISA_FP_LOG10 = (1 << 22),       /**< Base-10 logarithm */
  ISA_FP_EXP = (1 << 23),         /**< Exponential */
  ISA_FP_EXP2 = (1 << 24),        /**< Base-2 exponential */
  ISA_FP_POW = (1 << 25),         /**< Power function */
  
  // Component Extraction
  ISA_FP_EXTRACT_EXP = (1 << 26), /**< Extract exponent */
  ISA_FP_EXTRACT_MANT = (1 << 27), /**< Extract mantissa */
  ISA_FP_SCALEF = (1 << 28),      /**< Scale by power of 2 */
  
  // Sign Manipulation
  ISA_FP_COPY_SIGN = (1 << 29),   /**< Copy sign from one value to another */
  
  // Exception Handling
  ISA_FP_EXCEPTION_CONTROL = (1 << 30), /**< Control floating-point exceptions */
} coil_fp_instr_t;

/**
* @brief Vector Specific Instructions
* Operations applicable only to vector types at assembly level
*/
typedef enum coil_vec_instr {
  // Data Movement
  ISA_VEC_PERMUTE = (1 << 0),     /**< Vector permute/shuffle elements */
  ISA_VEC_EXTRACT = (1 << 1),     /**< Extract element from vector */
  ISA_VEC_INSERT = (1 << 2),      /**< Insert element into vector */
  ISA_VEC_BROADCAST = (1 << 3),   /**< Broadcast scalar to all elements */
  
  // Element Conversion
  ISA_VEC_PACK = (1 << 4),        /**< Pack elements with saturation */
  ISA_VEC_UNPACK = (1 << 5),      /**< Unpack/expand elements */
  
  // Lane Operations
  ISA_VEC_LANE_CROSSING = (1 << 6), /**< Support for operations across lanes */
  ISA_VEC_BLEND = (1 << 7),       /**< Blend/select between vectors */
  
  // Reduction Operations
  ISA_VEC_HORIZONTAL_ADD = (1 << 8), /**< Horizontal addition */
  ISA_VEC_HORIZONTAL_SUB = (1 << 9), /**< Horizontal subtraction */
  ISA_VEC_HORIZONTAL_MUL = (1 << 10), /**< Horizontal multiplication */
  
  // Dot Products
  ISA_VEC_DOT_PRODUCT = (1 << 11), /**< Dot product */
  
  // Masking
  ISA_VEC_MASK_OPERATIONS = (1 << 12), /**< Masked vector operations */
  ISA_VEC_COMPRESS = (1 << 13),   /**< Compress vector using mask */
  ISA_VEC_EXPAND = (1 << 14),     /**< Expand vector using mask */
  
  // Memory Operations
  ISA_VEC_GATHER = (1 << 15),     /**< Gather (indexed load) */
  ISA_VEC_SCATTER = (1 << 16),    /**< Scatter (indexed store) */
  
  // Math
  ISA_VEC_SQRT = (1 << 17),       /**< Vector square root */
  ISA_VEC_FMA = (1 << 18),        /**< Vector fused multiply-add */
  
  // Comparison & Selection
  ISA_VEC_COMPARE = (1 << 19),    /**< Vector compare to mask */
  ISA_VEC_SELECT = (1 << 20),     /**< Select using mask */
  
  // Cross-Lane Ops
  ISA_VEC_SHIFT_ELEMENTS = (1 << 21), /**< Shift vector elements */
  ISA_VEC_TABLE_LOOKUP = (1 << 22),   /**< Table lookup */
  
  // Advanced Bit Manipulation
  ISA_VEC_BITWISE_SELECT = (1 << 23), /**< Bitwise select */
  
  // Special Types
  ISA_VEC_INT8 = (1 << 24),      /**< 8-bit integer vector element support */
  ISA_VEC_INT16 = (1 << 25),     /**< 16-bit integer vector element support */
  ISA_VEC_INT32 = (1 << 26),     /**< 32-bit integer vector element support */
  ISA_VEC_INT64 = (1 << 27),     /**< 64-bit integer vector element support */
  ISA_VEC_FP16 = (1 << 28),      /**< Half-precision floating-point vector support */
  ISA_VEC_FP32 = (1 << 29),      /**< Single-precision floating-point vector support */
  ISA_VEC_FP64 = (1 << 30),      /**< Double-precision floating-point vector support */
} coil_vec_instr_t;

/**
* @brief Processing Unit Instructions
*
* Core control flow instructions common to most binary processing units
*/
typedef enum coil_instr {
  /* Unconditional Control Flow */
  ISA_CF_BR = (1 << 0),          /**< Direct branch */
  ISA_CF_BR_IND = (1 << 1),      /**< Indirect branch */
  ISA_CF_CALL = (1 << 2),        /**< Function call */
  ISA_CF_RET = (1 << 3),         /**< Return from function */
  ISA_CF_TRAP = (1 << 4),        /**< Trap/system call */
  ISA_CF_INT = (1 << 5),         /**< Software interrupt */
  ISA_CF_IRET = (1 << 6),        /**< Return from interrupt */
  
  /* Conditional Control Flow */
  ISA_CF_BR_EQ = (1 << 7),       /**< Branch if equal */
  ISA_CF_BR_NE = (1 << 8),       /**< Branch if not equal */
  ISA_CF_BR_LT = (1 << 9),       /**< Branch if less than */
  ISA_CF_BR_LE = (1 << 10),      /**< Branch if less than or equal */
  ISA_CF_BR_GT = (1 << 11),      /**< Branch if greater than */
  ISA_CF_BR_GE = (1 << 12),      /**< Branch if greater than or equal */
  ISA_CF_BR_CARRY = (1 << 13),   /**< Branch if carry */
  ISA_CF_BR_OFLOW = (1 << 14),   /**< Branch if overflow */
  ISA_CF_BR_ZERO = (1 << 15),    /**< Branch if zero */
  ISA_CF_BR_NEG = (1 << 16),     /**< Branch if negative */
  ISA_CF_BR_POS = (1 << 17),     /**< Branch if positive */
  
  /* Memory Barrier/Synchronization */
  ISA_CF_FENCE = (1 << 18),      /**< Memory fence */
  ISA_CF_MFENCE = (1 << 19),     /**< Memory fence */
  ISA_CF_LFENCE = (1 << 20),     /**< Load fence */
  ISA_CF_SFENCE = (1 << 21),     /**< Store fence */
  ISA_CF_LOCK = (1 << 22),       /**< Lock prefix for atomic operations */
  
  /* Execution Control */
  ISA_CF_NOP = (1 << 23),        /**< No operation */
  ISA_CF_HALT = (1 << 24),       /**< Halt processor */
  ISA_CF_WAIT = (1 << 25),       /**< Wait state */
  ISA_CF_PAUSE = (1 << 26),      /**< Pause (for spin-wait loops) */
  
  /* Predicated Execution */
  ISA_CF_PREDICATION = (1 << 27), /**< Support for predicated execution */
  
  /* Atomic Execution */
  ISA_CF_ATOMIC_BLOCK = (1 << 28), /**< Support for atomic execution blocks */
  
  /* Jump Table Support */
  ISA_CF_JUMP_TABLE = (1 << 29),   /**< Support for jump tables */
} coil_instr_t;

/**
* @brief CPU Specific Instructions
* Instructions only applicable to CPUs, not general binary processing units
*/
typedef enum coil_cpu_instr {
  /* System Identification */
  ISA_CPU_CPUID = (1 << 0),      /**< Get CPU identification */
  ISA_CPU_RDTSC = (1 << 1),      /**< Read time-stamp counter */
  
  /* System Registers */
  ISA_CPU_RDMSR = (1 << 2),      /**< Read model-specific register */
  ISA_CPU_WRMSR = (1 << 3),      /**< Write model-specific register */
  
  /* Cache Control */
  ISA_CPU_CACHE_CONTROL = (1 << 4), /**< Cache control operations */
  ISA_CPU_PREFETCH = (1 << 5),      /**< Prefetch data */
  ISA_CPU_CLFLUSH = (1 << 6),       /**< Cache line flush */
  ISA_CPU_INVD = (1 << 7),          /**< Invalidate cache */
  ISA_CPU_WBINVD = (1 << 8),        /**< Write back and invalidate cache */
  
  /* Memory Management */
  ISA_CPU_TLB_CONTROL = (1 << 9),  /**< TLB control operations */
  ISA_CPU_INVLPG = (1 << 10),      /**< Invalidate TLB entry */
  
  /* System Management */
  ISA_CPU_SMM = (1 << 11),         /**< System Management Mode operations */
  ISA_CPU_HLT = (1 << 12),         /**< Halt until interrupt */
  ISA_CPU_RSM = (1 << 13),         /**< Resume from system management mode */
  
  /* Fast System Calls */
  ISA_CPU_FAST_SYSCALL = (1 << 14), /**< Fast system call operations */
  ISA_CPU_SYSENTER = (1 << 15),     /**< Fast system call entry */
  ISA_CPU_SYSEXIT = (1 << 16),      /**< Fast system call exit */
  
  /* Performance Monitoring */
  ISA_CPU_PMC = (1 << 17),         /**< Performance-monitoring counter operations */
  ISA_CPU_RDPMC = (1 << 18),       /**< Read performance-monitoring counter */
  
  /* Virtualization */
  ISA_CPU_VIRT = (1 << 19),        /**< Virtualization support */
  ISA_CPU_VMXON = (1 << 20),       /**< Enter VMX operation */
  ISA_CPU_VMXOFF = (1 << 21),      /**< Exit VMX operation */
  ISA_CPU_VMRUN = (1 << 22),       /**< Run virtual machine */
  ISA_CPU_VMEXIT = (1 << 23),      /**< Exit from virtual machine */
  
  /* Power Management */
  ISA_CPU_POWERMGMT = (1 << 24),   /**< Power management operations */
  ISA_CPU_MONITOR = (1 << 25),     /**< Monitor address range */
  ISA_CPU_MWAIT = (1 << 26),       /**< Wait until monitored address changes */
  
  /* Security Features */
  ISA_CPU_RANDOM = (1 << 27),      /**< Hardware random number generation */
  ISA_CPU_RDRAND = (1 << 28),      /**< Read random number */
  ISA_CPU_RDSEED = (1 << 29),      /**< Read random seed */
  ISA_CPU_ENCR = (1 << 30),        /**< Hardware encryption support */
  ISA_CPU_AES = (1ULL << 31),      /**< AES instruction support */
  ISA_CPU_SHA = (1ULL << 32),      /**< SHA instruction support */
  
  /* Trusted Execution */
  ISA_CPU_TRUSTED_EXEC = (1ULL << 33), /**< Trusted execution support */
  ISA_CPU_SGX = (1ULL << 34),      /**< Software Guard Extensions */
  ISA_CPU_SME = (1ULL << 35),      /**< Secure Memory Encryption */
} coil_cpu_instr_t;

/**
* @brief Hardware Type Configuration
*/
typedef struct coil_int {
  int _sizeof;                   /**< Size of the integer type in bytes */
  int _allign;                   /**< Alignment requirement in bytes */
  coil_cpu_reg_t reg;            /**< Register type used for this integer type */
  coil_type_instr_t basic_instr; /**< Basic instructions supported for this type */
  coil_int_instr_t int_instr;    /**< Integer-specific instructions supported */
} coil_int_t;

typedef struct coil_fp {
  int _sizeof;                   /**< Size of the floating-point type in bytes */
  int _allign;                   /**< Alignment requirement in bytes */
  coil_cpu_reg_t reg;            /**< Register type used for this floating-point type */
  coil_type_instr_t basic_instr; /**< Basic instructions supported for this type */
  coil_fp_instr_t fp_instr;      /**< Floating-point-specific instructions supported */
} coil_fp_t;

typedef struct coil_vec {
  int _sizeof;                   /**< Size of the vector type in bytes */
  int _allign;                   /**< Alignment requirement in bytes */
  coil_cpu_reg_t reg;            /**< Register type used for this vector type */
  coil_type_instr_t basic_instr; /**< Basic instructions supported for this type */
  coil_vec_instr_t vec_instr;    /**< Vector-specific instructions supported */
  int element_count;             /**< Number of elements in the vector */
  int element_size;              /**< Size of each element in bytes */
} coil_vec_t;

/**
* @brief Cache and Memory Subsystem Configuration
*/
typedef struct coil_cache_level {
  uint32_t size_kb;              /**< Cache size in kilobytes */
  uint32_t line_size;            /**< Cache line size in bytes */
  uint32_t associativity;        /**< Cache associativity */
  uint32_t latency_cycles;       /**< Access latency in cycles */
  bool inclusive;                /**< Whether the cache is inclusive */
  bool write_back;               /**< Whether the cache is write-back (vs write-through) */
} coil_cache_level_t;

typedef struct coil_memory_subsystem {
  coil_cache_level_t l1i;        /**< L1 instruction cache */
  coil_cache_level_t l1d;        /**< L1 data cache */
  coil_cache_level_t l2;         /**< L2 cache */
  coil_cache_level_t l3;         /**< L3 cache */
  
  uint32_t page_size_kb;         /**< Default page size in kilobytes */
  uint32_t tlb_entries;          /**< Number of TLB entries */
  
  uint32_t memory_bandwidth_gbps; /**< Memory bandwidth in GB/s */
  uint32_t memory_latency_ns;    /**< Memory latency in nanoseconds */
  
  bool support_uncached;         /**< Support for uncached memory access */
  bool support_write_combining;  /**< Support for write combining */
  bool support_write_through;    /**< Support for write-through memory */
  
  bool support_ecc;              /**< Support for ECC memory */
  bool support_non_temporal;     /**< Support for non-temporal memory access */
  
  bool support_locked_ops;       /**< Support for locked memory operations */
  bool support_unaligned_access; /**< Support for unaligned memory access */
} coil_memory_subsystem_t;

/**
* @brief Execution Resources Configuration
*/
typedef struct coil_execution {
  uint32_t pipeline_stages;      /**< Number of pipeline stages */
  uint32_t issue_width;          /**< Instruction issue width */
  uint32_t dispatch_width;       /**< Instruction dispatch width */
  
  uint32_t int_units;            /**< Number of integer execution units */
  uint32_t fp_units;             /**< Number of floating-point execution units */
  uint32_t vector_units;         /**< Number of vector execution units */
  uint32_t branch_units;         /**< Number of branch execution units */
  uint32_t load_store_units;     /**< Number of load/store units */
  
  bool out_of_order;             /**< Support for out-of-order execution */
  bool speculative_execution;    /**< Support for speculative execution */
  bool branch_prediction;        /**< Support for branch prediction */
  
  uint32_t reorder_buffer_size;  /**< Size of the reorder buffer */
  uint32_t reservation_stations; /**< Number of reservation stations */
} coil_execution_t;

/**
* @brief Base configuration
*/
typedef struct coil_conf {
  /* Version information */
  coil_version_t version;           /**< Structure version information */
  coil_pu_type_t pu;                /**< Processing unit information */
  
  /* Type capabilities */
  coil_int_t _int8;                 /**< 8-bit integer support */
  coil_int_t _int16;                /**< 16-bit integer support */
  coil_int_t _int32;                /**< 32-bit integer support */
  coil_int_t _int64;                /**< 64-bit integer support */
  coil_int_t _int128;               /**< 128-bit integer support (if available) */

  coil_fp_t _fp8e5m2;               /**< 8-bit E5M2 floating point support */
  coil_fp_t _fp8e4m3;               /**< 8-bit E4M3 floating point support */
  coil_fp_t _fp16;                  /**< 16-bit IEEE half precision */
  coil_fp_t _fpb16;                 /**< 16-bit brain floating point */
  coil_fp_t _fp32;                  /**< 32-bit IEEE single precision */
  coil_fp_t _fpt32;                 /**< 32-bit tensor float */
  coil_fp_t _fp64;                  /**< 64-bit IEEE double precision */
  coil_fp_t _fp80;                  /**< 80-bit x87 extended precision */
  coil_fp_t _fp128;                 /**< 128-bit IEEE quad precision */

  coil_vec_t _vec128;               /**< 128-bit vector */
  coil_vec_t _vec256;               /**< 256-bit vector */
  coil_vec_t _vec512;               /**< 512-bit vector */
  coil_vec_t _vec1024;              /**< 1024-bit vector (if available) */
  
  /* Instruction capabilities */
  coil_instr_t instr;               /**< Common processing unit instructions */

  /* Memory subsystem */
  coil_memory_subsystem_t memory;   /**< Memory subsystem configuration */
  
  /* Execution resources */
  coil_execution_t execution;       /**< Execution resources configuration */

  /* ABI definitions */
  // TODO
};

/**
* @brief CPU configuration structure with versioning and extended capabilities
*/
typedef struct coil_cpu {
  /* Version information */
  coil_version_t version;           /**< Structure version information */
  coil_pu_type_t pu;                /**< Processing unit information */
  
  /* Type capabilities */
  coil_int_t _int8;                 /**< 8-bit integer support */
  coil_int_t _int16;                /**< 16-bit integer support */
  coil_int_t _int32;                /**< 32-bit integer support */
  coil_int_t _int64;                /**< 64-bit integer support */
  coil_int_t _int128;               /**< 128-bit integer support (if available) */

  coil_fp_t _fp8e5m2;               /**< 8-bit E5M2 floating point support */
  coil_fp_t _fp8e4m3;               /**< 8-bit E4M3 floating point support */
  coil_fp_t _fp16;                  /**< 16-bit IEEE half precision */
  coil_fp_t _fpb16;                 /**< 16-bit brain floating point */
  coil_fp_t _fp32;                  /**< 32-bit IEEE single precision */
  coil_fp_t _fpt32;                 /**< 32-bit tensor float */
  coil_fp_t _fp64;                  /**< 64-bit IEEE double precision */
  coil_fp_t _fp80;                  /**< 80-bit x87 extended precision */
  coil_fp_t _fp128;                 /**< 128-bit IEEE quad precision */

  coil_vec_t _vec128;               /**< 128-bit vector */
  coil_vec_t _vec256;               /**< 256-bit vector */
  coil_vec_t _vec512;               /**< 512-bit vector */
  coil_vec_t _vec1024;              /**< 1024-bit vector (if available) */
  
  /* Instruction capabilities */
  coil_instr_t instr;               /**< Common processing unit instructions */

  /* Memory subsystem */
  coil_memory_subsystem_t memory;   /**< Memory subsystem configuration */
  
  /* Execution resources */
  coil_execution_t execution;       /**< Execution resources configuration */
  
  /* ABI definitions */
  // TODO

  /* Basic identification */
  coil_cpu_arch_t arch;             /**< CPU architecture */
  char ident[64];                   /**< CPU identifier */
  char product[64];                 /**< Product name */
  char vendor[64];                  /**< Vendor name */
  uint32_t generation;              /**< Generation or family */
  uint32_t year;                    /**< Release year */
  char microarch[64];               /**< Microarchitecture name */
  uint32_t arch_version;            /**< Architecture version */
  uint32_t impl_version;            /**< Implementation version */
  char isa_string[256];             /**< ISA string representation */

  /* Processing Specific Instructions */
  coil_cpu_instr_t instrcpu;        /**< CPU-specific instructions */
  
  /* Power management */
  uint32_t tdp_watts;               /**< Thermal design power in watts */
  uint32_t base_frequency_mhz;      /**< Base frequency in MHz */
  uint32_t max_frequency_mhz;       /**< Maximum frequency in MHz */
  bool dynamic_frequency;           /**< Support for dynamic frequency scaling */
  bool power_states;                /**< Support for power states (C-states) */
  
} coil_cpu_t;

/**
* @brief GPU-specific instructions and features
* For future extension to GPU support
*/
typedef enum coil_gpu_instr {
  /* Core GPU Operations */
  ISA_GPU_BARRIER = (1 << 0),      /**< Barrier synchronization */
  ISA_GPU_ATOMIC = (1 << 1),       /**< Atomic operations */
  ISA_GPU_VOTE = (1 << 2),         /**< Vote operations */
  ISA_GPU_SHUFFLE = (1 << 3),      /**< Shuffle operations */
  
  /* Thread Management */
  ISA_GPU_THREAD_SYNC = (1 << 4),  /**< Thread synchronization */
  ISA_GPU_WARP_SYNC = (1 << 5),    /**< Warp synchronization */
  ISA_GPU_BLOCK_SYNC = (1 << 6),   /**< Block synchronization */
  
  /* Parallel Primitives */
  ISA_GPU_REDUCE = (1 << 7),       /**< Reduction operations */
  ISA_GPU_SCAN = (1 << 8),         /**< Scan operations */
  
  /* Graphics Operations */
  ISA_GPU_TEXTURE = (1 << 9),      /**< Texture operations */
  ISA_GPU_RASTERIZE = (1 << 10),   /**< Rasterization operations */
  ISA_GPU_INTERP = (1 << 11),      /**< Interpolation operations */
  
  /* Specialized Math */
  ISA_GPU_FP16_MATH = (1 << 12),   /**< Half-precision math operations */
  ISA_GPU_TENSOR = (1 << 13),      /**< Tensor operations */
  ISA_GPU_MATRIX = (1 << 14),      /**< Matrix operations */
} coil_gpu_instr_t;

#endif // COIL_CONFIG_H