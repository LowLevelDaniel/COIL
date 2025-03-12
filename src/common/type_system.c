/**
* @file type_system.c
* @brief Type system implementation for COIL/HOIL
*/

#include "common/type_system.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
* @brief Extended type information for complex types
*/
typedef struct {
  coil_type_t     type_code;    /**< Encoded type */
  union {
    struct_type_t   struct_info;  /**< Structure information */
    function_type_t func_info;    /**< Function information */
  } data;
} extended_type_info_t;

/**
* @brief Type registry structure
*/
struct type_registry {
  extended_type_info_t* types;      /**< Array of extended type information */
  uint32_t              capacity;   /**< Current capacity */
  uint32_t              count;      /**< Number of entries */
};

type_registry_t* coil_create_type_registry(void) {
  type_registry_t* registry = (type_registry_t*)malloc(sizeof(type_registry_t));
  if (registry == NULL) {
    return NULL;
  }
  
  registry->capacity = 16;
  registry->count = 0;
  registry->types = (extended_type_info_t*)malloc(
    registry->capacity * sizeof(extended_type_info_t)
  );
  
  if (registry->types == NULL) {
    free(registry);
    return NULL;
  }
  
  return registry;
}

void coil_free_type_registry(type_registry_t* registry) {
  if (registry == NULL) {
    return;
  }
  
  // Free all stored complex types
  for (uint32_t i = 0; i < registry->count; i++) {
    extended_type_info_t* info = &registry->types[i];
    
    if (coil_get_type_category(info->type_code) == TYPE_CATEGORY_STRUCT) {
      struct_type_t* struct_info = &info->data.struct_info;
      free(struct_info->elements);
    } else if (coil_get_type_category(info->type_code) == TYPE_CATEGORY_FUNCTION) {
      function_type_t* func_info = &info->data.func_info;
      free(func_info->params);
    }
  }
  
  free(registry->types);
  free(registry);
}

coil_type_t coil_create_type(
  type_category_t category,
  uint8_t width,
  uint8_t qualifiers,
  uint16_t attributes
) {
  return (((uint32_t)category & 0xF) << 28) |
        (((uint32_t)width & 0xFF) << 20) |
        (((uint32_t)qualifiers & 0xFF) << 12) |
        (attributes & 0xFFF);
}

coil_type_t coil_create_pointer_type(
  coil_type_t base_type,
  memory_space_t memory_space,
  uint8_t qualifiers
) {
  // We store the base type in the attributes field (12 bits)
  // and memory space in the lower bits of width
  uint16_t attributes = (uint16_t)(base_type & 0xFFF);
  uint8_t width = 0x40 | ((uint8_t)memory_space & 0x7);  // 64-bit pointer by default
  
  return coil_create_type(
    TYPE_CATEGORY_POINTER,
    width,
    qualifiers,
    attributes
  );
}

coil_type_t coil_create_vector_type(
  coil_type_t element_type,
  uint8_t element_count
) {
  // Store element type in attributes field
  uint16_t attributes = (uint16_t)(element_type & 0xFFF);
  
  return coil_create_type(
    TYPE_CATEGORY_VECTOR,
    element_count,
    0,  // No qualifiers for vectors
    attributes
  );
}

static int type_registry_add(
  type_registry_t* registry,
  extended_type_info_t* info
) {
  if (registry->count >= registry->capacity) {
    uint32_t new_capacity = registry->capacity * 2;
    extended_type_info_t* new_types = (extended_type_info_t*)realloc(
      registry->types,
      new_capacity * sizeof(extended_type_info_t)
    );
    
    if (new_types == NULL) {
      return -1;  // Out of memory
    }
    
    registry->types = new_types;
    registry->capacity = new_capacity;
  }
  
  registry->types[registry->count] = *info;
  return registry->count++;
}

static extended_type_info_t* find_type_info(
  type_registry_t* registry,
  coil_type_t type
) {
  for (uint32_t i = 0; i < registry->count; i++) {
    if (registry->types[i].type_code == type) {
      return &registry->types[i];
    }
  }
  return NULL;
}

coil_type_t coil_create_array_type(
  type_registry_t* registry,
  coil_type_t element_type,
  uint32_t element_count
) {
  // For now, we'll just encode small arrays directly
  // More complex arrays would need extended type info
  
  // Store element type and limited count in attributes
  uint16_t attributes = (uint16_t)(element_type & 0xFFF);
  
  // For now, limit array size or use 0 for unsized
  uint8_t width = (element_count > 255) ? 0 : (uint8_t)element_count;
  
  return coil_create_type(
    TYPE_CATEGORY_ARRAY,
    width,
    0,  // No qualifiers for arrays
    attributes
  );
}

coil_type_t coil_create_struct_type(
  type_registry_t* registry,
  const char* name,
  struct_element_t* elements,
  uint32_t element_count
) {
  // Create a unique ID for this structure
  uint16_t struct_id = (uint16_t)registry->count;
  
  // Create the basic type
  coil_type_t type = coil_create_type(
    TYPE_CATEGORY_STRUCT,
    0,  // Width field not used for structs
    0,  // No qualifiers for structs
    struct_id & 0xFFF  // Use lower 12 bits of struct_id
  );
  
  // Set up extended type info
  extended_type_info_t info;
  info.type_code = type;
  
  // Initialize struct info
  struct_type_t* struct_info = &info.data.struct_info;
  struct_info->name = name ? strdup(name) : NULL;
  struct_info->element_count = element_count;
  
  // Copy elements
  struct_info->elements = (struct_element_t*)malloc(
    element_count * sizeof(struct_element_t)
  );
  if (struct_info->elements == NULL) {
    if (struct_info->name) free((void*)struct_info->name);
    return COIL_TYPE_VOID;  // Error
  }
  
  // Calculate offsets and size
  uint32_t offset = 0;
  uint32_t max_alignment = 1;
  
  for (uint32_t i = 0; i < element_count; i++) {
    struct_info->elements[i].type = elements[i].type;
    struct_info->elements[i].name = elements[i].name ? strdup(elements[i].name) : NULL;
    
    // Calculate alignment for this field
    uint32_t field_alignment = coil_get_type_alignment(registry, elements[i].type);
    max_alignment = (field_alignment > max_alignment) ? field_alignment : max_alignment;
    
    // Align the offset
    offset = (offset + field_alignment - 1) & ~(field_alignment - 1);
    struct_info->elements[i].offset = offset;
    
    // Add field size to offset
    offset += coil_get_type_size(registry, elements[i].type);
  }
  
  // Align final size to struct alignment
  struct_info->size = (offset + max_alignment - 1) & ~(max_alignment - 1);
  struct_info->alignment = max_alignment;
  
  // Add to registry
  if (type_registry_add(registry, &info) < 0) {
    // Failed to add, clean up
    if (struct_info->name) free((void*)struct_info->name);
    for (uint32_t i = 0; i < element_count; i++) {
      if (struct_info->elements[i].name) free((void*)struct_info->elements[i].name);
    }
    free(struct_info->elements);
    return COIL_TYPE_VOID;  // Error
  }
  
  return type;
}

coil_type_t coil_create_function_type(
  type_registry_t* registry,
  coil_type_t return_type,
  function_param_t* params,
  uint32_t param_count,
  bool is_variadic
) {
  // Create a unique ID for this function type
  uint16_t func_id = (uint16_t)registry->count;
  
  // Create the basic type
  coil_type_t type = coil_create_type(
    TYPE_CATEGORY_FUNCTION,
    0,  // Width field not used for functions
    0,  // No qualifiers for function types
    func_id & 0xFFF  // Use lower 12 bits of func_id
  );
  
  // Set up extended type info
  extended_type_info_t info;
  info.type_code = type;
  
  // Initialize function info
  function_type_t* func_info = &info.data.func_info;
  func_info->return_type = return_type;
  func_info->param_count = param_count;
  func_info->is_variadic = is_variadic;
  
  // Copy parameters
  func_info->params = (function_param_t*)malloc(
    param_count * sizeof(function_param_t)
  );
  if (func_info->params == NULL) {
    return COIL_TYPE_VOID;  // Error
  }
  
  for (uint32_t i = 0; i < param_count; i++) {
    func_info->params[i].type = params[i].type;
    func_info->params[i].name = params[i].name ? strdup(params[i].name) : NULL;
  }
  
  // Add to registry
  if (type_registry_add(registry, &info) < 0) {
    // Failed to add, clean up
    for (uint32_t i = 0; i < param_count; i++) {
      if (func_info->params[i].name) free((void*)func_info->params[i].name);
    }
    free(func_info->params);
    return COIL_TYPE_VOID;  // Error
  }
  
  return type;
}

type_category_t coil_get_type_category(coil_type_t type) {
  return (type_category_t)((type >> 28) & 0xF);
}

uint8_t coil_get_type_width(coil_type_t type) {
  return (uint8_t)((type >> 20) & 0xFF);
}

uint8_t coil_get_type_qualifiers(coil_type_t type) {
  return (uint8_t)((type >> 12) & 0xFF);
}

uint16_t coil_get_type_attributes(coil_type_t type) {
  return (uint16_t)(type & 0xFFF);
}

uint32_t coil_get_type_size(type_registry_t* registry, coil_type_t type) {
  type_category_t category = coil_get_type_category(type);
  uint8_t width = coil_get_type_width(type);
  
  switch (category) {
    case TYPE_CATEGORY_VOID:
      return 0;
      
    case TYPE_CATEGORY_BOOLEAN:
      return 1;  // Boolean takes 1 byte in memory
      
    case TYPE_CATEGORY_INTEGER:
    case TYPE_CATEGORY_FLOAT:
      return width / 8;  // Width is in bits
      
    case TYPE_CATEGORY_POINTER:
      return 8;  // 64-bit pointers by default
      
    case TYPE_CATEGORY_VECTOR: {
      uint16_t elem_type_attr = coil_get_type_attributes(type);
      coil_type_t elem_type = (coil_type_t)elem_type_attr;
      return width * coil_get_type_size(registry, elem_type);
    }
      
    case TYPE_CATEGORY_ARRAY: {
      uint16_t elem_type_attr = coil_get_type_attributes(type);
      coil_type_t elem_type = (coil_type_t)elem_type_attr;
      return width * coil_get_type_size(registry, elem_type);
    }
      
    case TYPE_CATEGORY_STRUCT: {
      uint16_t struct_id = coil_get_type_attributes(type);
      const struct_type_t* struct_info = coil_get_struct_info(registry, type);
      if (struct_info) {
        return struct_info->size;
      }
      return 0;
    }
      
    case TYPE_CATEGORY_FUNCTION:
      return 0;  // Functions don't have a size
      
    default:
      return 0;
  }
}

uint32_t coil_get_type_alignment(type_registry_t* registry, coil_type_t type) {
  type_category_t category = coil_get_type_category(type);
  uint8_t width = coil_get_type_width(type);
  
  switch (category) {
    case TYPE_CATEGORY_VOID:
      return 1;
      
    case TYPE_CATEGORY_BOOLEAN:
      return 1;
      
    case TYPE_CATEGORY_INTEGER:
    case TYPE_CATEGORY_FLOAT:
      return width / 8;  // Align to size
      
    case TYPE_CATEGORY_POINTER:
      return 8;  // 64-bit alignment
      
    case TYPE_CATEGORY_VECTOR: {
      uint16_t elem_type_attr = coil_get_type_attributes(type);
      coil_type_t elem_type = (coil_type_t)elem_type_attr;
      return coil_get_type_alignment(registry, elem_type);
    }
      
    case TYPE_CATEGORY_ARRAY: {
      uint16_t elem_type_attr = coil_get_type_attributes(type);
      coil_type_t elem_type = (coil_type_t)elem_type_attr;
      return coil_get_type_alignment(registry, elem_type);
    }
      
    case TYPE_CATEGORY_STRUCT: {
      const struct_type_t* struct_info = coil_get_struct_info(registry, type);
      if (struct_info) {
        return struct_info->alignment;
      }
      return 1;
    }
      
    case TYPE_CATEGORY_FUNCTION:
      return 1;  // Functions don't have alignment
      
    default:
      return 1;
  }
}

bool coil_types_compatible(
  type_registry_t* registry,
  coil_type_t type1,
  coil_type_t type2
) {
  // Exact match is always compatible
  if (type1 == type2) {
    return true;
  }
  
  type_category_t cat1 = coil_get_type_category(type1);
  type_category_t cat2 = coil_get_type_category(type2);
  
  // Different categories are generally incompatible
  if (cat1 != cat2) {
    // Special case: integers and pointers
    if ((cat1 == TYPE_CATEGORY_INTEGER && cat2 == TYPE_CATEGORY_POINTER) ||
        (cat1 == TYPE_CATEGORY_POINTER && cat2 == TYPE_CATEGORY_INTEGER)) {
      return true;  // Allow integer/pointer conversion
    }
    return false;
  }
  
  // Category-specific compatibility checks
  switch (cat1) {
    case TYPE_CATEGORY_INTEGER: {
      uint8_t width1 = coil_get_type_width(type1);
      uint8_t width2 = coil_get_type_width(type2);
      
      // Allow conversion between integer types
      return true;
    }
      
    case TYPE_CATEGORY_FLOAT: {
      uint8_t width1 = coil_get_type_width(type1);
      uint8_t width2 = coil_get_type_width(type2);
      
      // Allow conversion between float types
      return true;
    }
      
    case TYPE_CATEGORY_POINTER: {
      // Get base types
      coil_type_t base1 = coil_get_pointer_base_type(type1);
      coil_type_t base2 = coil_get_pointer_base_type(type2);
      
      // Void pointers are compatible with any pointer
      if (base1 == COIL_TYPE_VOID || base2 == COIL_TYPE_VOID) {
        return true;
      }
      
      // Otherwise, base types must be compatible
      return coil_types_compatible(registry, base1, base2);
    }
      
    case TYPE_CATEGORY_VECTOR: {
      uint8_t width1 = coil_get_type_width(type1);
      uint8_t width2 = coil_get_type_width(type2);
      
      // Element count must match
      if (width1 != width2) {
        return false;
      }
      
      // Element types must be compatible
      coil_type_t elem1 = coil_get_element_type(registry, type1);
      coil_type_t elem2 = coil_get_element_type(registry, type2);
      
      return coil_types_compatible(registry, elem1, elem2);
    }
      
    // Other types require exact match
    default:
      return false;
  }
}

coil_type_t coil_get_pointer_base_type(coil_type_t type) {
  if (coil_get_type_category(type) != TYPE_CATEGORY_POINTER) {
    return COIL_TYPE_VOID;  // Error: not a pointer
  }
  
  // Base type is stored in attributes
  uint16_t attributes = coil_get_type_attributes(type);
  return (coil_type_t)attributes;
}

coil_type_t coil_get_element_type(type_registry_t* registry, coil_type_t type) {
  type_category_t category = coil_get_type_category(type);
  
  if (category != TYPE_CATEGORY_VECTOR && category != TYPE_CATEGORY_ARRAY) {
    return COIL_TYPE_VOID;  // Error: not a vector or array
  }
  
  // Element type is stored in attributes
  uint16_t attributes = coil_get_type_attributes(type);
  return (coil_type_t)attributes;
}

const struct_type_t* coil_get_struct_info(
  type_registry_t* registry,
  coil_type_t type
) {
  if (coil_get_type_category(type) != TYPE_CATEGORY_STRUCT) {
    return NULL;  // Not a struct
  }
  
  extended_type_info_t* info = find_type_info(registry, type);
  if (info == NULL) {
    return NULL;  // Not found in registry
  }
  
  return &info->data.struct_info;
}

const function_type_t* coil_get_function_info(
  type_registry_t* registry,
  coil_type_t type
) {
  if (coil_get_type_category(type) != TYPE_CATEGORY_FUNCTION) {
    return NULL;  // Not a function
  }
  
  extended_type_info_t* info = find_type_info(registry, type);
  if (info == NULL) {
    return NULL;  // Not found in registry
  }
  
  return &info->data.func_info;
}