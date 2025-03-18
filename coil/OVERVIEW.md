# Overview

COIL is made up 5 major componenets to make it powerful and optimizable. The majority of the work should be done by the COIL assembler which is why so many features are supported at the level which should be right above the assembly language. This is why COIL is split into levels. 

## Core Principles
Lets start of with COILs core principles. What is governing the team behind this project, why we do this and where we expect this to lead.

1. *Reliability*
  COIL should  rarely reach a state where a feature that is possible on other systems are not possible on this system. From supporting 64 bit integers on 16 bit systems to supporting floats on systems without FPUs. COIL wants to allow the developer to not worry about what is going on inside their system which is especially useful for library developers who don't know what system they will have their code compiled/assembled on. COIL wishes to create a unified configuration for targeting systems in a universal and always supported system. COIL knows emulating every feature isn't always possible but we will try when it is needed.
2. *Optimizability*
  COIL takes on more high level language features into its framework not only to take the load of the compiler but to improve the ability to optimize. Performant code is important to LLT and the world and COIL will take this to heart. We aim to be able to optimize at least as good as any other current compiler and then we will go further. Utilizing native instructions that only exist on the specific architecture will be done where possible or not done at all if requested. These optimizations will never change the outcome of your code, we simply wish to make it run faster and utilize faster instructions.
3. *Self Reliant*
  COIL binaries should rarely if ever need runtime support like a linked standard library or even worse some interpreter environment. We will have support for both as interpreter environments are of course very useful in debugging and who can live without a standard library, but this is not forced upon the programmer and when it is possible not to link the COIL assembler is expected not to expect the linking of the standard library. Binaries should be able to run in bare metal environments when needed. COIL is there to support the programmer not limit you.
4. *Extensible*
  COIL should be able to be easily expanded by you. You need a specific instruction in COIL then you can add it in the COIL assembler. COIL assemblers are expected to build in a nice and extensible way and we will try our best to ensure room for extensibility from the format.
5. *Integrable*
  COIL will be able to be integrated with whichever toolchain you currently support with minimal effort. Past COIL is only the COIL compatible linker. Before COIL can be a mixture of anything you want. COIL is here for you, not instead of you, nor in the way of you, COIL does not hold your hand through all difficulties but we ensure that the difficulty is solvable in a nice way once you decide on the solution.
6. *Low Level*
  It goes without saying from the name of the team behind this but COIL should not abstract features away with no way around abstractions. Abstractions can be really handy for optimizations and convenience but there should always be a way to access the underlying instruction functionality. For example abstracting stack management through ABIs and Variables is incredibly useful for optimizations but lacks underlying register control which is why a virtual register system is available for direct register manipulation.

## Core Componenets
Following the core principles are the core componenets of COIL. These are detailed more in the hyperlinks and should be checked out in your own time if you wish to understand more about how COIL works.

1. [*Directives*](./dir.md)
  Special Instructions to talk directly to the underlying COIL assembler. The COIL assembler you use could specify some of its own directives but these are not supported under the COIL format and should not be expected universally.
2. [*Types*](./type.md)
  The type system for COIL is really complex for an intermediate language that should be able to sit between assembly and native binaries but is required for certain optimizations and universability standards. Composite types like structures, unions, etc... will be supported by COIL including fixed types, abstract width and complex types. COIL will make room to implement all possible types at the lowest level to help with underlying optimizations. These features though could be blocked by versions of COIL. It is not expected for COIL assemblers implmenting a version less then 2.*.* to implement composite types. COIL will start with only fixed types and registers and move on as needed in major updates.
3. [*Instructions*](./isa.md)
  The COIL ISA. COILs Instruction Set is relatively simple, there is no complex logical expressions like with C if statements. Abstractions on top of logic is not supported. flag setting functions and branch conditionals are utilized. Arithmetic is the same as regular assembler. move operations and optional stack manipulation is all possible. This is does actually help i believe even if conditions would be nice for compiler developers at least not before 2.*.* they will not be considered. The only abstraction of assembly is variable, lifetime and stack management. 
4. [*Virtual Registers*](./reg.md)
  The COIL virtual register system is a way to provide access to the abstracted away registers. Providing a simple way to interact with registers is still highly important. For example with linux system calls where you want to write syscall(60, 0) and have it complete the linux exit syscall well you can define an ABI utilizing the virtual register system in the configuration format or directly in COIL code. This ABI could be called 'abi-linux-64-syscall' and it can be implemented separatly for ARM and x86 devices to specify that the first argument goes to rax for x86 and x0 for arm. This virtual register system could also be used for direct transfer into registers.
5. [*Object*](./obj.md)
  COIL object formats are split into two. One for COIL code and one for native binary output. The native binary output does need its own format for cases like mixed GPU and CPU code to link PTX code and etc... This is something for COIL compliant linker developers to worry about in the future. COF (COIL Object Format) is the output of compilers and the input of COIL assemblers, NCOF (Native COIL Object Format) is the output of COIL assemblers and the input of linkers.
6. [*Configuration*](./conf.md)
  The COIL configuration is a system to define features of CPUs in a universal format and specific format to help the optimizer attempt to emulate instructions that don't exist. This means the configuration files have no mention to architecture specific instructions. There is a different format for each processing unit but trust me this makes it easier. You should read more about this no matter who you are as this is integral to all stages of COIL compilation.
7. [*STD*](./std.md)
  The COIL standard library is talked about briefly in a little more detail below where you can get a good view of what features belong to the standard library, why and where they are needed and much more. Further reading here is mainly for Compiler developers who want to build their standard library on COIL standard library or expose COIL standard library directly.

## Levels

0. *RAW* 
  No dependency COIL code will exist specified under level 0 or RAW. Made for systems without an operating environment of any kind just a small compact binary with no bloat and only code defined by the developer.
1. *SSTD*
  A Simple Standard for COIL which can be statically linked into the binary for systems without dynamic linking capability but where size is not considered to be a problem. There is potential for bloat as its very rare that all of the SSTD will be used in your code but it does provide a nice convenience.
2. *ESTD*
  The Extended Standard for COIL has to be dynamically linked in almost all cases not that it would matter since it requires a well formed operating system environment with requirements for a lot of OS provided functionality. COIL code built using the ESTD should be expected to only work on major operating systems like windows, linux or bsd systems. Grahpics is not inherintly needed in ESTD as its listed as part of GSTD which is even more limited on the systems it provides.
3. *GSTD*
  The Graphics Standard for COIL is limited to a few select operating systems with a good background like Windows, Linux distros supporting waylay/x11 or other display managers, and certain bsd style systems like OSX. This Standard library can be useful especially in graphics related tasks like applications, games maybe even simulations and if you are only looking for a non developer to utilize the code written then this will come of no cost to you.
