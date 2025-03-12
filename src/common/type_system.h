/**
* @file type_system.h
* @brief Type system implementation for COIL/HOIL
*
* This file defines the type system used by both the HOIL compiler
* and the COIL assembler, following the COIL Binary Specification.
*/

#ifndef COIL_TYPE_SYSTEM_H
#define COIL_TYPE_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

/**
* @brief Type categories as defined in the COIL specification
*/
typedef enum {
  TYPE_CATEGORY_VOID     = 0x00,  /**< Void type */
  TYPE_CATEGORY_BOOLEAN  = 0x01,  /**< Boolean type */
  TYPE_CATEGORY_INTEGER  = 0x02,  /**< Integer types */
  TYPE_CATEGORY_FLOAT    = 0x03,  /**< Floating-point types */
  TYPE_CATEGORY_POINTER  = 0x04,  /**< Pointer types */
  TYPE_CATEGORY_VECTOR   = 0x05,  /**< SIMD vector types */
  TYPE_CATEGORY_ARRAY    = 0x06,  /**< Array types */
  TYPE_CATEGORY_STRUCT   = 0x07,  /**< Structure types */
  TYPE_CATEGORY_FUNCTION = 0x08   /**< Function types */
} type_category_t;

/**
* @brief Type qualifiers as defined in the COIL specification
*/
typedef enum {
  TYPE_QUALIFIER_NONE     = 0x00,  /**< No qualifiers */
  TYPE_QUALIFIER_UNSIGNED = 0x01,  /**< Unsigned integer type */
  TYPE_QUALIFIER_CONST    = 0x02,  /**< Constant value */
  TYPE_QUALIFIER_VOLATILE = 0x04,  /**< Volatile memory access */
  TYPE_QUALIFIER_RESTRICT = 0x08,  /**< Non-aliased pointer */
  TYPE_QUALIFIER_ATOMIC   = 0x10   /**< Atomic access */
} type_qualifier_t;

/**
* @brief Compact 32-bit type encoding
*
* Format: [category:4][width:8][qualifiers:8][attributes:12]
*/
typedef uint32_t coil_type_t;

/**
* @brief Memory spaces for pointers
*/
typedef enum {
  MEMORY_SPACE_GLOBAL   = 0,  /**< Global memory */
  MEMORY_SPACE_LOCAL    = 1,  /**< Local memory */
  MEMORY_SPACE_SHARED   = 2,  /**< Shared memory */
  MEMORY_SPACE_CONSTANT = 3,  /**< Constant memory */
  MEMORY_SPACE_PRIVATE  = 4   /**< Private (function-local) memory */
} memory_space_t;

/**
* @brief Element of a structure type
*/
typedef struct {
  const char*   name;    /**< Field name */
  coil_type_t   type;    /**< Field type */
  uint32_t      offset;  /**< Offset in bytes */
} struct_element_t;

/**
* @brief Structure type definition
*/
typedef struct {
  const char*      name;           /**< Structure name or NULL */
  uint32_t         element_count;  /**< Number of elements */
  struct_element_t* elements;      /**< Array of elements */
  uint32_t         size;           /**< Total size in bytes */
  uint32_t         alignment;      /**< Alignment in bytes */
} struct_type_t;

/**
* @brief Function parameter
*/
typedef struct {
  const char*   name;    /**< Parameter name or NULL */
  coil_type_t   type;    /**< Parameter type */
} function_param_t;

/**
* @brief Function type definition
*/
typedef struct {
  coil_type_t      return_type;    /**< Return type */
  uint32_t         param_count;    /**< Number of parameters */
  function_param_t* params;        /**< Array of parameters */
  bool             is_variadic;    /**< True if function takes variable args */
} function_type_t;

/**
* @brief Type registry for managing complex types
*/
typedef struct type_registry type_registry_t;

/**
* @brief Create a new type registry
* 
* @return A new type registry
*/
type_registry_t* coil_create_type_registry(void);

/**
* @brief Free a type registry and all associated resources
* 
* @param registry The type registry to free
*/
void coil_free_type_registry(type_registry_t* registry);

/**
* @brief Create a basic type
* 
* @param category Type category
* @param width Width in bits or elements
* @param qualifiers Type qualifiers
* @param attributes Type-specific attributes
* @return The encoded type
*/
coil_type_t coil_create_type(
  type_category_t category,
  uint8_t width,
  uint8_t qualifiers,
  uint16_t attributes
);

/**
* @brief Create a pointer type
* 
* @param base_type Type pointed to
* @param memory_space Memory space
* @param qualifiers Type qualifiers
* @return The encoded pointer type
*/
coil_type_t coil_create_pointer_type(
  coil_type_t base_type,
  memory_space_t memory_space,
  uint8_t qualifiers
);

/**
* @brief Create a vector type
* 
* @param element_type Element type
* @param element_count Number of elements
* @return The encoded vector type
*/
coil_type_t coil_create_vector_type(
  coil_type_t element_type,
  uint8_t element_count
);

/**
* @brief Create an array type
* 
* @param registry Type registry
* @param element_type Element type
* @param element_count Number of elements (0 for unsized array)
* @return The encoded array type
*/
coil_type_t coil_create_array_type(
  type_registry_t* registry,
  coil_type_t element_type,
  uint32_t element_count
);

/**
* @brief Create a structure type
* 
* @param registry Type registry
* @param name Structure name or NULL
* @param elements Array of structure elements
* @param element_count Number of elements
* @return The encoded structure type
*/
coil_type_t coil_create_struct_type(
  type_registry_t* registry,
  const char* name,
  struct_element_t* elements,
  uint32_t element_count
);

/**
* @brief Create a function type
* 
* @param registry Type registry
* @param return_type Return type
* @param params Array of parameters
* @param param_count Number of parameters
* @param is_variadic True if function takes variable args
* @return The encoded function type
*/
coil_type_t coil_create_function_type(
  type_registry_t* registry,
  coil_type_t return_type,
  function_param_t* params,
  uint32_t param_count,
  bool is_variadic
);

/**
* @brief Get the category of a type
* 
* @param type The type to query
* @return The type category
*/
type_category_t coil_get_type_category(coil_type_t type);

/**
* @brief Get the width of a type
* 
* @param type The type to query
* @return The type width in bits or elements
*/
uint8_t coil_get_type_width(coil_type_t type);

/**
* @brief Get the qualifiers of a type
* 
* @param type The type to query
* @return The type qualifiers
*/
uint8_t coil_get_type_qualifiers(coil_type_t type);

/**
* @brief Get the attributes of a type
* 
* @param type The type to query
* @return The type attributes
*/
uint16_t coil_get_type_attributes(coil_type_t type);

/**
* @brief Get the size of a type in bytes
* 
* @param registry Type registry
* @param type The type to query
* @return The size in bytes
*/
uint32_t coil_get_type_size(type_registry_t* registry, coil_type_t type);

/**
* @brief Get the alignment of a type in bytes
* 
* @param registry Type registry
* @param type The type to query
* @return The alignment in bytes
*/
uint32_t coil_get_type_alignment(type_registry_t* registry, coil_type_t type);

/**
* @brief Check if two types are compatible
* 
* @param registry Type registry
* @param type1 First type
* @param type2 Second type
* @return true if the types are compatible, false otherwise
*/
bool coil_types_compatible(
  type_registry_t* registry,
  coil_type_t type1,
  coil_type_t type2
);

/**
* @brief Get the pointed-to type for a pointer
* 
* @param type Pointer type
* @return The pointed-to type
*/
coil_type_t coil_get_pointer_base_type(coil_type_t type);

/**
* @brief Get the element type for a vector or array
* 
* @param registry Type registry
* @param type Vector or array type
* @return The element type
*/
coil_type_t coil_get_element_type(type_registry_t* registry, coil_type_t type);

/**
* @brief Get structure information
* 
* @param registry Type registry
* @param type Structure type
* @return Pointer to structure information or NULL if not a structure
*/
const struct_type_t* coil_get_struct_info(
  type_registry_t* registry,
  coil_type_t type
);

/**
* @brief Get function type information
* 
* @param registry Type registry
* @param type Function type
* @return Pointer to function type information or NULL if not a function
*/
const function_type_t* coil_get_function_info(
  type_registry_t* registry,
  coil_type_t type
);

/**
* @brief Predefined type codes
*/
#define COIL_TYPE_VOID     ((coil_type_t)0x00000000)  /**< Void type */
#define COIL_TYPE_BOOL     ((coil_type_t)0x01000001)  /**< Boolean (1 bit) */
#define COIL_TYPE_INT8     ((coil_type_t)0x02080000)  /**< 8-bit signed integer */
#define COIL_TYPE_UINT8    ((coil_type_t)0x02080100)  /**< 8-bit unsigned integer */
#define COIL_TYPE_INT16    ((coil_type_t)0x02100000)  /**< 16-bit signed integer */
#define COIL_TYPE_UINT16   ((coil_type_t)0x02100100)  /**< 16-bit unsigned integer */
#define COIL_TYPE_INT32    ((coil_type_t)0x02200000)  /**< 32-bit signed integer */
#define COIL_TYPE_UINT32   ((coil_type_t)0x02200100)  /**< 32-bit unsigned integer */
#define COIL_TYPE_INT64    ((coil_type_t)0x02400000)  /**< 64-bit signed integer */
#define COIL_TYPE_UINT64   ((coil_type_t)0x02400100)  /**< 64-bit unsigned integer */
#define COIL_TYPE_FLOAT16  ((coil_type_t)0x03100000)  /**< 16-bit floating-point */
#define COIL_TYPE_FLOAT32  ((coil_type_t)0x03200000)  /**< 32-bit floating-point */
#define COIL_TYPE_FLOAT64  ((coil_type_t)0x03400000)  /**< 64-bit floating-point */
#define COIL_TYPE_PTR      ((coil_type_t)0x04400000)  /**< Generic pointer */

#endif /* COIL_TYPE_SYSTEM_H */