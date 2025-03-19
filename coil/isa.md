# Instruction Set Architecture

The COIL ISA is made to be simple with reliant on COIL assembler interpretation of an instruction via the types used. This means instructions can be summed down from addb, addw, addl, addq, addf, etc... into simply just add and it will automatically compile to right native instruction from the operands in the instruction. The format is defined first then the opcode. The type system is defined [here](./type.md) and should not be expected to be explained in this file.

## Format
The format of the instruction depends on the instruction. The only thing set in stone is the the frist 8 bits is an opcode. The rest after should be interpreted based on the opcode. Below is the list of opcodes supported, the version when they were added, version when they were removed (if applicable) and the structure of the instruction.

## Opcode

```c
enum opcode {
  OP_NOP = 0, /**< no operation */
  
  // Control Flow (0x01 - 0x4F)
  OP_CF_SYMB = 0x01, /**< define a symbol */
  OP_CF_BR   = 0x02, /**< unconditional branch to location (optional abi) */
  OP_CF_BRC  = 0x03, /**< conditional branch to location (optional abi) */
  OP_CF_CALL = 0x04, /**< unconditional branch to location with return address (and optional abi) */
  OP_CF_RET  = 0x05, /**< unconditional branch to return address */
  OP_CF_INT  = 0x05, /**< unconditional branch to interrupt table */
  OP_CF_IRET = 0x05, /**< unconditional branch to return address from interrupt table */
  OP_CF_WFI  = 0x05, /**< wait for interrupt */
  OP_CF_SYSC = 0x05, /**< unconditional branch to system call table */
  OP_CF_WFE  = 0x05, /**< wait for event */
  OP_CF_SEV  = 0x05, /**< send event */

  // Operand Manipulation (0x50 - 0x7F)
  OP_OM_ADD = 0x50, /**< addition operation */
  OP_OM_SUB = 0x51, /**< subtraction operation */
  OP_OM_MUL = 0x52, /**< multiplication operation */
  OP_OM_DIV = 0x53, /**< divide operation */ 
  OP_OM_MOD = 0x54, /**< modulus operation */
  OP_OM_NEG = 0x55, /**< negate operation */

  // Bit Manipulation (0x80 - 0x9F)
  OP_BM_AND = 0x80, /**< and operation */
  OP_BM_OR  = 0x81, /**< or operation */
  OP_BM_XOR = 0x82, /**< xor operation */
  OP_BM_NOT = 0x83, /**< not operation */
  OP_BM_SHL = 0x84, /**< shl operation */
  OP_BM_SHR = 0x85, /**< shr operation */

  // ...

  // Memory Operations (0xD0 - 0xEF)
  OP_MO_MOV  = 0xD0, /**< move value of one operand to another */
  OP_MO_PUSH = 0xD1, /**< push a state (a new type of coil representing a collection of register values like pusha or pushfd) */
  OP_MO_POP  = 0xD2, /**< pop a state (release the state into program) */
  OP_MO_CVAR = 0xD3, /**< create variable */
  OP_MO_DVAR = 0xD4, /**< delete variable */

  // Reserved (for assemblers who believe they need their own instructions)
  // utilize the 0xF. section. This will not be used by standard COIL and is left for you.
}

```

#### Control Flow
Control flow operations are mostly different from one another. Below is the format for each one.

...


#### Basic Arithmetic
All basic arithmetic instructions follow the same formula depending if the instruction is in place then it will look like one of the following
```
[opcode: uint8_t][qualifier: uint8_t][dest: operand_t][op1: operand_t][op2: operand_t] 

or

[opcode: uint8_t][qualifier: uint8_t][dest and op1: operand_t][op2: operand_t] 

operand_t = [qual: enum operand_qualifier][type: uint8_t][void: data[]]
```
## Enumerations

```c
enum operand_qualifier : uint8_t {
  COIL_OPQUAL_IMM = (1 << 0), // value is written into instruction
  COIL_OPQUAL_VAR = (1 << 1), // value is at variable destination
  // if neither then value is written into instruction
  // the rest are currently reserved
}
```

```c

```
