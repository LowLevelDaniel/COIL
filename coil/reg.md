# Virtual Register System (Version 0.1.0)

The COIL Virtual Register System provides a consistent, architecture-independent way to access and manipulate registers across different platforms. This system allows direct register access when needed, while enabling COIL code to remain portable.

## Design Principles

The virtual register system follows these core principles:

1. **Universality**: The same virtual register code should function across different architectures
2. **Transparent Mapping**: Each virtual register maps to a native register or stack location
3. **Fallback Mechanism**: When registers aren't available, stack locations are used automatically
4. **ABI Compatibility**: The system facilitates interfacing with platform-specific ABIs
5. **Last Resort**: Direct register access should only be used when higher-level abstractions are insufficient

## Register Naming Convention

COIL virtual registers follow a consistent naming convention:

```
R<width><number>
```

Where:
- **R** indicates a register
- **width** is one of:
  - **B**: Byte (8-bit)
  - **W**: Word (16-bit)
  - **L**: Long (32-bit)
  - **Q**: Quad (64-bit)
- **number** is a zero-based index

For example:
- RB0: 8-bit register #0
- RW3: 16-bit register #3
- RL7: 32-bit register #7
- RQ12: 64-bit register #12

## Register Categories

COIL defines several categories of virtual registers:

1. **General Purpose Registers**: Used for general computation
2. **Special Registers**: Used for specific purposes (stack pointer, flags, etc.)
3. **Segment Registers**: Used for memory segmentation on supported architectures

## General Purpose Registers

COIL provides 16 general-purpose registers in each width category:

```
// 8-bit registers
RB0, RB1, RB2, RB3, RB4, RB5, RB6, RB7, RB8, RB9, RB10, RB11, RB12, RB13, RB14, RB15

// 16-bit registers
RW0, RW1, RW2, RW3, RW4, RW5, RW6, RW7, RW8, RW9, RW10, RW11, RW12, RW13, RW14, RW15

// 32-bit registers
RL0, RL1, RL2, RL3, RL4, RL5, RL6, RL7, RL8, RL9, RL10, RL11, RL12, RL13, RL14, RL15

// 64-bit registers
RQ0, RQ1, RQ2, RQ3, RQ4, RQ5, RQ6, RQ7, RQ8, RQ9, RQ10, RQ11, RQ12, RQ13, RQ14, RQ15
```

Smaller registers may be part of larger registers, depending on the architecture. For example, on x86-64, RB0 is the lower byte of RW0, which is the lower word of RL0, which is the lower dword of RQ0.

## Special Registers

COIL defines the following special registers:

```
// Stack pointer registers
RSP - stack pointer

// Base pointer registers
RBP - base pointer

// Instruction pointer registers
RIP - instruction pointer

// Flags register
RF - Flags register
```

The appropriate width register will be selected based on the target architecture.

## Segment Registers

For architectures that support memory segmentation, COIL defines the following segment registers:

```
S0, S1, S2, S3, S4, S5
```

## Register Mapping for x86-64

The following table shows how COIL virtual registers map to native x86-64 registers:

### General Purpose Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| RQ0 | RAX | Accumulator, return value |
| RQ1 | RBX | Base register |
| RQ2 | RCX | Counter register |
| RQ3 | RDX | Data register |
| RQ4 | RDI | Destination index |
| RQ5 | RSI | Source index |
| RQ6 | RSP | Stack pointer |
| RQ7 | RBP | Base pointer |
| RQ8 | R8 | General purpose |
| RQ9 | R9 | General purpose |
| RQ10 | R10 | General purpose |
| RQ11 | R11 | General purpose |
| RQ12 | R12 | General purpose |
| RQ13 | R13 | General purpose |
| RQ14 | R14 | General purpose |
| RQ15 | R15 | General purpose |

### 32-bit Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| RL0 | EAX | Lower 32 bits of RAX |
| RL1 | EBX | Lower 32 bits of RBX |
| RL2 | ECX | Lower 32 bits of RCX |
| RL3 | EDX | Lower 32 bits of RDX |
| RL4 | EDI | Lower 32 bits of RDI |
| RL5 | ESI | Lower 32 bits of RSI |
| RL6 | ESP | Lower 32 bits of RSP |
| RL7 | EBP | Lower 32 bits of RBP |
| RL8 | R8D | Lower 32 bits of R8 |
| RL9 | R9D | Lower 32 bits of R9 |
| RL10 | R10D | Lower 32 bits of R10 |
| RL11 | R11D | Lower 32 bits of R11 |
| RL12 | R12D | Lower 32 bits of R12 |
| RL13 | R13D | Lower 32 bits of R13 |
| RL14 | R14D | Lower 32 bits of R14 |
| RL15 | R15D | Lower 32 bits of R15 |

### 16-bit Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| RW0 | AX | Lower 16 bits of RAX |
| RW1 | BX | Lower 16 bits of RBX |
| RW2 | CX | Lower 16 bits of RCX |
| RW3 | DX | Lower 16 bits of RDX |
| RW4 | DI | Lower 16 bits of RDI |
| RW5 | SI | Lower 16 bits of RSI |
| RW6 | SP | Lower 16 bits of RSP |
| RW7 | BP | Lower 16 bits of RBP |
| RW8 | R8W | Lower 16 bits of R8 |
| RW9 | R9W | Lower 16 bits of R9 |
| RW10 | R10W | Lower 16 bits of R10 |
| RW11 | R11W | Lower 16 bits of R11 |
| RW12 | R12W | Lower 16 bits of R12 |
| RW13 | R13W | Lower 16 bits of R13 |
| RW14 | R14W | Lower 16 bits of R14 |
| RW15 | R15W | Lower 16 bits of R15 |

### 8-bit Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| RB0 | AL | Lower 8 bits of RAX |
| RB1 | BL | Lower 8 bits of RBX |
| RB2 | CL | Lower 8 bits of RCX |
| RB3 | DL | Lower 8 bits of RDX |
| RB4 | DIL | Lower 8 bits of RDI |
| RB5 | SIL | Lower 8 bits of RSI |
| RB6 | SPL | Lower 8 bits of RSP |
| RB7 | BPL | Lower 8 bits of RBP |
| RB8 | R8B | Lower 8 bits of R8 |
| RB9 | R9B | Lower 8 bits of R9 |
| RB10 | R10B | Lower 8 bits of R10 |
| RB11 | R11B | Lower 8 bits of R11 |
| RB12 | R12B | Lower 8 bits of R12 |
| RB13 | R13B | Lower 8 bits of R13 |
| RB14 | R14B | Lower 8 bits of R14 |
| RB15 | R15B | Lower 8 bits of R15 |

### Segment Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| S0 | SS | Stack segment |
| S1 | CS | Code segment |
| S2 | DS | Data segment |
| S3 | ES | Extra segment |
| S4 | FS | General purpose |
| S5 | GS | General purpose |

### Special Registers

| COIL Register | x86-64 Register | Notes |
|---------------|-----------------|-------|
| RSP | RSP | Stack pointer |
| RBP | RBP | Base pointer |
| RIP | RIP | Instruction pointer |
| RF  | RFLAGS | Flags register |

## Register Fallback Mechanism

When targeting architectures with fewer physical registers than COIL virtual registers, the COIL assembler implements a fallback mechanism:

1. Physical registers are assigned to the most frequently used virtual registers
2. Remaining virtual registers are allocated on the stack
3. Load/store operations are automatically inserted when accessing stack-allocated registers

For example, on x86-32, which lacks the R8-R15 registers:
- Registers R0-R7 map to physical registers
- Registers R8-R15 are allocated on the stack

## ABI Integration

**None of this is settled on look into [ABI Specification](./abi.md) for more information**

To facilitate integration with platform-specific ABIs, COIL provides a mechanism to define register usage according to ABI conventions:

```
.abi_def "abi-name"
    .arg 0, RQ0    // First argument goes to register RQ0
    .arg 1, RQ1    // Second argument goes to register RQ1
    .result RQ0    // Return value placed in RQ0
.end_abi
```

This allows COIL code to use ABI-compliant register assignments without hard-coding architecture-specific details.

For common ABIs, COIL provides predefined configurations:

### Linux x86-64 System Call ABI

```
.abi_def "abi-linux-x64-syscall"
    .arg 0, RQ0    // Syscall number in RAX
    .arg 1, RQ4    // First argument in RDI
    .arg 2, RQ5    // Second argument in RSI
    .arg 3, RQ2    // Third argument in RDX
    .arg 4, RQ8    // Fourth argument in R10
    .arg 5, RQ9    // Fifth argument in R8
    .arg 6, RQ10   // Sixth argument in R9
    .result RQ0    // Return value in RAX
.end_abi
```

### Windows x64 Calling Convention

```
.abi_def "abi-windows-x64-call"
  .arg 0, RQ2    // First argument in RCX
  .arg 1, RQ3    // Second argument in RDX
  .arg 2, RQ8    // Third argument in R8
  .arg 3, RQ9    // Fourth argument in R9
  // Additional arguments on stack
  .result RQ0    // Return value in RAX
.end_abi
```

## Using Virtual Registers

COIL instructions can reference virtual registers directly:

```
// Example of using virtual registers
MOV RQ0, 42        // Load immediate value 42 into RQ0
ADD RQ1, RQ0, RQ2  // RQ1 = RQ0 + RQ2
ADD RQ0, RQ1 // RQ0 = RQ0 + RQ1
```

When interfacing with ABIs, the ABI definition can be specified:

```
// Call using Linux x86-64 syscall ABI
SYSC abi-linux-x64-syscall, (60, 0), ()
```

## Optimization Considerations

COIL assemblers should perform the following optimizations when using virtual registers:

1. Register allocation to minimize stack usage
2. Elimination of unnecessary register moves
3. Reordering operations to maximize register utilization
4. Coalescing related operations to minimize register pressure

## Implementation Requirements

COIL assemblers must:

1. Support all virtual registers defined in this specification
2. Implement the fallback mechanism for architectures with insufficient physical registers
3. Support predefined ABI definitions for supported platforms
4. Allow custom ABI definitions

## Future Extensions

Future versions of COIL will extend the virtual register system to include:

1. Floating-point registers
2. Vector/SIMD registers
3. Special-purpose registers for additional architectures
4. Support for other CPU architectures (ARM, RISC-V, etc.)
5. Support for non-CPU processing units (GPUs, FPGAs, etc.)

These extensions will be defined in later specification versions.