# Overview

COIL consists of 5 major components that make it powerful and optimizable. The majority of the work is done by the COIL processor, which transforms architecture-independent COIL binary code into architecture-specific native code. This is why COIL is split into abstraction levels.

## Core Principles
These principles govern the development of COIL and shape its future direction:

1. *Reliability*
  COIL should rarely reach a state where features possible on some systems are not possible on others. From supporting 64-bit integers on 16-bit systems to supporting floats on systems without FPUs, COIL allows developers to ignore underlying system details. This is especially useful for library developers who don't know the target system for their code. When necessary, COIL will emulate features to provide consistent behavior.

2. *Optimizability*
  COIL incorporates high-level language features to ease the burden on compilers and improve optimization opportunities. Performance is a core priority, and COIL aims to optimize at least as well as any current compiler, potentially going further. The processor will utilize architecture-specific instructions when possible (unless explicitly disabled) to improve performance without changing program behavior.

3. *Self Reliant*
  COIL binaries should rarely require runtime support like linked standard libraries or interpreter environments. While such support is available for debugging or convenience, it's never forced upon developers. Binaries should be able to run in bare metal environments when needed. COIL exists to support programmers, not limit them.

4. *Extensible*
  COIL is designed to be easily expanded. If you need a specific instruction, you can add it to your COIL processor implementation. COIL processors should be built with extensibility in mind, and the format itself provides room for customization.

5. *Integrable*
  COIL can be integrated with existing toolchains with minimal effort. The only COIL-specific component required is a COIL-compatible linker. COIL does not dictate your entire development process; it complements it.

6. *Low Level*
  As the name of the team suggests, COIL does not abstract features away without providing access to underlying functionality. Abstractions are useful for optimizations and convenience, but there should always be ways to directly control hardware features when needed. For example, while stack management is abstracted through ABIs and variables to help optimization, direct register control remains available through the virtual register system.

## Core Components
Following the core principles are the core components of COIL:

1. [*Directives*](./dir.md)
  Special instructions that control the COIL processor's behavior. The COIL processor you use may define implementation-specific directives, but these are not guaranteed to be supported universally.

2. [*Types*](./type.md)
  The COIL type system is complex for an intermediate language, but this complexity enables certain optimizations and universality. Initially (v0.1.0), COIL supports only fixed types and registers. Composite types like structures and unions will be added in version 2.0.0.

3. [*Instructions*](./isa.md)
  The COIL ISA (Instruction Set Architecture) is relatively simple. It avoids complex logical expressions (like C's if statements) and focuses on operations closer to traditional assembly: flag-setting functions, branch conditionals, arithmetic operations, move operations, and optional stack manipulation. The main abstractions are variable, lifetime, and stack management.

4. [*Virtual Registers*](./reg.md)
  The virtual register system provides access to the otherwise abstracted registers. This is important for operations like Linux system calls, where you might want to write syscall(60, 0) to execute the Linux exit syscall. You can define an ABI using the virtual register system in the configuration format or directly in COIL code to ensure proper register assignment across architectures.

5. [*Object*](./obj.md)
  COIL uses two object formats: COF (COIL Object Format) for architecture-independent code, and NCOF (Native COIL Object Format) for architecture-specific output. NCOF is particularly important for cases like mixed GPU and CPU code that need specialized linking.

6. [*Configuration*](./conf.md)
  The COIL configuration system defines processing unit features in a universal format to help the processor emulate instructions that don't exist on particular hardware. The configuration has no architecture-specific instructions, making it adaptable across different systems.

7. [*STD*](./std.md)
  The COIL standard library is organized into levels based on dependency requirements and platform support.

## Levels

0. *RAW* 
  No dependency COIL code exists at level 0 (RAW). This is for systems without an operating environment, where a small, compact binary with no external dependencies is required.

1. *SSTD*
  The Simple Standard can be statically linked into the binary for systems without dynamic linking capability but where size is not a critical concern. This may introduce some bloat since rarely will all SSTD functionality be used, but it provides convenience.

2. *ESTD*
  The Extended Standard typically requires dynamic linking and a well-formed operating system environment with OS-provided functionality. COIL code built using the ESTD should be expected to work only on major operating systems like Windows, Linux, or BSD systems.

3. *GSTD*
  The Graphics Standard is limited to select operating systems with display capabilities like Windows, Linux distros supporting Wayland/X11, and certain BSD-style systems like macOS. This standard library is useful for graphics-related tasks like applications, games, or simulations.
