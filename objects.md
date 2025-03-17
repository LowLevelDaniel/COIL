# COIL Object Formats

## Introduction

COIL employs two distinct object formats to balance the needs of optimization, cross-platform compatibility, and integration with existing tools:

1. **COIL Object Format (COF)**: A rich intermediate format that preserves COIL's abstractions, enabling link-time optimization and cross-platform capabilities.

2. **Native Object Format (NOF)**: A specialized object format created specifically for COIL that contains machine code targeting multiple processing units and architectures. This format is designed to work with COIL-compliant linkers and provides superior support for heterogeneous computing environments.

This document specifies the structure and usage of these formats, as well as the processes for converting between them.

## Design Philosophy

The COIL object formats are designed according to several key principles:

1. **Preserving Information**: The COIL Object Format preserves all semantic information necessary for optimization and cross-platform adaptation.

2. **Tool Integration**: The Native Object Format is compatible with existing linkers, debuggers, and other tools for the target platform.

3. **Performance**: Both formats are designed for efficient processing, with minimal overhead for the functionality they provide.

4. **Extensibility**: The formats can be extended to support new features and target architectures without breaking compatibility.

## COIL Object Format (COF)

The COIL Object Format is a rich intermediate representation that preserves COIL's abstractions. It serves as the primary interchange format for COIL tools and can be thought of as "COIL in binary form."

### COF Structure

A COF file consists of several sections:

1. **Header**: Identifies the file as a COIL object and provides essential metadata.
2. **Configuration Table**: Contains the target configuration information.
3. **Symbol Table**: Lists all global symbols with their properties.
4. **Type Table**: Contains type definitions used in the program.
5. **Variable Table**: Lists all variables with their types and properties.
6. **Function Table**: Contains function definitions with their ABIs.
7. **Code Sections**: Contains the actual COIL instructions for each function.
8. **Data Sections**: Contains initialized data.
9. **Relocation Table**: Contains information for resolving references.
10. **Debug Information**: Optional debugging information.

This structure is detailed in the COIL Binary Format Specification.

### COF Benefits

The COIL Object Format provides several key benefits:

1. **Link-Time Optimization**: By preserving high-level semantic information, COF enables powerful link-time optimizations.

2. **Cross-Target Support**: COF can contain code for multiple target architectures, enabling heterogeneous computing and polyglot binaries.

3. **Deferred Target Selection**: Final target selection can be deferred until link time, enabling a single compiled object to be reused for multiple targets.

4. **Rich Debugging Information**: COF can include detailed debugging information tied to the original source code.

### COF Use Cases

The COIL Object Format is primarily used in the following scenarios:

1. **Intermediate Compilation**: As the output of the COIL assembler and input to the COIL linker or translator.

2. **Library Distribution**: As a format for distributing libraries that need to work across multiple target architectures.

3. **Whole-Program Optimization**: As input to whole-program optimization tools that analyze and optimize across function and module boundaries.

## Native Object Format (NOF)

The Native Object Format is a specialized format created specifically for COIL that contains optimized machine code targeting one or more processing units and architectures. Unlike traditional formats like ELF or PE, the NOF is designed from the ground up to support heterogeneous computing and cross-architecture compatibility.

NOF is processed by COIL-compliant linkers that understand its multi-architecture nature and can generate appropriate executable formats for the target platform. This approach allows COIL to maintain its cross-platform capabilities all the way through the linking stage, rather than losing this information in translation to traditional object formats.

### NOF Structure

The Native Object Format has a consistent structure across all platforms and includes:

1. **Header**: Identifies the file format and architecture.
2. **Section Table**: Lists the sections in the file.
3. **Code Sections**: Contains the native machine code.
4. **Data Sections**: Contains initialized data.
5. **Symbol Table**: Lists all global symbols with their addresses.
6. **Relocation Table**: Contains information for resolving references.
7. **Debug Information**: Optional debugging information in a format like DWARF or PDB.

### NOF Benefits

The Native Object Format provides several key benefits:

1. **Tool Compatibility**: NOF files can be processed by standard tools like linkers, debuggers, and profilers for the target platform.

2. **Runtime Efficiency**: NOF files contain native machine code ready for execution without further translation.

3. **Size Efficiency**: NOF files typically include only the information needed for linking and execution, resulting in smaller file sizes compared to COF.

### NOF Use Cases

The Native Object Format is primarily used in the following scenarios:

1. **Final Linking**: As input to standard platform linkers to create executable files or shared libraries.

2. **Distribution**: As a format for distributing platform-specific libraries or modules.

3. **Integration**: For integration with existing codebases and build systems that expect standard object formats.

## Conversion Between Formats

The COIL toolchain includes tools for converting between the COIL Object Format and Native Object Formats:

### COF to NOF Conversion

The `coil-translate` tool converts a COIL Object Format file to a Native Object Format file for a specific target architecture. This process involves:

1. **Instruction Selection**: Mapping COIL instructions to native machine instructions for the target architecture.
2. **Register Allocation**: Assigning physical registers to COIL's virtual registers.
3. **Memory Layout**: Determining the memory layout for variables and data structures.
4. **ABI Adaptation**: Implementing the target's calling conventions and ABI requirements.
5. **Optimization**: Performing target-specific optimizations.
6. **Format Generation**: Generating the appropriate object file format for the target.

For example:
```
coil-translate input.cof -target=x86_64 -format=elf -o output.o
```

### NOF to COF Conversion

While less common, the `coil-decompile` tool can convert a Native Object Format file back to a partial COIL Object Format file through a process of decompilation. This process involves:

1. **Instruction Recognition**: Identifying patterns of native instructions that correspond to COIL instructions.
2. **Control Flow Analysis**: Reconstructing function boundaries and control flow structures.
3. **Type Inference**: Inferring types based on instruction usage patterns.
4. **Variable Recovery**: Identifying memory locations and registers used for variables.

This conversion is necessarily lossy, as some high-level information cannot be reliably recovered from machine code.

For example:
```
coil-decompile input.o -arch=x86_64 -o output.cof
```

## Linking

COIL provides two approaches to linking:

### COIL Linking

The `coil-link` tool links multiple COIL Object Format files into a single COF file, performing cross-module optimizations in the process:

1. **Symbol Resolution**: Resolving external references between modules.
2. **Function Inlining**: Inlining functions across module boundaries when beneficial.
3. **Dead Code Elimination**: Removing unused functions and variables.
4. **Constant Propagation**: Propagating constants across module boundaries.
5. **Target Specialization**: Specializing code for specific target configurations when final targets are known.

For example:
```
coil-link module1.cof module2.cof -o program.cof
```

### Native Linking

After conversion to Native Object Format, standard platform linkers can be used to create executable files or shared libraries:

For ELF (Linux, etc.):
```
ld module1.o module2.o -o program
```

For PE (Windows):
```
link module1.obj module2.obj /out:program.exe
```

For Mach-O (macOS):
```
ld module1.o module2.o -o program
```

## Mixed Format Workflow

COIL supports a mixed format workflow where some modules are kept in COF format until final linking, while others are converted to NOF early for integration with existing codebases:

```
# Compile COIL source to COF
coil-assemble module1.coil -o module1.cof
coil-assemble module2.coil -o module2.cof

# Link COIL modules with cross-module optimization
coil-link module1.cof module2.cof -o program.cof

# Convert to target-specific NOF
coil-translate program.cof -target=x86_64 -format=elf -o program.o

# Link with standard libraries using native linker
gcc program.o -lc -o program
```

This workflow enables COIL's advanced optimization capabilities while maintaining compatibility with existing toolchains.

## Example: Object Format Workflow

Here's a detailed example of a COIL object format workflow for a heterogeneous computing application:

### Source Files

**kernel.coil** (GPU code):
```
function vector_add(a: ptr<f32>, b: ptr<f32>, c: ptr<f32>, n: i32) -> void target(CUDA) {
    var idx : i32 = get_global_id(0)
    if (idx < n) {
        c[idx] = a[idx] + b[idx]
    }
}
```

**main.coil** (CPU code):
```
import "kernel.coil"

function main() -> i32 {
    var n : i32 = 1024
    var a : [1024]f32
    var b : [1024]f32
    var c : [1024]f32
    
    // Initialize arrays
    var i : i32 = 0
    while (i < n) {
        a[i] = (f32)i
        b[i] = (f32)(2 * i)
        i = i + 1
    }
    
    // Call GPU kernel
    var block_size : i32 = 256
    var grid_size : i32 = (n + block_size - 1) / block_size
    
    target(CUDA) {
        call vector_add(a, b, c, n) grid(grid_size) block(block_size)
    }
    
    // Verify results
    i = 0
    while (i < n) {
        if (c[i] != a[i] + b[i]) {
            return 1
        }
        i = i + 1
    }
    
    return 0
}
```

### Compilation to COF

```bash
# Compile kernel.coil to COF
coil-assemble kernel.coil -o kernel.cof

# Compile main.coil to COF
coil-assemble main.coil -o main.cof
```

The resulting COF files contain both CPU and GPU code, with the target switching preserved.

### Linking COF Files

```bash
# Link COF files together
coil-link kernel.cof main.cof -o program.cof
```

During linking, the COIL linker can perform optimizations like:
- Inlining the vector_add function into its call site if beneficial
- Specializing the GPU memory transfer code based on the specific CPU and GPU targets
- Optimizing the data layout for efficient CPU-GPU transfers

### Translation to NOF

```bash
# Translate to platform-specific NOF files
coil-translate program.cof -target=x86_64 -format=elf -o program_cpu.o
coil-translate program.cof -target=cuda -format=cubin -o program_gpu.cubin
```

The translation process:
1. For the CPU code:
   - Selects x86_64 instructions for the CPU portions
   - Generates appropriate CUDA runtime API calls for target switching
   - Creates an ELF object file

2. For the GPU code:
   - Translates COIL's GPU code to CUDA PTX or binary format
   - Creates a CUBIN file containing the GPU kernels

### Final Linking

```bash
# Link with system libraries to create executable
gcc program_cpu.o -lcudart -L/path/to/cuda/lib -o program
```

The final executable contains:
1. Native x86_64 code for the CPU portions
2. CUDA runtime calls for GPU interaction
3. Embedded or referenced GPU code for kernel execution

## Debugging Support

Both object formats include support for debugging information, though in different ways:

### COF Debugging Information

The COIL Object Format includes rich debugging information that maps directly to the COIL source code:

1. **Source Mapping**: Maps COIL instructions to source line numbers.
2. **Variable Information**: Includes type and location information for all variables.
3. **Type Information**: Includes complete type definitions.
4. **Function Information**: Includes parameter names, types, and scopes.
5. **Target Information**: Maps target-specific code to the original source.

### NOF Debugging Information

When translating to Native Object Format, COIL generates debugging information in the appropriate format for the target platform:

1. **DWARF** for ELF objects (Linux, BSD, etc.)
2. **PDB** for PE objects (Windows)
3. **STABS** or **DWARF** for Mach-O objects (macOS)

The debugging information maps the native machine code back to the original COIL source, enabling source-level debugging with standard tools.

### Example Debugging Workflow

```bash
# Compile with debugging information
coil-assemble program.coil -g -o program.cof

# Translate to native format with debugging information
coil-translate program.cof -target=x86_64 -format=elf -g -o program.o

# Link with debugging information
gcc program.o -g -o program

# Debug with standard tools
gdb ./program
```

## Performance Considerations

The choice between COIL Object Format and Native Object Format involves several performance trade-offs:

### COF Performance Characteristics

1. **File Size**: COF files are typically larger due to the rich semantic information they contain.
2. **Processing Time**: Processing COF files can be more computationally intensive due to the high-level analysis and optimization opportunities.
3. **Optimization Potential**: COF files enable more powerful cross-module and cross-target optimizations.

### NOF Performance Characteristics

1. **File Size**: NOF files are typically smaller, containing only the information needed for linking and execution.
2. **Processing Time**: Processing NOF files is usually faster, as they require less semantic analysis.
3. **Tool Integration**: NOF files integrate seamlessly with existing platform tools, avoiding the overhead of additional translation steps.

### Choosing the Right Format

For optimal performance, consider the following guidelines:

1. Use COF throughout the build process when:
   - Cross-module optimization is important
   - Target flexibility is required
   - The application targets multiple architectures

2. Convert to NOF early when:
   - Integration with existing codebases is the priority
   - Build time is a critical concern
   - The target architecture is fixed and known in advance

## Conclusion

COIL's dual object format approach provides a flexible foundation for low-level programming across diverse hardware architectures. The COIL Object Format enables powerful optimizations and cross-platform capabilities, while the Native Object Format ensures compatibility with existing tools and efficient execution on target platforms.

By understanding the strengths and use cases of each format, developers can leverage COIL's capabilities to create high-performance code that works seamlessly across heterogeneous computing environments while maintaining integration with established toolchains and ecosystems.