# COIL Configuration File Format

## 1. Introduction

The COIL configuration file format provides a standardized way to specify compilation parameters, target architectures, optimization settings, and other configuration options for COIL compilers and assemblers. This document defines the structure and syntax of COIL configuration files.

## 2. File Format

COIL configuration files use a simple key-value syntax with hierarchical sections. The format is designed to be human-readable, easy to parse, and extensible.

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

The `target` section defines the target processing units and architectures:

```
target {
  # Primary target
  primary = "x86_64"
  
  # Additional targets
  additional = ["arm64", "riscv64"]
  
  # Target-specific features
  features {
    x86_64 = ["avx2", "sse4.2", "fma"]
    arm64 = ["neon", "crypto"]
  }
  
  # Minimum architecture versions
  min_version {
    x86_64 = "haswell"
    arm64 = "armv8.2-a"
  }
}
```

### 3.2 Compilation Section

The `compilation` section defines general compilation parameters:

```
compilation {
  # Input files or directories
  input = ["src/main.c", "src/lib.c"]
  
  # Output file
  output = "build/program.coil"
  
  # Include paths
  include_paths = ["include", "/usr/include"]
  
  # Define macros
  defines = {
    DEBUG = 1
    VERSION = "1.0.0"
  }
  
  # Warning level
  warnings = "all"
  
  # Treat warnings as errors
  werror = false
}
```

### 3.3 Optimization Section

The `optimization` section defines optimization settings:

```
optimization {
  # Optimization level
  level = 2  # 0-3
  
  # Size vs speed preference (0.0 = size, 1.0 = speed)
  size_speed = 0.5
  
  # Enable/disable specific optimizations
  enable = [
    "inline",
    "vectorize",
    "unroll-loops"
  ]
  
  disable = [
    "auto-vectorize"
  ]
  
  # Loop unrolling factor
  unroll_factor = 4
  
  # Inlining threshold
  inline_threshold = 250
  
  # Function-specific optimization settings
  function {
    "matrix_multiply" {
      level = 3
      enable = ["vectorize", "unroll-loops"]
      unroll_factor = 8
    }
  }
}
```

### 3.4 Memory Section

The `memory` section defines memory model configuration:

```
memory {
  # Default memory model
  model = "relaxed"
  
  # Memory space allocation preferences
  allocation {
    default = "global"
    arrays = "shared"
    constants = "constant"
  }
  
  # Alignment preferences
  alignment {
    default = 8
    vectors = 16
    matrices = 32
  }
  
  # Memory safety checks
  safety {
    bounds_check = true
    null_check = true
    type_check = true
  }
}
```

### 3.5 Processing Units Section

The `processing_units` section defines specific processing unit configurations:

```
processing_units {
  # CPU configuration
  cpu {
    class = "CPU_VECTOR"
    architecture = "x86_64"
    features = ["avx2", "avx512f"]
    threads = 16
  }
  
  # GPU configuration
  gpu {
    class = "GPU_COMPUTE"
    architecture = "cuda"
    compute_capability = "8.0"
    memory = 8192  # MB
    max_threads_per_block = 1024
  }
  
  # TPU configuration
  tpu {
    class = "TPU"
    version = 3
    chips = 4
  }
  
  # Distribution of work
  distribution {
    matrix_ops = "gpu"
    vector_ops = "cpu"
    tensor_ops = "tpu"
  }
}
```

### 3.6 Debug Section

The `debug` section defines debugging options:

```
debug {
  # Debug information level
  level = 2  # 0-3
  
  # Include source mapping
  source_map = true
  
  # Generate debug symbols
  symbols = true
  
  # Profiling information
  profiling = false
  
  # Debug specific sections
  sections = ["memory", "execution"]
}
```

## 4. Conditional Configuration

COIL configuration files support conditional sections based on platform, architecture, or user-defined variables:

```
# Platform-specific configuration
if(platform == "linux") {
  compilation {
    defines {
      LINUX = 1
    }
  }
}

# Architecture-specific configuration
if(arch == "x86_64") {
  optimization {
    enable = ["avx-vectorize"]
  }
} else if(arch == "arm64") {
  optimization {
    enable = ["neon-vectorize"]
  }
}

# Feature-specific configuration
if(has_feature("avx512")) {
  memory {
    alignment {
      vectors = 64
    }
  }
}
```

## 5. Variable Definitions and References

Variables can be defined and referenced throughout the configuration:

```
# Define variables
vars {
  base_dir = "/project"
  num_threads = 8
  features = ["avx2", "sse4"]
}

# Reference variables
compilation {
  include_paths = ["${base_dir}/include"]
}

processing_units {
  cpu {
    threads = ${num_threads}
    features = ${features}
  }
}
```

## 6. Includes and Imports

Configuration files can include other configuration files:

```
# Include another configuration file
include "base_config.coil"

# Override specific settings
optimization {
  level = 3
}
```

## 7. Configuration File Example

Here's a complete example of a COIL configuration file for a heterogeneous computing project:

```
# Main COIL configuration file for Matrix Multiplication Project
# Version: 1.0

# Include base configurations
include "defaults.coil"

# Project information
project {
  name = "MatrixMultiply"
  version = "1.0.0"
  author = "COIL Developer"
}

# Target configuration
target {
  primary = "x86_64"
  additional = ["cuda", "opencl"]
  
  features {
    x86_64 = ["avx2", "avx512f", "fma"]
    cuda = ["sm_80"]
  }
}

# Compilation options
compilation {
  input = ["src/main.c", "src/matrix.c", "src/utils.c"]
  output = "build/matmul.coil"
  
  include_paths = ["include", "lib/include"]
  
  defines {
    VERSION = "1.0.0"
    MAX_MATRIX_SIZE = 8192
  }
  
  warnings = "all"
}

# Optimization settings
optimization {
  level = 3
  size_speed = 0.8  # Prefer speed
  
  enable = [
    "inline",
    "vectorize",
    "unroll-loops",
    "function-specialization"
  ]
  
  # Specific optimizations for matrix operations
  function {
    "matrix_multiply" {
      level = 3
      enable = ["vectorize", "unroll-loops", "gpu-offload"]
      unroll_factor = 8
    }
  }
}

# Memory configuration
memory {
  model = "relaxed"
  
  allocation {
    default = "global"
    matrices = "shared"
    constants = "constant"
  }
  
  alignment {
    default = 16
    vectors = 32
    matrices = 64
  }
  
  safety {
    bounds_check = true
    null_check = true
  }
}

# Processing unit configuration
processing_units {
  cpu {
    class = "CPU_VECTOR"
    architecture = "x86_64"
    features = ["avx512f"]
    threads = 16
  }
  
  gpu {
    class = "GPU_COMPUTE"
    architecture = "cuda"
    compute_capability = "8.0"
    memory = 16384  # MB
  }
  
  distribution {
    small_matrices = "cpu"
    large_matrices = "gpu"
    threshold = 1024  # Size threshold for CPU vs GPU
  }
}

# Debug settings
debug {
  level = 1
  source_map = true
  symbols = true
}
```

## 8. Implementation Guidelines

When implementing a COIL configuration parser:

1. Parse the file in a single pass, building a hierarchical structure
2. Process includes early to create a merged configuration
3. Resolve variables after the entire configuration is loaded
4. Evaluate conditionals based on the target environment
5. Validate against a schema to catch configuration errors
6. Provide sensible defaults for missing values
7. Generate warnings for deprecated or invalid settings