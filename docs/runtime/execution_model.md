# COIL Execution Model Extensions

The COIL Execution Model Extensions provide optional capabilities for managing execution across heterogeneous processing units. Unlike the core execution model, these extensions **require runtime support** and are typically used in environments with operating system services.

## Overview

- **Optional Extensions:** Not required for core COIL functionality
- **Runtime Dependent:** Requires a runtime system or OS services
- **Heterogeneous Support:** Coordinates execution across multiple devices
- **Dynamic Discovery:** Runtime detection of processing units
- **Asynchronous Execution:** Support for non-blocking operations

## Core vs. Extended Execution

The execution model has two tiers:

1. **Core Execution Model:** 
   - Works without any runtime system
   - Suitable for bare-metal environments
   - Single-device focused
   - Statically resolved at compile time

2. **Extended Execution Model:**
   - Requires runtime support
   - Coordinates multiple devices
   - Supports dynamic discovery and selection
   - Enables asynchronous execution

## Extended Execution Domains

| Domain ID | Name | Description | Runtime Required |
|-----------|------|-------------|------------------|
| 0x3 | SIMT | Single Instruction Multiple Threads | Yes |
| 0x4 | DATAFLOW | Data-driven execution | Yes |
| 0x7 | TASK | Task-based parallelism | Yes |
| 0x8 | PIPELINE | Advanced pipeline parallelism | Yes |

## Device Management Extensions

Extensions for managing multiple processing units:

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

## Multi-Device Execution

Extensions for executing code across multiple devices:

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

## Kernel Execution

Support for compute kernel execution:

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

## Asynchronous Execution

Extensions for non-blocking operations:

```c
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

## Cross-Device Synchronization

Extensions for synchronizing execution across devices:

```c
/**
 * @brief Synchronize with a device
 * @param device_id Device to synchronize with
 * @return Status code
 */
coil_ext_status_t coil_ext_synchronize_device(
    coil_device_id_t device_id);
```

## Task-Based Parallelism

Extensions for task-based parallel execution:

```c
/**
 * @brief Create a task
 * @param function Task function
 * @param args Task arguments
 * @param options Task creation options
 * @param task Output parameter for task handle
 * @return Status code
 */
coil_ext_status_t coil_ext_create_task(
    coil_function_t function,
    const coil_value_t* args,
    const coil_task_options_t* options,
    coil_ext_task_t* task);

/**
 * @brief Submit a task for execution
 * @param task Task to submit
 * @param device_id Target device (0 for automatic selection)
 * @param event Output parameter for completion event
 * @return Status code
 */
coil_ext_status_t coil_ext_submit_task(
    coil_ext_task_t task,
    coil_device_id_t device_id,
    coil_ext_event_t* event);
```

## Runtime Detection

Extensions can be detected at runtime:

```c
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

## Example: Heterogeneous Computing

```c
// Example of heterogeneous computing with runtime extensions
#include "coil_runtime.h"

int main() {
    // Initialize COIL runtime
    coil_ext_init();
    
    // Discover available devices
    uint32_t device_count;
    coil_ext_get_device_count(&device_count);
    
    // Get the first GPU device
    coil_device_id_t gpu_device = COIL_INVALID_DEVICE;
    for (uint32_t i = 0; i < device_count; i++) {
        coil_device_info_t info;
        coil_ext_get_device_info(i, &info);
        
        if (info.type == COIL_DEVICE_TYPE_GPU) {
            gpu_device = i;
            break;
        }
    }
    
    if (gpu_device != COIL_INVALID_DEVICE) {
        // Load a kernel function
        coil_function_t kernel = coil_ext_load_function("my_kernel");
        
        // Set up kernel launch parameters
        size_t global_size[2] = {1024, 1024};
        size_t local_size[2] = {16, 16};
        
        // Create input and output buffers
        coil_ext_memory_t input, output;
        coil_ext_allocate_buffer(gpu_device, sizeof(float) * 1024 * 1024, &input);
        coil_ext_allocate_buffer(gpu_device, sizeof(float) * 1024 * 1024, &output);
        
        // Set kernel arguments
        coil_value_t args[2];
        args[0].memory = input;
        args[1].memory = output;
        
        // Launch kernel
        coil_ext_launch_kernel(gpu_device, kernel, 2, global_size, local_size, args, NULL);
        
        // Wait for completion
        coil_ext_synchronize_device(gpu_device);
        
        // Clean up
        coil_ext_free_memory(input);
        coil_ext_free_memory(output);
    }
    
    // Shut down COIL runtime
    coil_ext_shutdown();
    
    return 0;
}
```

## When to Use Runtime Extensions

These execution model extensions should be used when:

1. Your application needs to coordinate across multiple devices
2. You need to discover devices at runtime
3. You need asynchronous execution capabilities
4. You need dynamic load balancing across devices
5. You're targeting heterogeneous computing environments

If your application needs to operate without an OS or runtime (e.g., firmware, bootloaders), use only the core execution model.