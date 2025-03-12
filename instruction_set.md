# COIL Instruction Set Specification

## 1. Introduction

The COIL Instruction Set defines the operations supported by the COIL intermediate representation. This document specifies the encoding format, instruction categories, and the complete instruction set available in COIL.

## 2. Instruction Encoding

### 2.1 Basic Instruction Format

Each COIL instruction follows this binary format:

```
[opcode: uint8][qualifier: uint8][length: uint8][argument_count: uint8][[argument],...]
```

Where:
- `opcode`: The primary operation code (8 bits)
- `qualifier`: Additional operation specifier or flags (8 bits)
- `length`: Total length of the instruction in bytes (8 bits)
- `argument_count`: Number of arguments that follow (8 bits)
- `argument`: One or more arguments as specified by `argument_count`

### 2.2 Argument Format

Each argument has the format:

```
[value_opcode: uint4][value_type: uint4][data: varies]
```

Where:
- `value_opcode`: Describes how the value is encoded (4 bits)
- `value_type`: The type of the value (4 bits)
- `data`: The actual value data (length depends on the value type)

### 2.3 Value Opcodes

| Opcode | Name | Description |
|--------|------|-------------|
| 0x0 | LITERAL | Literal value |
| 0x1 | REGISTER | Register reference |
| 0x2 | MEMORY | Memory reference |
| 0x3 | IMMEDIATE | Immediate value |
| 0x4 | OFFSET | PC-relative offset |
| 0x5 | LABEL | Label reference |
| 0x6 | VECTOR | Vector of values |
| 0x7 | PREDICATE | Predicate register |
| 0x8 | REFERENCE | Reference to previous value |
| 0x9 | SYMBOL | Symbol reference |
| 0xA | INDIRECT | Indirect reference |
| 0xB-0xF | RESERVED | Reserved for future use |

### 2.4 Value Types

| Type | Name | Description |
|------|------|-------------|
| 0x0 | VOID | No type/void |
| 0x1 | INT8 | 8-bit integer |
| 0x2 | INT16 | 16-bit integer |
| 0x3 | INT32 | 32-bit integer |
| 0x4 | INT64 | 64-bit integer |
| 0x5 | UINT8 | 8-bit unsigned integer |
| 0x6 | UINT16 | 16-bit unsigned integer |
| 0x7 | UINT32 | 32-bit unsigned integer |
| 0x8 | UINT64 | 64-bit unsigned integer |
| 0x9 | FLOAT32 | 32-bit floating point |
| 0xA | FLOAT64 | 64-bit floating point |
| 0xB | BOOL | Boolean |
| 0xC | PTR | Pointer |
| 0xD | VECTOR | Vector type (details in extended data) |
| 0xE | STRUCT | Structure type (details in extended data) |
| 0xF | EXT | Extended type (details in extended data) |

## 3. Instruction Categories

COIL instructions are organized into the following categories:

1. Core Operations
2. Memory Operations
3. Arithmetic Operations
4. Bitwise Operations
5. Control Flow Operations
6. Vector Operations
7. Matrix Operations
8. Parallel Processing Operations
9. Specialized Hardware Operations
10. Type Conversion Operations
11. Synchronization Operations

## 4. Core Operations

### 4.1 Basic Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x00 | NOP | No operation | `NOP` |
| 0x01 | MOV | Move/copy value | `MOV dst, src` |
| 0x02 | SWAP | Swap values | `SWAP a, b` |
| 0x03 | PUSH | Push to stack | `PUSH value` |
| 0x04 | POP | Pop from stack | `POP dst` |
| 0x05 | DUP | Duplicate top of stack | `DUP` |
| 0x06 | EXTRACT | Extract component | `EXTRACT dst, src, idx` |
| 0x07 | INSERT | Insert component | `INSERT dst, src, idx, val` |

## 5. Memory Operations

### 5.1 Basic Memory Access

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x10 | LOAD | Load from memory | `LOAD dst, [addr]` |
| 0x11 | STORE | Store to memory | `STORE [addr], src` |
| 0x12 | LEA | Load effective address | `LEA dst, [base+offset]` |

### 5.2 Advanced Memory Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x13 | ALLOC | Allocate memory | `ALLOC dst, size, [align]` |
| 0x14 | FREE | Free allocated memory | `FREE addr` |
| 0x15 | MEMCPY | Copy memory block | `MEMCPY dst, src, size` |
| 0x16 | MEMSET | Set memory block | `MEMSET dst, value, size` |
| 0x17 | MEMZERO | Zero memory block | `MEMZERO dst, size` |
| 0x18 | PREFETCH | Prefetch memory | `PREFETCH [addr], hint` |
| 0x19 | FENCE | Memory fence | `FENCE type` |
| 0x1A | ATOMIC | Atomic memory operation | `ATOMIC op, [addr], val` |
| 0x1B | CMPXCHG | Compare and exchange | `CMPXCHG [addr], cmp, val` |

### 5.3 Advanced Memory Access

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x1C | GATHER | Gather from indexed addresses | `GATHER dst, base, indices` |
| 0x1D | SCATTER | Scatter to indexed addresses | `SCATTER base, indices, values` |
| 0x1E | STRIDED_LOAD | Load with stride | `STRIDED_LOAD dst, [addr], stride, count` |
| 0x1F | STRIDED_STORE | Store with stride | `STRIDED_STORE [addr], src, stride, count` |

## 6. Arithmetic Operations

### 6.1 Integer Arithmetic

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x20 | ADD | Add | `ADD dst, a, b` |
| 0x21 | SUB | Subtract | `SUB dst, a, b` |
| 0x22 | MUL | Multiply | `MUL dst, a, b` |
| 0x23 | DIV | Divide | `DIV dst, a, b` |
| 0x24 | REM | Remainder | `REM dst, a, b` |
| 0x25 | NEG | Negate | `NEG dst, src` |
| 0x26 | ABS | Absolute value | `ABS dst, src` |
| 0x27 | MIN | Minimum | `MIN dst, a, b` |
| 0x28 | MAX | Maximum | `MAX dst, a, b` |
| 0x29 | ADDC | Add with carry | `ADDC dst, a, b, cin` |
| 0x2A | SUBB | Subtract with borrow | `SUBB dst, a, b, bin` |
| 0x2B | MULH | Multiply high bits | `MULH dst, a, b` |
| 0x2C | INC | Increment | `INC dst` |
| 0x2D | DEC | Decrement | `DEC dst` |

### 6.2 Floating-Point Arithmetic

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x30 | FADD | Floating-point add | `FADD dst, a, b` |
| 0x31 | FSUB | Floating-point subtract | `FSUB dst, a, b` |
| 0x32 | FMUL | Floating-point multiply | `FMUL dst, a, b` |
| 0x33 | FDIV | Floating-point divide | `FDIV dst, a, b` |
| 0x34 | FREM | Floating-point remainder | `FREM dst, a, b` |
| 0x35 | FNEG | Floating-point negate | `FNEG dst, src` |
| 0x36 | FABS | Floating-point absolute | `FABS dst, src` |
| 0x37 | FSQRT | Floating-point square root | `FSQRT dst, src` |
| 0x38 | FMIN | Floating-point minimum | `FMIN dst, a, b` |
| 0x39 | FMAX | Floating-point maximum | `FMAX dst, a, b` |
| 0x3A | FMADD | Fused multiply-add | `FMADD dst, a, b, c` |
| 0x3B | FMSUB | Fused multiply-subtract | `FMSUB dst, a, b, c` |
| 0x3C | RSQRT | Reciprocal square root | `RSQRT dst, src` |
| 0x3D | RCP | Reciprocal | `RCP dst, src` |
| 0x3E | SIN | Sine | `SIN dst, src` |
| 0x3F | COS | Cosine | `COS dst, src` |
| 0x40 | TAN | Tangent | `TAN dst, src` |
| 0x41 | EXP | Exponential | `EXP dst, src` |
| 0x42 | LOG | Logarithm | `LOG dst, src` |
| 0x43 | POW | Power | `POW dst, base, exp` |

## 7. Bitwise Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x50 | AND | Bitwise AND | `AND dst, a, b` |
| 0x51 | OR | Bitwise OR | `OR dst, a, b` |
| 0x52 | XOR | Bitwise XOR | `XOR dst, a, b` |
| 0x53 | NOT | Bitwise NOT | `NOT dst, src` |
| 0x54 | SHL | Shift left | `SHL dst, src, amount` |
| 0x55 | SHR | Logical shift right | `SHR dst, src, amount` |
| 0x56 | SAR | Arithmetic shift right | `SAR dst, src, amount` |
| 0x57 | ROL | Rotate left | `ROL dst, src, amount` |
| 0x58 | ROR | Rotate right | `ROR dst, src, amount` |
| 0x59 | BSWAP | Byte swap | `BSWAP dst, src` |
| 0x5A | BITREV | Bit reverse | `BITREV dst, src` |
| 0x5B | CLZ | Count leading zeros | `CLZ dst, src` |
| 0x5C | CTZ | Count trailing zeros | `CTZ dst, src` |
| 0x5D | POPCNT | Population count | `POPCNT dst, src` |
| 0x5E | PDEP | Parallel bits deposit | `PDEP dst, src, mask` |
| 0x5F | PEXT | Parallel bits extract | `PEXT dst, src, mask` |

## 8. Control Flow Operations

### 8.1 Basic Control Flow

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x60 | JMP | Unconditional jump | `JMP target` |
| 0x61 | BR | Conditional branch | `BR cond, true_target, false_target` |
| 0x62 | CALL | Call function | `CALL target, [args...]` |
| 0x63 | RET | Return from function | `RET [value]` |
| 0x64 | TAILCALL | Tail call optimization | `TAILCALL target, [args...]` |

### 8.2 Advanced Control Flow

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x65 | SWITCH | Switch statement | `SWITCH value, [cases...]` |
| 0x66 | LOOP | Start loop | `LOOP label, count` |
| 0x67 | LOOP_END | End loop | `LOOP_END label` |
| 0x68 | BREAK | Break from loop | `BREAK [label]` |
| 0x69 | CONTINUE | Continue loop | `CONTINUE [label]` |
| 0x6A | TRY | Try block start | `TRY label` |
| 0x6B | CATCH | Catch block | `CATCH type, handler` |
| 0x6C | THROW | Throw exception | `THROW value` |
| 0x6D | FINALLY | Finally block | `FINALLY label` |

### 8.3 Predication

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x6E | PREDICATE | Set predicate register | `PREDICATE dst, cond` |
| 0x6F | PRED_EXEC | Predicated execution | `PRED_EXEC pred, instr` |
| 0x70 | SELECT | Select based on condition | `SELECT dst, cond, true_val, false_val` |

## 9. Vector Operations

### 9.1 Vector Creation and Manipulation

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x80 | VLOAD | Load vector | `VLOAD dst, [addr]` |
| 0x81 | VSTORE | Store vector | `VSTORE [addr], src` |
| 0x82 | VSPLAT | Splat scalar to vector | `VSPLAT dst, scalar` |
| 0x83 | VEXTRACT | Extract element from vector | `VEXTRACT dst, vec, idx` |
| 0x84 | VINSERT | Insert element into vector | `VINSERT dst, vec, idx, val` |
| 0x85 | VSHUFFLE | Shuffle vector elements | `VSHUFFLE dst, vec, mask` |

### 9.2 Vector Arithmetic

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x86 | VADD | Vector add | `VADD dst, a, b` |
| 0x87 | VSUB | Vector subtract | `VSUB dst, a, b` |
| 0x88 | VMUL | Vector multiply | `VMUL dst, a, b` |
| 0x89 | VDIV | Vector divide | `VDIV dst, a, b` |
| 0x8A | VDOT | Vector dot product | `VDOT dst, a, b` |
| 0x8B | VCROSS | Vector cross product | `VCROSS dst, a, b` |
| 0x8C | VFMADD | Vector fused multiply-add | `VFMADD dst, a, b, c` |
| 0x8D | VHADD | Horizontal vector add | `VHADD dst, vec` |

### 9.3 Vector Comparisons and Masking

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x8E | VCMP | Vector compare | `VCMP dst, a, b, op` |
| 0x8F | VBLEND | Vector blend | `VBLEND dst, a, b, mask` |
| 0x90 | VMASK | Apply vector mask | `VMASK dst, src, mask` |

## 10. Matrix Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xA0 | MLOAD | Load matrix | `MLOAD dst, [addr]` |
| 0xA1 | MSTORE | Store matrix | `MSTORE [addr], src` |
| 0xA2 | MADD | Matrix add | `MADD dst, a, b` |
| 0xA3 | MSUB | Matrix subtract | `MSUB dst, a, b` |
| 0xA4 | MMUL | Matrix multiply | `MMUL dst, a, b` |
| 0xA5 | MTRANS | Matrix transpose | `MTRANS dst, src` |
| 0xA6 | MINV | Matrix inverse | `MINV dst, src` |
| 0xA7 | MDET | Matrix determinant | `MDET dst, src` |

## 11. Parallel Processing Operations

### 11.1 Thread Management

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xB0 | THREAD_ID | Get thread ID | `THREAD_ID dst, [dimension]` |
| 0xB1 | THREAD_COUNT | Get thread count | `THREAD_COUNT dst, [dimension]` |
| 0xB2 | THREAD_GROUP | Get thread group ID | `THREAD_GROUP dst, [dimension]` |
| 0xB3 | THREAD_BARRIER | Thread barrier | `THREAD_BARRIER [scope]` |
| 0xB4 | THREAD_FENCE | Thread memory fence | `THREAD_FENCE [scope]` |

### 11.2 Work Distribution

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xB5 | DISPATCH | Launch parallel work | `DISPATCH func, dim, global, local` |
| 0xB6 | REDUCE | Parallel reduction | `REDUCE dst, src, op` |
| 0xB7 | SCAN | Parallel prefix scan | `SCAN dst, src, op` |
| 0xB8 | VOTE | Thread vote operation | `VOTE dst, pred, op` |
| 0xB9 | SHUFFLE | Thread shuffle | `SHUFFLE dst, src, lane` |

## 12. Specialized Hardware Operations

### 12.1 Graphics Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xC0 | TEXTURE_SAMPLE | Sample from texture | `TEXTURE_SAMPLE dst, tex, coord` |
| 0xC1 | INTERPOLATE | Interpolate values | `INTERPOLATE dst, attr, bary` |
| 0xC2 | RASTERIZE | Rasterization operations | `RASTERIZE dst, prim, attribs` |

### 12.2 Machine Learning Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xD0 | MATMUL | Tensor matrix multiply | `MATMUL dst, a, b` |
| 0xD1 | CONV | Convolution | `CONV dst, input, filter, params` |
| 0xD2 | POOL | Pooling operation | `POOL dst, input, size, stride, type` |
| 0xD3 | ACTIVATION | Activation function | `ACTIVATION dst, input, type` |
| 0xD4 | QUANTIZE | Quantize values | `QUANTIZE dst, src, params` |
| 0xD5 | DEQUANTIZE | Dequantize values | `DEQUANTIZE dst, src, params` |

### 12.3 Security Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xE0 | ENCRYPT | Hardware encryption | `ENCRYPT dst, src, key, algo` |
| 0xE1 | DECRYPT | Hardware decryption | `DECRYPT dst, src, key, algo` |
| 0xE2 | HASH | Hardware hash function | `HASH dst, src, algo` |
| 0xE3 | RANDOM | Hardware random number | `RANDOM dst, [params]` |

## 13. Type Conversion Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xF0 | TRUNC | Truncate value | `TRUNC dst, src` |
| 0xF1 | ZEXT | Zero extend | `ZEXT dst, src` |
| 0xF2 | SEXT | Sign extend | `SEXT dst, src` |
| 0xF3 | FPTOUI | Float to unsigned int | `FPTOUI dst, src` |
| 0xF4 | FPTOSI | Float to signed int | `FPTOSI dst, src` |
| 0xF5 | UITOFP | Unsigned int to float | `UITOFP dst, src` |
| 0xF6 | SITOFP | Signed int to float | `SITOFP dst, src` |
| 0xF7 | FPTRUNC | Float truncate | `FPTRUNC dst, src` |
| 0xF8 | FPEXT | Float extend | `FPEXT dst, src` |
| 0xF9 | BITCAST | Bitcast | `BITCAST dst, src` |
| 0xFA | INTTOPTR | Int to pointer | `INTTOPTR dst, src` |
| 0xFB | PTRTOINT | Pointer to int | `PTRTOINT dst, src` |

## 14. Instruction Qualifiers

Instruction qualifiers modify the behavior of instructions. The 8-bit qualifier field can be used to specify:

### 14.1 Memory Access Qualifiers

| Bit | Name | Description |
|-----|------|-------------|
| 0 | VOLATILE | Prevent optimization of memory accesses |
| 1 | ALIGNED | Memory access is aligned |
| 2 | ATOMIC | Memory access is atomic |
| 3 | COHERENT | Enforce memory coherence |

### 14.2 Floating-Point Qualifiers

| Bit | Name | Description |
|-----|------|-------------|
| 0 | FAST_MATH | Allow fast math optimizations |
| 1 | PRECISE | Maintain precision |
| 2 | DENORMAL | Allow denormal values |
| 3-4 | ROUNDING | Rounding mode (0=nearest, 1=zero, 2=up, 3=down) |

### 14.3 Optimization Qualifiers

| Bit | Name | Description |
|-----|------|-------------|
| 0 | INLINE | Hint to inline |
| 1 | UNROLL | Hint to unroll loop |
| 2 | VECTORIZE | Hint to vectorize |
| 3 | LIKELY | Branch is likely taken |
| 4 | UNLIKELY | Branch is unlikely taken |

## 15. Assembler Syntax

### 15.1 Basic Syntax

COIL assembler uses a simple syntax:

```
[label:] mnemonic [qualifier] operand1, operand2, ... ; comment
```

Example:
```
start:  MOV     r0, 42          ; Initialize r0 to 42
        ADD     r1, r0, 10      ; r1 = r0 + 10
        JUMP    next            ; Jump to next
next:   RETURN  r1              ; Return r1
```

### 15.2 Qualifiers

Qualifiers are specified in angle brackets:

```
mnemonic<qualifier1,qualifier2> operands
```

Example:
```
LOAD<volatile,aligned> r0, [addr]
FADD<fast_math> r0, r1, r2
```

### 15.3 Type Annotations

Type annotations are specified after operands with a colon:

```
mnemonic dst:type, src:type
```

Example:
```
MOV     r0:i32, 42
FADD    r0:f32, r1:f32, r2:f32
```

### 15.4 Vector Operations

Vector operations use angle bracket notation for element access:

```
MOV     v0<0>, 1.0     ; Set first element of v0 to 1.0
ADD     v1, v2, v3     ; Element-wise vector addition
```

### 15.5 Memory Operations

Memory addresses are enclosed in square brackets:

```
LOAD    r0, [r1]       ; Load from address in r1
STORE   [r1+8], r0     ; Store r0 at address r1+8
```

## 16. Implementation Guidelines

When implementing a COIL instruction set processor:

1. Verify instruction length and argument count for correctness
2. Handle qualifiers as modifiers to the basic operation
3. Implement fallbacks for unsupported hardware operations
4. Use the `length` field to skip unknown instructions
5. Validate type compatibility for operations
6. Handle different value encodings properly
7. Implement proper error handling for invalid instructions

## 17. Versioning

The COIL instruction set uses versioning to maintain compatibility:

- Instructions 0x00-0x7F are part of the core instruction set
- Instructions 0x80-0xBF are for SIMD and parallel processing
- Instructions 0xC0-0xEF are for specialized hardware operations
- Instructions 0xF0-0xFF are for type conversions and utility operations

New instructions will be added without changing existing opcodes to maintain backward compatibility.