/**
 * @file target.h
 * @brief Target architecture configuration for COIL assembler
 *
 * This file defines the target architecture configuration component
 * for the COIL assembler, which describes the capabilities and constraints
 * of the target hardware.
 */

#ifndef COIL_TARGET_H
#define COIL_TARGET_H

#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Target architecture structure
 */
typedef struct target_architecture target_architecture_t;

/**
 * @brief Target configuration structure
 */
typedef struct target_config target_config_t;

/**
 * @brief Memory ordering models
 */
typedef enum {
  MEMORY_MODEL_STRONG,       /**< Strong memory model (sequential consistency) */
  MEMORY_MODEL_ACQUIRE_RELEASE, /**< Acquire-release memory model */
  MEMORY_MODEL_RELAXED       /**< Relaxed memory model */
} memory_model_t;

/**
 * @brief Resource properties
 */
typedef struct {
  uint32_t registers;      /**< Available hardware registers */
  uint32_t vector_width;   /**< Native vector width in bits */
  uint32_t min_alignment;  /**< Minimum memory alignment */
  memory_model_t* memory_models; /**< Supported memory ordering models */
  uint32_t memory_model_count;  /**< Number of supported memory models */
} target_resources_t;

/**
 * @brief Memory properties
 */
typedef struct {
  uint32_t alignment;      /**< Alignment requirements */
  uint32_t page_size;      /**< Virtual memory page size */
  uint32_t cacheline_size; /**< Cache line size */
} target_memory_t;

/**
 * @brief Optimization properties
 */
typedef struct {
  uint32_t vector_threshold; /**< When to use vector operations */
  uint32_t unroll_factor;    /**< Default loop unrolling factor */
  bool     use_fma;          /**< Whether to use fused multiply-add */
} target_optimization_t;

/**
 * @brief Create a new target architecture
 * 
 * @param name Architecture name
 * @param error_context Error context
 * @return The new target architecture, or NULL on error
 */
target_architecture_t* coil_create_target_architecture(
  const char* name,
  error_context_t* error_context
);

/**
 * @brief Free a target architecture
 * 
 * @param arch The target architecture to free
 */
void coil_free_target_architecture(target_architecture_t* arch);

/**
 * @brief Create a new target configuration
 * 
 * @param arch The target architecture
 * @param name Configuration name
 * @return The new target configuration, or NULL on error
 */
target_config_t* coil_create_target_config(
  target_architecture_t* arch,
  const char* name
);

/**
 * @brief Free a target configuration
 * 
 * @param config The target configuration to free
 */
void coil_free_target_config(target_config_t* config);

/**
 * @brief Load a target configuration from a file
 * 
 * @param arch The target architecture
 * @param path Configuration file path
 * @return The loaded target configuration, or NULL on error
 */
target_config_t* coil_load_target_config(
  target_architecture_t* arch,
  const char* path
);

/**
 * @brief Set architecture vendor
 * 
 * @param arch The target architecture
 * @param vendor Vendor name
 * @return 0 on success, -1 on error
 */
int coil_set_architecture_vendor(
  target_architecture_t* arch,
  const char* vendor
);

/**
 * @brief Add architecture feature
 * 
 * @param arch The target architecture
 * @param feature Feature name
 * @param description Feature description
 * @return 0 on success, -1 on error
 */
int coil_add_architecture_feature(
  target_architecture_t* arch,
  const char* feature,
  const char* description
);

/**
 * @brief Set target resources
 * 
 * @param config The target configuration
 * @param resources Resource properties
 * @return 0 on success, -1 on error
 */
int coil_set_target_resources(
  target_config_t* config,
  const target_resources_t* resources
);

/**
 * @brief Set target memory properties
 * 
 * @param config The target configuration
 * @param memory Memory properties
 * @return 0 on success, -1 on error
 */
int coil_set_target_memory(
  target_config_t* config,
  const target_memory_t* memory
);

/**
 * @brief Set target optimization properties
 * 
 * @param config The target configuration
 * @param optimization Optimization properties
 * @return 0 on success, -1 on error
 */
int coil_set_target_optimization(
  target_config_t* config,
  const target_optimization_t* optimization
);

/**
 * @brief Add feature to target configuration
 * 
 * @param config The target configuration
 * @param feature Feature name
 * @return 0 on success, -1 on error
 */
int coil_add_target_feature(
  target_config_t* config,
  const char* feature
);

/**
 * @brief Check if target has a feature
 * 
 * @param config The target configuration
 * @param feature Feature name
 * @return true if the feature is supported, false otherwise
 */
bool coil_target_has_feature(
  const target_config_t* config,
  const char* feature
);

/**
 * @brief Get target architecture name
 * 
 * @param config The target configuration
 * @return The architecture name
 */
const char* coil_get_target_architecture(const target_config_t* config);

/**
 * @brief Get target vendor
 * 
 * @param config The target configuration
 * @return The vendor name
 */
const char* coil_get_target_vendor(const target_config_t* config);

/**
 * @brief Get target resources
 * 
 * @param config The target configuration
 * @return The resource properties
 */
const target_resources_t* coil_get_target_resources(const target_config_t* config);

/**
 * @brief Get target memory properties
 * 
 * @param config The target configuration
 * @return The memory properties
 */
const target_memory_t* coil_get_target_memory(const target_config_t* config);

/**
 * @brief Get target optimization properties
 * 
 * @param config The target configuration
 * @return The optimization properties
 */
const target_optimization_t* coil_get_target_optimization(const target_config_t* config);

/**
 * @brief Create default target configuration for current platform
 * 
 * @param error_context Error context
 * @return The target configuration, or NULL on error
 */
target_config_t* coil_create_default_target(error_context_t* error_context);

/**
 * @brief Check if target satisfies requirements
 * 
 * @param config The target configuration
 * @param required_features Required features
 * @param required_count Number of required features
 * @return true if all requirements are satisfied, false otherwise
 */
bool coil_target_satisfies_requirements(
  const target_config_t* config,
  const char** required_features,
  uint32_t required_count
);

/**
 * @brief Print target configuration
 * 
 * @param config The target configuration
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of bytes written, or -1 on error
 */
int coil_print_target_config(
  const target_config_t* config,
  char* buffer,
  size_t size
);

#endif /* COIL_TARGET_H */