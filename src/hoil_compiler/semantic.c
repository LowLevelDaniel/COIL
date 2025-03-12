/**
 * @file semantic.c
 * @brief Semantic analyzer implementation for the HOIL compiler
 */

#include "hoil_compiler/semantic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Symbol kind
 */
typedef enum {
  SYMBOL_GLOBAL,     /**< Global variable */
  SYMBOL_FUNCTION,   /**< Function */
  SYMBOL_PARAMETER,  /**< Function parameter */
  SYMBOL_LOCAL,      /**< Local variable */
  SYMBOL_LABEL       /**< Label */
} symbol_kind_t;

/**
 * @brief Symbol table entry
 */
typedef struct {
  const char*   name;       /**< Symbol name */
  symbol_kind_t kind;       /**< Symbol kind */
  coil_type_t   type;       /**< Symbol type */
  
  union {
    global_variable_t* global;  /**< Global variable reference */
    function_t*       function; /**< Function reference */
    uint32_t          param_idx; /**< Parameter index */
    uint32_t          local_idx; /**< Local variable index */
    basic_block_t*    block;    /**< Basic block reference */
  } ref;
} symbol_entry_t;

/**
 * @brief Symbol table
 */
typedef struct {
  symbol_entry_t* entries;      /**< Symbol entries */
  uint32_t        count;        /**< Number of entries */
  uint32_t        capacity;     /**< Capacity of entries array */
} symbol_table_t;

/**
 * @brief Semantic analyzer structure
 */
struct semantic_analyzer {
  error_context_t* error_context;  /**< Error context */
  symbol_table_t   global_symbols; /**< Global symbol table */
  symbol_table_t   local_symbols;  /**< Local symbol table (for current function) */
  bool             had_error;      /**< True if an error occurred */
};

/**
 * @brief Report a semantic error
 * 
 * @param analyzer The semantic analyzer
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  semantic_analyzer_t* analyzer,
  uint32_t code,
  const char* message
) {
  if (analyzer->error_context != NULL) {
    REPORT_ERROR(analyzer->error_context, ERROR_ERROR, ERROR_CATEGORY_SEMANTIC,
                code, message, NULL, 0, 0);
  }
  
  analyzer->had_error = true;
}

/**
 * @brief Initialize a symbol table
 * 
 * @param table The symbol table
 * @return 0 on success, -1 on error
 */
static int init_symbol_table(symbol_table_t* table) {
  table->capacity = 16;
  table->count = 0;
  
  table->entries = (symbol_entry_t*)malloc(
    table->capacity * sizeof(symbol_entry_t)
  );
  
  if (table->entries == NULL) {
    return -1;
  }
  
  return 0;
}

/**
 * @brief Free a symbol table
 * 
 * @param table The symbol table
 */
static void free_symbol_table(symbol_table_t* table) {
  if (table->entries != NULL) {
    // Free symbol names
    for (uint32_t i = 0; i < table->count; i++) {
      free((void*)table->entries[i].name);
    }
    
    free(table->entries);
    table->entries = NULL;
  }
  
  table->count = 0;
  table->capacity = 0;
}

/**
 * @brief Clear a symbol table
 * 
 * @param table The symbol table
 */
static void clear_symbol_table(symbol_table_t* table) {
  // Free symbol names
  for (uint32_t i = 0; i < table->count; i++) {
    free((void*)table->entries[i].name);
  }
  
  table->count = 0;
}

/**
 * @brief Add a symbol to a symbol table
 * 
 * @param table The symbol table
 * @param name Symbol name
 * @param kind Symbol kind
 * @param type Symbol type
 * @return The symbol entry, or NULL on error
 */
static symbol_entry_t* add_symbol(
  symbol_table_t* table,
  const char* name,
  symbol_kind_t kind,
  coil_type_t type
) {
  // Check for duplicate symbol
  for (uint32_t i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].name, name) == 0) {
      return NULL;  // Duplicate symbol
    }
  }
  
  // Ensure capacity
  if (table->count >= table->capacity) {
    uint32_t new_capacity = table->capacity * 2;
    symbol_entry_t* new_entries = (symbol_entry_t*)realloc(
      table->entries,
      new_capacity * sizeof(symbol_entry_t)
    );
    
    if (new_entries == NULL) {
      return NULL;
    }
    
    table->entries = new_entries;
    table->capacity = new_capacity;
  }
  
  // Add symbol
  symbol_entry_t* entry = &table->entries[table->count++];
  entry->name = strdup(name);
  entry->kind = kind;
  entry->type = type;
  
  return entry;
}

/**
 * @brief Find a symbol in a symbol table
 * 
 * @param table The symbol table
 * @param name Symbol name
 * @return The symbol entry, or NULL if not found
 */
static symbol_entry_t* find_symbol(symbol_table_t* table, const char* name) {
  for (uint32_t i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].name, name) == 0) {
      return &table->entries[i];
    }
  }
  
  return NULL;
}

semantic_analyzer_t* hoil_create_semantic_analyzer(error_context_t* error_context) {
  semantic_analyzer_t* analyzer = (semantic_analyzer_t*)malloc(sizeof(semantic_analyzer_t));
  if (analyzer == NULL) {
    return NULL;
  }
  
  analyzer->error_context = error_context;
  analyzer->had_error = false;
  
  if (init_symbol_table(&analyzer->global_symbols) != 0) {
    free(analyzer);
    return NULL;
  }
  
  if (init_symbol_table(&analyzer->local_symbols) != 0) {
    free_symbol_table(&analyzer->global_symbols);
    free(analyzer);
    return NULL;
  }
  
  return analyzer;
}

void hoil_free_semantic_analyzer(semantic_analyzer_t* analyzer) {
  if (analyzer == NULL) {
    return;
  }
  
  free_symbol_table(&analyzer->global_symbols);
  free_symbol_table(&analyzer->local_symbols);
  
  free(analyzer);
}

int hoil_analyze_type(semantic_analyzer_t* analyzer, module_t* module, coil_type_t type) {
  if (analyzer == NULL || module == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Validate the type based on its category
  type_category_t category = coil_get_type_category(type);
  
  switch (category) {
    case TYPE_CATEGORY_VOID:
    case TYPE_CATEGORY_BOOLEAN:
    case TYPE_CATEGORY_INTEGER:
    case TYPE_CATEGORY_FLOAT:
      // Basic types are always valid
      return 0;
      
    case TYPE_CATEGORY_POINTER: {
      // Check base type
      coil_type_t base_type = coil_get_pointer_base_type(type);
      return hoil_analyze_type(analyzer, module, base_type);
    }
      
    case TYPE_CATEGORY_VECTOR: {
      // Check element type and count
      coil_type_t elem_type = coil_get_element_type(module->type_registry, type);
      uint8_t elem_count = coil_get_type_width(type);
      
      if (elem_count == 0) {
        report_error(analyzer, ERROR_TYPE_INVALID,
                    "Vector must have at least one element");
        return -1;
      }
      
      return hoil_analyze_type(analyzer, module, elem_type);
    }
      
    case TYPE_CATEGORY_ARRAY: {
      // Check element type
      coil_type_t elem_type = coil_get_element_type(module->type_registry, type);
      return hoil_analyze_type(analyzer, module, elem_type);
    }
      
    case TYPE_CATEGORY_STRUCT: {
      // Check struct elements
      const struct_type_t* struct_info = coil_get_struct_info(module->type_registry, type);
      
      if (struct_info == NULL) {
        report_error(analyzer, ERROR_TYPE_UNKNOWN, "Unknown struct type");
        return -1;
      }
      
      for (uint32_t i = 0; i < struct_info->element_count; i++) {
        if (hoil_analyze_type(analyzer, module, struct_info->elements[i].type) != 0) {
          return -1;
        }
      }
      
      return 0;
    }
      
    case TYPE_CATEGORY_FUNCTION: {
      // Check function return type and parameters
      const function_type_t* func_info = coil_get_function_info(module->type_registry, type);
      
      if (func_info == NULL) {
        report_error(analyzer, ERROR_TYPE_UNKNOWN, "Unknown function type");
        return -1;
      }
      
      // Check return type
      if (hoil_analyze_type(analyzer, module, func_info->return_type) != 0) {
        return -1;
      }
      
      // Check parameter types
      for (uint32_t i = 0; i < func_info->param_count; i++) {
        if (hoil_analyze_type(analyzer, module, func_info->params[i].type) != 0) {
          return -1;
        }
      }
      
      return 0;
    }
      
    default:
      report_error(analyzer, ERROR_TYPE_UNKNOWN, "Unknown type category");
      return -1;
  }
}

int hoil_analyze_global(semantic_analyzer_t* analyzer, module_t* module, global_variable_t* global) {
  if (analyzer == NULL || module == NULL || global == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Check if global name is unique
  if (find_symbol(&analyzer->global_symbols, global->name) != NULL) {
    char error_msg[128];
    snprintf(error_msg, sizeof(error_msg),
             "Global variable '%s' is already defined",
             global->name);
    report_error(analyzer, ERROR_SEMANTIC_REDEFINED, error_msg);
    return -1;
  }
  
  // Validate global type
  if (hoil_analyze_type(analyzer, module, global->type) != 0) {
    return -1;
  }
  
  // Add global to symbol table
  symbol_entry_t* entry = add_symbol(
    &analyzer->global_symbols,
    global->name,
    SYMBOL_GLOBAL,
    global->type
  );
  
  if (entry == NULL) {
    report_error(analyzer, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return -1;
  }
  
  entry->ref.global = global;
  
  return 0;
}

int hoil_analyze_instruction(
  semantic_analyzer_t* analyzer,
  module_t* module,
  function_t* function,
  basic_block_t* block,
  instruction_t* instr
) {
  if (analyzer == NULL || module == NULL || function == NULL || 
      block == NULL || instr == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Validate instruction operands
  for (uint8_t i = 0; i < instr->operand_count; i++) {
    instruction_operand_t* op = &instr->operands[i];
    
    switch (op->kind) {
      case OPERAND_REGISTER:
        // Check if register is within range
        if (op->value.reg >= function->register_count) {
          char error_msg[128];
          snprintf(error_msg, sizeof(error_msg),
                   "Register %u is out of range (max %u)",
                   op->value.reg, function->register_count - 1);
          report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND, error_msg);
          return -1;
        }
        break;
        
      case OPERAND_BASIC_BLOCK: {
        // Check if block exists
        basic_block_t* target_block = coil_find_basic_block(function, op->value.block);
        if (target_block == NULL) {
          char error_msg[128];
          snprintf(error_msg, sizeof(error_msg),
                   "Unknown basic block '%s'",
                   op->value.block);
          report_error(analyzer, ERROR_SEMANTIC_UNDEFINED, error_msg);
          return -1;
        }
        
        // Store reference to block
        symbol_entry_t* entry = find_symbol(&analyzer->local_symbols, op->value.block);
        if (entry != NULL) {
          entry->ref.block = target_block;
        }
        break;
      }
        
      case OPERAND_FUNCTION: {
        // Check if function exists
        function_t* target_func = coil_find_function(module, op->value.func);
        if (target_func == NULL) {
          char error_msg[128];
          snprintf(error_msg, sizeof(error_msg),
                   "Unknown function '%s'",
                   op->value.func);
          report_error(analyzer, ERROR_SEMANTIC_UNDEFINED, error_msg);
          return -1;
        }
        
        // Store reference to function
        symbol_entry_t* entry = find_symbol(&analyzer->global_symbols, op->value.func);
        if (entry != NULL) {
          entry->ref.function = target_func;
        }
        break;
      }
        
      case OPERAND_GLOBAL: {
        // Check if global exists
        global_variable_t* global = coil_find_global(module, op->value.global);
        if (global == NULL) {
          char error_msg[128];
          snprintf(error_msg, sizeof(error_msg),
                   "Unknown global variable '%s'",
                   op->value.global);
          report_error(analyzer, ERROR_SEMANTIC_UNDEFINED, error_msg);
          return -1;
        }
        
        // Store reference to global
        symbol_entry_t* entry = find_symbol(&analyzer->global_symbols, op->value.global);
        if (entry != NULL) {
          entry->ref.global = global;
        }
        break;
      }
        
      default:
        // Other operand types don't need validation
        break;
    }
  }
  
  // Validate destination operand
  if (instr->dest.kind == OPERAND_REGISTER) {
    // Check if register is within range
    if (instr->dest.value.reg >= function->register_count) {
      char error_msg[128];
      snprintf(error_msg, sizeof(error_msg),
               "Register %u is out of range (max %u)",
               instr->dest.value.reg, function->register_count - 1);
      report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND, error_msg);
      return -1;
    }
  }
  
  // Validate specific instructions
  switch (instr->opcode) {
    case OPCODE_BR:
      // Branch instruction validation
      if (instr->operand_count != 1 && instr->operand_count != 3) {
        report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND,
                    "Branch instruction must have 1 or 3 operands");
        return -1;
      }
      
      if (instr->operand_count == 3) {
        // Conditional branch
        if (instr->operands[0].kind != OPERAND_REGISTER) {
          report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND,
                      "Branch condition must be a register");
          return -1;
        }
      }
      break;
      
    case OPCODE_CALL:
      // Call instruction validation
      if (instr->operand_count < 1) {
        report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND,
                    "Call instruction must have at least 1 operand");
        return -1;
      }
      
      if (instr->operands[0].kind != OPERAND_FUNCTION) {
        report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND,
                    "First operand of call instruction must be a function");
        return -1;
      }
      break;
      
    case OPCODE_RET:
      // Return instruction validation
      if (instr->operand_count > 1) {
        report_error(analyzer, ERROR_SEMANTIC_INVALID_OPERAND,
                    "Return instruction must have 0 or 1 operand");
        return -1;
      }
      
      if (instr->operand_count == 1) {
        // Check return type compatibility
        // This would require type checking which is beyond the
        // scope of this example
      }
      break;
      
    default:
      // Other instructions don't need special validation
      break;
  }
  
  return 0;
}

int hoil_analyze_basic_block(
  semantic_analyzer_t* analyzer,
  module_t* module,
  function_t* function,
  basic_block_t* block
) {
  if (analyzer == NULL || module == NULL || function == NULL || block == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Add block to symbol table
  symbol_entry_t* entry = add_symbol(
    &analyzer->local_symbols,
    block->name,
    SYMBOL_LABEL,
    COIL_TYPE_VOID
  );
  
  if (entry == NULL) {
    // Check if it's a duplicate
    entry = find_symbol(&analyzer->local_symbols, block->name);
    if (entry != NULL) {
      char error_msg[128];
      snprintf(error_msg, sizeof(error_msg),
               "Duplicate basic block '%s'",
               block->name);
      report_error(analyzer, ERROR_SEMANTIC_REDEFINED, error_msg);
      return -1;
    } else {
      report_error(analyzer, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
      return -1;
    }
  }
  
  entry->ref.block = block;
  
  // Analyze each instruction
  for (uint32_t i = 0; i < block->instr_count; i++) {
    if (hoil_analyze_instruction(analyzer, module, function, block, block->instructions[i]) != 0) {
      return -1;
    }
  }
  
  // Check for terminator instruction
  if (block->instr_count > 0) {
    instruction_t* last_instr = block->instructions[block->instr_count - 1];
    
    bool is_terminator = 
      last_instr->opcode == OPCODE_BR || 
      last_instr->opcode == OPCODE_SWITCH || 
      last_instr->opcode == OPCODE_RET;
    
    if (!is_terminator) {
      char error_msg[128];
      snprintf(error_msg, sizeof(error_msg),
               "Basic block '%s' does not end with a terminator instruction",
               block->name);
      report_error(analyzer, ERROR_SEMANTIC_INVALID_CONTROL, error_msg);
      return -1;
    }
  } else {
    char error_msg[128];
    snprintf(error_msg, sizeof(error_msg),
             "Empty basic block '%s'",
             block->name);
    report_error(analyzer, ERROR_SEMANTIC_INVALID_CONTROL, error_msg);
    return -1;
  }
  
  return 0;
}

int hoil_analyze_function(semantic_analyzer_t* analyzer, module_t* module, function_t* function) {
  if (analyzer == NULL || module == NULL || function == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Check if function name is unique
  if (find_symbol(&analyzer->global_symbols, function->name) != NULL) {
    char error_msg[128];
    snprintf(error_msg, sizeof(error_msg),
             "Function '%s' is already defined",
             function->name);
    report_error(analyzer, ERROR_SEMANTIC_REDEFINED, error_msg);
    return -1;
  }
  
  // Validate return type
  if (hoil_analyze_type(analyzer, module, function->return_type) != 0) {
    return -1;
  }
  
  // Add function to global symbol table
  symbol_entry_t* entry = add_symbol(
    &analyzer->global_symbols,
    function->name,
    SYMBOL_FUNCTION,
    function->return_type
  );
  
  if (entry == NULL) {
    report_error(analyzer, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return -1;
  }
  
  entry->ref.function = function;
  
  // Clear local symbol table for this function
  clear_symbol_table(&analyzer->local_symbols);
  
  // Add parameters to local symbol table
  for (uint32_t i = 0; i < function->param_count; i++) {
    symbol_entry_t* param_entry = add_symbol(
      &analyzer->local_symbols,
      function->parameters[i].name,
      SYMBOL_PARAMETER,
      function->parameters[i].type
    );
    
    if (param_entry == NULL) {
      // Check if it's a duplicate
      param_entry = find_symbol(&analyzer->local_symbols, function->parameters[i].name);
      if (param_entry != NULL) {
        char error_msg[128];
        snprintf(error_msg, sizeof(error_msg),
                 "Duplicate parameter '%s'",
                 function->parameters[i].name);
        report_error(analyzer, ERROR_SEMANTIC_REDEFINED, error_msg);
        return -1;
      } else {
        report_error(analyzer, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
        return -1;
      }
    }
    
    param_entry->ref.param_idx = i;
  }
  
  // Skip analysis for external functions
  if (function->is_external) {
    return 0;
  }
  
  // Check if ENTRY block exists
  basic_block_t* entry_block = NULL;
  for (uint32_t i = 0; i < function->block_count; i++) {
    if (strcmp(function->blocks[i]->name, "ENTRY") == 0) {
      entry_block = function->blocks[i];
      break;
    }
  }
  
  if (entry_block == NULL) {
    char error_msg[128];
    snprintf(error_msg, sizeof(error_msg),
             "Function '%s' has no ENTRY block",
             function->name);
    report_error(analyzer, ERROR_SEMANTIC_INVALID_CONTROL, error_msg);
    return -1;
  }
  
  // Analyze each basic block
  for (uint32_t i = 0; i < function->block_count; i++) {
    if (hoil_analyze_basic_block(analyzer, module, function, function->blocks[i]) != 0) {
      return -1;
    }
  }
  
  return 0;
}

int hoil_analyze_module(semantic_analyzer_t* analyzer, module_t* module) {
  if (analyzer == NULL || module == NULL) {
    return -1;
  }
  
  analyzer->had_error = false;
  
  // Clear symbol tables
  clear_symbol_table(&analyzer->global_symbols);
  clear_symbol_table(&analyzer->local_symbols);
  
  // Analyze global variables
  for (uint32_t i = 0; i < module->global_count; i++) {
    if (hoil_analyze_global(analyzer, module, module->globals[i]) != 0) {
      return -1;
    }
  }
  
  // Analyze functions
  for (uint32_t i = 0; i < module->function_count; i++) {
    if (hoil_analyze_function(analyzer, module, module->functions[i]) != 0) {
      return -1;
    }
  }
  
  return 0;
}