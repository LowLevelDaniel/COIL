/**
* @file module.h
* @brief Module structure definitions for COIL/HOIL
*
* This file defines the module structure used by both the HOIL compiler
* and the COIL assembler to represent a compilation unit.
*/

#ifndef COIL_MODULE_H
#define COIL_MODULE_H

#include "common/type_system.h"
#include "common/instruction.h"
#include <stdint.h>
#include <stdbool.h>

/**
* @brief Global variable definition
*/
typedef struct {
  const char*   name;         /**< Variable name */
  coil_type_t   type;         /**< Variable type */
  bool          is_constant;  /**< True if constant (immutable) */
  bool          is_external;  /**< True if defined in another module */
  const void*   initializer;  /**< Initial value or NULL */
  uint32_t      alignment;    /**< Alignment in bytes */
} global_variable_t;

/**
* @brief Function parameter definition
*/
typedef struct {
  const char*   name;         /**< Parameter name */
  coil_type_t   type;         /**< Parameter type */
} function_parameter_t;

/**
* @brief Basic block structure
*/
typedef struct {
  const char*     name;           /**< Block name */
  instruction_t** instructions;   /**< Array of instructions */
  uint32_t        instr_count;    /**< Number of instructions */
  uint32_t        instr_capacity; /**< Capacity of instructions array */
} basic_block_t;

/**
* @brief Function definition
*/
typedef struct {
  const char*            name;           /**< Function name */
  coil_type_t            return_type;    /**< Return type */
  function_parameter_t*  parameters;     /**< Array of parameters */
  uint32_t               param_count;    /**< Number of parameters */
  bool                   is_variadic;    /**< True if function is variadic */
  bool                   is_external;    /**< True if defined in another module */
  basic_block_t**        blocks;         /**< Array of basic blocks */
  uint32_t               block_count;    /**< Number of basic blocks */
  uint32_t               block_capacity; /**< Capacity of blocks array */
  uint32_t               register_count; /**< Number of virtual registers used */
} function_t;

/**
* @brief Target requirements
*/
typedef struct {
  const char**  required_features;   /**< Array of required features */
  uint32_t      required_count;      /**< Number of required features */
  const char**  preferred_features;  /**< Array of preferred features */
  uint32_t      preferred_count;     /**< Number of preferred features */
  const char*   device_class;        /**< Preferred device class or NULL */
} target_requirements_t;

/**
* @brief Relocation entry
*/
typedef struct {
  const char*   symbol;      /**< Symbol name */
  uint32_t      offset;      /**< Offset in code section */
  uint32_t      type;        /**< Relocation type */
  int64_t       addend;      /**< Addend value */
} relocation_t;

/**
* @brief Module structure
*/
typedef struct {
  const char*           name;               /**< Module name */
  type_registry_t*      type_registry;      /**< Type registry */
  global_variable_t**   globals;            /**< Array of global variables */
  uint32_t              global_count;       /**< Number of global variables */
  uint32_t              global_capacity;    /**< Capacity of globals array */
  function_t**          functions;          /**< Array of functions */
  uint32_t              function_count;     /**< Number of functions */
  uint32_t              function_capacity;  /**< Capacity of functions array */
  target_requirements_t target;             /**< Target requirements */
  relocation_t**        relocations;        /**< Array of relocations */
  uint32_t              relocation_count;   /**< Number of relocations */
  uint32_t              relocation_capacity; /**< Capacity of relocations array */
} module_t;

/**
* @brief Create a new module
* 
* @param name Module name
* @return The new module
*/
module_t* coil_create_module(const char* name);

/**
* @brief Free a module and all associated resources
* 
* @param module The module to free
*/
void coil_free_module(module_t* module);

/**
* @brief Add a global variable to a module
* 
* @param module The module
* @param name Variable name
* @param type Variable type
* @param is_constant True if constant (immutable)
* @param is_external True if defined in another module
* @param initializer Initial value or NULL
* @param alignment Alignment in bytes
* @return The new global variable
*/
global_variable_t* coil_add_global(
  module_t* module,
  const char* name,
  coil_type_t type,
  bool is_constant,
  bool is_external,
  const void* initializer,
  uint32_t alignment
);

/**
* @brief Find a global variable by name
* 
* @param module The module
* @param name Global variable name
* @return The global variable or NULL if not found
*/
global_variable_t* coil_find_global(module_t* module, const char* name);

/**
* @brief Create a new function
* 
* @param module The module
* @param name Function name
* @param return_type Return type
* @param params Array of parameters
* @param param_count Number of parameters
* @param is_variadic True if function is variadic
* @param is_external True if defined in another module
* @return The new function
*/
function_t* coil_create_function(
  module_t* module,
  const char* name,
  coil_type_t return_type,
  function_parameter_t* params,
  uint32_t param_count,
  bool is_variadic,
  bool is_external
);

/**
* @brief Find a function by name
* 
* @param module The module
* @param name Function name
* @return The function or NULL if not found
*/
function_t* coil_find_function(module_t* module, const char* name);

/**
* @brief Create a new basic block
* 
* @param function The function
* @param name Block name
* @return The new basic block
*/
basic_block_t* coil_create_basic_block(function_t* function, const char* name);

/**
* @brief Find a basic block by name
* 
* @param function The function
* @param name Block name
* @return The basic block or NULL if not found
*/
basic_block_t* coil_find_basic_block(function_t* function, const char* name);

/**
* @brief Add an instruction to a basic block
* 
* @param block The basic block
* @param instr The instruction to add
* @return 0 on success, -1 on error
*/
int coil_add_instruction(basic_block_t* block, instruction_t* instr);

/**
* @brief Set target requirements
* 
* @param module The module
* @param required Required features
* @param required_count Number of required features
* @param preferred Preferred features
* @param preferred_count Number of preferred features
* @param device_class Preferred device class or NULL
* @return 0 on success, -1 on error
*/
int coil_set_target_requirements(
  module_t* module,
  const char** required,
  uint32_t required_count,
  const char** preferred,
  uint32_t preferred_count,
  const char* device_class
);

/**
* @brief Add a relocation entry
* 
* @param module The module
* @param symbol Symbol name
* @param offset Offset in code section
* @param type Relocation type
* @param addend Addend value
* @return The new relocation entry
*/
relocation_t* coil_add_relocation(
  module_t* module,
  const char* symbol,
  uint32_t offset,
  uint32_t type,
  int64_t addend
);

/**
* @brief Convert module to binary representation
* 
* @param module The module
* @param buffer Buffer to write to
* @param buffer_size Size of the buffer
* @return Number of bytes written, or -1 on error
*/
int coil_module_to_binary(
  const module_t* module,
  uint8_t* buffer,
  size_t buffer_size
);

/**
* @brief Parse module from binary representation
* 
* @param buffer Binary buffer
* @param buffer_size Size of the buffer
* @return Parsed module, or NULL on error
*/
module_t* coil_module_from_binary(const uint8_t* buffer, size_t buffer_size);

#endif /* COIL_MODULE_H */