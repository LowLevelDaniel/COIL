# COIL Target Switching System

## Introduction

One of COIL's most powerful and unique features is its target switching capability. This system allows developers to create code that can seamlessly transition between different hardware architectures, instruction sets, and execution environments within a single codebase. The target switching system is what enables COIL to excel at heterogeneous computing, polyglot binaries, and mode transitions in systems programming.

## Design Philosophy

The target switching system is designed with several key principles in mind:

1. **Transparent Transitions**: Make architecture transitions as seamless as possible for the developer.
2. **Explicit Control**: Give programmers explicit control over when and how target switching occurs.
3. **Flexible Integration**: Allow for flexible integration with existing code and tools for each target.
4. **Performance-Focused**: Minimize overhead for cross-target calls and data transfers.
5. **Static Analysis Friendly**: Enable comprehensive static analysis across architecture boundaries.

## Target Configuration

Each target in COIL represents a specific hardware architecture, instruction set, or execution environment. Targets are defined in the configuration file and specified in the Configuration Table of the COIL binary format.

### Target Definition Structure

A target definition includes:

1. **Target ID**: A unique identifier for the target.
2. **Architecture**: The hardware architecture or instruction set (e.g., x86_64, ARM64, CUDA).
3. **Word Size**: The natural word size for the architecture (typically 32 or 64 bits).
4. **Endianness**: The byte ordering (little or big endian).
5. **Register Set**: The available registers and their properties.
6. **ABI Definitions**: The supported Application Binary Interfaces.
7. **Special Features**: Architecture-specific features and instructions.
8. **Constraints**: Limitations or requirements for code targeting this architecture.

### Example Target Definitions

In the configuration file, targets might be defined as follows:

```
target x86_64 {
    word_size = 64
    endian = little
    registers = {
        general = [RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10, R11, R12, R13, R14, R15]
        float = [XMM0, XMM1, ..., XMM15]
        vector = [YMM0, YMM1, ..., YMM15]
        virtual = {
            R1 = RAX
            R2 = RBX
            // ...
            F1 = XMM0
            // ...
            V1 = YMM0
            // ...
        }
    }
    abi_default = x86_64_sysv
    abi_supported = [x86_64_sysv, win64]
    features = [SSE4.2, AVX2, FMA, BMI2]
}

target arm64 {
    word_size = 64
    endian = little
    registers = {
        general = [X0, X1, ..., X30, SP, XZR]
        float = [V0, V1, ..., V31]
        virtual = {
            R1 = X0
            R2 = X1
            // ...
            F1 = V0
            // ...
            V1 = V0
            // ...
        }
    }
    abi_default = aapcs64
    abi_supported = [aapcs64]
    features = [NEON, CRC]
}

target cuda {
    word_size = 64
    endian = little
    registers = {
        general = [R0, R1, ..., R255]
        vector = [V0, V1, ..., V255]
        special = [ThreadIdx, BlockIdx, BlockDim, GridDim]
        virtual = {
            R1 = R0
            // ...
            V1 = V0
            // ...
        }
    }
    abi_default = cuda_device
    abi_supported = [cuda_device]
    features = [PTX6.0, SM7.5]
    constraints = {
        max_threads_per_block = 1024
        max_blocks_per_grid = [2147483647, 65535, 65535]
    }
}
```

## Target Switching Directives

COIL provides directives for switching between targets during assembly:

### Target Directive

The most common way to switch targets is using the target directive:

```
.target <target_id>
    ; Code for the specified target
.target default  ; or another target ID to switch back
```

For example, in COIL:

```
.section .text exec, read
process_data:
    ; CPU code
    push r0, r1          ; Save parameters
    mov r0, r1           ; r0 = size
    mul r0, r0, 4        ; r0 = size * sizeof(f32)
    bl memory.allocate   ; Call allocation function
    mov r4, r0           ; r4 = result pointer
    
    ; Switch to GPU for computation
    .target GPU
    mov r0, 256          ; block_size = 256
    mov r1, r2           ; r1 = size
    add r1, r0, -1       ; r1 = size + block_size - 1
    div r1, r1, r0       ; r1 = (size + block_size - 1) / block_size = grid_size
    
    ; Launch GPU kernel
    pop r2, r3           ; Restore original parameters
    push r4              ; Save result pointer
    bl kernel_launch     ; Call kernel launch function
    
    ; Back to CPU code
    .target x86_64       ; Return to CPU target
    pop r0               ; r0 = result pointer
    pop r1, r2           ; Restore original parameters
    push r0, r1          ; Save for memory.free
    bl display_results   ; Call display function
    pop r0, r1           ; Prepare for memory.free
    bl memory.free       ; Free allocated memory
    ret
```

### HOIL Representation

For documentation purposes, HOIL provides a higher-level representation:

```
function process_data(data: ptr<f32>, size: i32) -> void {
    // CPU code
    var result : ptr<f32> = memory.allocate(size * sizeof(f32))
    
    // Switch to GPU for computation
    target(GPU) {
        var block_size : i32 = 256
        var grid_size : i32 = (size + block_size - 1) / block_size
        
        // GPU-specific code
        kernel_launch(grid_size, block_size, data, result, size)
    }
    
    // Back to CPU code
    display_results(result, size)
    memory.free(result)
}
```

This makes the concept easier to understand, but actual COIL code uses the target directive.

### Target-Specific Functions

Functions can be declared for a specific target:

```
function <name>(<params>) -> <return_type> target(<target_id>) {
    // Target-specific implementation
}
```

For example:

```
function vector_add(a: ptr<f32>, b: ptr<f32>, c: ptr<f32>, n: i32) -> void target(GPU) {
    var idx : i32 = get_global_id(0)
    if (idx < n) {
        c[idx] = a[idx] + b[idx]
    }
}
```

### Inline Target Switch

For simple operations, an inline target switch can be used:

```
var result : i32 = target(ARM64) expression
```

For example:

```
var crc : u32 = target(ARM64) intrinsic.crc32(data, size)
```

## Cross-Target Data Handling

When switching between targets, data often needs to be transferred between different memory spaces. COIL provides several mechanisms for handling this:

### Automatic Data Transfer

For simple cases, COIL can automatically transfer data when crossing target boundaries:

```
function process_on_gpu(data: ptr<f32>, size: i32) -> f32 target(GPU) {
    // This function runs on the GPU, but is called from CPU code
    // COIL automatically transfers 'data' to GPU memory
    var sum : f32 = 0.0
    var idx : i32 = get_global_id(0)
    if (idx < size) {
        sum += data[idx]
    }
    return sum
}

function main() -> i32 {
    var host_data : [1024]f32
    // Initialize data...
    
    // Automatic data transfer when crossing target boundary
    var result : f32 = call process_on_gpu(host_data, 1024)
    
    return 0
}
```

### Explicit Data Transfer

For more control, COIL provides explicit data transfer functions:

```
// Allocate memory on a specific target
function memory.allocate_on(size: usize, target_id: i32) -> ptr<void>

// Transfer data between targets
function memory.transfer(dest: ptr<void>, dest_target: i32, src: ptr<void>, src_target: i32, size: usize) -> void
```

Example usage:

```
function process_large_array(host_data: ptr<f32>, size: i32) -> void {
    // Allocate memory on GPU
    var gpu_data : ptr<f32> = (ptr<f32>)memory.allocate_on(size * sizeof(f32), TARGET_GPU)
    var gpu_result : ptr<f32> = (ptr<f32>)memory.allocate_on(size * sizeof(f32), TARGET_GPU)
    
    // Explicitly transfer data to GPU
    memory.transfer(gpu_data, TARGET_GPU, host_data, TARGET_CPU, size * sizeof(f32))
    
    // Run GPU computation
    target(GPU) {
        var block_size : i32 = 256
        var grid_size : i32 = (size + block_size - 1) / block_size
        kernel_launch(grid_size, block_size, gpu_data, gpu_result, size)
    }
    
    // Allocate result buffer on host
    var host_result : ptr<f32> = (ptr<f32>)memory.allocate(size * sizeof(f32))
    
    // Transfer results back to host
    memory.transfer(host_result, TARGET_CPU, gpu_result, TARGET_GPU, size * sizeof(f32))
    
    // Process results on host...
    
    // Clean up
    memory.free(host_result)
    memory.free_on(gpu_data, TARGET_GPU)
    memory.free_on(gpu_result, TARGET_GPU)
}
```

### Shared Memory Spaces

For some target combinations, memory can be shared directly:

```
function process_with_shared_memory() -> void {
    // Allocate memory accessible from both CPU and GPU
    var shared_data : ptr<f32> = (ptr<f32>)memory.allocate_shared(1024 * sizeof(f32), TARGET_CPU | TARGET_GPU)
    
    // Initialize on CPU
    var i : i32 = 0
    while (i < 1024) {
        shared_data[i] = (f32)i
        i = i + 1
    }
    
    // Process on GPU with direct access to the same memory
    target(GPU) {
        var block_size : i32 = 256
        var grid_size : i32 = (1024 + block_size - 1) / block_size
        kernel_launch(grid_size, block_size, shared_data, 1024)
    }
    
    // CPU can now directly access results without transfer
    var sum : f32 = 0.0
    i = 0
    while (i < 1024) {
        sum += shared_data[i]
        i = i + 1
    }
    
    // Clean up
    memory.free_shared(shared_data, TARGET_CPU | TARGET_GPU)
}
```

## Target Switching Implementation

The COIL assembler implements target switching through several mechanisms:

### Function-Level Switching

When a function is declared for a specific target, the assembler generates:

1. The function implementation using the target's instruction set
2. Appropriate trampolines for cross-target calls
3. Metadata to identify the target-specific nature of the function

### Block-Level Switching

For target blocks within a function, the assembler:

1. Generates code for the main target up to the block
2. Switches to generating code for the specified target for the block
3. Inserts appropriate transition code at block boundaries
4. Switches back to the main target after the block

### Data Transfer

For data transfers, the assembler:

1. Analyzes data usage across target boundaries
2. Inserts appropriate data transfer operations
3. Optimizes transfers to minimize overhead (coalescing, pipelining, etc.)

## Use Cases

The target switching capability enables several important use cases:

### Heterogeneous Computing

COIL's target switching allows seamless integration of different computing resources:

```
function process_image(image: ptr<u8>, width: i32, height: i32) -> void {
    // Preprocessing on CPU
    convert_format(image, width, height)
    
    // Main processing on GPU
    target(GPU) {
        var block_size : [2]i32 = [16, 16]
        var grid_size : [2]i32 = [(width + 15) / 16, (height + 15) / 16]
        apply_filters_kernel(grid_size, block_size, image, width, height)
    }
    
    // Additional processing on CPU
    post_process(image, width, height)
    
    // Special-case processing on DSP for audio components
    target(DSP) {
        extract_and_process_audio(image, width, height)
    }
}
```

### Polyglot Binaries

COIL can generate binaries that run on multiple architectures:

```
function main() -> i32 {
    // Code common to all architectures
    init_system()
    
    // Architecture-specific optimizations
    target(x86_64) {
        // x86_64 optimized implementation
        process_data_avx512(data, size)
    }
    
    target(ARM64) {
        // ARM64 optimized implementation
        process_data_neon(data, size)
    }
    
    target(RISCV) {
        // RISC-V optimized implementation
        process_data_rvv(data, size)
    }
    
    return 0
}
```

### Mode Transitions

COIL can handle transitions between different processor modes:

```
function boot_sequence() -> void target(x86_16) {
    // 16-bit real mode code (bootloader)
    setup_basic_system()
    
    // Transition to 32-bit protected mode
    enable_a20()
    load_gdt()
    
    target(x86_32) {
        // 32-bit protected mode code
        setup_paging()
        
        target(x86_64) {
            // 64-bit long mode code
            init_kernel()
        }
    }
}
```

## Example: Complete GPU Computation

Here's a comprehensive example demonstrating COIL's target switching for GPU computation:

```
// Define data structure
struct Particle {
    position : [3]f32
    velocity : [3]f32
    mass : f32
    force : [3]f32
}

// GPU kernel for particle simulation
function update_particles(particles: ptr<Particle>, num_particles: i32, dt: f32) -> void target(GPU) {
    var idx : i32 = get_global_id(0)
    if (idx >= num_particles) {
        return
    }
    
    // Update position based on velocity
    particles[idx].position[0] += particles[idx].velocity[0] * dt
    particles[idx].position[1] += particles[idx].velocity[1] * dt
    particles[idx].position[2] += particles[idx].velocity[2] * dt
    
    // Update velocity based on force
    var inv_mass : f32 = 1.0 / particles[idx].mass
    particles[idx].velocity[0] += particles[idx].force[0] * inv_mass * dt
    particles[idx].velocity[1] += particles[idx].force[1] * inv_mass * dt
    particles[idx].velocity[2] += particles[idx].force[2] * inv_mass * dt
    
    // Reset force for next iteration
    particles[idx].force[0] = 0.0
    particles[idx].force[1] = 0.0
    particles[idx].force[2] = 0.0
}

// GPU kernel for computing forces between particles
function compute_forces(particles: ptr<Particle>, num_particles: i32, g: f32) -> void target(GPU) {
    var idx : i32 = get_global_id(0)
    if (idx >= num_particles) {
        return
    }
    
    var pos_i : [3]f32 = particles[idx].position
    var mass_i : f32 = particles[idx].mass
    
    var force : [3]f32 = [0.0, 0.0, 0.0]
    
    // Compute gravitational force with all other particles
    var j : i32 = 0
    while (j < num_particles) {
        if (j != idx) {
            var pos_j : [3]f32 = particles[j].position
            var mass_j : f32 = particles[j].mass
            
            var dx : f32 = pos_j[0] - pos_i[0]
            var dy : f32 = pos_j[1] - pos_i[1]
            var dz : f32 = pos_j[2] - pos_i[2]
            
            var dist_sqr : f32 = dx*dx + dy*dy + dz*dz + 0.0001  // Softening factor
            var dist : f32 = sqrt(dist_sqr)
            var dist_cubed : f32 = dist_sqr * dist
            
            var f : f32 = g * mass_i * mass_j / dist_cubed
            
            force[0] += f * dx
            force[1] += f * dy
            force[2] += f * dz
        }
        j = j + 1
    }
    
    // Apply computed force
    particles[idx].force[0] += force[0]
    particles[idx].force[1] += force[1]
    particles[idx].force[2] += force[2]
}

// Main simulation function running on CPU
function simulate_particles(num_particles: i32, num_steps: i32, dt: f32) -> void {
    // Allocate memory for particles on host
    var host_particles : ptr<Particle> = (ptr<Particle>)memory.allocate(num_particles * sizeof(Particle))
    
    // Initialize particles
    initialize_random_particles(host_particles, num_particles)
    
    // Allocate memory on GPU
    var dev_particles : ptr<Particle> = (ptr<Particle>)memory.allocate_on(num_particles * sizeof(Particle), TARGET_GPU)
    
    // Copy initial data to GPU
    memory.transfer(dev_particles, TARGET_GPU, host_particles, TARGET_CPU, num_particles * sizeof(Particle))
    
    // Simulation loop
    var step : i32 = 0
    while (step < num_steps) {
        // Run GPU computation
        target(GPU) {
            var block_size : i32 = 256
            var grid_size : i32 = (num_particles + block_size - 1) / block_size
            
            // Compute forces between particles
            call compute_forces(dev_particles, num_particles, 6.67430e-11) grid(grid_size) block(block_size)
            
            // Update particle positions and velocities
            call update_particles(dev_particles, num_particles, dt) grid(grid_size) block(block_size)
        }
        
        step = step + 1
        
        // Every 100 steps, copy data back to host for visualization
        if (step % 100 == 0) {
            memory.transfer(host_particles, TARGET_CPU, dev_particles, TARGET_GPU, num_particles * sizeof(Particle))
            visualize_particles(host_particles, num_particles, step)
        }
    }
    
    // Final transfer back to host
    memory.transfer(host_particles, TARGET_CPU, dev_particles, TARGET_GPU, num_particles * sizeof(Particle))
    
    // Save results
    save_particle_state(host_particles, num_particles)
    
    // Clean up
    memory.free(host_particles)
    memory.free_on(dev_particles, TARGET_GPU)
}
```

## Example: Bootloader with Mode Transitions

Here's an example demonstrating COIL's target switching for an x86 bootloader:

```
// Entry point in 16-bit real mode
function bootloader_entry() -> void target(x86_16) {
    // Set up segment registers
    asm {
        xor ax, ax
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, 0x7C00
    }
    
    // Print welcome message
    call io.puts("COIL Bootloader starting...\n")
    
    // Load kernel from disk
    load_kernel()
    
    // Prepare for protected mode
    disable_interrupts()
    load_gdt()
    enable_a20()
    
    // Switch to protected mode
    asm {
        mov eax, cr0
        or al, 1
        mov cr0, eax
        
        // Far jump to flush instruction pipeline
        jmp 0x08:protected_mode_entry
    }
    
    // This point is never reached
}

// Protected mode entry (32-bit)
function protected_mode_entry() -> void target(x86_32) {
    // Set up segment registers for protected mode
    asm {
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov esp, 0x90000
    }
    
    // Print status message (now using VGA text mode)
    call io.puts("Entered 32-bit protected mode\n")
    
    // Set up paging
    setup_paging()
    
    // Switch to long mode
    asm {
        mov eax, cr4
        or eax, 1 << 5  // PAE
        mov cr4, eax
        
        mov ecx, 0xC0000080  // EFER MSR
        rdmsr
        or eax, 1 << 8       // LME
        wrmsr
        
        mov eax, cr0
        or eax, 1 << 31      // PG
        mov cr0, eax
        
        // Far jump to 64-bit code
        jmp 0x18:long_mode_entry
    }
    
    // This point is never reached
}

// Long mode entry (64-bit)
function long_mode_entry() -> void target(x86_64) {
    // Set up segment registers for long mode
    asm {
        mov ax, 0x20
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov rsp, 0x90000
    }
    
    // Print status message
    call io.puts("Entered 64-bit long mode\n")
    
    // Jump to kernel
    call kernel_main()
    
    // Should never return, but if it does, halt
    while (1) {
        asm {
            hlt
        }
    }
}

// Kernel main function
function kernel_main() -> void target(x86_64) {
    // Initialize kernel subsystems
    init_memory()
    init_interrupts()
    init_scheduler()
    
    // Start first process
    start_init_process()
    
    // Enable interrupts and enter idle loop
    enable_interrupts()
    while (1) {
        asm {
            hlt
        }
    }
}
```

## Conclusion

COIL's target switching system provides a powerful mechanism for creating code that can seamlessly transition between different hardware architectures, instruction sets, and execution environments. This capability enables developers to write highly optimized code for heterogeneous computing systems, create polyglot binaries that run efficiently on multiple architectures, and handle mode transitions in systems programming.

By making architecture transitions explicit yet straightforward, COIL gives programmers fine-grained control over where and how their code executes, while abstracting away much of the complexity associated with cross-architecture development. This balance of control and abstraction is key to COIL's value proposition as a low-level intermediate language for modern computing systems.