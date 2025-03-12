/**
 * @file translator.h
 * @brief Translator from COIL instructions to native code
 *
 * This file defines the translator component for the COIL assembler,
 * which translates COIL instructions to native code for the target architecture.
 */

#ifndef COIL_TRANSLATOR_H
#define COIL_TRANSLATOR_H

#include "common/instruction.h"
#include "common/module.h"
#include "common/error.h"
#include "coil_assembler/target.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Native code buffer structure
 */
typedef struct native_buffer native_buffer_t;

/**
 * @brief Translator structure
 */
typedef struct translator translator_t;

/**
 * @brief Create a new translator
 * 
 * @param target Target configuration
 * @param error_context Error context
 * @return The new translator, or NULL on error
 */
translator_t* coil_create_translator(
  target_config_t* target,
  error_context_t* error_context
);

/**
 * @brief Free a translator
 * 
 * @param translator The translator to free
 */
void coil_free_translator(translator_t* translator);

/**
 * @brief Create a new native code buffer
 * 
 * @param translator The translator
 * @param initial_capacity Initial buffer capacity
 * @return The new native code buffer, or NULL on error
 */
native_buffer_t* coil_create_native_buffer(
  translator_t* translator,
  size_t initial_capacity
);

/**
 * @brief Free a native code buffer
 * 
 * @param buffer The buffer to free
 */
void coil_free_native_buffer(native_buffer_t* buffer);

/**
 * @brief Get the buffer data
 * 
 * @param buffer The native code buffer
 * @return The buffer data
 */
const uint8_t* coil_get_buffer_data(const native_buffer_t* buffer);

/**
 * @brief Get the buffer size
 * 
 * @param buffer The native code buffer
 * @return The buffer size
 */
size_t coil_get_buffer_size(const native_buffer_t* buffer);

/**
 * @brief Reset the buffer
 * 
 * @param buffer The native code buffer
 */
void coil_reset_buffer(native_buffer_t* buffer);

/**
 * @brief Translate a COIL instruction to native code
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
int coil_translate_instruction(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
);

/**
 * @brief Translate a COIL function to native code
 * 
 * @param translator The translator
 * @param function The COIL function
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
int coil_translate_function(
  translator_t* translator,
  const function_t* function,
  native_buffer_t* buffer
);

/**
 * @brief Translate a COIL module to native code
 * 
 * @param translator The translator
 * @param module The COIL module
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
int coil_translate_module(
  translator_t* translator,
  const module_t* module,
  native_buffer_t* buffer
);

/**
 * @brief Output the native code to a file
 * 
 * @param translator The translator
 * @param buffer The native code buffer
 * @param file Output file
 * @return 0 on success, -1 on error
 */
int coil_output_native_code(
  translator_t* translator,
  const native_buffer_t* buffer,
  FILE* file
);

/**
 * @brief Get the native code for a COIL instruction
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param code Output buffer for native code
 * @param code_size Size of output buffer
 * @return Size of native code, or -1 on error
 */
int coil_get_native_code(
  translator_t* translator,
  const instruction_t* instr,
  uint8_t* code,
  size_t code_size
);

/**
 * @brief Generate native assembly code for a COIL instruction
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of bytes written, or -1 on error
 */
int coil_generate_assembly(
  translator_t* translator,
  const instruction_t* instr,
  char* buffer,
  size_t size
);

/**
 * @brief Check if a COIL instruction can be translated
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @return true if the instruction can be translated, false otherwise
 */
bool coil_can_translate_instruction(
  translator_t* translator,
  const instruction_t* instr
);

/**
 * @brief Get the size of the native code for a COIL instruction
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @return Size of native code in bytes
 */
size_t coil_get_native_code_size(
  translator_t* translator,
  const instruction_t* instr
);

/**
 * @brief Set the optimization level
 * 
 * @param translator The translator
 * @param level Optimization level (0-3)
 * @return 0 on success, -1 on error
 */
int coil_set_optimization_level(
  translator_t* translator,
  uint32_t level
);

#endif /* COIL_TRANSLATOR_H */