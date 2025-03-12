# Core COIL

This section documents the core components of COIL that operate **without any runtime dependencies**. All core features:

- Work without any operating system or runtime library
- Function in bare-metal environments like bootloaders and firmware
- Are resolved entirely at compile/link time
- Map directly to hardware capabilities
- Maintain predictable and deterministic behavior

## Core Components

### [Type System](type_system.md)
A comprehensive type system that works without runtime type information. Includes primitive types, composites, and hardware-specific types, all statically resolved at compile time.

### [Memory Model](memory_model.md)
The memory space organization, allocation mechanisms, and access patterns. Supports direct hardware memory access for bare-metal environments.

### [Instruction Set](instruction_set.md)
The binary encoding for COIL operations, from basic arithmetic to specialized hardware instructions. All instructions can execute without runtime support.

### [Processing Units](processing_units.md)
The abstract model of computational devices targeted by COIL, including classification, capabilities, and characteristics.

### [Serialization Format](serialization_format.md)
The binary encoding format for COIL modules, designed to be efficient, self-contained, and usable in low-level environments.

### [Optimization](optimization.md)
Compile-time optimization strategies, techniques, and annotations, with no runtime overhead.

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

For applications that need to coordinate execution across multiple devices or integrate with operating systems, see the [Runtime Extensions](../runtime/README.md).