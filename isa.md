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

Target switching has a specific syntax for complex switching:

```
CF SWITCH target entry_point MAP(var1 -> var2, ...)
```

Example:

```
CF SWITCH arm64 gpu_function MAP($0 -> $0, $1 -> $1)
```

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

COIL instructions are encoded in a specific binary format designed for efficient parsing and compact representation. Each instruction follows a consistent encoding pattern:

### Basic Instruction Format

```c
/**
 * @brief Basic instruction format
 */
typedef struct coil_instruction {
  uint8_t opcode;            /**< Primary opcode */
  uint8_t operand_count;     /**< Number of operands */
  uint16_t ext_data_size;    /**< Size of extended data (0 if none) */
  uint8_t operands[];        /**< Variable-length operand data */
  /* Extended data follows operands if ext_data_size > 0 */
} coil_instruction_t;
```

### Detailed Encoding

The opcode is encoded as a 8-bit value with specific bit patterns:

```c
/**
 * @brief Opcode bit layout
 * 
 * [7:5] - Category (3 bits)
 * [4:0] - Operation (5 bits)
 */

/* Opcode category bits (bits 7-5) */
#define COIL_CAT_CF    0x00  /* 000xxxxx - Control Flow */
#define COIL_CAT_MEM   0x20  /* 001xxxxx - Memory Operations */
#define COIL_CAT_MATH  0x40  /* 010xxxxx - Arithmetic Operations */
#define COIL_CAT_BIT   0x60  /* 011xxxxx - Bit Manipulation */
#define COIL_CAT_VEC   0x80  /* 100xxxxx - Vector Operations */
#define COIL_CAT_ATM   0xA0  /* 101xxxxx - Atomic Operations */
#define COIL_CAT_VAR   0xC0  /* 110xxxxx - Variable Management */
#define COIL_CAT_FRAME 0xE0  /* 111xxxxx - Frame Management */

/* Control Flow operations (bits 4-0) */
#define COIL_OP_CF_BR     0x00  /* 00000000 - Unconditional branch */
#define COIL_OP_CF_BRC    0x01  /* 00000001 - Conditional branch */
#define COIL_OP_CF_CALL   0x02  /* 00000010 - Function call */
#define COIL_OP_CF_RET    0x03  /* 00000011 - Return from function */
#define COIL_OP_CF_INT    0x04  /* 00000100 - Software interrupt */
#define COIL_OP_CF_IRET   0x05  /* 00000101 - Return from interrupt */
#define COIL_OP_CF_HLT    0x06  /* 00000110 - Halt execution */
#define COIL_OP_CF_SYSC   0x07  /* 00000111 - System call */
#define COIL_OP_CF_TRAP   0x08  /* 00001000 - Trap to exception handler */
#define COIL_OP_CF_WFE    0x09  /* 00001001 - Wait for event */
#define COIL_OP_CF_SEV    0x0A  /* 00001010 - Send event */
#define COIL_OP_CF_FENCE  0x0B  /* 00001011 - Memory fence */
#define COIL_OP_CF_YIELD  0x0C  /* 00001100 - Yield execution */
#define COIL_OP_CF_SWITCH 0x0D  /* 00001101 - Switch target architecture */
#define COIL_OP_CF_NOP    0x0E  /* 00001110 - No operation */

/* Memory operations (bits 4-0) */
#define COIL_OP_MEM_MOV      0x00  /* 00100000 - Move data */
#define COIL_OP_MEM_PUSH     0x01  /* 00100001 - Push to stack */
#define COIL_OP_MEM_POP      0x02  /* 00100010 - Pop from stack */
#define COIL_OP_MEM_LOAD     0x03  /* 00100011 - Load from memory */
#define COIL_OP_MEM_STORE    0x04  /* 00100100 - Store to memory */
#define COIL_OP_MEM_PREFETCH 0x05  /* 00100101 - Prefetch memory */
#define COIL_OP_MEM_EXCHANGE 0x06  /* 00100110 - Exchange values */
#define COIL_OP_MEM_COMPARE  0x07  /* 00100111 - Compare and update flags */
#define COIL_OP_MEM_TEST     0x08  /* 00101000 - Test and update flags */
#define COIL_OP_MEM_FILL     0x09  /* 00101001 - Fill memory block */
#define COIL_OP_MEM_COPY     0x0A  /* 00101010 - Copy memory block */
#define COIL_OP_MEM_ZERO     0x0B  /* 00101011 - Zero memory block */
#define COIL_OP_MEM_PUSH_STATE 0x0C /* 00101100 - Save register state */
#define COIL_OP_MEM_POP_STATE  0x0D /* 00101101 - Restore register state */
#define COIL_OP_MEM_OUT      0x0E  /* 00101110 - Output to I/O port */
#define COIL_OP_MEM_IN       0x0F  /* 00101111 - Input from I/O port */
#define COIL_OP_MEM_LGDT     0x10  /* 00110000 - Load global descriptor table */

/* Similar definitions for other categories... */
```

### Operand Encoding

Each operand is encoded with a type byte followed by operand-specific data:

```c
/**
 * @brief Operand type byte
 * 
 * [7:6] - Operand class (2 bits)
 * [5:0] - Operand type details (6 bits)
 */

/* Operand classes (bits 7-6) */
#define COIL_OPERAND_REGISTER  0x00  /* 00xxxxxx - Register operand */
#define COIL_OPERAND_IMMEDIATE 0x40  /* 01xxxxxx - Immediate value */
#define COIL_OPERAND_MEMORY    0x80  /* 10xxxxxx - Memory operand */
#define COIL_OPERAND_VARIABLE  0xC0  /* 11xxxxxx - Variable reference */

/* Register operand types (bits 5-0) */
#define COIL_REG_GP      0x00  /* 00000000 - General purpose register */
#define COIL_REG_FP      0x01  /* 00000001 - Floating point register */
#define COIL_REG_VEC     0x02  /* 00000010 - Vector register */
#define COIL_REG_SPECIAL 0x03  /* 00000011 - Special register */

/* Immediate operand types (bits 5-0) */
#define COIL_IMM_INT8    0x00  /* 01000000 - 8-bit integer */
#define COIL_IMM_INT16   0x01  /* 01000001 - 16-bit integer */
#define COIL_IMM_INT32   0x02  /* 01000010 - 32-bit integer */
#define COIL_IMM_INT64   0x03  /* 01000011 - 64-bit integer */
#define COIL_IMM_FLOAT32 0x04  /* 01000100 - 32-bit float */
#define COIL_IMM_FLOAT64 0x05  /* 01000101 - 64-bit float */
#define COIL_IMM_SYMBOL  0x06  /* 01000110 - Symbol reference */

/* Memory operand types (bits 5-0) */
#define COIL_MEM_DIRECT     0x00  /* 10000000 - Direct memory access [addr] */
#define COIL_MEM_REG        0x01  /* 10000001 - Register indirect [reg] */
#define COIL_MEM_REG_DISP   0x02  /* 10000010 - Register + displacement [reg+disp] */
#define COIL_MEM_REG_REG    0x03  /* 10000011 - Register + register [reg1+reg2] */
#define COIL_MEM_REG_REG_SCALE 0x04 /* 10000100 - Register + scaled register [reg1+reg2*scale] */
#define COIL_MEM_REG_PRE_INC  0x05 /* 10000101 - Pre-increment [++reg] */
#define COIL_MEM_REG_PRE_DEC  0x06 /* 10000110 - Pre-decrement [--reg] */
#define COIL_MEM_REG_POST_INC 0x07 /* 10000111 - Post-increment [reg++] */
#define COIL_MEM_REG_POST_DEC 0x08 /* 10001000 - Post-decrement [reg--] */

/* Variable operand types (bits 5-0) */
#define COIL_VAR_DIRECT    0x00  /* 11000000 - Direct variable reference */
#define COIL_VAR_ADDR      0x01  /* 11000001 - Variable address */
#define COIL_VAR_ELEM      0x02  /* 11000010 - Array element */
#define COIL_VAR_FIELD     0x03  /* 11000011 - Structure field */
```

### Register Operand Format

```c
/**
 * @brief Register operand format
 */
typedef struct coil_reg_operand {
  uint8_t type;      /**< Type byte (COIL_OPERAND_REGISTER | register type) */
  uint8_t reg_id;    /**< Register ID */
  uint8_t flags;     /**< Register flags (e.g., partial register access) */
} coil_reg_operand_t;
```

### Immediate Operand Format

```c
/**
 * @brief Immediate operand format (example for 32-bit integer)
 */
typedef struct coil_imm_i32_operand {
  uint8_t type;      /**< Type byte (COIL_OPERAND_IMMEDIATE | COIL_IMM_INT32) */
  int32_t value;     /**< Immediate value */
} coil_imm_i32_operand_t;
```

### Memory Operand Format

```c
/**
 * @brief Memory operand format (example for register + displacement)
 */
typedef struct coil_mem_reg_disp_operand {
  uint8_t type;      /**< Type byte (COIL_OPERAND_MEMORY | COIL_MEM_REG_DISP) */
  uint8_t reg_id;    /**< Base register ID */
  int32_t disp;      /**< Displacement value */
} coil_mem_reg_disp_operand_t;
```

### Variable Operand Format

```c
/**
 * @brief Variable operand format
 */
typedef struct coil_var_operand {
  uint8_t type;      /**< Type byte (COIL_OPERAND_VARIABLE | variable type) */
  uint8_t var_id;    /**< Variable ID */
} coil_var_operand_t;
```

### Extended Data Format

For instructions that require additional information (like condition codes for branches), extended data is appended after the operands:

```c
/**
 * @brief Conditional branch extended data
 */
typedef struct coil_brc_ext_data {
  uint8_t condition_code;  /**< Condition code (EQ, NE, etc.) */
  uint8_t prediction;      /**< Branch prediction hint */
} coil_brc_ext_data_t;
```

### Complete Instruction Example

Here's an example of how a complete instruction is encoded:

```c
/**
 * @brief Example: MATH ADD R0, R1, 42
 */

/* Instruction header */
struct {
  uint8_t opcode = 0x40;        /* COIL_CAT_MATH | 0x00 (ADD) */
  uint8_t operand_count = 3;    /* 3 operands */
  uint16_t ext_data_size = 0;   /* No extended data */
} header;

/* Operand 1: Register R0 */
struct {
  uint8_t type = 0x00;          /* COIL_OPERAND_REGISTER | COIL_REG_GP */
  uint8_t reg_id = 0x00;        /* R0 */
  uint8_t flags = 0x00;         /* No flags */
} operand1;

/* Operand 2: Register R1 */
struct {
  uint8_t type = 0x00;          /* COIL_OPERAND_REGISTER | COIL_REG_GP */
  uint8_t reg_id = 0x01;        /* R1 */
  uint8_t flags = 0x00;         /* No flags */
} operand2;

/* Operand 3: Immediate value 42 */
struct {
  uint8_t type = 0x40;          /* COIL_OPERAND_IMMEDIATE | COIL_IMM_INT32 */
  int32_t value = 42;           /* Value 42 */
} operand3;
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

## Instruction Encoding Examples

Here are binary encoding examples for several common instructions:

### Example 1: MATH ADD R0, R1, 42

```
/* Binary representation */
0x40                    /* Opcode: MATH ADD */
0x03                    /* 3 operands */
0x00 0x00               /* No extended data */
0x00 0x00 0x00          /* Operand 1: R0 (register) */
0x00 0x01 0x00          /* Operand 2: R1 (register) */
0x40 0x2A 0x00 0x00 0x00 /* Operand 3: 42 (immediate int32) */
```

### Example 2: CF BRC EQ label

```
/* Binary representation */
0x01                    /* Opcode: CF BRC */
0x01                    /* 1 operand */
0x02 0x00               /* 2 bytes of extended data */
0x40 0xXX 0xXX 0xXX 0xXX /* Operand 1: label (immediate symbol) */
0x00 0x00               /* Extended data: condition=EQ(0), prediction=0 */
```

### Example 3: MEM MOV [R0], R1

```
/* Binary representation */
0x20                    /* Opcode: MEM MOV */
0x02                    /* 2 operands */
0x00 0x00               /* No extended data */
0x80 0x01 0x00          /* Operand 1: [R0] (memory, register indirect) */
0x00 0x01 0x00          /* Operand 2: R1 (register) */
```

### Example 4: VAR DECL $0 : int32 = 10

```
/* Binary representation */
0xC0                    /* Opcode: VAR DECL */
0x01                    /* 1 operand */
0x06 0x00               /* 6 bytes of extended data */
0xC0 0x00               /* Operand 1: $0 (variable) */
0x02 0x00               /* Extended data: type=int32 */
0x0A 0x00 0x00 0x00     /* Extended data: initial value=10 */
```

## Conclusion

The COIL Instruction Set Architecture provides a powerful, consistent approach to low-level programming across diverse hardware architectures. By integrating with COIL's type system and variable management capabilities, the instruction set eliminates the need for architecture-specific variants while still providing full access to hardware capabilities.

This approach simplifies code generation, improves readability, and enables sophisticated cross-architecture optimization, making COIL an ideal intermediate language for system programming, embedded development, and high-performance computing.