# HOIL Language Specification

## 1. Overview

The Human Oriented Intermediate Language (HOIL) is the textual representation of COIL (Compiler Optimization Intermediate Language). While COIL is designed as a binary format for efficiency, HOIL provides a human-readable syntax that makes COIL code accessible for reading, writing, and understanding.

HOIL serves multiple important purposes in the COIL ecosystem:
- **Readability**: Makes COIL accessible to human readers and writers
- **Debugging**: Helps developers inspect and debug intermediate code
- **Educational**: Aids in learning and understanding the COIL model
- **Direct Programming**: Enables manual coding at the IR level
- **Testing**: Facilitates testing of COIL toolchains

The relationship between HOIL and COIL is direct and bidirectional:
- Every HOIL construct has a corresponding COIL binary representation
- Every COIL binary can be decompiled to equivalent HOIL
- No semantic information is lost in translation between formats

## 2. Language Syntax

### 2.1 Basic Structure

A HOIL program consists of several sections:

```
// Module declaration
MODULE "module_name";

// Optional target requirements
TARGET {
  required_features = ["feature1", "feature2"]
  device_class = "CPU"
}

// Type definitions
TYPE point { x: f32, y: f32 }

// Constants
CONSTANT PI: f64 = 3.14159265359;

// Global variables
GLOBAL counter: i32 = 0;

// External function declarations
EXTERN FUNCTION print(message: ptr<i8>) -> void;

// Function definitions
FUNCTION add(a: i32, b: i32) -> i32 {
    ENTRY:
        result = ADD a, b;
        RET result;
}
```

### 2.2 Lexical Structure

#### Identifiers
- Start with a letter or underscore
- Can contain letters, digits, and underscores
- Case-sensitive
- Cannot match reserved keywords

#### Comments
- Line comments: `// comment`
- Block comments: `/* comment */`

#### Numbers
- Decimal: `123`, `3.14159`
- Hexadecimal: `0xFF`, `0x1A3`
- Binary: `0b1101`, `0b0010_1100`
- Scientific: `1.23e5`, `9.87e-3`

#### Strings
- Enclosed in double quotes: `"Hello, world!"`
- Escape sequences: `\n`, `\t`, `\"`, `\\`
- Null terminator: `\00`

### 2.3 Types

#### Basic Types
```
i8       // 8-bit signed integer
u8       // 8-bit unsigned integer
i16      // 16-bit signed integer
u16      // 16-bit unsigned integer
i32      // 32-bit signed integer
u32      // 32-bit unsigned integer
i64      // 64-bit signed integer
u64      // 64-bit unsigned integer
f16      // 16-bit floating-point
f32      // 32-bit floating-point
f64      // 64-bit floating-point
bool     // Boolean
void     // Void type
```

#### Pointer Types
```
ptr<i32>                // Pointer to i32
ptr<i32, global>        // Pointer to global i32
ptr<i32, constant>      // Pointer to constant i32
```

#### Vector Types
```
vec<f32, 4>    // Vector of 4 32-bit floats
vec<i16, 8>    // Vector of 8 16-bit integers
```

#### Array Types
```
array<i32, 10>    // Array of 10 32-bit integers
array<u8>         // Unsized array of bytes
```

#### Structure Types
```
TYPE point {
    x: f32,
    y: f32,
    z: f32
}
```

#### Function Types
```
function(i32, i32) -> i32    // Function taking two i32 parameters and returning i32
```

### 2.4 Functions

#### Function Declaration
```
FUNCTION add(a: i32, b: i32) -> i32 {
    ENTRY:
        result = ADD a, b;
        RET result;
}
```

#### External Function Declaration
```
EXTERN FUNCTION printf(format: ptr<i8>, ...) -> i32;
```

#### Target-Specific Function
```
FUNCTION vector_add(a: vec<f32, 4>, b: vec<f32, 4>) -> vec<f32, 4>
TARGET "GPU" {
    ENTRY:
        result = VADD a, b;
        RET result;
}
```

### 2.5 Basic Blocks

Functions are organized into labeled basic blocks:

```
FUNCTION max(a: i32, b: i32) -> i32 {
    ENTRY:
        is_greater = CMP_GT a, b;
        BR is_greater, THEN, ELSE;
        
    THEN:
        RET a;
        
    ELSE:
        RET b;
}
```

Each block ends with a terminator instruction (branch, return, etc.).

### 2.6 Instructions

Instructions have the format:
```
[result =] OPCODE operand1, operand2, ...;
```

Most instructions assign their result to a named variable.

#### Arithmetic Instructions
```
result = ADD a, b;        // Addition
result = SUB a, b;        // Subtraction
result = MUL a, b;        // Multiplication
result = DIV a, b;        // Division
result = REM a, b;        // Remainder
result = NEG a;           // Negation
```

#### Logical Instructions
```
result = AND a, b;        // Bitwise AND
result = OR a, b;         // Bitwise OR
result = XOR a, b;        // Bitwise XOR
result = NOT a;           // Bitwise NOT
result = SHL a, b;        // Shift left
result = SHR a, b;        // Shift right
```

#### Comparison Instructions
```
result = CMP_EQ a, b;     // Equal
result = CMP_NE a, b;     // Not equal
result = CMP_LT a, b;     // Less than
result = CMP_LE a, b;     // Less than or equal
result = CMP_GT a, b;     // Greater than
result = CMP_GE a, b;     // Greater than or equal
```

#### Memory Instructions
```
value = LOAD ptr;             // Load from memory
STORE ptr, value;             // Store to memory
ptr = LEA array_base, index;  // Load effective address
FENCE memory;                 // Memory fence
```

#### Control Flow Instructions
```
BR ALWAYS, label;             // Unconditional branch
BR condition, true_label, false_label;  // Conditional branch
result = CALL func(arg1, arg2);  // Function call
RET value;                    // Return with value
RET;                          // Return void
```

#### Vector Instructions
```
result = VADD a, b;           // Vector addition
result = VDOT a, b;           // Vector dot product
result = VSPLAT scalar, 4;    // Splat scalar to vector
result = VLOAD ptr;           // Load vector from memory
```

#### Atomic Instructions
```
result = ATOMIC_ADD ptr, value;  // Atomic add
result = ATOMIC_CAS ptr, expected, desired;  // Compare and swap
```

#### Conversion Instructions
```
result = CONVERT a, i32;      // Convert to i32
result = TRUNC a, i16;        // Truncate to i16
result = EXTEND a, i64;       // Extend to i64
```

## 3. Detailed Semantics

### 3.1 Memory Model

HOIL inherits COIL's memory model with explicit memory spaces:

```
global_var = LOAD [global_ptr];             // Load from global memory
STORE [local_ptr], value;                   // Store to local memory
FENCE memory_order_acquire;                 // Memory fence with acquire semantics
atomic_val = ATOMIC_ADD [ptr], 1, memory_order_relaxed;  // Atomic add with relaxed ordering
```

### 3.2 Type System

HOIL has a strong, static type system with compile-time checking:

```
// Type definitions
TYPE matrix3x3 { rows: array<vec<f32, 3>, 3> }

// Function with these types
FUNCTION matrix_vector_mul(m: matrix3x3, v: vec<f32, 3>) -> vec<f32, 3> {
    ENTRY:
        row0 = EXTRACT m.rows, 0;
        row1 = EXTRACT m.rows, 1;
        row2 = EXTRACT m.rows, 2;
        
        dot0 = VDOT row0, v;
        dot1 = VDOT row1, v;
        dot2 = VDOT row2, v;
        
        result = VPACK dot0, dot1, dot2;
        RET result;
}
```

### 3.3 Control Flow

HOIL uses structured control flow with labeled blocks:

```
FUNCTION factorial(n: i32) -> i32 {
    ENTRY:
        // Base case
        is_base = CMP_LE n, 1;
        BR is_base, BASE_CASE, RECURSIVE_CASE;
        
    BASE_CASE:
        RET 1;
        
    RECURSIVE_CASE:
        n_minus_1 = SUB n, 1;
        fact_nm1 = CALL factorial(n_minus_1);
        result = MUL n, fact_nm1;
        RET result;
}
```

### 3.4 Function Calls

Functions can be called with explicit parameter passing:

```
FUNCTION caller() -> i32 {
    ENTRY:
        // Call with literal arguments
        sum1 = CALL add(5, 7);
        
        // Call with variables
        a = LOAD_I32 10;
        b = LOAD_I32 20;
        sum2 = CALL add(a, b);
        
        // Call with mixed arguments
        sum3 = CALL add(sum1, b);
        
        total = ADD sum1, ADD sum2, sum3;
        RET total;
}
```

## 4. Implementation Strategy

The current implementation strategy for HOIL follows these guidelines:

### 4.1 Parser Implementation

1. Two-pass parsing:
   - First pass collects symbol information and module structure
   - Second pass resolves all references and generates intermediate form

2. Recursive descent parser pattern for clean handling of nested structures

3. Context-aware parsing that understands instruction semantics and type rules

4. Robust error handling with clear diagnostics and recovery mechanisms

### 4.2 HOIL to COIL Conversion

1. Direct mapping from HOIL syntax to COIL binary format

2. Type checking during conversion to ensure type safety

3. Two-stage conversion:
   - Parse HOIL to in-memory abstract syntax tree (AST)
   - Convert AST to COIL binary representation

4. Optimization opportunities during conversion

### 4.3 COIL to HOIL Decompilation

1. Structured decompilation that recovers control flow structures

2. Symbolic naming for readability:
   - Generate meaningful names for blocks
   - Create descriptive variable names where possible
   - Maintain original names when debug information is present

3. Pretty printing with proper indentation and formatting

4. Comment generation to improve readability

### 4.4 IDE Integration

1. Syntax highlighting for major editors and IDEs

2. Language server protocol implementation for code intelligence

3. Integration with debugging tools

4. Code navigation and refactoring support

## 5. Future Directions

HOIL is designed to evolve alongside COIL with these planned enhancements:

### 5.1 Syntax Enhancements

1. **Macro Support**: Introduce simple macro system for code reuse and generation
   ```
   MACRO VECTOR_OP(name, op) {
       FUNCTION name(a: vec<f32, 4>, b: vec<f32, 4>) -> vec<f32, 4> {
           ENTRY:
               result = op a, b;
               RET result;
       }
   }
   
   VECTOR_OP(vec_add, VADD)
   VECTOR_OP(vec_mul, VMUL)
   ```

2. **Pattern Matching**: Higher-level pattern matching for complex optimizations
   ```
   PATTERN {
       x = ADD a, 0;
   } REPLACE {
       x = a;  // Simplify addition with zero
   }
   ```

3. **Template Support**: Generics-like templates for code parameterization
   ```
   TEMPLATE<T: numeric_type, N: int>
   FUNCTION vector_sum(v: vec<T, N>) -> T {
       // Implementation
   }
   ```

### 5.2 Tool Improvements

1. **Interactive HOIL Shell**: REPL environment for experimentation
2. **Visual Debugger**: Graphical debugging of HOIL code execution
3. **Performance Analyzer**: Tools to analyze and optimize HOIL code
4. **Auto-Vectorizer**: Automatic vectorization of scalar HOIL code

### 5.3 Language Integration

1. **Multi-language Support**: Better integration with source languages
2. **Source Mapping**: Improved source code mapping for debugging
3. **Higher-level Abstractions**: Optional higher-level constructs for common patterns
4. **Domain-Specific Extensions**: Specialized syntax for domains like ML, graphics

### 5.4 Educational Focus

1. **Interactive Tutorials**: Step-by-step guides for learning HOIL
2. **Visualization Tools**: Visual representation of HOIL execution
3. **Comparative Views**: Side-by-side views of source code, HOIL, and native code
4. **Playground Environments**: Online experimentation platforms

## 6. Examples

### 6.1 Simple Function

```
FUNCTION gcd(a: i32, b: i32) -> i32 {
    ENTRY:
        is_b_zero = CMP_EQ b, 0;
        BR is_b_zero, DONE, LOOP;
        
    LOOP:
        remainder = REM a, b;
        a = b;
        b = remainder;
        is_b_zero = CMP_EQ b, 0;
        BR is_b_zero, DONE, LOOP;
        
    DONE:
        RET a;
}
```

### 6.2 Vector Processing

```
FUNCTION normalize(v: vec<f32, 3>) -> vec<f32, 3> {
    ENTRY:
        // Compute length squared
        len_sq = VDOT v, v;
        
        // Check for zero vector
        is_zero = CMP_LT len_sq, 0.000001;
        BR is_zero, ZERO_VECTOR, NORMAL;
        
    ZERO_VECTOR:
        zero = VSPLAT 0.0, 3;
        RET zero;
        
    NORMAL:
        // Compute inverse square root
        len = SQRT len_sq;
        inv_len = DIV 1.0, len;
        
        // Scale vector
        scale = VSPLAT inv_len, 3;
        result = VMUL v, scale;
        
        RET result;
}
```

### 6.3 Memory Management

```
FUNCTION copy_buffer(src: ptr<u8>, dst: ptr<u8>, size: i32) -> void {
    ENTRY:
        i = LOAD_I32 0;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_CHECK:
        in_bounds = CMP_LT i, size;
        BR in_bounds, LOOP_BODY, DONE;
        
    LOOP_BODY:
        // Get source and destination addresses
        src_ptr = ADD src, i;
        dst_ptr = ADD dst, i;
        
        // Load and store byte
        value = LOAD [src_ptr];
        STORE [dst_ptr], value;
        
        // Increment counter
        i = ADD i, 1;
        BR ALWAYS, LOOP_CHECK;
        
    DONE:
        RET;
}
```

### 6.4 Complete Program Example

```
MODULE "image_processing";

// Type definitions
TYPE rgb_pixel {
    r: u8,
    g: u8,
    b: u8
}

TYPE image {
    width: i32,
    height: i32,
    data: ptr<rgb_pixel>
}

// Constants
CONSTANT MAX_INTENSITY: u8 = 255;

// Global variables
GLOBAL processed_count: i32 = 0;

// Function to invert pixel colors
FUNCTION invert_pixel(pixel: rgb_pixel) -> rgb_pixel {
    ENTRY:
        new_pixel = pixel;  // Create copy
        
        // Invert each channel
        new_pixel.r = SUB MAX_INTENSITY, pixel.r;
        new_pixel.g = SUB MAX_INTENSITY, pixel.g;
        new_pixel.b = SUB MAX_INTENSITY, pixel.b;
        
        RET new_pixel;
}

// Function to process an entire image
FUNCTION invert_image(img: image) -> void {
    ENTRY:
        // Calculate total pixels
        size = MUL img.width, img.height;
        i = LOAD_I32 0;
        BR ALWAYS, LOOP_CHECK;
        
    LOOP_CHECK:
        in_bounds = CMP_LT i, size;
        BR in_bounds, LOOP_BODY, DONE;
        
    LOOP_BODY:
        // Get pixel pointer
        pixel_ptr = ADD img.data, i;
        
        // Load pixel
        pixel = LOAD [pixel_ptr];
        
        // Process pixel
        new_pixel = CALL invert_pixel(pixel);
        
        // Store result
        STORE [pixel_ptr], new_pixel;
        
        // Increment counter
        i = ADD i, 1;
        BR ALWAYS, LOOP_CHECK;
        
    DONE:
        // Update global counter
        old_count = LOAD processed_count;
        new_count = ADD old_count, size;
        STORE processed_count, new_count;
        
        RET;
}

// Vector-optimized version for CPU targets
FUNCTION invert_image(img: image) -> void
TARGET "CPU" {
    ENTRY:
        // Vector implementation that processes multiple pixels at once
        // using CPU vector instructions
        // ...
}
```

## 7. Conclusion

HOIL provides a human-readable representation of COIL that maintains all the power and flexibility of the binary format while making it accessible for human consumption. The direct 1:1 mapping ensures that HOIL can be reliably translated to and from COIL without loss of information.

As a critical component of the COIL ecosystem, HOIL enables debugging, education, testing, and direct programming at the intermediate representation level. Its clear syntax and semantics make it an ideal tool for understanding how high-level language constructs map to lower-level operations.

The future development of HOIL will focus on enhancing its usability, extending its expressiveness, and improving its tooling while maintaining perfect compatibility with COIL's evolving capabilities.