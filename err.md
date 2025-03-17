# COIL Error Handling and Debugging

This document details how COIL handles errors and provides debugging capabilities across the development lifecycle.

## 1. Error Model Overview

COIL defines a comprehensive error model that covers:

1. **Compile-Time Errors**: Detected during HOIL-to-COIL compilation
2. **Link-Time Errors**: Detected during the linking phase
3. **Runtime Errors**: Detected during program execution
4. **Debugging Information**: Used by debugging tools

## 2. Compile-Time Error Handling

### 2.1 Error Categories

COIL defines several categories of compile-time errors:

1. **Syntax Errors**: Invalid HOIL syntax
2. **Type Errors**: Type mismatch or invalid type operations
3. **Scope Errors**: Variable access outside valid scope
4. **Declaration Errors**: Invalid declarations or redefinitions
5. **Constraint Errors**: Violations of architectural constraints

### 2.2 Error Format

Errors are reported in a structured format:

```
<file>:<line>:<column>: error: <message>
  |
<line> | <source line>
  |     <caret pointing to error location>
  |
note: <additional information>
```

Example error message:

```
main.hoil:42:15: error: type mismatch in assignment
  |
42 | var i32 x = 3.14
  |               ^^^^ expected 'i32', found 'f32'
  |
note: consider using an explicit conversion with cast.i32
```

### 2.3 Warning System

Warnings indicate potential issues that don't prevent compilation:

```
main.hoil:67:10: warning: unused variable 'counter'
  |
67 | var i32 counter = 0
  |          ^^^^^^^
  |
note: consider removing the variable or marking it as unused with '_counter'
```

Warning levels:
- **Level 1**: Basic warnings (unused variables, dead code)
- **Level 2**: Moderate warnings (implicit conversions, potential bugs)
- **Level 3**: Strict warnings (style issues, optimization hints)

### 2.4 Error Recovery

The COIL compiler attempts to recover from errors to report multiple issues in a single pass:

```
# Multiple errors reported in one compilation
main.hoil:42:15: error: type mismatch in assignment
main.hoil:43:8: error: undefined variable 'y'
main.hoil:44:12: error: duplicate variable declaration 'z'
```

## 3. Link-Time Error Handling

### 3.1 Symbol Resolution Errors

Errors related to unresolved or conflicting symbols:

```
link error: undefined reference to symbol 'calculate'
  |
  | referenced in main.o
  |
note: consider implementing the missing procedure or importing it from a library
```

### 3.2 Type Compatibility Errors

Errors related to incompatible types between modules:

```
link error: incompatible type for symbol 'process_data'
  |
  | in module1.o: procedure process_data(i32, f32) -> i32
  | in module2.o: procedure process_data(i32, i32) -> i32
  |
note: different parameter types at position 2
```

### 3.3 Architecture Compatibility Errors

Errors due to architecture mismatches:

```
link error: incompatible architecture for object file 'gpu_module.o'
  |
  | main program targets x86_64
  | gpu_module.o targets CUDA SM_80
  |
note: consider using device-specific code sections
```

## 4. Runtime Error Handling

### 4.1 Exception Model

COIL supports a lightweight exception model:

```
# Try-catch block in HOIL
try {
    var i32 result = divide(a, b)
} catch (EXCEPTION_DIVIDE_BY_ZERO) {
    io_println("Error: Division by zero")
} catch {
    io_println("Unknown error occurred")
}
```

### 4.2 Error Codes

Standard error codes are defined for common error conditions:

```
const i32 ERROR_SUCCESS = 0
const i32 ERROR_INVALID_ARGUMENT = 1
const i32 ERROR_OUT_OF_MEMORY = 2
const i32 ERROR_DIVIDE_BY_ZERO = 3
const i32 ERROR_NULL_POINTER = 4
const i32 ERROR_OUT_OF_BOUNDS = 5
```

### 4.3 Runtime Checks

COIL can insert runtime checks for error conditions:

```
# Bounds checking for array access
var i32[10] array
var i32 index = get_index()

# With runtime checks enabled
if (index < 0 || index >= 10) {
    raise ERROR_OUT_OF_BOUNDS
} else {
    var i32 value = array[index]
}
```

Runtime checks can be controlled with compiler flags:

```
# Enable all runtime checks
coil-assembler --runtime-checks input.hoil -o output.coil

# Enable specific checks
coil-assembler --check-bounds --check-null input.hoil -o output.coil
```

### 4.4 Stack Unwinding

On architectures that support it, COIL provides stack unwinding for exception handling:

```
procedure outer() {
    try {
        middle()
    } catch {
        handle_error()
    }
}

procedure middle() {
    inner()
}

procedure inner() {
    # This error will propagate to the catch block in outer()
    raise ERROR_INVALID_ARGUMENT
}
```

## 5. Debugging Support

### 5.1 Debug Information Format

COIL embeds debugging information in compiled code:

```
# Generate debug information
coil-assembler --debug input.hoil -o output.coil
```

Debug information includes:
1. **Source mapping**: Maps COIL instructions to HOIL source lines
2. **Variable locations**: Tracks where variables are stored
3. **Type information**: Provides detailed type data
4. **Scope information**: Describes variable scopes
5. **Procedure boundaries**: Marks the start and end of procedures

### 5.2 Source-Level Debugging

COIL supports source-level debugging through standard debugging protocols:

```
# Start a debugging session
coil-debug output.coil
```

Debugging commands:
```
break main           # Set breakpoint at the main procedure
break file.hoil:42   # Set breakpoint at line 42
run                  # Start execution
step                 # Execute one source line
next                 # Step over procedure calls
continue             # Resume execution
print x              # Print variable value
backtrace            # Show call stack
```

### 5.3 Tracing

COIL supports execution tracing for debugging:

```
# Enable tracing
coil-run --trace output.coil

# Trace output
Entering procedure 'main'
Line 10: var i32 x = 42
  Assigned 42 to 'x'
Line 11: var i32 y = x * 2
  Computed x * 2 = 84
  Assigned 84 to 'y'
```

### 5.4 Memory Debugging

Tools for detecting memory errors:

```
# Run with memory checking
coil-memcheck output.coil
```

Memory checks include:
1. **Use after free**: Detecting access to freed memory
2. **Double free**: Detecting multiple frees of the same memory
3. **Memory leaks**: Detecting unreachable allocated memory
4. **Buffer overflows**: Detecting out-of-bounds access
5. **Uninitialized reads**: Detecting reads from uninitialized memory

### 5.5 Performance Debugging

Tools for performance analysis:

```
# Run with profiling
coil-profile output.coil
```

Profiling information includes:
1. **Procedure timing**: Time spent in each procedure
2. **Call counts**: Number of times each procedure is called
3. **Hot spots**: Most time-consuming code sections
4. **Memory stats**: Allocation patterns and sizes
5. **Cache behavior**: Cache hit/miss statistics

## 6. Debugging Interface

### 6.1 Debug API

COIL provides a standard debugging API for integration with external tools:

```c
// Initialize debugging session
DebugSession* debug_init(const char* filename);

// Set breakpoint
int debug_set_breakpoint(DebugSession* session, const char* location);

// Execute program
int debug_run(DebugSession* session);

// Single step
int debug_step(DebugSession* session);

// Evaluate expression
DebugValue* debug_eval(DebugSession* session, const char* expression);

// Get current location
DebugLocation* debug_get_location(DebugSession* session);

// Get call stack
DebugCallStack* debug_get_stack(DebugSession* session);
```

### 6.2 Debug Protocol

COIL implements a standard debug protocol for IDE integration:

```json
// Set breakpoint request
{
  "command": "setBreakpoint",
  "location": {
    "file": "main.hoil",
    "line": 42
  }
}

// Breakpoint response
{
  "status": "success",
  "breakpointId": 1,
  "location": {
    "file": "main.hoil",
    "line": 42,
    "column": 0
  }
}
```

### 6.3 Debug Hooks

Provides hooks for custom debugging extensions:

```
# Define a debug hook in HOIL
debugHook breakpointHit(i32 breakpointId) {
    io_printf("Hit breakpoint %d\n", breakpointId)
    return DEBUG_CONTINUE
}
```

## 7. Error Handling Strategies

### 7.1 Error Propagation

COIL supports different error propagation strategies:

#### 7.1.1 Return Code Propagation

```
procedure divide(i32 a, i32 b) -> i32, i32 {
    if (b == 0) {
        return 0, ERROR_DIVIDE_BY_ZERO
    }
    return a / b, ERROR_SUCCESS
}

procedure calculate() -> i32, i32 {
    var i32 result, error = divide(10, 0)
    if (error != ERROR_SUCCESS) {
        return 0, error
    }
    return result, ERROR_SUCCESS
}
```

#### 7.1.2 Exception Propagation

```
procedure divide(i32 a, i32 b) -> i32 {
    if (b == 0) {
        raise ERROR_DIVIDE_BY_ZERO
    }
    return a / b
}

procedure calculate() -> i32 {
    try {
        return divide(10, 0)
    } catch {
        return 0
    }
}
```

### 7.2 Resource Management

COIL provides resource management strategies for error handling:

#### 7.2.1 Defer Statement

```
procedure process_file(ptr_t filename) -> i32 {
    var ptr_t file = fs_open(filename, FS_READ)
    if (file == NULL) {
        return ERROR_FILE_NOT_FOUND
    }
    
    defer fs_close(file)  # Will be called on all exit paths
    
    # Process file...
    if (error_condition) {
        return ERROR_PROCESSING_FAILED
        # fs_close is automatically called here
    }
    
    return ERROR_SUCCESS
    # fs_close is automatically called here too
}
```

#### 7.2.2 Resource Guards

```
procedure process_data(ptr_t data, i32 size) -> i32 {
    var ptr_t buffer = mem_alloc(size)
    guard buffer with mem_free  # Automatically free on scope exit
    
    # Use buffer...
    
    return ERROR_SUCCESS
}
```

### 7.3 Error Recovery

Techniques for recovering from errors:

```
procedure retry_operation() -> i32 {
    var i32 attempts = 0
    
    while (attempts < 3) {
        var i32 result = perform_operation()
        if (result == ERROR_SUCCESS) {
            return ERROR_SUCCESS
        }
        
        attempts = attempts + 1
        io_printf("Operation failed, retrying (%d/3)...\n", attempts)
        sleep(1000)  # Wait 1 second before retrying
    }
    
    return ERROR_OPERATION_FAILED
}
```

## 8. Language-Level Safety Features

### 8.1 Null Safety

COIL provides optional null safety features:

```
# Non-nullable type
var ptr_t! file = fs_open("data.txt", FS_READ)  # Must not be NULL

# Nullable type
var ptr_t? buffer = get_buffer()  # May be NULL

# Null checking
if (buffer != NULL) {
    # Safe to use buffer here
}

# Null assertion
var ptr_t data = buffer!  # Runtime error if buffer is NULL
```

### 8.2 Bounds Safety

Array bounds checking:

```
# Safe array access with bounds checking
var i32[10] array
var i32 index = get_index()

var i32 value = array.safe[index]  # Raises exception if out of bounds

# Or using subscript checking
var i32 value = array[check index]

# Range-based loops are always bounds-checked
for (var i32 value in array) {
    # Safe access to each element
}
```

### 8.3 Type Safety

Strong type checking and explicit conversions:

```
var i32 a = 42
var f32 b = 3.14

# Error: Implicit conversion not allowed
# var i32 c = b

# Explicit conversion required
var i32 c = cast.i32(b)  # Converts 3.14 to 3
```

## 9. Debug Modes

COIL supports different debugging modes for various needs:

### 9.1 Development Mode

Maximizes debugging capabilities:
- Full debug information
- All runtime checks enabled
- Detailed error messages
- No optimizations that affect debugging

```
coil-assembler --mode=development input.hoil -o output.coil
```

### 9.2 Release Mode

Balances performance and error handling:
- Limited debug information
- Critical runtime checks only
- Basic error messages
- Production-level optimizations

```
coil-assembler --mode=release input.hoil -o output.coil
```

### 9.3 Production Mode

Prioritizes performance:
- No debug information
- Minimal runtime checks
- Minimal error handling
- Maximum optimizations

```
coil-assembler --mode=production input.hoil -o output.coil
```

### 9.4 Custom Mode

Configurable debug settings:

```
coil-assembler \
  --debug-level=1 \
  --check-bounds \
  --check-null \
  --optimize=2 \
  input.hoil -o output.coil
```

## 10. Debugging Techniques

### 10.1 Conditional Breakpoints

Breakpoints that trigger only when a condition is met:

```
# In the debugger
break main.hoil:42 if x > 100
```

### 10.2 Data Breakpoints

Breakpoints that trigger when memory changes:

```
# In the debugger
watch counter  # Break when 'counter' changes
```

### 10.3 Log Points

Print information without stopping execution:

```
# In the debugger
logpoint main.hoil:42 "Counter value: {counter}"
```

### 10.4 Function Tracing

Trace function entries and exits:

```
# In the debugger
trace-procedure calculate
```

### 10.5 Post-Mortem Debugging

Analyze program state after a crash:

```
# Run program and save crash dump
coil-run --save-crash=crash.dump output.coil

# Analyze crash dump
coil-debug --load-crash=crash.dump
```

## 11. Error Handling Best Practices

### 11.1 Error Checking

Always check errors from operations that can fail:

```
# Good: Check error code
var i32 file = fs_open("data.txt", FS_READ)
if (file < 0) {
    handle_error(file)
    return
}

# Bad: Ignore error
var i32 file = fs_open("data.txt", FS_READ)
```

### 11.2 Error Contexts

Provide context when propagating errors:

```
procedure process_file(ptr_t filename) -> i32 {
    var i32 file = fs_open(filename, FS_READ)
    if (file < 0) {
        io_printf("Error opening file '%s': %s\n", 
                 filename, error_string(file))
        return file
    }
    
    # Continue processing...
}
```

### 11.3 Error Categories

Group related errors together:

```
# Define error categories
const i32 ERROR_CATEGORY_IO = 1000
const i32 ERROR_CATEGORY_MEMORY = 2000
const i32 ERROR_CATEGORY_NETWORK = 3000

# Define specific errors
const i32 ERROR_FILE_NOT_FOUND = ERROR_CATEGORY_IO + 1
const i32 ERROR_FILE_PERMISSION = ERROR_CATEGORY_IO + 2
const i32 ERROR_FILE_LOCKED = ERROR_CATEGORY_IO + 3
```

### 11.4 Assertion Usage

Use assertions for invariant conditions:

```
procedure calculate_area(i32 width, i32 height) -> i32 {
    # Assert preconditions
    assert width > 0, "Width must be positive"
    assert height > 0, "Height must be positive"
    
    return width * height
}
```

### 11.5 Logging

Use structured logging for error information:

```
procedure process_data(ptr_t data, i32 size) -> i32 {
    if (data == NULL) {
        log_error("process_data", "Null data pointer received")
        return ERROR_NULL_POINTER
    }
    
    if (size <= 0) {
        log_error("process_data", "Invalid size: %d", size)
        return ERROR_INVALID_ARGUMENT
    }
    
    # Processing...
    log_info("process_data", "Successfully processed %d bytes", size)
    return ERROR_SUCCESS
}
```

## 12. Case Studies

### 12.1 Memory Management Debugging

Debugging memory leaks in a program:

```
# Run with memory leak detection
coil-memcheck --leak-check=full output.coil

# Memory leak report
==12345== HEAP SUMMARY:
==12345==   in use at exit: 1,024 bytes in 1 blocks
==12345==   total heap usage: 8 allocs, 7 frees, 8,192 bytes allocated
==12345== 
==12345== 1,024 bytes in 1 blocks are definitely lost
==12345==    at 0x4C2B0: mem_alloc (memory.coil:45)
==12345==    by 0x5A123: allocate_buffer (buffer.coil:28)
==12345==    by 0x6D456: main (main.coil:17)
```

### 12.2 Multithreaded Debugging

Debugging race conditions in multithreaded code:

```
# Run with thread sanitizer
coil-thread-check output.coil

# Thread race report
==12346== WARNING: ThreadSanitizer: data race (pid=12346)
==12346==   Write of size 4 at 0x7fff00000010 by thread T1:
==12346==     #0 update_counter thread.coil:25
==12346==     #1 worker_thread thread.coil:42
==12346== 
==12346==   Previous write of size 4 at 0x7fff00000010 by main thread:
==12346==     #0 update_counter thread.coil:25
==12346==     #1 main main.coil:33
==12346== 
==12346== SUMMARY: ThreadSanitizer: data race thread.coil:25 update_counter
```

### 12.3 Performance Debugging

Identifying performance bottlenecks:

```
# Run with performance profiler
coil-profile output.coil

# Profile report
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  μs/call  μs/call  name    
 70.00      0.07     0.07     1000    70.00    70.00  process_data
 20.00      0.09     0.02     1000    20.00    20.00  calculate_hash
 10.00      0.10     0.01        1 10000.00 100000.00  main
```

### 12.4 Debugging a Complex Bug

Step-by-step process of tracking down a subtle bug:

1. **Identify symptoms**: Program occasionally crashes with ERROR_NULL_POINTER
2. **Reproduce the issue**: Find a reliable way to trigger the bug
3. **Add logging**: Insert logging statements to narrow down the location
4. **Use conditional breakpoints**: Break only when certain conditions are met
5. **Backtrace analysis**: Look at the call stack when the error occurs
6. **Root cause identification**: Find that a cleanup routine is freeing memory too early
7. **Fix verification**: Modify the code and verify the fix works