# COIL Runtime Extensions

Runtime Extensions provide **optional capabilities** for managing execution across heterogeneous processing units and operating systems. These extensions are **not required** for core COIL functionality but offer significant advantages for complex environments.

> **Important:** Unlike core COIL components, runtime extensions typically require operating system support or a runtime library.

## Runtime Components

### [Execution Model](execution-model.md)
Extended execution models for heterogeneous computing, including multi-threading, device coordination, and asynchronous execution.

### [Device Management](device-management.md)
Discovering, querying, and managing multiple processing units, including device selection and capability detection.

### [Memory Management](memory-management.md)
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

If your application needs to operate without an OS or runtime library (e.g., firmware, bootloaders), use only [Core COIL](../core-spec/README.md) features.

## Implementation Examples

### Device Discovery

```c
// Example runtime extension for device discovery
#include "coil_runtime.h"

void discover_devices() {
    uint32_t device_count;
    
    // Get number of available devices
    coil_ext_get_device_count(&device_count);
    
    printf("Found %d devices:\n", device_count);
    
    // Query each device
    for (uint32_t i = 0; i < device_count; i++) {
        coil_device_info_t info;
        coil_ext_get_device_info(i, &info);
        
        printf("Device %d: %s (%s)\n", i, info.name, info.vendor);
        printf("  Type: %s\n", get_device_type_string(info.type));
        printf("  Compute Units: %d\n", info.compute_units);
        printf("  Memory: %zu bytes\n", info.global_memory_size);
    }
}
```

### Multi-Device Execution

```c
// Example runtime extension for multi-device execution
void execute_across_devices(coil_function_t kernel, size_t data_size) {
    uint32_t device_count;
    coil_ext_get_device_count(&device_count);
    
    // Divide work among available devices
    size_t work_per_device = data_size / device_count;
    
    // Create event for synchronization
    coil_ext_event_t completion_event;
    coil_ext_create_event(0, NULL, &completion_event);
    
    // Launch kernel on each device
    for (uint32_t i = 0; i < device_count; i++) {
        size_t offset = i * work_per_device;
        size_t work_size = (i == device_count - 1) ? 
                           (data_size - offset) : work_per_device;
        
        // Set kernel arguments
        coil_value_t args[2];
        args[0].uinteger = offset;
        args[1].uinteger = work_size;
        
        // Launch asynchronously
        coil_ext_launch_kernel_async(i, kernel, 1, &work_size, NULL, 
                                    args, NULL, completion_event);
    }
    
    // Wait for all devices to complete
    coil_ext_wait_event(completion_event, 0);
    coil_ext_release_event(completion_event);
}
```

These examples demonstrate the extended capabilities provided by runtime extensions, which go beyond what core COIL can do.

## Creating Runtime Libraries

Assembler creators can implement runtime libraries that provide these extensions. A typical implementation follows these steps:

1. Identify the platform-specific services needed (e.g., device management APIs)
2. Create a thin wrapper around these services with a consistent API
3. Implement the core COIL runtime extension interfaces
4. Ensure proper error handling and resource management
5. Optimize for performance and minimize overhead
6. Test across diverse hardware configurations

For detailed guidelines on implementing runtime libraries, see the [Assembler Creator Guide](../assembler-guide/README.md).

## Platform-Specific Considerations

Runtime extensions may vary across platforms:

- **Windows**: Utilize Windows APIs for device management and process/thread coordination
- **Linux**: Leverage Linux-specific features and interfaces
- **macOS/iOS**: Use Apple's device APIs and memory management
- **Android**: Adapt to Android's process model and hardware abstraction layer
- **Embedded OSes**: Consider resource constraints and specialized hardware

## Best Practices

When using runtime extensions:

1. **Maintain Core Compatibility**: Ensure your code can fall back to core functionality
2. **Feature Detection**: Check for extension availability before using
3. **Error Handling**: Implement robust error handling for runtime operations
4. **Resource Management**: Properly acquire and release runtime resources
5. **Platform Abstraction**: Abstract platform-specific details when possible
6. **Performance Optimization**: Consider overheads of runtime extensions
7. **Testing**: Test across diverse platforms and device configurations

## Further Reading

- [Core COIL Specification](../core-spec/README.md): The foundation that runtime extensions build upon
- [Execution Model](execution-model.md): Details on extended execution models
- [Device Management](device-management.md): Managing multiple processing units
- [Memory Management](memory-management.md): Cross-device memory operations
- [Debugging Support](debugging.md): Runtime debugging capabilities