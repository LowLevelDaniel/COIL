/**
 * @file target.c
 * @brief Target architecture configuration implementation for COIL assembler
 */

#include "coil_assembler/target.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Feature definition
 */
typedef struct {
  char*       name;         /**< Feature name */
  char*       description;  /**< Feature description */
} feature_t;

/**
 * @brief Target architecture structure
 */
struct target_architecture {
  char*       name;          /**< Architecture name */
  char*       vendor;        /**< Vendor name */
  feature_t*  features;      /**< Available features */
  uint32_t    feature_count; /**< Number of features */
  uint32_t    feature_capacity; /**< Feature array capacity */
  error_context_t* error_context; /**< Error context */
};

/**
 * @brief Target configuration structure
 */
struct target_config {
  target_architecture_t* architecture; /**< Target architecture */
  char*                 name;         /**< Configuration name */
  char**                features;     /**< Enabled features */
  uint32_t              feature_count; /**< Number of enabled features */
  uint32_t              feature_capacity; /**< Feature array capacity */
  target_resources_t    resources;    /**< Resource properties */
  target_memory_t       memory;       /**< Memory properties */
  target_optimization_t optimization; /**< Optimization properties */
};

/**
 * @brief Report a target error
 * 
 * @param arch The target architecture
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  target_architecture_t* arch,
  uint32_t code,
  const char* message
) {
  if (arch->error_context != NULL) {
    REPORT_ERROR(arch->error_context, ERROR_ERROR, ERROR_CATEGORY_ASSEMBLER,
                code, message, NULL, 0, 0);
  }
}

target_architecture_t* coil_create_target_architecture(
  const char* name,
  error_context_t* error_context
) {
  if (name == NULL) {
    return NULL;
  }
  
  target_architecture_t* arch = (target_architecture_t*)malloc(
    sizeof(target_architecture_t)
  );
  
  if (arch == NULL) {
    return NULL;
  }
  
  arch->name = strdup(name);
  if (arch->name == NULL) {
    free(arch);
    return NULL;
  }
  
  arch->vendor = NULL;
  arch->features = NULL;
  arch->feature_count = 0;
  arch->feature_capacity = 0;
  arch->error_context = error_context;
  
  return arch;
}

void coil_free_target_architecture(target_architecture_t* arch) {
  if (arch == NULL) {
    return;
  }
  
  if (arch->name != NULL) {
    free(arch->name);
  }
  
  if (arch->vendor != NULL) {
    free(arch->vendor);
  }
  
  if (arch->features != NULL) {
    for (uint32_t i = 0; i < arch->feature_count; i++) {
      if (arch->features[i].name != NULL) {
        free(arch->features[i].name);
      }
      if (arch->features[i].description != NULL) {
        free(arch->features[i].description);
      }
    }
    free(arch->features);
  }
  
  free(arch);
}

target_config_t* coil_create_target_config(
  target_architecture_t* arch,
  const char* name
) {
  if (arch == NULL || name == NULL) {
    return NULL;
  }
  
  target_config_t* config = (target_config_t*)malloc(
    sizeof(target_config_t)
  );
  
  if (config == NULL) {
    return NULL;
  }
  
  config->architecture = arch;
  config->name = strdup(name);
  if (config->name == NULL) {
    free(config);
    return NULL;
  }
  
  config->features = NULL;
  config->feature_count = 0;
  config->feature_capacity = 0;
  
  // Initialize with default values
  memset(&config->resources, 0, sizeof(target_resources_t));
  memset(&config->memory, 0, sizeof(target_memory_t));
  memset(&config->optimization, 0, sizeof(target_optimization_t));
  
  return config;
}

void coil_free_target_config(target_config_t* config) {
  if (config == NULL) {
    return;
  }
  
  if (config->name != NULL) {
    free(config->name);
  }
  
  if (config->features != NULL) {
    for (uint32_t i = 0; i < config->feature_count; i++) {
      if (config->features[i] != NULL) {
        free(config->features[i]);
      }
    }
    free(config->features);
  }
  
  if (config->resources.memory_models != NULL) {
    free(config->resources.memory_models);
  }
  
  free(config);
}

target_config_t* coil_load_target_config(
  target_architecture_t* arch,
  const char* path
) {
  // Not yet implemented
  report_error(arch, ERROR_GENERAL_NOT_IMPLEMENTED,
              "Configuration loading not yet implemented");
  return NULL;
}

int coil_set_architecture_vendor(
  target_architecture_t* arch,
  const char* vendor
) {
  if (arch == NULL || vendor == NULL) {
    return -1;
  }
  
  if (arch->vendor != NULL) {
    free(arch->vendor);
  }
  
  arch->vendor = strdup(vendor);
  if (arch->vendor == NULL) {
    return -1;
  }
  
  return 0;
}

int coil_add_architecture_feature(
  target_architecture_t* arch,
  const char* feature,
  const char* description
) {
  if (arch == NULL || feature == NULL) {
    return -1;
  }
  
  // Check for duplicate
  for (uint32_t i = 0; i < arch->feature_count; i++) {
    if (strcmp(arch->features[i].name, feature) == 0) {
      if (description != NULL) {
        // Update description
        if (arch->features[i].description != NULL) {
          free(arch->features[i].description);
        }
        arch->features[i].description = strdup(description);
        if (arch->features[i].description == NULL) {
          return -1;
        }
      }
      return 0;
    }
  }
  
  // Resize features array if needed
  if (arch->feature_count >= arch->feature_capacity) {
    uint32_t new_capacity = arch->feature_capacity == 0 ? 8 : arch->feature_capacity * 2;
    feature_t* new_features = (feature_t*)realloc(
      arch->features,
      new_capacity * sizeof(feature_t)
    );
    
    if (new_features == NULL) {
      return -1;
    }
    
    arch->features = new_features;
    arch->feature_capacity = new_capacity;
  }
  
  // Add feature
  arch->features[arch->feature_count].name = strdup(feature);
  if (arch->features[arch->feature_count].name == NULL) {
    return -1;
  }
  
  if (description != NULL) {
    arch->features[arch->feature_count].description = strdup(description);
    if (arch->features[arch->feature_count].description == NULL) {
      free(arch->features[arch->feature_count].name);
      return -1;
    }
  } else {
    arch->features[arch->feature_count].description = NULL;
  }
  
  arch->feature_count++;
  
  return 0;
}

int coil_set_target_resources(
  target_config_t* config,
  const target_resources_t* resources
) {
  if (config == NULL || resources == NULL) {
    return -1;
  }
  
  // Free any existing memory models
  if (config->resources.memory_models != NULL) {
    free(config->resources.memory_models);
  }
  
  // Copy resources
  config->resources = *resources;
  
  // Create a copy of memory models
  if (resources->memory_model_count > 0 && resources->memory_models != NULL) {
    config->resources.memory_models = (memory_model_t*)malloc(
      resources->memory_model_count * sizeof(memory_model_t)
    );
    
    if (config->resources.memory_models == NULL) {
      config->resources.memory_model_count = 0;
      return -1;
    }
    
    memcpy(
      config->resources.memory_models,
      resources->memory_models,
      resources->memory_model_count * sizeof(memory_model_t)
    );
  } else {
    config->resources.memory_models = NULL;
    config->resources.memory_model_count = 0;
  }
  
  return 0;
}

int coil_set_target_memory(
  target_config_t* config,
  const target_memory_t* memory
) {
  if (config == NULL || memory == NULL) {
    return -1;
  }
  
  config->memory = *memory;
  
  return 0;
}

int coil_set_target_optimization(
  target_config_t* config,
  const target_optimization_t* optimization
) {
  if (config == NULL || optimization == NULL) {
    return -1;
  }
  
  config->optimization = *optimization;
  
  return 0;
}

int coil_add_target_feature(
  target_config_t* config,
  const char* feature
) {
  if (config == NULL || feature == NULL) {
    return -1;
  }
  
  // Check if the feature exists in the architecture
  bool found = false;
  for (uint32_t i = 0; i < config->architecture->feature_count; i++) {
    if (strcmp(config->architecture->features[i].name, feature) == 0) {
      found = true;
      break;
    }
  }
  
  if (!found) {
    return -1;  // Feature not defined in the architecture
  }
  
  // Check for duplicate
  for (uint32_t i = 0; i < config->feature_count; i++) {
    if (strcmp(config->features[i], feature) == 0) {
      return 0;  // Already enabled
    }
  }
  
  // Resize features array if needed
  if (config->feature_count >= config->feature_capacity) {
    uint32_t new_capacity = config->feature_capacity == 0 ? 8 : config->feature_capacity * 2;
    char** new_features = (char**)realloc(
      config->features,
      new_capacity * sizeof(char*)
    );
    
    if (new_features == NULL) {
      return -1;
    }
    
    config->features = new_features;
    config->feature_capacity = new_capacity;
  }
  
  // Add feature
  config->features[config->feature_count] = strdup(feature);
  if (config->features[config->feature_count] == NULL) {
    return -1;
  }
  
  config->feature_count++;
  
  return 0;
}

bool coil_target_has_feature(
  const target_config_t* config,
  const char* feature
) {
  if (config == NULL || feature == NULL) {
    return false;
  }
  
  for (uint32_t i = 0; i < config->feature_count; i++) {
    if (strcmp(config->features[i], feature) == 0) {
      return true;
    }
  }
  
  return false;
}

const char* coil_get_target_architecture(const target_config_t* config) {
  if (config == NULL || config->architecture == NULL) {
    return NULL;
  }
  
  return config->architecture->name;
}

const char* coil_get_target_vendor(const target_config_t* config) {
  if (config == NULL || config->architecture == NULL) {
    return NULL;
  }
  
  return config->architecture->vendor;
}

const target_resources_t* coil_get_target_resources(const target_config_t* config) {
  if (config == NULL) {
    return NULL;
  }
  
  return &config->resources;
}

const target_memory_t* coil_get_target_memory(const target_config_t* config) {
  if (config == NULL) {
    return NULL;
  }
  
  return &config->memory;
}

const target_optimization_t* coil_get_target_optimization(const target_config_t* config) {
  if (config == NULL) {
    return NULL;
  }
  
  return &config->optimization;
}

target_config_t* coil_create_default_target(error_context_t* error_context) {
  // Create x86_64 architecture
  target_architecture_t* arch = coil_create_target_architecture(
    "x86_64",
    error_context
  );
  
  if (arch == NULL) {
    return NULL;
  }
  
  // Set vendor
  coil_set_architecture_vendor(arch, "generic");
  
  // Add features
  coil_add_architecture_feature(arch, "sse", "Streaming SIMD Extensions");
  coil_add_architecture_feature(arch, "sse2", "Streaming SIMD Extensions 2");
  coil_add_architecture_feature(arch, "sse3", "Streaming SIMD Extensions 3");
  coil_add_architecture_feature(arch, "ssse3", "Supplemental SSE3");
  coil_add_architecture_feature(arch, "sse4.1", "Streaming SIMD Extensions 4.1");
  coil_add_architecture_feature(arch, "sse4.2", "Streaming SIMD Extensions 4.2");
  coil_add_architecture_feature(arch, "avx", "Advanced Vector Extensions");
  coil_add_architecture_feature(arch, "avx2", "Advanced Vector Extensions 2");
  coil_add_architecture_feature(arch, "fma", "Fused Multiply-Add");
  coil_add_architecture_feature(arch, "popcnt", "POPCNT instruction");
  coil_add_architecture_feature(arch, "aes", "AES instruction set");
  coil_add_architecture_feature(arch, "pclmul", "PCLMULQDQ instruction");
  
  // Create target configuration
  target_config_t* config = coil_create_target_config(arch, "x86_64_generic");
  if (config == NULL) {
    coil_free_target_architecture(arch);
    return NULL;
  }
  
  // Add basic features
  coil_add_target_feature(config, "sse");
  coil_add_target_feature(config, "sse2");
  
  // Set resources
  target_resources_t resources;
  memory_model_t memory_models[] = {
    MEMORY_MODEL_STRONG,
    MEMORY_MODEL_ACQUIRE_RELEASE,
    MEMORY_MODEL_RELAXED
  };
  
  resources.registers = 16;  // x86_64 has 16 general-purpose registers
  resources.vector_width = 128;  // SSE/SSE2 has 128-bit vectors
  resources.min_alignment = 16;  // 16-byte alignment for SSE
  resources.memory_models = memory_models;
  resources.memory_model_count = sizeof(memory_models) / sizeof(memory_models[0]);
  
  coil_set_target_resources(config, &resources);
  
  // Set memory properties
  target_memory_t memory;
  memory.alignment = 8;  // 8-byte alignment for 64-bit
  memory.page_size = 4096;  // 4KB pages
  memory.cacheline_size = 64;  // 64-byte cache lines
  
  coil_set_target_memory(config, &memory);
  
  // Set optimization properties
  target_optimization_t optimization;
  optimization.vector_threshold = 4;  // Use vectors for 4+ elements
  optimization.unroll_factor = 4;  // Unroll loops by factor of 4
  optimization.use_fma = false;  // FMA not enabled by default
  
  coil_set_target_optimization(config, &optimization);
  
  return config;
}

bool coil_target_satisfies_requirements(
  const target_config_t* config,
  const char** required_features,
  uint32_t required_count
) {
  if (config == NULL) {
    return false;
  }
  
  // No requirements
  if (required_features == NULL || required_count == 0) {
    return true;
  }
  
  // Check that all required features are enabled
  for (uint32_t i = 0; i < required_count; i++) {
    if (!coil_target_has_feature(config, required_features[i])) {
      return false;
    }
  }
  
  return true;
}

int coil_print_target_config(
  const target_config_t* config,
  char* buffer,
  size_t size
) {
  if (config == NULL || buffer == NULL || size == 0) {
    return -1;
  }
  
  int len = snprintf(buffer, size, 
                    "Target: %s (%s)\n",
                    config->name,
                    config->architecture->name);
  
  if (len < 0 || (size_t)len >= size) {
    return -1;
  }
  
  // Add vendor
  if (config->architecture->vendor != NULL) {
    int vendor_len = snprintf(buffer + len, size - len,
                            "Vendor: %s\n",
                            config->architecture->vendor);
    
    if (vendor_len < 0 || (size_t)(len + vendor_len) >= size) {
      return -1;
    }
    
    len += vendor_len;
  }
  
  // Add enabled features
  int feat_len = snprintf(buffer + len, size - len, "Features: ");
  if (feat_len < 0 || (size_t)(len + feat_len) >= size) {
    return -1;
  }
  
  len += feat_len;
  
  for (uint32_t i = 0; i < config->feature_count; i++) {
    int feature_len = snprintf(buffer + len, size - len,
                             "%s%s",
                             config->features[i],
                             i < config->feature_count - 1 ? ", " : "");
    
    if (feature_len < 0 || (size_t)(len + feature_len) >= size) {
      return -1;
    }
    
    len += feature_len;
  }
  
  // Add newline
  if (len + 1 < size) {
    buffer[len++] = '\n';
    buffer[len] = '\0';
  } else {
    return -1;
  }
  
  // Add resources
  int res_len = snprintf(buffer + len, size - len,
                       "Resources:\n"
                       "  Registers: %u\n"
                       "  Vector width: %u bits\n"
                       "  Min alignment: %u bytes\n",
                       config->resources.registers,
                       config->resources.vector_width,
                       config->resources.min_alignment);
  
  if (res_len < 0 || (size_t)(len + res_len) >= size) {
    return -1;
  }
  
  len += res_len;
  
  // Add memory properties
  int mem_len = snprintf(buffer + len, size - len,
                       "Memory:\n"
                       "  Alignment: %u bytes\n"
                       "  Page size: %u bytes\n"
                       "  Cache line: %u bytes\n",
                       config->memory.alignment,
                       config->memory.page_size,
                       config->memory.cacheline_size);
  
  if (mem_len < 0 || (size_t)(len + mem_len) >= size) {
    return -1;
  }
  
  len += mem_len;
  
  // Add optimization properties
  int opt_len = snprintf(buffer + len, size - len,
                       "Optimization:\n"
                       "  Vector threshold: %u\n"
                       "  Unroll factor: %u\n"
                       "  Use FMA: %s\n",
                       config->optimization.vector_threshold,
                       config->optimization.unroll_factor,
                       config->optimization.use_fma ? "yes" : "no");
  
  if (opt_len < 0 || (size_t)(len + opt_len) >= size) {
    return -1;
  }
  
  len += opt_len;
  
  return len;
}