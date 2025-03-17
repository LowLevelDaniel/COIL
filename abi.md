# COIL ABI and Function System

## Introduction

The Application Binary Interface (ABI) and function system in COIL provides a structured approach to defining, calling, and implementing functions across different target architectures. This system enables interoperability between functions compiled from different source languages or for different target architectures, while maintaining performance and control.

## Design Philosophy

The COIL ABI and function system is designed with several principles in mind:

1. **Explicit Control**: Give programmers explicit control over calling conventions when needed.
2. **Cross-Platform Compatibility**: Allow functions to be called across different target architectures.
3. **Performance**: Enable high-performance function calls optimized for the target architecture.
4. **Interoperability**: Support calling and being called by functions written in other languages.
5. **Optimization-Friendly**: Provide enough information to enable advanced optimizations like inlining.

## ABI Configuration

ABIs in COIL are defined in the configuration file and specified in the ABI Mapping Table of the COIL binary format. Each ABI definition includes:

1. **Parameter Passing Convention**: How parameters are passed to functions (registers, stack, or a combination).
2. **Return Value Convention**: How return values are handled (registers, memory, or a combination).
3. **Stack Frame Convention**: How the stack frame is set up and torn down.
4. **Register Usage**: Which registers are caller-saved vs. callee-saved.
5. **Name Mangling**: How function names are encoded in the symbol table.

### Example ABI Definitions

In the configuration file, ABIs might be defined as follows:

```
abi x86_64_sysv {
    param_regs = [RDI, RSI, RDX, RCX, R8, R9]
    float_param_regs = [XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7]
    return_reg = RAX
    float_return_reg = XMM0
    caller_saved = [RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11, ...]
    callee_saved = [RBX, RSP, RBP, R12, R13, R14, R15]
    stack_alignment = 16
    shadow_space = 0
}

abi win64 {
    param_regs = [RCX, RDX, R8, R9]
    float_param_regs = [XMM0, XMM1, XMM2, XMM3]
    return_reg = RAX
    float_return_reg = XMM0
    caller_saved = [RAX, RCX, RDX, R8, R9, R10, R11, ...]
    callee_saved = [RBX, RSP, RBP, RSI, RDI, R12, R13, R14, R15]
    stack_alignment = 16
    shadow_space = 32
}

abi arm64 {
    param_regs = [X0, X1, X2, X3, X4, X5, X6, X7]
    float_param_regs = [V0, V1, V2, V3, V4, V5, V6, V7]
    return_reg = X0
    float_return_reg = V0
    caller_saved = [X0, X1, X2, X3, X4, X5, X6, X7, X9, X10, X11, X12, X13, X14, X15, ...]
    callee_saved = [X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30]
    stack_alignment = 16
    shadow_space = 0
}
```

## Function Declaration

In COIL, functions are explicitly declared with their parameters, return type, and ABI. A function declaration specifies:

1. The function's name
2. Its parameter list with types
3. Its return type
4. The ABI to use (optional, defaults to the target's default ABI)
5. Any function attributes (inline, noreturn, etc.)
6. The target architecture (optional, defaults to current target)

### HOIL Syntax for Function Declaration

```
function <name>(<params>) -> <return_type> [abi(<abi_name>)] [<attributes>] [target(<target_id>)] {
    <body>
}
```

Examples in HOIL:

```
function add(a: i32, b: i32) -> i32 {
    return a + b
}

function process_data(data: ptr<void>, size: u64) -> void abi(system) noreturn {
    // Function body
}

function kernel_function(x: f32, y: f32) -> f32 target(GPU) {
    // GPU-specific code
}
```

## Function Calls

COIL provides a straightforward syntax for function calls:

```
<result> = call <function_name>(<arguments>)
```

The assembler automatically handles:
1. Argument passing according to the function's ABI
2. Setting up the stack frame if needed
3. Handling the return value
4. Preserving caller-saved registers

Example in HOIL:

```
var sum : i32 = call add(5, 10)  // Call the add function defined earlier
```

For direct control over the call mechanism, COIL also provides a lower-level call instruction:

```
<result> = callraw <function_ptr> <abi_name> (<register_args>) (<stack_args>)
```

Example:

```
var func_ptr : ptr<void> = &add
var result : i32 = callraw func_ptr x86_64_sysv (R1=5, R2=10) ()
```

## Cross-Target Function Calls

One of COIL's powerful features is the ability to call functions across different target architectures. The assembler automatically generates the necessary glue code to:

1. Translate arguments from the calling target's ABI to the called target's ABI
2. Switch execution context to the called target
3. Translate return values back to the calling target's format
4. Switch execution context back to the calling target

This is particularly useful for heterogeneous computing, where different parts of the program might run on different processing units (e.g., CPU and GPU).

Example in HOIL:

```
function cpu_function() -> void {
    var x : f32 = 1.0
    var y : f32 = 2.0
    var result : f32 = call gpu_kernel(x, y)  // Call GPU function from CPU
}

function gpu_kernel(a: f32, b: f32) -> f32 target(GPU) {
    return a + b  // GPU code
}
```

## Function Pointers

COIL supports function pointers, allowing for dynamic dispatch and callback patterns:

```
var func_ptr : ptr<function(i32, i32) -> i32>
func_ptr = &add
var result : i32 = call func_ptr(5, 10)
```

## Inline Functions

COIL supports function inlining through the `inline` attribute. When a function is marked as inline, the assembler may replace calls to it with the function body, eliminating the call overhead.

```
function square(x: i32) -> i32 inline {
    return x * x
}

function calculate() -> i32 {
    return square(5)  // May be inlined as "return 5 * 5"
}
```

## Intrinsic Functions

COIL provides a set of intrinsic functions that map directly to target-specific instructions or operations:

```
var result : i32 = call intrinsic.popcount(0x1234)  // Population count instruction
```

Common intrinsics include:
- Math operations (sqrt, sin, cos, etc.)
- Bit manipulation (popcount, ctz, clz, etc.)
- Memory operations (prefetch, fence, etc.)
- SIMD operations (vector add, multiply, etc.)

## Exception Handling

COIL supports exception handling through the `try`/`catch` syntax:

```
try {
    // Code that might throw an exception
} catch (exception_var : exception_type) {
    // Exception handling code
}
```

The exception handling mechanism is implemented according to the target's ABI specifications (e.g., zero-cost exceptions on x86_64, SJLJ on older platforms).

## Stack Frame Management

COIL provides explicit control over stack frame setup and teardown when needed:

```
function example() -> void {
    stackframe {
        allocate 64  // Allocate 64 bytes on the stack
        // Use stack memory
    }  // Automatically deallocated when scope ends
}
```

## Register Saving and Restoration

While COIL's ABI system automatically handles register preservation according to calling conventions, programmers can also explicitly save and restore registers when needed:

```
function low_level_function() -> void {
    save R1, R2, R3  // Explicitly save registers
    // Use R1, R2, R3
    restore R1, R2, R3  // Explicitly restore registers
}
```

## Tail Calls

COIL supports tail call optimization through the `tailcall` instruction:

```
function factorial_tail(n: i32, acc: i32) -> i32 {
    if (n <= 1) {
        return acc
    } else {
        tailcall factorial_tail(n - 1, n * acc)
    }
}
```

## Variable-Argument Functions

COIL supports variable-argument functions through the `varargs` parameter:

```
function printf(format: ptr<i8>, varargs) -> i32 abi(c) {
    // Implementation
}

function example() -> void {
    call printf("Value: %d\n", 42)
}
```

## Function Attributes

COIL supports various function attributes to control generation and optimization:

- `inline`: Suggest that calls to this function should be inlined
- `noinline`: Prevent inlining of this function
- `noreturn`: Function does not return (e.g., exits the program)
- `pure`: Function has no side effects and return value depends only on arguments
- `const`: Function has no side effects and always returns the same value for the same arguments
- `hot`: Function is likely to be executed frequently
- `cold`: Function is unlikely to be executed frequently
- `naked`: Function does not have a standard prologue/epilogue
- `interrupt`: Function is an interrupt handler

Example:

```
function exit(status: i32) -> void noreturn {
    // Exit the program
}

function fast_math(x: f32, y: f32) -> f32 pure {
    return x * y + x
}
```

## ABI Interoperability

COIL provides explicit support for interoperating with functions using different ABIs:

```
function external_c_function(a: i32, b: ptr<i8>) -> i32 abi(c) extern;

function my_function() -> i32 {
    var str : [8]i8 = "Hello"
    return call external_c_function(42, &str[0])
}
```

## Example: Complete Function System Usage

Here's a more comprehensive example demonstrating various aspects of COIL's function system:

```
// Define an external C function
function printf(format: ptr<i8>, varargs) -> i32 abi(c) extern;

// Define a utility function with the default ABI
function fibonacci(n: i32) -> i32 {
    if (n <= 1) {
        return n
    } else {
        var a : i32 = call fibonacci(n - 1)
        var b : i32 = call fibonacci(n - 2)
        return a + b
    }
}

// Define a function using a specific ABI
function main(argc: i32, argv: ptr<ptr<i8>>) -> i32 abi(system) {
    var result : i32 = call fibonacci(10)
    call printf("Fibonacci(10) = %d\n", result)
    return 0
}

// Define a target-specific function
function gpu_calculate(input: ptr<f32>, output: ptr<f32>, count: i32) -> void target(GPU) {
    var idx : i32 = get_global_id(0)
    if (idx < count) {
        output[idx] = input[idx] * 2.0
    }
}

// Define a function that calls across targets
function process_data(data: ptr<f32>, size: i32) -> void {
    var output : [1024]f32
    call gpu_calculate(data, &output[0], size)
    // Wait for GPU completion
    var sum : f32 = 0.0
    var i : i32 = 0
    while (i < size) {
        sum = sum + output[i]
        i = i + 1
    }
    call printf("Sum: %f\n", sum)
}
```

## Conclusion

COIL's ABI and function system provides a powerful yet flexible approach to defining and using functions across different target architectures. By making ABIs explicit and configurable, COIL enables high-performance, cross-platform code that can seamlessly interoperate with existing codebases while maintaining control over the underlying implementation details.