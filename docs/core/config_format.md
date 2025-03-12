# COIL Assembler Configuration File Format

## 1. Introduction

The COIL Assembler configuration file format provides a standardized way to define target architectures, features, and compilation parameters for the COIL assembler. This document defines the structure and syntax of these configuration files and clarifies their relationship to the COIL language and assembler.

### 1.1 Role in the COIL Ecosystem

The configuration file acts as an external definition mechanism for the COIL assembler:

1. **Target Definition**: Defines available processing unit targets and their capabilities
2. **Feature Registry**: Catalogs hardware features and capabilities for reference by COIL code
3. **Assembly Control**: Provides parameters for the assembly process
4. **Optimization Guidance**: Offers optimization hints specific to targets
5. **Output Control**: Specifies binary output requirements

The COIL language itself includes operations to specify which target(s) the code is being compiled for, referring to target identifiers defined in these configuration files. This separation allows the COIL language to remain hardware-agnostic while enabling precise targeting during assembly.

### 1.2 Configuration Auto-Generation

Configuration files can be manually created or automatically generated:

1. **Manual Creation**: Hand-crafted for specific target environments
2. **Auto-Generation**: Generated using platform detection (e.g., cpuid on x86, device tree on ARM)
3. **Target Database**: Generated from a database of known target specifications
4. **Hybrid Approach**: Auto-generated with manual customization

Future tools will support automatic configuration file generation for the current machine, simplifying the development process.

## 2. File Format

COIL Assembler configuration files use a simple key-value syntax with hierarchical sections. The format is designed to be human-readable, easy to parse, and extensible.

### 2.1 Basic Syntax

```
# This is a comment
key = value
section {
  nested_key = value
  another_key = "string value"
  
  nested_section {
    deeper_key = value
  }
}
```

### 2.2 Data Types

| Type | Example | Description |
|------|---------|-------------|
| Integer | `42` | Decimal integer |
| Hexadecimal | `0x2A` | Hexadecimal integer |
| Float | `3.14` | Floating point value |
| Boolean | `true`, `false` | Boolean value |
| String | `"hello"` | String (quoted) |
| Array | `[1, 2, 3]` | Array of values |
| Map | `{key = value}` | Key-value mapping |

## 3. Standard Sections

### 3.1 Target Section

The `target` section defines processor targets that can be referenced from COIL code:

```
target {
  # Target identifier and basic properties
  "x86_64_avx2" {
    architecture = "x86_64"
    vendor = "generic"
    
    # Core capabilities
    features = [
      "sse", "sse2", "sse3", "ssse3", "sse4.1", "sse4.2",
      "avx", "avx2", "fma", "popcnt", "aes", "pclmul"
    ]
    
    # Optional features that may affect code generation
    optional_features = ["avx512f", "avx512vl", "avx512bw", "avx512dq"]
    
    # Hardware resources
    resources {
      registers = 16
      vector_width = 256  # AVX2 = 256-bit vectors
      min_alignment = 32
      memory_models = ["strong", "acquire_release", "relaxed"]
    }
    
    # Bare-metal specifics
    bare_metal {
      boot_modes = ["bios", "uefi", "linuxboot"]
      interrupt_model = "apic"
      io_modes = ["port", "mmio"]
      address_width = 64
      page_size = 4096
    }
  }
  
  # ARM64 target example
  "arm64_neon" {
    architecture = "aarch64"
    vendor = "generic"
    
    features = ["neon", "crc", "crypto", "asimd"]
    optional_features = ["sve", "sve2", "lse"]
    
    resources {
      registers = 31
      vector_width = 128
      min_alignment = 16
      memory_models = ["strong", "acquire_release", "relaxed"]
    }
    
    bare_metal {
      boot_modes = ["uefi", "atf", "u-boot"]
      interrupt_model = "gic"
      io_modes = ["mmio"]
      address_width = 64
      page_size = 4096
    }
  }
  
  # Additional target definitions...
}
```

### 3.2 Compilation Section

The `compilation` section defines general assembly and compilation parameters:

```
compilation {
  # Input files or directories
  input = ["src/main.coil", "src/lib.coil"]
  
  # Output file
  output = "build/program.bin"
  
  # Include paths
  include_paths = ["include", "/usr/include/coil"]
  
  # Define macros
  defines = {
    DEBUG = 1
    VERSION = "1.0.0"
  }
  
  # Warning level
  warnings = "all"
  
  # Treat warnings as errors
  werror = false
  
  # Assembly modes
  assembly {
    strict_mode = true
    position_independent = true
    default_target = "x86_64_avx2"  # Default target if not specified in COIL code
    generate_listings = true
    listing_format = "annotated"
  }
}
```

### 3.3 Optimization Section

The `optimization` section provides target-specific optimization guidance:

```
optimization {
  # Default optimization level
  level = 2  # 0-3
  
  # Size vs speed preference (0.0 = size, 1.0 = speed)
  size_speed = 0.5
  
  # Target-specific optimization profiles
  target_optimizations {
    "x86_64_avx2" {
      preferred_vector_width = 256
      unroll_factor = 4
      instruction_fusion = true
      cache_line_size = 64
      preferred_memory_access = 32
    }
    
    "arm64_neon" {
      preferred_vector_width = 128
      unroll_factor = 4
      instruction_fusion = true
      cache_line_size = 64
      preferred_memory_access = 16
    }
  }
  
  # Function-specific hints
  function_hints {
    "matrix_multiply" {
      level = 3
      vectorize = true
      unroll_factor = 8
    }
  }
}
```

### 3.4 Memory Section

The `memory` section defines memory model configuration:

```
memory {
  # Default memory alignment
  default_alignment = 8
  
  # Target-specific memory configurations
  target_memory {
    "x86_64_avx2" {
      alignments = {
        vector = 32
        cache_line = 64
        page = 4096
      }
      
      # Memory hierarchy
      hierarchy = {
        l1_cache_size = 32768
        l2_cache_size = 262144
        l3_cache_size = 8388608
        cacheline_size = 64
      }
      
      # Memory-mapped I/O regions for bare-metal
      mmio_regions = {
        apic = { base = 0xFEE00000, size = 0x1000 }
        ioapic = { base = 0xFEC00000, size = 0x1000 }
      }
    }
    
    "arm64_neon" {
      alignments = {
        vector = 16
        cache_line = 64
        page = 4096
      }
      
      # Memory hierarchy
      hierarchy = {
        l1_cache_size = 65536
        l2_cache_size = 1048576
        cacheline_size = 64
      }
    }
  }
}
```

### 3.5 Processing Units Definition Section

The `processing_units` section defines the detailed characteristics of available processing units:

```
processing_units {
  # x86_64 CPU definitions
  "x86_64" {
    # Base x86_64 features (available on all x86_64 CPUs)
    base_features = ["x87", "mmx", "sse", "sse2"]
    
    # Feature sets
    feature_sets {
      # SSE feature set
      "sse" = ["sse", "sse2", "sse3", "ssse3", "sse4.1", "sse4.2"]
      
      # AVX feature set
      "avx" = ["avx", "avx2", "fma"]
      
      # AVX-512 feature set
      "avx512" = ["avx512f", "avx512vl", "avx512bw", "avx512dq", "avx512cd"]
    }
    
    # Instruction extensions and their requirements
    instruction_extensions {
      "popcnt" = { min_cpu = "nehalem" }
      "avx" = { min_cpu = "sandybridge" }
      "avx2" = { min_cpu = "haswell" }
      "avx512f" = { min_cpu = "skylake-avx512" }
    }
    
    # Vector width by feature
    vector_widths {
      "sse" = 128
      "avx" = 256
      "avx512" = 512
    }
    
    # Specific microarchitectures
    microarchitectures {
      "haswell" {
        features = ["avx2", "fma", "bmi1", "bmi2", "popcnt", "aes", "pclmul"]
        vector_width = 256
        cacheline_size = 64
      }
      
      "skylake" {
        features = ["avx2", "fma", "bmi1", "bmi2", "popcnt", "aes", "pclmul"]
        vector_width = 256
        cacheline_size = 64
      }
      
      "zen3" {
        features = ["avx2", "fma", "bmi1", "bmi2", "popcnt", "aes", "pclmul"]
        vector_width = 256
        cacheline_size = 64
      }
    }
  }
  
  # ARM64 CPU definitions
  "arm64" {
    base_features = ["neon", "asimd"]
    
    feature_sets {
      "crypto" = ["aes", "sha1", "sha2", "pmull"]
      "simd" = ["neon", "asimd"]
      "advanced_simd" = ["sve", "sve2"]
    }
    
    instruction_extensions {
      "sve" = { min_cpu = "armv8.2-a" }
      "sve2" = { min_cpu = "armv8.4-a" }
    }
    
    vector_widths {
      "neon" = 128
      "sve" = "scalable"  # SVE is scalable
    }
    
    microarchitectures {
      "cortex-a76" {
        features = ["neon", "asimd", "crc", "crypto"]
        vector_width = 128
        cacheline_size = 64
      }
      
      "cortex-a78" {
        features = ["neon", "asimd", "crc", "crypto"]
        vector_width = 128
        cacheline_size = 64
      }
    }
  }
}
```

### 3.6 Output Section

The `output` section defines output file formats and options:

```
output {
  # Primary output format
  format = "bin"  # bin, elf, pe, etc.
  
  # Additional output formats
  additional_formats = ["elf", "hex"]
  
  # Output directory
  directory = "build"
  
  # Output file name pattern
  filename = "program-${target}"
  
  # Include debug information
  debug_info = true
  
  # Target-specific output options
  target_output {
    "x86_64_avx2" {
      elf {
        class = 64
        data = "little"
        os_abi = "sysv"
        abi_version = 0
        type = "exec"
        machine = "x86_64"
        entry_point = "main"
      }
      
      bin {
        base_address = 0x400000
      }
    }
    
    "arm64_neon" {
      elf {
        class = 64
        data = "little"
        os_abi = "sysv"
        abi_version = 0
        type = "exec"
        machine = "aarch64"
        entry_point = "main"
      }
    }
  }
}
```

## 4. Integration with COIL Language

### 4.1 Target Selection in COIL

The COIL language includes operations to select which target configuration to use:

```
# In COIL assembly:
TARGET "x86_64_avx2"  # Select the x86_64_avx2 target defined in config

# Function for a specific target
FUNCTION matrix_multiply TARGET "x86_64_avx2" {
  # Function implementation for x86_64 with AVX2
}

# Alternative implementation for different target
FUNCTION matrix_multiply TARGET "arm64_neon" {
  # Function implementation for ARM64 with NEON
}

# Check for specific features
IF_FEATURE "avx2" {
  # Code using AVX2 instructions
}
```

### 4.2 Feature Detection in COIL

COIL operations for checking features defined in the configuration:

```
# Check if current target has a feature
HAS_FEATURE "avx2"  # Returns boolean result

# Select code path based on feature
SELECT_FEATURE {
  "avx512": {
    # AVX-512 implementation
  },
  "avx2": {
    # AVX2 implementation
  },
  "sse4.2": {
    # SSE4.2 implementation
  },
  default: {
    # Generic implementation
  }
}
```

### 4.3 Target-Specific Memory Access

COIL operations for target-specific memory handling:

```
# Target-optimal vector load
LOAD_VECTOR v1, [ptr], TARGET_OPTIMAL_VECTOR_WIDTH

# Target-specific alignment
ALIGN TARGET_VECTOR_ALIGNMENT
```

## 5. Configuration Auto-Generation

### 5.1 Platform Detection

Future tools will enable automatic configuration generation:

```
# Command-line usage example
coil-config-gen --target=current --output=machine.config

# Library usage
import coil_config
config = coil_config.detect_platform()
config.save("machine.config")
```

### 5.2 Platform Detection Methods

The platform detection varies by architecture:

1. **x86/x86-64**: Uses CPUID instruction to detect features
2. **ARM/ARM64**: Uses identification registers and device tree
3. **RISC-V**: Uses CSR registers and platform specifications
4. **Other Architectures**: Uses architecture-specific detection methods

### 5.3 Example Auto-Generated Configuration

An automatically generated configuration might look like:

```
# Auto-generated COIL Assembler configuration
# Generated: 2025-03-12T15:30:45
# Platform: x86_64, AMD Ryzen 9 5950X

target {
  "current" {
    architecture = "x86_64"
    vendor = "amd"
    model = "ryzen_9_5950X"
    
    features = [
      "x87", "mmx", "sse", "sse2", "sse3", "ssse3",
      "sse4.1", "sse4.2", "avx", "avx2", "fma",
      "aes", "pclmul", "popcnt", "rdrand", "bmi1", "bmi2"
    ]
    
    resources {
      physical_cores = 16
      logical_cores = 32
      vector_width = 256
      min_alignment = 32
      memory_models = ["strong", "acquire_release", "relaxed"]
    }
    
    memory {
      l1_cache_size = 32768
      l2_cache_size = 524288
      l3_cache_size = 67108864
      cacheline_size = 64
      page_size = 4096
    }
  }
}

compilation {
  assembly {
    default_target = "current"
  }
}
```

## 6. Configuration Composition

### 6.1 Base Configurations

Define reusable base configurations:

```
# base_x86_64.config
processing_units {
  "x86_64" {
    base_features = ["x87", "mmx", "sse", "sse2"]
    # ...
  }
}
```

### 6.2 Including Base Configurations

Include and extend base configurations:

```
# Include base x86_64 definitions
include "base_x86_64.config"

# Extend with specific target
target {
  "x86_64_custom" {
    architecture = "x86_64"
    # Add more features and settings...
  }
}
```

### 6.3 Configuration Libraries

Libraries of pre-defined configurations for common platforms:

```
# Select from standard configuration library
include "coil_config_lib/x86_64/intel/skylake.config"
include "coil_config_lib/x86_64/amd/zen3.config"
include "coil_config_lib/arm64/apple/m1.config"
```

## 7. Implementation Guidelines

### 7.1 Configuration Parser Implementation

When implementing a COIL Assembler configuration parser:

1. Parse the file in a single pass, building a hierarchical structure
2. Process includes early to create a merged configuration
3. Resolve variables after the entire configuration is loaded
4. Validate against a schema to catch configuration errors
5. Provide sensible defaults for missing values
6. Generate warnings for deprecated or invalid settings
7. Convert the configuration into internal assembler settings
8. Make the configuration data accessible to the assembler during code generation

### 7.2 COIL Assembler Integration

Guidelines for integrating configuration with the COIL assembler:

1. Access target definitions when processing TARGET directives
2. Use feature information to validate instruction usage
3. Apply target optimization hints during code generation
4. Generate proper error messages for unavailable features
5. Apply memory layout rules based on target requirements
6. Format output according to target specifications

### 7.3 Configuration for IDEs and Tools

Supporting configuration in development environments:

1. Provide autocompletion based on available targets and features
2. Highlight errors when using features not available in the target
3. Offer quick-fixes for common configuration issues
4. Show hover information about target capabilities
5. Support navigation to feature definitions