# HOIL - Human Oriented Intermediate Language

HOIL (Human Oriented Intermediate Language) is the human-readable textual representation of COIL. While COIL is primarily a binary format optimized for machine processing, HOIL provides a readable syntax for developers to write code that will be assembled into COIL.

## 1. Syntax Overview

HOIL syntax is designed to be concise yet readable, finding a balance between traditional assembly language and higher-level languages. It uses a line-based structure with support for blocks and scopes.

### 1.1 Basic Structure

A HOIL file consists of these elements:

```
# Module declaration
module example

# Imports
import stdio
import math

# Global variables
var i32 global_counter = 0
var f64 [align(16)] alignment_example

# Procedures
procedure main() {
    var i32 local_var = 42
    call stdio.printf("Value: %d\n", local_var)
}

# Data sections
data constants {
    string hello_string = "Hello, COIL!"
    i32 magic_numbers[] = {42, 123, 7, 256}
}
```

### 1.2 Comments

HOIL supports single-line comments using the `#` character:

```
# This is a comment
var i32 x = 10  # This is an end-of-line comment
```

Multi-line comments are enclosed between `/*` and `*/`:

```
/*
 * This is a multi-line comment
 * that spans several lines
 */
```

## 2. Data Types

HOIL supports all the data types defined in COIL, with a human-readable syntax.

### 2.1 Basic Types

```
# Integer types
var i8 byte_val = 42     # 8-bit signed integer
var i16 short_val = 1000  # 16-bit signed integer
var i32 int_val = 70000   # 32-bit signed integer
var i64 long_val = 5000000000  # 64-bit signed integer
var i128 huge_val    # 128-bit signed integer (if supported)

# Unsigned integer types
var u8 ubyte_val = 42     # 8-bit unsigned integer
var u16 ushort_val = 1000  # 16-bit unsigned integer
var u32 uint_val = 70000   # 32-bit unsigned integer
var u64 ulong_val = 5000000000  # 64-bit unsigned integer
var u128 uhuge_val    # 128-bit unsigned integer (if supported)

# Floating-point types
var f16 half_val = 3.14   # 16-bit floating-point (if supported)
var f32 float_val = 3.14159  # 32-bit floating-point
var f64 double_val = 3.14159265358979  # 64-bit floating-point
var f80 extended_val  # 80-bit extended precision (if supported)
var f128 quad_val     # 128-bit quad precision (if supported)

# Brain floating-point
var bf16 bfloat_val   # Brain floating-point (if supported)

# Boolean type
var bool flag = true   # Boolean value

# Address type
var addr.i32 ptr      # Pointer to i32
```

### 2.2 Vector Types

```
# Fixed-size vectors
var v128.i32 vec1     # 128-bit vector of 32-bit integers (4 elements)
var v256.f32 vec2     # 256-bit vector of 32-bit floats (8 elements)
var v512.i64 vec3     # 512-bit vector of 64-bit integers (8 elements)

# Scalable vectors (where supported)
var vscalable.i32 vec4  # Scalable vector of 32-bit integers
```

### 2.3 Arrays and Composite Types

```
# Fixed-size arrays
var i32 array1[10]       # Array of 10 32-bit integers
var f64 matrix[3][3]     # 3x3 matrix of 64-bit floats

# Structures
struct point {
    f32 x
    f32 y
}

var point p1             # Structure variable
var point points[10]     # Array of structures
```

## 3. Variables and Constants

### 3.1 Variable Declaration

Variables are declared using the `var` keyword, with optional initialization:

```
var i32 counter = 0       # Variable with initialization
var f64 temperature       # Variable without initialization
```

### 3.2 Constants

Constants are declared using the `const` keyword:

```
const i32 MAX_COUNT = 100
const f64 PI = 3.14159265358979
```

### 3.3 Variable Attributes

Variables can have attributes enclosed in square brackets:

```
var i32 [volatile] io_register     # Volatile variable
var i32 [register] counter         # Register allocation hint
var f64 [align(16)] aligned_data   # Alignment specification
```

## 4. Operators and Expressions

HOIL provides a rich set of operators for building expressions:

### 4.1 Arithmetic Operators

```
var i32 a = 10
var i32 b = 20
var i32 c

add.i32 c, a, b     # c = a + b
sub.i32 c, a, b     # c = a - b
mul.i32 c, a, b     # c = a * b
div.i32 c, a, b     # c = a / b
rem.i32 c, a, b     # c = a % b
neg.i32 c, a        # c = -a
```

### 4.2 Bitwise Operators

```
and.i32 c, a, b     # c = a & b
or.i32 c, a, b      # c = a | b
xor.i32 c, a, b     # c = a ^ b
not.i32 c, a        # c = ~a
shl.i32 c, a, 3     # c = a << 3
shr.i32 c, a, 2     # c = a >> 2 (logical shift)
sar.i32 c, a, 2     # c = a >> 2 (arithmetic shift)
```

### 4.3 Comparison Operators

```
cmp.i32 a, b        # Compare a and b, set flags
test.i32 a, b       # Test (bitwise AND), set flags

# Branch based on comparison
cmp.i32 a, b
branch less, label  # Branch if a < b
```

### 4.4 Memory Operations

```
var i32 val
var addr.i32 ptr = &val    # Address-of operator

load.i32 val, [ptr]        # Load from memory
store.i32 [ptr], 42        # Store to memory

# Array access
var i32 arr[10]
var i32 index = 5
var i32 value

load.i32 value, [arr + index * 4]  # Load arr[index]
```

## 5. Control Flow

HOIL provides various control flow constructs:

### 5.1 Labels and Branches

```
label_1:              # Define a label
  add.i32 x, x, 1
  cmp.i32 x, 10
  branch less, label_1  # Branch if x < 10
```

### 5.2 Structured Control Flow

HOIL provides higher-level structured control flow that gets translated to labels and branches:

```
# If-else statement
if (x < 10) {
    add.i32 y, y, 1
} else {
    sub.i32 y, y, 1
}

# While loop
while (x < 10) {
    add.i32 x, x, 1
}

# Do-while loop
do {
    add.i32 x, x, 1
} while (x < 10)

# For loop
for (var i32 i = 0; i < 10; add.i32 i, i, 1) {
    add.i32 sum, sum, i
}
```

### 5.3 Switch Statement

```
switch (x) {
    case 1:
        add.i32 y, y, 1
        break
    case 2:
        add.i32 y, y, 2
        break
    default:
        sub.i32 y, y, 1
}
```

## 6. Procedures

Procedures are HOIL's equivalent of functions:

### 6.1 Procedure Declaration

```
procedure add(i32 a, i32 b) -> i32 {
    var i32 result
    add.i32 result, a, b
    return result
}
```

### 6.2 Procedure Attributes

Procedures can have attributes that affect their behavior:

```
procedure [noreturn] exit(i32 code) {
    syscall SYS_EXIT, code
}

procedure [inline] add_inline(i32 a, i32 b) -> i32 {
    var i32 result
    add.i32 result, a, b
    return result
}
```

### 6.3 Procedure Calls

```
var i32 a = 10
var i32 b = 20
var i32 c

call add(a, b) -> c
```

### 6.4 External Procedures

```
# Declaration of an external procedure
extern procedure printf(addr.i8 format, ...) -> i32

# Calling an external procedure
var i32 result
call printf("Value: %d\n", 42) -> result
```

## 7. Data Sections

HOIL allows defining initialized data:

```
data read_only {
    string message = "Hello, COIL!"
    i32 lookup_table[4] = {1, 2, 3, 4}
}

data read_write {
    i32 counter = 0
    f64 values[100] = {1.0, 2.0}  # Rest initialized to 0
}
```

## 8. Modules and Imports

### 8.1 Module Declaration

```
module example
```

### 8.2 Imports

```
import stdio     # Import the entire module
import math.sin  # Import a specific symbol
```

### 8.3 Exports

```
export procedure add  # Export a procedure
export var counter    # Export a variable
```

## 9. Inline Assembly

HOIL allows embedding raw assembly language for specific architectures:

```
asm x86_64 {
    "mov rax, 42"
    "ret"
}
```

## 10. Examples

### 10.1 Simple Function

```
module math

procedure factorial(i32 n) -> i32 {
    var i32 result = 1
    var i32 i = 2
    
    while (i <= n) {
        mul.i32 result, result, i
        add.i32 i, i, 1
    }
    
    return result
}
```

### 10.2 Vector Processing

```
procedure vector_add(v128.f32 a, v128.f32 b) -> v128.f32 {
    var v128.f32 result
    vadd.f32 result, a, b
    return result
}
```

### 10.3 System Interaction

```
module os

import stdio

procedure main() -> i32 {
    var i32 status
    
    call stdio.printf("Enter status code: ")
    call stdio.scanf("%d", &status)
    
    if (status != 0) {
        call stdio.printf("Error: %d\n", status)
    } else {
        call stdio.printf("Success!\n")
    }
    
    return status
}
```

## 11. Special Features

### 11.1 Built-in Functions

HOIL provides built-in functions for common operations:

```
var i32 bits = popcount(value)    # Count set bits
var i32 zeros = countleadzeros(value)  # Count leading zeros
var f64 root = sqrt(value)        # Square root
```

### 11.2 Compiler Hints

```
# Memory access pattern hint
prefetch [array + 64], read

# Branch prediction hint
branch_likely equal, fast_path
branch_unlikely overflow, error_handler
```

### 11.3 SIMD Operations

```
# Load 4 consecutive 32-bit floats into a vector
vload.f32 vec, [data]

# Parallel addition of 4 pairs of floats
vadd.f32 result, vec1, vec2

# Extract the second element
vextract.f32 scalar, vec, 1
```

## 12. Compilation and Assembly

HOIL source code is compiled to COIL binary format using a HOIL assembler. The process typically involves:

1. Parsing HOIL source code
2. Translating HOIL constructs to COIL instructions
3. Performing optimizations
4. Generating COIL binary output

```
hoil-assembler -o output.coil input.hoil
```

The resulting COIL binary can then be executed by a COIL virtual machine or further compiled to native code using a COIL compiler.