# Computer Oriented Intermediate Language
Computer Oriented Intermediate Language (COIL) is the mix of a new format for cross compatible machine language, similar to wasm if you will. Unlike normal assembly/machine languages there are some abstractions to help developers, the compilers / interpreters should be in charge of optimizations, variables and symbols. The reason is of course because machines have different register counts and stack demands. By abstracting variables it allows the coil compiler / interpreter to use the registers based on the system. The reason i even call it an assembly language is simply because it doesn't have functions, its control flow is indistinguishable from assembly language. Control flow is of course similar on most systems.
<br>
<br>
COIL is built with CPUs to begin with to make it easier to get this project of the ground but in the future it should be an asynchronous cross device compatible language essentially meaning that it will support devices with the ability to complete parallel actions like CPUs and GPUs or any other processing unit in the future. Eventually it should also be compatible with quantum computers but this is of course maybe still a decade out from the beginning of this project.
<br>
<br>
This Repositroy defines no such COIL Assembler, a seperate COILA exists for this purpose. Here is the needed functionality to read and write COIL tokens to be used by COIL assemblers and COIL writers.
<br>
See COIL/in for COIL Assembler Creators or see COIL/out for programming language developers.
 


