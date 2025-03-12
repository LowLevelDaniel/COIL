/**
* @file optimizer.h
* @brief Optimization engine for COIL assembler
*
* This file defines the optimization engine component for the COIL assembler,
* which applies target-specific optimizations to generated code.
*/

#ifndef COIL_OPTIMIZER_H
#define COIL_OPTIMIZER_H

#include "common/instruction.h"
#include "common/module.h"
#include "common/error.h"
#include "coil_assembler/target.h"
#include <stdint.h>
#include <stdbool.h>

/**
* @brief Optimization engine structure
*/
typedef struct optimizer optimizer_t;

/**
* @brief Optimization level
*/
typedef enum {
  OPT_LEVEL_NONE = 0,   /**< No optimization */
  OPT_LEVEL_BASIC = 1,  /**< Basic optimizations */
  OPT_LEVEL_NORMAL = 2, /**< Normal optimizations */
  OPT_LEVEL_AGGRESSIVE = 3 /**< Aggressive optimizations */
} opt_level_t;

/**
* @brief Create a new optimizer
* 
* @param target Target configuration
* @param error_context Error context
* @return The new optimizer, or NULL on error
*/
optimizer_t* coil_create_optimizer(
  target_config_t* target,
  error_context_t* error_context
);

/**
* @brief Free an optimizer
* 
* @param optimizer The optimizer to free
*/
void coil_free_optimizer(optimizer_t* optimizer);

/**
* @brief Set optimization level
* 
* @param optimizer The optimizer
* @param level Optimization level
* @return 0 on success, -1 on error
*/
int coil_set_opt_level(optimizer_t* optimizer, opt_level_t level);

/**
* @brief Enable experimental optimizations
* 
* @param optimizer The optimizer
* @param enable True to enable, false to disable
*/
void coil_enable_experimental(optimizer_t* optimizer, bool enable);

/**
* @brief Optimize a function
* 
* @param optimizer The optimizer
* @param function The function to optimize
* @return 0 on success, -1 on error
*/
int coil_optimize_function(optimizer_t* optimizer, function_t* function);

/**
* @brief Optimize a module
* 
* @param optimizer The optimizer
* @param module The module to optimize
* @return 0 on success, -1 on error
*/
int coil_optimize_module(optimizer_t* optimizer, module_t* module);

/**
* @brief Optimize an instruction
* 
* @param optimizer The optimizer
* @param instr The instruction to optimize
* @return The optimized instruction, or NULL on error
*/
instruction_t* coil_optimize_instruction(optimizer_t* optimizer, instruction_t* instr);

/**
* @brief Optimize a basic block
* 
* @param optimizer The optimizer
* @param block The basic block to optimize
* @return 0 on success, -1 on error
*/
int coil_optimize_basic_block(optimizer_t* optimizer, basic_block_t* block);

#endif /* COIL_OPTIMIZER_H */