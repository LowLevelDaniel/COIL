# COIL Architecture

This document describes the architectural design of COIL, including its execution model, type system, and memory model.

## 1. Execution Model

COIL employs a register-based execution model that abstracts the underlying hardware while maintaining efficient mapping to native instructions.

### Virtual Execution Environment

- **Abstract Register Set**: COIL defines a set of virtual registers that are mapped to physical registers or memory locations by the assembler.
- **Execution Flow**: COIL follows a linear execution model with explicit control flow instructions.
- **Instruction Granularity**: Each COIL instruction maps to one or more native instructions based on the target architecture's capabilities.

### Code Organization

- **Modules**: COIL code is organized into modules, which are the basic unit of compilation.
- **Procedures**: Within modules, code is further organized into procedures, which are equivalent to functions.
- **Blocks**: Procedures contain blocks of instructions, which are sequences of instructions with a single entry point and potentially multiple exit points.

### Calling Convention

COIL abstracts away specific calling conventions while providing mechanisms to interface with native code:

- **Procedure Boundaries**: COIL procedures have well-defined entry and exit points.
- **Parameter Passing**: Parameters are passed through virtual registers or stack locations based on the target architecture's ABI.
- **Return Values**: Return values are handled similarly to parameters.
- **ABI Compliance**: When interacting with native code, COIL can adhere to the target architecture's ABI requirements.

## 2. Type System

COIL's type system is derived from the CPU configuration format to provide a consistent representation across different architectures.

### Basic Types

COIL defines the following basic types based on the hardware capabilities described in the CPU configuration:

- **Integer Types**: Based on `config_hw_int_type_t` from the configuration, including signed and unsigned variants.
  - `i8`, `i16`, `i32`, `i64`, `i128` (when supported)
  - `u8`, `u16`, `u32`, `u64`, `u128` (when supported)

- **Floating-Point Types**: Based on `config_hw_fp_type_t` from the configuration.
  - `f16`, `f32`, `f64`, `f80` (when supported), `f128` (when supported)
  - `bf16` (brain floating point, when supported)

- **Vector Types**: Based on `config_hw_vec_type_t` from the configuration.
  - `v128`, `v256`, `v512` (when supported)
  - Scalable vectors of various element types and counts

- **Boolean Type**: `bool` - Single-bit truth value, typically expanded to a full byte or word.

- **Address Types**: `addr` - Memory address with architecture-specific width.

### Type Attributes

Types can have additional attributes that provide more information for the assembler:

- **Alignment**: Specifies the required memory alignment for a value.
- **Volatile**: Indicates that a value may change outside the control of the program.
- **Constant**: Indicates that a value should not be modified after initialization.

### Type Conversions

COIL provides both implicit and explicit type conversions:

- **Implicit Conversions**: Performed automatically in certain contexts, such as widening integer conversions.
- **Explicit Conversions**: Performed using conversion instructions, such as integer-to-float or truncation.

## 3. Memory Model

COIL provides a consistent memory model that can adapt to the underlying hardware's capabilities.

### Address Spaces

COIL defines the following address spaces:

- **Global**: The default address space for global variables and data.
- **Local**: Used for procedure-local variables, typically mapped to the stack.
- **Constant**: Used for constant data that shouldn't be modified.
- **Device-Specific**: Additional address spaces may be defined for specific hardware features (e.g., GPU memory).

### Memory Operations

COIL provides the following memory operations:

- **Load**: Read from memory into a virtual register.
- **Store**: Write from a virtual register to memory.
- **Atomic Operations**: Perform atomic read-modify-write operations on memory.
- **Memory Barriers**: Control memory ordering and visibility.

### Memory Ordering

COIL supports various memory ordering semantics based on the `config_memory_model_t` from the configuration:

- **Relaxed**: No ordering constraints beyond basic coherency.
- **Acquire**: All previous reads/writes from other threads are visible before this operation.
- **Release**: All subsequent reads/writes from other threads will see operations before this one.
- **Acquire-Release**: Combines acquire and release semantics.
- **Sequential Consistency**: Full ordering of operations across all threads.

## 4. Variable System

One of COIL's key features is its variable system, which abstracts away register allocation and stack management.

### Variable Declaration

Variables in COIL are declared with a type and an optional initial value:

```
var i32 x = 10  # Integer variable with initial value
var f64 y       # Floating-point variable without initial value
```

### Variable Allocation

The COIL assembler allocates variables either to registers or to stack locations based on:

- Variable lifetime analysis
- Register pressure
- Target architecture capabilities
- Optimization level

### Variable Scope

Variables in COIL have well-defined scopes:

- **Global Scope**: Variables declared at the module level, accessible throughout the module.
- **Procedure Scope**: Variables declared within a procedure, accessible only within that procedure.
- **Block Scope**: Variables declared within a block, accessible only within that block and its nested blocks.

## 5. Instruction Categories

COIL instructions are categorized based on the capabilities described in the CPU configuration:

- **Arithmetic Operations**: Based on `config_instr_int_1_t`, `config_instr_int_2_t`, `config_instr_fp_1_t`, and `config_instr_fp_2_t`.
- **Vector Operations**: Based on `config_instr_vector_1_t` and `config_instr_vector_2_t`.
- **Memory Operations**: Load, store, and atomic operations.
- **Control Flow Operations**: Based on `config_instr_control_flow_1_t` and `config_instr_control_flow_2_t`.
- **Synchronization Operations**: Based on `config_instr_mem_sync_t`.
- **Security Operations**: Based on `config_security_features_1_t` and `config_security_features_2_t`.
- **Specialized Operations**: Architecture-specific operations that don't fit into the above categories.

Each category contains instructions that map to the capabilities described in the CPU configuration, with fallback implementations provided when native support is not available.