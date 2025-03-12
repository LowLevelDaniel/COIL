# COIL Processing Units Specification

## 1. Introduction

The COIL Processing Units specification defines the abstract model of computational devices targeted by COIL. This document describes the classification, capabilities, and characteristics of various processing units, providing compiler developers with the information needed to effectively target diverse hardware architectures. Importantly, COIL's core functionality operates on single devices without requiring runtime support, while multi-device targeting is provided as an optional extension for more complex environments.

## 2. Processing Unit Model

### 2.1 Abstract Processing Unit (APU)

COIL defines an Abstract Processing Unit (APU) as a logical computational device with specific capabilities:

```c
/**
 * @struct coil_processing_unit
 * @brief Abstract processing unit definition
 */
typedef struct {
  uint32_t pu_class;         /* Processing unit class */
  uint32_t architecture;     /* Architecture ID */
  uint32_t version;          /* Architecture version */
  uint32_t capability_mask;  /* Capability flags */
  uint32_t memory_model;     /* Memory model identifier */
  uint32_t compute_model;    /* Compute model identifier */
  uint32_t features;         /* Feature bitfield */
  uint32_t lanes;            /* SIMD lane count (if applicable) */
  uint32_t max_threads;      /* Maximum thread count (if applicable) */
  uint32_t local_memory;     /* Local memory size in bytes */
  uint32_t shared_memory;    /* Shared memory size in bytes */
  uint32_t constant_memory;  /* Constant memory size in bytes */
  uint32_t registers;        /* Register count */
  uint32_t max_clock;        /* Maximum clock frequency in MHz */
  uint32_t energy_profile;   /* Energy efficiency profile */
  uint32_t extensions;       /* Extension support bitfield */
} coil_processing_unit_t;
```

### 2.2 Core vs. Extended Functionality

COIL distinguishes between core functionality and optional extensions:

| Aspect | Core Functionality | Optional Extensions |
|--------|-------------------|---------------------|
| Target | Single processing unit | Multiple processing units |
| Runtime | No runtime system required | Optional runtime for multi-device |
| Environment | Bare-metal, bootloaders, UEFI | OS-hosted environments |
| Memory Model | Direct memory access | Optional memory abstraction |
| Compilation | Ahead-of-time compilation | Additional just-in-time options |
| Synchronization | Simple synchronization primitives | Advanced multi-device synchronization |

## 3. Processing Unit Classification

### 3.1 Primary Processing Unit Classes

| Class ID | Name | Description |
|----------|------|-------------|
| 0x0000 | GENERIC | Generic fallback processing unit |
| 0x0001 | CPU_SCALAR | Traditional scalar CPU |
| 0x0002 | CPU_VECTOR | CPU with vector extensions |
| 0x0003 | CPU_MATRIX | CPU with matrix/tensor extensions |
| 0x0010 | GPU_GRAPHICS | Graphics-focused GPU |
| 0x0011 | GPU_COMPUTE | Compute-focused GPU |
| 0x0012 | GPU_UNIFIED | Unified graphics/compute GPU |
| 0x0020 | TPU | Tensor Processing Unit |
| 0x0021 | NPU | Neural Processing Unit |
| 0x0030 | DSP | Digital Signal Processor |
| 0x0040 | FPGA | Field-Programmable Gate Array |
| 0x0050 | QPU | Quantum Processing Unit |
| 0x0060 | SPU | Security Processing Unit |
| 0x0070 | APU | Audio Processing Unit |
| 0x0080 | VPU | Vision Processing Unit |
| 0x0090 | XPU | Media Processing Unit |
| 0x00A0 | IPU | Intelligence Processing Unit |
| 0x00B0 | HPU | Holographic Processing Unit |
| 0xF000-0xFFFF | CUSTOM | Vendor-specific custom processing units |

### 3.2 Architecture Identifiers

Architecture identifiers specify the instruction set architecture (ISA) implemented by a processing unit:

| Range | Category | Examples |
|-------|----------|----------|
| 0x0000-0x00FF | x86 Family | x86, x86_64, AVX-512, etc. |
| 0x0100-0x01FF | ARM Family | ARMv7, ARMv8, ARMv9, SVE, etc. |
| 0x0200-0x02FF | RISC-V Family | RV32, RV64, RV128, vector extensions |
| 0x0300-0x03FF | POWER Family | POWER9, POWER10, etc. |
| 0x0400-0x04FF | MIPS Family | MIPS64, etc. |
| 0x0500-0x05FF | GPU ISA | NVIDIA PTX, AMD GCN/RDNA, Intel Xe, etc. |
| 0x0600-0x06FF | ML Accelerators | TPU, NVDLA, etc. |
| 0x0700-0x07FF | DSP ISA | Hexagon, TI C6x, etc. |
| 0x0800-0x08FF | FPGA | Xilinx, Intel, etc. |
| 0x0900-0x09FF | Quantum | Various quantum architectures |
| 0x0A00-0x0AFF | WebAssembly | WASM, WASI |
| 0x0B00-0x0BFF | JVM | Java bytecode |
| 0x0C00-0x0CFF | .NET | CIL bytecode |
| 0xF000-0xFFFF | Custom | Custom/proprietary architectures |

## 4. Capability Masks

Capability masks define the operations and features supported by a processing unit:

### 4.1 Basic Capability Flags

| Bit | Capability | Description |
|-----|------------|-------------|
| 0 | INTEGER | Integer arithmetic support |
| 1 | FLOAT32 | 32-bit floating-point support |
| 2 | FLOAT64 | 64-bit floating-point support |
| 3 | SIMD | SIMD vector operations |
| 4 | ATOMIC | Atomic operations |
| 5 | THREADS | Multi-threading support |
| 6 | BARRIER | Thread barrier synchronization |
| 7 | LOCAL_MEM | Local memory support |
| 8 | SHARED_MEM | Shared memory support |
| 9 | CONSTANT_MEM | Constant memory support |
| 10 | TEXTURE | Texture memory and operations |
| 11 | GATHER_SCATTER | Gather/scatter memory operations |
| 12 | REDUCTION | Reduction operations |
| 13 | DOUBLE_PRECISION | Double-precision arithmetic |
| 14 | HALF_PRECISION | Half-precision arithmetic |
| 15 | FMA | Fused multiply-add |
| 16 | TRANSCENDENTAL | Transcendental functions (sin, cos, exp, etc.) |
| 17 | BIT_MANIPULATION | Bit manipulation operations |
| 18 | POPCNT | Population count |
| 19 | PREFETCH | Memory prefetch support |
| 20 | UNALIGNED | Unaligned memory access |
| 21 | PREDICATION | Predicated execution |
| 22 | INDIRECT_BRANCH | Indirect branches |
| 23 | EXCEPTIONS | Exception handling |
| 24-31 | RESERVED | Reserved for future use |

### 4.2 Advanced Capability Flags

| Bit | Capability | Description |
|-----|------------|-------------|
| 32 | MATRIX_MUL | Matrix multiplication |
| 33 | TENSOR_OPS | Tensor operations |
| 34 | SPARSE | Sparse computation |
| 35 | QUANTIZED | Quantized arithmetic |
| 36 | RAY_TRACING | Ray tracing acceleration |
| 37 | RASTERIZATION | Rasterization acceleration |
| 38 | SHADING | Programmable shading |
| 39 | NEURAL_NETWORK | Neural network acceleration |
| 40 | ENCRYPTION | Hardware encryption |
| 41 | HASHING | Hardware hash functions |
| 42 | RANDOM | Hardware random number generation |
| 43 | COMPRESSION | Hardware compression/decompression |
| 44 | SIGNAL_PROCESSING | Signal processing acceleration |
| 45 | VIDEO_CODING | Video encoding/decoding |
| 46 | IMAGE_PROCESSING | Image processing acceleration |
| 47 | AUDIO_PROCESSING | Audio processing acceleration |
| 48 | QUANTUM_GATE | Quantum gate operations |
| 49 | QUANTUM_MEASURE | Quantum measurement operations |
| 50 | RECONFIGURABLE | Runtime reconfigurability |
| 51 | DYNAMIC_PARALLELISM | Dynamic parallelism |
| 52 | VIRTUAL_MEMORY | Virtual memory support |
| 53 | ERROR_CORRECTION | Error detection and correction |
| 54 | POWER_MANAGEMENT | Power management capabilities |
| 55-63 | RESERVED | Reserved for future use |

## 5. Compute Models

COIL defines several compute models that describe how processing units execute code:

### 5.1 Compute Model Types

| ID | Model | Description |
|----|-------|-------------|
| 0x0 | SCALAR | Sequential scalar execution |
| 0x1 | SUPERSCALAR | Out-of-order superscalar execution |
| 0x2 | VECTOR | Vector processing |
| 0x3 | SIMD | Single Instruction Multiple Data |
| 0x4 | SIMT | Single Instruction Multiple Threads |
| 0x5 | MIMD | Multiple Instruction Multiple Data |
| 0x6 | DATAFLOW | Data-driven execution |
| 0x7 | STREAMING | Stream processing |
| 0x8 | SYSTOLIC | Systolic array processing |
| 0x9 | NEURAL | Neural network processing |
| 0xA | QUANTUM | Quantum circuit processing |
| 0xB | ASYNCHRONOUS | Asynchronous event-driven processing |
| 0xC | PIPELINE | Pipeline processing |

### 5.2 Compute Model Attributes

```c
/**
 * @struct coil_compute_model_attrs
 * @brief Attributes of a compute model
 */
typedef struct {
  uint32_t model_id;          /* Compute model identifier */
  uint32_t parallelism;       /* Degree of parallelism */
  uint32_t granularity;       /* Execution granularity */
  uint32_t branching;         /* Branching model */
  uint32_t synchronization;   /* Synchronization model */
  uint32_t scheduling;        /* Scheduling model */
  uint32_t features;          /* Special features */
} coil_compute_model_attrs_t;
```

## 6. Processing Unit Capabilities

### 6.1 CPU Capabilities

Modern CPUs support various instruction set extensions:

| Feature | Description |
|---------|-------------|
| SSE | Streaming SIMD Extensions |
| AVX | Advanced Vector Extensions |
| AVX2 | Advanced Vector Extensions 2 |
| AVX-512 | Advanced Vector Extensions 512-bit |
| FMA | Fused Multiply-Add |
| BMI | Bit Manipulation Instructions |
| ADX | Multi-Precision Add-Carry Instruction Extensions |
| AES | Advanced Encryption Standard Instructions |
| SHA | Secure Hash Algorithm Instructions |
| RDRAND | Hardware Random Number Generator |
| TSX | Transactional Synchronization Extensions |
| SGX | Software Guard Extensions |
| AMX | Advanced Matrix Extensions |

### 6.2 GPU Capabilities

GPU features and capabilities:

| Feature | Description |
|---------|-------------|
| COMPUTE_SHADER | Compute shader support |
| GRAPHICS_PIPELINE | Graphics rendering pipeline |
| RAY_TRACING | Hardware ray tracing acceleration |
| MESH_SHADER | Mesh shader support |
| VARIABLE_RATE_SHADING | Variable rate shading |
| TENSOR_CORES | Dedicated tensor computation units |
| RT_CORES | Dedicated ray tracing cores |
| ASYNC_COMPUTE | Asynchronous compute support |
| SHARED_MEMORY | Fast shared memory |
| TEXTURE_UNITS | Dedicated texture sampling units |
| UNIFIED_MEMORY | Unified memory architecture |
| GRAPHICS_INTEROP | Graphics/compute interoperability |

### 6.3 AI Accelerator Capabilities

AI-specific processing unit capabilities:

| Feature | Description |
|---------|-------------|
| MATRIX_MULTIPLY | Hardware matrix multiplication |
| CONVOLUTION | Hardware convolution |
| ACTIVATION | Hardware activation functions |
| QUANTIZED_MATH | Quantized arithmetic |
| SPARSITY | Sparse tensor operations |
| MIXED_PRECISION | Mixed-precision computation |
| WEIGHT_COMPRESSION | Weight compression |
| TENSOR_CORES | Dedicated tensor cores |
| WINOGRAD | Winograd convolution algorithm |
| INT8_OPERATIONS | 8-bit integer operations |
| BFLOAT16 | Brain floating-point format |
| TF32 | TensorFloat-32 format |

### 6.4 FPGA Capabilities

FPGA-specific capabilities:

| Feature | Description |
|---------|-------------|
| DSP_BLOCKS | Digital Signal Processing blocks |
| BLOCK_RAM | Block RAM resources |
| DISTRIBUTED_RAM | Distributed RAM resources |
| LUT_COUNT | Look-Up Table resources |
| FF_COUNT | Flip-Flop resources |
| HIGH_SPEED_IO | High-speed I/O capabilities |
| HARD_PROCESSOR | Hard processor system |
| PARTIAL_RECONFIGURATION | Partial reconfiguration capability |
| ENCRYPTION | Bitstream encryption |
| HIGH_LEVEL_SYNTHESIS | High-level synthesis support |
| SERDES | Serializer/Deserializer capabilities |
| TRANSCEIVERS | Transceiver capabilities |

## 7. Processing Unit Configuration

### 7.1 Configuration Parameters

COIL defines configuration parameters for processing units:

```c
/**
 * @struct coil_pu_config
 * @brief Processing unit configuration
 */
typedef struct {
  uint32_t pu_id;            /* Processing unit identifier */
  uint32_t pu_class;         /* Processing unit class */
  uint32_t architecture;     /* Architecture */
  uint32_t compute_model;    /* Compute model */
  uint32_t clock_freq;       /* Clock frequency in MHz */
  uint32_t core_count;       /* Number of cores */
  uint32_t thread_count;     /* Number of hardware threads */
  uint32_t simd_width;       /* SIMD width in bits */
  uint32_t reg_file_size;    /* Register file size */
  uint32_t l1_cache;         /* L1 cache size in bytes */
  uint32_t l2_cache;         /* L2 cache size in bytes */
  uint32_t l3_cache;         /* L3 cache size in bytes */
  uint32_t local_mem;        /* Local memory size in bytes */
  uint32_t shared_mem;       /* Shared memory size in bytes */
  uint32_t global_mem;       /* Global memory size in bytes */
  uint32_t mem_bandwidth;    /* Memory bandwidth in MB/s */
  uint32_t features;         /* Feature flags */
  uint32_t extensions;       /* Extension support */
  uint32_t power_limit;      /* Power limit in milliwatts */
} coil_pu_config_t;
```

### 7.2 Configuration Query (Core Functionality)

COIL provides mechanisms to query processing unit configurations at compile time:

```
PU_CONFIG config_option, value   // Static configuration directive
PU_FEATURE feature, support      // Static feature detection
PU_CAPABILITY capability, level  // Static capability detection
```

These directives are processed during compilation to tailor code generation for the target device.

## 8. Hardware-Specific Optimization

### 8.1 Instruction Selection

COIL enables hardware-specific instruction selection:

```
TARGET_INSTR architecture, instruction, [operands...]
```

This directive selects specific hardware instructions during compilation.

### 8.2 Specialization Hints

COIL provides hints for architecture-specific specialization:

```
HINT_ARCHITECTURE arch, feature
HINT_OPTIMIZE target, parameter
HINT_SPECIALIZE function, arch
```

These hints guide the compiler in generating optimized code for specific architectures.

### 8.3 Architecture-Specific Sections

COIL supports architecture-specific code sections:

```
ARCH_BEGIN architecture
...architecture-specific code...
ARCH_ELSE
...fallback code...
ARCH_END
```

This allows for architecture-specific implementations with fallbacks for compatibility.

## 9. Power and Thermal Management

### 9.1 Power States

COIL defines power management states:

| State | Description |
|-------|-------------|
| FULL_POWER | Maximum performance mode |
| BALANCED | Balance of performance and power efficiency |
| POWER_SAVE | Power-saving mode |
| LOW_POWER | Minimum power consumption |
| THERMAL_LIMIT | Thermal throttling |

### 9.2 Static Power Management Directives

COIL provides static power management directives:

```
POWER_PROFILE profile
POWER_CONSTRAINT constraint, value
POWER_REGION_BEGIN profile
POWER_REGION_END
```

These directives are processed at compile time to influence code generation for power efficiency.

## 10. Reliability and Error Handling

### 10.1 Error Detection

COIL supports hardware error detection:

```
ERROR_DETECT enable, error_types
ERROR_HANDLER handler, error_types
```

### 10.2 Error Recovery

For error recovery and resilience:

```
ERROR_RECOVER strategy
CHECKPOINT state
RESTORE checkpoint
```

### 10.3 Error Types

| Error Type | Description |
|------------|-------------|
| MEMORY | Memory error (ECC, parity) |
| COMPUTATION | Computational error |
| OVERFLOW | Arithmetic overflow |
| UNDERFLOW | Arithmetic underflow |
| DENORMAL | Denormal floating-point value |
| INFINITY | Infinity result |
| NAN | Not-a-Number result |
| HARDWARE | Hardware malfunction |
| THERMAL | Thermal issue |
| POWER | Power issue |

## 11. Single Device Processing Units (Core Functionality)

### 11.1 Device Characteristics

Core functionality for targeting a single processing unit:

```c
/**
 * @struct coil_device_characteristics
 * @brief Characteristics of a single target device
 */
typedef struct {
  uint32_t architecture;      /* Target architecture */
  uint32_t features;          /* Available features */
  uint32_t constraints;       /* Hardware constraints */
  uint32_t optimizations;     /* Applicable optimizations */
} coil_device_characteristics_t;
```

### 11.2 Direct Hardware Access

Core functionality for direct hardware access:

```
HW_REGISTER address, size, access_type
HW_MEMORY_REGION start, end, attributes
HW_PORT port, size, access_type
```

These directives provide direct hardware access for bare-metal programming.

### 11.3 Bare-Metal Support

Core functionality for bare-metal environments:

```
BOOT_ENTRY entry_point
INTERRUPT_VECTOR vector, handler
EXCEPTION_HANDLER handler
MEMORY_MAP region, start, end, attributes
```

These directives support bare-metal programming without a runtime system.

## 12. Multi-Device Support (Optional Extension)

> **Note**: The following multi-device capabilities are provided as optional extensions and are not required for core COIL functionality. They can be used when targeting heterogeneous computing environments that provide appropriate runtime support.

### 12.1 Device Discovery (Optional)

Optional extension for runtime device discovery:

```c
/**
 * @struct coil_device_query
 * @brief Optional query for available devices
 * @note Requires runtime support, not available in bare-metal environments
 */
typedef struct {
  uint32_t device_count;     /* Number of devices found */
  uint32_t device_ids[16];   /* Array of device IDs */
  uint32_t flags;            /* Query flags */
} coil_device_query_t;
```

### 12.2 Device Selection (Optional)

Optional extension for runtime device selection:

```
DEVICE_SELECT device_id    // Optional runtime device selection
DEVICE_FALLBACK device_id  // Optional fallback device
```

These operations require runtime support and are not available in bare-metal environments.

### 12.3 Communication Patterns (Optional)

Optional extension for multi-device communication:

| Pattern | Description | Runtime Required |
|---------|-------------|------------------|
| POINT_TO_POINT | Direct communication between two PUs | Yes |
| BROADCAST | One-to-many distribution | Yes |
| GATHER | Many-to-one collection | Yes |
| SCATTER | One-to-many distribution of partitioned data | Yes |
| REDUCE | Many-to-one reduction with operation | Yes |
| ALL_REDUCE | Reduction with result broadcast to all PUs | Yes |
| ALL_TO_ALL | Each PU communicates with all others | Yes |
| STENCIL | Nearest-neighbor communication | Yes |

## 13. Implementation Guidelines

### 13.1 Single Device Implementation (Core)

When implementing for a single processing unit (core functionality):

1. Focus on direct hardware mapping without abstractions
2. Generate code specific to the target architecture
3. Utilize all available hardware features
4. Optimize for the specific memory hierarchy
5. Consider bare-metal requirements (no OS dependencies)
6. Provide fallbacks for unsupported hardware features
7. Focus on static, compile-time optimizations
8. Minimize code size and memory usage
9. Ensure predictable performance

### 13.2 Multi-Device Implementation (Optional)

When implementing optional multi-device support:

1. Create layered implementation with core functionality first
2. Add optional runtime support for device management
3. Implement device discovery and selection
4. Add optional memory transfer capabilities
5. Provide synchronization mechanisms
6. Support dynamic load balancing
7. Implement fallbacks for unavailable devices
8. Consider heterogeneous architecture differences
9. Support different device capabilities

### 13.3 Performance Tuning

For optimal performance on specific processing units:

1. Consider SIMD width and vectorization opportunities
2. Optimize data layout for cache hierarchy
3. Minimize memory transfers
4. Align data for efficient memory access
5. Utilize architecture-specific optimizations
6. Consider power and thermal constraints
7. Optimize for the specific execution model

## 14. Future Directions

COIL is designed to accommodate emerging processing unit architectures:

1. **Neuromorphic Computing**: Brain-inspired computing models
2. **Optical Computing**: Photonic computing elements
3. **Biological Computing**: DNA and molecular computing
4. **Quantum Computing**: Advanced quantum processing paradigms
5. **In-Memory Computing**: Computing within memory structures
6. **Edge AI**: Specialized low-power AI processors
7. **Heterogeneous Integration**: Tightly coupled diverse processing units
8. **Domain-Specific Accelerators**: Highly specialized computing engines

All future extensions will maintain the principle that core functionality works without runtime dependencies, while advanced multi-device features remain optional extensions.