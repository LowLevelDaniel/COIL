# COIL Toolchain Integration

## Introduction

COIL (Computer Oriented Intermediate Language) occupies a unique position in the development toolchain, bridging the gap between high-level languages and machine code. This document outlines how COIL integrates with existing toolchains, the transformation process from COIL to native code, and the requirements for COIL assemblers and linkers.

## Toolchain Position

COIL sits at a crucial junction in the development pipeline:

```
High-Level Languages → [Compiler Frontend] → (COF) COIL → [COIL Assembler] → (NOF) Machine Code → [COIL Linker] → Final Binary
```

This positioning enables several key workflows:

1. **High-Level Language Compilation**: Compiler backends can target COIL instead of generating architecture-specific assembly, simplifying cross-platform development.

2. **Assembly Language Compilation**: Assembly language compilers can generate COIL code, providing a unified binary format across architectures.

3. **Direct COIL Development**: For performance-critical or cross-architecture code, developers can write COIL directly using a COIL assembler.

4. **Binary Translation**: Existing binaries can be translated to COIL for cross-architecture execution or optimization.

## Binary Nature of COIL

Unlike intermediate representations such as LLVM IR that have text-based representations, COIL is fundamentally a binary format designed for direct consumption by toolchains. This binary-first approach provides several advantages:

1. **Processing Efficiency**: Minimal parsing overhead for toolchain components.
2. **Compact Representation**: Efficient storage and transfer of intermediate code.
3. **Direct Hardware Mapping**: Binary structure closely aligns with hardware capabilities.
4. **Self-Contained**: All necessary information embedded in the binary format.

While COIL can be represented textually for debugging and human inspection, the authoritative representation is always binary. This design choice aligns with COIL's goal of providing a direct, efficient bridge between high-level languages and machine code.

## All-Feature Implementation Policy

A defining characteristic of COIL is its "All Feature Implementation Policy," which requires that COIL assemblers must implement all operations defined in the COIL instruction set. This policy ensures consistency and portability across different target architectures.

### Implementation Strategies

COIL assemblers can implement COIL instructions using several strategies, in descending order of preference:

1. **Direct Native Implementation**: When the target architecture directly supports an equivalent operation, the assembler maps the COIL instruction to the corresponding native instruction.

   ```
   ; COIL instruction
   MATH ADD R0, R1, R2
   
   ; x86-64 native implementation
   add rax, rbx  ; Assuming R0 maps to rax and R1 maps to rbx
   ```

2. **Optimized Instruction Sequences**: When no direct equivalent exists, the assembler generates an optimized sequence of native instructions.

   ```
   ; COIL instruction
   MATH ABS R0, R1
   
   ; x86-64 native implementation (no direct ABS instruction)
   mov rax, rbx      ; Copy source to destination
   test rax, rax     ; Set flags based on value
   jge .skip_neg     ; Skip negation if already positive
   neg rax           ; Negate if negative
   .skip_neg:
   ```

3. **Inline Expansion**: For more complex operations, especially those involving SIMD or specialized arithmetic, the assembler may expand the COIL instruction into an inline sequence of operations.

   ```
   ; COIL instruction
   MATH ADD $0, 10  ; Add immediate to vector variable
   
   ; Expanded implementation (if vector + immediate not directly supported)
   MATH ADD VEC GET($1, 0), 10  ; Add to first element
   MATH ADD VEC GET($1, 1), 10  ; Add to second element
   MATH ADD VEC GET($1, 2), 10  ; Add to third element
   MATH ADD VEC GET($1, 3), 10  ; Add to fourth element
   ```

4. **Runtime Library Calls**: For operations that would be inefficient to implement inline, especially those requiring significant code size, the assembler may generate a call to a runtime library function.

   ```
   ; COIL instruction
   MATH POW R0, R1, R2  ; Power function (x^y)
   
   ; Implementation via runtime library call
   call __coil_pow  ; Call to runtime power function
   ```

This approach ensures that all COIL instructions can be executed on any supported target architecture, though with varying efficiency based on the hardware capabilities.

### Exception for Environment-Dependent Operations

The only exception to the all-feature implementation policy is for operations that fundamentally require operating system support, particularly certain atomic operations. These operations cannot be efficiently emulated inline and require runtime library support. In such cases, the COIL assembler must provide a clear mechanism for accessing these operations through the target environment.

## Transformation Process

The transformation from COIL to native code involves several key steps:

1. **Target Configuration Selection**: The COIL assembler selects the appropriate target configuration based on the specified target architecture.

2. **Instruction Mapping**: Each COIL instruction is mapped to one or more native instructions according to the implementation strategies described above.

3. **Register Allocation**: Virtual registers are mapped to physical registers based on the target architecture's register set and the specified ABI.

4. **Variable Management**: Variables are allocated to registers or memory locations based on their lifetimes, usage patterns, and the available registers.

5. **Optimization**: Architecture-specific optimizations are applied to the generated code, such as instruction scheduling, peephole optimization, and target-specific patterns.

6. **Binary Generation**: The final native code is generated in the appropriate binary format (NOF or platform-specific format).

## Multiple Target Support

One of COIL's unique capabilities is its support for multiple target architectures within a single binary. This feature enables several advanced use cases:

1. **Heterogeneous Computing**: Different sections of code can target different processing units (CPU, GPU, DSP, etc.), with explicit control over data movement between units.

2. **Polyglot Binaries**: A single binary can contain optimized code for multiple CPU architectures, with runtime selection of the appropriate code path.

3. **Mode Transitions**: For system programming, COIL can express transitions between processor modes (e.g., x86 real mode to protected mode) with explicit control over the transition process.

The COIL toolchain supports these use cases through its target switching mechanism, which allows explicit control over when and how execution transitions between different target architectures.

## COIL Assembler Requirements

A compliant COIL assembler must meet the following requirements:

1. **Complete Instruction Support**: Implement all operations defined in the COIL instruction set according to the all-feature implementation policy.

2. **Binary Format Generation**: Generate valid COIL Object Format (COF) or Native Object Format (NOF) binary files.

3. **Multiple Target Support**: Support multiple target architectures within a single binary, with appropriate data mapping between targets.

4. **Optimization Capabilities**: Provide architecture-specific optimizations to generate efficient native code.

5. **Validation**: Perform validation of COIL code to ensure correctness and consistency.

6. **Error Reporting**: Provide clear error messages for invalid or unsupported code.

## COIL Linker Requirements

A compliant COIL linker must meet the following requirements:

1. **Object Format Support**: Process COIL Object Format (COF) and Native Object Format (NOF) binaries.

2. **Multiple Target Linking**: Link code targeting different architectures into a unified binary.

3. **Relocation Processing**: Resolve relocations and symbols across multiple target sections.

4. **Target Switching**: Preserve and validate target switching information.

5. **Binary Generation**: Generate executable binaries in the appropriate format for the target platform.

## Example Workflow

The following example demonstrates a typical workflow using COIL:

1. **Source Code**: High-level language code (e.g., C, C++, Rust) is written by the developer.

   ```c
   int add(int a, int b) {
     return a + b;
   }
   ```

2. **Compiler Frontend**: The compiler frontend processes the source code and generates an abstract syntax tree (AST) or intermediate representation.

3. **COIL Generation**: The compiler backend generates COIL code from the AST or IR.

   ```
   DIR HINT add FUNC GLOBAL
   DIR LABEL add
     FRAME ENTER
     
     ; Function parameters
     VAR DECL $0 : int32  ; Parameter 'a'
     VAR DECL $1 : int32  ; Parameter 'b'
     
     ; Compute result
     MATH ADD R0, $0, $1
     
     ; Cleanup
     VAR DLT $1
     VAR DLT $0
     
     FRAME LEAVE
     CF RET
   DIR HINT add ENDFUNC
   ```

4. **COIL Assembler**: The COIL assembler processes the COIL code and generates native code for the target architecture.

   For x86-64:
   ```
   add:
     push rbp
     mov rbp, rsp
     
     ; Parameters already in edi (a) and esi (b)
     
     ; Compute result
     mov eax, edi
     add eax, esi
     
     ; Return
     leave
     ret
   ```

   For ARM64:
   ```
   add:
     ; Parameters already in w0 (a) and w1 (b)
     
     ; Compute result
     add w0, w0, w1
     
     ; Return
     ret
   ```

5. **Linking**: The native code is linked with other object files to create the final executable.

This workflow demonstrates how COIL enables efficient cross-platform development by providing a consistent intermediate representation that can be targeted by multiple compiler backends and assembled for multiple target architectures.

## Conclusion

COIL's unique position in the development toolchain, combined with its binary-first design and all-feature implementation policy, provides a powerful foundation for cross-platform and low-level system programming. By bridging the gap between high-level languages and machine code, COIL enables efficient development for diverse hardware platforms while maintaining direct control over hardware resources.

The COIL toolchain's support for multiple target architectures within a single binary opens up new possibilities for heterogeneous computing, polyglot binaries, and complex system programming, making it a valuable addition to the modern developer's toolkit.