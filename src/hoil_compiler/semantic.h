/**
 * @file semantic.h
 * @brief Semantic analyzer for the HOIL compiler
 *
 * This file defines the semantic analyzer component for the HOIL compiler,
 * which performs semantic checks on the parsed AST.
 */

#ifndef HOIL_SEMANTIC_H
#define HOIL_SEMANTIC_H

#include "common/module.h"
#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Semantic analyzer structure
 */
typedef struct semantic_analyzer semantic_analyzer_t;

/**
 * @brief Create a new semantic analyzer
 * 
 * @param error_context Error context
 * @return The new semantic analyzer, or NULL on error
 */
semantic_analyzer_t* hoil_create_semantic_analyzer(error_context_t* error_context);

/**
 * @brief Free a semantic analyzer
 * 
 * @param analyzer The semantic analyzer to free
 */
void hoil_free_semantic_analyzer(semantic_analyzer_t* analyzer);

/**
 * @brief Analyze a module
 * 
 * @param analyzer The semantic analyzer
 * @param module The module to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_module(semantic_analyzer_t* analyzer, module_t* module);

/**
 * @brief Analyze a function
 * 
 * @param analyzer The semantic analyzer
 * @param module The module
 * @param function The function to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_function(semantic_analyzer_t* analyzer, module_t* module, function_t* function);

/**
 * @brief Analyze a global variable
 * 
 * @param analyzer The semantic analyzer
 * @param module The module
 * @param global The global variable to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_global(semantic_analyzer_t* analyzer, module_t* module, global_variable_t* global);

/**
 * @brief Analyze a type
 * 
 * @param analyzer The semantic analyzer
 * @param module The module
 * @param type The type to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_type(semantic_analyzer_t* analyzer, module_t* module, coil_type_t type);

/**
 * @brief Analyze a basic block
 * 
 * @param analyzer The semantic analyzer
 * @param module The module
 * @param function The function
 * @param block The basic block to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_basic_block(
  semantic_analyzer_t* analyzer,
  module_t* module,
  function_t* function,
  basic_block_t* block
);

/**
 * @brief Analyze an instruction
 * 
 * @param analyzer The semantic analyzer
 * @param module The module
 * @param function The function
 * @param block The basic block
 * @param instr The instruction to analyze
 * @return 0 on success, -1 on error
 */
int hoil_analyze_instruction(
  semantic_analyzer_t* analyzer,
  module_t* module,
  function_t* function,
  basic_block_t* block,
  instruction_t* instr
);

#endif /* HOIL_SEMANTIC_H */