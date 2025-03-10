/**
* @file format.h
* @brief Definition of the COIL instruction format
*
* This file contains definitions specific to the COIL (Computer Oriented Intermediate 
* Language) Format
*/

#ifndef COIL_FORMAT_H
#define COIL_FORMAT_H

#include <stdint.h>
#include <stddef.h>

/**
* @brief Maximum number of operands an instruction can have
*/
#define COIL_MAX_OPERANDS 8

/**
* @brief Operation codes for COIL instructions
*/
typedef enum coil_opcode : uint8_t {
  /* Memory Operations 0x0x */
  COIL_MEM_DEF_VAR  = 0x01, /**< Define a variable with a type */
  COIL_MEM_DEF_SYMB = 0x02, /**< Define a symbol (identifer to program memory) */
  COIL_MEM_PRMT_VAR = 0x03, /**< Promote a varibale (if this variable is to be used alot this will hint coil to store in a register) */
  COIL_MEM_DEMT_VAR = 0x04, /**< Demote a varibale */
  COIL_MEM_REDEF    = 0x05, /**< Redefine a variable with a new type */
  COIL_MEM_DLT      = 0x06, /**< Free the space occupied by a variable */
  COIL_MEM_MOVE     = 0x07, /**< Move value from src to dest */
  COIL_MEM_LOAD     = 0x08, /**< Load value from memory address */
  COIL_MEM_STORE    = 0x09, /**< Store value to memory address */
  COIL_MEM_PUSHA    = 0x0A, /**< Create a value of type state containing all registers */
  COIL_MEM_PUSHFD   = 0x0B, /**< Create a value of type state containing all flags */
  COIL_MEM_PUSHS    = 0x0C, /**< Create a value of type state containing all flags */
  COIL_MEM_POPA     = 0x0D, /**< Return all registes into the program */
  COIL_MEM_POPFD    = 0x0E, /**< Return all flags into the program */
  COIL_MEM_POPS     = 0x0F, /**< Return the state into the program */

  /* Arithmetic Operations 0x1x */
  COIL_ARITH_ADD    = 0x10, /**< Add two values into a separate destination (+) */
  COIL_ARITH_ADDE   = 0x11, /**< Add two values into the destination (+=) */
  COIL_ARITH_SUB    = 0x12, /**< Sub two values into a separate destination (-) */
  COIL_ARITH_SUBE   = 0x13, /**< Sub two values into the destination (-=) */
  COIL_ARITH_MUL    = 0x14, /**< Mul two values into a separate destination (*) */
  COIL_ARITH_MULE   = 0x15, /**< Mul two values into the destination (*=) */
  COIL_ARITH_DIV    = 0x16, /**< Div two values into a separate destination (/) */
  COIL_ARITH_DIVE   = 0x17, /**< Div two values into the destination (/=) */
  COIL_ARITH_MOD    = 0x18, /**< Mod two values into a separate destination (%) */
  COIL_ARITH_MODE   = 0x19, /**< Mod two values into the destination (%=) */
  COIL_ARITH_NEG    = 0x1A, /**< Neg two values into a separate destination */
  COIL_ARITH_NEGE   = 0x1B, /**< Neg two values into the destination */
  // flag modifications
  COIL_ARITH_CMP    = 0x1C, /**< compare two values and set flags */

  /* Bitwise Operations 0x2x */
  COIL_BIT_AND      = 0x21, /**< Bitwise AND */
  COIL_BIT_OR       = 0x22, /**< Bitwise OR */
  COIL_BIT_XOR      = 0x23, /**< Bitwise XOR */
  COIL_BIT_NOT      = 0x24, /**< Bitwise NOT */
  COIL_BIT_SHL      = 0x25, /**< Shift left x bits */
  COIL_BIT_SHR      = 0x26, /**< Shift right x bits */
  // flag modications
  COIL_ARITH_TEST   = 0x27, /**< bitwise test and set flags */
  
  /* Control Flow Operations 0x3x */
  COIL_CF_JMP       = 0x31, /**< Unconditional jump */
  COIL_CF_JEQ       = 0x32, /**< Jump if equal */
  COIL_CF_JNE       = 0x33, /**< Jump if not equal */
  COIL_CF_JLT       = 0x34, /**< Jump if less than */
  COIL_CF_JLE       = 0x35, /**< Jump if less or equal */
  COIL_CF_JGT       = 0x36, /**< Jump if greater than */
  COIL_CF_JGE       = 0x37, /**< Jump if greater or equal */
  COIL_CF_CALL      = 0x38, /**< unconditional jump to function with a return address */
  COIL_CF_RET       = 0x39, /**< unconditional jump to return address set by call */
  COIL_CF_INT       = 0x3A, /**< unconditional jump to interrupt routine */
  COIL_CF_IRET      = 0x3B, /**< unconditional jump to return addres set by int */
  COIL_CF_SYSC      = 0x3C, /**< unconditional jump to system call routine */

  /* Value Opcode */
  COIL_VAL_VAR      = 0x40, /**< Variable */
  COIL_VAL_IMM      = 0x40, /**< Immediate Value */
  COIL_VAL_SYMB     = 0x40, /**< Symbol */
} coil_opcode_t;

typedef enum coil_opcode_qualifer : uint8_t {
  COIL_QUAL_VOLATILE  = (1 << 1), /**< Don't optimize */
  COIL_QUAL_ATOMIC    = (1 << 2), /**< Should use an atomic operation */
  COIL_QUAL_NO_INLINE = (1 << 3), /**< Don't attempt inlining (for call) */
} coil_opcode_qualifer_t;

/**
* @brief Value Operation Codes
*/
typedef enum coil_type : uint8_t {
  // Fixed
  COIL_TYPE_INT8     = 0x01,  /**< 8-bit integer */
  COIL_TYPE_INT16    = 0x02,  /**< 16-bit integer */
  COIL_TYPE_INT32    = 0x03,  /**< 32-bit integer */
  COIL_TYPE_INT64    = 0x04,  /**< 64-bit integer */

  COIL_TYPE_UINT8    = 0x05,  /**< 8-bit unsigned integer */
  COIL_TYPE_UINT16   = 0x06,  /**< 16-bit unsigned integer */
  COIL_TYPE_UINT32   = 0x07,  /**< 32-bit unsigned integer */
  COIL_TYPE_UINT64   = 0x08,  /**< 64-bit unsigned integer */

  COIL_TYPE_FLOAT8   = 0x09,  /**< 8-bit floating point (E4M3)  */
  COIL_TYPE_FLOATB8  = 0x0A,  /**< 8-bit floating point (E5M2) */
  COIL_TYPE_FLOAT16  = 0x0B,  /**< 16-bit floating point */
  COIL_TYPE_FLOATB16 = 0x0C,  /**< 16-bit floating point */
  COIL_TYPE_FLOAT32  = 0x0D,  /**< 32-bit floating point */
  COIL_TYPE_FLOAT64  = 0x0E,  /**< 64-bit floating point */

  // Optimized Types
  COIL_TYPE_BOOL     = 0x10,  /**< Boolean type (1 bit (optimizations could be combining multiple booleans into the same register etc...)) */

  // Alias
  COIL_TYPE_ADDR     = 0x20,  /**< Pointer type (just a COIL_TYPE_DUINT) can be used for pointers / symbols etc... */
  COIL_TYPE_DINT     = 0x21,  /**< The default (and therefore fastest) integer type */
  COIL_TYPE_DUINT    = 0x22,  /**< The default (and therefore fastest) unsigned integer type */
  COIL_TYPE_DFLOAT   = 0x23,  /**< The default (and therefore fastest) float type */
  COIL_TYPE_SIZE     = 0x24,  /**< The largest fixed size unsigned integer */
  COIL_TYPE_SSIZE    = 0x25,  /**< The largest fixed size integer */

  // Special
  COIL_TYPE_STATE    = 0x30   /**< A specific architecture dependent state abstract */
} coil_type_t;

/**
*  @brief operand / value
*/
typedef struct coil_operand {
  coil_opcode_t op;
  coil_type_t type;
  uint64_t data;
  // Variable identifier
  // Symbol Map Index
  // Imm
} coil_operand_t;

/**
* @brief Structure representing a complete COIL instruction
*/
typedef struct coil_instruction {
  coil_opcode_t opcode;                       /**< Operation code */
  uint8_t qualifiers;                         /**< Operation qualifiers */
  uint8_t operand_count;                      /**< Number of operands */
  coil_operand_t operands[COIL_MAX_OPERANDS]; /**< Array of operands */
} coil_instruction_t;

#endif /* COIL_FORMAT_H */