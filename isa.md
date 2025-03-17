# COIL Instruction Set

This document defines the comprehensive instruction set for COIL, based on the capabilities described in the CPU configuration format.

## 1. Instruction Format

Each COIL instruction follows a consistent format in both its binary and HOIL representations.

### Binary Format

In binary format, each instruction consists of:

- **Opcode** (1-2 bytes): Identifies the operation
- **Type Information** (0-1 byte): Encodes operand types when necessary
- **Operand Count** (0-1 byte): Number of operands (may be implicit for some instructions)
- **Operands** (variable length): Encoded operand values or references

### HOIL Format

In HOIL, instructions follow this general syntax:

```
[label:] operation[.type] [destination,] [source1[, source2[, ...]]]
```

Where:
- `label` is an optional jump target
- `operation` is the instruction mnemonic
- `.type` is an optional type suffix
- `destination` is the target operand (when applicable)
- `source1`, `source2`, etc. are the source operands

## 2. Instruction Categories

### 2.1 Arithmetic Operations

#### Integer Arithmetic

These instructions correspond to capabilities defined in `config_instr_int_1_t` and `config_instr_int_2_t`.

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `add` | `add.type dst, src1, src2` | Addition | `CONFIG_INSTR_INT_ADD` |
| `sub` | `sub.type dst, src1, src2` | Subtraction | `CONFIG_INSTR_INT_SUB` |
| `mul` | `mul.type dst, src1, src2` | Multiplication | `CONFIG_INSTR_INT_MUL` |
| `div` | `div.type dst, src1, src2` | Division | `CONFIG_INSTR_INT_DIV` |
| `rem` | `rem.type dst, src1, src2` | Remainder | `CONFIG_INSTR_INT_REM` |
| `neg` | `neg.type dst, src` | Negation | `CONFIG_INSTR_INT_NEG` |
| `abs` | `abs.type dst, src` | Absolute value | `CONFIG_INSTR_INT_ABS` |
| `min` | `min.type dst, src1, src2` | Minimum | `CONFIG_INSTR_INT_MIN` |
| `max` | `max.type dst, src1, src2` | Maximum | `CONFIG_INSTR_INT_MAX` |
| `muladd` | `muladd.type dst, a, b, c` | Multiply-add (a*b+c) | `CONFIG_INSTR_INT_MULADD` |
| `mulsub` | `mulsub.type dst, a, b, c` | Multiply-subtract (a*b-c) | `CONFIG_INSTR_INT_MULSUB` |
| `addc` | `addc.type dst, src1, src2` | Add with carry | `CONFIG_INSTR_INT_ADDC` |
| `subc` | `subc.type dst, src1, src2` | Subtract with carry/borrow | `CONFIG_INSTR_INT_SUBC` |
| `mulh` | `mulh.type dst, src1, src2` | Multiply high | `CONFIG_INSTR_INT_MULH` |

#### Bitwise Operations

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `and` | `and.type dst, src1, src2` | Bitwise AND | `CONFIG_INSTR_INT_AND` |
| `or` | `or.type dst, src1, src2` | Bitwise OR | `CONFIG_INSTR_INT_OR` |
| `xor` | `xor.type dst, src1, src2` | Bitwise XOR | `CONFIG_INSTR_INT_XOR` |
| `not` | `not.type dst, src` | Bitwise NOT | `CONFIG_INSTR_INT_NOT` |
| `andn` | `andn.type dst, src1, src2` | AND with complement | `CONFIG_INSTR_INT_ANDN` |
| `orn` | `orn.type dst, src1, src2` | OR with complement | `CONFIG_INSTR_INT_ORN` |
| `xnor` | `xnor.type dst, src1, src2` | Exclusive NOR | `CONFIG_INSTR_INT_XNOR` |

#### Shifts and Rotates

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `shl` | `shl.type dst, src, amount` | Shift left logical | `CONFIG_INSTR_INT_SHL` |
| `shr` | `shr.type dst, src, amount` | Shift right logical | `CONFIG_INSTR_INT_SHR` |
| `sar` | `sar.type dst, src, amount` | Shift right arithmetic | `CONFIG_INSTR_INT_SAR` |
| `rol` | `rol.type dst, src, amount` | Rotate left | `CONFIG_INSTR_INT_ROL` |
| `ror` | `ror.type dst, src, amount` | Rotate right | `CONFIG_INSTR_INT_ROR` |

#### Bit Manipulation

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `bset` | `bset.type dst, src, bit` | Set bit | `CONFIG_INSTR_INT_BIT_SET` |
| `bclr` | `bclr.type dst, src, bit` | Clear bit | `CONFIG_INSTR_INT_BIT_CLR` |
| `btog` | `btog.type dst, src, bit` | Toggle bit | `CONFIG_INSTR_INT_BIT_TOGGLE` |
| `btest` | `btest.type dst, src, bit` | Test bit | `CONFIG_INSTR_INT_BIT_TEST` |
| `bextr` | `bextr.type dst, src, pos, len` | Extract bits | `CONFIG_INSTR_INT_BIT_EXTRACT` |
| `bins` | `bins.type dst, src, val, pos, len` | Insert bits | `CONFIG_INSTR_INT_BIT_INSERT` |
| `popcnt` | `popcnt.type dst, src` | Population count | `CONFIG_INSTR_INT_POPCOUNT` |
| `clz` | `clz.type dst, src` | Count leading zeros | `CONFIG_INSTR_INT_CLZ` |
| `ctz` | `ctz.type dst, src` | Count trailing zeros | `CONFIG_INSTR_INT_CTZ` |
| `bswap` | `bswap.type dst, src` | Byte swap | `CONFIG_INSTR_INT_BYTE_SWAP` |

#### Floating-Point Arithmetic

These instructions correspond to capabilities defined in `config_instr_fp_1_t` and `config_instr_fp_2_t`.

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `fadd` | `fadd.type dst, src1, src2` | FP addition | `CONFIG_INSTR_FP_ADD` |
| `fsub` | `fsub.type dst, src1, src2` | FP subtraction | `CONFIG_INSTR_FP_SUB` |
| `fmul` | `fmul.type dst, src1, src2` | FP multiplication | `CONFIG_INSTR_FP_MUL` |
| `fdiv` | `fdiv.type dst, src1, src2` | FP division | `CONFIG_INSTR_FP_DIV` |
| `frem` | `frem.type dst, src1, src2` | FP remainder | `CONFIG_INSTR_FP_REM` |
| `fneg` | `fneg.type dst, src` | FP negation | `CONFIG_INSTR_FP_NEG` |
| `fabs` | `fabs.type dst, src` | FP absolute value | `CONFIG_INSTR_FP_ABS` |
| `fsqrt` | `fsqrt.type dst, src` | FP square root | `CONFIG_INSTR_FP_SQRT` |
| `frsqrt` | `frsqrt.type dst, src` | FP reciprocal square root | `CONFIG_INSTR_FP_RSQRT` |
| `frecip` | `frecip.type dst, src` | FP reciprocal | `CONFIG_INSTR_FP_RECIP` |
| `fma` | `fma.type dst, a, b, c` | FP fused multiply-add (a*b+c) | `CONFIG_INSTR_FP_FMA` |
| `fms` | `fms.type dst, a, b, c` | FP fused multiply-subtract (a*b-c) | `CONFIG_INSTR_FP_FMS` |
| `fmin` | `fmin.type dst, src1, src2` | FP minimum | `CONFIG_INSTR_FP_MIN` |
| `fmax` | `fmax.type dst, src1, src2` | FP maximum | `CONFIG_INSTR_FP_MAX` |

#### Floating-Point Special Functions

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `fsin` | `fsin.type dst, src` | FP sine | `CONFIG_INSTR_FP_SIN` |
| `fcos` | `fcos.type dst, src` | FP cosine | `CONFIG_INSTR_FP_COS` |
| `ftan` | `ftan.type dst, src` | FP tangent | `CONFIG_INSTR_FP_TAN` |
| `fexp` | `fexp.type dst, src` | FP exponential | `CONFIG_INSTR_FP_EXP` |
| `flog` | `flog.type dst, src` | FP logarithm | `CONFIG_INSTR_FP_LOG` |
| `flog2` | `flog2.type dst, src` | FP base-2 logarithm | `CONFIG_INSTR_FP_LOG2` |
| `flog10` | `flog10.type dst, src` | FP base-10 logarithm | `CONFIG_INSTR_FP_LOG10` |

### 2.2 Type Conversion Operations

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `itof` | `itof.dst_type.src_type dst, src` | Integer to FP conversion | `CONFIG_INSTR_FP_INT_TO_FP` |
| `ftoi` | `ftoi.dst_type.src_type dst, src` | FP to integer conversion | `CONFIG_INSTR_FP_FP_TO_INT` |
| `ftof` | `ftof.dst_type.src_type dst, src` | FP precision conversion | `CONFIG_INSTR_FP_FP_TO_FP` |
| `round` | `round.type dst, src` | FP rounding | `CONFIG_INSTR_FP_ROUND` |
| `ceil` | `ceil.type dst, src` | FP ceiling | `CONFIG_INSTR_FP_CEIL` |
| `floor` | `floor.type dst, src` | FP floor | `CONFIG_INSTR_FP_FLOOR` |
| `trunc` | `trunc.type dst, src` | FP truncation | `CONFIG_INSTR_FP_TRUNC` |

### 2.3 Vector Operations

These instructions correspond to capabilities defined in `config_instr_vector_1_t` and `config_instr_vector_2_t`.

#### Vector Arithmetic

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `vadd` | `vadd.type dst, src1, src2` | Vector add | `CONFIG_INSTR_VEC_ADD` |
| `vsub` | `vsub.type dst, src1, src2` | Vector subtract | `CONFIG_INSTR_VEC_SUB` |
| `vmul` | `vmul.type dst, src1, src2` | Vector multiply | `CONFIG_INSTR_VEC_MUL` |
| `vdiv` | `vdiv.type dst, src1, src2` | Vector divide | `CONFIG_INSTR_VEC_DIV` |
| `vabs` | `vabs.type dst, src` | Vector absolute value | `CONFIG_INSTR_VEC_ABS` |
| `vneg` | `vneg.type dst, src` | Vector negate | `CONFIG_INSTR_VEC_NEG` |
| `vfma` | `vfma.type dst, a, b, c` | Vector FMA | `CONFIG_INSTR_VEC_FMA` |
| `vdot` | `vdot.type dst, src1, src2` | Vector dot product | `CONFIG_INSTR_VEC_DOT` |

#### Vector Comparison and Selection

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `vcmp` | `vcmp.type dst, src1, src2, cond` | Vector compare | `CONFIG_INSTR_VEC_CMP` |
| `vmin` | `vmin.type dst, src1, src2` | Vector minimum | `CONFIG_INSTR_VEC_MIN` |
| `vmax` | `vmax.type dst, src1, src2` | Vector maximum | `CONFIG_INSTR_VEC_MAX` |
| `vblend` | `vblend.type dst, src1, src2, mask` | Vector blend | `CONFIG_INSTR_VEC_BLEND` |

#### Vector Manipulation

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `vshuffle` | `vshuffle.type dst, src, indices` | Vector shuffle | `CONFIG_INSTR_VEC_SHUFFLE` |
| `vpermute` | `vpermute.type dst, src, order` | Vector permute | `CONFIG_INSTR_VEC_PERMUTE` |
| `vcompress` | `vcompress.type dst, src, mask` | Vector compress | `CONFIG_INSTR_VEC_COMPRESS` |
| `vexpand` | `vexpand.type dst, src, mask` | Vector expand | `CONFIG_INSTR_VEC_EXPAND` |
| `vextract` | `vextract.elem_type dst, src, idx` | Vector extract element | `CONFIG_INSTR_VEC_EXTRACT` |
| `vinsert` | `vinsert.type dst, src, val, idx` | Vector insert element | `CONFIG_INSTR_VEC_INSERT` |

### 2.4 Memory Operations

#### Basic Memory Access

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `load` | `load.type dst, [addr]` | Load from memory | `CONFIG_INSTR_INT_LOAD`/`CONFIG_INSTR_FP_LOAD`/`CONFIG_INSTR_VEC_LOAD` |
| `store` | `store.type [addr], src` | Store to memory | `CONFIG_INSTR_INT_STORE`/`CONFIG_INSTR_FP_STORE`/`CONFIG_INSTR_VEC_STORE` |
| `move` | `move.type dst, src` | Move register to register | `CONFIG_INSTR_INT_MOVE`/`CONFIG_INSTR_FP_MOVE`/`CONFIG_INSTR_VEC_MOVE` |
| `loadimm` | `loadimm.type dst, imm` | Load immediate value | `CONFIG_INSTR_INT_LOAD_IMM`/`CONFIG_INSTR_FP_LOAD_IMM` |
| `xchg` | `xchg.type a, b` | Exchange values | `CONFIG_INSTR_INT_EXCHANGE`/`CONFIG_INSTR_FP_EXCHANGE` |

#### Advanced Memory Access

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `prefetch` | `prefetch [addr], hint` | Prefetch memory | `CONFIG_INSTR_INT_PREFETCH`/`CONFIG_INSTR_FP_PREFETCH`/`CONFIG_INSTR_VEC_PREFETCH` |
| `gather` | `gather.type dst, [base], indices, scale` | Vector gather | `CONFIG_INSTR_VEC_GATHER` |
| `scatter` | `scatter.type [base], src, indices, scale` | Vector scatter | `CONFIG_INSTR_VEC_SCATTER` |
| `push` | `push.type src` | Push to stack | `CONFIG_INSTR_INT_PUSH` |
| `pop` | `pop.type dst` | Pop from stack | `CONFIG_INSTR_INT_POP` |

#### Atomic Operations

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `atomic.add` | `atomic.add.type [addr], val` | Atomic add | `CONFIG_INSTR_INT_ATOMIC_ADD`/`CONFIG_INSTR_FP_ATOMIC_ADD`/`CONFIG_INSTR_VEC_ATOMIC_ADD` |
| `atomic.sub` | `atomic.sub.type [addr], val` | Atomic subtract | `CONFIG_INSTR_INT_ATOMIC_SUB`/`CONFIG_INSTR_FP_ATOMIC_SUB`/`CONFIG_INSTR_VEC_ATOMIC_SUB` |
| `atomic.and` | `atomic.and.type [addr], val` | Atomic AND | `CONFIG_INSTR_INT_ATOMIC_AND`/`CONFIG_INSTR_VEC_ATOMIC_AND` |
| `atomic.or` | `atomic.or.type [addr], val` | Atomic OR | `CONFIG_INSTR_INT_ATOMIC_OR`/`CONFIG_INSTR_VEC_ATOMIC_OR` |
| `atomic.xor` | `atomic.xor.type [addr], val` | Atomic XOR | `CONFIG_INSTR_INT_ATOMIC_XOR`/`CONFIG_INSTR_VEC_ATOMIC_XOR` |
| `atomic.xchg` | `atomic.xchg.type dst, [addr], val` | Atomic exchange | `CONFIG_INSTR_INT_ATOMIC_XCHG`/`CONFIG_INSTR_FP_ATOMIC_XCHG`/`CONFIG_INSTR_VEC_ATOMIC_XCHG` |
| `atomic.cmpxchg` | `atomic.cmpxchg.type dst, [addr], cmp, val` | Atomic compare-exchange | `CONFIG_INSTR_INT_ATOMIC_CMPXCHG`/`CONFIG_INSTR_FP_ATOMIC_CAS` |

### 2.5 Control Flow Operations

These instructions correspond to capabilities defined in `config_instr_control_flow_1_t` and `config_instr_control_flow_2_t`.

#### Basic Control Flow

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `jump` | `jump label` | Unconditional branch | `CONFIG_INSTR_CF_BRANCH` |
| `branch` | `branch cond, label` | Conditional branch | `CONFIG_INSTR_CF_BRANCH_COND` |
| `call` | `call procedure` | Call procedure | `CONFIG_INSTR_CF_CALL` |
| `ret` | `ret [value]` | Return from procedure | `CONFIG_INSTR_CF_RET` |
| `jumpr` | `jumpr reg` | Jump to register | `CONFIG_INSTR_CF_JUMP_REG` |
| `branchr` | `branchr cond, reg` | Conditional branch to register | `CONFIG_INSTR_CF_BRANCH_REG` |

#### Advanced Control Flow

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `cmov` | `cmov.type dst, src, cond` | Conditional move | `CONFIG_INSTR_CF_CONDITIONAL_MOVE` |
| `cset` | `cset.type dst, cond` | Conditional set | `CONFIG_INSTR_CF_CONDITIONAL_SET` |
| `csel` | `csel.type dst, true_val, false_val, cond` | Conditional select | `CONFIG_INSTR_CF_CONDITIONAL_SEL` |
| `loop` | `loop counter, label` | Loop instruction | `CONFIG_INSTR_CF_LOOP` |

#### Exception Handling

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `trap` | `trap code` | Trap instruction | `CONFIG_INSTR_CF_TRAP` |
| `syscall` | `syscall number` | System call | `CONFIG_INSTR_CF_SYSCALL` |
| `break` | `break` | Breakpoint | `CONFIG_INSTR_CF_BREAK` |
| `except` | `except code` | Generate exception | `CONFIG_INSTR_CF_EXCEPTION` |

### 2.6 Synchronization Operations

These instructions correspond to capabilities defined in `config_instr_mem_sync_t`.

| Instruction | Syntax | Description | Corresponding CPU Capability |
|-------------|--------|-------------|------------------------------|
| `fence` | `fence` | Memory fence/barrier | `CONFIG_INSTR_MEM_FENCE` |
| `afence` | `afence` | Acquire barrier | `CONFIG_INSTR_MEM_ACQUIRE_BARRIER` |
| `rfence` | `rfence` | Release barrier | `CONFIG_INSTR_MEM_RELEASE_BARRIER` |
| `dmb` | `dmb options` | Data memory barrier | `CONFIG_INSTR_MEM_DMB` |
| `dsb` | `dsb options` | Data synchronization barrier | `CONFIG_INSTR_MEM_DSB` |
| `isb` | `isb` | Instruction synchronization barrier | `CONFIG_INSTR_MEM_ISB` |
| `specbar` | `specbar` | Speculation barrier | `CONFIG_INSTR_MEM_SPECULATION_BAR` |

### 2.7 Comparison Operations

These set condition flags or predicate registers based on comparison results.

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `cmp` | `cmp.type a, b` | Compare values and set flags |
| `fcmp` | `fcmp.type a, b` | Compare floating-point values and set flags |
| `test` | `test.type a, b` | Test (bitwise AND) and set flags |
| `vcmp` | `vcmp.type a, b, pred` | Vector compare and set predicate |

## 3. Instruction Encoding

### 3.1 Opcode Format

COIL uses a variable-length opcode encoding to balance compactness with expressiveness.

- **Primary Opcode** (8 bits): Identifies the primary operation category
- **Extended Opcode** (0-8 bits): Provides additional information for the operation

### 3.2 Type Encoding

Types are encoded as follows:

- **Base Type** (3 bits): Integer, floating-point, vector, or special
- **Width** (4 bits): Bit width (8, 16, 32, 64, 128, etc.)
- **Sign** (1 bit): Signed or unsigned (for integer types)

### 3.3 Operand Encoding

Operands are encoded based on their kind:

- **Register Variables** (4-8 bits): Reference to a variable allocated to a register
- **Stack Variables** (8-16 bits): Reference to a variable allocated to the stack
- **Immediate Values** (variable length): Encoded based on the value size
- **Labels** (variable length): Encoded as offsets from the current instruction
- **Addresses** (variable length): Encoded based on the addressing mode

### 3.4 Full Instruction Encoding

A complete COIL instruction in binary format follows this structure:

```
+---------------+----------------+----------------+------------------+
| Primary       | Extended       | Type & Operand | Operand Data     |
| Opcode (8b)   | Opcode (0-8b)  | Info (8-16b)   | (Variable)       |
+---------------+----------------+----------------+------------------+
```

## 4. Instruction Expansion

When a COIL instruction doesn't have direct hardware support, the assembler expands it into a sequence of supported instructions. This process is transparent to the developer and ensures that all COIL features are available on any supported architecture.

### 4.1 Expansion Rules

The assembler follows these general rules for instruction expansion:

1. **Direct Mapping**: If the instruction has direct hardware support, it's mapped directly.
2. **Simple Expansion**: If the instruction can be implemented using a short sequence of supported instructions, it's expanded inline.
3. **Library Call**: For complex operations, the assembler may generate a call to a library function that implements the operation.

### 4.2 Expansion Examples

Here are some examples of instruction expansions:

#### Example 1: Population Count

```
# COIL instruction
popcnt.i32 r1, r2

# Expansion on architecture without native POPCNT
move.i32 r3, r2
loadimm.i32 r1, 0
loop:
  test.i32 r3, r3
  branch zero, end
  and.i32 r4, r3, 1
  add.i32 r1, r1, r4
  shr.i32 r3, r3, 1
  jump loop
end:
```

#### Example 2: Fused Multiply-Add

```
# COIL instruction
fma.f32 r1, r2, r3, r4

# Expansion on architecture without native FMA
fmul.f32 r5, r2, r3
fadd.f32 r1, r5, r4
```

## 5. Instruction Optimization

The COIL assembler performs various optimizations on instructions based on the target architecture's capabilities and the specific context.

### 5.1 Peephole Optimizations

The assembler identifies common instruction patterns and replaces them with more efficient alternatives.

### 5.2 Dead Code Elimination

Instructions that have no effect on the program's observable behavior are eliminated.

### 5.3 Strength Reduction

Complex operations are replaced with simpler, equivalent operations when possible.

### 5.4 Register Allocation

The assembler assigns variables to registers or stack locations based on usage patterns to minimize memory accesses.

### 5.5 Instruction Scheduling

Instructions are reordered to minimize stalls and maximize throughput based on the target architecture's pipeline characteristics.