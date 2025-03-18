# Objects

## COIL Object Format (COF)
The output of compilers and the input to COIL assemblers. Similar to other object files this contains tables of implementations and tries to cut down for space efficiency. COIL code should be able to be transported across systems and compiled on each system. This Object Format therefore needs to be compact to allow the transfer of data to be minimal between systems.

...

## Native COIL Object Format (NCOF)
The native object format does not need to be transferred and only needs to be able to be used by the COIL compatible linker to complete the final step. This object format is only needed realistically the same reason cuda has its own linker. Combining code across completely different processing units is a challenging task but one COIL will take on and this seems to be the best way.

...