# COIL Type System

The COIL Type System provides a framework for representing data types across diverse processing units. It operates **entirely at compile time** without runtime type information, making it suitable for bare-metal environments.

## Overview

- **Static Resolution:** All types resolved at compile time
- **Compact Encoding:** Efficient 32-bit representation of types
- **Comprehensive:** Supports primitive, composite, and specialized types
- **Zero Runtime Cost:** No runtime type information needed
- **Hardware Mapping:** Direct mapping to hardware types

## Type Encoding

Each COIL type is encoded as a 32-bit value with the following structure:

```
[category:4][width:8][qualifiers:8][attributes:12]
```

Where:
- `category`: The primary type category (4 bits)
- `width`: The bit width or size of the type (8 bits)
- `qualifiers`: Type qualifiers (8 bits)
- `attributes`: Additional type attributes (12 bits)

This compact encoding allows efficient type handling at compile time.

## Type Categories

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

## Primitive Types

### Integer Types

```
INT8, INT16, INT32, INT64       // Signed integers
UINT8, UINT16, UINT32, UINT64   // Unsigned integers
```

Integer types represent whole numbers with various bit widths.

### Floating-Point Types

```
FLOAT16   // Half precision (16-bit)
FLOAT32   // Single precision (32-bit)
FLOAT64   // Double precision (64-bit)
```

Floating-point types represent real numbers with various precisions.

### Boolean and Void Types

```
BOOL      // Boolean value (true/false)
VOID      // Void type (no value)
```

## Composite Types

### Pointer Types

Pointers reference memory locations in specific memory spaces:

```
PTR       // Generic pointer
GLOBAL_PTR  // Pointer to global memory
SHARED_PTR  // Pointer to shared memory
LOCAL_PTR   // Pointer to local memory
```

All pointer operations are resolved at compile time.

### Vector Types

Vectors are homogeneous collections of elements processed in SIMD fashion:

```
VEC2, VEC3, VEC4      // 2/3/4-element vectors
VEC8, VEC16, VEC32    // 8/16/32-element vectors
```

Vector operations map directly to hardware SIMD instructions where available.

### Structure and Array Types

```
STRUCT    // Composite of different types
ARRAY     // Collection of same-type elements
```

Structure and array layouts are determined at compile time.

## Type Safety

COIL enforces type safety through static analysis:

1. Strict compile-time type checking for operations
2. Validation of conversions during compilation
3. Memory access type validation at compile time
4. Compile-time array bounds checking where possible

## Bare-Metal Considerations

For bare-metal environments, COIL types:

1. Map directly to hardware representations
2. Honor hardware size/alignment requirements
3. Support direct register mapping
4. Include special types for memory-mapped I/O
5. Support interrupt vector types
6. Allow direct hardware register access

## Example Type Definitions

```
// Integer types
i32         // 32-bit signed integer
u16         // 16-bit unsigned integer

// Floating point
f32         // 32-bit float
f64         // 64-bit float

// Vectors
vec<f32,4>  // 4-element vector of floats

// Pointers
ptr<u8>     // Pointer to unsigned byte
```

## Differences from Runtime Type Systems

Unlike runtime-dependent type systems, COIL types:

1. Have no type objects or runtime descriptors
2. Perform all type checking at compile time
3. Generate no runtime type information
4. Require no runtime type resolution
5. Have zero runtime overhead