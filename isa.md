# COIL Instruction Set Architecture

## Introduction

The COIL Instruction Set Architecture (ISA) provides a unified instruction set that works across different target architectures. Unlike traditional assembly languages that have architecture-specific instructions, COIL instructions are consistent across targets while still allowing full access to hardware capabilities. This document specifies the core COIL instruction set and explains how it enables consistent programming across diverse hardware platforms.

## Design Philosophy

The COIL ISA is designed with several key principles in mind:

1. **Consistency**: Instructions behave consistently across different target architectures.
2. **Type Awareness**: Instructions work with COIL's type system to eliminate the need for type-specific variants.
3. **Completeness**: The instruction set covers all operations needed for system programming.
4. **Efficiency**: Instructions map efficiently to native hardware operations.
5. **Extensibility**: The instruction set can be extended for specialized hardware features.
6. **Binary Representation**: Optimized for compact binary encoding while providing a clear assembly syntax.

## Instruction Categories

COIL instructions are organized into functional categories that provide a clear, logical grouping of related operations:

### 1. Control Flow (CF)

Instructions that control program execution flow:

- **CF BR**: Unconditional branch to label
- **CF BRC**: Conditional branch to label
- **CF CALL**: Function call
- **CF RET**: Return from function
- **CF INT**: Software interrupt
- **CF IRET**: Return from interrupt
- **CF HLT**: Halt execution
- **CF SYSC**: System call with ABI specification
- **CF TRAP**: Trap to exception handler
- **CF WFE**: Wait for event
- **CF SEV**: Send event
- **CF FENCE**: Memory fence
- **CF YIELD**: Yield execution
- **CF SWITCH**: Switch target architecture
- **CF NOP**: No operation

#### Condition Codes

For conditional branches:

- **EQ**: Equal
- **NE**: Not equal
- **LT**: Less than
- **LE**: Less than or equal
- **GT**: Greater than
- **GE**: Greater than or equal
- **Z**: Zero
- **NZ**: Not zero
- **CS**: Carry set
- **CC**: Carry clear
- **VS**: Overflow set
- **VC**: Overflow clear
- **NS**: Negative set
- **NC**: Negative clear
- **PS**: Parity set
- **PC**: Parity clear

### 2. Memory Operations (MEM)

Instructions that operate on memory:

- **MEM MOV**: Move data between registers, memory, and immediate values
- **MEM PUSH**: Push to stack
- **MEM POP**: Pop from stack
- **MEM LOAD**: Load from memory to register or variable
- **MEM STORE**: Store from register or variable to memory
- **MEM PREFETCH**: Prefetch memory into cache
- **MEM EXCHANGE**: Exchange values
- **MEM COMPARE**: Compare and update flags
- **MEM TEST**: Test and update flags
- **MEM FILL**: Fill memory block
- **MEM COPY**: Copy memory block
- **MEM ZERO**: Zero memory block
- **MEM PUSH STATE**: Save register state
- **MEM POP STATE**: Restore register state
- **MEM OUT**: Output to I/O port (architecture-specific)
- **MEM IN**: Input from I/O port (architecture-specific)
- **MEM LGDT**: Load global descriptor table (architecture-specific)

### 3. Arithmetic Operations (MATH)

Instructions for mathematical operations:

- **MATH ADD**: Addition
- **MATH SUB**: Subtraction
- **MATH MUL**: Multiplication
- **MATH DIV**: Division
- **MATH MOD**: Modulo
- **MATH NEG**: Negation
- **MATH INC**: Increment
- **MATH DEC**: Decrement
- **MATH ABS**: Absolute value
- **MATH SQRT**: Square root
- **MATH MIN**: Minimum
- **MATH MAX**: Maximum
- **MATH FMA**: Fused multiply-add
- **MATH ROUND**: Round
- **MATH FLOOR**: Floor
- **MATH CEIL**: Ceiling
- **MATH TRUNC**: Truncate

### 4. Bit Manipulation (BIT)

Instructions for bit-level operations:

- **BIT AND**: Bitwise AND
- **BIT OR**: Bitwise OR
- **BIT XOR**: Bitwise XOR
- **BIT NOT**: Bitwise NOT
- **BIT ANDN**: Bitwise AND-NOT
- **BIT ORN**: Bitwise OR-NOT
- **BIT XNOR**: Bitwise XNOR
- **BIT SHL**: Shift left logical
- **BIT SHR**: Shift right logical
- **BIT SAR**: Shift arithmetic right
- **BIT ROL**: Rotate left
- **BIT ROR**: Rotate right
- **BIT RCL**: Rotate left through carry
- **BIT RCR**: Rotate right through carry
- **BIT BSWAP**: Byte swap
- **BIT BITREV**: Bit reverse
- **BIT CLZ**: Count leading zeros
- **BIT CTZ**: Count trailing zeros
- **BIT POPCNT**: Population count
- **BIT PARITY**: Parity
- **BIT EXTRACT**: Bit field extract
- **BIT INSERT**: Bit field insert
- **BIT SET**: Set bit
- **BIT CLR**: Clear bit
- **BIT TST**: Test bit
- **BIT TGL**: Toggle bit
- **BIT CMP**: Compare values and set flags

### 5. Vector Operations (VEC)

Instructions for SIMD/vector operations:

- **VEC MOV**: Vector move
- **VEC ADD**: Vector add
- **VEC SUB**: Vector subtract
- **VEC MUL**: Vector multiply
- **VEC DIV**: Vector divide
- **VEC MIN**: Vector minimum
- **VEC MAX**: Vector maximum
- **VEC AND**: Vector bitwise AND
- **VEC OR**: Vector bitwise OR
- **VEC XOR**: Vector bitwise XOR
- **VEC NOT**: Vector bitwise NOT
- **VEC SHL**: Vector shift left
- **VEC SHR**: Vector shift right
- **VEC CMP**: Vector compare
- **VEC ABS**: Vector absolute value
- **VEC SQRT**: Vector square root
- **VEC FMA**: Vector fused multiply-add
- **VEC ROUND**: Vector round
- **VEC INSERT**: Insert element into vector
- **VEC EXTRACT**: Extract element from vector
- **VEC SHUFFLE**: Shuffle vector elements
- **VEC BROADCAST**: Broadcast scalar to vector
- **VEC PACK**: Pack elements with saturation
- **VEC UNPACK**: Unpack elements
- **VEC DOT**: Vector dot product
- **VEC HADD**: Horizontal addition
- **VEC HSUB**: Horizontal subtraction
- **VEC GATHER**: Gather elements
- **VEC SCATTER**: Scatter elements
- **VEC BLEND**: Blend vectors based on mask
- **VEC COMPRESS**: Compress vector using mask
- **VEC EXPAND**: Expand vector using mask
- **VEC ZERO**: Zero vector

### 6. Atomic Operations (ATM)

Instructions for atomic memory operations:

- **ATM ADD**: Atomic add
- **ATM SUB**: Atomic subtract
- **ATM AND**: Atomic AND
- **ATM OR**: Atomic OR
- **ATM XOR**: Atomic XOR
- **ATM NAND**: Atomic NAND
- **ATM XCHG**: Atomic exchange
- **ATM CAS**: Atomic compare-and-swap
- **ATM FETCH_ADD**: Atomic fetch and add
- **ATM FETCH_SUB**: Atomic fetch and subtract
- **ATM FETCH_AND**: Atomic fetch and AND
- **ATM FETCH_OR**: Atomic fetch and OR
- **ATM FETCH_XOR**: Atomic fetch and XOR
- **ATM FETCH_NAND**: Atomic fetch and NAND

### 7. Variable Management (VAR)

Instructions for variable and scope management:

- **VAR DECL**: Variable declaration
- **VAR PMT**: Make variable permanent (extend lifetime)
- **VAR DMT**: Destroy variable (end lifetime)
- **VAR DLT**: Delete variable (release storage)
- **VAR ALIAS**: Create variable alias

### 8. Frame Management (FRAME)

Instructions for function frame management:

- **FRAME ENTER**: Enter function frame
- **FRAME LEAVE**: Leave function frame
- **FRAME SAVE**: Save registers
- **FRAME REST**: Restore registers

### 9. Directives (DIR)

Pseudo-instructions for assembler directives:

- **DIR SECT**: Section declaration
- **DIR ALIGN**: Alignment directive
- **DIR HINT**: Symbol hint (function, global, etc.)
- **DIR LABEL**: Label definition
- **DIR TARGET**: Target architecture specification
- **DIR CONFIG**: Configuration block
- **DIR ABI**: ABI specification
- **DIR INST**: Raw data directive
- **DIR ZERO**: Zero-filled data directive
- **DIR ASCII**: ASCII string directive
- **DIR UNICODE**: Unicode string directive
- **DIR PADD**: Padding directive
- **DIR INCLUDE**: Include file directive
- **DIR MACRO**: Macro definition
- **DIR ENDM**: End macro definition
- **DIR STRUCT**: Structure definition
- **DIR ENDSTRUCT**: End structure definition
- **DIR CONST**: Constant definition

## Instruction Syntax

COIL assembly syntax follows a consistent pattern:

```
[category] [operation] [destination], [source1], [source2], ... -> [output1], [output2], ...
```

Examples:

```
MATH ADD R0, R1, 42       ; R0 = R1 + 42
MEM MOV [R0], R1          ; Store R1 at address in R0
CF BRC EQ label           ; Branch to label if equal flag is set
VAR DECL $0 : int32 = 10  ; Declare int32 variable with initial value 10
```

### System Call Syntax

System calls have a special syntax to specify the ABI and parameter passing:

```
CF SYSC abi-name (arg1, arg2, ...) -> (result1, result2, ...)
```

Example:

```
CF SYSC abi-linux-x86_64 (0x01, 1, str_ptr, str_len) -> ($0)
```

### Target Switch Syntax

Target switching has a specific syntax for complex switching. For the most part this is handled by the user though for example in x86 bios bootloaders you would have to be in charge of changing to protected and completing the long jump to 32 bit code. This program only controls which code is compiled for which target. If however you are working in an environment where an operating system is present then you could utilize the COIL standard extensions to complete your goal. More on this in [isa extension](isaext.md)

## Type System

COIL supports a rich type system that is used in variable declarations and to specify the behavior of instructions:

### Basic Types

- **void**: No value
- **int8**, **int16**, **int32**, **int64**, **int128**: Integer types
- **uint8**, **uint16**, **uint32**, **uint64**, **uint128**: Unsigned integer types
- **fp16**, **fp32**, **fp64**, **fp80**, **fp128**: Floating point types
- **ptr(type)**: Pointer to type
- **vec128(type)**, **vec256(type)**, **vec512(type)**: Vector of type

### Type Declarations

Types are explicitly specified in variable declarations:

```
VAR DECL $0 : int32                ; 32-bit integer
VAR DECL $1 : ptr(int8)            ; Pointer to 8-bit integer
VAR DECL $2 : vec128(float32)      ; 128-bit vector of 32-bit floats
VAR DECL $3 : int32[10]            ; Array of 10 32-bit integers
```

## Operand Types

COIL supports several types of operands:

### Register Operands

```
R0, R1, R2, ...          ; General purpose registers
F0, F1, F2, ...          ; Floating point registers
V0, V1, V2, ...          ; Vector registers
```

### Variable References

```
$0, $1, $2, ...          ; Variable references
```

### Memory Operands

```
[R0]                     ; Memory at address in R0
[R0 + 4]                 ; Memory at address R0+4
[R0 + R1]                ; Memory at address R0+R1
[R0 + R1*4]              ; Memory at address R0+(R1*4)
[R0++]                   ; Memory at address in R0, then increment R0
[--R0]                   ; Decrement R0, then access memory at address in R0
```

### Immediate Values

```
42                       ; Decimal integer
0xFF                     ; Hexadecimal integer
0b1010                   ; Binary integer
3.14159                  ; Floating point
'A'                      ; Character
```

### Symbols and Labels

```
label_name               ; Code label
data_symbol              ; Data symbol
```

## ABI System

COIL explicitly defines Application Binary Interfaces (ABIs) to handle function calls and system calls:

```
DIR ABI abi-name {
  args = [ registers-for-arguments ]
  rets = [ registers-for-return-values ]
  preserved = [ callee-saved-registers ]
  volatile = [ caller-saved-registers ]
  stack_args = order
  stack_cleanup = responsibility
  stack_align = alignment
}
```

Example:

```
DIR ABI abi-linux-x86_64 {
  args = [ R0, R4, R5, R3, R6, R7 ]
  rets = [ R0 ]
  preserved = [ R1, R10, R11, R12, R13 ]
  volatile = [ R0, R2, R3, R4, R5, R6, R7, R8, R9 ]
  stack_align = 16
}
```

## Binary Instruction Format

In the binary format, COIL instructions are encoded as follows:

### Basic Instruction Format

```
[opcode: uint8_t][operand_count: uint8_t][[operand]...]
```

### Operand Format

```
[operand] = [type: uint8_t][qual: uint8_t][data: variable]
```

For operations that require additional information (like conditional branches):

```
[opcode: uint8_t][operand_count: uint8_t][ext_data_size: uint16_t][ext_data][operand]...
```

## Implementation Requirements

COIL assemblers must implement all operations in the instruction set following these guidelines:

1. **Direct Implementation**: Use native instructions when directly supported by hardware.
2. **Optimized Expansion**: Use efficient instruction sequences for operations not directly supported.
3. **Library Call**: For complex operations, a call to a runtime library function may be used.
4. **Emulation**: As a last resort, operations may be emulated using available instructions.

This approach ensures that all COIL instructions can be executed on any supported target architecture, though with varying efficiency.

## Examples

### Basic Arithmetic

```
MATH ADD R0, R1, 42      ; Add immediate to register
MATH SUB R0, R0, R2      ; Subtract register from register
MATH MUL R0, R3, 2       ; Multiply register by immediate
MATH DIV R0, R0, 10      ; Divide register by immediate
```

### Control Flow

```
CF BR label              ; Unconditional branch
CF BRC EQ label          ; Branch if equal
CF CALL function         ; Call function
CF RET                   ; Return from function
```

### Memory Operations

```
MEM MOV R0, [R1]         ; Load from memory to register
MEM MOV [R0], R1         ; Store register to memory
MEM PUSH R0              ; Push register to stack
MEM POP R0               ; Pop stack to register
```

### Variable Management

```
VAR DECL $0 : int32 = 0  ; Declare integer variable
MATH ADD $0, $0, 1       ; Increment variable
VAR DLT $0               ; Delete variable
```

### Vector Operations

```
VAR DECL $0 : vec128(float32)  ; Declare vector variable
VEC BROADCAST $0, 1.0          ; Set all elements to 1.0
VEC ADD $0, $0, V0             ; Add vector register to variable
VEC EXTRACT R0, $0, 2          ; Extract element 2 to scalar register
```

### Atomic Operations

```
ATM ADD [R0], 1                ; Atomic increment
ATM CAS [R0], R1, R2 -> R3     ; Compare and swap
```

## Conclusion

The COIL Instruction Set Architecture provides a powerful, consistent approach to low-level programming across diverse hardware architectures. By integrating with COIL's type system and variable management capabilities, the instruction set eliminates the need for architecture-specific variants while still providing full access to hardware capabilities.

This approach simplifies code generation, improves readability, and enables sophisticated cross-architecture optimization, making COIL an ideal intermediate language for system programming, embedded development, and high-performance computing.