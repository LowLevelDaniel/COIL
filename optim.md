# COIL Optimization Techniques

This document describes the optimization techniques used in COIL to produce efficient code across different target architectures.

## 1. Overview

COIL's optimization system operates at multiple levels:

1. **HOIL-level Optimizations**: Applied to the source code representation
2. **COIL-level Optimizations**: Applied to the intermediate representation
3. **Architecture-Specific Optimizations**: Applied based on target architecture
4. **Link-Time Optimizations**: Applied when linking multiple modules
5. **Runtime Optimizations**: Applied during execution (for JIT compilation)

## 2. Optimization Pipeline

The COIL optimization pipeline processes code through multiple stages:

```
HOIL Source Code
      │
      ▼
   Parsing
      │
      ▼
 AST Generation
      │
      ▼
Semantic Analysis
      │
      ▼
 HOIL-level Opts
      │
      ▼
  IR Generation
      │
      ▼
 COIL-level Opts
      │
      ▼
Native Code Generation
      │
      ▼
Architecture-Specific Opts
      │
      ▼
  Object Files
      │
      ▼
Link-Time Opts
      │
      ▼
Final Executable
```

## 3. HOIL-Level Optimizations

### 3.1 Constant Folding and Propagation

Evaluates constant expressions at compile time and propagates the results:

```
# Before optimization
var i32 a = 10
var i32 b = 20
var i32 c = a + b

# After optimization
var i32 a = 10
var i32 b = 20
var i32 c = 30
```

### 3.2 Dead Code Elimination

Removes code that has no effect on the program's output:

```
# Before optimization
var i32 a = 10
var i32 b = 20
var i32 c = a + b
var i32 d = 30  # Never used

# After optimization
var i32 a = 10
var i32 b = 20
var i32 c = a + b
```

### 3.3 Common Subexpression Elimination

Identifies and eliminates redundant computations:

```
# Before optimization
var i32 a = x * y + z
var i32 b = x * y + w

# After optimization
var i32 temp = x * y
var i32 a = temp + z
var i32 b = temp + w
```

### 3.4 Loop Optimizations

#### 3.4.1 Loop Invariant Code Motion

Moves computations outside of loops when their results don't change:

```
# Before optimization
for (var i32 i = 0; i < n; i++) {
    var i32 x = a * b  # Invariant
    arr[i] = x + i
}

# After optimization
var i32 x = a * b  # Moved outside loop
for (var i32 i = 0; i < n; i++) {
    arr[i] = x + i
}
```

#### 3.4.2 Loop Unrolling

Replicates loop bodies to reduce loop control overhead:

```
# Before optimization
for (var i32 i = 0; i < 4; i++) {
    arr[i] = arr[i] * 2
}

# After optimization (full unrolling)
arr[0] = arr[0] * 2
arr[1] = arr[1] * 2
arr[2] = arr[2] * 2
arr[3] = arr[3] * 2
```

#### 3.4.3 Loop Fusion

Combines adjacent loops with the same iteration space:

```
# Before optimization
for (var i32 i = 0; i < n; i++) {
    a[i] = b[i] * 2
}
for (var i32 i = 0; i < n; i++) {
    c[i] = a[i] + d[i]
}

# After optimization
for (var i32 i = 0; i < n; i++) {
    a[i] = b[i] * 2
    c[i] = a[i] + d[i]
}
```

### 3.5 Function Inlining

Replaces function calls with the function body:

```
# Before optimization
procedure add(i32 a, i32 b) -> i32 {
    return a + b
}

procedure main() {
    var i32 x = add(10, 20)
}

# After optimization
procedure main() {
    var i32 x = 10 + 20  # Inlined and constant-folded
}
```

## 4. COIL-Level Optimizations

### 4.1 Register Allocation

Assigns variables to registers to minimize memory access:

```
# Before register allocation
var i32 a = load.i32 [x]
var i32 b = load.i32 [y]
var i32 c = add.i32 a, b
store.i32 [z], c

# After register allocation (pseudo-code)
R1 = load.i32 [x]
R2 = load.i32 [y]
R1 = add.i32 R1, R2  # Reuse R1 for result
store.i32 [z], R1
```

### 4.2 Instruction Selection

Chooses the optimal instructions for the target architecture:

```
# Generic COIL
mul.i32 c, a, 2

# x86_64 (using LEA for multiplication by 2)
lea ecx, [eax + eax]  # c = a * 2

# ARM (using shift for multiplication by 2)
lsl w2, w0, #1  # c = a * 2
```

### 4.3 Instruction Scheduling

Reorders instructions to minimize pipeline stalls:

```
# Before scheduling
load.i32 r1, [addr1]  # 5-cycle latency
add.i32 r3, r1, r2    # Depends on r1
load.i32 r4, [addr2]  # Independent 5-cycle latency
add.i32 r5, r4, r6    # Depends on r4

# After scheduling
load.i32 r1, [addr1]  # Start load 1
load.i32 r4, [addr2]  # Start load 2 (overlap latency)
add.i32 r3, r1, r2    # Use result from load 1
add.i32 r5, r4, r6    # Use result from load 2
```

### 4.4 Peephole Optimizations

Applies small, local transformations to improve code:

```
# Before optimization
not.i32 r1, r2
not.i32 r3, r1

# After optimization (double negation elimination)
move.i32 r3, r2
```

### 4.5 Strength Reduction

Replaces expensive operations with cheaper equivalents:

```
# Before optimization
mul.i32 r1, r2, 16

# After optimization
shl.i32 r1, r2, 4  # Shift left by 4 is equivalent to multiply by 16
```

### 4.6 Dead Store Elimination

Removes stores that are overwritten before being read:

```
# Before optimization
store.i32 [addr], r1
store.i32 [addr], r2  # Overwrites previous store

# After optimization
store.i32 [addr], r2  # Only the second store is needed
```

## 5. Architecture-Specific Optimizations

### 5.1 Instruction Set Extensions

Utilizes architecture-specific extensions when available:

```
# Generic COIL
popcnt.i32 r1, r2

# x86_64 with POPCNT
popcnt eax, ebx  # Native instruction

# ARM64 with ARMv8.0 extensions
cnt v0.8b, v1.8b   # Count set bits
addv b0, v0.8b    # Add elements
umov w0, v0.b[0]  # Move to general purpose register
```

### 5.2 SIMD Vectorization

Transforms scalar operations into vector operations:

```
# Scalar loop
for (var i32 i = 0; i < n; i++) {
    c[i] = a[i] + b[i]
}

# Vectorized for x86_64 with AVX
for (var i32 i = 0; i < n; i += 8) {
    var v256.f32 va = vload.f32 [a + i*4]
    var v256.f32 vb = vload.f32 [b + i*4]
    var v256.f32 vc = vadd.f32 va, vb
    vstore.f32 [c + i*4], vc
}
```

### 5.3 Cache Optimization

Optimizes code for better cache utilization:

```
# Before optimization (bad cache locality)
for (var i32 i = 0; i < n; i++) {
    for (var i32 j = 0; j < n; j++) {
        sum += matrix[j][i]  # Strided access
    }
}

# After optimization (improved cache locality)
for (var i32 j = 0; j < n; j++) {
    for (var i32 i = 0; i < n; i++) {
        sum += matrix[j][i]  # Sequential access
    }
}
```

### 5.4 Branch Prediction Hints

Adds architecture-specific branch prediction hints:

```
# Generic COIL
branch equal, label

# x86_64 with likely hint
jz label  # No direct hint, but can use layout

# ARM64 with likely hint
b.eq label  # Can use CSEL for predictable branches
```

### 5.5 Architecture-Specific Intrinsics

Provides access to architecture-specific features:

```
# x86_64 RDTSC intrinsic
var u64 cycles = intrinsic.x86.rdtsc()

# ARM64 CRC32 intrinsic
var u32 crc = intrinsic.arm.crc32w(initial, value)
```

## 6. Link-Time Optimizations

### 6.1 Interprocedural Analysis

Analyzes and optimizes across procedure boundaries:

```
# Before LTO (in separate modules)
// Module A
export procedure increment(i32 x) -> i32 {
    return x + 1
}

// Module B
import procedure increment(i32) -> i32
procedure main() {
    var i32 y = increment(10)
}

# After LTO
procedure main() {
    var i32 y = 11  # Inlined, constant propagated, and folded
}
```

### 6.2 Global Variable Optimization

Optimizes usage of global variables:

```
# Before LTO
// Module A
export var i32 counter = 42

// Module B
import var i32 counter
procedure print_counter() {
    io_printf("Counter: %d\n", counter)
}

# After LTO (if counter is not modified elsewhere)
procedure print_counter() {
    io_printf("Counter: %d\n", 42)  # Counter value propagated
}
```

### 6.3 Code Layout Optimization

Optimizes the layout of code for better cache behavior:

1. **Function Grouping**: Places frequently called functions close together
2. **Hot/Cold Splitting**: Separates hot code paths from cold ones
3. **Alignment**: Aligns functions to cache line boundaries

### 6.4 Whole-Program Optimization

Performs optimizations considering the entire program:

1. **Unused Function Elimination**: Removes functions that are never called
2. **Unused Global Elimination**: Removes unused global variables
3. **Constant Propagation**: Propagates constants throughout the program
4. **Type Specialization**: Specializes functions for specific argument types

## 7. Runtime Optimizations

### 7.1 Just-In-Time Compilation

For COIL virtual machines with JIT capabilities:

1. **Hotspot Detection**: Identifies frequently executed code regions
2. **Runtime Type Information**: Uses observed types for specialization
3. **Speculative Optimizations**: Makes optimistic assumptions with fallbacks
4. **Adaptive Recompilation**: Recompiles code based on runtime behavior

### 7.2 Profile-Guided Optimization

Uses profile data collected from previous runs:

```
# Collect profile data
coil-run --profile profile.data program.coil

# Use profile data for optimization
coil-assembler --profile-data=profile.data input.hoil -o optimized.coil
```

## 8. Optimization Levels

COIL defines several standard optimization levels:

### 8.1 Level 0 (O0)

- No optimizations
- Fastest compilation
- Best for debugging
- Direct mapping from HOIL to COIL

### 8.2 Level 1 (O1)

- Basic optimizations
- Fast compilation
- Moderate code quality
- Includes:
  - Constant folding
  - Dead code elimination
  - Simple register allocation
  - Basic peephole optimizations

### 8.3 Level 2 (O2)

- Comprehensive optimizations
- Balanced compilation speed and code quality
- Default level for most applications
- Includes O1 plus:
  - Common subexpression elimination
  - Loop optimizations
  - Function inlining
  - Advanced register allocation
  - Basic vectorization

### 8.4 Level 3 (O3)

- Aggressive optimizations
- Slower compilation
- Highest code quality
- Includes O2 plus:
  - Loop unrolling
  - Aggressive inlining
  - Advanced vectorization
  - Interleaving
  - Aggressive instruction scheduling

### 8.5 Size Optimization (Os)

- Optimizes for code size
- Moderate performance
- Includes:
  - O2 optimizations that don't increase code size
  - Function merging
  - Code factoring
  - Instruction encoding optimization

## 9. Optimization Control

### 9.1 Compiler Flags

Controls optimization behavior through flags:

```
# Set optimization level
coil-assembler -O2 input.hoil -o output.coil

# Enable/disable specific optimizations
coil-assembler -O2 --enable-inline --disable-unroll input.hoil -o output.coil
```

### 9.2 Pragma Directives

Controls optimization behavior within the source code:

```
#pragma optimize(level=3)
procedure hot_function() {
    // Optimized at O3 level
}

#pragma optimize(inline)
procedure always_inline(i32 x) -> i32 {
    return x + 1
}

#pragma optimize(no_vectorize)
procedure no_vector_loop() {
    // Loop that shouldn't be vectorized
}
```

### 9.3 Attributes

Controls optimization behavior through attributes:

```
procedure [inline] fast_function(i32 x) -> i32 {
    return x * 2
}

procedure [no_inline] complex_function(i32 x) -> i32 {
    // Complex implementation that shouldn't be inlined
}
```

## 10. Optimization Metrics

### 10.1 Performance Metrics

Measures to evaluate optimization effectiveness:

1. **Execution Time**: Total time to execute the program
2. **Throughput**: Operations per second
3. **Latency**: Time to complete a specific operation
4. **Cache Behavior**: Cache hit/miss rates
5. **Branch Prediction**: Branch prediction accuracy

### 10.2 Size Metrics

Measures related to code size:

1. **Binary Size**: Total size of the executable
2. **Instruction Count**: Number of machine instructions
3. **Instruction Bytes**: Size of instruction encoding
4. **Data Size**: Size of global data

### 10.3 Optimization Reports

Reports generated to analyze optimization results:

```
# Generate optimization report
coil-assembler --report-opt=report.txt input.hoil -o output.coil
```

Example report content:
```
Optimization Report for input.hoil
==================================

Function: main
  Inlined calls: 3
  Loop optimizations:
    - Loop at line 42: Unrolled (factor: 4)
    - Loop at line 67: Vectorized (4 elements per iteration)
  Register allocation:
    - 12 variables mapped to 6 registers
    - 2 variables spilled to stack

Function: calculate
  Inlined calls: 0
  Branch optimizations:
    - Branch at line 83: Predicted as usually taken
  SIMD optimizations:
    - Vector operation at line 95: Used AVX2 instructions
```

## 11. Case Studies

### 11.1 Matrix Multiplication Optimization

Demonstrates multiple optimization techniques applied to matrix multiplication:

```
# Original HOIL code
procedure matrix_multiply(ptr_t a, ptr_t b, ptr_t c, i32 m, i32 n, i32 k) {
    for (var i32 i = 0; i < m; i++) {
        for (var i32 j = 0; j < n; j++) {
            var f32 sum = 0.0
            for (var i32 l = 0; l < k; l++) {
                sum += a[i*k + l] * b[l*n + j]
            }
            c[i*n + j] = sum
        }
    }
}
```

Optimization steps:
1. **Loop Reordering**: Adjust loop nesting for better cache locality
2. **Tiling**: Break loops into tiles that fit in cache
3. **Vectorization**: Use SIMD instructions for inner computations
4. **Unrolling**: Unroll inner loops to reduce overhead
5. **Prefetching**: Add prefetch instructions for upcoming data

```
# Optimized HOIL code (conceptual)
procedure matrix_multiply(ptr_t a, ptr_t b, ptr_t c, i32 m, i32 n, i32 k) {
    // Constants for tiling
    const i32 TILE_M = 64
    const i32 TILE_N = 64
    const i32 TILE_K = 64
    
    // Tiled loops
    for (var i32 i0 = 0; i0 < m; i0 += TILE_M) {
        for (var i32 j0 = 0; j0 < n; j0 += TILE_N) {
            for (var i32 l0 = 0; l0 < k; l0 += TILE_K) {
                // Tile boundaries
                var i32 i_end = min(i0 + TILE_M, m)
                var i32 j_end = min(j0 + TILE_N, n)
                var i32 l_end = min(l0 + TILE_K, k)
                
                // Process tile
                for (var i32 i = i0; i < i_end; i++) {
                    for (var i32 j = j0; j < j_end; j += 8) {
                        // Initialize accumulator vectors
                        var v256.f32 sum0 = vbroadcast.f32(0.0)
                        var v256.f32 sum1 = vbroadcast.f32(0.0)
                        
                        // Prefetch next iteration
                        prefetch [a + i*k + l_end], read
                        
                        // Vector inner loop
                        for (var i32 l = l0; l < l_end; l++) {
                            var v256.f32 a_vec = vbroadcast.f32(a[i*k + l])
                            var v256.f32 b_vec0 = vload.f32 [b + l*n + j]
                            var v256.f32 b_vec1 = vload.f32 [b + l*n + j + 4]
                            
                            // FMA operations
                            sum0 = vfma.f32 sum0, a_vec, b_vec0
                            sum1 = vfma.f32 sum1, a_vec, b_vec1
                        }
                        
                        // Store results
                        vstore.f32 [c + i*n + j], sum0
                        vstore.f32 [c + i*n + j + 4], sum1
                    }
                }
            }
        }
    }
}
```

### 11.2 String Search Optimization

Demonstrates optimizations for string search:

```
# Original HOIL code
procedure find_substring(ptr_t haystack, i32 haystack_len, ptr_t needle, i32 needle_len) -> i32 {
    for (var i32 i = 0; i <= haystack_len - needle_len; i++) {
        var bool match = true
        for (var i32 j = 0; j < needle_len; j++) {
            if (haystack[i + j] != needle[j]) {
                match = false
                break
            }
        }
        if (match) {
            return i
        }
    }
    return -1
}
```

Optimization steps:
1. **Boyer-Moore Algorithm**: Skip characters based on pattern
2. **SIMD Comparison**: Use vector instructions for parallel comparison
3. **Branch Elimination**: Use predication/masking instead of branches
4. **Architecture-specific**: Use string instructions on x86

```
# Optimized HOIL code for x86_64 (simplified)
procedure find_substring(ptr_t haystack, i32 haystack_len, ptr_t needle, i32 needle_len) -> i32 {
    // Special case for short needle
    if (needle_len <= 16) {
        // Use SSE/AVX for parallel comparison
        var v128.i8 first_char = vbroadcast.i8(needle[0])
        var v128.i8 needle_vec = vload.i8 [needle]
        
        for (var i32 i = 0; i <= haystack_len - 16; i += 16) {
            var v128.i8 window = vload.i8 [haystack + i]
            var v128.i8 eq = vcmpeq.i8 window, first_char
            var i32 mask = vmovmask.i8 eq
            
            while (mask != 0) {
                var i32 pos = ctz.i32 mask
                mask = mask & (mask - 1)  // Clear lowest set bit
                
                if (i + pos + needle_len <= haystack_len) {
                    var v128.i8 candidate = vload.i8 [haystack + i + pos]
                    var v128.i8 eq2 = vcmpeq.i8 candidate, needle_vec
                    var i32 eq_mask = vmovmask.i8 eq2
                    
                    if ((eq_mask & ((1 << needle_len) - 1)) == ((1 << needle_len) - 1)) {
                        return i + pos
                    }
                }
            }
        }
        
        // Handle remaining bytes
        for (var i32 i = haystack_len - 16; i <= haystack_len - needle_len; i++) {
            // ...
        }
    } else {
        // Use Boyer-Moore algorithm for longer strings
        // ...
    }
    
    return -1
}
```

## 12. Advanced Optimization Techniques

### 12.1 Automatic Vectorization

Analyzes loops and automatically transforms them to use vector instructions:

```
# Original scalar loop
for (var i32 i = 0; i < n; i++) {
    c[i] = a[i] + b[i]
}

# Compiler-vectorized loop (internally)
for (var i32 i = 0; i < n; i += 4) {
    var v128.f32 va = vload.f32 [a + i*4]
    var v128.f32 vb = vload.f32 [b + i*4]
    var v128.f32 vc = vadd.f32 va, vb
    vstore.f32 [c + i*4], vc
}
```

Vectorization heuristics:
1. **Loop Structure**: Simple for-loops with known trip counts
2. **Data Dependencies**: No cross-iteration dependencies
3. **Memory Access**: Sequential and aligned access patterns
4. **Operation Types**: Operations that have vector equivalents

### 12.2 Auto-Parallelization

Automatically identifies and parallelizes independent loops:

```
# Original sequential loop
for (var i32 i = 0; i < n; i++) {
    result[i] = expensive_computation(data[i])
}

# Compiler-parallelized loop (internally)
parallel_for(0, n, 1, i => {
    result[i] = expensive_computation(data[i])
})
```

Parallelization heuristics:
1. **Loop Independence**: No cross-iteration dependencies
2. **Work Amount**: Sufficient work to overcome threading overhead
3. **Memory Patterns**: Non-conflicting memory access patterns

### 12.3 Speculative Optimization

Performs optimizations based on likely but unproven assumptions:

```
# Original code with potential alias
procedure transform(ptr_t a, ptr_t b, i32 n) {
    for (var i32 i = 0; i < n; i++) {
        a[i] = a[i] + b[i]
    }
}

# Speculatively optimized (if a and b don't overlap)
procedure transform(ptr_t a, ptr_t b, i32 n) {
    // Vectorized assuming no overlap
    for (var i32 i = 0; i < n; i += 4) {
        var v128.f32 va = vload.f32 [a + i*4]
        var v128.f32 vb = vload.f32 [b + i*4]
        var v128.f32 vr = vadd.f32 va, vb
        vstore.f32 [a + i*4], vr
    }
    
    // Runtime check for overlap
    if (potentially_overlapping(a, b, n)) {
        // Call non-optimized version as fallback
        transform_safe(a, b, n)
    }
}
```

### 12.4 Memory Hierarchy Optimization

Optimizes code considering the memory hierarchy:

1. **Data Layout Transformation**: Reorganizes data structures for better access
2. **Software Prefetching**: Inserts prefetch instructions
3. **Cache Blocking/Tiling**: Restructures loops to fit data in cache
4. **Memory Access Coalescing**: Combines multiple memory accesses

```
# Original struct-of-arrays layout
struct Data {
    f32 x[1000]
    f32 y[1000]
    f32 z[1000]
}

# Transformed array-of-structs layout for better locality
struct Point {
    f32 x
    f32 y
    f32 z
}
struct OptimizedData {
    Point points[1000]
}
```

### 12.5 Hardware-Specific Tuning

Adapts code to specific hardware characteristics:

1. **Instruction Latency/Throughput**: Schedules instructions based on pipeline characteristics
2. **Cache Line Size**: Aligns data to cache line boundaries
3. **Branch Predictor Behavior**: Arranges branches to match predictor behavior
4. **Prefetcher Patterns**: Structures access patterns to trigger hardware prefetching

```
# Cache-aligned data structure
struct [align(64)] AlignedData {
    f32 values[16]  # 64 bytes = one cache line on many architectures
}
```

### 12.6 Domain-Specific Optimizations

Applies optimizations specific to certain domains:

1. **Graphics Processing**: Optimizations for shaders and rendering
2. **Signal Processing**: FFT and filter optimizations
3. **Machine Learning**: Neural network operation optimizations
4. **Cryptography**: Optimizations for crypto algorithms

```
# Domain-specific intrinsic for fast inverse square root
procedure fast_inverse_sqrt(f32 x) -> f32 {
    // Domain-specific optimization
    return intrinsic.fast_inv_sqrt(x)
}
```