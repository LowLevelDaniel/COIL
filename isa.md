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

## Format

[opcode: uint8_t][operand_count: uint8_t][[operand]...]
[operand] = [type: uint8_t][qual: uint8_t][data: void[]]

#### Opcode
```c
enum {
  // CF
  COIL_OP_CF_BR,
  COIL_OP_CF_BRC,
  COIL_OP_CF_CALL,
  COIL_OP_CF_RET,
  COIL_OP_CF_INT,
  COIL_OP_CF_IRET,
  COIL_OP_CF_HLT,
  COIL_OP_CF_SYSC,
  COIL_OP_CF_TRAP,
  COIL_OP_CF_WFE,
  COIL_OP_CF_SEV,
  // ...
  
  // MEM
  COIL_OP_MEM_MOV,
  COIL_OP_MEM_PUSH,
  COIL_OP_MEM_POP,
  COIL_OP_MEM_LOAD,
  COIL_OP_MEM_STORE,
  COIL_OP_MEM_PREFETCH,
  // ...

  // MATH
  COIL_OP_MATH_ADD,
  COIL_OP_MATH_SUB,
  COIL_OP_MATH_MUL,
  COIL_OP_MATH_DIV,
  COIL_OP_MATH_MOD,
  COIL_OP_MATH_NEG,
  // ...

  // BIT
  COIL_OP_BIT_AND,
  COIL_OP_BIT_OR,
  COIL_OP_BIT_XOR,
  COIL_OP_BIT_NOT,
  COIL_OP_BIT_ANDN,
  COIL_OP_BIT_ORN,
  COIL_OP_BIT_XNOR,
  COIL_OP_BIT_SHL,
  COIL_OP_BIT_SHR,
  // ...

  // VECTOR
  COIL_OP_VEC_INSERT,
  COIL_OP_VEC_EXTRACT,
  COIL_OP_VEC_SHUFFLE,
  COIL_OP_VEC_DOT,
  COIL_OP_VEC_GATHER,
  COIL_OP_VEC_SCATTER,
  // ...

  // ATOMIC
  COIL_OP_ATM_ADD,
  COIL_OP_ATM_SUB,
  COIL_OP_ATM_AND,
  COIL_OP_ATM_OR,
  COIL_OP_ATM_XOR,
  COIL_OP_ATM_NAND,
  COIL_OP_ATM_XCHG,
  COIL_OP_ATM_CAS,
  COIL_OP_ATM_FETCH_ADD,
  COIL_OP_ATM_FETCH_SUB,
  COIL_OP_ATM_FETCH_AND,
  COIL_OP_ATM_FETCH_OR,
  COIL_OP_ATM_FETCH_XOR,
  COIL_OP_ATM_FETCH_NAND,
  // ...

  // VARIABLE
  COIL_OP_VAR_DECL,
  COIL_OP_VAR_PMT,
  COIL_OP_VAR_DMT,
  COIL_OP_VAR_DLT,
  // ...

  // FRAME
  COIL_OP_FRM_ENTER,
  COIL_OP_FRM_LEAVE,
  // ...

  // DIRECTIVES
  COIL_OP_DIR_SECT,
  COIL_OP_DIR_INST,
  COIL_OP_DIR_PADD,
  // ...
}
```

#### Type
```c
enum {
  COIL_TYPE_VOID = 0,

  COIL_TYPE_INT8,
  COIL_TYPE_INT16,
  COIL_TYPE_INT32,
  COIL_TYPE_INT64,
  // ...

  COIL_TYPE_FP8_e5m2,
  COIL_TYPE_FP8_e4m3,
  COIL_TYPE_FP16,
  COIL_TYPE_FP16b,
  COIL_TYPE_FP32,
  COIL_TYPE_FP32t,
  COIL_TYPE_FP64,
  COIL_TYPE_FP80,
  COIL_TYPE_FP128,
  // ...

  COIL_TYPE_VEC128,
  COIL_TYPE_VEC256,
  COIL_TYPE_VEC512,

  // aliases
  COIL_TYPE_PTR,   // natural pointer (not to be confused with ptr qualififer)
  COIL_TYPE_INT,   // most natural integer (normally largest)
  COIL_TYPE_FLOAT, // most natural float   (normally largest)
  COIL_TYPE_VEC,   // most natural vector  (normally largest)
}
```

#### Type Qualifiers
```c
enum {
  COIL_QUAL_IMM  = (1 << 0),
  COIL_QUAL_VAR  = (1 << 1),
  COIL_QUAL_SYMB = (1 << 2),
  COIL_QUAL_VREG = (1 << 3),
  COIL_QUAL_VOL  = (1 << 4),
  COIL_QUAL_SIGN = (1 << 5),
  COIL_QUAL_PTR  = (1 << 6)
}
```

## Conclusion

The COIL Instruction Set Architecture provides a powerful, consistent approach to low-level programming across diverse hardware architectures. By integrating with COIL's type system, the instruction set eliminates the need for type-specific instruction variants while still providing full access to hardware capabilities.

This approach simplifies code generation, improves readability, and enables sophisticated cross-architecture optimization, making COIL an ideal intermediate language for system programming, embedded development, and high-performance computing.