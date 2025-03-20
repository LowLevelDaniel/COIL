# COIL Explanatory Language (CEL) (Version 1.0.0)

CEL is a human-readable representation of COIL binary code. It is important to understand that CEL is not a real programming language that gets compiled, but rather a documentation tool used for explaining the COIL binary format, especially in the standard section documentation.

**IMPORTANT NOTE:** CEL exists solely as a documentation tool within this specification. Neither CEL nor the COIL binary format have actual implementations in this repository, which contains only documentation describing the COIL specification.

## Purpose of CEL

You can safely ignore CEL for the most part if you are just building a COIL processor, though it may help you understand the binary format. Other documentation may use C syntax for enumerations due to its simple keyword structure. Understanding C and assembly concepts will be beneficial when working with COIL.

CEL uses assembly-like syntax to represent COIL binary instructions in a human-readable form. Each CEL instruction maps directly to a COIL binary instruction, which in turn can be processed by a COIL processor into native code.

For example, a COIL binary instruction for adding two registers might be represented in CEL as:

```
ADD RQ0, RQ1, RQ2  ; Add registers RQ1 and RQ2, store in RQ0
```

But the actual COIL format is binary data following the structure defined in the ISA specification.

## Key Points About CEL

- CEL is not a programming language
- CEL is not processed by COIL processors
- CEL is purely for human readability and documentation
- CEL provides a consistent way to document COIL binary instructions
- CEL syntax intentionally resembles assembly for familiarity

## Relationship to COIL Binary Format

Remember: COIL processors do not read or process CEL - they only process binary COIL Object Format (COF) files and produce Native COIL Object Format (NCOF) files. CEL exists solely to make the specification documentation more accessible to humans.

When reading the COIL specification, you will often see CEL examples followed by the equivalent binary encoding. Both represent the same operation, but the former is for human understanding while the latter is the actual format processed by implementations.

## CEL Syntax Conventions

### Instructions

Basic instruction format:
```
OPCODE [OPERANDS...]  ; Optional comment
```

Example:
```
MOV RQ0, 42        ; Load value 42 into register RQ0
ADD RQ1, RQ0, RQ2  ; RQ1 = RQ0 + RQ2
```

### Directives

Directives use a leading period:
```
.directive [parameters]
```

Example:
```
.version 1.0.0
.target CPU
.section .text, "x"
```

### Labels

Labels define locations in code:
```
label_name:
```

Example:
```
loop_start:
  ADD RQ0, RQ0, 1
  CMP RQ0, 10
  BRC NE, loop_start
```

### Data Definition

Data can be defined using appropriate directives:
```
.byte 0x12, 0x34, 0x56
.word 0x1234, 0x5678
.long 0x12345678
.quad 0x1234567890ABCDEF
.string "Hello, COIL"
```

## Documentation Usage

Throughout the COIL specification, CEL is used to illustrate how binary instructions would appear in a human-readable format. This documentation approach helps developers understand the intent and operation of COIL code without having to read binary values directly.

When implementing a COIL processor, refer to the binary format specifications rather than CEL examples, which are provided solely for clarity and documentation purposes.