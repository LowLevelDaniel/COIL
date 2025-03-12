/**
* @file module.c
* @brief Module structure implementation for COIL/HOIL
*/

#include "common/module.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

module_t* coil_create_module(const char* name) {
  module_t* module = (module_t*)malloc(sizeof(module_t));
  if (module == NULL) {
    return NULL;
  }
  
  // Initialize module
  module->name = name ? strdup(name) : NULL;
  module->type_registry = coil_create_type_registry();
  
  if (module->type_registry == NULL) {
    if (module->name) free((void*)module->name);
    free(module);
    return NULL;
  }
  
  // Initialize globals array
  module->global_capacity = 16;
  module->global_count = 0;
  module->globals = (global_variable_t**)malloc(
    module->global_capacity * sizeof(global_variable_t*)
  );
  
  if (module->globals == NULL) {
    coil_free_type_registry(module->type_registry);
    if (module->name) free((void*)module->name);
    free(module);
    return NULL;
  }
  
  // Initialize functions array
  module->function_capacity = 16;
  module->function_count = 0;
  module->functions = (function_t**)malloc(
    module->function_capacity * sizeof(function_t*)
  );
  
  if (module->functions == NULL) {
    free(module->globals);
    coil_free_type_registry(module->type_registry);
    if (module->name) free((void*)module->name);
    free(module);
    return NULL;
  }
  
  // Initialize relocations array
  module->relocation_capacity = 16;
  module->relocation_count = 0;
  module->relocations = (relocation_t**)malloc(
    module->relocation_capacity * sizeof(relocation_t*)
  );
  
  if (module->relocations == NULL) {
    free(module->functions);
    free(module->globals);
    coil_free_type_registry(module->type_registry);
    if (module->name) free((void*)module->name);
    free(module);
    return NULL;
  }
  
  // Initialize target requirements
  memset(&module->target, 0, sizeof(target_requirements_t));
  
  return module;
}

void coil_free_module(module_t* module) {
  if (module == NULL) {
    return;
  }
  
  // Free module name
  if (module->name) {
    free((void*)module->name);
  }
  
  // Free type registry
  coil_free_type_registry(module->type_registry);
  
  // Free global variables
  for (uint32_t i = 0; i < module->global_count; i++) {
    global_variable_t* global = module->globals[i];
    if (global->name) free((void*)global->name);
    free(global);
  }
  free(module->globals);
  
  // Free functions
  for (uint32_t i = 0; i < module->function_count; i++) {
    function_t* func = module->functions[i];
    
    // Free function name
    if (func->name) free((void*)func->name);
    
    // Free parameters
    for (uint32_t j = 0; j < func->param_count; j++) {
      if (func->parameters[j].name) {
        free((void*)func->parameters[j].name);
      }
    }
    free(func->parameters);
    
    // Free basic blocks
    for (uint32_t j = 0; j < func->block_count; j++) {
      basic_block_t* block = func->blocks[j];
      
      // Free block name
      if (block->name) free((void*)block->name);
      
      // Free instructions
      for (uint32_t k = 0; k < block->instr_count; k++) {
        coil_free_instruction(block->instructions[k]);
      }
      free(block->instructions);
      
      free(block);
    }
    free(func->blocks);
    
    free(func);
  }
  free(module->functions);
  
  // Free target requirements
  for (uint32_t i = 0; i < module->target.required_count; i++) {
    free((void*)module->target.required_features[i]);
  }
  free(module->target.required_features);
  
  for (uint32_t i = 0; i < module->target.preferred_count; i++) {
    free((void*)module->target.preferred_features[i]);
  }
  free(module->target.preferred_features);
  
  if (module->target.device_class) {
    free((void*)module->target.device_class);
  }
  
  // Free relocations
  for (uint32_t i = 0; i < module->relocation_count; i++) {
    relocation_t* reloc = module->relocations[i];
    if (reloc->symbol) free((void*)reloc->symbol);
    free(reloc);
  }
  free(module->relocations);
  
  // Free module
  free(module);
}

global_variable_t* coil_add_global(
  module_t* module,
  const char* name,
  coil_type_t type,
  bool is_constant,
  bool is_external,
  const void* initializer,
  uint32_t alignment
) {
  if (module == NULL) {
    return NULL;
  }
  
  // Check if we need to expand the globals array
  if (module->global_count >= module->global_capacity) {
    uint32_t new_capacity = module->global_capacity * 2;
    global_variable_t** new_globals = (global_variable_t**)realloc(
      module->globals,
      new_capacity * sizeof(global_variable_t*)
    );
    
    if (new_globals == NULL) {
      return NULL;
    }
    
    module->globals = new_globals;
    module->global_capacity = new_capacity;
  }
  
  // Create new global variable
  global_variable_t* global = (global_variable_t*)malloc(sizeof(global_variable_t));
  if (global == NULL) {
    return NULL;
  }
  
  global->name = name ? strdup(name) : NULL;
  global->type = type;
  global->is_constant = is_constant;
  global->is_external = is_external;
  global->initializer = initializer;
  global->alignment = alignment;
  
  // Add to module
  module->globals[module->global_count++] = global;
  
  return global;
}

global_variable_t* coil_find_global(module_t* module, const char* name) {
  if (module == NULL || name == NULL) {
    return NULL;
  }
  
  for (uint32_t i = 0; i < module->global_count; i++) {
    if (module->globals[i]->name && strcmp(module->globals[i]->name, name) == 0) {
      return module->globals[i];
    }
  }
  
  return NULL;
}

function_t* coil_create_function(
  module_t* module,
  const char* name,
  coil_type_t return_type,
  function_parameter_t* params,
  uint32_t param_count,
  bool is_variadic,
  bool is_external
) {
  if (module == NULL) {
    return NULL;
  }
  
  // Check if we need to expand the functions array
  if (module->function_count >= module->function_capacity) {
    uint32_t new_capacity = module->function_capacity * 2;
    function_t** new_functions = (function_t**)realloc(
      module->functions,
      new_capacity * sizeof(function_t*)
    );
    
    if (new_functions == NULL) {
      return NULL;
    }
    
    module->functions = new_functions;
    module->function_capacity = new_capacity;
  }
  
  // Create new function
  function_t* func = (function_t*)malloc(sizeof(function_t));
  if (func == NULL) {
    return NULL;
  }
  
  func->name = name ? strdup(name) : NULL;
  func->return_type = return_type;
  func->is_variadic = is_variadic;
  func->is_external = is_external;
  func->block_count = 0;
  func->block_capacity = 8;
  func->register_count = 0;
  
  // Copy parameters
  func->param_count = param_count;
  func->parameters = (function_parameter_t*)malloc(
    param_count * sizeof(function_parameter_t)
  );
  
  if (func->parameters == NULL) {
    if (func->name) free((void*)func->name);
    free(func);
    return NULL;
  }
  
  for (uint32_t i = 0; i < param_count; i++) {
    func->parameters[i].name = params[i].name ? strdup(params[i].name) : NULL;
    func->parameters[i].type = params[i].type;
  }
  
  // Create blocks array
  func->blocks = (basic_block_t**)malloc(func->block_capacity * sizeof(basic_block_t*));
  if (func->blocks == NULL) {
    for (uint32_t i = 0; i < param_count; i++) {
      if (func->parameters[i].name) free((void*)func->parameters[i].name);
    }
    free(func->parameters);
    if (func->name) free((void*)func->name);
    free(func);
    return NULL;
  }
  
  // Add to module
  module->functions[module->function_count++] = func;
  
  return func;
}

function_t* coil_find_function(module_t* module, const char* name) {
  if (module == NULL || name == NULL) {
    return NULL;
  }
  
  for (uint32_t i = 0; i < module->function_count; i++) {
    if (module->functions[i]->name && strcmp(module->functions[i]->name, name) == 0) {
      return module->functions[i];
    }
  }
  
  return NULL;
}

basic_block_t* coil_create_basic_block(function_t* function, const char* name) {
  if (function == NULL) {
    return NULL;
  }
  
  // Check if we need to expand the blocks array
  if (function->block_count >= function->block_capacity) {
    uint32_t new_capacity = function->block_capacity * 2;
    basic_block_t** new_blocks = (basic_block_t**)realloc(
      function->blocks,
      new_capacity * sizeof(basic_block_t*)
    );
    
    if (new_blocks == NULL) {
      return NULL;
    }
    
    function->blocks = new_blocks;
    function->block_capacity = new_capacity;
  }
  
  // Create new basic block
  basic_block_t* block = (basic_block_t*)malloc(sizeof(basic_block_t));
  if (block == NULL) {
    return NULL;
  }
  
  block->name = name ? strdup(name) : NULL;
  block->instr_count = 0;
  block->instr_capacity = 16;
  
  // Create instructions array
  block->instructions = (instruction_t**)malloc(
    block->instr_capacity * sizeof(instruction_t*)
  );
  
  if (block->instructions == NULL) {
    if (block->name) free((void*)block->name);
    free(block);
    return NULL;
  }
  
  // Add to function
  function->blocks[function->block_count++] = block;
  
  return block;
}

basic_block_t* coil_find_basic_block(function_t* function, const char* name) {
  if (function == NULL || name == NULL) {
    return NULL;
  }
  
  for (uint32_t i = 0; i < function->block_count; i++) {
    if (function->blocks[i]->name && strcmp(function->blocks[i]->name, name) == 0) {
      return function->blocks[i];
    }
  }
  
  return NULL;
}

int coil_add_instruction(basic_block_t* block, instruction_t* instr) {
  if (block == NULL || instr == NULL) {
    return -1;
  }
  
  // Check if we need to expand the instructions array
  if (block->instr_count >= block->instr_capacity) {
    uint32_t new_capacity = block->instr_capacity * 2;
    instruction_t** new_instructions = (instruction_t**)realloc(
      block->instructions,
      new_capacity * sizeof(instruction_t*)
    );
    
    if (new_instructions == NULL) {
      return -1;
    }
    
    block->instructions = new_instructions;
    block->instr_capacity = new_capacity;
  }
  
  // Add instruction
  block->instructions[block->instr_count++] = instr;
  
  return 0;
}

int coil_set_target_requirements(
  module_t* module,
  const char** required,
  uint32_t required_count,
  const char** preferred,
  uint32_t preferred_count,
  const char* device_class
) {
  if (module == NULL) {
    return -1;
  }
  
  // Free existing requirements
  for (uint32_t i = 0; i < module->target.required_count; i++) {
    free((void*)module->target.required_features[i]);
  }
  free(module->target.required_features);
  
  for (uint32_t i = 0; i < module->target.preferred_count; i++) {
    free((void*)module->target.preferred_features[i]);
  }
  free(module->target.preferred_features);
  
  if (module->target.device_class) {
    free((void*)module->target.device_class);
  }
  
  // Initialize new requirements
  module->target.required_count = required_count;
  module->target.preferred_count = preferred_count;
  module->target.device_class = device_class ? strdup(device_class) : NULL;
  
  // Copy required features
  if (required_count > 0) {
    module->target.required_features = (const char**)malloc(
      required_count * sizeof(char*)
    );
    
    if (module->target.required_features == NULL) {
      module->target.required_count = 0;
      return -1;
    }
    
    for (uint32_t i = 0; i < required_count; i++) {
      module->target.required_features[i] = strdup(required[i]);
    }
  } else {
    module->target.required_features = NULL;
  }
  
  // Copy preferred features
  if (preferred_count > 0) {
    module->target.preferred_features = (const char**)malloc(
      preferred_count * sizeof(char*)
    );
    
    if (module->target.preferred_features == NULL) {
      for (uint32_t i = 0; i < module->target.required_count; i++) {
        free((void*)module->target.required_features[i]);
      }
      free(module->target.required_features);
      module->target.required_count = 0;
      module->target.preferred_count = 0;
      return -1;
    }
    
    for (uint32_t i = 0; i < preferred_count; i++) {
      module->target.preferred_features[i] = strdup(preferred[i]);
    }
  } else {
    module->target.preferred_features = NULL;
  }
  
  return 0;
}

relocation_t* coil_add_relocation(
  module_t* module,
  const char* symbol,
  uint32_t offset,
  uint32_t type,
  int64_t addend
) {
  if (module == NULL) {
    return NULL;
  }
  
  // Check if we need to expand the relocations array
  if (module->relocation_count >= module->relocation_capacity) {
    uint32_t new_capacity = module->relocation_capacity * 2;
    relocation_t** new_relocations = (relocation_t**)realloc(
      module->relocations,
      new_capacity * sizeof(relocation_t*)
    );
    
    if (new_relocations == NULL) {
      return NULL;
    }
    
    module->relocations = new_relocations;
    module->relocation_capacity = new_capacity;
  }
  
  // Create new relocation
  relocation_t* reloc = (relocation_t*)malloc(sizeof(relocation_t));
  if (reloc == NULL) {
    return NULL;
  }
  
  reloc->symbol = symbol ? strdup(symbol) : NULL;
  reloc->offset = offset;
  reloc->type = type;
  reloc->addend = addend;
  
  // Add to module
  module->relocations[module->relocation_count++] = reloc;
  
  return reloc;
}

// Structure of a COIL binary file:
// [magic:4][version:4][section_count:4][flags:4]
// [section_type:4][offset:4][size:4] (repeated section_count times)
// [section data] (repeated section_count times)

#define COIL_MAGIC 0x434F494C  // "COIL" in ASCII
#define COIL_VERSION 0x00010000  // 1.0.0

// Section types
#define SECTION_TYPE_HEADER    0
#define SECTION_TYPE_TYPE      1
#define SECTION_TYPE_FUNCTION  2
#define SECTION_TYPE_GLOBAL    3
#define SECTION_TYPE_CONSTANT  4
#define SECTION_TYPE_CODE      5
#define SECTION_TYPE_RELOC     6
#define SECTION_TYPE_METADATA  7

int coil_module_to_binary(
  const module_t* module,
  uint8_t* buffer,
  size_t buffer_size
) {
  if (module == NULL || buffer == NULL) {
    return -1;
  }
  
  // We need at least enough space for the header
  if (buffer_size < 16) {
    return -1;
  }
  
  // For now, this is just a basic implementation that puts minimal
  // information in the binary to demonstrate the concept.
  
  // Write header
  *((uint32_t*)&buffer[0]) = COIL_MAGIC;
  *((uint32_t*)&buffer[4]) = COIL_VERSION;
  *((uint32_t*)&buffer[8]) = 1;  // One section for now
  *((uint32_t*)&buffer[12]) = 0;  // No flags
  
  // Write section table
  *((uint32_t*)&buffer[16]) = SECTION_TYPE_METADATA;
  *((uint32_t*)&buffer[20]) = 28;  // Offset of section data
  *((uint32_t*)&buffer[24]) = module->name ? strlen(module->name) + 1 : 1;
  
  // Write section data (just the module name for now)
  if (module->name) {
    if (buffer_size < 28 + strlen(module->name) + 1) {
      return -1;
    }
    strcpy((char*)&buffer[28], module->name);
    return 28 + strlen(module->name) + 1;
  } else {
    if (buffer_size < 29) {
      return -1;
    }
    buffer[28] = 0;  // Empty name
    return 29;
  }
}

module_t* coil_module_from_binary(const uint8_t* buffer, size_t buffer_size) {
  if (buffer == NULL || buffer_size < 16) {
    return NULL;
  }
  
  // Check magic number
  uint32_t magic = *((uint32_t*)&buffer[0]);
  if (magic != COIL_MAGIC) {
    return NULL;
  }
  
  // Check version
  uint32_t version = *((uint32_t*)&buffer[4]);
  if ((version >> 16) != (COIL_VERSION >> 16)) {
    // Major version mismatch
    return NULL;
  }
  
  // Read header
  uint32_t section_count = *((uint32_t*)&buffer[8]);
  // uint32_t flags = *((uint32_t*)&buffer[12]);
  
  // For now, we only look for the metadata section to get the module name
  const char* module_name = NULL;
  
  for (uint32_t i = 0; i < section_count; i++) {
    uint32_t section_type = *((uint32_t*)&buffer[16 + i * 12]);
    uint32_t section_offset = *((uint32_t*)&buffer[20 + i * 12]);
    // uint32_t section_size = *((uint32_t*)&buffer[24 + i * 12]);
    
    if (section_type == SECTION_TYPE_METADATA) {
      module_name = (const char*)&buffer[section_offset];
      break;
    }
  }
  
  // Create module with the name we found (or NULL)
  return coil_create_module(module_name);
}