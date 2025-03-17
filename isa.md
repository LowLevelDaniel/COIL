# COIL Instruction Set Architecture

## Introduction

The COIL Instruction Set Architecture (ISA) provides a unified instruction set that works across different target architectures. Unlike traditional assembly languages that have architecture-specific instructions, COIL instructions are consistent across targets while still allowing full access to hardware capabilities. This document specifies the core COIL instruction set and explains how the capability-based configuration system enables consistent programming across diverse hardware platforms.

## Design Philosophy

The COIL ISA is designed with several key principles in mind:

1. **Consistency**: Instructions behave consistently across different target architectures.
2. **Type Awareness**: Instructions work with COIL's type system to eliminate the need for instruction variants.
3. **Completeness**: The instruction set covers all operations needed for system programming.
4. **Efficiency**: Instructions map efficiently to native hardware operations.
5. **Extensibility**: The instruction set can be extended for specialized hardware features.

## Capability-Based Instruction System

A key innovation in COIL is how the instruction set integrates with the capability-based configuration system. Instead of defining separate instructions for different data types (e.g., `add`, `addf`, `addi`), COIL defines a unified set of operations whose implementation is determined by the operand types and the capabilities of the target architecture.

For example, the `add` instruction works with:
- Integer operands (8, 16, 32, 64-bit, signed or unsigned)
- Floating-point operands (32 or 64-bit)
- Vector operands (when supported by the target)

The COIL assembler consults the capability bitfields in the target configuration to determine:
1. Whether the operation is supported for the given operand types
2. How to optimally implement the instruction on the target architecture
3. What constraints apply (e.g., alignment requirements, latency considerations)

This allows programmers to write consistent code that works across different architectures while still getting optimal performance on each target.

## Register Model

COIL uses a virtual register model that maps to the physical registers of the target architecture. The virtual registers include:

- General-purpose registers: `r0`, `r1`, `r2`, ... (number depends on configuration)
- Floating-point registers: `f0`, `f1`, `f2`, ... (number depends on configuration)
- Vector registers: `v0`, `v1`, `v2`, ... (number depends on configuration)
- Special registers: `pc` (program counter), `sp` (stack pointer), `fp` (frame pointer), etc.

The mapping of virtual to physical registers is defined in the target configuration.

## Instruction Format

COIL instructions have a consistent format across all operation types:

```
[<label>:] <opcode> [<operand>[, <operand>...]] [; <comment>]
```

Where:
- `<label>` is an optional label that can be used as a jump target
- `<opcode>` is the instruction operation code
- `<operand>` is an instruction operand (register, immediate value, memory reference, etc.)
- `<comment>` is an optional comment that is ignored by the assembler

## Operand Types

COIL supports several types of operands:

1. **Register**: A virtual register (e.g., `r0`, `f1`, `v2`)
2. **Immediate**: A constant value (e.g., `42`, `0x1F`, `1.5`)
3. **Memory**: A memory reference (e.g., `[r0]`, `[r1 + 8]`, `[symbol]`)
4. **Label**: A code location (e.g., `loop_start`, `.L1`)
5. **Symbol**: A named constant or variable (e.g., `counter`, `MAX_SIZE`)

## Memory Addressing Modes

COIL supports various memory addressing modes:

1. **Register Indirect**: `[r0]` - Address in register
2. **Displaced**: `[r0 + 8]` - Address in register plus displacement
3. **Indexed**: `[r0 + r1]` - Address in register plus index register
4. **Scaled Indexed**: `[r0 + r1 * 4]` - Address in register plus scaled index
5. **Absolute**: `[0x1000]` - Absolute address
6. **Symbol-based**: `[counter]` - Address of symbol
7. **PC-relative**: `[pc + 16]` - Address relative to program counter

## Core Instruction Categories

The COIL instruction set is organized into several categories:

### Data Movement Instructions

```
mov <dest>, <src>       ; Move data from src to dest
ldr <dest>, <addr>      ; Load from memory to register
str <src>, <addr>       ; Store from register to memory
push <src>              ; Push register onto stack
pop <dest>              ; Pop from stack into register
```

Example:
```
mov r0, 42              ; r0 = 42
ldr r1, [buffer + 8]    ; r1 = *(buffer + 8)
str r0, [r1]            ; *r1 = r0
push r0                 ; Push r0 onto stack
pop r2                  ; Pop from stack into r2
```

### Arithmetic Instructions

```
add <dest>, <src1>, <src2>   ; dest = src1 + src2
sub <dest>, <src1>, <src2>   ; dest = src1 - src2
mul <dest>, <src1>, <src2>   ; dest = src1 * src2
div <dest>, <src1>, <src2>   ; dest = src1 / src2
rem <dest>, <src1>, <src2>   ; dest = src1 % src2
neg <dest>, <src>            ; dest = -src
inc <dest>                   ; dest = dest + 1
dec <dest>                   ; dest = dest - 1
```

Example:
```
add r0, r1, r2          ; r0 = r1 + r2
sub r3, r0, 5           ; r3 = r0 - 5
mul r4, r3, r1          ; r4 = r3 * r1
div r5, r4, 2           ; r5 = r4 / 2
rem r6, r5, 10          ; r6 = r5 % 10
neg r7, r6              ; r7 = -r6
inc r0                  ; r0 = r0 + 1
dec r1                  ; r1 = r1 - 1
```

Note that these same instructions work with integer and floating-point operands:

```
add f0, f1, f2          ; f0 = f1 + f2 (floating-point addition)
```

### Logical Instructions

```
and <dest>, <src1>, <src2>   ; dest = src1 & src2
or <dest>, <src1>, <src2>    ; dest = src1 | src2
xor <dest>, <src1>, <src2>   ; dest = src1 ^ src2
not <dest>, <src>            ; dest = ~src
shl <dest>, <src>, <count>   ; dest = src << count
shr <dest>, <src>, <count>   ; dest = src >> count (logical)
sar <dest>, <src>, <count>   ; dest = src >> count (arithmetic)
rol <dest>, <src>, <count>   ; dest = (src << count) | (src >> (width - count))
ror <dest>, <src>, <count>   ; dest = (src >> count) | (src << (width - count))
```

Example:
```
and r0, r1, r2          ; r0 = r1 & r2
or r3, r0, 0xF          ; r3 = r0 | 0xF
xor r4, r3, r1          ; r4 = r3 ^ r1
not r5, r4              ; r5 = ~r4
shl r6, r5, 3           ; r6 = r5 << 3
shr r7, r6, 2           ; r7 = r6 >> 2 (logical)
sar r8, r7, 1           ; r8 = r7 >> 1 (arithmetic)
```

### Comparison Instructions

```
cmp <src1>, <src2>      ; Compare src1 and src2, set flags
test <src1>, <src2>     ; Test bits (logical AND), set flags
```

Example:
```
cmp r0, r1              ; Compare r0 and r1
test r2, 0x3            ; Test if bits 0 and 1 are set in r2
```

### Conditional Instructions

```
cmov<cond> <dest>, <src>     ; Conditional move
cset<cond> <dest>            ; Set if condition true, clear if false
```

Where `<cond>` is one of:
- `eq`: Equal
- `ne`: Not equal
- `lt`: Less than
- `le`: Less than or equal
- `gt`: Greater than
- `ge`: Greater than or equal
- `z`: Zero
- `nz`: Not zero
- Plus other condition codes specific to certain operations

Example:
```
cmoveq r0, r1           ; r0 = r1 if equal flag is set
csetlt r2               ; r2 = 1 if less than flag is set, otherwise r2 = 0
```

### Branch Instructions

```
jmp <label>             ; Unconditional jump
j<cond> <label>         ; Conditional jump
call <target>           ; Call function
ret                     ; Return from function
```

Example:
```
jmp loop_start          ; Jump to loop_start
jeq target              ; Jump to target if equal flag is set
jlt .L1                 ; Jump to .L1 if less than flag is set
call process_data       ; Call process_data function
ret                     ; Return from current function
```

### System Instructions

```
syscall                 ; System call
int <number>            ; Generate interrupt
cli                     ; Clear interrupt flag
sti                     ; Set interrupt flag
hlt                     ; Halt processor
nop                     ; No operation
```

Example:
```
mov r0, 1               ; System call number (write)
mov r1, 1               ; File descriptor (stdout)
mov r2, message         ; Buffer address
mov r3, message_len     ; Buffer length
syscall                 ; Perform system call
```

### Memory Barrier Instructions

```
fence                   ; Memory fence (full barrier)
fence.load              ; Load fence
fence.store             ; Store fence
```

Example:
```
str r0, [shared_data]   ; Store to shared memory
fence.store             ; Ensure store is visible to other processors
```

### SIMD/Vector Instructions

```
vadd <dest>, <src1>, <src2>    ; Vector addition
vmul <dest>, <src1>, <src2>    ; Vector multiplication
vdot <dest>, <src1>, <src2>    ; Vector dot product
vshuffle <dest>, <src>, <mask> ; Vector shuffle
```

Example:
```
vadd v0, v1, v2         ; Add vectors v1 and v2, store in v0
vmul v3, v0, v4         ; Multiply vectors v0 and v4, store in v3
vdot r0, v1, v2         ; Calculate dot product of v1 and v2, store in r0
```

### Type Conversion Instructions

```
cvt.<dst_type>.<src_type> <dest>, <src>   ; Convert from src_type to dst_type
```

Where `<src_type>` and `<dst_type>` are type specifications like:
- `i8`: 8-bit integer
- `i16`: 16-bit integer
- `i32`: 32-bit integer
- `i64`: 64-bit integer
- `f32`: 32-bit float
- `f64`: 64-bit float

Example:
```
cvt.f32.i32 f0, r0      ; Convert 32-bit integer in r0 to 32-bit float in f0
cvt.i64.f64 r1, f1      ; Convert 64-bit float in f1 to 64-bit integer in r1
```

## Special Instructions

### Target Switch Instruction

```
target <target_id>      ; Switch to the specified target
```

Example:
```
target x86_64
    ; x86_64-specific code
target arm64
    ; ARM64-specific code
```

### Inline Assembly Instruction

```
asm {
    ; Target-specific assembly code
}
```

Example:
```
target x86_64
asm {
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r0, rax
}
```

## Type-Aware Instruction Behavior

### How Type Awareness Works

When the COIL assembler processes an instruction, it determines the appropriate implementation based on the operand types:

1. **Explicit Types**: For explicitly typed operands (e.g., variables declared with types)
2. **Inferred Types**: For operands whose types can be inferred from context
3. **Default Types**: For immediate values with no explicit type information

For example, with the `add` instruction:

```
add r0, r1, 5           ; Integer addition (inferred from r1's type)
add f0, f1, 2.5         ; Floating-point addition (inferred from f1's type and the literal 2.5)
```

### Benefits of Type Awareness

This type-aware approach provides several benefits:

1. **Simplified Instruction Set**: Fewer distinct instructions to learn and use
2. **Consistent Semantics**: Same instruction name for the same conceptual operation
3. **Better Code Readability**: Clearer code without type-specific prefixes/suffixes
4. **Easier Code Generation**: Compiler front-ends can generate more consistent code

### Example: Handling Different Numeric Types

Consider a simple function to add two numbers and store the result:

```
; Function that works with both integers and floating-point values
add_numbers:
    add r0, r1, r2      ; r0 = r1 + r2
    str r0, [r3]        ; *r3 = r0
    ret
```

This same function works correctly whether the numbers are integers or floating-point values, because the COIL assembler generates the appropriate target instructions based on the types of `r1` and `r2`.

## Instruction Encoding

COIL instructions are encoded in a variable-length binary format that balances compactness with flexibility:

```
| Opcode (8 bits) | Type Info (8 bits) | Operand Count (4 bits) | Flags (4 bits) | Operands (variable) |
```

The Type Info field encodes information about the operand types, which the assembler uses to generate the correct machine code for the target architecture.

## Example: Complete Function in COIL

Here's an example of a complete function in COIL that calculates the absolute value of a number:

```
; Function: Calculate absolute value
; Parameters: r0 - input value
; Returns: r0 - absolute value
.function abs
abs:
    cmp r0, 0           ; Compare input with 0
    jge .done           ; If r0 >= 0, skip negation
    neg r0, r0          ; r0 = -r0
.done:
    ret                 ; Return with result in r0
.endfunction
```

This function works with both integer and floating-point values due to COIL's type awareness. The assembler generates different machine code based on whether `r0` contains an integer or a floating-point value.

## Example: Comparing COIL to Traditional Assembly

Here's a comparison between COIL and traditional assembly languages for a simple function:

### COIL:
```
; Calculate sum of array elements
.function sum_array
sum_array:
    ; r0 = array pointer, r1 = array length
    mov r2, 0           ; Initialize sum
    mov r3, 0           ; Initialize index
.loop:
    cmp r3, r1          ; Compare index with length
    jge .done           ; If index >= length, exit loop
    ldr r4, [r0 + r3 * 4] ; Load array[index]
    add r2, r2, r4      ; sum += array[index]
    inc r3              ; index++
    jmp .loop           ; Repeat
.done:
    mov r0, r2          ; Return sum
    ret
.endfunction
```

### x86-64 Assembly:
```
; Calculate sum of array elements
sum_array:
    xor eax, eax        ; Initialize sum
    xor ecx, ecx        ; Initialize index
.loop:
    cmp ecx, esi        ; Compare index with length
    jge .done           ; If index >= length, exit loop
    mov edx, [rdi + rcx * 4] ; Load array[index]
    add eax, edx        ; sum += array[index]
    inc ecx             ; index++
    jmp .loop           ; Repeat
.done:
    ret                 ; Return sum in eax
```

### ARM64 Assembly:
```
; Calculate sum of array elements
sum_array:
    mov w2, #0          ; Initialize sum
    mov w3, #0          ; Initialize index
.loop:
    cmp w3, w1          ; Compare index with length
    b.ge .done          ; If index >= length, exit loop
    ldr w4, [x0, w3, uxtw #2] ; Load array[index]
    add w2, w2, w4      ; sum += array[index]
    add w3, w3, #1      ; index++
    b .loop             ; Repeat
.done:
    mov w0, w2          ; Return sum
    ret
```

Notice how COIL provides a consistent syntax across architectures, while still generating efficient native code for each target.

## Target-Specific Extensions

While COIL provides a unified instruction set, it also allows access to target-specific features through extensions:

```
.target x86_64
.extension avx2
    vgather v0, [r0 + v1 * 4], v2  ; AVX2 gather instruction
```

These extensions are only available when targeting architectures that support them, but they maintain COIL's consistent syntax and type awareness.

## Optimized Instruction Selection

The COIL assembler performs sophisticated instruction selection based on the target architecture:

1. **Instruction Combining**: Combines multiple COIL instructions into a single native instruction when possible.
2. **Specialized Instructions**: Uses specialized hardware instructions for common patterns.
3. **Addressing Mode Optimization**: Selects the most efficient addressing modes for the target.
4. **Register Allocation**: Allocates registers according to the target's capabilities and constraints.

For example, a sequence like:
```
mul r0, r1, 2
add r0, r0, 1
```

Might be translated to a single LEA instruction on x86:
```
lea eax, [rcx * 2 + 1]
```

## Conclusion

The COIL Instruction Set Architecture provides a powerful, consistent approach to low-level programming across diverse hardware architectures. By integrating with COIL's type system, the instruction set eliminates the need for type-specific instruction variants while still providing full access to hardware capabilities.

This approach simplifies code generation, improves readability, and enables sophisticated cross-architecture optimization, making COIL an ideal intermediate language for system programming, embedded development, and high-performance computing.