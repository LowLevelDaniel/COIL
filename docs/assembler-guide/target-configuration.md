# Target Configuration for COIL Assemblers

This document guides assembler creators on how to design and implement target configuration systems for COIL assemblers. A robust target configuration system is critical for accurately defining target architectures, their capabilities, and optimization strategies.

## Target Configuration Overview

The target configuration defines the characteristics and capabilities of a processing unit that a COIL assembler targets. This information allows the assembler to make appropriate decisions when translating COIL code to native code.

A complete target configuration includes:

1. **Basic architecture information**: Identifier, class, vendor, etc.
2. **Feature sets**: Available instructions, extensions, and capabilities
3. **Resource information**: Registers, memory hierarchy, etc.
4. **Memory model**: Alignment, addressing, ordering, etc.
5. **Optimization parameters**: Target-specific optimization guidance

## Configuration File Format

COIL assemblers typically use a structured configuration file format that allows for clear definition of target characteristics. The recommended format follows this structure:

```
# Target configuration for X architecture
target {
  "identifier" {
    architecture = "arch_name"
    vendor = "vendor_name"
    
    features = [
      "feature1", "feature2", "feature3"
    ]
    
    resources {
      property1 = value1
      property2 = value2
    }
    
    memory {
      model = "model_name"
      alignment = value
    }
    
    optimization {
      parameter1 = value1
      parameter2 = value2
    }
  }
}
```

This hierarchical structure allows for clear organization of target properties and easy extension.

## Basic Target Definition

Start with the basic target identification information:

```
target {
  "cpu" {                      # Unique target identifier (device class)
    device_class = "CPU"       # Processing unit class
    vendor = "generic"         # Hardware vendor (or "generic")
    version = "1.0"            # Configuration version
    description = "Generic CPU target" # Human-readable description
  }
}
```

The target identifier should be unique and descriptive, typically including the architecture name and key feature sets.

## Feature Sets Definition

Define the features available on the target:

```
target {
  "cpu" {
    # ... basic information ...
    
    # Assembler-specific implementation details for the given hardware
    # These are used by the assembler but not exposed to HOIL/COIL users
    implementation {
      architecture = "x86_64"
      features = ["sse4.2", "avx2", "fma"]
      register_map = "x86_64_standard"
      instruction_set = "x86_64_extended"
      vector_implementation = "native"  # Could be "native" or "emulated"
      memory_addressing = "flat_64bit"
    }
  }
}
```

Features should be listed from most basic to most advanced, with optional or rarely used features separated for clarity.

## Resource Definition

Define the hardware resources available on the target:

```
target {
  "cpu" {
    # ... previous sections ...
    
    resources {
      # These resources are used by the assembler for native code generation
      # but are transparent to the HOIL/COIL user
      physical_cores = 8
      logical_cores = 16
      cache_size = 16777216      # 16MB cache
      cacheline_size = 64        # Cache line size in bytes
    }
  }
}
```

This section should include all hardware resources that affect code generation and optimization.

## Memory Model Definition

Define the memory model characteristics:

```
target {
  "x86_64_avx2" {
    # ... previous sections ...
    
    memory {
      # Alignment requirements
      min_alignment = 1          # Minimum alignment in bytes
      preferred_alignment = 16   # Preferred alignment in bytes
      vector_alignment = 32      # Vector alignment in bytes
      
      # Memory ordering model
      ordering = "strong"        # Memory ordering model
      
      # Virtual memory characteristics
      page_size = 4096           # Page size in bytes
      
      # Addressing capabilities  
      address_width = 64         # Address width in bits
      address_spaces = ["flat"]  # Supported address spaces
    }
  }
}
```

The memory model definition is crucial for correct code generation, especially for memory operations.

## Bare-Metal Configuration

For bare-metal targets, include hardware-specific information:

```
target {
  "cpu_bare" {
    # ... previous sections ...
    
    bare_metal {
      # Boot and initialization
      boot_modes = ["firmware", "direct"]
      
      # Interrupt model
      interrupt_support = true
      max_interrupt_priority = 15
      
      # I/O capabilities
      io_modes = ["memory_mapped", "port_mapped"]
      
      # Default memory regions
      reserved_regions = {
        firmware = { base = 0xF0000000, size = 0x10000 }
        peripherals = { base = 0xE0000000, size = 0x10000 }
      }
    }
  }
}
```

This section is especially important for assemblers targeting bootloaders, firmware, or operating system kernels.

## Processing Unit Definition

Define detailed processing unit characteristics:

```
target {
  "cpu" {
    # ... previous sections ...
    
    processing_unit {
      # General characteristics - visible to HOIL/COIL
      pu_class = "CPU"            # Processing unit class
      
      # Assembler implementation details - not exposed to HOIL/COIL
      implementation {
        vector_width = 256        # Native vector width if available
        vector_implementation = "native"  # or "emulated"
        simd_implementation = "avx2"  # Implementation detail for assembler
      }
    }
  }
}
```

This section helps the assembler understand the processing unit's computation capabilities and make informed decisions.

## Optimization Configuration

Define optimization guidelines for the target:

```
target {
  "cpu" {
    # ... previous sections ...
    
    optimization {
      # Assembler optimization settings - transparent to HOIL/COIL
      implementation {
        # When to use native vector operations vs. scalar emulation
        vector_threshold = 4      # Use vector ops for 4+ elements
        
        # Hardware-specific optimizations
        use_fma = true            # Use fused multiply-add if available
        unroll_factor = 4         # Default loop unrolling factor
        
        # Function-specific optimization strategies
        function_optimizations {
          "matrix_multiply" {
            strategy = "blocked"  # Block matrix multiplication
            block_size = 64       # Block size
            vectorize = true      # Use vector instructions when available
          }
        }
      }
    }
  }
}
```

These optimization guidelines help the assembler generate more efficient code for the specific target.

## Output Configuration

Define output format and options:

```
target {
  "cpu_vector" {
    # ... previous sections ...
    
    output {
      # Primary output format
      format = "elf"  # elf, pe, mach-o, bin, etc.
      
      # Format-specific options
      elf {
        class = 64                # 64-bit ELF
        data = "little"           # Little-endian
        os_abi = "sysv"           # System V ABI
        type = "exec"             # Executable file
      }
      
      # Debug information
      debug_info = true           # Include debug information
      debug_format = "dwarf"      # Debug format
      dwarf_version = 4           # DWARF version
    }
  }
}
```

This section configures how the assembler should format its output for the target platform.

## Target Discovery and Configuration Generation

Implement automatic target configuration generation:

```c
// Example target detection function
target_config_t* detect_current_target() {
    target_config_t* config = create_empty_target_config();
    
    // Detect architecture
    config->architecture = detect_architecture();
    
    // Detect vendor
    config->vendor = detect_vendor();
    
    // Detect features
    detect_features(config);
    
    // Detect resources
    detect_resources(config);
    
    // Set optimization parameters
    set_default_optimizations(config);
    
    return config;
}

// Save the detected configuration to a file
void save_target_config(target_config_t* config, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "# Auto-generated COIL target configuration\n");
    fprintf(file, "# Generated: %s\n", get_timestamp());
    fprintf(file, "target {\n");
    fprintf(file, "  \"current\" {\n");
    
    // Write architecture and vendor
    fprintf(file, "    architecture = \"%s\"\n", config->architecture);
    fprintf(file, "    vendor = \"%s\"\n", config->vendor);
    
    // Write features
    fprintf(file, "    features = [\n");
    for (int i = 0; i < config->feature_count; i++) {
        fprintf(file, "      \"%s\"%s\n", 
                config->features[i],
                i < config->feature_count - 1 ? "," : "");
    }
    fprintf(file, "    ]\n");
    
    // Write other sections...
    
    fprintf(file, "  }\n");
    fprintf(file, "}\n");
    
    fclose(file);
}
```

This allows users to automatically generate configurations for their current system, simplifying the setup process.

## Configuration Inheritance and Composition

Support configuration inheritance to reduce duplication:

```
# base_cpu.config - Basic CPU device class
target {
  "base_cpu" {
    device_class = "CPU"
    vendor = "generic"
    
    # Base configuration - visible to HOIL/COIL users
    # ...
    
    # Implementation details - only for assembler
    implementation {
      architecture = "generic"
      vector_implementation = "emulated"  # Fall back to emulation
    }
  }
}

# x86_64_cpu.config - CPU with x86_64 implementation
include "base_cpu.config"

target {
  "x86_64_cpu" {
    inherits = "base_cpu"
    
    # Implementation details for x86_64
    implementation {
      architecture = "x86_64"
      features = ["sse4.2", "avx2"]
      vector_implementation = "native"  # Use hardware vector support
    }
  }
}
```

This approach allows for maintainable configuration hierarchies and easier updates.

## Configuration Validation

Implement configuration validation to catch errors:

```c
// Example validation function
bool validate_target_config(target_config_t* config) {
    // Check required fields
    if (!config->device_class || !config->vendor) {
        report_error("Missing required device class or vendor");
        return false;
    }
    
    // Validate assembler implementation details
    if (config->implementation.vector_implementation == VECTOR_IMPL_NATIVE) {
        // If native vector implementation is specified, verify hardware support
        if (!has_hardware_vector_support(&config->implementation)) {
            report_warning("Native vector implementation specified but hardware doesn't support it. Falling back to emulation.");
            config->implementation.vector_implementation = VECTOR_IMPL_EMULATED;
        }
    }
    
    // More validation of implementation details...
    
    return true;
}
```

Thorough validation helps prevent misconfigurations that could lead to incorrect code generation.

## Configuration Interface

Implement a clean API for accessing configuration data:

```c
// Example configuration API - only methods needed by HOIL/COIL
bool target_is_device_class(const target_config_t* config, const char* device_class);
const char* target_get_device_class(const target_config_t* config);
const char* target_get_vendor(const target_config_t* config);

// Example implementation API - only used by the assembler internally
bool target_impl_supports_feature(const target_config_t* config, const char* feature);
bool target_impl_has_native_vector(const target_config_t* config);
uint32_t target_impl_get_vector_width(const target_config_t* config);
const char* target_impl_get_architecture(const target_config_t* config);
```

A well-designed API makes it easier for the rest of the assembler to correctly use the configuration data.

## Configuration Documentation

Generate documentation from configurations:

```c
// Example documentation generator
void generate_target_docs(const target_config_t* config, const char* output_path) {
    FILE* file = fopen(output_path, "w");
    if (!file) return;
    
    fprintf(file, "# %s Target Documentation\n\n", config->name);
    fprintf(file, "Architecture: %s\n", config->architecture);
    fprintf(file, "Vendor: %s\n", config->vendor);
    
    fprintf(file, "\n## Features\n\n");
    for (int i = 0; i < config->feature_count; i++) {
        fprintf(file, "- %s\n", config->features[i]);
    }
    
    // More documentation...
    
    fclose(file);
}
```

Auto-generated documentation ensures it stays synchronized with the actual configuration.

## Best Practices for Target Configuration

1. **Use hierarchical structure**: Organize configurations with clear hierarchies
2. **Support inheritance**: Allow targets to inherit from base configurations
3. **Validate thoroughly**: Check for consistency and correctness
4. **Provide defaults**: Include sensible defaults for optional parameters
5. **Support auto-detection**: Implement automatic target detection
6. **Document well**: Include comments and documentation
7. **Keep extensible**: Design for future expansion
8. **Version configurations**: Track configuration versions
9. **Test across targets**: Verify configurations on multiple platforms
10. **Separate concerns**: Keep target configuration separate from assembler code

## Further Reading

- [Processing Units Model](../core-spec/processing-units.md): The abstract model of computational devices
- [Instruction Implementation](instruction-implementation.md): How to map COIL instructions to native instructions
- [Binary Translation](binary-translation.md): Translating COIL to native code
- [Bare-Metal Considerations](bare-metal-considerations.md): Special considerations for bare-metal targets