# Objects

## COIL Object Format (COF)
The output of compilers and the input to COIL processors. Similar to other object files this contains tables of implementations and tries to cut down for space efficiency. COIL code should be able to be transported across systems and processed on each system. This Object Format therefore needs to be compact to allow the transfer of data to be minimal between systems.

[Detailed specification to be developed as the instruction set architecture is finalized]

## Native COIL Object Format (NCOF)
The native object format does not need to be transferred and only needs to be able to be used by the COIL compatible linker to complete the final step. This object format is only needed realistically the same reason cuda has its own linker. Combining code across completely different processing units is a challenging task but one COIL will take on and this seems to be the best way.

[Detailed specification to be developed as the instruction set architecture is finalized]