# COIL Type System Specification

## 1. Introduction

The COIL Type System provides a comprehensive framework for representing data types across diverse processing units. This document specifies the type encoding format, primitive types, composite types, and type qualifiers available in COIL. The type system is designed to operate without runtime dependencies, making it suitable for bare-metal environments like bootloaders and UEFI applications while still supporting complex workloads when needed.

## 2. Type Encoding

### 2.1 Basic Type Format

Each COIL type is encoded as a 32-bit value with the following structure:

```
[category:4][width:8][qualifiers:8][attributes:12]
```

Where:
- `category`: The primary type category (4 bits)
- `width`: The bit width or size of the type (8 bits)
- `qualifiers`: Type qualifiers (8 bits)
- `attributes`: Additional type attributes (12 bits)

This compact, static encoding ensures types can be processed efficiently at compile time without requiring runtime type information.

### 2.2 Type Categories

| ID | Category | Description |
|----|----------|-------------|
| 0x0 | VOID | Void type (no value) |
| 0x1 | INTEGER | Integer types |
| 0x2 | FLOAT | Floating-point types |
| 0x3 | BOOLEAN | Boolean type |
| 0x4 | POINTER | Pointer types |
| 0x5 | VECTOR | Vector types |
| 0x6 | MATRIX | Matrix types |
| 0x7 | TENSOR | Tensor types |
| 0x8 | ARRAY | Array types |
| 0x9 | STRUCT | Structure types |
| 0xA | UNION | Union types |
| 0xB | FUNCTION | Function types |
| 0xC | ENUM | Enumeration types |
| 0xD | OPAQUE | Opaque/abstract types |
| 0xE | SPECIALIZED | Hardware-specific specialized types |
| 0xF | EXTENDED | Extended type (defined in separate table) |

### 2.3 Type Qualifiers

Type qualifiers are represented as bit flags in the qualifiers field:

| Bit | Name | Description |
|-----|------|-------------|
| 0 | CONST | Value cannot be modified |
| 1 | VOLATILE | Value may change externally |
| 2 | RESTRICT | No aliasing with other pointers |
| 3 | ATOMIC | Atomic access |
| 4 | SIGNED | Signed type (for integers) |
| 5 | UNSIGNED | Unsigned type (for integers) |
| 6 | SATURATING | Saturating arithmetic |
| 7 | PRECISE | Precise evaluation (no optimization) |

## 3. Primitive Types

### 3.1 Integer Types

Integer types represent whole numbers with various bit widths.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| INT1 | 0x10010 | 1 | 1-bit signed integer |
| INT8 | 0x10810 | 8 | 8-bit signed integer |
| INT16 | 0x11010 | 16 | 16-bit signed integer |
| INT32 | 0x12010 | 32 | 32-bit signed integer |
| INT64 | 0x14010 | 64 | 64-bit signed integer |
| INT128 | 0x18010 | 128 | 128-bit signed integer |
| UINT1 | 0x10020 | 1 | 1-bit unsigned integer |
| UINT8 | 0x10820 | 8 | 8-bit unsigned integer |
| UINT16 | 0x11020 | 16 | 16-bit unsigned integer |
| UINT32 | 0x12020 | 32 | 32-bit unsigned integer |
| UINT64 | 0x14020 | 64 | 64-bit unsigned integer |
| UINT128 | 0x18020 | 128 | 128-bit unsigned integer |

#### 3.1.1 Specialized Integer Types

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| SAT_INT8 | 0x10850 | 8 | 8-bit saturating signed integer |
| SAT_INT16 | 0x11050 | 16 | 16-bit saturating signed integer |
| SAT_INT32 | 0x12050 | 32 | 32-bit saturating signed integer |
| SAT_UINT8 | 0x10860 | 8 | 8-bit saturating unsigned integer |
| SAT_UINT16 | 0x11060 | 16 | 16-bit saturating unsigned integer |
| SAT_UINT32 | 0x12060 | 32 | 32-bit saturating unsigned integer |

### 3.2 Floating-Point Types

Floating-point types represent real numbers with various precisions.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| FLOAT16 | 0x21000 | 16 | Half-precision floating-point (IEEE 754) |
| FLOAT32 | 0x22000 | 32 | Single-precision floating-point (IEEE 754) |
| FLOAT64 | 0x24000 | 64 | Double-precision floating-point (IEEE 754) |
| FLOAT80 | 0x25000 | 80 | Extended-precision floating-point (x87) |
| FLOAT128 | 0x28000 | 128 | Quad-precision floating-point (IEEE 754) |
| BFLOAT16 | 0x21001 | 16 | Brain floating-point format |
| TFLOAT32 | 0x22001 | 32 | Tensor float format |

#### 3.2.1 Alternative Floating-Point Formats

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| POSIT16 | 0x21100 | 16 | 16-bit posit number |
| POSIT32 | 0x22100 | 32 | 32-bit posit number |
| POSIT64 | 0x24100 | 64 | 64-bit posit number |

### 3.3 Fixed-Point Types

Fixed-point types represent real numbers with fixed precision.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| FIXED8_8 | 0x11100 | 16 | 8.8 fixed-point number |
| FIXED16_16 | 0x22100 | 32 | 16.16 fixed-point number |
| FIXED8_24 | 0x22101 | 32 | 8.24 fixed-point number |
| FIXED24_8 | 0x22102 | 32 | 24.8 fixed-point number |
| UFIXED8_8 | 0x11120 | 16 | 8.8 unsigned fixed-point number |
| UFIXED16_16 | 0x22120 | 32 | 16.16 unsigned fixed-point number |

### 3.4 Boolean Type

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| BOOL | 0x30001 | 1 | Boolean value (true/false) |
| BOOL8 | 0x30008 | 8 | 8-bit boolean value |

### 3.5 Void Type

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| VOID | 0x00000 | 0 | Void type (no value) |

## 4. Composite Types

### 4.1 Pointer Types

Pointers reference memory locations, with optional memory space information. All pointer operations are resolved at compile time without requiring a runtime system.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| PTR | 0x40000 | 64 | Generic pointer |
| PTR32 | 0x40020 | 32 | 32-bit pointer |
| PTR64 | 0x40040 | 64 | 64-bit pointer |

#### 4.1.1 Memory Space Qualified Pointers

Memory space qualified pointers indicate the memory space of the referenced data, which is particularly important for bare-metal programming.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| GLOBAL_PTR | 0x40100 | 64 | Pointer to global memory |
| SHARED_PTR | 0x40200 | 64 | Pointer to shared memory |
| LOCAL_PTR | 0x40300 | 64 | Pointer to local memory |
| CONSTANT_PTR | 0x40400 | 64 | Pointer to constant memory |
| TEXTURE_PTR | 0x40500 | 64 | Pointer to texture memory |

### 4.2 Vector Types

Vectors are homogeneous collections of elements processed in SIMD fashion. Vector operations map directly to hardware SIMD instructions where available.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| VEC2 | 0x50002 | varies | 2-element vector |
| VEC3 | 0x50003 | varies | 3-element vector |
| VEC4 | 0x50004 | varies | 4-element vector |
| VEC8 | 0x50008 | varies | 8-element vector |
| VEC16 | 0x50010 | varies | 16-element vector |
| VEC32 | 0x50020 | varies | 32-element vector |
| VEC64 | 0x50040 | varies | 64-element vector |

The actual size of a vector depends on its element type.

### 4.3 Matrix Types

Matrices are 2D arrays typically used for linear algebra operations.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| MAT2X2 | 0x60022 | varies | 2x2 matrix |
| MAT3X3 | 0x60033 | varies | 3x3 matrix |
| MAT4X4 | 0x60044 | varies | 4x4 matrix |
| MAT2X3 | 0x60023 | varies | 2x3 matrix |
| MAT2X4 | 0x60024 | varies | 2x4 matrix |
| MAT3X2 | 0x60032 | varies | 3x2 matrix |
| MAT3X4 | 0x60034 | varies | 3x4 matrix |
| MAT4X2 | 0x60042 | varies | 4x2 matrix |
| MAT4X3 | 0x60043 | varies | 4x3 matrix |

### 4.4 Tensor Types

Tensors are multi-dimensional arrays used for deep learning and scientific computing.

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| TENSOR1D | 0x70001 | varies | 1D tensor |
| TENSOR2D | 0x70002 | varies | 2D tensor |
| TENSOR3D | 0x70003 | varies | 3D tensor |
| TENSOR4D | 0x70004 | varies | 4D tensor |

### 4.5 Array Types

Arrays are collections of elements with a fixed size, accessed by index. Array layouts and access patterns are determined at compile time.

The array type is encoded with the element type and size in the extended attributes.

```
ARRAY(element_type, size)
```

### 4.6 Structure Types

Structures group related data elements of potentially different types. Structure layouts are determined at compile time.

Structure types are defined separately and referenced by ID.

```
STRUCT(id)
```

### 4.7 Union Types

Unions represent multiple possible types for the same memory region. Union layouts are determined at compile time.

```
UNION(id)
```

### 4.8 Function Types

Function types represent the signature of a function.

```
FUNCTION(return_type, [param_types...])
```

## 5. Specialized Hardware Types

### 5.1 GPU-Specific Types

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| HALF2 | 0xE0002 | 32 | Packed half-precision pair (for GPU) |
| HALF4 | 0xE0004 | 64 | Packed half-precision quad (for GPU) |
| TEXTURE1D | 0xE1001 | opaque | 1D texture reference |
| TEXTURE2D | 0xE1002 | opaque | 2D texture reference |
| TEXTURE3D | 0xE1003 | opaque | 3D texture reference |
| TEXTURECUBE | 0xE1004 | opaque | Cube texture reference |
| SAMPLER | 0xE2000 | opaque | Texture sampler state |

### 5.2 AI Accelerator Types

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| WEIGHT | 0xE3000 | varies | Neural network weight |
| ACTIVATION | 0xE3001 | varies | Neural network activation |
| QUINT4 | 0xE3004 | 4 | 4-bit quantized unsigned integer |
| QUINT8 | 0xE3008 | 8 | 8-bit quantized unsigned integer |
| QINT4 | 0xE3014 | 4 | 4-bit quantized signed integer |
| QINT8 | 0xE3018 | 8 | 8-bit quantized signed integer |

### 5.3 Specialized Register Types

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| PRED | 0xE4000 | 1 | Predicate register |
| PRED4 | 0xE4004 | 4 | 4-bit predicate register |
| PRED8 | 0xE4008 | 8 | 8-bit predicate register |
| PRED16 | 0xE4010 | 16 | 16-bit predicate register |
| PRED32 | 0xE4020 | 32 | 32-bit predicate register |
| PRED64 | 0xE4040 | 64 | 64-bit predicate register |
| ACCUM | 0xE5000 | varies | Accumulator register |

### 5.4 Quantum Computing Types

| Type | Encoding | Width (bits) | Description |
|------|----------|--------------|-------------|
| QUBIT | 0xE6000 | quantum | Quantum bit |
| QUREG | 0xE6001 | quantum | Quantum register |
| QUCIRCUIT | 0xE6002 | quantum | Quantum circuit |

## 6. Type Qualifiers

Type qualifiers modify the characteristics of a type.

### 6.1 Mutability Qualifiers

| Qualifier | Bit | Description |
|-----------|-----|-------------|
| CONST | 0 | Value cannot be modified |
| VOLATILE | 1 | Value may change externally (important for MMIO) |
| IMMUTABLE | ? | Value cannot be modified after initialization |

### 6.2 Memory Qualifiers

| Qualifier | Bit | Description |
|-----------|-----|-------------|
| RESTRICT | 2 | No aliasing with other pointers |
| ATOMIC | 3 | Atomic access to memory |
| COHERENT | ? | Memory access is coherent |
| READONLY | ? | Memory can only be read |
| WRITEONLY | ? | Memory can only be written |

### 6.3 Numeric Type Qualifiers

| Qualifier | Bit | Description |
|-----------|-----|-------------|
| SIGNED | 4 | Signed numeric type |
| UNSIGNED | 5 | Unsigned numeric type |
| SATURATING | 6 | Saturating arithmetic |
| PRECISE | 7 | Precise evaluation (no optimization) |
| FAST | ? | Fast math (relaxed precision) |

## 7. Type Definition and Extension

### 7.1 Structure Definition

Structure types are defined with a layout of fields, determined at compile time:

```c
/**
 * @struct coil_struct_type_def
 * @brief Structure type definition (compile-time)
 */
typedef struct {
  uint32_t id;              /* Structure identifier */
  uint32_t name_offset;     /* Offset to name string */
  uint32_t field_count;     /* Number of fields */
  uint32_t fields_offset;   /* Offset to field definitions */
  uint32_t size;            /* Total structure size in bytes */
  uint32_t alignment;       /* Structure alignment requirement */
  uint32_t attributes;      /* Structure attributes */
} coil_struct_type_def_t;
```

### 7.2 Union Definition

Union types are defined similarly, with layout determined at compile time:

```c
/**
 * @struct coil_union_type_def
 * @brief Union type definition (compile-time)
 */
typedef struct {
  uint32_t id;              /* Union identifier */
  uint32_t name_offset;     /* Offset to name string */
  uint32_t member_count;    /* Number of members */
  uint32_t members_offset;  /* Offset to member definitions */
  uint32_t size;            /* Total union size in bytes */
  uint32_t alignment;       /* Union alignment requirement */
  uint32_t attributes;      /* Union attributes */
} coil_union_type_def_t;
```

### 7.3 Function Type Definition

Function types define the signature of a function:

```c
/**
 * @struct coil_function_type_def
 * @brief Function type definition (compile-time)
 */
typedef struct {
  uint32_t id;              /* Function type identifier */
  uint32_t return_type;     /* Return type encoding */
  uint32_t param_count;     /* Number of parameters */
  uint32_t params_offset;   /* Offset to parameter definitions */
  uint32_t varargs;         /* 1 if function takes variable arguments */
  uint32_t calling_convention; /* Calling convention identifier */
  uint32_t attributes;      /* Function type attributes */
} coil_function_type_def_t;
```

## 8. Type Representations

### 8.1 Textual Representation

Types are represented textually in COIL assembly:

```
i32                     // 32-bit signed integer
u16                     // 16-bit unsigned integer
f32                     // 32-bit float
bool                    // Boolean
ptr<u8>                 // Pointer to 8-bit unsigned integer
vec<f32, 4>             // 4-element vector of 32-bit floats
mat<f32, 4, 4>          // 4x4 matrix of 32-bit floats
tensor<f32, 2, 3, 4>    // 2x3x4 tensor of 32-bit floats
array<i32, 10>          // Array of 10 32-bit integers
```

### 8.2 Binary Representation

In the COIL binary format, types are encoded as described in Section 2.1.

For complex types that require more information than fits in the basic encoding, an extended type table is used. This table is generated at compile time and included in the binary.

## 9. Type Conversions

### 9.1 Implicit Conversions

COIL defines a set of safe implicit conversions, all resolved at compile time:

| From | To | Condition |
|------|----|----|
| INT8 | INT16/INT32/INT64 | Always safe |
| INT16 | INT32/INT64 | Always safe |
| INT32 | INT64 | Always safe |
| UINT8 | UINT16/UINT32/UINT64 | Always safe |
| UINT16 | UINT32/UINT64 | Always safe |
| UINT32 | UINT64 | Always safe |
| FLOAT16 | FLOAT32/FLOAT64 | Always safe |
| FLOAT32 | FLOAT64 | Always safe |

### 9.2 Explicit Conversions

All other conversions require explicit conversion operations, which are checked at compile time.

## 10. Type Compatibility

### 10.1 Assignment Compatibility

Types A and B are assignment-compatible if:
1. A and B are the same type
2. A can be implicitly converted to B
3. A and B are pointers to assignment-compatible types
4. A is a derived type of B

Compatibility is checked at compile time for type safety.

### 10.2 Operation Compatibility

Types A and B are operation-compatible for an operation if:
1. A and B are the same type
2. A and B are both numeric types
3. A and B are both vector types with the same element count and operation-compatible element types

Operation compatibility is checked at compile time.

## 11. Type Safety

### 11.1 Type Checking Rules

COIL enforces type safety through static analysis:
1. Strict compile-time type checking for operations
2. Validation of conversions during compilation
3. Memory access type validation at compile time
4. Compile-time array bounds checking where possible
5. Null pointer checking at compile time where possible

### 11.2 Type Aliases

Type aliases allow creating alternative names for existing types:

```c
/**
 * @struct coil_type_alias
 * @brief Type alias definition (compile-time)
 */
typedef struct {
  uint32_t id;              /* Alias identifier */
  uint32_t name_offset;     /* Offset to alias name */
  uint32_t target_type;     /* Target type encoding */
} coil_type_alias_t;
```

## 12. Generic Types

### 12.1 Type Parameters

COIL supports parameterized types for generic programming, resolved at compile time:

```c
/**
 * @struct coil_type_parameter
 * @brief Type parameter definition (compile-time)
 */
typedef struct {
  uint32_t id;              /* Parameter identifier */
  uint32_t name_offset;     /* Offset to parameter name */
  uint32_t constraints_count; /* Number of constraints */
  uint32_t constraints_offset; /* Offset to constraint types */
} coil_type_parameter_t;
```

### 12.2 Generic Type Instantiation

Generic types are instantiated with concrete types during compilation:

```c
/**
 * @struct coil_generic_instance
 * @brief Generic type instantiation (compile-time)
 */
typedef struct {
  uint32_t generic_id;      /* Generic type identifier */
  uint32_t instance_id;     /* Instance identifier */
  uint32_t arguments_count; /* Number of type arguments */
  uint32_t arguments_offset; /* Offset to type arguments */
} coil_generic_instance_t;
```

## 13. Bare-Metal Implementation Guidelines

### 13.1 Type System Implementation for Bare Metal

When implementing the COIL type system for bare-metal environments:

1. **Static Type Resolution**: Resolve all types at compile/link time
2. **No Runtime Type Information**: Avoid generating runtime type metadata
3. **Direct Hardware Mapping**: Map types directly to hardware representations
4. **Size and Alignment Awareness**: Honor hardware size/alignment requirements
5. **No Dynamic Allocation**: Avoid dynamic type allocation or construction
6. **Memory-Efficient Representation**: Use compact type encodings
7. **Hardware Register Mapping**: Support direct mapping to hardware registers
8. **MMIO Support**: Special handling for memory-mapped I/O types
9. **Interrupt Vector Types**: Support for interrupt vector tables
10. **Position-Independent Types**: Support for position-independent code

### 13.2 Type Lowering for Bare Metal

For target-specific code generation in bare-metal environments:

1. **Direct Register Mapping**: Map types to hardware registers
2. **Memory Layout Control**: Precise control over memory layout
3. **Static Memory Allocation**: Pre-allocate all required memory
4. **Hardware-Specific Types**: Support for special hardware types
5. **Alignment Requirements**: Honor hardware alignment requirements
6. **Bitfield Packing**: Efficient packing of bit fields for hardware registers
7. **Volatile Handling**: Proper handling of volatile memory accesses
8. **Atomic Operations**: Support for atomic operations where available
9. **Interrupt Context Types**: Special handling for interrupt contexts
10. **Device Register Types**: Types for device register access

### 13.3 Type Extensions for Bare Metal

For custom bare-metal hardware features:

1. **Hardware Register Maps**: Types representing hardware register maps
2. **Device Configuration Types**: Types for device configuration
3. **Memory-Mapped I/O Types**: Special types for MMIO access
4. **Port I/O Types**: Types for port I/O operations
5. **Interrupt Vector Types**: Types for interrupt vector tables
6. **DMA Buffer Types**: Special types for DMA buffer handling
7. **Hardware Timer Types**: Types for hardware timer access
8. **Power Management Types**: Types for power management hardware
9. **Security Hardware Types**: Types for security-related hardware
10. **Boot Configuration Types**: Types for boot configuration data

## 14. Future Extensions

Areas for future development in the COIL type system:

1. **Dependent Types**: Types that depend on values
2. **Effect Types**: Track side effects in the type system
3. **Linear/Affine Types**: Enhanced resource management
4. **Refinement Types**: Types with predicates
5. **Higher-kinded Types**: Type constructors that take types as parameters

These extensions would maintain the principle of static resolution during compilation without introducing runtime type dependencies.