# COIL Variable Management System

## Introduction

One of COIL's key abstractions that differentiates it from traditional assembly languages is its variable management system. This system provides a structured approach to handling data while maintaining the performance characteristics of low-level code.

## Design Philosophy

The COIL variable management system is designed with several goals in mind:

1. **Optimization-Friendly**: Allow the COIL assembler to make intelligent decisions about register allocation and memory layout.
2. **Type Safety**: Provide enough type information to catch common errors at compile time.
3. **Close to the Metal**: Maintain a direct correspondence between variables and their physical storage.
4. **Flexibility**: Support a wide range of data types and storage classes to accommodate diverse programming needs.

## Variable Declaration

In COIL, variables must be explicitly declared before use. A variable declaration specifies:

1. The variable's name
2. Its data type
3. Its storage class (optional, defaults to "auto")
4. Initialization value (optional)
5. Target-specific attributes (optional)

### HOIL Syntax for Variable Declaration

```
var <name> : <type> [storage(<storage_class>)] [= <initial_value>] [target(<target_id>)]
```

Examples in HOIL:

```
var counter : u32                      // 32-bit unsigned integer with auto storage
var pi : f64 = 3.14159                 // 64-bit float with initial value
var buffer : u8[1024] storage(static)  // Static buffer of 1024 bytes
var acc : u64 storage(register)        // 64-bit register variable
var x_coord : i32 target(GPU)          // 32-bit integer specific to GPU target
```

## Data Types

COIL supports a rich set of primitive and composite data types:

### Primitive Types

| Type      | Description                           | Size (bits) |
|-----------|---------------------------------------|-------------|
| i8        | 8-bit signed integer                  | 8           |
| u8        | 8-bit unsigned integer                | 8           |
| i16       | 16-bit signed integer                 | 16          |
| u16       | 16-bit unsigned integer               | 16          |
| i32       | 32-bit signed integer                 | 32          |
| u32       | 32-bit unsigned integer               | 32          |
| i64       | 64-bit signed integer                 | 64          |
| u64       | 64-bit unsigned integer               | 64          |
| f32       | 32-bit IEEE 754 floating point        | 32          |
| f64       | 64-bit IEEE 754 floating point        | 64          |
| bool      | Boolean value                         | 8           |
| ptr       | Target-dependent pointer type         | 32/64       |
| void      | No type (used for pointers)           | 0           |

### Composite Types

COIL also supports several composite types:

1. **Arrays**: Fixed-size collections of elements of the same type
   ```
   var array : u32[10]  // Array of 10 unsigned 32-bit integers
   ```

2. **Structures**: Collections of named fields of possibly different types
   ```
   struct Point {
     x : i32
     y : i32
   }
   var origin : Point = {0, 0}
   ```

3. **Unions**: Overlaid fields sharing the same memory location
   ```
   union IntOrFloat {
     i : i32
     f : f32
   }
   var data : IntOrFloat = {i: 42}
   ```

4. **Pointers**: References to other variables
   ```
   var value : i32 = 42
   var ptr : ptr<i32> = &value
   ```

## Storage Classes

COIL supports different storage classes to give programmers control over variable lifetime and location:

1. **Auto** (default): Automatic storage duration, typically allocated on the stack or in registers.
2. **Static**: Static storage duration, allocated once and persists for the entire program execution.
3. **Register**: Hint to the assembler that this variable should preferably be kept in a register.
4. **Global**: Global variable accessible from multiple functions.
5. **Thread_local**: Per-thread global variable.

## Type System Implementation

The COIL type system is implemented through the Type Table in the COIL binary format. Each type has a unique ID and a definition that specifies its properties.

When the COIL assembler processes a variable declaration, it:

1. Locates the type definition in the Type Table
2. Allocates appropriate storage based on the type's size and alignment requirements
3. Generates the necessary instructions to initialize the variable (if an initial value is provided)
4. Records the variable's properties in the Variable Table

## Register Allocation

One of the key advantages of COIL's variable system is that it allows the assembler to perform intelligent register allocation. The assembler analyzes:

1. Variable lifetimes (when variables are first defined and last used)
2. Access patterns (how frequently variables are used)
3. Data dependencies between variables
4. Target architecture register constraints

Based on this analysis, the assembler decides which variables to keep in registers and which to spill to memory, optimizing for performance while maintaining correctness.

## Memory Layout Optimization

For variables stored in memory, COIL can optimize their layout to:

1. Minimize padding between structure fields
2. Align data for optimal access speed
3. Group related variables to improve cache locality
4. Separate hot and cold data to reduce cache pollution

## Target-Specific Variables

COIL supports target-specific variables through the `target` attribute. These variables are only visible and accessible when generating code for the specified target. This is particularly useful for heterogeneous computing where different variables might exist on different processing units.

## Variable Scoping

COIL supports lexical scoping of variables. Each variable is visible from its declaration point until the end of its enclosing scope. Scopes are defined by function boundaries and explicit block constructs.

In HOIL:
```
function example() {
  var x : i32 = 10
  
  {  // Start of inner scope
    var y : i32 = 20
    // Both x and y are visible here
  }  // End of inner scope
  
  // Only x is visible here
}
```

## Accessing Variables

Variables can be accessed directly by name in most COIL instructions. The assembler automatically generates the appropriate machine code to load or store the variable based on its storage location.

For example, in HOIL:
```
var a : i32 = 10
var b : i32 = 20
var c : i32

c = a + b  // COIL handles the loading of a and b and storing to c
```

The COIL assembler might translate this to:
- Load value of 'a' into register R1
- Load value of 'b' into register R2
- Add R1 and R2, storing result in R3
- Store value from R3 into memory location for 'c'

Or, if it determines through analysis that these variables are frequently used, it might keep them all in registers throughout their lifetimes, resulting in a single add instruction.

## Address-of Operator

COIL provides an address-of operator `&` to obtain the memory address of a variable:

```
var value : i32 = 42
var ptr : ptr<i32> = &value
```

## Dereferencing Pointers

Pointers can be dereferenced using the `*` operator:

```
var value : i32 = 42
var ptr : ptr<i32> = &value
var copy : i32 = *ptr  // copy now contains 42
```

## Example: Variable Optimization

Consider the following HOIL code:

```
function calculate_average(values: ptr<i32>, count: i32) -> f32 {
  var sum : i32 = 0
  var i : i32 = 0
  
  while (i < count) {
    sum = sum + values[i]
    i = i + 1
  }
  
  var avg : f32 = (f32)sum / (f32)count
  return avg
}
```

The COIL assembler might optimize this by:

1. Keeping `sum` and `i` in registers throughout the loop
2. Using auto-increment addressing modes for accessing array elements
3. Performing the conversion and division in a single instruction if the target supports it
4. Returning the result directly in a floating-point register

The resulting assembly would be highly efficient, with minimal memory accesses and optimal use of the target's instruction set.

## Advanced Features

### Register Hints

For performance-critical code, COIL allows programmers to provide register hints:

```
var counter : i32 storage(register(R1))  // Suggest using register R1
```

### Alignment Control

COIL provides control over variable alignment:

```
var buffer : u8[1024] align(16)  // Align buffer to 16-byte boundary
```

### Volatile Variables

For memory-mapped I/O and other special cases, COIL supports volatile variables:

```
var status_reg : u32 volatile = 0x4000  // Memory-mapped status register
```

### Constant Variables

COIL allows marking variables as constant, which can enable additional optimizations:

```
var pi : f64 constant = 3.14159
```

## Conclusion

COIL's variable management system bridges the gap between high-level language variables and low-level register/memory operations. By providing a structured approach to data handling while maintaining close-to-the-metal performance, it enables programmers to write efficient cross-platform code without sacrificing control over the underlying hardware.