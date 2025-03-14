# COIL Processing Unit Configuration Format

## 1. Overview

The COIL Processing Unit Configuration Format provides a standardized mechanism for defining the capabilities and characteristics of target architectures. This configuration system serves as the essential bridge between architecture-independent COIL binary code and the specific hardware features available on target processing units.

The configuration format fulfills several critical roles in the COIL ecosystem:
- **Target Description**: Precisely specifies hardware capabilities and limitations
- **Feature Detection**: Enables runtime and compile-time feature detection
- **Optimization Guidance**: Provides parameters for optimizing code generation
- **Compatibility Verification**: Ensures COIL code can execute on target hardware
- **Assembler Configuration**: Configures COIL assemblers for specific target architectures

Through this configuration mechanism, COIL achieves its goal of architecture independence while still enabling highly optimized code generation for specific hardware targets.

## 2. Configuration File Format

### 2.1 Basic Structure

COIL processing unit configurations use a hierarchical structure with the following components:

```
target {
  "<target_identifier>" {
    // Basic properties
    architecture = "<architecture_name>"
    vendor = "<vendor_name>"
    description = "<human_readable_description>"
    
    // Features and capabilities
    features = ["<feature1>", "<feature2>", ...]
    
    // Resource specifications
    resources {
      // Resource properties
    }
    
    // Memory model configuration
    memory {
      // Memory properties
    }
    
    // Optimization parameters
    optimization {
      // Optimization settings
    }
  }
}
```

### 2.2 File Format

Configuration files can be stored in one of three formats:

1. **Text Format** (`.coilcfg`): Human-readable text format for direct editing
2. **Binary Format** (`.coilcfgb`): Compact binary representation for efficient loading
3. **Embedded Format**: Configuration embedded directly in COIL binary modules

The text format uses a simple syntax:
- Key-value pairs: `key = value`
- Arrays: `key = ["value1", "value2", ...]`
- Nested sections: `section { ... }`
- Comments: `// Comment` or `/* Multi-line comment */`

### 2.3 Configuration Inheritance

Configurations support inheritance to simplify the definition of related architectures:

```
target {
  "base_x86_64" {
    architecture = "x86_64"
    vendor = "generic"
    features = ["x86_64", "sse", "sse2"]
    // Base properties
  }
  
  "x86_64_avx2" : "base_x86_64" {
    // Inherits all properties from base_x86_64
    features += ["sse3", "ssse3", "sse4.1", "sse4.2", "avx", "avx2", "fma"]
    // Override or add additional properties
  }
}
```

In this example, `x86_64_avx2` inherits all properties from `base_x86_64` and then adds additional features. The `+=` operator appends to an inherited array.

## 3. Target Properties

### 3.1 Basic Properties

| Property | Description | Example |
|----------|-------------|---------|
| `architecture` | Base architecture name | `"x86_64"`, `"arm64"`, `"riscv64"` |
| `vendor` | Hardware vendor name or "generic" | `"intel"`, `"amd"`, `"nvidia"`, `"generic"` |
| `description` | Human-readable description | `"Intel x86_64 with AVX2 support"` |
| `version` | Architecture version | `"v3"`, `"1.0"` |
| `family` | Processor family | `"skylake"`, `"ampere"` |
| `model` | Specific processor model | `"i7-10700K"`, `"A100"` |
| `device_type` | Type of processing unit | `"CPU"`, `"GPU"`, `"TPU"`, `"NPU"`, `"DSP"`, `"FPGA"` |
| `endianness` | Byte order | `"little"`, `"big"` |
| `word_size` | Native word size in bits | `32`, `64` |

### 3.2 Features

Features are defined as an array of string identifiers:

```
features = [
  "x86_64",
  "sse", "sse2", "sse3", "ssse3", "sse4.1", "sse4.2",
  "avx", "avx2", "avx512f", "avx512vl", "avx512bw", "avx512dq",
  "fma", "popcnt", "aes", "pclmul"
]
```

Feature identifiers follow a standardized naming scheme:
- **ISA extensions**: `"sse"`, `"avx"`, `"neon"`, `"sve"`
- **Special instructions**: `"aes"`, `"sha"`, `"crc32"`
- **Hardware capabilities**: `"fma"`, `"popcnt"`, `"bmi1"`, `"bmi2"`
- **Memory features**: `"unaligned_access"`, `"non_temporal"`, `"prefetch"`
- **Concurrency features**: `"atomic_cas"`, `"memory_barrier"`

### 3.3 Feature Dependencies

Feature dependencies can be specified to ensure consistency:

```
feature_dependencies {
  "avx2" = ["avx"]
  "avx" = ["sse4.2"]
  "sse4.2" = ["sse4.1"]
  "sse4.1" = ["ssse3"]
  "ssse3" = ["sse3"]
  "sse3" = ["sse2"]
  "sse2" = ["sse"]
}
```

## 4. Resource Specifications

The `resources` section defines available hardware resources:

```
resources {
  // Register resources
  general_registers = 16
  float_registers = 16
  vector_registers = 32
  
  // Vector processing
  vector_width = 256       // in bits
  vector_lanes = 8         // for 32-bit elements with 256-bit vectors
  
  // Execution resources
  execution_units = 8
  pipeline_depth = 20
  issue_width = 4
  
  // Memory resources
  min_alignment = 32
  cache_line_size = 64
  
  // Parallelism
  hardware_threads = 2     // SMT/Hyperthreading
  max_cores = 32
}
```

### 4.1 Register Resources

| Property | Description | Example |
|----------|-------------|---------|
| `general_registers` | Available general-purpose registers | `16`, `32` |
| `float_registers` | Available floating-point registers | `16`, `32` |
| `vector_registers` | Available vector registers | `16`, `32` |
| `predicate_registers` | Available predicate registers (for SVE, etc.) | `8`, `16` |
| `special_registers` | Special-purpose registers | `8`, `16` |

### 4.2 Vector Processing Resources

| Property | Description | Example |
|----------|-------------|---------|
| `vector_width` | Native vector width in bits | `128`, `256`, `512` |
| `vector_lanes` | Number of lanes in vector unit | `4`, `8`, `16` |
| `supported_vector_types` | Supported vector element types | `["i8", "i16", "i32", "f32", "f64"]` |
| `vector_mask_type` | Type of vector masking | `"k_reg"`, `"vector_mask"` |

### 4.3 Execution Resources

| Property | Description | Example |
|----------|-------------|---------|
| `execution_units` | Number of parallel execution units | `4`, `8` |
| `pipeline_depth` | Typical instruction pipeline depth | `14`, `20` |
| `issue_width` | Maximum instructions issued per cycle | `2`, `4`, `6` |
| `dispatch_ports` | Number of dispatch ports | `8`, `10` |

## 5. Memory Model Configuration

The `memory` section defines memory system characteristics:

```
memory {
  // Addressing
  address_bits = 48
  max_memory = "256GB"
  page_size = "4KB"
  huge_page_sizes = ["2MB", "1GB"]
  
  // Alignment
  natural_alignment = 8
  preferred_alignment = 64
  strict_alignment = false
  
  // Cache hierarchy
  cache_levels = 3
  cache_line_size = 64
  
  // Memory ordering
  memory_models = ["strong", "acquire_release", "relaxed"]
  default_memory_model = "acquire_release"
  
  // Memory spaces
  supported_memory_spaces = ["global", "local", "shared", "constant", "private"]
}
```

### 5.1 Addressing Properties

| Property | Description | Example |
|----------|-------------|---------|
| `address_bits` | Number of address bits | `32`, `48`, `64` |
| `max_memory` | Maximum addressable memory | `"4GB"`, `"16TB"` |
| `page_size` | Default memory page size | `"4KB"`, `"8KB"` |
| `huge_page_sizes` | Supported huge page sizes | `["2MB", "1GB"]` |

### 5.2 Alignment Properties

| Property | Description | Example |
|----------|-------------|---------|
| `natural_alignment` | Natural data alignment | `4`, `8` |
| `preferred_alignment` | Preferred data alignment for performance | `16`, `64` |
| `strict_alignment` | Whether unaligned access is permitted | `true`, `false` |
| `alignment_penalty` | Performance cost of unaligned access | `"high"`, `"medium"`, `"low"` |

### 5.3 Memory Ordering Models

| Property | Description | Example |
|----------|-------------|---------|
| `memory_models` | Supported memory ordering models | `["strong", "acquire_release", "relaxed"]` |
| `default_memory_model` | Default memory ordering model | `"acquire_release"` |
| `supports_atomic_rmw` | Supports atomic read-modify-write | `true`, `false` |
| `supports_transactional_memory` | Supports hardware transactional memory | `true`, `false` |

## 6. Optimization Settings

The `optimization` section provides guidance for code generation:

```
optimization {
  // General optimization parameters
  unroll_factor = 4
  inline_threshold = 100
  
  // Vector optimization
  vector_threshold = 4
  prefer_vector_width = 256
  auto_vectorize = true
  
  // Machine-specific optimizations
  use_fma = true
  use_lea_for_add3 = true
  
  // Scheduling
  instruction_reordering = "aggressive"
  software_prefetch = true
  prefetch_distance = 64
  
  // Custom optimizations
  specialized_strategies = [
    "memcpy_avx",
    "matmul_fma",
    "strided_load_nontemporal"
  ]
}
```

### 6.1 General Optimization Parameters

| Property | Description | Example |
|----------|-------------|---------|
| `unroll_factor` | Default loop unrolling factor | `4`, `8` |
| `inline_threshold` | Function inlining threshold | `100`, `250` |
| `optimization_level` | Default optimization level | `1`, `2`, `3` |
| `size_level` | Code size optimization level | `0`, `1`, `2` |

### 6.2 Vector Optimization Parameters

| Property | Description | Example |
|----------|-------------|---------|
| `vector_threshold` | Min iterations for vectorization | `4`, `8` |
| `prefer_vector_width` | Preferred vector width in bits | `128`, `256`, `512` |
| `auto_vectorize` | Enable automatic vectorization | `true`, `false` |
| `vectorize_gather_scatter` | Use gather/scatter instructions | `true`, `false` |

### 6.3 Specialized Optimization Strategies

| Property | Description | Example |
|----------|-------------|---------|
| `specialized_strategies` | Named optimization strategies | `["memcpy_avx", "matmul_fma"]` |
| `custom_patterns` | Custom instruction patterns | See documentation |
| `disabled_optimizations` | Optimizations to avoid | `["unaligned_vector_store"]` |

## 7. Examples for Common Architectures

### 7.1 x86_64 with AVX2 (CPU)

```
target {
  "x86_64_avx2" {
    architecture = "x86_64"
    vendor = "generic"
    device_type = "CPU"
    description = "x86_64 with AVX2 support"
    
    features = [
      "x86_64", "mmx", "sse", "sse2", "sse3", "ssse3", 
      "sse4.1", "sse4.2", "avx", "avx2", "fma", 
      "popcnt", "aes", "pclmul", "f16c", "bmi1", "bmi2"
    ]
    
    resources {
      general_registers = 16
      float_registers = 16
      vector_registers = 16
      vector_width = 256
      min_alignment = 1
      hardware_threads = 2
      execution_units = 8
      pipeline_depth = 14
      issue_width = 4
    }
    
    memory {
      address_bits = 48
      max_memory = "256TB"
      page_size = "4KB"
      cache_line_size = 64
      memory_models = ["strong", "acquire_release", "relaxed"]
      default_memory_model = "strong"
      strict_alignment = false
    }
    
    optimization {
      unroll_factor = 4
      vector_threshold = 4
      prefer_vector_width = 256
      use_fma = true
      use_lea_for_add3 = true
      software_prefetch = true
      prefetch_distance = 64
      specialized_strategies = ["memcpy_avx", "matmul_fma"]
    }
  }
}
```

### 7.2 NVIDIA Ampere GPU

```
target {
  "nvidia_ampere" {
    architecture = "cuda"
    vendor = "nvidia"
    device_type = "GPU"
    family = "ampere"
    description = "NVIDIA Ampere GPU Architecture"
    
    features = [
      "cuda", "sm_80", "tensor_cores", "fp16", "bf16",
      "atomic64", "unified_memory", "cooperative_groups",
      "warp_shuffle", "dynamic_parallelism"
    ]
    
    resources {
      vector_width = 1024
      vector_lanes = 32
      warp_size = 32
      max_threads_per_block = 1024
      max_blocks_per_sm = 32
      max_shared_memory_per_block = "48KB"
      max_registers_per_thread = 255
      max_grid_dimensions = [2147483647, 65535, 65535]
    }
    
    memory {
      address_bits = 49
      global_memory_bus_width = 384
      memory_models = ["relaxed", "acquire_release", "scope_device"]
      default_memory_model = "relaxed"
      supported_memory_spaces = ["global", "shared", "local", "constant", "texture"]
      l2_cache_size = "40MB"
      cache_line_size = 128
      strict_alignment = true
    }
    
    optimization {
      preferred_warp_multiple = 32
      coalesce_threshold = 32
      shared_memory_banks = 32
      bank_width = 4
      use_tensor_cores = true
      occupancy_target = "high"
      specialized_strategies = ["tensor_core_gemm", "warp_shuffle_reduce"]
    }
  }
}
```

### 7.3 ARM Cortex-A78 (Mobile CPU)

```
target {
  "arm_cortex_a78" {
    architecture = "aarch64"
    vendor = "arm"
    device_type = "CPU"
    family = "cortex-a"
    model = "cortex-a78"
    description = "ARM Cortex-A78 Processor"
    
    features = [
      "aarch64", "neon", "fp16", "dotprod", "crypto",
      "crc", "lse", "sha2", "sha3", "rdm", "sve"
    ]
    
    resources {
      general_registers = 31
      float_registers = 32
      vector_registers = 32
      vector_width = 128
      predicate_registers = 16
      hardware_threads = 1
      execution_units = 6
      pipeline_depth = 13
      issue_width = 4
    }
    
    memory {
      address_bits = 48
      max_memory = "256TB"
      page_size = "4KB"
      huge_page_sizes = ["2MB", "1GB"]
      cache_line_size = 64
      memory_models = ["acquire_release", "relaxed"]
      default_memory_model = "acquire_release"
      strict_alignment = false
    }
    
    optimization {
      unroll_factor = 4
      vector_threshold = 2
      prefer_vector_width = 128
      auto_vectorize = true
      use_sve_when_available = true
      specialized_strategies = ["neon_memcpy", "sve_matmul"]
    }
  }
}
```

### 7.4 Google TPU v4 (Tensor Processing Unit)

```
target {
  "google_tpu_v4" {
    architecture = "tpu"
    vendor = "google"
    device_type = "TPU"
    family = "tpu"
    model = "v4"
    description = "Google TPU v4"
    
    features = [
      "tensorcore", "bfloat16", "mxu", "vpu",
      "systolic_array", "dynamic_compilation"
    ]
    
    resources {
      matrix_units = 2
      matrix_unit_dimensions = [128, 128]
      vector_width = 128
      execution_units = 2
      on_chip_memory = "32MB"
    }
    
    memory {
      hbm_capacity = "16GB"
      hbm_bandwidth = "1200GB/s"
      memory_models = ["relaxed"]
      default_memory_model = "relaxed"
      supported_memory_spaces = ["global", "hbm", "on_chip"]
    }
    
    optimization {
      matrix_multiply_accumulate = true
      tensor_layout = "row_major"
      tiling_dimensions = [128, 128, 128]
      specialized_strategies = ["bfloat16_matmul", "conv2d_optimization"]
      quantization = ["int8", "int4", "bfloat16"]
    }
  }
}
```

## 8. Usage in Assemblers

### 8.1 Loading Configurations

COIL assemblers load target configurations using a standardized API:

```c
// Load target configuration
target_config_t* config = load_target_config("x86_64_avx2.coilcfg");

// Validate configuration
if (!validate_target_config(config)) {
    report_error("Invalid target configuration");
    return NULL;
}

// Initialize assembler with configuration
initialize_assembler(assembler, config);
```

### 8.2 Feature Detection

Features can be queried using a simple API:

```c
// Check if a specific feature is available
if (has_feature(config, "avx2")) {
    // Use AVX2 instructions
    emit_avx2_code();
} else if (has_feature(config, "sse4.2")) {
    // Fall back to SSE4.2
    emit_sse42_code();
} else {
    // Use scalar code
    emit_scalar_code();
}

// Get vector width
int vector_width = get_resource_int(config, "vector_width");
```

### 8.3 Optimization Guidance

Assemblers use configuration parameters for optimization:

```c
// Get unroll factor
int unroll_factor = get_optimization_int(config, "unroll_factor", 1);

// Check if a specialized strategy is available
if (has_specialized_strategy(config, "memcpy_avx")) {
    // Use specialized memcpy implementation
    emit_specialized_memcpy();
}
```

### 8.4 Runtime Detection

COIL provides APIs for runtime detection of available features:

```c
// Detect current device capabilities
target_config_t* detected_config = detect_current_device();

// Create compatible configuration
target_config_t* compatible_config = create_compatible_config(
    required_config, detected_config);

// Check compatibility
if (!is_compatible(required_config, detected_config)) {
    report_error("Current device is not compatible with required features");
    return ERROR_INCOMPATIBLE;
}
```

## 9. Configuration Management

### 9.1 Configuration Discovery

COIL provides mechanisms for discovering available configurations:

```c
// Get list of available target configurations
target_config_list_t* configs = enumerate_target_configs();

// Print available configurations
for (int i = 0; i < configs->count; i++) {
    printf("Target: %s (%s)\n", 
           configs->items[i].name,
           configs->items[i].description);
}

// Find best match for current device
target_config_t* best_match = find_best_target_match(detect_current_device());
```

### 9.2 Configuration Validation

Configurations can be validated for consistency:

```c
// Validate configuration
validation_result_t result = validate_target_config(config);

// Check for validation errors
if (result.error_count > 0) {
    for (int i = 0; i < result.error_count; i++) {
        report_error("Validation error: %s", result.errors[i]);
    }
}

// Check for validation warnings
if (result.warning_count > 0) {
    for (int i = 0; i < result.warning_count; i++) {
        report_warning("Validation warning: %s", result.warnings[i]);
    }
}
```

### 9.3 Configuration Generation

Tools can automatically generate configurations:

```c
// Detect current device and generate configuration
target_config_t* config = detect_and_generate_config();

// Save configuration to file
if (save_target_config(config, "detected_device.coilcfg") != SUCCESS) {
    report_error("Failed to save configuration");
}
```

## 10. Future Directions

The COIL processing unit configuration format is designed to evolve with emerging hardware:

### 10.1 Enhanced Hardware Support

Future versions will add support for:
- Neuromorphic computing hardware
- Quantum computing integration
- Domain-specific accelerators
- Novel memory architectures

### 10.2 Extended Optimization Properties

Planned enhancements include:
- Machine learning-based optimization parameters
- Automatic performance tuning feedback
- Workload-specific optimization profiles
- Energy efficiency optimization parameters

### 10.3 Dynamic Configuration

Future support for:
- Runtime-adjustable configurations
- Workload-adaptive optimization
- Frequency/power scaling parameters
- Dynamic feature detection and adaptation

### 10.4 Ecosystem Integration

Improvements in:
- Integration with CI/CD pipelines
- Cloud-based configuration repositories
- Automated configuration generation
- Cross-vendor standardization efforts

## 11. Conclusion

The COIL Processing Unit Configuration Format provides a robust, flexible mechanism for describing target architectures and their capabilities. By standardizing this interface between COIL code and assemblers, it enables the COIL ecosystem to achieve architecture independence while still allowing highly optimized code generation.

Compiler developers benefit from a clear specification of target capabilities, while assembler developers gain a comprehensive framework for configuring their translation process. Through this configuration system, COIL fulfills its promise of "compile once, optimize everywhere" across the diverse landscape of modern processing units.