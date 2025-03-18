# COIL Cross-Platform Capabilities

## Introduction

One of COIL's primary design goals is to provide robust cross-platform capabilities that enable code to be written once and deployed across multiple architectures and processing units. This document explains the mechanisms that enable COIL's cross-platform features, provides concrete examples of cross-platform code, and outlines best practices for cross-platform development using COIL.

## Core Cross-Platform Mechanisms

COIL achieves its cross-platform capabilities through several key mechanisms:

### 1. Virtual Register System

The virtual register system provides a consistent interface to physical registers across different architectures:

```c
/**
 * @brief Virtual to physical register mapping
 */
typedef struct coil_register_mapping {
  uint8_t vreg_id;           /**< Virtual register ID */
  uint8_t preg_id;           /**< Physical register ID */
  uint8_t preg_class;        /**< Physical register class */
  uint8_t flags;             /**< Mapping flags */
} coil_register_mapping_t;
```

This mapping allows COIL code to use consistent register references (R0, R1, etc.) regardless of the target architecture, while the COIL assembler handles the mapping to the appropriate physical registers.

### 2. Configuration-Driven Architecture

The configuration system defines the capabilities and constraints of target architectures:

```c
/**
 * @brief Target configuration entry
 */
typedef struct coil_target_config {
  uint32_t target_id;        /**< Target identifier */
  uint8_t  arch_class;       /**< Architecture class */
  uint8_t  arch_type;        /**< Architecture type */
  uint8_t  word_size;        /**< Word size in bits (8, 16, 32, 64) */
  uint8_t  endianness;       /**< Byte ordering (little, big) */
  uint32_t features;         /**< Feature flags */
  uint32_t extensions;       /**< Extension flags */
} coil_target_config_t;
```

This configuration information enables the COIL assembler to make appropriate decisions about code generation for each target architecture.

### 3. Variable Management System

The variable system abstracts away the details of register allocation and memory layout:

```c
/**
 * @brief Variable declaration
 */
typedef struct coil_variable {
  uint8_t  var_id;           /**< Variable identifier */
  uint8_t  storage_class;    /**< Storage class */
  uint16_t type_id;          /**< Type identifier */
  uint32_t flags;            /**< Variable flags */
  uint32_t lifetime_start;   /**< Lifetime start position */
  uint32_t lifetime_end;     /**< Lifetime end position */
} coil_variable_t;
```

This abstraction allows COIL code to use variables in a consistent manner regardless of the target architecture, while the COIL assembler handles the mapping to appropriate registers or memory locations.

### 4. Target Switching Mechanism

The target switching mechanism enables explicit transitions between different architectures or processing units:

```c
/**
 * @brief Target switch instruction
 */
typedef struct coil_target_switch {
  uint32_t source_target;    /**< Source target ID */
  uint32_t dest_target;      /**< Destination target ID */
  uint32_t entry_point;      /**< Entry point in destination target */
  uint32_t return_point;     /**< Return point in source target */
  uint32_t data_mapping;     /**< Data mapping information */
} coil_target_switch_t;
```

This mechanism allows COIL code to explicitly control when and how execution transitions between different architectures or processing units.

### 5. ABI Abstraction

The ABI system defines how functions interface with each other and with the operating system:

```c
/**
 * @brief ABI definition
 */
typedef struct coil_abi {
  uint32_t abi_id;           /**< ABI identifier */
  uint8_t  calling_convention; /**< Calling convention */
  uint8_t  arg_passing;      /**< Argument passing method */
  uint8_t  return_value;     /**< Return value method */
  uint8_t  stack_alignment;  /**< Stack alignment */
  uint32_t name_offset;      /**< Name offset in string table */
} coil_abi_t;
```

This abstraction ensures that function calls work correctly across different target architectures, even when they have different calling conventions.

## Cross-Platform Development Workflow

The typical workflow for cross-platform development using COIL involves:

1. **Writing COIL Code**: Write COIL code using virtual registers, variables, and other abstractions.
2. **Defining Target Configurations**: Define the target architectures and their capabilities.
3. **Compiling for Multiple Targets**: Compile the COIL code for each target architecture.
4. **Linking and Packaging**: Link the compiled code and package it into a unified binary.
5. **Runtime Target Selection**: At runtime, select the appropriate code path based on the available hardware.

## Cross-Architecture Code Examples

### Example 1: Simple Function for Multiple Architectures

```
DIR HINT add_numbers FUNC GLOBAL
DIR LABEL add_numbers
  ; Function prologue
  FRAME ENTER

  ; Declare parameters
  VAR DECL $0 : int32  ; First parameter
  VAR DECL $1 : int32  ; Second parameter
  
  ; Compute sum
  MATH ADD R0, $0, $1
  
  ; Function epilogue
  VAR DLT $1
  VAR DLT $0
  FRAME LEAVE
  CF RET
DIR HINT add_numbers ENDFUNC
```

This simple function works across different architectures because:
- It uses virtual registers (R0) that map to different physical registers on each architecture
- It uses variables ($0, $1) that are allocated to registers or memory by the COIL assembler
- It uses standard COIL instructions (MATH ADD) that are implemented for each target architecture

### Example 2: Architecture-Specific Optimizations

```
DIR HINT vector_add FUNC GLOBAL
DIR LABEL vector_add
  ; Function prologue
  FRAME ENTER

  ; Declare parameters
  VAR DECL $0 : ptr(fp32)   ; First vector pointer
  VAR DECL $1 : ptr(fp32)   ; Second vector pointer
  VAR DECL $2 : ptr(fp32)   ; Result vector pointer
  VAR DECL $3 : int32       ; Vector length
  
  ; Vector implementation - COIL automatically selects the best 
  ; implementation based on available architecture capabilities
  VAR DECL $4 : vec256(fp32)  ; Vector register
  
  ; Process the vector - COIL will use AVX on x86-64, NEON on ARM64, or other
  ; appropriate SIMD instructions based on target architecture
  VEC LOAD $4, [$0]
  VEC ADD $4, $4, [$1]
  VEC STORE [$2], $4
  
  ; For architectures without vector support, COIL automatically
  ; expands these instructions into equivalent scalar operations
  
  CF BR function_end
  
  ; Alternative scalar implementation if needed for other reasons
  DIR LABEL scalar_implementation
  {
    ; Generic scalar implementation that works on any architecture
    VAR DECL $5 : int32 = 0     ; Loop counter
    ...
    ; Loop through vectors and add elements one by one
    DIR LABEL loop_start
    BIT CMP $5, $3
    CF BRC GE loop_end
    
    MEM MOV R0, [$0 + $5*4]
    MEM MOV R1, [$1 + $5*4]
    MATH ADD R0, R0, R1
    MEM MOV [$2 + $5*4], R0
    
    MATH ADD $5, $5, 1
    CF BR loop_start
    
    DIR LABEL loop_end
  }
  
  ; Function epilogue
  DIR LABEL function_end
  VAR DLT $5
  VAR DLT $4
  VAR DLT $3
  VAR DLT $2
  VAR DLT $1
  VAR DLT $0
  FRAME LEAVE
  CF RET
DIR HINT vector_add ENDFUNC
```

This example demonstrates:
- Architecture detection using `DIR TARGET QUERY`
- Architecture-specific code sections with `DIR TARGET arch`
- Fallback implementation for architectures without specific optimizations
- Consistent interface across all target architectures

### Example 3: Heterogeneous Computing (CPU/GPU)

```
DIR HINT matrix_multiply FUNC GLOBAL
DIR LABEL matrix_multiply
  ; Function prologue
  FRAME ENTER

  ; Declare parameters
  VAR DECL $0 : ptr(fp32)   ; Matrix A pointer
  VAR DECL $1 : int32       ; Matrix A rows
  VAR DECL $2 : int32       ; Matrix A columns
  VAR DECL $3 : ptr(fp32)   ; Matrix B pointer
  VAR DECL $4 : int32       ; Matrix B columns
  VAR DECL $5 : ptr(fp32)   ; Result matrix pointer
  
  ; COIL can determine at compile time if GPU target is available
  ; If no GPU target is specified or available at runtime, the CPU implementation is used
  
  ; GPU implementation
  DIR TARGET gpu
  {
    ; Prepare matrices for GPU
    VAR DECL $7 : ptr(fp32)   ; GPU Matrix A
    VAR DECL $8 : ptr(fp32)   ; GPU Matrix B
    VAR DECL $9 : ptr(fp32)   ; GPU Result matrix
    
    ; Allocate GPU memory
    CF SYSC abi-gpu (GPU_ALLOC, $1*$2*4) -> ($7)
    CF SYSC abi-gpu (GPU_ALLOC, $2*$4*4) -> ($8)
    CF SYSC abi-gpu (GPU_ALLOC, $1*$4*4) -> ($9)
    
    ; Copy matrices to GPU
    CF SYSC abi-gpu (GPU_COPY_TO, $0, $7, $1*$2*4) -> ()
    CF SYSC abi-gpu (GPU_COPY_TO, $3, $8, $2*$4*4) -> ()
    
    ; Switch to GPU for computation
    CF SWITCH gpu gpu_matrix_multiply MAP($1->$1, $2->$2, $4->$4, $7->$0, $8->$3, $9->$5)
    
    ; Copy result back to CPU
    CF SYSC abi-gpu (GPU_COPY_FROM, $9, $5, $1*$4*4) -> ()
    
    ; Free GPU memory
    CF SYSC abi-gpu (GPU_FREE, $7) -> ()
    CF SYSC abi-gpu (GPU_FREE, $8) -> ()
    CF SYSC abi-gpu (GPU_FREE, $9) -> ()
    
    CF BR function_end
  }
  
  ; CPU implementation
  DIR LABEL cpu_implementation
  {
    ; Generic CPU implementation
    VAR DECL $7 : int32 = 0    ; Row index
    VAR DECL $8 : int32 = 0    ; Column index
    VAR DECL $9 : int32 = 0    ; Inner index
    ...
    ; Triple nested loop for matrix multiplication
    ...
  }
  
  ; GPU implementation (separate target)
  DIR TARGET gpu
  DIR LABEL gpu_matrix_multiply
  {
    ; GPU kernel for matrix multiplication
    ...
    ; Use GPU-specific instructions for efficient implementation
    ...
    ; Return to CPU
    CF RETURN
  }
  
  ; Function epilogue
  DIR LABEL function_end
  ...
  FRAME LEAVE
  CF RET
DIR HINT matrix_multiply ENDFUNC
```

This example demonstrates:
- Detection of available processing units
- Memory allocation and data transfer between CPU and GPU
- Target switching with explicit data mapping
- Architecture-specific implementations with shared interface

## Cross-Architecture Binary Structure

When compiled for multiple target architectures, a COIL binary (in NOF format) has the following structure:

```
+----------------+
| NOF Header     |
+----------------+
| Target Table   |
| - x86-64       |
| - ARM64        |
| - RISC-V       |
+----------------+
| Section Table  |
+----------------+
| x86-64 Code    |
+----------------+
| x86-64 Data    |
+----------------+
| ARM64 Code     |
+----------------+
| ARM64 Data     |
+----------------+
| RISC-V Code    |
+----------------+
| RISC-V Data    |
+----------------+
| Target Switch  |
| Table          |
+----------------+
| Symbol Table   |
+----------------+
| String Table   |
+----------------+
```

This structure enables:
- Separate code sections for each target architecture
- Shared data sections where appropriate
- Target switching between different architectures
- Runtime selection of the appropriate code path

## Target Selection

COIL provides several mechanisms for managing target architectures:

### 1. Static Target Specification

Target architecture can be explicitly specified at compile time:

```
DIR TARGET x86-64
```

### 2. Multiple Target Compilation

COIL supports generating code for multiple targets in a single binary:

```
; This code applies to all targets
...

; This code is specific to x86-64
DIR TARGET x86-64
...

; This code is specific to ARM64
DIR TARGET arm64
...
```

### 3. Target Preference Specification

The developer can specify target preferences for multi-target binaries:

```
DIR TARGET auto {
  PREFER x86-64-avx
  ACCEPT x86-64
  ACCEPT arm64-neon
  ACCEPT fallback
}
```

## Handling Architecture-Specific Features

When working with architecture-specific features, COIL provides several approaches:

### 1. Architecture-Specific Optimizations

```
DIR TARGET x86-64 {
  ; x86-64 specific optimizations
  ; These are used only when the standard COIL instructions
  ; aren't sufficient for a particular task
}

DIR TARGET arm64 {
  ; ARM64 specific optimizations
}
```

### 2. Alternative Implementations

```
; Primary implementation using COIL standard instructions
; COIL will automatically select optimal implementation
VAR DECL $0 : vec256(fp32)
VEC ADD $0, $1, $2

; Alternative implementation for special cases
DIR LABEL alternative_implementation
...
```

### 3. Processing Unit Specialization

```
; Code that runs on the CPU
...

; Code that runs on the GPU
DIR TARGET gpu {
  ; GPU-specific code
}
```

## ABI Handling

COIL's ABI system enables consistent function calling across different architectures:

```
; Define system call ABI for different architectures
DIR ABI abi-linux-x86_64 {
  args = [ R0, R4, R5, R3, R6, R7 ]
  rets = [ R0 ]
  preserved = [ R1, R10, R11, R12, R13 ]
  stack_align = 16
}

DIR ABI abi-linux-arm64 {
  args = [ R0, R1, R2, R3, R4, R5 ]
  rets = [ R0 ]
  preserved = [ R11, R12, R13 ]
  stack_align = 16
}

; Use ABI for system call
CF SYSC abi-linux (0x01, 1, str_ptr, str_len) -> (R0)
```

This approach ensures that function calls and system calls work correctly regardless of the target architecture.

## Cross-Platform Best Practices

### 1. Use COIL Abstractions

- Use virtual registers (R0, R1, etc.) instead of physical registers
- Use variables ($0, $1, etc.) for data storage
- Use standard COIL instructions instead of architecture-specific ones
- Use the ABI system for function calls and system calls

### 2. Handle Architectural Differences

- Detect available features before using them
- Provide fallback implementations for unsupported features
- Use architecture-specific sections for optimized code
- Be aware of endianness and word size differences

### 3. Test on All Target Architectures

- Verify that code works correctly on all target architectures
- Check for performance bottlenecks on each architecture
- Validate cross-architecture transitions
- Test with different feature combinations

### 4. Optimize Appropriately

- Use architecture-specific optimizations where available
- Provide efficient fallback implementations
- Balance code size and performance
- Consider the overhead of feature detection and target switching

## Common Cross-Platform Challenges

### 1. Endianness Handling

COIL abstracts away endianness differences in standard operations. For cases where explicit endianness handling is needed:

```
; Endianness-agnostic COIL code
MEM MOV R0, [R1]    ; COIL handles endianness automatically

; For explicit byte swapping when needed
BIT BSWAP R0, R0    ; Explicit byte swap independent of endianness
```

### 2. Word Size Handling

COIL manages word size differences automatically through its type system:

```
; Word size independent code
VAR DECL $0 : int32    ; Always 32 bits regardless of architecture
VAR DECL $1 : ptr(int8) ; Pointer sized appropriately for architecture
```

### 3. Vector Length Differences

COIL handles vector length differences automatically:

```
; COIL automatically adapts to available vector lengths
VAR DECL $0 : vec256(fp32)  ; Uses 256-bit vectors where available
                           ; Falls back to multiple 128-bit operations
                           ; or scalar operations as needed
```

### 4. Memory Alignment Handling

COIL's memory operations automatically handle alignment requirements:

```
; COIL handles alignment automatically for standard operations
MEM MOV [R0], R1     ; COIL ensures proper alignment

; For operations requiring specific alignment
MEM LOAD R0, [R1], 16  ; Explicit 16-byte aligned load
```

## Conclusion

COIL's cross-platform capabilities provide a powerful foundation for developing code that can run efficiently across multiple architectures and processing units. By using COIL's abstractions, target switching mechanism, and configuration-driven approach, developers can write code once and deploy it to diverse hardware platforms.

The key to successful cross-platform development with COIL is to embrace its abstractions while being aware of the underlying hardware differences. By following the best practices outlined in this document and leveraging COIL's cross-platform features, developers can create high-performance, portable code for a wide range of applications, from embedded systems to high-performance computing.