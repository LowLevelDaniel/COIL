/**
* @file isa.h
* 
* @brief runtime memory of COIL Instructions
*
* [opcode: uint16_t][operand count:uint8_t][[operand]...]
* [operand] = [type:uint8_t][qualifer: uint8_t][data:void[]]
* 
*/

#ifndef COIL_ISA_H
#define COIL_ISA_H

#include <stdint.h>

typedef enum coil_isa : uint16_t {
  // Control Flow
  ISA_CF_BRANCH = 0x01   , /**< Branch/jump */
  ISA_CF_BRANCH_COND     , /**< Conditional branch */
  ISA_CF_CALL            , /**< Call subroutine */
  ISA_CF_RET             , /**< Return from subroutine */
  ISA_CF_JUMP_REG        , /**< Jump to register */
  ISA_CF_BRANCH_INDIRECT , /**< Indirect branch */
  ISA_CF_BRANCH_PREDICT  , /**< Branch prediction hints */
  ISA_CF_CONDITIONAL_MOVE, /**< Conditional move */
  ISA_CF_CONDITIONAL_SET , /**< Conditional set */
  ISA_CF_CONDITIONAL_SEL , /**< Conditional select */
  ISA_CF_ZEROING_REG     , /**< Zero-idiom recognition */
  ISA_CF_BRANCH_REG      , /**< Branch to register */
  ISA_CF_JUMP_TABLE      , /**< Jump table support */
  ISA_CF_COMPUTED_GOTO   , /**< Computed goto */
  ISA_CF_PRED_EXECUTION  , /**< Predicated execution */
  ISA_CF_GUARD           , /**< Guarded execution */
  ISA_CF_TRAP            , /**< Trap instruction */
  ISA_CF_SYSCALL         , /**< System call */
  ISA_CF_BREAK           , /**< Breakpoint */
  ISA_CF_EXCEPTION       , /**< Exception generation */
  ISA_CF_FUTEX           , /**< Fast user-space mutex */
  ISA_CF_MONITOR         , /**< Monitor memory */
  ISA_CF_MWAIT           , /**< Wait for monitor event */
  ISA_CF_YIELD           , /**< Yield to other thread */
  ISA_CF_PAUSE           , /**< Spin-wait hint */
  ISA_CF_WFE             , /**< Wait for event */
  ISA_CF_SEV             , /**< Send event */
  ISA_CF_WFI             , /**< Wait for interrupt */
  ISA_CF_BRANCH_LIKELY   , /**< Branch likely hint */
  ISA_CF_BRANCH_FAR      , /**< Far branch */
  ISA_CF_BRANCH_LINK     , /**< Branch and link */
  ISA_CF_BRANCH_COUNT    , /**< Branch and decrement */
  ISA_CF_BRANCH_EXCHANGE , /**< Branch and exchange execution state */
  ISA_CF_PRIV_CHANGE     , /**< Change privilege level */
  ISA_CF_SYSTEM_REG      , /**< System register access */
  ISA_CF_HALT            , /**< Halt processor */
  ISA_CF_SLEEP           , /**< Enter sleep mode */
  ISA_CF_CACHE_OP        , /**< Cache operation */
  ISA_CF_TLB_OP          , /**< TLB operation */
  ISA_CF_SMC             , /**< Secure monitor call */
  ISA_CF_HVC             , /**< Hypervisor call */
  ISA_CF_VMM             , /**< Virtual machine management */
  ISA_CF_SETJMP          , /**< Save execution context */
  ISA_CF_LONGJMP         , /**< Restore execution context */
  ISA_CF_LOOP_BUFFER     , /**< Hardware loop buffer */
  ISA_CF_BRANCH_FUSION   , /**< Branch fusion */
  ISA_CF_MACRO_OP_FUSION , /**< Macro-op fusion */
  ISA_CF_CONST_PROP      , /**< Constant propagation */
  ISA_CF_GUARD_CHECK     , /**< Guard checking */
  ISA_CF_SOFT_ERROR      , /**< Soft error detection */
  ISA_CF_ERROR_RECOVERY  , /**< Error recovery instructions */
  ISA_CF_FAULT_COLLECT   , /**< Fault collection */
  ISA_CF_HTM_BEGIN       , /**< Hardware transaction begin */
  ISA_CF_HTM_END         , /**< Hardware transaction end */
  ISA_CF_HTM_ABORT       , /**< Hardware transaction abort */
  ISA_CF_HTM_TEST        , /**< Hardware transaction test */
  ISA_CF_VMEXIT          , /**< VM exit handling */
  ISA_CF_VMENTER         , /**< VM entry */
  ISA_CF_VMCALL          , /**< VM call */

  // Memory Operations
  ISA_MEM_FENCE          ,  /**< Memory fence/barrier */
  ISA_MEM_ACQUIRE_BARRIER,  /**< Acquire barrier */
  ISA_MEM_RELEASE_BARRIER,  /**< Release barrier */
  ISA_MEM_DMB            ,  /**< Data memory barrier */
  ISA_MEM_DSB            ,  /**< Data synchronization barrier */
  ISA_MEM_ISB            ,  /**< Instruction synchronization barrier */
  ISA_MEM_SPECULATION_BAR,  /**< Speculation barrier */
  ISA_MEM_SYNC_ALL       ,  /**< Synchronize all memory accesses */
  ISA_MEM_SYNC_PROCESS   ,  /**< Synchronize process memory */
  ISA_MEM_SYNC_THREAD    ,  /**< Synchronize thread memory */
  ISA_MEM_SYNC_ACQUIRE   , /**< Acquire semantics */
  ISA_MEM_SYNC_RELEASE   , /**< Release semantics */
  ISA_MEM_SYNC_ACQREL    , /**< Acquire-release semantics */
  ISA_MEM_SYNC_CONSUME   , /**< Consume semantics */
  ISA_MEM_SYNC_SC        , /**< Sequential consistency */
  ISA_MEM_SYNC_LWSYNC    , /**< Lightweight sync (PowerPC) */
  ISA_MEM_SYNC_HWSYNC    , /**< Hardware sync (PowerPC) */
  ISA_MEM_SYNC_ISYNC     , /**< Instruction sync (PowerPC) */
  ISA_MEM_SYNC_TBEGIN    , /**< Transaction begin */
  ISA_MEM_SYNC_TEND      , /**< Transaction end */
  ISA_MEM_SYNC_TABORT    , /**< Transaction abort */

  ISA_MEM_LOAD           , /**< Load from memory */
  ISA_MEM_STORE          , /**< Store to memory */
  ISA_MEM_PREFETCH       , /**< Prefetch */
  ISA_MEM_MOVE           , /**< Move register to register */
  ISA_MEM_LOAD_IMM       , /**< Load immediate value */
  ISA_MEM_EXCHANGE       , /**< Exchange/swap values */
  ISA_MEM_PUSH           , /**< Push to stack */
  ISA_MEM_POP            , /**< Pop from stack */
  
  ISA_MEM_LOAD_MULTIPLE  , /**< Load multiple registers */
  ISA_MEM_STORE_MULTIPLE , /**< Store multiple registers */
  ISA_MEM_LOAD_PAIRED    , /**< Load pair of registers */
  ISA_MEM_STORE_PAIRED   , /**< Store pair of registers */

  ISA_MEM_ACQ            , /**< Memory barrier - acquire */
  ISA_MEM_REL            , /**< Memory barrier - release */
  ISA_MEM_ACQREL         , /**< Memory barrier - acquire-release */
  ISA_MEM_SEQ_CST        , /**< Memory barrier - sequential consistency */

  ISA_MEM_UNALIGNED_LOAD    , /**< Unaligned memory load */
  ISA_MEM_UNALIGNED_STORE   , /**< Unaligned memory store */
  ISA_MEM_NON_TEMPORAL_LOAD , /**< Non-temporal load */
  ISA_MEM_NON_TEMPORAL_STORE, /**< Non-temporal store */
  ISA_MEM_LOCKED_LOAD       , /**< Locked load */
  ISA_MEM_LOCKED_STORE      , /**< Locked store */
  ISA_MEM_COMPARE_EXCHANGE  , /**< Compare and exchange */
  
  ISA_MEM_ALLOCA           , /**< Dynamic stack allocation */
  ISA_MEM_STACK_SAVE       , /**< Save stack pointer */
  ISA_MEM_STACK_RESTORE    , /**< Restore stack pointer */

  // Basic Arithmetic
  ISA_BMATH_ADD,  /**< Addition */
  ISA_BMATH_SUB,  /**< Subtraction */
  ISA_BMATH_MUL,  /**< Multiplication */
  ISA_BMATH_DIV,  /**< Division */
  ISA_BMATH_REM,  /**< Remainder/Modulo */
  ISA_BMATH_NEG,  /**< Negation */
  ISA_BMATH_ABS,  /**< Absolute value */
  ISA_BMATH_MIN,  /**< Minimum */
  ISA_BMATH_MAX,  /**< Maximum */

  ISA_BMATH_MULADD, /**< Multiply-add */
  ISA_BMATH_MULSUB, /**< Multiply-subtract */
  ISA_BMATH_MADD,   /**< Multiply-accumulate */
  ISA_BMATH_MSUB,   /**< Multiply-subtract-accumulate */
  ISA_BMATH_ADDC,   /**< Add with carry */
  ISA_BMATH_SUBC,   /**< Subtract with carry/borrow */
  ISA_BMATH_MULH,   /**< Multiply high */

  // Bit Manipulation
  ISA_BIT_AND          , /**< Bitwise AND */
  ISA_BIT_OR           , /**< Bitwise OR */
  ISA_BIT_XOR          , /**< Bitwise XOR */
  ISA_BIT_NOT          , /**< Bitwise NOT */
  ISA_BIT_ANDN         , /**< AND with complement */
  ISA_BIT_ORN          , /**< OR with complement */
  ISA_BIT_XNOR         , /**< Exclusive NOR */
  ISA_BIT_SHL          , /**< Shift left logical */
  ISA_BIT_SHR          , /**< Shift right logical */
  ISA_BIT_SAR          , /**< Shift right arithmetic */
  ISA_BIT_ROL          , /**< Rotate left */
  ISA_BIT_ROR          , /**< Rotate right */
  ISA_BIT_SET          , /**< Set bit */
  ISA_BIT_CLR          , /**< Clear bit */
  ISA_BIT_TOGGLE       , /**< Toggle bit */
  ISA_BIT_TEST         , /**< Test bit */
  ISA_BIT_EXTRACT      , /**< Extract bits */
  ISA_BIT_INSERT       , /**< Insert bits */
  ISA_BIT_POPCOUNT     , /**< Population count */
  ISA_BIT_CLZ          , /**< Count leading zeros */
  ISA_BIT_CTZ          , /**< Count trailing zeros */
  ISA_BIT_BYTE_SWAP    , /**< Byte swap */
  ISA_BIT_FIELD_EXTRACT, /**< Extract contiguous bit field */
  ISA_BIT_FIELD_INSERT , /**< Insert contiguous bit field */
  ISA_BIT_REVERSE      , /**< Reverse bit order */
  ISA_BIT_BYTE_REVERSE , /**< Reverse byte order */
  ISA_BIT_PARITY       , /**< Calculate parity */

  // Advanced Arithmetic
  ISA_AMATH_FMA        , /**< Fused multiply-add */
  ISA_AMATH_FMS        , /**< Fused multiply-subtract */
  ISA_AMATH_FNMA       , /**< Fused negate-multiply-add */
  ISA_AMATH_FNMS       , /**< Fused negate-multiply-subtract */
  ISA_AMATH_SQRT       , /**< Square root */
  ISA_AMATH_RSQRT      , /**< Reciprocal square root */
  ISA_AMATH_RCP        , /**< Reciprocal */
  ISA_AMATH_CBRT       , /**< Cube root */
  ISA_AMATH_HYPOT      , /**< Hypotenuse calculation */
  ISA_AMATH_EXP        , /**< Exponential */
  ISA_AMATH_LOG        , /**< Logarithm */
  ISA_AMATH_POW        , /**< Power function */
  ISA_AMATH_SIN        , /**< Sine */
  ISA_AMATH_COS        , /**< Cosine */
  ISA_AMATH_TAN        , /**< Tangent */

  // Atomic Operations
  ISA_ATOM_ADD         , /**< Atomic add */
  ISA_ATOM_SUB         , /**< Atomic subtract */
  ISA_ATOM_AND         , /**< Atomic AND */
  ISA_ATOM_OR          , /**< Atomic OR */
  ISA_ATOM_XOR         , /**< Atomic XOR */
  ISA_ATOM_NAND        , /**< Atomic NAND */
  ISA_ATOM_XCHG        , /**< Atomic exchange */
  ISA_ATOM_CAS         , /**< Compare and swap */
  ISA_ATOM_FETCH_ADD   , /**< Atomic fetch and add */
  ISA_ATOM_FETCH_SUB   , /**< Atomic fetch and subtract */
  ISA_ATOM_FETCH_AND   , /**< Atomic fetch and AND */
  ISA_ATOM_FETCH_OR    , /**< Atomic fetch and OR */
  ISA_ATOM_FETCH_XOR   , /**< Atomic fetch and XOR */
  ISA_ATOM_FETCH_NAND  , /**< Atomic fetch and NAND */
  
  // Vector Operations
  ISA_VEC_ADD          , /**< Vector add */
  ISA_VEC_SUB          , /**< Vector subtract */
  ISA_VEC_MUL          , /**< Vector multiply */
  ISA_VEC_DIV          , /**< Vector divide */
  ISA_VEC_MAC          , /**< Vector multiply-accumulate */
  ISA_VEC_PACK         , /**< Vector pack */
  ISA_VEC_UNPACK       , /**< Vector unpack */
  ISA_VEC_SHUFFLE      , /**< Vector shuffle */
  ISA_VEC_INSERT       , /**< Vector insert element */
  ISA_VEC_EXTRACT      , /**< Vector extract element */
  ISA_VEC_SPLAT        , /**< Splat scalar to all elements */
  ISA_VEC_DOT          , /**< Vector dot product */
  ISA_VEC_GATHER       , /**< Vector gather */
  ISA_VEC_SCATTER      , /**< Vector scatter */
  
  // Variable Management (Optional abstraction layer)
  ISA_VAR_DECL         , /**< Declare variable */
  ISA_VAR_ALLOC        , /**< Allocate variable */
  ISA_VAR_FREE         , /**< Free variable */
  ISA_VAR_SCOPE_ENTER  , /**< Enter variable scope */
  ISA_VAR_SCOPE_EXIT   , /**< Exit variable scope */
  ISA_VAR_LIFETIME_START, /**< Start variable lifetime */
  ISA_VAR_LIFETIME_END , /**< End variable lifetime */
  ISA_VAR_BIND         , /**< Bind variable to register */
  ISA_VAR_UNBIND       , /**< Unbind variable from register */
  ISA_VAR_SPILL        , /**< Spill variable to memory */
  ISA_VAR_RELOAD       , /**< Reload variable from memory */
  
  // Frame Management
  ISA_FRAME_ENTER      , /**< Function entry */
  ISA_FRAME_EXIT       , /**< Function exit */
  ISA_FRAME_SAVE       , /**< Save registers */
  ISA_FRAME_RESTORE    , /**< Restore registers */
  
  // Data Movement Directives
  ISA_DATA_ALIGN       , /**< Align data */
  ISA_DATA_SECTION     , /**< Specify data section */
  ISA_DATA_GLOBAL      , /**< Declare global data */
  ISA_DATA_LOCAL       , /**< Declare local data */
  
  // End of ISA
  ISA_MAX_OPCODE
} coil_isa_t;

typedef enum coil_type : uint8_t {
  COIL_TYPE_VOID = 0x00,
  
  // Integer Types
  COIL_TYPE_INT8 = 0x01,
  COIL_TYPE_INT16,
  COIL_TYPE_INT32,
  COIL_TYPE_INT64,
  COIL_TYPE_INT128,
  
  // Unsigned Integer Types
  COIL_TYPE_UINT8,
  COIL_TYPE_UINT16,
  COIL_TYPE_UINT32,
  COIL_TYPE_UINT64,
  COIL_TYPE_UINT128,
  
  // Floating Point Types
  COIL_TYPE_FP8_e5m2,
  COIL_TYPE_FP8_e4m3,
  COIL_TYPE_FP16b,
  COIL_TYPE_FP16,
  COIL_TYPE_FP32,
  COIL_TYPE_FP32t,
  COIL_TYPE_FP64,
  COIL_TYPE_FP80,
  COIL_TYPE_FP128,

  // Vector Types
  COIL_TYPE_VEC128,
  COIL_TYPE_VEC256,
  COIL_TYPE_VEC512,
  COIL_TYPE_VEC1024,
  
  // Pointer Types
  COIL_TYPE_PTR32,
  COIL_TYPE_PTR64,
  
  // Special Types
  COIL_TYPE_LABEL,
  COIL_TYPE_SYMBOL,
  COIL_TYPE_VARREF,    /**< Variable reference for optional abstraction */
  COIL_TYPE_REGREF,    /**< Register reference */
  
  // End of Types
  COIL_TYPE_MAX
} coil_type_t;

typedef enum coil_qual : uint8_t {
  COIL_QUAL_NULL = (1 << 0),      /**< Null/empty qualifier */
  COIL_QUAL_IMM = (1 << 1),       /**< Immediate value */
  COIL_QUAL_VAR = (1 << 2),       /**< Variable reference */
  COIL_QUAL_SYMB = (1 << 3),      /**< Symbolic reference */
  COIL_QUAL_VREG = (1 << 4),      /**< Virtual register reference */
  COIL_QUAL_VOLATILE = (1 << 5),  /**< Volatile access */
  COIL_QUAL_ATOMIC = (1 << 6),    /**< Atomic access */
  COIL_QUAL_SIGNED = (1 << 7),    /**< Signed interpretation */
} coil_qual_t;

/**
 * @brief Extended qualifiers for variable management
 * Only used with optional variable abstraction
 */
typedef enum coil_var_qual : uint8_t {
  COIL_VARQ_TEMP = (1 << 0),      /**< Temporary variable */
  COIL_VARQ_PARAM = (1 << 1),     /**< Function parameter */
  COIL_VARQ_RETURN = (1 << 2),    /**< Return value */
  COIL_VARQ_GLOBAL = (1 << 3),    /**< Global variable */
  COIL_VARQ_LOCAL = (1 << 4),     /**< Local variable */
  COIL_VARQ_HOT = (1 << 5),       /**< Frequently accessed variable */
  COIL_VARQ_COLD = (1 << 6),      /**< Rarely accessed variable */
  COIL_VARQ_PRESERVED = (1 << 7), /**< Value must be preserved across calls */
} coil_var_qual_t;

/**
 * @brief Variable descriptor for the optional abstraction layer
 */
typedef struct coil_var_desc {
  uint32_t id;                    /**< Unique variable identifier */
  coil_type_t type;               /**< Data type of variable */
  uint8_t qualifiers;             /**< Basic qualifiers */
  uint8_t var_qualifiers;         /**< Variable-specific qualifiers */
  uint16_t scope_level;           /**< Nesting level of scope */
  uint32_t alignment;             /**< Required alignment in bytes */
  uint32_t size;                  /**< Size in bytes */
} coil_var_desc_t;

/**
 * @brief Frame descriptor for function stack frame management
 */
typedef struct coil_frame_desc {
  uint32_t id;                    /**< Frame identifier */
  uint32_t size;                  /**< Total frame size in bytes */
  uint32_t alignment;             /**< Frame alignment requirement */
  uint16_t saved_regs;            /**< Bitmap of saved registers */
  uint16_t local_var_count;       /**< Number of local variables */
  uint16_t arg_size;              /**< Size of arguments in bytes */
  uint16_t spill_size;            /**< Size of register spill area */
  uint8_t frame_reg;              /**< Register used as frame pointer */
  uint8_t flags;                  /**< Frame flags */
} coil_frame_desc_t;

/**
 * @brief Frame flags for optimization hints
 */
typedef enum coil_frame_flags : uint8_t {
  COIL_FRAME_LEAF = (1 << 0),     /**< Leaf function (makes no calls) */
  COIL_FRAME_DYNAMIC = (1 << 1),  /**< Has dynamic stack allocation */
  COIL_FRAME_NEEDS_FP = (1 << 2), /**< Requires frame pointer */
  COIL_FRAME_CUSTOM_ABI = (1 << 3), /**< Uses custom calling convention */
  COIL_FRAME_NAKED = (1 << 4),    /**< No prologue/epilogue */
  COIL_FRAME_VARARGS = (1 << 5),  /**< Variable arguments */
  COIL_FRAME_EXCEPTION = (1 << 6), /**< Has exception handling */
  COIL_FRAME_UNWIND = (1 << 7),   /**< Has unwind information */
} coil_frame_flags_t;

/**
 * @brief Operand structure
 */
typedef struct coil_operand {
  coil_type_t type;               /**< Operand data type */
  uint8_t qualifier;              /**< Operand qualifiers */
  void* data;                     /**< Operand data (content depends on type) */
} coil_operand_t;

/**
 * @brief Instruction structure
 */
typedef struct coil_instr {
  coil_isa_t opcode;              /**< Instruction opcode */
  uint8_t operand_count;          /**< Number of operands */
  coil_operand_t* operands;       /**< Array of operands */
} coil_instr_t;

// CPU (cpu specific ISA)
// GPU (gpu specific ISA)
// TPU (tpu specific ISA)
// QPU (qpu specific ISA)
// ...
// END

#endif /* COIL_ISA_H */