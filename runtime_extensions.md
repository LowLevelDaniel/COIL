# COIL Runtime Extensions (Optional)

## 1. Introduction

COIL Runtime Extensions provide optional capabilities for managing execution across heterogeneous processing units and operating systems. These extensions are **not required** for core COIL functionality, which can operate at the lowest levels (e.g., bootloaders, firmware, UEFI applications). However, they offer significant advantages when developing applications that target multiple processing units or require operating system integration.

## 2. Extension Architecture

### 2.1 Extension Design Principles

COIL Runtime Extensions follow these principles:

1. **Optional**: All extensions are optional and not required for core COIL functionality
2. **Modular**: Extensions can be used independently of each other
3. **Minimal Overhead**: Extensions introduce minimal overhead when not used
4. **Platform-Specific**: Extensions may be platform-specific (OS, hardware vendor)
5. **Versioned**: Extensions follow semantic versioning for compatibility

### 2.2 Extension Categories

Runtime extensions are grouped into categories:

1. **Device Management**: Discovering and managing processing units
2. **Memory Management**: Advanced memory operations across devices
3. **Execution Management**: Coordinating execution across processing units
4. **Platform Integration**: Operating system-specific functionality
5. **Debugging & Profiling**: Runtime debugging and performance analysis

## 3. Device Management Extensions

### 3.1 Device Discovery

Optional extensions for discovering available processing units:

```c
/**
 * @brief Get the number of available devices
 * @param count Output parameter for device count
 * @return Status code
 */
coil_ext_status_t coil_ext_get_device_count(uint32_t* count);

/**
 * @brief Get device information
 * @param device_id Device identifier
 * @param info Output parameter for device information
 * @return Status code
 */
coil_ext_status_t coil_ext_get_device_info(
    coil_device_id_t device_id,
    coil_device_info_t* info);
```

### 3.2 Device Capabilities

Extensions for querying device capabilities:

```c
/**
 * @brief Get device capabilities
 * @param device_id Device identifier
 * @param capabilities Output parameter for device capabilities
 * @return Status code
 */
coil_ext_status_t coil_ext_get_device_capabilities(
    coil_device_id_t device_id,
    coil_device_caps_t* capabilities);

/**
 * @brief Check if a device supports a feature
 * @param device_id Device identifier
 * @param feature Feature to check
 * @param supported Output parameter set to 1 if supported, 0 otherwise
 * @return Status code
 */
coil_ext_status_t coil_ext_device_supports_feature(
    coil_device_id_t device_id,
    uint32_t feature,
    int* supported);
```

### 3.3 Device Selection

Extensions for selecting optimal devices:

```c
/**
 * @brief Select devices matching criteria
 * @param criteria Device selection criteria
 * @param devices Output array for matching device IDs
 * @param max_devices Maximum number of devices to return
 * @param count Output parameter for number of devices returned
 * @return Status code
 */
coil_ext_status_t coil_ext_select_devices(
    const coil_device_criteria_t* criteria,
    coil_device_id_t* devices,
    uint32_t max_devices,
    uint32_t* count);
```

## 4. Memory Management Extensions

### 4.1 Cross-Device Memory Operations

Extensions for managing memory across devices:

```c
/**
 * @brief Allocate memory accessible by multiple devices
 * @param devices Array of device IDs
 * @param device_count Number of devices
 * @param size Size in bytes to allocate
 * @param options Allocation options
 * @param memory Output parameter for allocated memory
 * @return Status code
 */
coil_ext_status_t coil_ext_allocate_shared_memory(
    const coil_device_id_t* devices,
    uint32_t device_count,
    size_t size,
    const coil_alloc_options_t* options,
    coil_ext_memory_t* memory);

/**
 * @brief Free shared memory
 * @param memory Memory to free
 * @return Status code
 */
coil_ext_status_t coil_ext_free_shared_memory(
    coil_ext_memory_t memory);
```

### 4.2 Memory Transfer

Extensions for optimized memory transfers:

```c
/**
 * @brief Copy data between devices
 * @param dst Destination memory on target device
 * @param dst_device Destination device ID
 * @param src Source memory on source device
 * @param src_device Source device ID
 * @param size Size in bytes to copy
 * @param options Copy options
 * @return Status code
 */
coil_ext_status_t coil_ext_copy_between_devices(
    void* dst,
    coil_device_id_t dst_device,
    const void* src,
    coil_device_id_t src_device,
    size_t size,
    const coil_copy_options_t* options);
```

### 4.3 Unified Memory

Extensions for unified memory across devices:

```c
/**
 * @brief Allocate unified memory (automatically migrated)
 * @param size Size in bytes to allocate
 * @param options Allocation options
 * @param memory Output parameter for allocated memory
 * @return Status code
 */
coil_ext_status_t coil_ext_allocate_unified_memory(
    size_t size,
    const coil_alloc_options_t* options,
    coil_ext_memory_t* memory);

/**
 * @brief Prefetch unified memory to a device
 * @param memory Unified memory
 * @param device_id Target device ID
 * @param offset Offset within memory
 * @param size Size in bytes to prefetch
 * @return Status code
 */
coil_ext_status_t coil_ext_prefetch_unified_memory(
    coil_ext_memory_t memory,
    coil_device_id_t device_id,
    size_t offset,
    size_t size);
```

## 5. Execution Management Extensions

### 5.1 Function Execution

Extensions for executing functions on specific devices:

```c
/**
 * @brief Execute a function on a specific device
 * @param device_id Target device ID
 * @param function Function to execute
 * @param args Arguments to the function
 * @param result Output parameter for function result
 * @param options Execution options
 * @return Status code
 */
coil_ext_status_t coil_ext_execute_on_device(
    coil_device_id_t device_id,
    coil_function_t function,
    const coil_value_t* args,
    coil_value_t* result,
    const coil_exec_options_t* options);
```

### 5.2 Kernel Execution

Extensions for executing compute kernels:

```c
/**
 * @brief Launch a compute kernel on a device
 * @param device_id Target device ID
 * @param kernel Kernel function
 * @param global_size Global work dimensions
 * @param local_size Local work dimensions
 * @param args Kernel arguments
 * @param options Kernel execution options
 * @return Status code
 */
coil_ext_status_t coil_ext_launch_kernel(
    coil_device_id_t device_id,
    coil_function_t kernel,
    const size_t* global_size,
    const size_t* local_size,
    const coil_value_t* args,
    const coil_kernel_options_t* options);
```

### 5.3 Synchronization

Extensions for synchronizing execution:

```c
/**
 * @brief Synchronize with a device
 * @param device_id Device to synchronize with
 * @return Status code
 */
coil_ext_status_t coil_ext_synchronize_device(
    coil_device_id_t device_id);

/**
 * @brief Create an event
 * @param device_id Device ID (0 for host)
 * @param options Event creation options
 * @param event Output parameter for event handle
 * @return Status code
 */
coil_ext_status_t coil_ext_create_event(
    coil_device_id_t device_id,
    const coil_event_options_t* options,
    coil_ext_event_t* event);

/**
 * @brief Wait for an event
 * @param event Event to wait for
 * @param timeout_ms Timeout in milliseconds (0 = no timeout)
 * @return Status code
 */
coil_ext_status_t coil_ext_wait_event(
    coil_ext_event_t event,
    uint32_t timeout_ms);
```

## 6. Platform Integration Extensions

### 6.1 Operating System Integration

Extensions for OS-specific functionality:

```c
/**
 * @brief Get current platform information
 * @param platform Output parameter for platform information
 * @return Status code
 */
coil_ext_status_t coil_ext_get_platform_info(
    coil_platform_info_t* platform);

/**
 * @brief Open a file using OS file system
 * @param filename File name
 * @param mode Open mode
 * @param file Output parameter for file handle
 * @return Status code
 */
coil_ext_status_t coil_ext_file_open(
    const char* filename,
    const char* mode,
    coil_ext_file_t* file);

/**
 * @brief Create a thread using OS threading
 * @param function Thread function
 * @param arg Thread argument
 * @param options Thread creation options
 * @param thread Output parameter for thread handle
 * @return Status code
 */
coil_ext_status_t coil_ext_thread_create(
    coil_ext_thread_func_t function,
    void* arg,
    const coil_thread_options_t* options,
    coil_ext_thread_t* thread);
```

### 6.2 Interoperability

Extensions for interoperating with other frameworks:

```c
/**
 * @brief Import external memory (e.g., CUDA, OpenCL)
 * @param external_type External memory type
 * @param external_memory External memory handle
 * @param options Import options
 * @param memory Output parameter for COIL memory
 * @return Status code
 */
coil_ext_status_t coil_ext_import_external_memory(
    coil_ext_external_type_t external_type,
    void* external_memory,
    const coil_import_options_t* options,
    coil_ext_memory_t* memory);

/**
 * @brief Import external function (e.g., CUDA kernel)
 * @param external_type External function type
 * @param external_function External function handle
 * @param options Import options
 * @param function Output parameter for COIL function
 * @return Status code
 */
coil_ext_status_t coil_ext_import_external_function(
    coil_ext_external_type_t external_type,
    void* external_function,
    const coil_import_options_t* options,
    coil_function_t* function);
```

## 7. Debugging & Profiling Extensions

### 7.1 Debug Support

Extensions for runtime debugging:

```c
/**
 * @brief Set a breakpoint in a function
 * @param function Function to set breakpoint in
 * @param location Breakpoint location
 * @param options Breakpoint options
 * @param breakpoint Output parameter for breakpoint handle
 * @return Status code
 */
coil_ext_status_t coil_ext_set_breakpoint(
    coil_function_t function,
    const coil_location_t* location,
    const coil_breakpoint_options_t* options,
    coil_ext_breakpoint_t* breakpoint);

/**
 * @brief Continue execution after a breakpoint
 * @param options Continue options
 * @return Status code
 */
coil_ext_status_t coil_ext_continue(
    const coil_continue_options_t* options);
```

### 7.2 Profiling

Extensions for performance profiling:

```c
/**
 * @brief Start profiling
 * @param options Profiling options
 * @return Status code
 */
coil_ext_status_t coil_ext_profiling_start(
    const coil_profiling_options_t* options);

/**
 * @brief Stop profiling
 * @param result Output parameter for profiling result
 * @return Status code
 */
coil_ext_status_t coil_ext_profiling_stop(
    coil_ext_profiling_result_t* result);
```

## 8. Extension Registration

### 8.1 Extension Query

Mechanism to discover available extensions:

```c
/**
 * @brief Query available extensions
 * @param extensions Output array for extension info
 * @param max_extensions Maximum number of extensions to return
 * @param count Output parameter for number of extensions returned
 * @return Status code
 */
coil_ext_status_t coil_ext_query_extensions(
    coil_ext_info_t* extensions,
    uint32_t max_extensions,
    uint32_t* count);

/**
 * @brief Check if an extension is available
 * @param extension_name Extension name
 * @param available Output parameter set to 1 if available, 0 otherwise
 * @return Status code
 */
coil_ext_status_t coil_ext_is_available(
    const char* extension_name,
    int* available);
```

### 8.2 Extension Loading

Mechanism to load extensions:

```c
/**
 * @brief Load an extension
 * @param extension_name Extension name
 * @param options Load options
 * @return Status code
 */
coil_ext_status_t coil_ext_load(
    const char* extension_name,
    const coil_ext_load_options_t* options);

/**
 * @brief Unload an extension
 * @param extension_name Extension name
 * @return Status code
 */
coil_ext_status_t coil_ext_unload(
    const char* extension_name);
```

## 9. Standard Extensions

### 9.1 Operating System Extensions

Standard OS-specific extensions:

| Extension Name | Description |
|----------------|-------------|
| `coil_ext_posix` | POSIX operating system support |
| `coil_ext_windows` | Windows operating system support |
| `coil_ext_linux` | Linux-specific functionality |
| `coil_ext_macos` | macOS-specific functionality |
| `coil_ext_android` | Android-specific functionality |
| `coil_ext_ios` | iOS-specific functionality |

### 9.2 Vendor Extensions

Hardware vendor-specific extensions:

| Extension Name | Description |
|----------------|-------------|
| `coil_ext_cuda` | NVIDIA CUDA integration |
| `coil_ext_opencl` | OpenCL integration |
| `coil_ext_vulkan` | Vulkan Compute integration |
| `coil_ext_metal` | Apple Metal integration |
| `coil_ext_directx` | DirectX Compute integration |
| `coil_ext_hip` | AMD HIP integration |
| `coil_ext_oneapi` | Intel oneAPI integration |

### 9.3 Framework Extensions

ML framework integration extensions:

| Extension Name | Description |
|----------------|-------------|
| `coil_ext_tensorflow` | TensorFlow integration |
| `coil_ext_pytorch` | PyTorch integration |
| `coil_ext_onnx` | ONNX integration |
| `coil_ext_mxnet` | MXNet integration |

## 10. Extension Implementation

### 10.1 Extension Interface

Standard extension interface:

```c
/**
 * @struct coil_extension_interface
 * @brief Standard extension interface
 */
typedef struct {
  uint32_t version;           /* Interface version */
  const char* name;           /* Extension name */
  const char* vendor;         /* Extension vendor */
  uint32_t(*get_function_count)(void);
  const char*(*get_function_name)(uint32_t index);
  void*(*get_function_pointer)(const char* name);
  coil_ext_status_t(*initialize)(void);
  coil_ext_status_t(*finalize)(void);
} coil_extension_interface_t;
```

### 10.2 Extension Discovery

Extensions are discovered through:

1. Standard library path search
2. Environment variable `COIL_EXTENSION_PATH`
3. Application-provided extension directories
4. System-specific locations

### 10.3 Extension Version Compatibility

Extensions follow semantic versioning:

1. Major version changes indicate incompatible API changes
2. Minor version changes indicate backward-compatible additions
3. Patch version changes indicate backward-compatible bug fixes

## 11. Using Extensions Without Runtime Dependency

Extensions can be used in two ways:

### 11.1 Static Linking

Extensions can be statically linked into applications:

1. Include extension headers in application code
2. Compile with extension implementation
3. Link extension code into final binary
4. No runtime loading required

### 11.2 Dynamic Extension Loading

Extensions can be dynamically loaded when needed:

1. Check for extension availability at runtime
2. Load extension if available
3. Use extension functionality if loaded
4. Fallback to core functionality if not available

```c
// Example of using extensions with fallback
int have_cuda = 0;
coil_ext_is_available("coil_ext_cuda", &have_cuda);

if (have_cuda) {
    // Use CUDA extension
    coil_ext_load("coil_ext_cuda", NULL);
    // Use CUDA-accelerated functions
} else {
    // Use CPU-based fallback
}
```

## 12. Extension Development Guidelines

### 12.1 Creating New Extensions

Guidelines for developing new extensions:

1. Follow the standard extension interface
2. Make all functionality optional and provide fallbacks
3. Use semantic versioning for compatibility
4. Document extension dependencies
5. Provide thread-safety guarantees

### 12.2 Extension Best Practices

Best practices for extension development:

1. Minimize performance overhead
2. Provide clear error messages
3. Make extensions composable
4. Support multiple versions of underlying technologies
5. Provide static linking option for embedded systems

### 12.3 Extension Testing

Guidelines for testing extensions:

1. Test with and without other extensions loaded
2. Test on all supported platforms
3. Test with different versions of dependencies
4. Provide conformance tests
5. Test error handling

## 13. Future Extension Areas

Potential future extension areas:

1. **Distributed Computing**: Support for multi-node execution
2. **Virtualization**: Support for virtualized environments
3. **Security**: Support for secure enclaves and trusted execution
4. **Real-time**: Support for real-time guarantees
5. **Custom Hardware**: Support for emerging hardware architectures
6. **Cloud Integration**: Support for cloud-based execution

## 14. Conclusion

COIL Runtime Extensions provide optional capabilities for complex heterogeneous systems while preserving COIL's ability to function at the lowest level without a runtime system. By keeping these extensions optional and modular, COIL can serve both as a low-level system programming language and as a high-level heterogeneous computing platform.