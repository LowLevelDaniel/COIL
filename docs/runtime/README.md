# COIL Runtime Extensions

Runtime Extensions provide **optional capabilities** for managing execution across heterogeneous processing units and operating systems. These extensions are **not required** for core COIL functionality but offer significant advantages for complex environments.

> **Important:** Unlike core COIL components, runtime extensions typically require operating system support or a runtime library.

## Runtime Components

### [Execution Model](execution_model.md)
Extended execution models for heterogeneous computing, including multi-threading, device coordination, and asynchronous execution.

### [Device Management](device_management.md)
Discovering, querying, and managing multiple processing units, including device selection and capability detection.

### [Memory Management](memory_management.md)
Advanced memory operations across devices, including shared memory, unified memory, and cross-device transfers.

### [Debugging Support](debugging.md)
Runtime debugging capabilities, including breakpoints, profiling, and visual debugging tools.

## Key Differences from Core COIL

| Core COIL | Runtime Extensions |
|-----------|-------------------|
| No runtime dependencies | Requires runtime library or OS |
| Works in bare-metal environments | Typically needs OS services |
| All operations resolved at compile-time | May use runtime discovery and selection |
| Fixed memory allocation | Dynamic memory management |
| Single device operation | Multi-device coordination |
| Static error checking | Runtime error detection and handling |

## Extension Design Principles

1. **Optional and Modular**
   - All extensions are optional and not required for core functionality
   - Extensions can be used independently of each other

2. **Minimal Overhead**
   - Extensions introduce minimal overhead when not used
   - Core functionality remains efficient

3. **Platform-Specific**
   - Extensions may leverage OS-specific features
   - Different implementations for different platforms

4. **Clear Separation**
   - Clear boundary between core and extended functionality
   - Core can always function without extensions

## When to Use Runtime Extensions

Runtime Extensions are ideal for:

- **Heterogeneous computing** across multiple devices
- **Complex applications** that need OS services
- **High-performance computing** on multiple processing units
- **Machine learning workloads** across diverse accelerators
- **Applications with dynamic resource requirements**
- **Programs that need to adapt to available hardware**

If your application needs to operate without an OS or runtime library (e.g., firmware, bootloaders), use only [Core COIL](../core/README.md) features.