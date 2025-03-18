# COIL ABI and Function System

## Introduction

The Application Binary Interface (ABI) and function system in COIL provides a structured approach to defining, calling, and implementing functions across different target architectures. This system enables interoperability between functions compiled from different source languages or for different target architectures, while maintaining performance and control. For example the linux system call which may use different argument registers for different architectures and would require manual virtual register mapping. This is even more of a problem when there are multiple calling conventions for system call like windows. An ABI defined for each architecture helps with cross architectural code.

## Design Philosophy

The COIL ABI and function system is designed with several principles in mind:

1. **Explicit Control**: Give programmers explicit control over calling conventions when needed.
2. **Cross-Platform Compatibility**: Allow functions to be called across different target architectures.
3. **Performance**: Enable high-performance function calls optimized for the target architecture.
4. **Interoperability**: Support calling and being called by functions written in other languages.
5. **Optimization-Friendly**: Provide enough information to enable advanced optimizations like inlining.

## ABI Configuration

ABIs in COIL are defined in the configuration file and specified in the ABI Mapping Table of the COIL binary format. Each ABI definition includes:

1. **Parameter Passing Convention**: How parameters are passed to functions (registers, stack, or a combination).
2. **Return Value Convention**: How return values are handled (registers, memory, or a combination).
3. **Stack Frame Convention**: How the stack frame is set up and torn down.
4. **Register Usage**: Which registers are caller-saved vs. callee-saved.
5. **Name Mangling**: How function names are encoded in the symbol table.

## Conclusion

COIL's ABI and function system provides a powerful yet flexible approach to defining and using functions across different target architectures. By making ABIs explicit and configurable, COIL enables high-performance, cross-platform code that can seamlessly interoperate with existing codebases while maintaining control over the underlying implementation details.