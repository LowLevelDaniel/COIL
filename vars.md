# COIL Variable Management System

## Introduction

One of COIL's key abstractions that differentiates it from traditional assembly languages is its variable management system. Unlike standard assembly where registers and memory locations are managed manually, COIL provides a structured approach to handling data while maintaining the performance characteristics of low-level code. This system bridges the gap between high-level language constructs and low-level register/memory operations.

## Design Philosophy

The COIL variable management system is designed with several goals in mind:

1. **Optimization-Friendly**: Allow the COIL assembler to make intelligent decisions about register allocation and memory layout based on variable lifetimes and usage patterns.
2. **Type Safety**: Provide enough type information to catch common errors at compile time without imposing a full high-level type system.
3. **Close to the Metal**: Maintain a direct correspondence between variables and their physical storage locations in registers or memory.
4. **Flexibility**: Support a wide range of data types and storage classes to accommodate diverse programming needs.
5. **Binary Representation**: Optimize for compact and efficient binary encoding while preserving necessary metadata.

## Binary Representation

In the COIL binary format, variables are represented through several structures:

```c
/**
 * @brief Variable declaration record
 */
typedef struct coil_var_decl {
  uint8_t  var_id;              /**< Variable identifier (0-255) */
  uint8_t  storage_class;       /**< Storage class */
  uint16_t type_id;             /**< Type identifier */
  uint32_t flags;               /**< Variable flags */
  uint32_t lifetime_start;      /**< Lifetime start position */
  uint32_t lifetime_end;        /**< Lifetime end position (0 = to end of scope) */
  uint32_t initial_value_size;  /**< Size of initial value data (0 = no initial value) */
  uint8_t  initial_value[];     /**< Initial value data (if present) */
} coil_var_decl_t;

/**
 * @brief Variable access record
 */
typedef struct coil_var_access {
  uint8_t  var_id;              /**< Variable identifier */
  uint8_t  access_type;         /**< Access type (read/write/etc) */
  uint32_t position;            /**< Position in code */
} coil_var_access_t;
```

### Variable Identifiers

Variables are identified by an 8-bit identifier (0-255), which is unique within its scope. These identifiers are referenced in instructions using the syntax `$N` where N is the variable number.

### Storage Classes

```c
/**
 * @brief Variable storage classes
 */
enum coil_var_storage_class {
  COIL_VAR_STORAGE_AUTO = 0,    /**< Automatic storage (register or stack) */
  COIL_VAR_STORAGE_REGISTER = 1, /**< Register only (no spilling) */
  COIL_VAR_STORAGE_STACK = 2,    /**< Stack only */
  COIL_VAR_STORAGE_STATIC = 3,   /**< Static storage (data section) */
  COIL_VAR_STORAGE_THREAD = 4,   /**< Thread-local storage */
  COIL_VAR_STORAGE_GLOBAL = 5,   /**< Global storage */
};
```

### Variable Flags

```c
/**
 * @brief Variable flags
 */
enum coil_var_flags {
  COIL_VAR_FLAG_NONE = 0x00000000,    /**< No flags */
  COIL_VAR_FLAG_CONST = 0x00000001,   /**< Constant value */
  COIL_VAR_FLAG_VOLATILE = 0x00000002, /**< Volatile access */
  COIL_VAR_FLAG_READONLY = 0x00000004, /**< Read-only */
  COIL_VAR_FLAG_WRITEONLY = 0x00000008, /**< Write-only */
  COIL_VAR_FLAG_NOALIAS = 0x00000010, /**< No aliasing */
  COIL_VAR_FLAG_ALIGNED = 0x00000020, /**< Explicit alignment */
  COIL_VAR_FLAG_PERSISTENT = 0x00000040, /**< Persistent beyond scope */
  COIL_VAR_FLAG_EXPORTED = 0x00000080, /**< Exported symbol */
  COIL_VAR_FLAG_IMPORTED = 0x00000100, /**< Imported symbol */
};
```

### Access Types

```c
/**
 * @brief Variable access types
 */
enum coil_var_access_type {
  COIL_VAR_ACCESS_READ = 0,     /**< Read access */
  COIL_VAR_ACCESS_WRITE = 1,    /**< Write access */
  COIL_VAR_ACCESS_READ_WRITE = 2, /**< Read-write access */
  COIL_VAR_ACCESS_ADDRESS = 3,   /**< Address taken */
  COIL_VAR_ACCESS_ATOMIC = 4,    /**< Atomic access */
};
```

## Variable Declaration and Lifetime

Variables in COIL have explicit declaration points and lifetimes:

```
VAR DECL $0 : int32 = 42      ; Declare 32-bit integer with initial value 42
...
VAR PMT $0                    ; Make variable permanent (extend lifetime)
...
VAR DMT $0                    ; End variable lifetime
...
VAR DLT $0                    ; Delete variable (release storage)
```

The lifetime of a variable extends from its declaration to its explicit termination or the end of its scope. This information enables COIL assemblers to make intelligent decisions about register allocation and memory layout.

### Scope Management

Variables are scoped to their containing function or block:

```
FRAME ENTER                   ; Enter function frame (creates new scope)
  VAR DECL $0 : int32 = 0     ; Declare variable in this scope
  ...
  {                           ; Nested scope (implied by braces)
    VAR DECL $1 : int32 = 1   ; Declare variable in nested scope
    ...
  }                           ; $1 goes out of scope here
  ...
FRAME LEAVE                   ; Leave function frame (all variables go out of scope)
```

The COIL assembler tracks variable scopes and ensures that variables are properly deallocated when they go out of scope. This information is encoded in the binary format to enable optimization and validation.

## Register Allocation and Spilling

The COIL assembler is responsible for allocating registers to variables based on their lifetimes and usage patterns. The exact algorithm used for register allocation is not specified by the COIL format, allowing different implementations to use different strategies.

When there are more live variables than available registers, the assembler must spill some variables to memory. The decision of which variables to spill is also left to the implementation, but the binary format includes information about variable access patterns to guide this decision.

### Register Hints

COIL allows optional register hints to guide the register allocator:

```
VAR DECL $0 : int32 IN(R0)    ; Suggest allocating to register R0
```

These hints are not binding, but they provide guidance to the register allocator when specific registers are preferred for performance or compatibility reasons.

## Type System Integration

The variable system integrates with COIL's type system to provide type safety without imposing a full high-level type system:

```
VAR DECL $0 : int32           ; 32-bit integer
VAR DECL $1 : fp64            ; 64-bit floating point
VAR DECL $2 : ptr(int8)       ; Pointer to 8-bit integer
VAR DECL $3 : vec128(fp32)    ; 128-bit vector of 32-bit floats
VAR DECL $4 : int32[10]       ; Array of 10 32-bit integers
```

The type information is encoded in the binary format and used by the COIL assembler to validate operations, determine storage requirements, and optimize code generation.

### Type Conversions

When operations involve multiple variables of different types, COIL requires explicit type conversions:

```
VAR DECL $0 : int32 = 42      ; 32-bit integer
VAR DECL $1 : fp32            ; 32-bit float
...
MATH CONVERT $1, $0           ; Convert int32 to fp32
```

These explicit conversions ensure that type information is preserved and that the COIL assembler can generate appropriate code for the target architecture.

## Variable State Management

COIL provides explicit instructions for saving and restoring variable state during function calls and context switches:

```
MEM PUSH STATE $0             ; Save variable state to $0
...
MEM POP $0                    ; Restore variable state from $0
```

This capability is particularly useful when implementing context switches between different target architectures, as it allows state to be saved and restored in a consistent manner.

## Example Usage

The following example demonstrates variable declaration, usage, and lifecycle management in COIL:

```
FRAME ENTER                            ; Enter function scope

; Declare variables
VAR DECL $0 : int32 = 0                ; Loop counter
VAR DECL $1 : ptr(int32) = array_ptr   ; Array pointer
VAR DECL $2 : int32                    ; Temporary value

; Loop initialization
MEM MOV $0, 0                          ; Initialize counter to 0
CF BR loop_condition                   ; Branch to loop condition

; Loop body
DIR LABEL loop_body
  ; Calculate array index
  MATH MUL $2, $0, 4                   ; Multiply counter by 4 (size of int32)
  
  ; Read array element
  MEM MOV R0, [$1 + $2]                ; Load array[i] into R0
  
  ; Process element
  MATH ADD R0, R0, 1                   ; Increment value
  MEM MOV [$1 + $2], R0                ; Store back to array[i]
  
  ; Increment counter
  MATH ADD $0, $0, 1                   ; i++

; Loop condition
DIR LABEL loop_condition
  BIT CMP $0, 10                       ; Compare i with 10
  CF BRC LT loop_body                  ; If i < 10, continue loop

; Cleanup
VAR DLT $2                             ; Delete temporary value
VAR DLT $1                             ; Delete array pointer
VAR DLT $0                             ; Delete loop counter

FRAME LEAVE                            ; Leave function scope
```

## Cross-Architecture Considerations

The variable system is designed to work consistently across different target architectures. The COIL assembler is responsible for mapping variables to the appropriate registers or memory locations based on the target architecture's register set and ABI.

When targeting multiple architectures within a single binary, the variable system provides a consistent interface that allows code to be written once and compiled for multiple targets. The mapping between variables and physical storage locations is handled by the COIL assembler based on the target architecture configuration.

### Cross-Target Data Mapping

When switching between different target architectures, the variable system provides mechanisms for mapping data between different register sets and memory layouts:

```
; Source target (x86-64)
VAR DECL $0 : int32 = 42              ; Declare variable

; Switch to ARM64 target
CF SWITCH arm64 entry_point MAP($0 -> $0)

; Destination target (ARM64)
DIR LABEL entry_point
  ; Variable $0 is available with the same value
  ...
```

The `MAP` directive specifies how variables are mapped between targets, ensuring that data is correctly transferred during target switches.

## Implementation Requirements

A compliant COIL assembler must implement the following aspects of the variable management system:

1. **Variable Tracking**: Track variable declarations, lifetimes, and usages.
2. **Register Allocation**: Allocate registers to variables based on their lifetimes and usage patterns.
3. **Memory Layout**: Determine appropriate memory layouts for spilled variables and arrays.
4. **Type Checking**: Validate operations based on variable types.
5. **Cross-Target Mapping**: Support data mapping between different target architectures.

The exact algorithms used for these tasks are not specified by the COIL format, allowing different implementations to use different strategies as long as they produce correct and efficient code.

## Conclusion

COIL's variable management system bridges the gap between high-level language variables and low-level register/memory operations. By providing a structured approach to data handling while maintaining close-to-the-metal performance, it enables programmers to write efficient cross-platform code without sacrificing control over the underlying hardware.

The binary representation of variables, combined with explicit lifetime and type information, enables COIL assemblers to generate optimized code for diverse target architectures while maintaining a consistent programming model. This balance of abstraction and control makes COIL an effective intermediate representation for system programming, embedded development, and cross-platform applications.