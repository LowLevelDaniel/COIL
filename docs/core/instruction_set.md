# COIL Instruction Set

The COIL Instruction Set defines the operations supported by the COIL intermediate representation. All instructions can execute **without runtime support**, making them suitable for bare-metal environments.

## Instruction Encoding

### Basic Format

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

### Argument Format

Each argument has the format:

```
[value_opcode: uint4][value_type: uint4][data: varies]
```

Where:
- `value_opcode`: Describes how the value is encoded (4 bits)
- `value_type`: The type of the value (4 bits)
- `data`: The actual value data (length depends on the value type)

## Instruction Categories

COIL instructions are organized into the following categories:

1. Core Operations
2. Memory Operations
3. Arithmetic Operations
4. Bitwise Operations
5. Control Flow Operations
6. Vector Operations
7. Type Conversion Operations
8. Hardware-Specific Operations

## Core Operations

Basic operations for data movement and manipulation:

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

## Memory Operations

Operations for memory access and management:

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x10 | LOAD | Load from memory | `LOAD dst, [addr]` |
| 0x11 | STORE | Store to memory | `STORE [addr], src` |
| 0x12 | LEA | Load effective address | `LEA dst, [base+offset]` |
| 0x13 | ALLOC | Allocate memory | `ALLOC dst, size, [align]` |
| 0x14 | FREE | Free allocated memory | `FREE addr` |
| 0x15 | MEMCPY | Copy memory block | `MEMCPY dst, src, size` |
| 0x16 | MEMSET | Set memory block | `MEMSET dst, value, size` |
| 0x19 | FENCE | Memory fence | `FENCE type` |
| 0x1A | ATOMIC | Atomic memory operation | `ATOMIC op, [addr], val` |

## Arithmetic Operations

### Integer Arithmetic

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

### Floating-Point Arithmetic

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x30 | FADD | Floating-point add | `FADD dst, a, b` |
| 0x31 | FSUB | Floating-point subtract | `FSUB dst, a, b` |
| 0x32 | FMUL | Floating-point multiply | `FMUL dst, a, b` |
| 0x33 | FDIV | Floating-point divide | `FDIV dst, a, b` |
| 0x35 | FNEG | Floating-point negate | `FNEG dst, src` |
| 0x36 | FABS | Floating-point absolute | `FABS dst, src` |
| 0x37 | FSQRT | Floating-point square root | `FSQRT dst, src` |

## Bitwise Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x50 | AND | Bitwise AND | `AND dst, a, b` |
| 0x51 | OR | Bitwise OR | `OR dst, a, b` |
| 0x52 | XOR | Bitwise XOR | `XOR dst, a, b` |
| 0x53 | NOT | Bitwise NOT | `NOT dst, src` |
| 0x54 | SHL | Shift left | `SHL dst, src, amount` |
| 0x55 | SHR | Logical shift right | `SHR dst, src, amount` |
| 0x56 | SAR | Arithmetic shift right | `SAR dst, src, amount` |
| 0x5B | CLZ | Count leading zeros | `CLZ dst, src` |
| 0x5C | CTZ | Count trailing zeros | `CTZ dst, src` |
| 0x5D | POPCNT | Population count | `POPCNT dst, src` |

## Control Flow Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x60 | JMP | Unconditional jump | `JMP target` |
| 0x61 | BR | Conditional branch | `BR cond, true_target, false_target` |
| 0x62 | CALL | Call function | `CALL target, [args...]` |
| 0x63 | RET | Return from function | `RET [value]` |
| 0x65 | SWITCH | Switch statement | `SWITCH value, [cases...]` |
| 0x66 | LOOP | Start loop | `LOOP label, count` |
| 0x67 | LOOP_END | End loop | `LOOP_END label` |
| 0x70 | SELECT | Select based on condition | `SELECT dst, cond, true_val, false_val` |

## Vector Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0x80 | VLOAD | Load vector | `VLOAD dst, [addr]` |
| 0x81 | VSTORE | Store vector | `VSTORE [addr], src` |
| 0x82 | VSPLAT | Splat scalar to vector | `VSPLAT dst, scalar` |
| 0x86 | VADD | Vector add | `VADD dst, a, b` |
| 0x87 | VSUB | Vector subtract | `VSUB dst, a, b` |
| 0x88 | VMUL | Vector multiply | `VMUL dst, a, b` |
| 0x89 | VDIV | Vector divide | `VDIV dst, a, b` |
| 0x8A | VDOT | Vector dot product | `VDOT dst, a, b` |

## Type Conversion Operations

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xF0 | TRUNC | Truncate value | `TRUNC dst, src` |
| 0xF1 | ZEXT | Zero extend | `ZEXT dst, src` |
| 0xF2 | SEXT | Sign extend | `SEXT dst, src` |
| 0xF3 | FPTOUI | Float to unsigned int | `FPTOUI dst, src` |
| 0xF4 | FPTOSI | Float to signed int | `FPTOSI dst, src` |
| 0xF5 | UITOFP | Unsigned int to float | `UITOFP dst, src` |
| 0xF6 | SITOFP | Signed int to float | `SITOFP dst, src` |
| 0xF9 | BITCAST | Bitcast | `BITCAST dst, src` |

## Hardware-Specific Operations

Operations for direct hardware access in bare-metal environments:

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|--------|
| 0xE0 | IO_READ | Read from I/O port | `IO_READ dst, port` |
| 0xE1 | IO_WRITE | Write to I/O port | `IO_WRITE port, value` |
| 0xE2 | MMIO_READ | Read from memory-mapped I/O | `MMIO_READ dst, [addr]` |
| 0xE3 | MMIO_WRITE | Write to memory-mapped I/O | `MMIO_WRITE [addr], value` |
| 0xE4 | INTERRUPT | Generate interrupt | `INTERRUPT vector` |
| 0xE5 | CLI | Clear interrupt flag | `CLI` |
| 0xE6 | STI | Set interrupt flag | `STI` |
| 0xE7 | HLT | Halt processor | `HLT` |

## Instruction Qualifiers

Instruction qualifiers modify the behavior of instructions. The 8-bit qualifier field specifies:

### Memory Access Qualifiers

| Bit | Name | Description |
|-----|------|-------------|
| 0 | VOLATILE | Prevent optimization of memory accesses |
| 1 | ALIGNED | Memory access is aligned |
| 2 | ATOMIC | Memory access is atomic |
| 3 | COHERENT | Enforce memory coherence |

### Floating-Point Qualifiers

| Bit | Name | Description |
|-----|------|-------------|
| 0 | FAST_MATH | Allow fast math optimizations |
| 1 | PRECISE | Maintain precision |
| 2 | DENORMAL | Allow denormal values |

## Assembler Syntax

COIL assembler uses a simple syntax:

```
[label:] mnemonic [qualifier] operand1, operand2, ... ; comment
```

Example:
```
start:  MOV     r0, 42          ; Initialize r0 to 42
        ADD     r1, r0, 10      ; r1 = r0 + 10
        JMP     next            ; Jump to next
next:   RET     r1              ; Return r1
```

### Qualifiers in Syntax

Qualifiers are specified in angle brackets:

```
LOAD<volatile> r0, [addr]
FADD<fast_math> r0, r1, r2
```

### Type Annotations

Type annotations are specified after operands with a colon:

```
MOV     r0:i32, 42
FADD    r0:f32, r1:f32, r2:f32
```

## Bare-Metal Examples

### UEFI Bootloader Example

```
; UEFI bootloader entry point
uefi_main:
    ; Get UEFI system table from parameters
    MOV     r0, [args+0]        ; Image handle
    MOV     r1, [args+8]        ; System table pointer
    
    ; Call UEFI console output function
    LEA     r2, [hello_msg]     ; Load message address
    MOV     r3, [r1+0x40]       ; Get ConOut pointer from system table
    LEA     r4, [r3+0x8]        ; Get OutputString function pointer
    CALL    r4, r3, r2          ; Call OutputString(ConOut, message)
    
    ; Return success
    MOV     r0, 0               ; EFI_SUCCESS
    RET
    
hello_msg:
    DB "Hello from COIL UEFI application", 0
```

### Direct Hardware Access

```
; Initialize UART hardware
init_uart:
    ; Set baud rate
    MMIO_WRITE [UART_BASE+0x00], 0x03  ; Divisor latch access bit
    MMIO_WRITE [UART_BASE+0x04], 0x01  ; Divisor high byte
    MMIO_WRITE [UART_BASE+0x00], 0x00  ; Divisor low byte
    
    ; Set line control
    MMIO_WRITE [UART_BASE+0x0C], 0x03  ; 8 bits, no parity, 1 stop bit
    
    ; Enable and clear FIFOs
    MMIO_WRITE [UART_BASE+0x08], 0x07  ; Enable FIFO, clear RX/TX FIFOs
    
    RET
```

These examples demonstrate how COIL instructions can be used in bare-metal environments without any runtime dependencies.