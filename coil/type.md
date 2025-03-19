# Type System (Version 0.1.0)

The COIL type system provides a platform-independent way to define and manipulate data types across different architectures. This specification defines the primitive types supported in COIL version 0.1.0.

## Type Encoding

type is just a value from 0 to 255. Type can be followed by some optional data which isn't specified in the normal opcode but assumed to come when type: uint8_t is used. For example in the future with anonymous structures which could have the full type information in the instruction. The first value specifies what should be expected to be parsed.

```c
enum type {
  // Primitives (x00 - x0F)
  COIL_TYPE_INT = 0x00, // expect another uint8_t value describing the width
  COIL_TYPE_UINT = 0x01, // expect another uint8_t value describing the width
  COIL_TYPE_FLOAT = 0x02, // expect another uint8_t value describing the width
  
  // Extended (0x10 - 0x1F)
  COIL_TYPE_VEC = 0x10,

  // Composite (0x20 - 0x3F)
  COIL_TYPE_STRUCT = 0x20, // padded memory
  COIL_TYPE_UNION  = 0x21,  // alias the same memory
  COIL_TYPE_PACK   = 0x22,   // structure without padding
  
  // Memory (0x40 - 0x5F)
  COIL_TYPE_ARR = 0x40,

  // Other (0xF0 - 0xFF)
  COIL_TYPE_VOID = 0xF0
  COIL_TYPE_BOOL = 0xF1, // only requires 1 bit and can be optimized into bit maps
  COIL_TYPE_LINT = 0xF2, // the largest native integer supported
  COIL_TYPE_FINT = 0xF3, // the fastest native ineteger supported
  COIL_TYPE_PTR  = 0xF4,  // the native pointer type 
  COIL_TYPE_PARAM2 = 0xFD, // certain instructions support a parameter and this is the type format for the operand
  COIL_TYPE_PARAM1 = 0xFE, // certain instructions support a parameter and this is the type format for the operand
  COIL_TYPE_PARAM0 = 0xFF, // certain instructions support a parameter and this is the type format for the operand
}
```

## Parameters / Qualifiers

Parameters provide additional information about how the instruction should be executed. The specific interpretation of parameters depends on the instruction and type (PARAM2 - PARAM0).

### Branch Conditional Parameter
```c
// For COIL_CF_BRC
enum cond_parameter : uint8_t {
  COIL_BRIF_EQ   = 0x00, // Equal
  COIL_BRIF_NE   = 0x00, // Not equal
  COIL_BRIF_LT   = 0x00, // Less than
  COIL_BRIF_LE   = 0x00, // Less than or equal
  COIL_BRIF_GT   = 0x00, // Greater than
  COIL_BRIF_GE   = 0x00, // Greater than or equal
}
```

### Arithmetic Qualifier Bits
```c
// For arithmetic operations like ADD, SUB, MUL, etc. using COIL_TYPE_PARAM0
enum arith_qualifier : uint8_t {
  COIL_ARITH_IN_PLACE = (1 << 0), // First operand is also the destination
  COIL_ARITH_SIGNED   = (1 << 1), // Treat operands as signed values
  COIL_ARITH_SAT      = (1 << 2), // Use saturation arithmetic
  COIL_ARITH_FLOAT    = (1 << 3), // Force floating-point operation
  // Bits 4-7 reserved
}
```

### Memory Qualifier Bits
```c
// For memory operations using COIL_TYPE_PARAM0
enum memory_qualifier : uint8_t {
  COIL_MEM_VOLATILE = (1 << 0), // Volatile access
  COIL_MEM_ATOMIC   = (1 << 1), // Atomic access
  COIL_MEM_ALIGNED  = (1 << 2), // Ensure aligned access
  COIL_MEM_BARRIER  = (1 << 3), // Memory barrier
  // Bits 4-7 reserved
}
```


## Type Conversions

COIL supports both implicit and explicit type conversions:

### Implicit Conversions

The following implicit conversions are guaranteed to preserve the value:

1. Smaller integer to larger integer of the same signedness (e.g., i8 → i16)
2. Any integer to floating-point type (e.g., i32 → f32)
3. Any floating-point type to a larger floating-point type (e.g., f32 → f64)

### Explicit Conversions

All other conversions must be explicit using the appropriate conversion instructions:

1. **TRUNC**: Truncate a value to a smaller type (e.g., i32 → i16)
2. **EXT**: Extend a value to a larger type with sign extension (e.g., i8 → i32)
3. **FTOI**: Convert floating-point to integer (e.g., f32 → i32)
4. **ITOF**: Convert integer to floating-point (e.g., i32 → f32)
5. **FTOB**: Convert floating-point to bit representation (e.g., f32 → i32)
6. **BTOF**: Convert bit representation to floating-point (e.g., i32 → f32)

## Type Safety

COIL is a strongly-typed intermediate language. Type checking is performed by the COIL assembler to ensure type compatibility in operations. Explicit conversions must be used when required.

## Type Implementation Requirements

COIL assemblers must support all primitive types defined in this specification, even on architectures that do not natively support them. For example:

1. 64-bit integers must be supported on 32-bit architectures through emulation
2. Floating-point types must be supported on architectures without FPUs through software emulation
3. 128-bit types may be emulated through pairs of 64-bit values

## Future Extensions

Future versions of COIL will extend the type system to include:

1. Composite types (structures, unions)
2. Array types
3. Vector types for SIMD operations
4. Abstract width types
5. More specialized numeric types

These extensions will be defined in later specification versions.