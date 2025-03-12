# COIL Core Specification

This section documents the core components of COIL that operate **without any runtime dependencies**. All core features:

- Work without any operating system or runtime library
- Function in bare-metal environments like bootloaders and firmware
- Are resolved entirely at compile/link time
- Map directly to hardware capabilities
- Maintain predictable and deterministic behavior

## Core Components

### [Type System](type-system.md)
A comprehensive type system that works without runtime type information. Includes primitive types, composites, and hardware-specific types, all statically resolved at compile time.

### [Memory Model](memory-model.md)
The memory space organization, allocation mechanisms, and access patterns. Supports direct hardware memory access for bare-metal environments.

### [Instruction Set](instruction-set.md)
The binary encoding for COIL operations, from basic arithmetic to specialized hardware instructions. All instructions can execute without runtime support.

### [Processing Units](processing-units.md)
The abstract model of computational devices targeted by COIL, including classification, capabilities, and characteristics.

### [Serialization Format](serialization-format.md)
The binary encoding format for COIL modules, designed to be efficient, self-contained, and usable in low-level environments.

## Core Principles

1. **No Runtime System Required**
   - All operations can be performed without any runtime library
   - Perfect for environments where no OS is available

2. **Direct Hardware Mapping**
   - Operations map directly to hardware capabilities
   - No abstraction penalties or hidden costs

3. **Static Resolution**
   - All types, memory, and operations resolved at compile time
   - No dynamic dispatch or runtime type checking

4. **Minimal Memory Footprint**
   - No runtime metadata or support structures required
   - Efficient for memory-constrained devices

5. **Deterministic Behavior**
   - Predictable execution across implementations
   - Critical for real-time and safety-critical systems

## When to Use Core COIL

Core COIL is ideal for:

- **Bootloaders and firmware**
- **UEFI applications**
- **Operating system kernels**
- **Device drivers**
- **Embedded systems**
- **Safety-critical systems**
- **Real-time applications**

For applications that need to coordinate execution across multiple devices or integrate with operating systems, see the [Runtime Extensions](../runtime-spec/README.md).

## Core vs. Runtime Extensions

COIL makes a clear distinction between core functionality and optional runtime extensions:

| Feature | Core COIL | Runtime Extensions |
|---------|-----------|-------------------|
| Dependencies | None | May require OS or runtime |
| Environment | Works in bare-metal | Typically needs OS services |
| Device Scope | Single device | Multiple devices |
| Memory Model | Direct hardware access | May include abstractions |
| Error Handling | Compile-time checks | Runtime error handling |
| Device Discovery | Static configuration | Dynamic discovery |
| Synchronization | Basic primitives | Advanced coordination |

This separation ensures that core COIL remains usable in the most constrained environments while allowing extended functionality when appropriate.

## Implementation Guidelines

When implementing core COIL functionality:

1. Avoid dependencies on any external libraries or runtime systems
2. Ensure all operations can be statically resolved at compile/link time
3. Map operations directly to hardware capabilities where possible
4. Minimize memory usage and avoid hidden allocations
5. Provide clear error messages at compile time rather than runtime
6. Test thoroughly in bare-metal environments
7. Support direct hardware access for system programming
8. Maintain consistent behavior across implementations

## Example Use Cases

### Bootloader

```
// Example COIL bootloader (pseudo-code)
FUNCTION boot_entry() {
    // Initialize hardware
    MMIO_WRITE [UART_BASE+0x00], 0x03  // Init UART
    
    // Load kernel from disk
    disk_read(KERNEL_ADDRESS, KERNEL_SECTORS);
    
    // Jump to kernel
    JMP KERNEL_ENTRY;
}
```

### Device Driver

```
// Example COIL device driver (pseudo-code)
FUNCTION interrupt_handler() {
    // Read status register
    status = MMIO_READ [DEVICE_BASE+0x04];
    
    // Check error conditions
    IF_BIT_SET status, ERROR_BIT {
        reset_device();
    } ELSE {
        process_data();
    }
    
    // Acknowledge interrupt
    MMIO_WRITE [DEVICE_BASE+0x08], 1;
    
    RET;
}
```

These examples highlight the direct hardware access and bare-metal capabilities of core COIL.