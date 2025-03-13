# Cross-Device Computing with COIL/HOIL

This example demonstrates how to use COIL's runtime extensions to coordinate computation across multiple device types (CPU and GPU). It implements a parallel image processing application that distributes work efficiently between available processing units.

## 1. Image Processing Pipeline

```
MODULE "image_processor";

// Image data structure
TYPE image {
    width: i32,
    height: i32,
    channels: i32,
    data: ptr<u8>
}

// Processing parameters
TYPE processing_params {
    brightness: f32,
    contrast: f32,
    saturation: f32,
    gamma: f32
}

// Device information structure
TYPE device_info {
    id: i32,
    name: ptr<i8>,
    type: i32,    // 0=CPU, 1=GPU, 2=Other
    memory: i64,
    compute_units: i32
}

// Constants for device types
CONSTANT DEVICE_CPU: i32 = 0;
CONSTANT DEVICE_GPU: i32 = 1;
CONSTANT DEVICE_OTHER: i32 = 2;

// Entry point for the application
FUNCTION main(argc: i32, argv: ptr<ptr<i8>>) -> i32 {
    ENTRY:
        // Check arguments
        valid_args = CALL check_arguments(argc, argv);
        BR valid_args, INIT, SHOW_USAGE;
        
    SHOW_USAGE:
        CALL print_usage(argv[0]);
        result = LOAD_I32 1;
        RET result;
        
    INIT:
        // Initialize runtime
        init_success = CALL initialize_runtime();
        BR init_success, DISCOVER_DEVICES, INIT_FAILED;
        
    INIT_FAILED:
        CALL print_error("Failed to initialize runtime");
        result = LOAD_I32 2;
        RET result;
        
    DISCOVER_DEVICES:
        // Discover available devices
        devices = CALL discover_devices();
        device_count = CALL get_device_count();
        
        has_devices = CMP_GT device_count, 0;
        BR has_devices, LOAD_IMAGE, NO_DEVICES;
        
    NO_DEVICES:
        CALL print_error("No compatible devices found");
        result = LOAD_I32 3;
        RET result;
        
    LOAD_IMAGE:
        // Load input image
        input_path = argv[1];
        input_image = CALL load_image(input_path);
        
        valid_image = CMP_NE input_image, NULL;
        BR valid_image, CREATE_OUTPUT, LOAD_FAILED;
        
    LOAD_FAILED:
        CALL print_error("Failed to load input image");
        result = LOAD_I32 4;
        RET result;
        
    CREATE_OUTPUT:
        // Create output image with same dimensions
        output_image = CALL create_image(input_image->width, input_image->height, input_image->channels);
        
        valid_output = CMP_NE output_image, NULL;
        BR valid_output, SETUP_PARAMS, CREATE_FAILED;
        
    CREATE_FAILED:
        CALL free_image(input_image);
        CALL print_error("Failed to create output image");
        result = LOAD_I32 5;
        RET result;
        
    SETUP_PARAMS:
        // Set up processing parameters
        params = UNDEF processing_params;
        params.brightness = LOAD_F32 1.1;  // Slightly brighter
        params.contrast = LOAD_F32 1.2;    // More contrast
        params.saturation = LOAD_F32 1.1;  // Slightly more saturated
        params.gamma = LOAD_F32 0.9;       // Gamma correction
        
        // Process the image using available devices
        CALL process_image_multi_device(input_image, output_image, params, devices, device_count);
        
        // Save the output image
        output_path = argv[2];
        save_success = CALL save_image(output_image, output_path);
        
        BR save_success, CLEANUP, SAVE_FAILED;
        
    SAVE_FAILED:
        CALL print_error("Failed to save output image");
        result = LOAD_I32 6;
        BR ALWAYS, CLEANUP;
        
    CLEANUP:
        // Clean up resources
        CALL free_image(input_image);
        CALL free_image(output_image);
        CALL cleanup_devices(devices, device_count);
        CALL shutdown_runtime();
        
        // Return success
        result = LOAD_I32 0;
        RET result;
}

// Initialize the runtime environment
FUNCTION initialize_runtime() -> i32 {
    ENTRY:
        // Initialize COIL runtime
        status = CALL runtime_init();
        success = CMP_EQ status, 0;
        
        BR success, INIT_SUCCESS, INIT_FAILED;
        
    INIT_SUCCESS:
        result = LOAD_I32 1;  // true
        RET result;
        
    INIT_FAILED:
        result = LOAD_I32 0;  // false
        RET result;
}

// Discover available devices
FUNCTION discover_devices() -> ptr<device_info> {
    ENTRY:
        // Query number of devices
        device_count = CALL runtime_get_device_count();
        
        // Allocate array for device info
        array_size = MUL device_count, SIZEOF(device_info);
        devices = CALL malloc(array_size);
        
        // Check allocation
        valid_alloc = CMP_NE devices, NULL;
        BR valid_alloc, ENUMERATE_DEVICES, ALLOC_FAILED;
        
    ALLOC_FAILED:
        RET NULL;
        
    ENUMERATE_DEVICES:
        // Populate device information
        i = LOAD_I32 0;
        BR ALWAYS, ENUM_CHECK;
        
    ENUM_CHECK:
        enum_done = CMP_GE i, device_count;
        BR enum_done, ENUM_DONE, ENUM_DEVICE;
        
    ENUM_DEVICE:
        // Get device info
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        
        // Set device ID
        id_ptr = ADD device_ptr, OFFSETOF(device_info, id);
        STORE [id_ptr], i;
        
        // Get device name
        name = CALL runtime_get_device_name(i);
        name_ptr = ADD device_ptr, OFFSETOF(device_info, name);
        STORE [name_ptr], name;
        
        // Get device type
        type = CALL runtime_get_device_type(i);
        type_ptr = ADD device_ptr, OFFSETOF(device_info, type);
        STORE [type_ptr], type;
        
        // Get device memory
        memory = CALL runtime_get_device_memory(i);
        memory_ptr = ADD device_ptr, OFFSETOF(device_info, memory);
        STORE [memory_ptr], memory;
        
        // Get compute units
        compute_units = CALL runtime_get_device_compute_units(i);
        compute_units_ptr = ADD device_ptr, OFFSETOF(device_info, compute_units);
        STORE [compute_units_ptr], compute_units;
        
        // Print device info
        CALL print_device_info(device_ptr);
        
        // Next device
        i = ADD i, 1;
        BR ALWAYS, ENUM_CHECK;
        
    ENUM_DONE:
        // Store device count in global
        CALL set_device_count(device_count);
        
        RET devices;
}

// Process image using multiple devices
FUNCTION process_image_multi_device(input: ptr<image>, output: ptr<image>, 
                                  params: processing_params,
                                  devices: ptr<device_info>,
                                  device_count: i32) -> void {
    ENTRY:
        // Calculate total pixels
        width = input->width;
        height = input->height;
        channels = input->channels;
        total_pixels = MUL width, height;
        
        // Prepare for multi-device execution
        CALL prepare_execution_plan(total_pixels, devices, device_count);
        
        // Distribute work based on device capabilities
        CALL execute_image_processing(input, output, params, devices, device_count);
        
        RET;
}

// Prepare execution plan based on device capabilities
FUNCTION prepare_execution_plan(total_work: i32, devices: ptr<device_info>, device_count: i32) -> void {
    ENTRY:
        // Calculate total compute capability (simplified)
        total_capability = LOAD_I32 0;
        i = LOAD_I32 0;
        BR ALWAYS, SUM_CHECK;
        
    SUM_CHECK:
        sum_done = CMP_GE i, device_count;
        BR sum_done, ALLOCATE_WORK, SUM_CAPABILITY;
        
    SUM_CAPABILITY:
        // Get device info
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        
        // Get compute units
        compute_units_ptr = ADD device_ptr, OFFSETOF(device_info, compute_units);
        compute_units = LOAD [compute_units_ptr];
        
        // Get device type and apply weighting factor
        type_ptr = ADD device_ptr, OFFSETOF(device_info, type);
        type = LOAD [type_ptr];
        
        weight = LOAD_I32 1;
        is_gpu = CMP_EQ type, DEVICE_GPU;
        BR is_gpu, APPLY_GPU_WEIGHT, ADD_CAPABILITY;
        
    APPLY_GPU_WEIGHT:
        // GPUs get higher weight for image processing tasks
        weight = LOAD_I32 15;  // Assume GPU is ~15x faster per compute unit
        
    ADD_CAPABILITY:
        // Add weighted compute units to total
        weighted_units = MUL compute_units, weight;
        total_capability = ADD total_capability, weighted_units;
        
        // Next device
        i = ADD i, 1;
        BR ALWAYS, SUM_CHECK;
        
    ALLOCATE_WORK:
        // Allocate work proportionally to capability
        i = LOAD_I32 0;
        work_start = LOAD_I32 0;
        BR ALWAYS, ALLOC_CHECK;
        
    ALLOC_CHECK:
        alloc_done = CMP_GE i, device_count;
        BR alloc_done, DONE, ALLOC_DEVICE_WORK;
        
    ALLOC_DEVICE_WORK:
        // Get device info
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        
        // Get compute units
        compute_units_ptr = ADD device_ptr, OFFSETOF(device_info, compute_units);
        compute_units = LOAD [compute_units_ptr];
        
        // Get device type 
        type_ptr = ADD device_ptr, OFFSETOF(device_info, type);
        type = LOAD [type_ptr];
        
        // Calculate weight
        weight = LOAD_I32 1;
        is_gpu = CMP_EQ type, DEVICE_GPU;
        BR is_gpu, CALC_GPU_WEIGHT, CALC_WORK;
        
    CALC_GPU_WEIGHT:
        weight = LOAD_I32 15;
        
    CALC_WORK:
        // Calculate work proportion
        weighted_units = MUL compute_units, weight;
        work_ratio = DIV weighted_units, total_capability;
        work_amount = MUL total_work, work_ratio;
        
        // Ensure at least some work
        min_work = LOAD_I32 1;
        work_amount = CALL max_i32(work_amount, min_work);
        
        // Ensure we don't exceed total
        remaining = SUB total_work, work_start;
        work_amount = CALL min_i32(work_amount, remaining);
        
        // Set work range for this device
        CALL set_device_work_range(i, work_start, work_amount);
        
        // Log work distribution
        CALL log_work_distribution(i, work_start, work_amount);
        
        // Update start for next device
        work_start = ADD work_start, work_amount;
        
        // Next device
        i = ADD i, 1;
        BR ALWAYS, ALLOC_CHECK;
        
    DONE:
        RET;
}

// Execute image processing across multiple devices
FUNCTION execute_image_processing(input: ptr<image>, output: ptr<image>,
                               params: processing_params,
                               devices: ptr<device_info>,
                               device_count: i32) -> void {
    ENTRY:
        // Set up event for synchronization
        completion_event = CALL runtime_create_event();
        
        // Allocate memory across devices
        input_buffers = CALL allocate_device_buffers(input, devices, device_count);
        output_buffers = CALL allocate_device_buffers(output, devices, device_count);
        
        // Copy input data to device buffers
        CALL copy_data_to_devices(input, input_buffers, devices, device_count);
        
        // Launch kernels on each device
        i = LOAD_I32 0;
        BR ALWAYS, LAUNCH_CHECK;
        
    LAUNCH_CHECK:
        launch_done = CMP_GE i, device_count;
        BR launch_done, WAIT_COMPLETION, LAUNCH_DEVICE;
        
    LAUNCH_DEVICE:
        // Get device info
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        
        // Get device ID
        id_ptr = ADD device_ptr, OFFSETOF(device_info, id);
        device_id = LOAD [id_ptr];
        
        // Get device type
        type_ptr = ADD device_ptr, OFFSETOF(device_info, type);
        device_type = LOAD [type_ptr];
        
        // Get work range
        work_start = CALL get_device_work_start(i);
        work_amount = CALL get_device_work_amount(i);
        
        // Skip if no work assigned
        has_work = CMP_GT work_amount, 0;
        BR has_work, SELECT_KERNEL, NEXT_DEVICE;
        
    SELECT_KERNEL:
        // Select appropriate kernel based on device type
        is_gpu = CMP_EQ device_type, DEVICE_GPU;
        BR is_gpu, LAUNCH_GPU, LAUNCH_CPU;
        
    LAUNCH_GPU:
        // Get input and output buffers for this device
        input_buffer = input_buffers[i];
        output_buffer = output_buffers[i];
        
        // Launch GPU kernel
        CALL runtime_set_device(device_id);
        CALL launch_process_image_gpu(device_id, input_buffer, output_buffer, 
                                    input->width, input->height, input->channels,
                                    work_start, work_amount, params, completion_event);
        
        BR ALWAYS, NEXT_DEVICE;
        
    LAUNCH_CPU:
        // Get input and output buffers for this device
        input_buffer = input_buffers[i];
        output_buffer = output_buffers[i];
        
        // Launch CPU kernel
        CALL runtime_set_device(device_id);
        CALL launch_process_image_cpu(device_id, input_buffer, output_buffer,
                                    input->width, input->height, input->channels,
                                    work_start, work_amount, params, completion_event);
        
    NEXT_DEVICE:
        i = ADD i, 1;
        BR ALWAYS, LAUNCH_CHECK;
        
    WAIT_COMPLETION:
        // Wait for all devices to complete
        CALL runtime_wait_event(completion_event);
        
        // Copy results back from devices
        CALL copy_data_from_devices(output, output_buffers, devices, device_count);
        
        // Clean up device memory
        CALL free_device_buffers(input_buffers, device_count);
        CALL free_device_buffers(output_buffers, device_count);
        
        // Release event
        CALL runtime_release_event(completion_event);
        
        RET;
}

// GPU kernel for image processing (device-specific code)
FUNCTION process_image_gpu(input: ptr<u8>, output: ptr<u8>,
                         width: i32, height: i32, channels: i32,
                         start_pixel: i32, num_pixels: i32,
                         params: processing_params) -> void
TARGET "GPU" {
    ENTRY:
        // GPU-specific implementation
        // This would be optimized for GPU execution with vector operations
        
        // Get thread ID
        thread_id = CALL gpu_get_global_id(0);
        
        // Check if this thread has work to do
        valid_thread = CMP_LT thread_id, num_pixels;
        BR valid_thread, PROCESS_PIXEL, DONE;
        
    PROCESS_PIXEL:
        // Calculate actual pixel index
        pixel_index = ADD start_pixel, thread_id;
        
        // Check bounds
        total_pixels = MUL width, height;
        in_bounds = CMP_LT pixel_index, total_pixels;
        BR in_bounds, CALCULATE_OFFSET, DONE;
        
    CALCULATE_OFFSET:
        // Calculate byte offset
        byte_offset = MUL pixel_index, channels;
        
        // Process each channel
        c = LOAD_I32 0;
        BR ALWAYS, CHANNEL_CHECK;
        
    CHANNEL_CHECK:
        channel_done = CMP_GE c, channels;
        BR channel_done, DONE, PROCESS_CHANNEL;
        
    PROCESS_CHANNEL:
        // Get input value
        offset = ADD byte_offset, c;
        in_ptr = ADD input, offset;
        in_value = LOAD [in_ptr];
        
        // Convert to float [0-1]
        in_float = CONVERT in_value, f32;
        in_float = DIV in_float, 255.0;
        
        // Apply processing (simplified)
        // Brightness
        temp = MUL in_float, params.brightness;
        
        // Contrast
        temp = SUB temp, 0.5;
        temp = MUL temp, params.contrast;
        temp = ADD temp, 0.5;
        
        // Gamma correction
        temp = CALL gpu_pow(temp, params.gamma);
        
        // Clamp to [0-1]
        temp = CALL gpu_clamp(temp, 0.0, 1.0);
        
        // Convert back to byte
        temp = MUL temp, 255.0;
        out_value = CONVERT temp, u8;
        
        // Store output
        out_ptr = ADD output, offset;
        STORE [out_ptr], out_value;
        
        // Next channel
        c = ADD c, 1;
        BR ALWAYS, CHANNEL_CHECK;
        
    DONE:
        RET;
}

// CPU kernel for image processing
FUNCTION process_image_cpu(input: ptr<u8>, output: ptr<u8>,
                         width: i32, height: i32, channels: i32,
                         start_pixel: i32, num_pixels: i32,
                         params: processing_params) -> void
TARGET "CPU" {
    ENTRY:
        // CPU implementation with potential SIMD optimization
        end_pixel = ADD start_pixel, num_pixels;
        
        // Process pixels in chunks for better cache usage
        chunk_size = LOAD_I32 64;
        current = start_pixel;
        BR ALWAYS, CHUNK_CHECK;
        
    CHUNK_CHECK:
        chunk_done = CMP_GE current, end_pixel;
        BR chunk_done, DONE, PROCESS_CHUNK;
        
    PROCESS_CHUNK:
        // Calculate end of this chunk
        chunk_end = ADD current, chunk_size;
        chunk_end = CALL min_i32(chunk_end, end_pixel);
        
        // Process pixels in this chunk
        pixel = current;
        BR ALWAYS, PIXEL_CHECK;
        
    PIXEL_CHECK:
        pixel_done = CMP_GE pixel, chunk_end;
        BR pixel_done, NEXT_CHUNK, PROCESS_PIXEL;
        
    PROCESS_PIXEL:
        // Calculate byte offset
        byte_offset = MUL pixel, channels;
        
        // Process each channel
        c = LOAD_I32 0;
        BR ALWAYS, CHANNEL_CHECK;
        
    CHANNEL_CHECK:
        channel_done = CMP_GE c, channels;
        BR channel_done, NEXT_PIXEL, PROCESS_CHANNEL;
        
    PROCESS_CHANNEL:
        // Get input value
        offset = ADD byte_offset, c;
        in_ptr = ADD input, offset;
        in_value = LOAD [in_ptr];
        
        // Convert to float [0-1]
        in_float = CONVERT in_value, f32;
        in_float = DIV in_float, 255.0;
        
        // Apply processing (simplified)
        // Brightness
        temp = MUL in_float, params.brightness;
        
        // Contrast
        temp = SUB temp, 0.5;
        temp = MUL temp, params.contrast;
        temp = ADD temp, 0.5;
        
        // Gamma correction
        temp = CALL pow(temp, params.gamma);
        
        // Clamp to [0-1]
        temp = CALL clamp(temp, 0.0, 1.0);
        
        // Convert back to byte
        temp = MUL temp, 255.0;
        out_value = CONVERT temp, u8;
        
        // Store output
        out_ptr = ADD output, offset;
        STORE [out_ptr], out_value;
        
        // Next channel
        c = ADD c, 1;
        BR ALWAYS, CHANNEL_CHECK;
        
    NEXT_PIXEL:
        pixel = ADD pixel, 1;
        BR ALWAYS, PIXEL_CHECK;
        
    NEXT_CHUNK:
        current = chunk_end;
        BR ALWAYS, CHUNK_CHECK;
        
    DONE:
        RET;
}

// Helper functions for cross-device memory management
FUNCTION allocate_device_buffers(img: ptr<image>, devices: ptr<device_info>, device_count: i32) -> ptr<ptr<u8>> {
    ENTRY:
        // Calculate buffer size
        total_size = MUL img->width, img->height;
        total_size = MUL total_size, img->channels;
        
        // Allocate array of buffer pointers
        buffers_size = MUL device_count, SIZEOF(ptr<u8>);
        buffers = CALL malloc(buffers_size);
        
        // Allocate buffers on each device
        i = LOAD_I32 0;
        BR ALWAYS, ALLOC_CHECK;
        
    ALLOC_CHECK:
        alloc_done = CMP_GE i, device_count;
        BR alloc_done, ALLOC_DONE, ALLOC_DEVICE_BUFFER;
        
    ALLOC_DEVICE_BUFFER:
        // Get device ID
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        id_ptr = ADD device_ptr, OFFSETOF(device_info, id);
        device_id = LOAD [id_ptr];
        
        // Get work amount for this device
        work_amount = CALL get_device_work_amount(i);
        
        // Skip if no work assigned
        has_work = CMP_GT work_amount, 0;
        BR has_work, ALLOCATE_BUFFER, SET_NULL_BUFFER;
        
    ALLOCATE_BUFFER:
        // Set active device
        CALL runtime_set_device(device_id);
        
        // Allocate device memory
        buffer = CALL runtime_malloc_device(device_id, total_size);
        
        // Store buffer pointer
        buffer_ptr = ADD buffers, MUL i, SIZEOF(ptr<u8>);
        STORE [buffer_ptr], buffer;
        
        BR ALWAYS, NEXT_DEVICE;
        
    SET_NULL_BUFFER:
        // Store NULL for devices with no work
        buffer_ptr = ADD buffers, MUL i, SIZEOF(ptr<u8>);
        STORE [buffer_ptr], NULL;
        
    NEXT_DEVICE:
        i = ADD i, 1;
        BR ALWAYS, ALLOC_CHECK;
        
    ALLOC_DONE:
        RET buffers;
}

// Copy data to device buffers
FUNCTION copy_data_to_devices(img: ptr<image>, buffers: ptr<ptr<u8>>, devices: ptr<device_info>, device_count: i32) -> void {
    ENTRY:
        // Calculate total size
        total_size = MUL img->width, img->height;
        total_size = MUL total_size, img->channels;
        
        i = LOAD_I32 0;
        BR ALWAYS, COPY_CHECK;
        
    COPY_CHECK:
        copy_done = CMP_GE i, device_count;
        BR copy_done, COPY_DONE, CHECK_DEVICE;
        
    CHECK_DEVICE:
        // Get device buffer
        buffer_ptr = ADD buffers, MUL i, SIZEOF(ptr<u8>);
        buffer = LOAD [buffer_ptr];
        
        // Skip if NULL buffer (no work)
        is_null = CMP_EQ buffer, NULL;
        BR is_null, NEXT_DEVICE, COPY_TO_DEVICE;
        
    COPY_TO_DEVICE:
        // Get device ID
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        id_ptr = ADD device_ptr, OFFSETOF(device_info, id);
        device_id = LOAD [id_ptr];
        
        // Set active device
        CALL runtime_set_device(device_id);
        
        // Copy data from host to device
        CALL runtime_memcpy_to_device(device_id, buffer, img->data, total_size);
        
    NEXT_DEVICE:
        i = ADD i, 1;
        BR ALWAYS, COPY_CHECK;
        
    COPY_DONE:
        RET;
}

// Copy processed data from devices back to host
FUNCTION copy_data_from_devices(img: ptr<image>, buffers: ptr<ptr<u8>>, devices: ptr<device_info>, device_count: i32) -> void {
    ENTRY:
        // For each device with work
        i = LOAD_I32 0;
        BR ALWAYS, COPY_CHECK;
        
    COPY_CHECK:
        copy_done = CMP_GE i, device_count;
        BR copy_done, COPY_DONE, CHECK_DEVICE;
        
    CHECK_DEVICE:
        // Get device buffer
        buffer_ptr = ADD buffers, MUL i, SIZEOF(ptr<u8>);
        buffer = LOAD [buffer_ptr];
        
        // Skip if NULL buffer (no work)
        is_null = CMP_EQ buffer, NULL;
        BR is_null, NEXT_DEVICE, COPY_FROM_DEVICE;
        
    COPY_FROM_DEVICE:
        // Get device ID
        device_ptr = ADD devices, MUL i, SIZEOF(device_info);
        id_ptr = ADD device_ptr, OFFSETOF(device_info, id);
        device_id = LOAD [id_ptr];
        
        // Get work range
        work_start = CALL get_device_work_start(i);
        work_amount = CALL get_device_work_amount(i);
        
        // Set active device
        CALL runtime_set_device(device_id);
        
        // Calculate offsets and sizes
        channels = img->channels;
        start_offset = MUL work_start, channels;
        bytes_to_copy = MUL work_amount, channels;
        
        // Copy data from device to host (only the processed region)
        host_ptr = ADD img->data, start_offset;
        device_ptr = ADD buffer, start_offset;
        
        CALL runtime_memcpy_from_device(device_id, host_ptr, device_ptr, bytes_to_copy);
        
    NEXT_DEVICE:
        i = ADD i, 1;
        BR ALWAYS, COPY_CHECK;
        
    COPY_DONE:
        RET;
}

// Free device buffers
FUNCTION free_device_buffers(buffers: ptr<ptr<u8>>, device_count: i32) -> void {
    ENTRY:
        i = LOAD_I32 0;
        BR ALWAYS, FREE_CHECK;
        
    FREE_CHECK:
        free_done = CMP_GE i, device_count;
        BR free_done, FREE_ARRAY, FREE_BUFFER;
        
    FREE_BUFFER:
        // Get buffer pointer
        buffer_ptr = ADD buffers, MUL i, SIZEOF(ptr<u8>);
        buffer = LOAD [buffer_ptr];
        
        // Skip if NULL
        is_null = CMP_EQ buffer, NULL;
        BR is_null, NEXT_BUFFER, DO_FREE;
        
    DO_FREE:
        // Free device memory
        CALL runtime_free_device(buffer);
        
    NEXT_BUFFER:
        i = ADD i, 1;
        BR ALWAYS, FREE_CHECK;
        
    FREE_ARRAY:
        // Free array of pointers
        CALL free(buffers);
        
        RET;
}

// External functions from COIL runtime library
EXTERN FUNCTION runtime_init() -> i32;
EXTERN FUNCTION runtime_shutdown() -> void;
EXTERN FUNCTION runtime_get_device_count() -> i32;
EXTERN FUNCTION runtime_get_device_name(device_id: i32) -> ptr<i8>;
EXTERN FUNCTION runtime_get_device_type(device_id: i32) -> i32;
EXTERN FUNCTION runtime_get_device_memory(device_id: i32) -> i64;
EXTERN FUNCTION runtime_get_device_compute_units(device_id: i32) -> i32;
EXTERN FUNCTION runtime_set_device(device_id: i32) -> void;
EXTERN FUNCTION runtime_malloc_device(device_id: i32, size: i64) -> ptr<u8>;
EXTERN FUNCTION runtime_free_device(ptr: ptr<u8>) -> void;
EXTERN FUNCTION runtime_memcpy_to_device(device_id: i32, dst: ptr<u8>, src: ptr<u8>, size: i64) -> void;
EXTERN FUNCTION runtime_memcpy_from_device(device_id: i32, dst: ptr<u8>, src: ptr<u8>, size: i64) -> void;
EXTERN FUNCTION runtime_create_event() -> ptr<void>;
EXTERN FUNCTION runtime_release_event(event: ptr<void>) -> void;
EXTERN FUNCTION runtime_wait_event(event: ptr<void>) -> void;

// Launch kernels for specific devices
EXTERN FUNCTION launch_process_image_gpu(device_id: i32, input: ptr<u8>, output: ptr<u8>,
                                      width: i32, height: i32, channels: i32,
                                      start_pixel: i32, num_pixels: i32,
                                      params: processing_params, event: ptr<void>) -> void;

EXTERN FUNCTION launch_process_image_cpu(device_id: i32, input: ptr<u8>, output: ptr<u8>,
                                      width: i32, height: i32, channels: i32,
                                      start_pixel: i32, num_pixels: i32,
                                      params: processing_params, event: ptr<void>) -> void;

// Utility functions
EXTERN FUNCTION min_i32(a: i32, b: i32) -> i32;
EXTERN FUNCTION max_i32(a: i32, b: i32) -> i32;
EXTERN FUNCTION clamp(x: f32, min: f32, max: f32) -> f32;
EXTERN FUNCTION pow(x: f32, y: f32) -> f32;

// GPU-specific variants
EXTERN FUNCTION gpu_clamp(x: f32, min: f32, max: f32) -> f32 TARGET "GPU";
EXTERN FUNCTION gpu_pow(x: f32, y: f32) -> f32 TARGET "GPU";
EXTERN FUNCTION gpu_get_global_id(dim: i32) -> i32 TARGET "GPU";

// Work distribution tracking
EXTERN FUNCTION set_device_count(count: i32) -> void;
EXTERN FUNCTION get_device_count() -> i32;
EXTERN FUNCTION set_device_work_range(device_idx: i32, start: i32, amount: i32) -> void;
EXTERN FUNCTION get_device_work_start(device_idx: i32) -> i32;
EXTERN FUNCTION get_device_work_amount(device_idx: i32) -> i32;

// I/O functions
EXTERN FUNCTION load_image(path: ptr<i8>) -> ptr<image>;
EXTERN FUNCTION save_image(img: ptr<image>, path: ptr<i8>) -> i32;
EXTERN FUNCTION create_image(width: i32, height: i32, channels: i32) -> ptr<image>;
EXTERN FUNCTION free_image(img: ptr<image>) -> void;

// Utility functions
EXTERN FUNCTION check_arguments(argc: i32, argv: ptr<ptr<i8>>) -> i32;
EXTERN FUNCTION print_usage(program_name: ptr<i8>) -> void;
EXTERN FUNCTION print_error(message: ptr<i8>) -> void;
EXTERN FUNCTION print_device_info(device: ptr<device_info>) -> void;
EXTERN FUNCTION log_work_distribution(device_idx: i32, start: i32, amount: i32) -> void;
EXTERN FUNCTION cleanup_devices(devices: ptr<device_info>, device_count: i32) -> void;
```

## 2. System Architecture

The example implements a heterogeneous image processing pipeline with these key components:

1. **Device Discovery**: Identifies and queries all available processing units (CPUs, GPUs)
2. **Work Distribution**: Intelligently divides work based on device capabilities
3. **Memory Management**: Handles data transfer between host and devices
4. **Parallel Execution**: Processes image portions concurrently across devices
5. **Synchronization**: Ensures all devices complete their work before final assembly

### Device Discovery and Selection

The system enumerates all available devices and collects their capabilities:

```
// Discover available devices
FUNCTION discover_devices() -> ptr<device_info> {
    // Query number of devices
    device_count = CALL runtime_get_device_count();
    
    // For each device, collect:
    // - Device type (CPU/GPU/Other)
    // - Memory capacity
    // - Compute units
    // - Other capabilities
    ...
}
```

### Work Distribution

Work is distributed proportionally to device capabilities, with GPU compute units weighted more heavily for image processing:

```
// Prepare execution plan based on device capabilities
FUNCTION prepare_execution_plan(total_work: i32, devices: ptr<device_info>, device_count: i32) -> void {
    // Calculate total compute capability
    // For GPUs, apply higher weight (~15x per compute unit for image processing)
    ...
    
    // Allocate work proportionally
    // Ensure minimum work per device and respect total bounds
    ...
}
```

### Memory Management

The system handles data movement between host and devices:

```
// Allocate memory on each device
FUNCTION allocate_device_buffers(img: ptr<image>, devices: ptr<device_info>, device_count: i32) -> ptr<ptr<u8>> {
    ...
}

// Copy data to devices
FUNCTION copy_data_to_devices(img: ptr<image>, buffers: ptr<ptr<u8>>, devices: ptr<device_info>, device_count: i32) -> void {
    ...
}

// Copy results back from devices
FUNCTION copy_data_from_devices(img: ptr<image>, buffers: ptr<ptr<u8>>, devices: ptr<device_info>, device_count: i32) -> void {
    ...
}
```

### Parallel Execution

The system launches kernels on all available devices:

```
// Execute image processing across multiple devices
FUNCTION execute_image_processing(input: ptr<image>, output: ptr<image>,
                               params: processing_params,
                               devices: ptr<device_info>,
                               device_count: i32) -> void {
    // For each device with work assigned:
    //   1. Select appropriate kernel (CPU or GPU variant)
    //   2. Launch kernel with device-specific parameters
    //   3. Record in completion event
    ...
}
```

### Target-Specific Implementation

Device-specific code is implemented using COIL's target specification:

```
// GPU kernel implementation
FUNCTION process_image_gpu(...) -> void
TARGET "GPU" {
    // GPU-optimized implementation
    // Uses GPU intrinsics and memory model
    ...
}

// CPU kernel implementation
FUNCTION process_image_cpu(...) -> void
TARGET "CPU" {
    // CPU-optimized implementation
    // Leverages cache-friendly access patterns
    ...
}
```

## 3. Runtime Integration

This example demonstrates how COIL's runtime extensions enable cross-device computing:

1. **Runtime Initialization**: Sets up the COIL runtime environment
2. **Device Management**: Queries and configures multiple processing units
3. **Memory Transfer**: Moves data between host and device memory spaces
4. **Kernel Invocation**: Launches compute kernels on appropriate devices
5. **Synchronization**: Coordinates execution across heterogeneous devices

The COIL runtime provides these capabilities through a set of extension functions:

```
// Runtime initialization
EXTERN FUNCTION runtime_init() -> i32;
EXTERN FUNCTION runtime_shutdown() -> void;

// Device management
EXTERN FUNCTION runtime_get_device_count() -> i32;
EXTERN FUNCTION runtime_get_device_type(device_id: i32) -> i32;
EXTERN FUNCTION runtime_set_device(device_id: i32) -> void;

// Memory management
EXTERN FUNCTION runtime_malloc_device(device_id: i32, size: i64) -> ptr<u8>;
EXTERN FUNCTION runtime_memcpy_to_device(device_id: i32, dst: ptr<u8>, src: ptr<u8>, size: i64) -> void;
EXTERN FUNCTION runtime_memcpy_from_device(device_id: i32, dst: ptr<u8>, src: ptr<u8>, size: i64) -> void;

// Synchronization
EXTERN FUNCTION runtime_create_event() -> ptr<void>;
EXTERN FUNCTION runtime_wait_event(event: ptr<void>) -> void;
```

## 4. Conclusion

This cross-device computing example demonstrates several key capabilities of COIL and its runtime extensions:

1. **Device Abstraction**: Works across heterogeneous processing units
2. **Intelligent Work Distribution**: Allocates work based on device capabilities
3. **Memory Management**: Efficient data transfer between memory spaces
4. **Target-Specific Optimization**: Device-specific implementations of key algorithms
5. **Synchronization**: Coordination of parallel execution across devices

The code showcases how COIL enables developers to write portable, efficient code that leverages all available computational resources, from CPUs to GPUs and other specialized hardware.