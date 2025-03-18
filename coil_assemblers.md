# COIL Implementation Requirements

## Introduction

This document outlines the requirements and guidelines for implementing COIL toolchains, including assemblers, linkers, and runtime components. It defines what constitutes a compliant COIL implementation and provides guidance for tool developers to ensure compatibility and consistency across different implementations.

## Core Implementation Philosophy

COIL implementations must adhere to several fundamental principles:

1. **All-Feature Implementation Policy**: COIL assemblers must implement all operations defined in the COIL instruction set, either directly or through emulation.
2. **Binary Format Compliance**: Implementations must produce and consume valid COIL Object Format (COF) and Native Object Format (NOF) files according to the specifications.
3. **Cross-Platform Support**: Implementations must support COIL's cross-platform capabilities, including virtual registers, target switching, and multiple target architectures.
4. **Optimization Freedom**: While maintaining semantic correctness, implementations are free to apply optimizations to improve code quality.
5. **Error Handling**: Implementations must provide clear error messages and diagnostics for invalid or unsupported code.

## COIL Assembler Requirements

### 1. Instruction Set Implementation

A compliant COIL assembler must implement all instructions defined in the COIL Instruction Set Architecture (ISA). For each instruction, the assembler must use one of the following implementation strategies, in descending order of preference:

1. **Direct Native Implementation**: Use a single equivalent native instruction when available.
2. **Optimized Instruction Sequence**: Use a short sequence of native instructions that efficiently implement the operation.
3. **Inline Expansion**: Expand the instruction into a longer sequence of simpler operations.
4. **Runtime Library Call**: Generate a call to a runtime library function for complex operations.

The only exceptions to this requirement are operations that fundamentally require operating system support, such as certain atomic operations or system calls. For these operations, the assembler must provide a clear mechanism for accessing them through the target environment.

Examples of instruction implementation:

```
; Direct native implementation (x86-64)
MATH ADD R0, R1    ; Translates to: add rax, rbx

; Optimized instruction sequence (x86-64)
MATH ABS R0, R1    ; Translates to:
                  ; mov rax, rbx
                  ; test rax, rax
                  ; jge .skip
                  ; neg rax
                  ; .skip:

; Inline expansion (x86-64)
VEC ADD V0, V1, 10 ; Translates to:
                  ; vpaddd ymm0, ymm1, [constant_vector_10]

; Runtime library call (complex math)
MATH POW R0, R1, R2 ; Translates to:
                   ; call __coil_pow
```

### 2. Binary Format Generation

A compliant COIL assembler must be able to generate valid COIL Object Format (COF) files according to the specification. The generated COF files must include:

1. **Correct Header**: Valid magic number, version, and offsets.
2. **Target Information**: Complete target architecture descriptions.
3. **Section Data**: Properly structured code and data sections.
4. **Symbol Table**: Complete symbol information with proper attributes.
5. **Relocation Information**: Accurate relocation records for external references.
6. **Variable Information**: Complete variable metadata for optimization.
7. **Type Information**: Detailed type definitions for complex data structures.

The assembler must also be able to generate Native Object Format (NOF) files either directly or through a separate tool. The generated NOF files must include:

1. **Native Machine Code**: Correctly translated COIL instructions for each target architecture.
2. **Target Switch Information**: Proper records for transitions between target architectures.
3. **Data Mapping Information**: Accurate mapping of data between different target architectures.

### 3. Virtual Register System

A compliant COIL assembler must implement the virtual register system, which maps COIL's virtual registers to physical registers on the target architecture. The implementation must:

1. **Maintain Consistency**: Ensure that virtual registers are consistently mapped to the same physical registers throughout a function.
2. **Handle Constraints**: Respect architectural constraints such as special-purpose registers.
3. **Optimize Usage**: Allocate registers efficiently to minimize spilling.
4. **Support ABI Requirements**: Map registers according to the target ABI for function calls.

Binary encoding example for virtual register mapping:

```c
// Virtual register R0 mapped to physical register rax (x86-64)
struct {
  uint8_t vreg_id = 0x00;   // R0
  uint8_t preg_id = 0x00;   // rax
  uint8_t preg_class = 0x01; // General purpose
  uint8_t flags = 0x00;     // No flags
} reg_mapping_r0;
```

### 4. Variable Management System

A compliant COIL assembler must implement the variable management system, which tracks variables and their lifetimes. The implementation must:

1. **Allocate Storage**: Assign variables to registers or memory locations based on their lifetimes and usage patterns.
2. **Track Lifetimes**: Maintain accurate information about when variables are live.
3. **Perform Type Checking**: Validate operations based on variable types.
4. **Optimize Access**: Minimize memory operations for frequently accessed variables.
5. **Ensure Consistency**: Maintain correct variable state across control flow changes.

Binary encoding example for variable declaration:

```c
// Declaration of 32-bit integer variable
struct {
  uint8_t var_id = 0x00;    // Variable $0
  uint8_t storage_class = 0x00; // Automatic storage
  uint16_t type_id = 0x0001; // int32
  uint32_t flags = 0x00000000; // No flags
  uint32_t lifetime_start = 0x00000010; // Start position
  uint32_t lifetime_end = 0x00000100; // End position
} var_decl;
```

### 5. Target Architecture Support

A compliant COIL assembler must support multiple target architectures. For each supported architecture, the implementation must:

1. **Implement Instruction Mapping**: Map COIL instructions to appropriate native instructions.
2. **Handle Register Allocation**: Map virtual registers to physical registers efficiently.
3. **Support Target-Specific Features**: Provide access to target-specific instructions and registers when needed.
4. **Implement ABIs**: Support the target's application binary interfaces for function calls and system calls.

The assembler should automatically select the best instructions for the target architecture without requiring explicit queries or checks in the COIL code. For example, vector operations should automatically use SIMD instructions when available and fall back to scalar operations when necessary.

### 6. Target Switching Mechanism

A compliant COIL assembler must implement the target switching mechanism, which enables code to transition between different target architectures. The implementation must:

1. **Generate Switch Code**: Produce code that correctly transfers control between different target architectures.
2. **Handle Data Mapping**: Generate code that properly transfers data between target-specific formats.
3. **Preserve State**: Ensure that necessary state is preserved across target switches.
4. **Support Multiple Switching Modes**: Implement function calls, one-way jumps, and other switching modes.

Binary encoding example for target switch:

```c
// Target switch from x86-64 to ARM64
struct {
  uint32_t source_target = 0x00000001; // x86-64
  uint32_t dest_target = 0x00000002;  // ARM64
  uint32_t switch_type = 0x00000000;  // Function call
  uint32_t switch_flags = 0x00000001; // Save state
  uint32_t entry_point = 0x00000010;  // Entry point symbol
  uint32_t return_point = 0x00000020; // Return point symbol
  uint32_t data_mapping_count = 0x00000002; // Number of mappings
  uint32_t data_mapping_offset = 0x00000100; // Offset to mapping table
} target_switch;
```

### 7. Validation and Verification

A compliant COIL assembler must include validation and verification capabilities to ensure the correctness of COIL code. The implementation must:

1. **Type Checking**: Verify that operations are used with compatible types.
2. **Control Flow Validation**: Ensure that control flow is well-formed and structured.
3. **Variable Lifetime Checking**: Validate that variables are not used outside their lifetimes.
4. **ABI Compliance**: Verify that function calls adhere to the specified ABI requirements.
5. **Target Compatibility**: Check that target-specific operations are valid for the specified target.

### 8. Optimization Capabilities

A compliant COIL assembler should include optimization capabilities to improve the quality of generated code. Potential optimizations include:

1. **Register Allocation**: Intelligent allocation of variables to registers based on usage patterns.
2. **Instruction Selection**: Selection of the most efficient instruction sequences for each operation.
3. **Instruction Scheduling**: Reordering of instructions to minimize pipeline stalls and dependencies.
4. **Constant Folding**: Evaluation of constant expressions at compile time.
5. **Dead Code Elimination**: Removal of code that has no effect on the program's observable behavior.
6. **Strength Reduction**: Replacement of expensive operations with equivalent cheaper operations.
7. **Loop Optimization**: Transformation of loops for better performance (unrolling, fusion, etc.).

## COIL Linker Requirements

### 1. Object Format Support

A compliant COIL linker must be able to process COIL Object Format (COF) and Native Object Format (NOF) files. The implementation must:

1. **Parse Headers**: Correctly interpret file headers and tables.
2. **Process Sections**: Handle code, data, and metadata sections appropriately.
3. **Resolve Symbols**: Link symbol references between different object files.
4. **Process Relocations**: Apply relocations to resolve addresses and offsets.
5. **Handle Target-Specific Sections**: Process sections for different target architectures.

### 2. Link-Time Optimization

A compliant COIL linker should support link-time optimization to improve the quality of the final binary. Potential optimizations include:

1. **Interprocedural Optimization**: Optimizations that span function boundaries.
2. **Function Inlining**: Inlining of small or frequently called functions.
3. **Dead Function Elimination**: Removal of unused functions.
4. **Global Variable Optimization**: Optimizations based on global variable usage.
5. **Cross-Module Optimization**: Optimizations that span multiple input files.

### 3. Target Switching Support

A compliant COIL linker must support target switching between different architectures. The implementation must:

1. **Link Target-Specific Code**: Correctly link code sections for different target architectures.
2. **Resolve Cross-Target References**: Handle references between code for different targets.
3. **Generate Target Switch Tables**: Create tables that define how execution transitions between targets.
4. **Map Data Between Targets**: Define how data is mapped between different target architectures.

### 4. Output Format Generation

A compliant COIL linker must be able to generate various output formats depending on the target platform and use case. Supported formats should include:

1. **Executable**: Self-contained executable binary.
2. **Shared Library**: Dynamically loadable library with exported symbols.
3. **Static Library**: Archive of object files for later linking.
4. **Position-Independent Code**: Code that can be loaded at any address.
5. **Relocatable Object**: Object file that can be further linked.

### 5. Metadata Generation

A compliant COIL linker should generate metadata to support debugging and analysis. This metadata may include:

1. **Debug Information**: Symbol and line number information for debugging.
2. **Profile Information**: Data to support profile-guided optimization.
3. **Type Information**: Detailed type definitions for complex data structures.
4. **ABI Information**: Descriptions of function interfaces and calling conventions.

## COIL Runtime Requirements

### 1. Target Selection

A compliant COIL runtime must include mechanisms for selecting the appropriate target architecture based on the available hardware and specified preferences. The implementation must:

1. **Detect Hardware Capabilities**: Identify the features and capabilities of the current hardware.
2. **Select Optimal Target**: Choose the most appropriate target code based on the detected capabilities.
3. **Support Fallback Targets**: Fall back to compatible targets if the preferred target is not available.

### 2. Target Switching

A compliant COIL runtime must support switching between different target architectures during execution. The implementation must:

1. **Switch Execution Context**: Properly transition execution from one target to another.
2. **Map Data**: Correctly transform data between different target-specific formats.
3. **Preserve State**: Ensure that necessary state is preserved across target switches.
4. **Handle Errors**: Properly handle errors that occur during target switching.

### 3. Runtime Library

A compliant COIL runtime may include a runtime library that provides implementations of complex operations that are not directly supported by the hardware. This library should be efficient, portable, and compatible with the COIL ABI.

### 4. Optimization

A compliant COIL runtime should include capabilities for runtime optimization based on the specific hardware and execution characteristics. Potential optimizations include:

1. **Just-In-Time Compilation**: Dynamic compilation of code based on runtime information.
2. **Adaptive Optimization**: Adjustment of execution strategies based on observed behavior.
3. **Specialized Implementations**: Selection of specialized code paths for specific hardware features.

## Binary Format Validation

To ensure compatibility and correctness, COIL provides a reference validator for COIL Object Format (COF) and Native Object Format (NOF) files. This validator checks:

1. **Format Compliance**: Verification that the file adheres to the format specification.
2. **Consistency**: Checking that internal references and structures are consistent.
3. **Completeness**: Validation that all required information is present.
4. **Semantic Correctness**: Verification that the file's contents make logical sense.

Implementations are encouraged to use this validator to ensure that their output is compliant with the COIL specifications.

## Test Suite

A reference test suite is available to validate COIL implementations. This suite includes:

1. **Feature Tests**: Tests for each feature of the COIL instruction set.
2. **Corner Cases**: Tests for edge cases and unusual code patterns.
3. **Performance Tests**: Tests to evaluate the efficiency of generated code.
4. **Cross-Platform Tests**: Tests that verify correct behavior across multiple target architectures.

Implementations should pass all tests in the reference suite to be considered compliant.

## Implementation Certification

To be certified as a compliant COIL implementation, a toolchain must:

1. **Pass the Reference Test Suite**: Successfully complete all tests in the reference suite.
2. **Validate Binary Format Output**: Produce COF and NOF files that pass the reference validator.
3. **Implement All Required Features**: Support all features defined in the COIL specification.
4. **Provide Documentation**: Document any implementation-specific behaviors or extensions.

Certified implementations may use the "COIL Compliant" designation in their documentation and marketing materials.

## Conclusion

By adhering to these implementation requirements, COIL toolchains can ensure compatibility, correctness, and performance across different platforms and environments. The combination of strict format compliance, flexible optimization strategies, and comprehensive validation creates a robust ecosystem for cross-platform, low-level development.

COIL's implementation philosophy balances the need for consistency and compatibility with the flexibility to innovate and optimize. By defining what constitutes a compliant implementation while allowing freedom in how those requirements are met, COIL encourages a diverse ecosystem of tools that all work together seamlessly.