# COIL Cross-Architecture Behavior

This document details how COIL provides consistent behavior across different CPU architectures while still enabling architecture-specific optimizations.

## 1. CPU Configuration Integration

COIL's cross-architecture strategy is built on the CPU configuration format, which provides detailed information about the capabilities, constraints, and features of target architectures.

### 1.1 Configuration Processing

When a COIL assembler or compiler processes code, it performs the following steps:

1. **Load CPU Configuration**: The CPU configuration structure is loaded and validated
2. **Feature Detection**: Available hardware features are identified
3. **Instruction Mapping**: COIL instructions are mapped to native capabilities
4. **Expansion Planning**: Fallback sequences are prepared for unsupported instructions
5. **Optimization Selection**: Architecture-appropriate optimizations are chosen

### 1.2 Multiple Target Support

A single COIL toolchain can support multiple target architectures:

```
# Assemble for different architectures
coil-assembler --target=x86_64 input.hoil -o output.x86_64.coil
coil-assembler --target=arm64 input.hoil -o output.arm64.coil
coil-assembler --target=riscv64 input.hoil -o output.riscv64.coil
```

### 1.3 Configuration Sources

CPU configurations can come from various sources:

1. **Built-in Configurations**: Common architectures included with the toolchain
2. **JSON/XML Files**: Separate configuration files for specific platforms
3. **Runtime Detection**: Dynamic detection on the host system
4. **Custom Specifications**: User-provided specifications for specialized hardware

## 2. Instruction Support Across Architectures

### 2.1 Support Matrix

COIL provides a consistent set of instructions across all supported architectures, even when native hardware support varies:

| Instruction | x86_64 | ARM64 | RISC-V | PowerPC | MIPS |
|-------------|--------|-------|--------|---------|------|
| `add.i32`   | Native | Native| Native | Native  | Native |
| `popcnt.i32`| Native | Native| Expanded| Native | Expanded |
| `fma.f64`   | Native | Native| Native | Native  | Expanded |
| `atomic.xchg`| Native| Native| Native | Native  | Expanded |
| `simd.i32x4.add`| Native| Native| Expanded| Native| Expanded |

### 2.2 Instruction Implementation Categories

COIL instructions fall into one of four implementation categories:

1. **Universally Native**: Supported natively on all target architectures
2. **Commonly Native**: Supported natively on most architectures, expanded on others
3. **Rarely Native**: Supported natively on few architectures, expanded on most
4. **Always Expanded**: Never supported natively, always implemented through expansion

### 2.3 Instruction Expansion Examples

Here are examples of how COIL expands instructions on architectures that don't support them natively:

#### Example 1: Population Count (`popcnt`) on RISC-V

```
# Original COIL instruction
popcnt.i32 r1, r2

# Expanded sequence on RISC-V
move.i32 r3, r2    # Copy source to temporary
loadimm.i32 r1, 0  # Initialize result
loop:
  test.i32 r3, r3
  branch zero, end
  and.i32 r4, r3, 1
  add.i32 r1, r1, r4
  shr.i32 r3, r3, 1
  jump loop
end:
```

#### Example 2: Fused Multiply-Add (`fma`) on MIPS without native FMA

```
# Original COIL instruction
fma.f64 r1, r2, r3, r4  # r1 = r2 * r3 + r4

# Expanded sequence on MIPS
fmul.f64 r5, r2, r3
fadd.f64 r1, r5, r4
```

#### Example 3: Vector Addition on Scalar-only Architectures

```
# Original COIL instruction
vadd.f32 v1, v2, v3  # Add 4 pairs of floats

# Expanded sequence on a scalar-only architecture
loadimm.i32 r10, 0   # Initialize counter
loop:
  cmp.i32 r10, 4
  branch greater_equal, end
  
  vextract.f32 r1, v2, r10  # Extract element from v2
  vextract.f32 r2, v3, r10  # Extract element from v3
  fadd.f32 r3, r1, r2       # Add elements
  vinsert.f32 v1, r3, r10   # Insert result into v1
  
  add.i32 r10, r10, 1
  jump loop
end:
```

## 3. Type Handling Across Architectures

### 3.1 Basic Type Mapping

COIL ensures consistent type behavior by mapping COIL types to native types:

| COIL Type | x86_64 | ARM64 | RISC-V | PowerPC |
|-----------|--------|-------|--------|---------|
| `i8`      | byte   | byte  | byte   | byte    |
| `i16`     | word   | halfword | halfword | halfword |
| `i32`     | dword  | word  | word   | word    |
| `i64`     | qword  | dword | dword  | dword   |
| `f32`     | float  | float | float  | float   |
| `f64`     | double | double| double | double  |
| `v128`    | xmm    | neon  | v      | vector  |

### 3.2 Type Emulation

For types that aren't natively supported, COIL provides emulation:

#### Example: 128-bit Integer Operations on 64-bit Architectures

```
# Original COIL instruction
add.i128 r1, r2, r3

# Expanded on 64-bit architecture
add.i64 r1_low, r2_low, r3_low      # Add low 64 bits
addc.i64 r1_high, r2_high, r3_high  # Add high 64 bits with carry
```

### 3.3 Vector Length Adaptation

For vector types, COIL adapts to the target's supported vector lengths:

```
# Original COIL code with scalable vectors
var vscalable.i32 vec

# On AVX2 (x86_64)
# Implemented as 256-bit vectors (8 x i32)

# On SVE (ARM)
# Implemented as scalable vectors (n x i32)

# On scalar architecture
# Emulated with array and loops
```

## 4. Memory Model Adaptation

### 4.1 Memory Ordering Guarantees

COIL provides consistent memory ordering semantics across architectures with different memory models:

| Memory Operation | Strong Ordering (x86) | Weak Ordering (ARM) | Relaxed Ordering (RISC-V) |
|------------------|----------------------|---------------------|---------------------------|
| Regular Load/Store | No fences | Load-Acquire/Store-Release | Fences before/after |
| Atomic Operations | Native | Native + Fences | Native + Fences |
| Explicit Barriers | MFENCE | DMB/DSB | FENCE |

### 4.2 Memory Barrier Insertion

COIL automatically inserts appropriate memory barriers based on the target architecture's memory model:

```
# Original COIL code
atomic.store.i32 [addr], value  # Release semantics

# On x86_64
mov [addr], value  # Already has release semantics

# On ARM
stlr value, [addr]  # Store-release instruction

# On RISC-V
fence rw, w        # Release fence
sw value, [addr]   # Regular store
```

### 4.3 Atomic Operation Implementation

COIL implements atomic operations consistently across architectures:

```
# Original COIL instruction
atomic.cmpxchg.i32 r1, [addr], r2, r3

# On x86_64
mov eax, r2
lock cmpxchg [addr], r3
mov r1, eax

# On ARM64
loop:
  ldaxr w4, [addr]
  cmp w4, w2
  bne fail
  stlxr w5, w3, [addr]
  cbnz w5, loop
  mov w1, w4
  b done
fail:
  mov w1, w4
done:

# On RISC-V
loop:
  fence rw, rw
  lr.w t0, (addr)
  bne t0, a1, fail
  sc.w t1, a2, (addr)
  bnez t1, loop
  mv a0, t0
  j done
fail:
  mv a0, t0
done:
  fence rw, rw
```

## 5. ABI Adaptation

### 5.1 Procedure Call Interface

COIL adapts to different calling conventions and ABIs:

| Aspect | x86_64 System V | ARM64 AAPCS | RISC-V LP64D |
|--------|----------------|-------------|-------------|
| Integer Arguments | rdi, rsi, rdx, rcx, r8, r9 | x0-x7 | a0-a7 |
| FP Arguments | xmm0-xmm7 | v0-v7 | fa0-fa7 |
| Return Value | rax, rdx | x0, x1 | a0, a1 |
| Callee-saved | rbx, rbp, r12-r15 | x19-x28 | s0-s11 |

### 5.2 Stack Frame Layout

COIL adapts stack frames to the target ABI:

```
# x86_64 System V ABI Stack Frame
+-----------------+
| Return Address  |
+-----------------+
| Saved RBP       |
+-----------------+
| Local Variables |
+-----------------+
| Saved Registers |
+-----------------+
| Parameter Area  |
+-----------------+

# ARM64 AAPCS Stack Frame
+-----------------+
| FP, LR          |
+-----------------+
| Saved Registers |
+-----------------+
| Local Variables |
+-----------------+
| Parameter Area  |
+-----------------+
```

### 5.3 Data Structure Layout

COIL ensures consistent data structure layouts while respecting architecture constraints:

```
# Original COIL structure
struct example {
    i32 a
    i64 b
    i8 c
    i16 d
}

# On x86_64 (natural alignment)
a: offset 0, size 4, alignment 4
b: offset 8, size 8, alignment 8
c: offset 16, size 1, alignment 1
d: offset 18, size 2, alignment 2
Total size: 24 bytes

# On ARM (natural alignment)
a: offset 0, size 4, alignment 4
b: offset 8, size 8, alignment 8
c: offset 16, size 1, alignment 1
d: offset 18, size 2, alignment 2
Total size: 24 bytes

# On architecture requiring strict alignment
a: offset 0, size 4, alignment 4
b: offset 8, size 8, alignment 8
c: offset 16, size 1, alignment 1
d: offset 18, size 2, alignment 2
Total size: 24 bytes
```

## 6. Feature Detection and Adaptation

### 6.1 Runtime Feature Detection

COIL can include code that detects available features at runtime:

```
# COIL code with runtime feature detection
if (cpu_has_feature(FEATURE_AVX2)) {
    # Use AVX2 implementation
} else if (cpu_has_feature(FEATURE_SSE4_2)) {
    # Use SSE4.2 fallback
} else {
    # Use scalar fallback
}
```

### 6.2 Multi-Version Generation

COIL can generate multiple versions of the same code optimized for different feature sets:

```
# Generate multiple versions
procedure vector_add(v128.f32 a, v128.f32 b) -> v128.f32 [multiversion] {
    var v128.f32 result
    vadd.f32 result, a, b
    return result
}

# Generates:
# - AVX2 version for x86_64 with AVX2
# - SSE version for x86_64 with SSE
# - NEON version for ARM with NEON
# - Scalar version for architectures without SIMD
```

### 6.3 Specializations

COIL can specialize procedures for specific architectures:

```
# Base implementation
procedure [default] hash(ptr_t data, u64 length) -> u64 {
    # Generic implementation
}

# x86_64 with SSE4.2 specialization
procedure [arch=x86_64, feature=SSE4_2] hash(ptr_t data, u64 length) -> u64 {
    # SSE4.2 optimized implementation using CRC32
}

# ARM64 with CRC specialization
procedure [arch=arm64, feature=CRC] hash(ptr_t data, u64 length) -> u64 {
    # ARM CRC optimized implementation
}
```

## 7. Optimization Across Architectures

### 7.1 Architecture-Specific Optimizations

COIL applies different optimizations based on the target architecture:

| Optimization | x86_64 | ARM64 | RISC-V |
|--------------|--------|-------|--------|
| Loop Unrolling | Based on μops | Based on instruction count | Based on instruction count |
| Prefetching | Use `prefetchnta` | Use `prfm` | Software prefetch |
| Vectorization | Use AVX/SSE | Use NEON/SVE | Use V extension or scalar |
| Memory Access | Consider cache line size (64B) | Consider cache line size (64/128B) | Consider cache line size |

### 7.2 Processor-Specific Tuning

COIL can tune for specific processor implementations within an architecture:

```
# Compile with processor-specific tuning
coil-assembler --target=x86_64 --cpu=skylake input.hoil -o output.coil
coil-assembler --target=arm64 --cpu=cortex-a76 input.hoil -o output.coil
```

### 7.3 Optimization Trade-offs

COIL considers architecture-specific trade-offs:

1. **x86_64**: Prioritize reducing μops, minimize partial register updates
2. **ARM64**: Focus on instruction count, minimize register pressure
3. **RISC-V**: Balance instruction count and register usage
4. **PowerPC**: Optimize for the load/store architecture model

## 8. Cross-Architecture Debugging

### 8.1 Consistent Debug Information

COIL provides consistent debugging information across architectures:

```
# Generate debug information
coil-assembler --debug input.hoil -o output.coil

# Debug with architecture-specific debugger
x86_64-coil-gdb output.coil    # x86_64 debugging
arm64-coil-gdb output.coil     # ARM64 debugging
```

### 8.2 Source-Level Debugging

COIL maps between source code and machine code locations:

```
# Debug mapping information
Source line 42, file.hoil
├── COIL instruction at offset 0x1234
└── Machine instruction at address 0x7FFF00001234
```

### 8.3 Architecture-Neutral Debugging

COIL enables debugging at the HOIL level, abstracted from the target architecture:

```
# Architecture-neutral debugging
coil-debug output.coil

# Commands work the same regardless of target architecture
> break main
> run
> print x
> next
```

## 9. Cross-Architecture Testing and Validation

### 9.1 Validation Test Suite

COIL includes a validation test suite to verify consistent behavior:

```
# Run validation tests on multiple architectures
coil-validate --target=x86_64 test-suite/
coil-validate --target=arm64 test-suite/
coil-validate --target=riscv64 test-suite/
```

### 9.2 Reference Results

Tests include reference results to compare against:

```
# Test with reference results
Input: [1, 2, 3, 4]
Expected output: [2, 4, 6, 8]
```

### 9.3 Cross-Architecture Simulation

COIL can simulate different architectures for testing:

```
# Simulate execution on a different architecture
coil-sim --target=riscv64 output.x86_64.coil
```

## 10. Heterogeneous Computing

### 10.1 Cross-Device Communication

COIL facilitates communication between different computing devices:

```
# Copy data between CPU and GPU
device_mem_copy_to(gpu, gpu_data, cpu_data, size)
device_execute(gpu, kernel, args)
device_mem_copy_from(gpu, result_cpu, result_gpu, result_size)
```

### 10.2 Device Switching

COIL provides mechanisms for switching between devices:

```
# Execute code on different devices
procedure main() {
    # CPU code
    var i32 result = 0
    
    # Switch to GPU
    device_begin(gpu)
    {
        # GPU code
        parallel_for(0, 1000, 1, i => {
            atomic.add(result, i)
        })
    }
    device_end()
    
    # Back to CPU
    io_printf("Result: %d\n", result)
}
```

### 10.3 Heterogeneous Compilation

COIL supports compiling code for multiple devices:

```
# Compile for multiple targets
coil-assembler --target=x86_64,nvidia-sm_80 input.hoil -o output.coil
```

## 11. Cross-Architecture Interface Guidelines

To ensure consistent behavior across architectures, developers should follow these guidelines:

### 11.1 Data Type Usage

```
# Recommended: Use explicit-sized types
var i32 counter
var f64 value

# Avoid: Architecture-dependent types
var int counter  # Size varies by architecture
var float value  # Precision may vary
```

### 11.2 Memory Alignment

```
# Recommended: Respect natural alignment
var i64 [align(8)] aligned_value

# Avoid: Assuming specific alignment
var i64 value  # May cause issues on architectures requiring alignment
```

### 11.3 Endianness Handling

```
# Recommended: Use byte swapping for serialization
var u32 value = read_u32()
var u32 network_value = bswap.u32(value)  # Convert to big-endian

# Avoid: Assuming endianness
var u32 value = read_u32()  # May be little or big endian
```

### 11.4 Vectorization Hints

```
# Recommended: Use vector length-agnostic code
procedure process_array(ptr_t data, u64 length) {
    # Will vectorize appropriately for each architecture
    for (var u64 i = 0; i < length; i++) {
        data[i] = data[i] * 2
    }
}

# Avoid: Assuming specific vector lengths
procedure process_array(ptr_t data, u64 length) {
    # Assumes 4-element vectors
    for (var u64 i = 0; i < length; i += 4) {
        vload.f32 v1, [data + i*4]
        vmul.f32 v1, v1, [2.0, 2.0, 2.0, 2.0]
        vstore.f32 [data + i*4], v1
    }
}
```

## 12. Real-World Examples

### 12.1 Cryptographic Hash Function

A SHA-256 implementation that adapts to different architectures:

```
procedure sha256(ptr_t data, u64 length, ptr_t hash) {
    # Check for hardware acceleration
    if (cpu_has_feature(FEATURE_SHA)) {
        # Use hardware acceleration
        sha256_hw(data, length, hash)
    } else {
        # Use software implementation
        sha256_sw(data, length, hash)
    }
}

# Hardware-accelerated implementation
procedure [feature=SHA] sha256_hw(ptr_t data, u64 length, ptr_t hash) {
    # Architecture-specific hardware acceleration
    # - x86_64: Uses SHA extensions
    # - ARM64: Uses Crypto extensions
    # - RISC-V: Uses K extensions if available
}

# Software implementation
procedure sha256_sw(ptr_t data, u64 length, ptr_t hash) {
    # Portable software implementation
    # Uses COIL's guaranteed instructions only
}
```

### 12.2 Matrix Multiplication

A matrix multiplication routine that adapts to different architectures:

```
procedure matrix_multiply(ptr_t a, ptr_t b, ptr_t c, u32 m, u32 n, u32 k) {
    # Select implementation based on architecture and features
    if (cpu_has_feature(FEATURE_AVX512)) {
        matrix_multiply_avx512(a, b, c, m, n, k)
    } else if (cpu_has_feature(FEATURE_NEON)) {
        matrix_multiply_neon(a, b, c, m, n, k)
    } else if (cpu_has_feature(FEATURE_VECTOR)) {
        matrix_multiply_rvv(a, b, c, m, n, k)
    } else {
        matrix_multiply_scalar(a, b, c, m, n, k)
    }
}

# Scalar implementation (works on all architectures)
procedure matrix_multiply_scalar(ptr_t a, ptr_t b, ptr_t c, u32 m, u32 n, u32 k) {
    # Three nested loops with scalar operations
}

# x86_64 with AVX-512
procedure [feature=AVX512] matrix_multiply_avx512(ptr_t a, ptr_t b, ptr_t c, u32 m, u32 n, u32 k) {
    # Implementation using 512-bit vectors
}

# ARM64 with NEON
procedure [feature=NEON] matrix_multiply_neon(ptr_t a, ptr_t b, ptr_t c, u32 m, u32 n, u32 k) {
    # Implementation using 128-bit NEON vectors
}

# RISC-V with Vector extension
procedure [feature=VECTOR] matrix_multiply_rvv(ptr_t a, ptr_t b, ptr_t c, u32 m, u32 n, u32 k) {
    # Implementation using scalable vectors
}
```

### 12.3 Memory Copy

A memory copy routine optimized for different architectures:

```
procedure mem_copy(ptr_t dest, ptr_t src, u64 size) {
    # Small sizes - use simple loop
    if (size < 16) {
        for (var u64 i = 0; i < size; i++) {
            store.u8 [dest + i], load.u8 [src + i]
        }
        return
    }
    
    # Ensure destination is aligned
    var u64 align_offset = 8 - (cast.u64(dest) & 7)
    if (align_offset != 8) {
        for (var u64 i = 0; i < align_offset; i++) {
            store.u8 [dest + i], load.u8 [src + i]
        }
        dest += align_offset
        src += align_offset
        size -= align_offset
    }
    
    # Use vector operations for bulk transfer
    var u64 vector_size = 32
    var u64 vector_count = size / vector_size
    for (var u64 i = 0; i < vector_count; i++) {
        vload.u8 v1, [src + i * vector_size]
        vstore.u8 [dest + i * vector_size], v1
    }
    
    # Handle remaining bytes
    var u64 remaining = size % vector_size
    var u64 offset = vector_count * vector_size
    for (var u64 i = 0; i < remaining; i++) {
        store.u8 [dest + offset + i], load.u8 [src + offset + i]
    }
}
```

This implementation adapts to different architectures:
- On x86_64, vector operations use AVX/SSE instructions
- On ARM64, vector operations use NEON instructions
- On RISC-V, vector operations use Vector extension or scalar fallback
- The alignment handling works consistently across all architectures