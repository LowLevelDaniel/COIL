# Computer Oriented Intermediate Language
Computer Oriented Intermediate Language (COIL) is an intermediate language built for all types of processing units including (CPUs, GPUs, TPUs and all other processing units which take in opcode and perform actions). COIL is built to be highly optimizable made to be compatible with all potential processing extensions like vectors, encryption or even security like built in pages via the cpu and virtualization extensions. 
<br>
COIL exposes such features through something like a float-vec-3 which can be a typename for some simd instruction register set and when coil encounters this type the assembler can perform optimizations using any available simd instruction set that supports the type wanted.
<br>
Anything that isn't supported by the processing unit will be supported the COIL assembler though a static library or inline code i.e the vector operations will be unrolled and performed synchrously for processing units lacking vector support.
<br>
All possible extensions a processing unit can support COIL will support.
<br>
<br>
This Repositroy defines no such COIL Assembler, a seperate COILA exists for this purpose. Here is the needed functionality to read and write COIL tokens to be used by COIL assemblers and COIL writers.

 


