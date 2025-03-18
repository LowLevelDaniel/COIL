# Instruction Set Architecture

The COIL ISA is made to be simple with reliant on COIL assembler interpretation of an instruction via the types used. This means instructions can be summed down from addb, addw, addl, addq, addf, etc... into simply just add and it will automatically compile to right native instruction from the operands in the instruction. The format is defined first then the opcode. The type system is defined [here](./type.md) and should not be expected to be explained in this file.

## Format

```
[opcode:uint8_t][][]
```

## Opcode
