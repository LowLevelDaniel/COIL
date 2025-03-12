/**
 * @file coil_assembler.c
 * @brief COIL assembler implementation
 */

#include "coil_assembler/coil_assembler.h"
#include "coil_assembler/binary_parser.h"
#include "coil_assembler/instruction_decoder.h"
#include "coil_assembler/translator.h"
#include "coil_assembler/optimizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief COIL assembler structure
 */
struct coil_assembler {
  error_context_t*     error_context;  /**< Error context */
  binary_parser_t*     parser;         /**< Binary parser */
  instruction_decoder_t* decoder;      /**< Instruction decoder */
  translator_t*        translator;     /**< Translator */
  optimizer_t*         optimizer;      /**< Optimizer */
  target_config_t*     target;         /**< Target configuration */
  module_t*            module;         /**< Assembled module */
  opt_level_t          opt_level;      /**< Optimization level */
  bool                 experimental;   /**< Experimental optimizations flag */
  bool                 dump_ir;        /**< Dump IR flag */
  bool                 verbose;        /**< Verbose mode */
  bool                 had_error;      /**< True if an error occurred */
};

coil_assembler_t* coil_create_assembler(error_context_t* error_context) {
  coil_assembler_t* assembler = (coil_assembler_t*)malloc(sizeof(coil_assembler_t));
  if (assembler == NULL) {
    return NULL;
  }
  
  assembler->error_context = error_context;
  assembler->parser = NULL;
  assembler->decoder = NULL;
  assembler->translator = NULL;
  assembler->optimizer = NULL;
  assembler->target = NULL;
  assembler->module = NULL;
  assembler->opt_level = OPT_LEVEL_NONE;
  assembler->experimental = false;
  assembler->dump_ir = false;
  assembler->verbose = false;
  assembler->had_error = false;
  
  // Create binary parser
  assembler->parser = coil_create_binary_parser(error_context);
  if (assembler->parser == NULL) {
    coil_free_assembler(assembler);
    return NULL;
  }
  
  // Create instruction decoder
  assembler->decoder = coil_create_instruction_decoder(error_context);
  if (assembler->decoder == NULL) {
    coil_free_assembler(assembler);
    return NULL;
  }
  
  // Create default target
  assembler->target = coil_create_default_target(error_context);
  if (assembler->target == NULL) {
    coil_free_assembler(assembler);
    return NULL;
  }
  
  // Create translator
  assembler->translator = coil_create_translator(assembler->target, error_context);
  if (assembler->translator == NULL) {
    coil_free_assembler(assembler);
    return NULL;
  }
  
  // Create optimizer
  assembler->optimizer = coil_create_optimizer(assembler->target, error_context);
  if (assembler->optimizer == NULL) {
    coil_free_assembler(assembler);
    return NULL;
  }
  
  return assembler;
}

void coil_free_assembler(coil_assembler_t* assembler) {
  if (assembler == NULL) {
    return;
  }
  
  if (assembler->parser != NULL) {
    coil_free_binary_parser(assembler->parser);
  }
  
  if (assembler->decoder != NULL) {
    coil_free_instruction_decoder(assembler->decoder);
  }
  
  if (assembler->translator != NULL) {
    coil_free_translator(assembler->translator);
  }
  
  if (assembler->optimizer != NULL) {
    coil_free_optimizer(assembler->optimizer);
  }
  
  if (assembler->target != NULL) {
    coil_free_target_config(assembler->target);
  }
  
  if (assembler->module != NULL) {
    coil_free_module(assembler->module);
  }
  
  free(assembler);
}

int coil_set_target(coil_assembler_t* assembler, const char* target_name) {
  if (assembler == NULL || target_name == NULL) {
    return -1;
  }
  
  // Free previous target
  if (assembler->target != NULL) {
    coil_free_target_config(assembler->target);
    assembler->target = NULL;
  }
  
  // For now, we only support the default target
  // In a real implementation, we would load the target configuration
  // based on the target name
  
  if (strcmp(target_name, "x86_64") == 0 ||
      strcmp(target_name, "x86_64_generic") == 0) {
    assembler->target = coil_create_default_target(assembler->error_context);
    if (assembler->target == NULL) {
      if (assembler->error_context != NULL) {
        REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                    ERROR_CATEGORY_ASSEMBLER, ERROR_ASSEMBLER_NO_TARGET,
                    "Failed to create target", NULL, 0, 0);
      }
      
      return -1;
    }
    
    // Update translator and optimizer
    if (assembler->translator != NULL) {
      coil_free_translator(assembler->translator);
    }
    
    assembler->translator = coil_create_translator(
      assembler->target,
      assembler->error_context
    );
    
    if (assembler->translator == NULL) {
      if (assembler->error_context != NULL) {
        REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                    ERROR_CATEGORY_ASSEMBLER, ERROR_GENERAL_OUT_OF_MEMORY,
                    "Out of memory", NULL, 0, 0);
      }
      
      return -1;
    }
    
    if (assembler->optimizer != NULL) {
      coil_free_optimizer(assembler->optimizer);
    }
    
    assembler->optimizer = coil_create_optimizer(
      assembler->target,
      assembler->error_context
    );
    
    if (assembler->optimizer == NULL) {
      if (assembler->error_context != NULL) {
        REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                    ERROR_CATEGORY_ASSEMBLER, ERROR_GENERAL_OUT_OF_MEMORY,
                    "Out of memory", NULL, 0, 0);
      }
      
      return -1;
    }
    
    return 0;
  } else {
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_ASSEMBLER, ERROR_ASSEMBLER_UNSUPPORTED,
                  "Unsupported target", NULL, 0, 0);
    }
    
    return -1;
  }
}

void coil_set_opt_level(coil_assembler_t* assembler, opt_level_t level) {
  if (assembler == NULL) {
    return;
  }
  
  assembler->opt_level = level;
  
  if (assembler->optimizer != NULL) {
    coil_set_opt_level(assembler->optimizer, level);
  }
  
  if (assembler->translator != NULL) {
    coil_set_optimization_level(assembler->translator, (uint32_t)level);
  }
}

void coil_enable_experimental(coil_assembler_t* assembler, bool enable) {
  if (assembler == NULL) {
    return;
  }
  
  assembler->experimental = enable;
  
  if (assembler->optimizer != NULL) {
    coil_enable_experimental(assembler->optimizer, enable);
  }
}

void coil_set_dump_ir(coil_assembler_t* assembler, bool dump_ir) {
  if (assembler == NULL) {
    return;
  }
  
  assembler->dump_ir = dump_ir;
}

void coil_set_verbose(coil_assembler_t* assembler, bool verbose) {
  if (assembler == NULL) {
    return;
  }
  
  assembler->verbose = verbose;
}

/**
 * @brief Dump a module to the console (for debugging)
 * 
 * @param assembler The assembler
 * @param module The module
 */
static void dump_module(coil_assembler_t* assembler, module_t* module) {
  if (assembler == NULL || module == NULL) {
    return;
  }
  
  printf("Module: %s\n", module->name ? module->name : "<unnamed>");
  
  printf("Functions: %u\n", module->function_count);
  for (uint32_t i = 0; i < module->function_count; i++) {
    function_t* func = module->functions[i];
    
    printf("  Function: %s\n", func->name ? func->name : "<unnamed>");
    printf("    External: %s\n", func->is_external ? "true" : "false");
    printf("    Blocks: %u\n", func->block_count);
    
    for (uint32_t j = 0; j < func->block_count; j++) {
      basic_block_t* block = func->blocks[j];
      
      printf("      Block: %s\n", block->name ? block->name : "<unnamed>");
      printf("        Instructions: %u\n", block->instr_count);
      
      for (uint32_t k = 0; k < block->instr_count; k++) {
        instruction_t* instr = block->instructions[k];
        
        char instr_str[256];
        coil_print_instruction(instr, instr_str, sizeof(instr_str));
        
        printf("          %s\n", instr_str);
      }
    }
  }
}

module_t* coil_assemble_file(coil_assembler_t* assembler, const char* filename) {
  if (assembler == NULL || filename == NULL) {
    return NULL;
  }
  
  if (assembler->target == NULL) {
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_ASSEMBLER, ERROR_ASSEMBLER_NO_TARGET,
                  "No target specified", NULL, 0, 0);
    }
    
    assembler->had_error = true;
    return NULL;
  }
  
  // Free previous module
  if (assembler->module != NULL) {
    coil_free_module(assembler->module);
    assembler->module = NULL;
  }
  
  // Parse binary file
  assembler->module = coil_parse_binary_file(assembler->parser, filename);
  
  if (assembler->module == NULL) {
    assembler->had_error = true;
    return NULL;
  }
  
  // Apply optimization if requested
  if (assembler->opt_level > OPT_LEVEL_NONE) {
    if (coil_optimize_module(assembler->optimizer, assembler->module) != 0) {
      assembler->had_error = true;
      return NULL;
    }
  }
  
  // Dump IR if requested
  if (assembler->dump_ir) {
    dump_module(assembler, assembler->module);
  }
  
  // Check target compatibility
  if (!coil_target_satisfies_requirements(
      assembler->target,
      assembler->module->target.required_features,
      assembler->module->target.required_count)) {
    
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_ASSEMBLER, ERROR_ASSEMBLER_NO_TARGET,
                  "Target does not satisfy module requirements", NULL, 0, 0);
    }
    
    assembler->had_error = true;
    return NULL;
  }
  
  if (assembler->verbose) {
    printf("Assembled module: %s\n", assembler->module->name ? assembler->module->name : "<unnamed>");
    printf("Functions: %u\n", assembler->module->function_count);
    printf("Globals: %u\n", assembler->module->global_count);
  }
  
  return assembler->module;
}

int coil_generate_native_code(
  coil_assembler_t* assembler,
  const char* output_filename
) {
  if (assembler == NULL || output_filename == NULL || assembler->module == NULL) {
    return -1;
  }
  
  // Create output file
  FILE* file = fopen(output_filename, "wb");
  if (file == NULL) {
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_SYSTEM, ERROR_SYSTEM_IO,
                  "Failed to open output file", NULL, 0, 0);
    }
    
    assembler->had_error = true;
    return -1;
  }
  
  // Create native code buffer
  native_buffer_t* buffer = coil_create_native_buffer(
    assembler->translator,
    1024  // Initial size
  );
  
  if (buffer == NULL) {
    fclose(file);
    
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_GENERAL, ERROR_GENERAL_OUT_OF_MEMORY,
                  "Out of memory", NULL, 0, 0);
    }
    
    assembler->had_error = true;
    return -1;
  }
  
  // Translate module to native code
  if (coil_translate_module(assembler->translator, assembler->module, buffer) != 0) {
    coil_free_native_buffer(buffer);
    fclose(file);
    
    assembler->had_error = true;
    return -1;
  }
  
  // Write native code to file
  if (coil_output_native_code(assembler->translator, buffer, file) != 0) {
    coil_free_native_buffer(buffer);
    fclose(file);
    
    assembler->had_error = true;
    return -1;
  }
  
  coil_free_native_buffer(buffer);
  fclose(file);
  
  if (assembler->verbose) {
    printf("Generated native code: %s\n", output_filename);
  }
  
  return 0;
}

int coil_generate_assembly(
  coil_assembler_t* assembler,
  const char* output_filename
) {
  if (assembler == NULL || output_filename == NULL || assembler->module == NULL) {
    return -1;
  }
  
  // Create output file
  FILE* file = fopen(output_filename, "w");
  if (file == NULL) {
    if (assembler->error_context != NULL) {
      REPORT_ERROR(assembler->error_context, ERROR_ERROR,
                  ERROR_CATEGORY_SYSTEM, ERROR_SYSTEM_IO,
                  "Failed to open output file", NULL, 0, 0);
    }
    
    assembler->had_error = true;
    return -1;
  }
  
  // Write assembly header
  fprintf(file, "# Assembly generated by COIL Assembler\n");
  fprintf(file, "# Module: %s\n\n", 
         assembler->module->name ? assembler->module->name : "<unnamed>");
  
  // Write assembly for each function
  for (uint32_t i = 0; i < assembler->module->function_count; i++) {
    function_t* func = assembler->module->functions[i];
    
    // Skip external functions
    if (func->is_external) {
      continue;
    }
    
    fprintf(file, ".globl %s\n", func->name);
    fprintf(file, "%s:\n", func->name);
    
    // Function prologue
    fprintf(file, "    pushq %%rbp\n");
    fprintf(file, "    movq %%rsp, %%rbp\n");
    
    // Generate assembly for each instruction
    for (uint32_t j = 0; j < func->block_count; j++) {
      basic_block_t* block = func->blocks[j];
      
      fprintf(file, "%s:\n", block->name);
      
      for (uint32_t k = 0; k < block->instr_count; k++) {
        instruction_t* instr = block->instructions[k];
        
        char asm_str[256];
        coil_generate_assembly(
          assembler->translator,
          instr,
          asm_str,
          sizeof(asm_str)
        );
        
        fprintf(file, "%s\n", asm_str);
      }
    }
    
    fprintf(file, "\n");
  }
  
  fclose(file);
  
  if (assembler->verbose) {
    printf("Generated assembly: %s\n", output_filename);
  }
  
  return 0;
}

bool coil_had_error(coil_assembler_t* assembler) {
  if (assembler == NULL) {
    return true;
  }
  
  return assembler->had_error;
}