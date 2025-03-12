/**
 * @file hoil_compiler.h
 * @brief HOIL compiler
 *
 * This file defines the HOIL compiler component, which compiles
 * HOIL source code to COIL binary format.
 */

#ifndef HOIL_COMPILER_H
#define HOIL_COMPILER_H

#include "common/module.h"
#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief HOIL compiler structure
 */
typedef struct hoil_compiler hoil_compiler_t;

/**
 * @brief Create a new HOIL compiler
 * 
 * @param error_context Error context
 * @return The new compiler, or NULL on error
 */
hoil_compiler_t* hoil_create_compiler(error_context_t* error_context);

/**
 * @brief Free a HOIL compiler
 * 
 * @param compiler The compiler to free
 */
void hoil_free_compiler(hoil_compiler_t* compiler);

/**
 * @brief Set whether to dump the AST
 * 
 * @param compiler The compiler
 * @param dump_ast True to dump AST
 */
void hoil_set_dump_ast(hoil_compiler_t* compiler, bool dump_ast);

/**
 * @brief Set whether to validate the module
 * 
 * @param compiler The compiler
 * @param validate True to validate
 */
void hoil_set_validate(hoil_compiler_t* compiler, bool validate);

/**
 * @brief Set whether to optimize the module
 * 
 * @param compiler The compiler
 * @param optimize True to optimize
 */
void hoil_set_optimize(hoil_compiler_t* compiler, bool optimize);

/**
 * @brief Compile HOIL source code
 * 
 * @param compiler The compiler
 * @param source HOIL source code
 * @param source_length Source code length
 * @param filename Source file name (for error reporting)
 * @return The compiled module, or NULL on error
 */
module_t* hoil_compile_string(
  hoil_compiler_t* compiler,
  const char* source,
  size_t source_length,
  const char* filename
);

/**
 * @brief Compile a HOIL source file
 * 
 * @param compiler The compiler
 * @param filename Source file name
 * @return The compiled module, or NULL on error
 */
module_t* hoil_compile_file(
  hoil_compiler_t* compiler,
  const char* filename
);

/**
 * @brief Write compiled module to a binary file
 * 
 * @param compiler The compiler
 * @param filename Output file name
 * @return 0 on success, -1 on error
 */
int hoil_write_binary(hoil_compiler_t* compiler, const char* filename);

/**
 * @brief Check if the compiler had an error
 * 
 * @param compiler The compiler
 * @return True if an error occurred
 */
bool hoil_had_error(hoil_compiler_t* compiler);

#endif /* HOIL_COMPILER_H */