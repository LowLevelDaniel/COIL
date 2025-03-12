/**
* @file instruction.h
* @brief Instruction set definitions for COIL/HOIL
*
* This file defines the instruction set used by both the HOIL compiler
* and the COIL assembler, following the COIL Binary Specification.
*/

#ifndef COIL_INSTRUCTION_H
#define COIL_INSTRUCTION_H

#include "common/type_system.h"
#include <stdint.h>

/**
* @brief Opcodes for COIL instructions
*/
typedef enum {
  /* Arithmetic */
  OPCODE_ADD  = 0x01,  /**< Addition */
  OPCODE_SUB  = 0x02,  /**< Subtraction */
  OPCODE_MUL  = 0x03,  /**< Multiplication */
  OPCODE_DIV  = 0x04,  /**< Division */
  OPCODE_REM  = 0x05,  /**< Remainder */
  OPCODE_NEG  = 0x06,  /**< Negation */
  OPCODE_ABS  = 0x07,  /**< Absolute value */
  OPCODE_MIN  = 0x08,  /**< Minimum */
  OPCODE_MAX  = 0x09,  /**< Maximum */
  OPCODE_FMA  = 0x0A,  /**< Fused multiply-add */
  
  /* Logical */
  OPCODE_AND  = 0x10,  /**< Bitwise AND */
  OPCODE_OR   = 0x11,  /**< Bitwise OR */
  OPCODE_XOR  = 0x12,  /**< Bitwise XOR */
  OPCODE_NOT  = 0x13,  /**< Bitwise NOT */
  OPCODE_SHL  = 0x14,  /**< Shift left */
  OPCODE_SHR  = 0x15,  /**< Shift right */
  
  /* Comparison */
  OPCODE_CMP_EQ  = 0x20,  /**< Equal */
  OPCODE_CMP_NE  = 0x21,  /**< Not equal */
  OPCODE_CMP_LT  = 0x22,  /**< Less than */
  OPCODE_CMP_LE  = 0x23,  /**< Less than or equal */
  OPCODE_CMP_GT  = 0x24,  /**< Greater than */
  OPCODE_CMP_GE  = 0x25,  /**< Greater than or equal */
  
  /* Memory */
  OPCODE_LOAD     = 0x30,  /**< Load from memory */
  OPCODE_STORE    = 0x31,  /**< Store to memory */
  OPCODE_ATOMIC_OP = 0x32,  /**< Atomic memory operation */
  OPCODE_FENCE    = 0x33,  /**< Memory barrier */
  OPCODE_LEA      = 0x34,  /**< Load effective address */
  
  /* Control Flow */
  OPCODE_BR      = 0x40,  /**< Branch */
  OPCODE_SWITCH  = 0x41,  /**< Switch statement */
  OPCODE_CALL    = 0x42,  /**< Function call */
  OPCODE_RET     = 0x43,  /**< Return */
  
  /* Conversion */
  OPCODE_CONVERT = 0x50,  /**< Type conversion */
  OPCODE_TRUNC   = 0x51,  /**< Truncate */
  OPCODE_EXTEND  = 0x52,  /**< Extend */
  
  /* Vector */
  OPCODE_VADD     = 0x60,  /**< Vector addition */
  OPCODE_VSUB     = 0x61,  /**< Vector subtraction */
  OPCODE_VMUL     = 0x62,  /**< Vector multiplication */
  OPCODE_VDIV     = 0x63,  /**< Vector division */
  OPCODE_VDOT     = 0x64,  /**< Vector dot product */
  OPCODE_VCROSS   = 0x65,  /**< Vector cross product */
  OPCODE_VSPLAT   = 0x66,  /**< Scalar to vector */
  OPCODE_VEXTRACT = 0x67,  /**< Extract vector element */
  OPCODE_VINSERT  = 0x68,  /**< Insert vector element */
  
  /* Loads/Constants */
  OPCODE_LOAD_I8  = 0x70,  /**< Load 8-bit integer constant */
  OPCODE_LOAD_I16 = 0x71,  /**< Load 16-bit integer constant */
  OPCODE_LOAD_I32 = 0x72,  /**< Load 32-bit integer constant */
  OPCODE_LOAD_I64 = 0x73,  /**< Load 64-bit integer constant */
  OPCODE_LOAD_F32 = 0x74,  /**< Load 32-bit float constant */
  OPCODE_LOAD_F64 = 0x75,  /**< Load 64-bit float constant */
  OPCODE_UNDEF    = 0x76,  /**< Undefined value */
  
  /* Special */
  OPCODE_HLT      = 0xF0,  /**< Halt */
  OPCODE_NOP      = 0xF1,  /**< No operation */
  OPCODE_TRAP     = 0xF2,  /**< Trap/debug breakpoint */
  OPCODE_UNREACHABLE = 0xF3  /**< Unreachable code */
} opcode_t;

/**
* @brief Operand kinds
*/
typedef enum {
  OPERAND_REGISTER,      /**< Virtual register */
  OPERAND_IMMEDIATE,     /**< Immediate value */
  OPERAND_BASIC_BLOCK,   /**< Basic block reference */
  OPERAND_FUNCTION,      /**< Function reference */
  OPERAND_GLOBAL,        /**< Global variable reference */
  OPERAND_MEMORY         /**< Memory reference */
} operand_kind_t;

/**
* @brief Memory ordering options for memory operations
*/
typedef enum {
  MEMORY_ORDER_RELAXED,  /**< No ordering constraints */
  MEMORY_ORDER_ACQUIRE,  /**< Acquire ordering semantics */
  MEMORY_ORDER_RELEASE,  /**< Release ordering semantics */
  MEMORY_ORDER_ACQ_REL,  /**< Acquire-release ordering semantics */
  MEMORY_ORDER_SEQ_CST   /**< Sequentially consistent ordering */
} memory_order_t;

/**
* @brief Instruction flags
*/
typedef enum {
  FLAG_NONE           = 0x00,  /**< No flags */
  FLAG_VOLATILE       = 0x01,  /**< Volatile memory access */
  FLAG_SIGNED         = 0x02,  /**< Signed operation */
  FLAG_UNSIGNED       = 0x04,  /**< Unsigned operation */
  FLAG_EXACT          = 0x08,  /**< Exact operation (no overflow) */
  FLAG_FAST           = 0x10,  /**< Fast math (approx) */
  FLAG_TAIL_CALL      = 0x20,  /**< Tail call optimization */
  FLAG_INLINE         = 0x40,  /**< Inline function */
  FLAG_NOINLINE       = 0x80   /**< Never inline function */
} instruction_flag_t;

/**
* @brief Instruction operand
*/
typedef struct {
  operand_kind_t kind;     /**< Operand kind */
  
  union {
    uint32_t        reg;      /**< Register number */
    int64_t         imm;      /**< Immediate value */
    const char*     block;    /**< Basic block name */
    const char*     func;     /**< Function name */
    const char*     global;   /**< Global variable name */
    struct {
      uint32_t      base;     /**< Base register */
      int32_t       offset;   /**< Offset */
      uint32_t      index;    /**< Index register (0 if none) */
      uint8_t       scale;    /**< Scale for index (1, 2, 4, or 8) */
    } mem;                   /**< Memory reference */
  } value;
  
  coil_type_t      type;    /**< Operand type */
} instruction_operand_t;

/**
* @brief Instruction structure
*/
typedef struct {
  opcode_t               opcode;       /**< Operation code */
  uint8_t                flags;        /**< Instruction flags */
  instruction_operand_t  dest;         /**< Destination operand */
  instruction_operand_t* operands;     /**< Source operands */
  uint8_t                operand_count; /**< Number of source operands */
  coil_type_t            type;         /**< Operation type */
  const char*            result_name;  /**< Name for the result (optional) */
} instruction_t;

/**
* @brief Create a register operand
* 
* @param reg Register number
* @param type Operand type
* @return The register operand
*/
instruction_operand_t coil_create_reg_operand(uint32_t reg, coil_type_t type);

/**
* @brief Create an immediate operand
* 
* @param imm Immediate value
* @param type Operand type
* @return The immediate operand
*/
instruction_operand_t coil_create_imm_operand(int64_t imm, coil_type_t type);

/**
* @brief Create a basic block reference operand
* 
* @param name Basic block name
* @return The basic block reference operand
*/
instruction_operand_t coil_create_block_operand(const char* name);

/**
* @brief Create a function reference operand
* 
* @param name Function name
* @param type Function type
* @return The function reference operand
*/
instruction_operand_t coil_create_func_operand(const char* name, coil_type_t type);

/**
* @brief Create a global variable reference operand
* 
* @param name Global variable name
* @param type Global variable type
* @return The global variable reference operand
*/
instruction_operand_t coil_create_global_operand(const char* name, coil_type_t type);

/**
* @brief Create a memory reference operand
* 
* @param base Base register
* @param offset Offset
* @param index Index register (0 if none)
* @param scale Scale for index (1, 2, 4, or 8)
* @param type Referenced type
* @return The memory reference operand
*/
instruction_operand_t coil_create_mem_operand(
  uint32_t base,
  int32_t offset,
  uint32_t index,
  uint8_t scale,
  coil_type_t type
);

/**
* @brief Create a new instruction
* 
* @param opcode Operation code
* @param flags Instruction flags
* @param dest Destination operand
* @param operands Source operands
* @param operand_count Number of source operands
* @param type Operation type
* @param result_name Name for the result (can be NULL)
* @return The new instruction
*/
instruction_t* coil_create_instruction(
  opcode_t opcode,
  uint8_t flags,
  instruction_operand_t dest,
  instruction_operand_t* operands,
  uint8_t operand_count,
  coil_type_t type,
  const char* result_name
);

/**
* @brief Create a binary operation instruction
* 
* @param opcode Operation code
* @param flags Instruction flags
* @param dest Destination register
* @param lhs Left-hand side operand
* @param rhs Right-hand side operand
* @param type Operation type
* @param result_name Name for the result (can be NULL)
* @return The new instruction
*/
instruction_t* coil_create_binary_op(
  opcode_t opcode,
  uint8_t flags,
  uint32_t dest,
  instruction_operand_t lhs,
  instruction_operand_t rhs,
  coil_type_t type,
  const char* result_name
);

/**
* @brief Create a unary operation instruction
* 
* @param opcode Operation code
* @param flags Instruction flags
* @param dest Destination register
* @param src Source operand
* @param type Operation type
* @param result_name Name for the result (can be NULL)
* @return The new instruction
*/
instruction_t* coil_create_unary_op(
  opcode_t opcode,
  uint8_t flags,
  uint32_t dest,
  instruction_operand_t src,
  coil_type_t type,
  const char* result_name
);

/**
* @brief Create a load instruction
* 
* @param dest Destination register
* @param addr Memory address operand
* @param type Type to load
* @param flags Instruction flags (e.g., FLAG_VOLATILE)
* @param result_name Name for the result (can be NULL)
* @return The new instruction
*/
instruction_t* coil_create_load(
  uint32_t dest,
  instruction_operand_t addr,
  coil_type_t type,
  uint8_t flags,
  const char* result_name
);

/**
* @brief Create a store instruction
* 
* @param addr Memory address operand
* @param value Value to store
* @param flags Instruction flags (e.g., FLAG_VOLATILE)
* @return The new instruction
*/
instruction_t* coil_create_store(
  instruction_operand_t addr,
  instruction_operand_t value,
  uint8_t flags
);

/**
* @brief Create a branch instruction
* 
* @param cond Condition operand (NULL for unconditional)
* @param true_block Target block for true condition
* @param false_block Target block for false condition (NULL for unconditional)
* @return The new instruction
*/
instruction_t* coil_create_branch(
  instruction_operand_t* cond,
  const char* true_block,
  const char* false_block
);

/**
* @brief Create a function call instruction
* 
* @param dest Destination register (0 for void)
* @param func Function to call
* @param args Function arguments
* @param arg_count Number of arguments
* @param type Return type
* @param flags Instruction flags (e.g., FLAG_TAIL_CALL)
* @param result_name Name for the result (can be NULL)
* @return The new instruction
*/
instruction_t* coil_create_call(
  uint32_t dest,
  instruction_operand_t func,
  instruction_operand_t* args,
  uint8_t arg_count,
  coil_type_t type,
  uint8_t flags,
  const char* result_name
);

/**
* @brief Create a return instruction
* 
* @param value Value to return (NULL for void)
* @return The new instruction
*/
instruction_t* coil_create_return(instruction_operand_t* value);

/**
* @brief Free an instruction and associated resources
* 
* @param instr The instruction to free
*/
void coil_free_instruction(instruction_t* instr);

/**
* @brief Convert instruction to binary representation
* 
* @param instr The instruction
* @param buffer Buffer to write to
* @param buffer_size Size of the buffer
* @return Number of bytes written, or -1 on error
*/
int coil_instruction_to_binary(
  const instruction_t* instr,
  uint8_t* buffer,
  size_t buffer_size
);

/**
* @brief Parse instruction from binary representation
* 
* @param buffer Binary buffer
* @param buffer_size Size of the buffer
* @param registry Type registry for resolving types
* @return Parsed instruction, or NULL on error
*/
instruction_t* coil_instruction_from_binary(
  const uint8_t* buffer,
  size_t buffer_size,
  type_registry_t* registry
);

#endif /* COIL_INSTRUCTION_H */