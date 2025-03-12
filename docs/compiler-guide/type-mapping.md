# Mapping Language Types to COIL Types

This document provides guidance for compiler developers on how to map source language types to COIL's type system. Effective type mapping is crucial for generating correct COIL code that accurately represents your language's semantics.

## COIL Type System Overview

COIL's type system is designed to be comprehensive yet efficient, with all types resolved at compile time. The type system includes:

- Primitive types (integers, floats, booleans)
- Composite types (arrays, structures, unions)
- Pointer types with memory space qualifiers
- Vector types for SIMD operations
- Function types with parameter and return specifications

All COIL types are encoded in a compact 32-bit representation with this structure:
```
[category:4][width:8][qualifiers:8][attributes:12]
```

## Basic Type Mapping Principles

When mapping types from your source language to COIL, follow these principles:

1. **Preserve semantics**: Ensure the COIL type accurately represents the behavior of the source type
2. **Maintain precision**: Don't lose precision or range in numeric types
3. **Consider alignment**: Map to COIL types with appropriate alignment requirements
4. **Preserve memory layout**: Maintain the memory layout of composite types
5. **Support target variations**: Consider how types may vary across target architectures

## Common Language Type Mappings

### Integer Types

| Common Language Types | COIL Type | Notes |
|-----------------------|-----------|-------|
| char, int8_t | INT8 | 8-bit signed integer |
| unsigned char, uint8_t | UINT8 | 8-bit unsigned integer |
| short, int16_t | INT16 | 16-bit signed integer |
| unsigned short, uint16_t | UINT16 | 16-bit unsigned integer |
| int, int32_t | INT32 | 32-bit signed integer |
| unsigned int, uint32_t | UINT32 | 32-bit unsigned integer |
| long long, int64_t | INT64 | 64-bit signed integer |
| unsigned long long, uint64_t | UINT64 | 64-bit unsigned integer |

For languages with platform-dependent types like `long`, consider target architecture:
```c
// Example type mapper logic for 'long'
coil_type_t map_long_type(target_info_t* target) {
    if (target->is_64bit_long) {
        return COIL_TYPE_INT64;
    } else {
        return COIL_TYPE_INT32;
    }
}
```

### Floating-Point Types

| Common Language Types | COIL Type | Notes |
|-----------------------|-----------|-------|
| half, float16 | FLOAT16 | 16-bit floating-point |
| float | FLOAT32 | 32-bit floating-point |
| double | FLOAT64 | 64-bit floating-point |

For extended precision types, check if the target supports them:
```c
// Example type mapper for 'long double'
coil_type_t map_long_double_type(target_info_t* target) {
    if (target->has_float128) {
        return COIL_TYPE_FLOAT128; // Extended type if available
    } else {
        return COIL_TYPE_FLOAT64;  // Fallback to double
    }
}
```

### Boolean and Character Types

| Common Language Types | COIL Type | Notes |
|-----------------------|-----------|-------|
| bool | BOOL | Boolean value |
| char (as character) | UINT8 or INT8 | Depending on if char is signed |
| wchar_t | Target-dependent | Map based on target wchar_t size |

```c
// Example wchar_t mapping
coil_type_t map_wchar_type(target_info_t* target) {
    switch (target->wchar_size) {
        case 2: return COIL_TYPE_UINT16;
        case 4: return COIL_TYPE_UINT32;
        default: return COIL_TYPE_UINT16; // Common default
    }
}
```

### Pointer Types

Map language pointer types to COIL pointer types with appropriate memory spaces:

| Pointer Type | COIL Type | Notes |
|--------------|-----------|-------|
| General pointer | PTR | Generic pointer |
| Global memory pointer | GLOBAL_PTR | Points to global memory |
| Thread-local pointer | LOCAL_PTR | Points to thread-local memory |
| Shared memory pointer | SHARED_PTR | Points to shared memory |
| Constant memory pointer | CONST_PTR | Points to read-only memory |

For languages with pointer qualifiers (like C's `const`, `volatile`):
```c
// Example qualified pointer mapping
coil_type_t map_qualified_pointer(type_info_t* base_type, type_qualifiers_t quals) {
    coil_type_t ptr_type = COIL_TYPE_PTR;
    
    if (quals.is_const) {
        ptr_type |= COIL_TYPE_QUALIFIER_CONST;
    }
    if (quals.is_volatile) {
        ptr_type |= COIL_TYPE_QUALIFIER_VOLATILE;
    }
    if (quals.is_restrict) {
        ptr_type |= COIL_TYPE_QUALIFIER_RESTRICT;
    }
    
    return coil_create_pointer_type(ptr_type, map_type(base_type));
}
```

### Array Types

Map language array types to COIL array types:

```c
// Example array type mapping
coil_type_t map_array_type(type_info_t* element_type, size_t element_count) {
    coil_type_t coil_element_type = map_type(element_type);
    
    // For fixed-size arrays
    if (element_count > 0) {
        return coil_create_array_type(coil_element_type, element_count);
    } 
    // For unsized arrays
    else {
        return coil_create_unsized_array_type(coil_element_type);
    }
}
```

### Structure Types

Map language structure types to COIL structure types, preserving field layout:

```c
// Example structure mapping
coil_type_t map_struct_type(struct_info_t* struct_info) {
    coil_struct_builder_t builder = coil_create_struct_builder();
    
    for (int i = 0; i < struct_info->field_count; i++) {
        field_info_t* field = &struct_info->fields[i];
        coil_type_t field_type = map_type(field->type);
        
        // Add field to COIL struct
        coil_struct_builder_add_field(&builder, field->name, field_type, field->offset);
    }
    
    return coil_struct_builder_build(&builder);
}
```

### Union Types

Map language union types to COIL union types:

```c
// Example union mapping
coil_type_t map_union_type(union_info_t* union_info) {
    coil_union_builder_t builder = coil_create_union_builder();
    
    for (int i = 0; i < union_info->field_count; i++) {
        field_info_t* field = &union_info->fields[i];
        coil_type_t field_type = map_type(field->type);
        
        // Add field to COIL union
        coil_union_builder_add_field(&builder, field->name, field_type);
    }
    
    return coil_union_builder_build(&builder);
}
```

### Function Types

Map language function types to COIL function types:

```c
// Example function type mapping
coil_type_t map_function_type(function_info_t* func) {
    coil_type_t return_type = map_type(func->return_type);
    
    // Create parameter type array
    coil_type_t param_types[func->param_count];
    for (int i = 0; i < func->param_count; i++) {
        param_types[i] = map_type(func->params[i].type);
    }
    
    return coil_create_function_type(return_type, func->param_count, param_types);
}
```

### Vector Types

Map language vector types (or apply auto-vectorization) to COIL vector types:

```c
// Example vector type mapping
coil_type_t map_vector_type(type_info_t* element_type, size_t lane_count) {
    coil_type_t coil_element_type = map_type(element_type);
    return coil_create_vector_type(coil_element_type, lane_count);
}
```

## Language-Specific Considerations

### C/C++ Type Mapping

For C/C++ compilers, consider:
- Platform-dependent types (`long`, `size_t`, etc.)
- Handling of undefined behavior
- C++ class inheritance and virtual functions
- Templates and specializations
- References vs. pointers

### Memory Model Mapping

Different languages have different memory models:

- **Java/C#**: Object references and garbage collection
- **Rust**: Ownership and borrowing system
- **C/C++**: Direct memory manipulation
- **Functional languages**: Immutable data structures

Map these constructs appropriately to COIL's memory model, using:
- Appropriate pointer types
- Explicit lifetime annotations when needed
- Memory space qualifiers to represent language-specific memory spaces

## Type System Extensions

For language features not directly represented in COIL's core type system:

1. **Use metadata annotations**: Add type information in the metadata section
2. **Leverage COIL's extensibility**: Use the attribute bits in the type encoding
3. **Compose existing types**: Build complex types from simpler ones

## Type Mapping Cache

For efficiency, implement a type mapping cache:

```c
// Example type mapping cache
typedef struct {
    hashmap_t type_map;  // Maps source types to COIL types
} type_cache_t;

coil_type_t get_cached_type(type_cache_t* cache, type_info_t* type) {
    coil_type_t* cached = hashmap_get(&cache->type_map, type->id);
    if (cached) {
        return *cached;
    }
    
    // Map the type
    coil_type_t mapped_type = map_type_internal(type);
    
    // Cache the result
    hashmap_put(&cache->type_map, type->id, mapped_type);
    
    return mapped_type;
}
```

## Testing Type Mappings

Ensure your type mappings are correct by testing:

1. **Size and alignment**: Verify COIL types have the same size and alignment as source types
2. **Memory layout**: Check that composite types maintain the same memory layout
3. **Semantics**: Verify operations on mapped types have the same semantics as in the source language
4. **Edge cases**: Test corner cases like empty structures, zero-length arrays, etc.

## Type Debugging Information

Include debugging information for types:

```c
// Example type debugging info
void add_type_debug_info(coil_module_t* module, type_info_t* source_type, coil_type_t coil_type) {
    coil_debug_type_t debug_type;
    debug_type.name = source_type->name;
    debug_type.source_file = source_type->source_file;
    debug_type.line = source_type->line;
    debug_type.size = source_type->size;
    debug_type.alignment = source_type->alignment;
    
    coil_add_debug_type(module, coil_type, &debug_type);
}
```

## Conclusion

Effective type mapping is the foundation of a good COIL compiler. By carefully mapping your language's type system to COIL's type system, you ensure that the semantics of your language are preserved while enabling efficient code generation across diverse target architectures.

Remember that COIL's type system operates entirely at compile time, so all type information must be resolved during compilation.