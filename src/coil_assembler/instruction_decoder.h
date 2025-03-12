/**
* @file instruction_decoder.h
* @brief COIL instruction decoder
*
* This file defines the instruction decoder component for the COIL assembler,
* which decodes COIL binary instructions into an in-memory representation.
*/

#ifndef COIL_INSTRUCTION_DECODER_H
#define COIL_INSTRUCTION_DECODER_H

#include "common/instruction.h"
#include "common/module.h"
#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
* @brief Instruction decoder structure
*/
typedef struct instruction_decoder instruction_decoder_t;

/**
* @brief Create a new instruction decoder
* 
* @param error_context Error context
* @return The new instruction decoder, or NULL on error
*/
instruction_decoder_t* coil_create_instruction_decoder(error_context_t* error_context);

/**
* @brief Free an instruction decoder
* 
* @param decoder The instruction decoder to free
*/
void coil_free_instruction_decoder(instruction_decoder_t* decoder);

/**
* @brief Decode an instruction from binary data
* 
* @param decoder The instruction decoder
* @param data Binary data
* @param size Data size
* @param module The module (for type registry)
* @param function The function (for register mapping)
* @return The decoded instruction, or NULL on error
*/
instruction_t* coil_decode_instruction(
  instruction_decoder_t* decoder,
  const uint8_t* data,
  size_t size,
  module_t* module,
  function_t* function
);

/**
* @brief Decode a function's instructions from binary data
* 
* @param decoder The instruction decoder
* @param data Binary data
* @param size Data size
* @param module The module
* @param function The function
* @return 0 on success, -1 on error
*/
int coil_decode_function_instructions(
  instruction_decoder_t* decoder,
  const uint8_t* data,
  size_t size,
  module_t* module,
  function_t* function
);

/**
* @brief Get the size of an instruction in binary form
* 
* @param data Binary data
* @param size Data size
* @return Size of the instruction in bytes, or 0 if invalid
*/
uint32_t coil_get_instruction_size(const uint8_t* data, size_t size);

/**
* @brief Check if an instruction is valid
* 
* @param opcode Instruction opcode
* @param operand_count Number of operands
* @return true if valid, false otherwise
*/
bool coil_is_valid_instruction(uint8_t opcode, uint8_t operand_count);

/**
* @brief Get the mnemonic for an opcode
* 
* @param opcode Instruction opcode
* @return The mnemonic, or NULL if invalid
*/
const char* coil_get_opcode_mnemonic(uint8_t opcode);

/**
* @brief Print an instruction to a string
* 
* @param instr The instruction
* @param buffer Output buffer
* @param size Buffer size
* @return Number of bytes written, or -1 on error
*/
int coil_print_instruction(
  const instruction_t* instr,
  char* buffer,
  size_t size
);

#endif /* COIL_INSTRUCTION_DECODER_H */