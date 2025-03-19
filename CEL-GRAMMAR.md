# COIL Explanatory Language (CEL)

CEL is a human-readable representation of COIL binary code. It is important to understand that CEL is not a real programming language that gets compiled, but rather a documentation tool used for explaining the COIL binary format, especially in the [standard section](./coil/std.md).

You can safely ignore CEL for the most part if you are just building a COIL processor, though it may help you understand the binary format. Other documentation may use C syntax for enumerations due to its simple keyword structure. Understanding C and assembly concepts will be beneficial when working with COIL.

CEL uses assembly-like syntax to represent COIL binary instructions in a human-readable form. Each CEL instruction maps directly to a COIL binary instruction, which in turn can be processed by a COIL processor into native code.

For example, a COIL binary instruction for adding two registers might be represented in CEL as:

```
ADD RQ0, RQ1, RQ2  ; Add registers RQ1 and RQ2, store in RQ0
```

But the actual COIL format is binary data following the structure defined in the ISA specification.

Remember: COIL processors do not read or process CEL - they only process binary COIL Object Format (COF) files and produce Native COIL Object Format (NCOF) files.

