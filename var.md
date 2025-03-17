# COIL Variable System

One of COIL's key innovations is its variable system, which abstracts away direct register management and stack allocation while preserving the efficiency of assembly-level programming.

## 1. Variable Concept

In COIL, variables represent storage locations that are allocated either to hardware registers or stack memory locations. Unlike traditional assembly, developers don't need to manually assign registers or calculate stack offsets - this is handled automatically by the COIL assembler.

### 1.1 Variable Declaration

Variables are declared using the `var` keyword, specifying the type and an optional initial value:

```
# HOIL syntax
var i32 counter = 0       # 32-bit integer variable with initial value
var f64 temperature       # 64-bit floating-point variable without initial value
var v128 pixels           # 128-bit vector variable
```

In binary format, variable declarations are encoded in the globals section for module-level variables, or as part of procedure headers for local variables.

### 1.2 Variable Scoping

COIL supports three levels of variable scope:

1. **Global Scope**: Variables declared at the module level, accessible from any procedure in the module.
2. **Procedure Scope**: Variables declared within a procedure, accessible only within that procedure.
3. **Block Scope**: Variables declared within a block (enclosed by curly braces), accessible only within that block and its nested blocks.

```
# Global variable
var i32 global_counter = 0

procedure main() {
    # Procedure-scoped variable
    var i32 local_counter = 0
    
    {
        # Block-scoped variable
        var i32 block_counter = 0
        # Can access global_counter, local_counter, and block_counter here
    }
    
    # Can access global_counter and local_counter here, but not block_counter
}
```

### 1.3 Variable Lifetime

The lifetime of a variable depends on its scope:

- **Global Variables**: Exist for the entire program execution
- **Procedure Variables**: Exist from their declaration until the procedure returns
- **Block Variables**: Exist from their declaration until the block exits

## 2. Storage Allocation

The COIL assembler automatically allocates storage for variables based on a variety of factors.

### 2.1 Register Allocation

Variables that are frequently accessed and have non-overlapping lifetimes are prime candidates for register allocation. The assembler considers:

- **Usage Frequency**: Variables used more frequently are prioritized for register allocation
- **Lifetime**: Variables with non-overlapping lifetimes can share registers
- **Size**: Variables that fit in a single register are easier to allocate
- **Access Patterns**: Variables accessed in tight loops are prioritized
- **Register Constraints**: Some operations require specific registers on certain architectures

### 2.2 Stack Allocation

Variables that cannot be allocated to registers are stored on the stack. This includes:

- Variables with addresses taken (via the `&` operator)
- Variables too large to fit in registers
- Variables that would increase register pressure beyond available registers
- Variables explicitly marked for stack allocation with the `stack` attribute

### 2.3 Mixed Allocation

Some variables may use mixed allocation strategies:

- **Partial Register Allocation**: Parts of a large variable may be cached in registers while the full value lives on the stack
- **Temporary Register Allocation**: Stack variables may be temporarily loaded into registers for operations
- **Split Storage**: Different fields of a structure may be allocated differently

## 3. Variable Attributes

COIL variables can have attributes that influence their allocation and behavior:

```
var i32 [volatile] io_register     # Marks a variable as volatile (not optimizable)
var i32 [register] counter         # Hints that this variable should be in a register
var i32 [stack] large_array[1000]  # Explicitly allocates on the stack
var i32 [align(16)] aligned_data   # Specifies alignment requirements
```

Common attributes include:

- **volatile**: The variable may be modified externally; prevents certain optimizations
- **register**: Hints that the variable should be allocated to a register
- **stack**: Explicitly allocates the variable on the stack
- **align(n)**: Specifies the alignment requirement
- **constant**: The variable cannot be modified after initialization

## 4. Variable Operations

COIL provides several operations specific to its variable system:

### 4.1 Address Operations

```
# Get the address of a variable
var i32 x = 42
var addr.i32 ptr_to_x = &x

# Load from an address
var i32 y = *ptr_to_x
```

### 4.2 Variable Movement

```
# Move a value from one variable to another
move.i32 destination, source
```

### 4.3 Lifetime Control

```
# Explicitly end a variable's lifetime
release x

# Extend a variable's lifetime to the end of the outer scope
extend_life x
```

## 5. Implementation Details

### 5.1 Register Allocation Algorithm

The COIL assembler uses a graph coloring algorithm for register allocation:

1. **Build Interference Graph**: Variables that are live at the same time cannot share a register
2. **Assign Weights**: Weight nodes based on usage frequency and loop nesting
3. **Color the Graph**: Assign registers (colors) to variables (nodes)
4. **Spill if Necessary**: If not enough registers, spill some variables to the stack

### 5.2 Stack Frame Layout

The stack frame for a procedure contains:

1. **Return Address**: Saved by the caller or callee based on ABI
2. **Saved Registers**: Callee-saved registers that are used in the procedure
3. **Local Variables**: Variables allocated to the stack
4. **Spill Slots**: Temporary storage for register variables that are spilled
5. **Parameter Area**: Space for parameters passed to called procedures

```
+----------------------+
| Return Address       |  Higher addresses
+----------------------+
| Saved Registers      |
+----------------------+
| Local Variables      |
+----------------------+
| Spill Slots          |
+----------------------+
| Parameter Area       |  Lower addresses
+----------------------+
```

### 5.3 Optimizations

The COIL assembler performs several optimizations related to variables:

- **Dead Variable Elimination**: Removing unused variables
- **Variable Coalescing**: Merging variables with non-overlapping lifetimes
- **Register Promotion**: Moving stack variables to registers when beneficial
- **Loop Invariant Hoisting**: Moving invariant calculations out of loops
- **Common Subexpression Elimination**: Reusing already computed values

## 6. Debug Information

For debugging purposes, COIL maintains a mapping between variables in the source code and their allocated storage:

```
+----------------------+----------------------+
| Variable Name        | Storage Location     |
+----------------------+----------------------+
| x                    | Register R3          |
+----------------------+----------------------+
| y                    | Stack Offset -8      |
+----------------------+----------------------+
| z                    | Split (R5, Stack -16)|
+----------------------+----------------------+
```

This information is stored in the debug section of the binary when debug information is enabled.

## 7. ABI Interaction

When interacting with code compiled using other compilers or languages, COIL variables need to respect the target platform's ABI (Application Binary Interface).

### 7.1 Parameter Passing

COIL maps its variables to the ABI-specified registers or stack locations for parameter passing:

```
procedure external_call(i32 a, f64 b, i32 c) {
    # On x86-64, these might map to:
    # a -> rdi (register)
    # b -> xmm0 (register)
    # c -> rsi (register)
}
```

### 7.2 Return Values

Similarly, return values are mapped according to the ABI:

```
procedure i32 calculate() {
    var i32 result = 42
    return result  # On x86-64, returns in eax
}
```

### 7.3 Saved Registers

The COIL assembler automatically handles saving and restoring callee-saved registers based on the target architecture's ABI requirements.

## 8. Cross-Architecture Considerations

The variable system is designed to be architecture-neutral:

- The same COIL code can be compiled for different architectures
- Register allocation adapts to the available register set
- Stack layout adapts to the target ABI
- Variable attributes provide hints that work across architectures

When necessary, the COIL assembler consults the CPU configuration to determine the optimal allocation strategy for the target architecture.