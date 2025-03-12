/**
* @file optimizer.c
* @brief Optimization engine implementation for COIL assembler
*/

#include "coil_assembler/optimizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
* @brief Optimizer structure
*/
struct optimizer {
  target_config_t* target;         /**< Target configuration */
  error_context_t* error_context;  /**< Error context */
  opt_level_t      level;          /**< Optimization level */
  bool             experimental;   /**< Enable experimental optimizations */
  bool             had_error;      /**< True if an error occurred */
};

/**
* @brief Report an optimizer error
* 
* @param optimizer The optimizer
* @param code Error code
* @param message Error message
*/
static void report_error(
  optimizer_t* optimizer,
  uint32_t code,
  const char* message
) {
  if (optimizer->error_context != NULL) {
    REPORT_ERROR(optimizer->error_context, ERROR_ERROR, ERROR_CATEGORY_ASSEMBLER,
                code, message, NULL, 0, 0);
  }
  
  optimizer->had_error = true;
}

optimizer_t* coil_create_optimizer(
  target_config_t* target,
  error_context_t* error_context
) {
  if (target == NULL) {
    return NULL;
  }
  
  optimizer_t* optimizer = (optimizer_t*)malloc(sizeof(optimizer_t));
  if (optimizer == NULL) {
    return NULL;
  }
  
  optimizer->target = target;
  optimizer->error_context = error_context;
  optimizer->level = OPT_LEVEL_NONE;
  optimizer->experimental = false;
  optimizer->had_error = false;
  
  return optimizer;
}

void coil_free_optimizer(optimizer_t* optimizer) {
  if (optimizer == NULL) {
    return;
  }
  
  free(optimizer);
}

int coil_set_opt_level(optimizer_t* optimizer, opt_level_t level) {
  if (optimizer == NULL) {
    return -1;
  }
  
  if (level > OPT_LEVEL_AGGRESSIVE) {
    return -1;
  }
  
  optimizer->level = level;
  
  return 0;
}

void coil_enable_experimental(optimizer_t* optimizer, bool enable) {
  if (optimizer == NULL) {
    return;
  }
  
  optimizer->experimental = enable;
}

/**
* @brief Perform peephole optimization on a sequence of instructions
* 
* @param optimizer The optimizer
* @param instrs Array of instructions
* @param count Number of instructions
* @return 0 on success, -1 on error
*/
static int peephole_optimize(
  optimizer_t* optimizer,
  instruction_t** instrs,
  uint32_t count
) {
  if (count < 2) {
    return 0;  // Nothing to optimize
  }
  
  // Simple peephole optimizations for demonstration
  // In a real implementation, this would be much more sophisticated
  
  for (uint32_t i = 0; i < count - 1; i++) {
    instruction_t* curr = instrs[i];
    instruction_t* next = instrs[i + 1];
    
    // Pattern: MOV r1, r2; MOV r2, r1 -> eliminate second MOV
    if (curr->opcode == 0x89 && next->opcode == 0x89) {  // MOV opcodes
      // Check if the source/dest are swapped
      if (curr->dest.kind == OPERAND_REGISTER &&
          curr->operands[0].kind == OPERAND_REGISTER &&
          next->dest.kind == OPERAND_REGISTER &&
          next->operands[0].kind == OPERAND_REGISTER) {
        
        if (curr->dest.value.reg == next->operands[0].value.reg &&
            curr->operands[0].value.reg == next->dest.value.reg) {
          // Replace second MOV with NOP
          next->opcode = 0xF1;  // NOP opcode
          next->operand_count = 0;
        }
      }
    }
    
    // Pattern: ADD r1, 0 -> eliminate (no-op)
    if (curr->opcode == OPCODE_ADD &&
        curr->operand_count == 2 &&
        curr->operands[1].kind == OPERAND_IMMEDIATE &&
        curr->operands[1].value.imm == 0) {
      
      // If destination and first source are the same, eliminate completely
      if (curr->dest.kind == OPERAND_REGISTER &&
          curr->operands[0].kind == OPERAND_REGISTER &&
          curr->dest.value.reg == curr->operands[0].value.reg) {
        
        // Replace with NOP
        curr->opcode = 0xF1;  // NOP opcode
        curr->operand_count = 0;
      }
    }
  }
  
  return 0;
}

/**
* @brief Apply register allocation optimization
* 
* @param optimizer The optimizer
* @param function The function to optimize
* @return 0 on success, -1 on error
*/
static int optimize_register_allocation(
  optimizer_t* optimizer,
  function_t* function
) {
  // This is a simplified placeholder for register allocation
  // A real implementation would use more sophisticated algorithms
  // like linear scan or graph coloring
  
  // For now, just ensure we don't exceed available registers
  const target_resources_t* resources = coil_get_target_resources(optimizer->target);
  if (resources == NULL) {
    report_error(optimizer, ERROR_ASSEMBLER_NO_TARGET,
                "No target resources available");
    return -1;
  }
  
  if (function->register_count > resources->registers) {
    // We need to spill registers to memory
    // This is a complex operation that's beyond the scope of this example
    report_error(optimizer, ERROR_ASSEMBLER_BAD_MAPPING,
                "Register spilling not implemented");
    return -1;
  }
  
  return 0;
}

instruction_t* coil_optimize_instruction(optimizer_t* optimizer, instruction_t* instr) {
  if (optimizer == NULL || instr == NULL) {
    return NULL;
  }
  
  optimizer->had_error = false;
  
  // Skip optimization if level is NONE
  if (optimizer->level == OPT_LEVEL_NONE) {
    return instr;
  }
  
  // Simple instruction-level optimizations
  
  // Optimize: LOAD_I32 dest, 0 -> LOAD_I32 dest, 0 with XOR
  if (instr->opcode == OPCODE_LOAD_I32 &&
      instr->operand_count == 1 &&
      instr->operands[0].kind == OPERAND_IMMEDIATE &&
      instr->operands[0].value.imm == 0) {
    
    // For x86-64, XOR reg, reg is more efficient than MOV reg, 0
    // In a real implementation, we'd replace the instruction
    // For this example, we'll just leave it as is
  }
  
  return instr;
}

int coil_optimize_basic_block(optimizer_t* optimizer, basic_block_t* block) {
  if (optimizer == NULL || block == NULL) {
    return -1;
  }
  
  optimizer->had_error = false;
  
  // Skip optimization if level is NONE
  if (optimizer->level == OPT_LEVEL_NONE) {
    return 0;
  }
  
  // Apply peephole optimization
  if (peephole_optimize(optimizer, block->instructions, block->instr_count) != 0) {
    return -1;
  }
  
  // Optimize individual instructions
  for (uint32_t i = 0; i < block->instr_count; i++) {
    instruction_t* optimized = coil_optimize_instruction(optimizer, block->instructions[i]);
    
    if (optimized == NULL) {
      return -1;
    }
    
    // Replace with optimized instruction (might be the same)
    if (optimized != block->instructions[i]) {
      coil_free_instruction(block->instructions[i]);
      block->instructions[i] = optimized;
    }
  }
  
  return 0;
}

int coil_optimize_function(optimizer_t* optimizer, function_t* function) {
  if (optimizer == NULL || function == NULL) {
    return -1;
  }
  
  optimizer->had_error = false;
  
  // Skip optimization if level is NONE
  if (optimizer->level == OPT_LEVEL_NONE) {
    return 0;
  }
  
  // Apply optimization to each basic block
  for (uint32_t i = 0; i < function->block_count; i++) {
    if (coil_optimize_basic_block(optimizer, function->blocks[i]) != 0) {
      return -1;
    }
  }
  
  // Apply function-level optimizations
  
  // Register allocation (when level >= NORMAL)
  if (optimizer->level >= OPT_LEVEL_NORMAL) {
    if (optimize_register_allocation(optimizer, function) != 0) {
      return -1;
    }
  }
  
  // More aggressive optimizations
  if (optimizer->level >= OPT_LEVEL_AGGRESSIVE) {
    // Advanced optimizations like loop unrolling, function inlining, etc.
    // would go here
    
    // For experimental optimizations
    if (optimizer->experimental) {
      // Experimental optimizations would go here
    }
  }
  
  return 0;
}

int coil_optimize_module(optimizer_t* optimizer, module_t* module) {
  if (optimizer == NULL || module == NULL) {
    return -1;
  }
  
  optimizer->had_error = false;
  
  // Skip optimization if level is NONE
  if (optimizer->level == OPT_LEVEL_NONE) {
    return 0;
  }
  
  // Apply optimization to each function
  for (uint32_t i = 0; i < module->function_count; i++) {
    // Skip external functions
    if (module->functions[i]->is_external) {
      continue;
    }
    
    if (coil_optimize_function(optimizer, module->functions[i]) != 0) {
      return -1;
    }
  }
  
  return 0;
}