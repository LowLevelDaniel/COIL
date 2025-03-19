# Instruction Set Architecture (Version 1.0.0)

The COIL Instruction Set Architecture (ISA) defines the complete set of operations available in the COIL binary format. This specification documents the binary encoding, behavior, and implementation requirements for all instructions in version 1.0.0.

## Binary Format Overview

In the COIL binary format, instructions and directives share a common encoding structure but are distinguished by their opcode ranges:

```c
// Opcode ranges
#define OPCODE_RANGE_INSTRUCTION  0x00-0xCF  // Instructions
#define OPCODE_RANGE_DIRECTIVE    0xD0-0xDF  // Directives
#define OPCODE_RANGE_EXTENSION    0xE0-0xFF  // Implementation-specific extensions
```

## Instruction Format

All COIL instructions follow a consistent binary format:

```
[opcode: uint8_t][qualifier: uint8_t][operand_count: uint8_t][operands...]
```

Where:
- **opcode**: An 8-bit value identifying the operation
- **qualifier**: An 8-bit value providing additional information about the operation
- **operand_count**: The number of operands that follow
- **operands**: A variable number of operands whose format depends on the operation

## Operand Format

Operands use a consistent format to represent different types of values:

```
[qualifier: uint8_t][type: uint8_t][data: variable]
```

Where:
- **qualifier**: Specifies how to interpret the operand (immediate, variable, etc.)
- **type**: Specifies the data type of the operand (see type.md)
- **data**: The actual data, with size determined by type and qualifier

## Operand Qualifiers

```c
enum operand_qualifier : uint8_t {
    OPQUAL_IMM = 0x01, // Value is an immediate constant in the instruction
    OPQUAL_VAR = 0x02, // Value refers to a variable
    OPQUAL_REG = 0x03, // Value refers to a virtual register
    OPQUAL_MEM = 0x04, // Value refers to a memory address
    OPQUAL_LBL = 0x05, // Value refers to a label
    OPQUAL_STR = 0x06, // Value is a string literal (offset into string table)
    OPQUAL_SYM = 0x07, // Value refers to a symbol (index into symbol table)
    OPQUAL_REL = 0x08, // Value is a relative offset
    // 0x09-0xFF reserved for future qualifiers
};
```

## Instruction Opcodes

COIL instructions are organized into logical categories:

```c
enum instruction_opcode : uint8_t {
    // No Operation (0x00)
    OP_NOP = 0x00, // No operation
    
    // Control Flow (0x01 - 0x0F)
    OP_SYMB = 0x01, // Define a symbol
    OP_BR   = 0x02, // Unconditional branch to location
    OP_BRC  = 0x03, // Conditional branch to location
    OP_CALL = 0x04, // Call subroutine
    OP_RET  = 0x05, // Return from subroutine
    OP_INT  = 0x06, // Trigger interrupt
    OP_IRET = 0x07, // Return from interrupt
    OP_WFI  = 0x08, // Wait for interrupt
    OP_SYSC = 0x09, // System call
    OP_WFE  = 0x0A, // Wait for event
    OP_SEV  = 0x0B, // Send event
    OP_TRAP = 0x0C, // Software trap
    OP_HLT  = 0x0D, // Halt execution
    // 0x0E-0x0F reserved for future control flow operations
    
    // Arithmetic Operations (0x10 - 0x1F)
    OP_ADD  = 0x10, // Addition
    OP_SUB  = 0x11, // Subtraction
    OP_MUL  = 0x12, // Multiplication
    OP_DIV  = 0x13, // Division
    OP_MOD  = 0x14, // Modulus
    OP_NEG  = 0x15, // Negation
    OP_ABS  = 0x16, // Absolute value
    OP_INC  = 0x17, // Increment
    OP_DEC  = 0x18, // Decrement
    OP_ADDC = 0x19, // Add with carry
    OP_SUBB = 0x1A, // Subtract with borrow
    OP_MULH = 0x1B, // Multiplication high bits
    OP_ADDP = 0x1C, // Add packed (for SIMD)
    OP_SUBP = 0x1D, // Subtract packed (for SIMD)
    OP_MULP = 0x1E, // Multiply packed (for SIMD)
    OP_DIVP = 0x1F, // Divide packed (for SIMD)
    
    // Bit Manipulation (0x20 - 0x2F)
    OP_AND  = 0x20, // Bitwise AND
    OP_OR   = 0x21, // Bitwise OR
    OP_XOR  = 0x22, // Bitwise XOR
    OP_NOT  = 0x23, // Bitwise NOT
    OP_SHL  = 0x24, // Shift left
    OP_SHR  = 0x25, // Shift right logical
    OP_SAR  = 0x26, // Shift right arithmetic
    OP_ROL  = 0x27, // Rotate left
    OP_ROR  = 0x28, // Rotate right
    OP_CLZ  = 0x29, // Count leading zeros
    OP_CTZ  = 0x2A, // Count trailing zeros
    OP_POPC = 0x2B, // Population count
    OP_BSWP = 0x2C, // Byte swap
    OP_BEXT = 0x2D, // Bit extraction
    OP_BINS = 0x2E, // Bit insertion
    OP_BMSK = 0x2F, // Bit mask
    
    // Comparison Operations (0x30 - 0x3F)
    OP_CMP  = 0x30, // Compare
    OP_TEST = 0x31, // Test bits
    OP_CMPEQ = 0x32, // Compare equal
    OP_CMPNE = 0x33, // Compare not equal
    OP_CMPLT = 0x34, // Compare less than
    OP_CMPLE = 0x35, // Compare less than or equal
    OP_CMPGT = 0x36, // Compare greater than
    OP_CMPGE = 0x37, // Compare greater than or equal
    OP_CMPZ  = 0x38, // Compare to zero
    OP_CMPNZ = 0x39, // Compare not zero
    // 0x3A-0x3F reserved for future comparison operations
    
    // Data Movement (0x40 - 0x4F)
    OP_MOV   = 0x40, // Move data
    OP_LOAD  = 0x41, // Load from memory
    OP_STORE = 0x42, // Store to memory
    OP_XCHG  = 0x43, // Exchange data
    OP_LEA   = 0x44, // Load effective address
    OP_MOVI  = 0x45, // Move immediate value
    OP_MOVZ  = 0x46, // Move with zero extension
    OP_MOVS  = 0x47, // Move with sign extension
    OP_LDMUL = 0x48, // Load multiple
    OP_STMUL = 0x49, // Store multiple
    OP_LDSTR = 0x4A, // Load string
    OP_STSTR = 0x4B, // Store string
    // 0x4C-0x4F reserved for future data movement operations
    
    // Stack Operations (0x50 - 0x5F)
    OP_PUSH  = 0x50, // Push onto stack
    OP_POP   = 0x51, // Pop from stack
    OP_PUSHA = 0x52, // Push all registers
    OP_POPA  = 0x53, // Pop all registers
    OP_PUSHF = 0x54, // Push flags
    OP_POPF  = 0x55, // Pop flags
    OP_ADJSP = 0x56, // Adjust stack pointer
    // 0x57-0x5F reserved for future stack operations
    
    // Variable Operations (0x60 - 0x6F)
    OP_VARCR = 0x60, // Create variable
    OP_VARDL = 0x61, // Delete variable
    OP_VARSC = 0x62, // Create variable scope
    OP_VAREND = 0x63, // End variable scope
    OP_VARGET = 0x64, // Get variable value
    OP_VARSET = 0x65, // Set variable value
    OP_VARREF = 0x66, // Get variable reference
    // 0x67-0x6F reserved for future variable operations
    
    // Conversion Operations (0x70 - 0x7F)
    OP_TRUNC = 0x70, // Truncate value
    OP_ZEXT  = 0x71, // Zero extend value
    OP_SEXT  = 0x72, // Sign extend value
    OP_FTOI  = 0x73, // Float to integer
    OP_ITOF  = 0x74, // Integer to float
    OP_FTOB  = 0x75, // Float to bits
    OP_BTOF  = 0x76, // Bits to float
    OP_F32F64 = 0x77, // Float32 to Float64
    OP_F64F32 = 0x78, // Float64 to Float32
    // 0x79-0x7F reserved for future conversion operations
    
    // Atomic Operations (0x80 - 0x8F)
    OP_ATOMLD = 0x80, // Atomic load
    OP_ATOMST = 0x81, // Atomic store
    OP_ATOMXC = 0x82, // Atomic exchange
    OP_ATOMCMP = 0x83, // Atomic compare-exchange
    OP_ATOMADD = 0x84, // Atomic add
    OP_ATOMSUB = 0x85, // Atomic subtract
    OP_ATOMAND = 0x86, // Atomic AND
    OP_ATOMOR  = 0x87, // Atomic OR
    OP_ATOMXOR = 0x88, // Atomic XOR
    OP_ATOMMAX = 0x89, // Atomic maximum
    OP_ATOMMIN = 0x8A, // Atomic minimum
    OP_FENCE   = 0x8B, // Memory fence
    OP_BARRIER = 0x8C, // Memory barrier
    // 0x8D-0x8F reserved for future atomic operations
    
    // Floating Point Operations (0x90 - 0x9F)
    OP_FADD  = 0x90, // Floating-point addition
    OP_FSUB  = 0x91, // Floating-point subtraction
    OP_FMUL  = 0x92, // Floating-point multiplication
    OP_FDIV  = 0x93, // Floating-point division
    OP_FREM  = 0x94, // Floating-point remainder
    OP_FSQRT = 0x95, // Floating-point square root
    OP_FCEIL = 0x96, // Floating-point ceiling
    OP_FFLOOR = 0x97, // Floating-point floor
    OP_FROUND = 0x98, // Floating-point round
    OP_FTRUNC = 0x99, // Floating-point truncate
    OP_FABS   = 0x9A, // Floating-point absolute value
    OP_FNEG   = 0x9B, // Floating-point negation
    OP_FCMP   = 0x9C, // Floating-point compare
    OP_FMA    = 0x9D, // Fused multiply-add
    // 0x9E-0x9F reserved for future floating-point operations
    
    // Conditional Operations (0xA0 - 0xAF)
    OP_SELECT = 0xA0, // Conditional select
    OP_CMOV   = 0xA1, // Conditional move
    // 0xA2-0xAF reserved for future conditional operations
    
    // Vector/SIMD (reserved for future use)
    // 0xB0-0xBF reserved for vector/SIMD operations
    
    // Function Support (0xC0 - 0xCF)
    OP_ENTER  = 0xC0, // Function prologue
    OP_LEAVE  = 0xC1, // Function epilogue
    OP_PARAM  = 0xC2, // Function parameter
    OP_RESULT = 0xC3, // Function result
    OP_ALLOCA = 0xC4, // Allocate stack memory
    // 0xC5-0xCF reserved for future function operations
    
    // Directive range: 0xD0-0xDF (defined in dir.md)
    // Extension range: 0xE0-0xFF (implementation-specific)
};
```

## Instruction Qualifiers

Each instruction category uses its own qualifier bit definitions:

### Arithmetic Operation Qualifiers
```c
enum arith_qualifier : uint8_t {
    ARITH_DEFAULT = 0x00,     // Default behavior
    ARITH_SIGNED  = (1 << 0), // Signed operation
    ARITH_SAT     = (1 << 1), // Saturating operation
    ARITH_FLOAT   = (1 << 2), // Force floating point semantics
    ARITH_INT     = (1 << 3), // Force integer semantics
    ARITH_TRAP    = (1 << 4), // Trap on overflow
    ARITH_APPROX  = (1 << 5), // Allow approximation
    ARITH_UNCHECKED = (1 << 6), // Assume no overflow
    // Bit 7 reserved
};
```

### Branch Condition Qualifiers
```c
enum branch_qualifier : uint8_t {
    BR_ALWAYS = 0x00, // Always branch (unconditional)
    BR_EQ     = 0x01, // Equal / Zero
    BR_NE     = 0x02, // Not equal / Not zero
    BR_LT     = 0x03, // Less than
    BR_LE     = 0x04, // Less than or equal
    BR_GT     = 0x05, // Greater than
    BR_GE     = 0x06, // Greater than or equal
    BR_CARRY  = 0x07, // Carry flag set
    BR_OFLOW  = 0x08, // Overflow flag set
    BR_SIGN   = 0x09, // Sign flag set
    BR_PARITY = 0x0A, // Parity flag set
    BR_NCARRY = 0x0B, // Carry flag not set
    BR_NOFLOW = 0x0C, // Overflow flag not set
    BR_NSIGN  = 0x0D, // Sign flag not set
    BR_NPARITY = 0x0E, // Parity flag not set
    // 0x0F-0xFF reserved for future conditions
};
```

### Memory Operation Qualifiers
```c
enum memory_qualifier : uint8_t {
    MEM_DEFAULT   = 0x00,     // Default memory access
    MEM_VOLATILE  = (1 << 0), // Volatile access
    MEM_ATOMIC    = (1 << 1), // Atomic access
    MEM_ALIGNED   = (1 << 2), // Aligned access
    MEM_UNALIGNED = (1 << 3), // Unaligned access allowed
    MEM_NONTEMPORAL = (1 << 4), // Non-temporal access hint
    MEM_READONLY  = (1 << 5), // Read-only memory hint
    MEM_WRITEONLY = (1 << 6), // Write-only memory hint
    // Bit 7 reserved
};
```

### System Call Qualifiers
```c
enum syscall_qualifier : uint8_t {
    SYSC_DEFAULT = 0x00,      // Default system call
    SYSC_PRIV    = (1 << 0),  // Privileged system call
    SYSC_TRAP    = (1 << 1),  // Use trap instruction
    SYSC_INT     = (1 << 2),  // Use interrupt instruction
    SYSC_FAST    = (1 << 3),  // Use fast system call if available
    SYSC_VIRT    = (1 << 4),  // Virtualization-aware system call
    // Bits 5-7 reserved
};
```

## Detailed Instruction Specifications

Each instruction is defined below with its binary encoding, operands, behavior, and implementation requirements.

### Control Flow Instructions

#### NOP - No Operation
```
Opcode: 0x00
Qualifier: 0x00
Operands: None
Binary Format: [0x00][0x00][0x00]
Description: Performs no operation. May be used for alignment or timing purposes.
Implementation: May be optimized out if it has no observable effects.
```

#### BR - Unconditional Branch
```
Opcode: 0x02
Qualifier: 0x00
Operands: [target: operand_t]
Binary Format: [0x02][0x00][0x01][target]
Description: Transfers control to the specified target unconditionally.
Implementation:
  - Target may be a code label, address, or register
  - Must ensure all architectural side effects are handled correctly
```

#### BRC - Conditional Branch
```
Opcode: 0x03
Qualifier: [condition: branch_qualifier]
Operands: [target: operand_t]
Binary Format: [0x03][condition][0x01][target]
Description: Transfers control to the specified target if the condition is met.
Implementation:
  - Conditions are evaluated based on the current state of flags
  - Should use the most efficient conditional branch instruction available on the target architecture
```

#### CALL - Call Subroutine
```
Opcode: 0x04
Qualifier: [abi_qualifier: uint8_t]
Operands: [target: operand_t]
Binary Format: [0x04][abi_qualifier][0x01][target]
Description: Calls a subroutine at the specified target, saving the return address.
Implementation:
  - Must ensure the return address is saved according to the specified ABI
  - Must handle register saving as required by the ABI
  - ABI qualifier of 0x00 uses the default ABI for the platform
```

#### RET - Return from Subroutine
```
Opcode: 0x05
Qualifier: [abi_qualifier: uint8_t]
Operands: None or [value: operand_t]
Binary Format: [0x05][abi_qualifier][0x00] or [0x05][abi_qualifier][0x01][value]
Description: Returns from a subroutine, optionally with a return value.
Implementation:
  - Must restore any registers saved by the corresponding CALL
  - Must implement the return value according to the ABI
```

#### SYSC - System Call
```
Opcode: 0x09
Qualifier: [syscall_qualifier: uint8_t]
Operands: [syscall_number: operand_t][args: operand_t]...
Binary Format: [0x09][syscall_qualifier][operand_count][syscall_number][args]...
Description: Performs a system call with the given number and arguments.
Implementation:
  - Must map to the appropriate system call mechanism for the target platform
  - Must ensure arguments are passed according to the platform's system call convention
  - Should use the most efficient system call instruction available
```

### Arithmetic Instructions

#### ADD - Addition
```
Opcode: 0x10
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x10][arith_qualifier][0x03][dest][src1][src2]
Description: Adds src1 and src2, storing the result in dest.
Implementation:
  - Must handle signed/unsigned distinction based on qualifier
  - Must handle integer overflow according to qualifier
  - Should use the most efficient addition instruction available
```

#### SUB - Subtraction
```
Opcode: 0x11
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x11][arith_qualifier][0x03][dest][src1][src2]
Description: Subtracts src2 from src1, storing the result in dest.
Implementation:
  - Must handle signed/unsigned distinction based on qualifier
  - Must handle integer overflow according to qualifier
  - Should use the most efficient subtraction instruction available
```

#### MUL - Multiplication
```
Opcode: 0x12
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x12][arith_qualifier][0x03][dest][src1][src2]
Description: Multiplies src1 by src2, storing the result in dest.
Implementation:
  - Must handle signed/unsigned distinction based on qualifier
  - Must handle integer overflow according to qualifier
  - Should use the most efficient multiplication instruction available
```

#### DIV - Division
```
Opcode: 0x13
Qualifier: [arith_qualifier: uint8_t]
Operands: [dest: operand_t][src1: operand_t][src2: operand_t]
Binary Format: [0x13][arith_qualifier][0x03][dest][src1][src2]
Description: Divides src1 by src2, storing the result in dest.
Implementation:
  - Must handle signed/unsigned distinction based on qualifier
  - Must handle division by zero according to qualifier
  - Should use the most efficient division instruction available
```

### Data Movement Instructions

#### MOV - Move Data
```
Opcode: 0x40
Qualifier: 0x00
Operands: [dest: operand_t][src: operand_t]
Binary Format: [0x40][0x00][0x02][dest][src]
Description: Copies the value from src to dest.
Implementation:
  - Must handle different operand types (immediate, register, memory, etc.)
  - Should optimize out redundant moves where possible
```

#### LOAD - Load from Memory
```
Opcode: 0x41
Qualifier: [memory_qualifier: uint8_t]
Operands: [dest: operand_t][addr: operand_t]
Binary Format: [0x41][memory_qualifier][0x02][dest][addr]
Description: Loads a value from memory at addr into dest.
Implementation:
  - Must handle memory access according to qualifiers
  - Should use the most efficient load instruction available
```

#### STORE - Store to Memory
```
Opcode: 0x42
Qualifier: [memory_qualifier: uint8_t]
Operands: [addr: operand_t][src: operand_t]
Binary Format: [0x42][memory_qualifier][0x02][addr][src]
Description: Stores the value of src to memory at addr.
Implementation:
  - Must handle memory access according to qualifiers
  - Should use the most efficient store instruction available
```

### Other Important Instructions

#### ENTER - Function Prologue
```
Opcode: 0xC0
Qualifier: [abi_qualifier: uint8_t]
Operands: [frame_size: operand_t]
Binary Format: [0xC0][abi_qualifier][0x01][frame_size]
Description: Sets up a stack frame for a function according to the specified ABI.
Implementation:
  - Must save all registers required by the ABI
  - Must allocate stack space for local variables
  - Must maintain proper stack alignment
```

#### LEAVE - Function Epilogue
```
Opcode: 0xC1
Qualifier: [abi_qualifier: uint8_t]
Operands: None
Binary Format: [0xC1][abi_qualifier][0x00]
Description: Cleans up a stack frame when exiting a function.
Implementation:
  - Must restore all registers saved by ENTER
  - Must restore the stack pointer
  - Must maintain proper stack alignment
```

## Implementation Requirements

COIL processors must adhere to the following requirements:

1. **Instruction Fidelity**: All instructions must be implemented with the semantics defined in this specification.

2. **Feature-Based Implementation**: When a native instruction exists that matches the COIL instruction semantics, it should be used. Otherwise, the processor should generate the equivalent functionality through emulation.

3. **Optimization Freedom**: Processors are free to optimize instruction sequences as long as the observable behavior remains the same.

4. **Error Handling**: Processors should implement architecture-appropriate error handling for exceptional conditions.

5. **ABI Compliance**: Instructions that interact with ABIs must follow the ABI definitions in the configuration.

## Verification

A correctly implemented COIL processor should pass a verification suite that tests:

1. Instruction encoding/decoding
2. Semantic correctness for each instruction
3. Proper handling of edge cases
4. ABI compliance

Such verification suites are not part of this specification but are recommended for implementations.

## Binary Compatibility Guarantees

This instruction set specification establishes the following guarantees:

1. All COIL 1.0.0 processors must implement all instructions defined in this specification.
2. Future versions of COIL will maintain backward compatibility with version 1.0.0 instructions.
3. Future COIL processors must be able to process COIL 1.0.0 instructions correctly.

## Extension Mechanism

The opcode range 0xE0-0xFF is reserved for implementation-specific extensions. Extensions must be documented by the processor implementation and should follow the same general format as standard instructions.

## Architecture-Specific Considerations

While COIL is architecture-independent, the following considerations apply when implementing the ISA for specific architectures:

### x86-64
- Efficient use of CISC-style instructions
- Proper handling of condition codes
- Optimization for complex addressing modes

### ARM
- Efficient use of predicated execution
- Proper register allocation for ARM calling conventions
- Handling of ARM-specific memory ordering models

### RISC-V
- Efficient decomposition of complex COIL instructions into RISC-V primitives
- Proper implementation of compressed instruction formats
- Handling of RISC-V's weak memory model