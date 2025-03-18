# COIL Virtual Register System

## Introduction

The Virtual Register System is a fundamental abstraction in COIL that provides a unified approach to register usage across different architectures. It allows COIL code to reference registers in a consistent manner while enabling the COIL assembler to map these virtual registers to the actual physical registers of the target architecture.

## Design Philosophy

The virtual register system is designed with several key principles:

1. **Architecture Independence**: Virtual registers provide a consistent interface across different architectures.
2. **Efficiency**: The mapping between virtual and physical registers is straightforward and adds minimal overhead.
3. **Transparency**: The relationship between virtual registers and physical registers is explicit and predictable.
4. **Flexibility**: The system accommodates different register sets, sizes, and types across architectures.
5. **Simplicity**: The register mapping is predefined and consistent for all COIL code.

## Standard Virtual Register Naming

COIL defines standard virtual register names that are used consistently across all code:

### General Purpose Registers (R-registers)

The general purpose registers are named R0 through R15, mapping to architecture-specific registers:

```
; x86-64 mapping
R0 = rax    ; Return value, volatile
R1 = rbx    ; Callee-saved
R2 = rcx    ; 4th argument, volatile
R3 = rdx    ; 3rd argument, volatile
R4 = rdi    ; 1st argument, volatile
R5 = rsi    ; 2nd argument, volatile
R6 = r8     ; 5th argument, volatile
R7 = r9     ; 6th argument, volatile
R8 = r10    ; Volatile
R9 = r11    ; Volatile
R10 = r12   ; Callee-saved
R11 = r13   ; Callee-saved
R12 = r14   ; Callee-saved
R13 = r15   ; Callee-saved
R14 = rsp   ; Stack pointer (special)
R15 = rbp   ; Frame pointer (special)

; ARM64 mapping
R0 = x0     ; 1st argument, return value, volatile
R1 = x1     ; 2nd argument, volatile
R2 = x2     ; 3rd argument, volatile
R3 = x3     ; 4th argument, volatile
R4 = x4     ; 5th argument, volatile
R5 = x5     ; 6th argument, volatile
R6 = x6     ; 7th argument, volatile
R7 = x7     ; 8th argument, volatile
R8 = x8     ; Indirect result location, volatile
R9 = x9     ; Volatile
R10 = x10   ; Volatile
R11 = x19   ; Callee-saved
R12 = x20   ; Callee-saved
R13 = x21   ; Callee-saved
R14 = sp    ; Stack pointer (special)
R15 = x29   ; Frame pointer (special)
```

### Floating Point Registers (F-registers)

Floating point registers are named F0 through F15:

```
; x86-64 mapping
F0 = xmm0   ; 1st float argument, return value, volatile
F1 = xmm1   ; 2nd float argument, volatile
F2 = xmm2   ; 3rd float argument, volatile
F3 = xmm3   ; 4th float argument, volatile
F4 = xmm4   ; 5th float argument, volatile
F5 = xmm5   ; 6th float argument, volatile
F6 = xmm6   ; 7th float argument, volatile
F7 = xmm7   ; 8th float argument, volatile
F8 = xmm8   ; Volatile
F9 = xmm9   ; Volatile
F10 = xmm10 ; Volatile
F11 = xmm11 ; Volatile
F12 = xmm12 ; Volatile
F13 = xmm13 ; Volatile
F14 = xmm14 ; Volatile
F15 = xmm15 ; Volatile

; ARM64 mapping
F0 = v0/d0  ; 1st float argument, return value, volatile
F1 = v1/d1  ; 2nd float argument, volatile
F2 = v2/d2  ; 3rd float argument, volatile
F3 = v3/d3  ; 4th float argument, volatile
F4 = v4/d4  ; 5th float argument, volatile
F5 = v5/d5  ; 6th float argument, volatile
F6 = v6/d6  ; 7th float argument, volatile
F7 = v7/d7  ; 8th float argument, volatile
F8 = v8/d8  ; Callee-saved
F9 = v9/d9  ; Callee-saved
F10 = v10/d10 ; Callee-saved
F11 = v11/d11 ; Callee-saved
F12 = v12/d12 ; Callee-saved
F13 = v13/d13 ; Callee-saved
F14 = v14/d14 ; Callee-saved
F15 = v15/d15 ; Callee-saved
```

### Vector Registers (V-registers)

Vector registers are named V0 through V15 and typically map to the same physical registers as floating point registers but are used for SIMD operations:

```
; x86-64 mapping
V0 = ymm0/zmm0   ; AVX/AVX-512 register
V1 = ymm1/zmm1   ; AVX/AVX-512 register
...
V15 = ymm15/zmm15 ; AVX/AVX-512 register

; ARM64 mapping
V0 = v0.16b/q0   ; NEON quad register
V1 = v1.16b/q1   ; NEON quad register
...
V15 = v15.16b/q15 ; NEON quad register
```

### Special Registers

Special registers have architecture-specific names:

```
; x86-64 special registers
PC = rip    ; Program Counter
SP = rsp    ; Stack Pointer
FP = rbp    ; Frame Pointer
FLAGS = rflags ; Status Flags
CS = cs     ; Code Segment
DS = ds     ; Data Segment
SS = ss     ; Stack Segment

; ARM64 special registers
PC = pc     ; Program Counter
SP = sp     ; Stack Pointer
FP = x29    ; Frame Pointer
LR = x30    ; Link Register
NZCV = nzcv ; Status Flags
```

## Register Usage in COIL

COIL code explicitly references registers using their virtual names. This approach provides architecture independence while maintaining the performance characteristics of register-based computation.

### Direct Register References

```
; Example of direct register usage
MEM MOV R0, 42       ; Move immediate value 42 to R0
MATH ADD R0, R1      ; Add R1 to R0
```

### Register Usage in Function Calls

```
; Function parameters in registers based on ABI
CF CALL my_function  ; Arguments passed in R0-R7 (architecture-dependent)
MEM MOV R0, result   ; Return value in R0
```

### Special Register Access

```
; Stack operations
MEM PUSH R0          ; Push R0 to stack
MEM POP R1           ; Pop from stack to R1

; Status flags
BIT CMP R0, R1       ; Compare and set status flags
CF BRC EQ label      ; Branch if equal (using status flags)
```

## Binary Encoding

In the COIL binary format, virtual registers are encoded as simple 8-bit unsigned integer values:

```c
/**
 * @brief Predefined virtual register IDs
 */
enum coil_vreg_id {
  /* General purpose registers */
  COIL_REG_R0  = 0x00,
  COIL_REG_R1  = 0x01,
  COIL_REG_R2  = 0x02,
  COIL_REG_R3  = 0x03,
  COIL_REG_R4  = 0x04,
  COIL_REG_R5  = 0x05,
  COIL_REG_R6  = 0x06,
  COIL_REG_R7  = 0x07,
  COIL_REG_R8  = 0x08,
  COIL_REG_R9  = 0x09,
  COIL_REG_R10 = 0x0A,
  COIL_REG_R11 = 0x0B,
  COIL_REG_R12 = 0x0C,
  COIL_REG_R13 = 0x0D,
  COIL_REG_R14 = 0x0E,
  COIL_REG_R15 = 0x0F,
  
  /* Floating point registers */
  COIL_REG_F0  = 0x10,
  COIL_REG_F1  = 0x11,
  COIL_REG_F2  = 0x12,
  COIL_REG_F3  = 0x13,
  COIL_REG_F4  = 0x14,
  COIL_REG_F5  = 0x15,
  COIL_REG_F6  = 0x16,
  COIL_REG_F7  = 0x17,
  COIL_REG_F8  = 0x18,
  COIL_REG_F9  = 0x19,
  COIL_REG_F10 = 0x1A,
  COIL_REG_F11 = 0x1B,
  COIL_REG_F12 = 0x1C,
  COIL_REG_F13 = 0x1D,
  COIL_REG_F14 = 0x1E,
  COIL_REG_F15 = 0x1F,
  
  /* Vector registers */
  COIL_REG_V0  = 0x20,
  COIL_REG_V1  = 0x21,
  COIL_REG_V2  = 0x22,
  COIL_REG_V3  = 0x23,
  COIL_REG_V4  = 0x24,
  COIL_REG_V5  = 0x25,
  COIL_REG_V6  = 0x26,
  COIL_REG_V7  = 0x27,
  COIL_REG_V8  = 0x28,
  COIL_REG_V9  = 0x29,
  COIL_REG_V10 = 0x2A,
  COIL_REG_V11 = 0x2B,
  COIL_REG_V12 = 0x2C,
  COIL_REG_V13 = 0x2D,
  COIL_REG_V14 = 0x2E,
  COIL_REG_V15 = 0x2F,
  
  /* Special registers */
  COIL_REG_PC    = 0x30,  /* Program Counter */
  COIL_REG_SP    = 0x31,  /* Stack Pointer */
  COIL_REG_FP    = 0x32,  /* Frame Pointer */
  COIL_REG_FLAGS = 0x33,  /* Status Flags */
  COIL_REG_LR    = 0x34,  /* Link Register */
  
  /* x86-specific segment registers */
  COIL_REG_CS    = 0x40,  /* Code Segment */
  COIL_REG_DS    = 0x41,  /* Data Segment */
  COIL_REG_ES    = 0x42,  /* Extra Segment */
  COIL_REG_FS    = 0x43,  /* F Segment */
  COIL_REG_GS    = 0x44,  /* G Segment */
  COIL_REG_SS    = 0x45,  /* Stack Segment */
};
```

### Register Operand Encoding

```c
/**
 * @brief Register operand encoding in instruction format
 */
typedef struct coil_reg_operand {
  uint8_t reg_id;     /**< Virtual register ID */
  uint8_t flags;      /**< Modifier flags */
} coil_reg_operand_t;
```

### Register Modifier Flags

```c
/**
 * @brief Register operand modifier flags
 */
enum coil_reg_flags {
  COIL_REG_NONE      = 0x00,  /**< No modifiers */
  COIL_REG_PARTIAL_L = 0x01,  /**< Access lower portion (e.g., al in rax) */
  COIL_REG_PARTIAL_H = 0x02,  /**< Access high byte portion (e.g., ah in rax) */
  COIL_REG_PARTIAL_W = 0x03,  /**< Access word portion (e.g., ax in rax) */
  COIL_REG_PARTIAL_D = 0x04,  /**< Access double word portion (e.g., eax in rax) */
  COIL_REG_INDIRECT  = 0x10,  /**< Use register as memory address ([r]) */
  COIL_REG_PRE_INC   = 0x20,  /**< Pre-increment indirection ([++r]) */
  COIL_REG_PRE_DEC   = 0x30,  /**< Pre-decrement indirection ([--r]) */
  COIL_REG_POST_INC  = 0x40,  /**< Post-increment indirection ([r++]) */
  COIL_REG_POST_DEC  = 0x50,  /**< Post-decrement indirection ([r--]) */
  COIL_REG_INDEXED   = 0x60,  /**< Indexed indirection ([r+offset]) */
  COIL_REG_SCALED    = 0x70,  /**< Scaled indirection ([r+index*scale]) */
};
```

## Register Preservation

COIL defines register preservation rules according to calling conventions, which are specified in ABI definitions:

```
DIR ABI abi-linux-x86_64
{
  args = [ R0, R4, R5, R3, R6, R7 ]  ; System call arguments
  rets = [ R0 ]                      ; Return value
  preserved = [ R1, R10, R11, R12, R13 ] ; Callee-saved registers
  volatile = [ R0, R2, R3, R4, R5, R6, R7, R8, R9 ] ; Caller-saved registers
  stack_align = 16
}
```

## Register State Management

COIL provides explicit instructions for managing register state during function calls and context switches:

```
; Save registers at function entry
MEM PUSH STATE $0       ; Save register state to variable $0

; Restore registers before function exit
MEM POP $0              ; Restore register state from variable $0
```

## Target-Specific Register Access

For architecture-specific registers that don't map cleanly to the virtual register set, COIL provides target-specific directives:

```
; Access x86-specific registers
DIR TARGET x86-64
MEM MOV CR0, R0         ; Move R0 to Control Register 0

; Access ARM-specific registers
DIR TARGET arm64
MEM MOV TPIDR_EL0, R0   ; Move R0 to Thread ID Register
```

## Conclusion

The COIL Virtual Register System provides a consistent, architecture-independent approach to register usage while maintaining direct control over hardware resources. By using standard register names that map to architecture-specific physical registers, COIL allows for portable code that can target diverse hardware platforms without sacrificing performance.

The fixed virtual-to-physical register mapping simplifies code generation and analysis while still enabling efficient use of the underlying hardware. This balance of abstraction and control makes COIL an effective intermediate representation for low-level systems programming.