# COIL Directives and Section Management

## Introduction

COIL provides a comprehensive set of directives that control the assembly process, manage code and data organization, and communicate with the COIL linker. These directives give compilers and programmers precise control over how code is generated, optimized, and linked without requiring high-level language constructs.

## Design Philosophy

COIL directives are designed with several principles in mind:

1. **Explicit Control**: Provide explicit control over code generation and linking
2. **Minimalism**: Include only directives that are necessary for low-level system programming
3. **Cross-Platform**: Function consistently across target architectures
4. **Linker Integration**: Enable direct communication with the COIL linker
5. **Optimization Hints**: Allow providing hints to the optimizer without forcing specific implementations

## Section Management

COIL code is organized into sections that define both the content type and attributes of the contained code or data. Unlike traditional assembly languages where sections primarily differentiate between code and data, COIL sections include rich metadata about their purpose, access patterns, and optimization opportunities.

### Basic Section Directives

```
.section <name> [<flags>]
```

Begins a new section with the specified name and optional flags. Common section names include:

- `.text`: Contains executable code
- `.data`: Contains initialized data
- `.rodata`: Contains read-only data
- `.bss`: Contains uninitialized data
- `.init`: Contains initialization code
- `.fini`: Contains finalization code

Example:
```
.section .text exec, read
    // Executable code follows
```

### Section Flags

Section flags define attributes that apply to the entire section:

- `exec`: Section contains executable code
- `read`: Section can be read
- `write`: Section can be written to
- `alloc`: Section requires space in memory
- `tls`: Section contains thread-local storage
- `merge`: Section can be merged with other sections
- `strings`: Section contains null-terminated strings
- `linkorder`: Section ordering matters for linking
- `target(<target_id>)`: Section is specific to the named target

Example:
```
.section .rodata read, alloc, target(x86_64)
    // Read-only data specific to x86_64
```

### Subsections

Subsections allow finer-grained organization within a section:

```
.subsection <name> [<flags>]
```

Example:
```
.section .text exec, read
.subsection hot_path
    // Frequently executed code
.subsection cold_path
    // Rarely executed code
```

### Alignment

The alignment directive controls the alignment of the section or individual elements:

```
.align <value>
```

Example:
```
.section .data read, write
.align 16
    // Data aligned to 16-byte boundary
```

## Symbol Directives

### Symbol Definition

```
.global <symbol>
```

Marks a symbol as globally visible (accessible from other modules).

```
.local <symbol>
```

Marks a symbol as local to the current module.

```
.weak <symbol>
```

Defines a weak symbol that can be overridden by another definition.

### Symbol Binding

```
.bind <symbol>, <target_symbol>
```

Binds a symbol to a target symbol at link time.

Example:
```
.bind malloc, _system_malloc
```

### Entry Point

```
.entry <symbol>
```

Specifies the program entry point.

Example:
```
.entry main
```

## Data Directives

### Data Definition

```
.byte <value>[, <value>...]
```

Defines one or more bytes of data.

```
.word <value>[, <value>...]
```

Defines one or more 16-bit words of data.

```
.dword <value>[, <value>...]
```

Defines one or more 32-bit double words of data.

```
.qword <value>[, <value>...]
```

Defines one or more 64-bit quad words of data.

```
.float <value>[, <value>...]
```

Defines one or more 32-bit floating-point values.

```
.double <value>[, <value>...]
```

Defines one or more 64-bit floating-point values.

```
.ascii "<string>"
```

Defines an ASCII string without null termination.

```
.asciz "<string>"
```

Defines an ASCII string with null termination.

```
.space <size>[, <fill>]
```

Reserves a block of space with optional fill value.

Example:
```
.section .data read, write
    counter: .dword 0
    message: .asciz "Hello, COIL!"
    buffer: .space 1024, 0
```

### Structured Data

```
.struct <name>
    <field_name>: <field_directive> <value>
    // Additional fields...
.endstruct
```

Defines a structured data type.

Example:
```
.section .data read, write
.struct Point
    x: .dword 0
    y: .dword 0
.endstruct

    origin: Point
```

## Code Organization Directives

### Labels

Labels mark specific locations in the code that can be targeted by jumps and branches:

```
label_name:
```

Example:
```
loop_start:
    // Code
    cmp r1, r2
    jlt loop_start
```

### Local Labels

Local labels are visible only within a limited scope, typically a function:

```
.L<name>:
```

Example:
```
function_start:
    // Code
.L1:
    // More code
    jmp .L1
```

### Code Blocks

Code blocks group related instructions and can have their own scope:

```
.block [<name>]
    // Code
.endblock
```

Example:
```
.block initialization
    // Initialization code
.endblock
```

## Optimization Directives

### Function Attributes

```
.function <name> [<attributes>]
    // Function body
.endfunction
```

Defines a function with optional attributes like `inline`, `noreturn`, `pure`, etc.

Example:
```
.function calculate_crc32 pure
    // Pure function (no side effects)
.endfunction
```

### Alignment Hints

```
.align_loop <value>
```

Suggests loop alignment for optimal performance.

```
.align_branch <value>
```

Suggests branch alignment for optimal performance.

Example:
```
.align_loop 16
loop_start:
    // Loop body
    cmp r1, r2
    jlt loop_start
```

### Prefetch Hints

```
.prefetch <address>, <type>
```

Suggests prefetching memory for optimal cache performance.

Example:
```
    mov r1, [array_ptr]
    .prefetch [r1 + 64], read
    // Code that will soon access [r1 + 64]
```

## Target-Specific Directives

### Target Selection

```
.target <target_id>
```

Switches the current target for subsequent code.

Example:
```
.section .text exec, read
.target x86_64
    // x86_64 code
.target arm64
    // ARM64 code
```

### Feature Requirements

```
.require <feature>
```

Specifies that a particular architecture feature is required.

Example:
```
.target x86_64
.require avx2
    // Code using AVX2 instructions
```

## Linker Directives

### Linking Scripts

```
.linkscript
    // Linking script commands
.endlinkscript
```

Provides direct instructions to the COIL linker.

Example:
```
.linkscript
    map sections = [.text.hot, .text]
    stack_size = 8192
.endlinkscript
```

### Symbol Resolution

```
.resolve <symbol>, <resolution_strategy>
```

Specifies how a symbol should be resolved during linking.

Example:
```
.resolve external_function, dynamic
```

### Versioning

```
.version <symbol>, <version>
```

Specifies the version of a symbol for version-aware linking.

Example:
```
.version malloc, "COIL_1.0"
```

## Conditional Assembly

### Conditional Compilation

```
.if <condition>
    // Code to include if condition is true
.elseif <condition>
    // Code to include if previous conditions are false but this is true
.else
    // Code to include if all conditions are false
.endif
```

Conditionally includes or excludes code during assembly.

Example:
```
.if TARGET == "x86_64"
    // x86_64-specific code
.elseif TARGET == "arm64"
    // ARM64-specific code
.else
    .error "Unsupported target"
.endif
```

### Repetition

```
.repeat <count>
    // Code to repeat
.endrepeat
```

Repeats a block of code a specified number of times.

Example:
```
.repeat 4
    inc r1
.endrepeat
```

## Error Handling Directives

```
.error "<message>"
```

Generates an error during assembly with the specified message.

```
.warning "<message>"
```

Generates a warning during assembly with the specified message.

Example:
```
.if TARGET == "unknown"
    .error "Target must be specified"
.endif
```

## Example: Complete Section Organization

Here's a more complete example showing how COIL code is organized into sections:

```
// Define program header
.section .header read
    .dword 0x4C494F43  // "COIL" magic number
    .dword 1           // Version 1
    .dword _start      // Entry point
    .dword _end        // End of program

// Initialized read-only data
.section .rodata read, alloc
    hello_msg: .asciz "Hello, COIL!"
    error_msg: .asciz "Error: %s\n"
    pi:        .double 3.14159265358979323846

// Initialized read-write data
.section .data read, write, alloc
    counter:   .dword 0
    options:   .byte 0x01, 0x02, 0x03, 0x04
    .align 8
    timestamp: .qword 0

// Uninitialized data
.section .bss read, write, alloc
    .align 16
    buffer:    .space 4096
    temp:      .space 1024

// Code section
.section .text exec, read
// Program entry point
_start:
    // Initialize system
    mov r1, 0
    str r1, [counter]
    
    // Print welcome message
    mov r1, hello_msg
    bl print_string
    
    // Call main function
    bl main
    
    // Exit program
    mov r0, 0
    bl exit

// Main function
.function main
main:
    // Function prologue
    push fp
    mov fp, sp
    sub sp, sp, 16
    
    // Function body
    mov r0, 42
    str r0, [fp, -8]
    
    // Function epilogue
    mov sp, fp
    pop fp
    ret
.endfunction

// Hot path functions in separate subsection
.subsection .text.hot exec, read
.function calculate_crc32 pure
calculate_crc32:
    // Function implementation
    ret
.endfunction

// End of program marker
_end:
```

## HOIL Representation vs. Actual COIL

It's important to understand that HOIL (High-level Object Intermediate Language) used in examples is just a human-readable pseudocode to illustrate COIL concepts. Actual COIL code is much closer to traditional assembly but with the added benefits of COIL's type system, target switching, and other features.

### HOIL Example:
```
var counter : i32 = 0
while (i < size) {
    counter = counter + 1
    i = i + 1
}
```

### Equivalent Actual COIL:
```
    mov r1, 0          // i = 0
    mov r2, [size]     // Load size
    mov r3, 0          // counter = 0
loop_start:
    cmp r1, r2         // Compare i with size
    jge loop_end       // Jump if i >= size
    add r3, r3, 1      // counter += 1
    add r1, r1, 1      // i += 1
    jmp loop_start     // Repeat
loop_end:
    str r3, [counter]  // Store counter
```

## Conclusion

COIL directives and section management provide a powerful, explicit system for organizing code and data across multiple architectures. By giving compilers and programmers direct control over code generation, optimization, and linking, COIL achieves the flexibility and performance required for low-level system programming while maintaining its cross-platform capabilities.

The directive system eliminates the need for high-level language constructs by providing equivalent functionality at a lower level, allowing COIL to remain close to machine code while offering superior organization and optimization opportunities compared to traditional assembly languages.