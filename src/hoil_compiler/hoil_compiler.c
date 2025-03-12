/**
 * @file hoil_compiler.c
 * @brief HOIL compiler implementation
 */

#include "hoil_compiler/hoil_compiler.h"
#include "hoil_compiler/lexer.h"
#include "hoil_compiler/parser.h"
#include "hoil_compiler/semantic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief HOIL compiler structure
 */
struct hoil_compiler {
  error_context_t*      error_context;  /**< Error context */
  lexer_t*              lexer;          /**< Lexer */
  parser_t*             parser;         /**< Parser */
  semantic_analyzer_t*  analyzer;       /**< Semantic analyzer */
  module_t*             module;         /**< Compiled module */
  bool                  dump_ast;       /**< Dump AST flag */
  bool                  validate;       /**< Validate flag */
  bool                  optimize;       /**< Optimize flag */
  bool                  had_error;      /**< True if an error occurred */
};

hoil_compiler_t* hoil_create_compiler(error_context_t* error_context) {
  hoil_compiler_t* compiler = (hoil_compiler_t*)malloc(sizeof(hoil_compiler_t));
  if (compiler == NULL) {
    return NULL;
  }
  
  compiler->error_context = error_context;
  compiler->lexer = NULL;
  compiler->parser = NULL;
  compiler->analyzer = NULL;
  compiler->module = NULL;
  compiler->dump_ast = false;
  compiler->validate = true;
  compiler->optimize = false;
  compiler->had_error = false;
  
  // Create semantic analyzer
  compiler->analyzer = hoil_create_semantic_analyzer(error_context);
  if (compiler->analyzer == NULL) {
    hoil_free_compiler(compiler);
    return NULL;
  }
  
  return compiler;
}

void hoil_free_compiler(hoil_compiler_t* compiler) {
  if (compiler == NULL) {
    return;
  }
  
  if (compiler->lexer != NULL) {
    hoil_free_lexer(compiler->lexer);
  }
  
  if (compiler->parser != NULL) {
    hoil_free_parser(compiler->parser);
  }
  
  if (compiler->analyzer != NULL) {
    hoil_free_semantic_analyzer(compiler->analyzer);
  }
  
  if (compiler->module != NULL) {
    coil_free_module(compiler->module);
  }
  
  free(compiler);
}

void hoil_set_dump_ast(hoil_compiler_t* compiler, bool dump_ast) {
  if (compiler == NULL) {
    return;
  }
  
  compiler->dump_ast = dump_ast;
}

void hoil_set_validate(hoil_compiler_t* compiler, bool validate) {
  if (compiler == NULL) {
    return;
  }
  
  compiler->validate = validate;
}

void hoil_set_optimize(hoil_compiler_t* compiler, bool optimize) {
  if (compiler == NULL) {
    return;
  }
  
  compiler->optimize = optimize;
}

/**
 * @brief Dump a type to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module containing the type registry
 * @param type Type to dump
 * @param indent Indentation level
 */
static void dump_type(FILE* file, module_t* module, coil_type_t type, int indent) {
  if (file == NULL || module == NULL) {
    return;
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  // Determine type category
  type_category_t category = coil_get_type_category(type);
  
  switch (category) {
    case TYPE_CATEGORY_VOID:
      fprintf(file, "void");
      break;
      
    case TYPE_CATEGORY_BOOLEAN:
      fprintf(file, "bool");
      break;
      
    case TYPE_CATEGORY_INTEGER: {
      uint8_t width = coil_get_type_width(type);
      uint8_t qualifiers = coil_get_type_qualifiers(type);
      
      if (qualifiers & TYPE_QUALIFIER_UNSIGNED) {
        fprintf(file, "u%u", width);
      } else {
        fprintf(file, "i%u", width);
      }
      break;
    }
      
    case TYPE_CATEGORY_FLOAT: {
      uint8_t width = coil_get_type_width(type);
      fprintf(file, "f%u", width);
      break;
    }
      
    case TYPE_CATEGORY_POINTER: {
      coil_type_t base_type = coil_get_pointer_base_type(type);
      fprintf(file, "ptr<");
      dump_type(file, module, base_type, 0);
      
      // Check for qualifiers
      uint8_t qualifiers = coil_get_type_qualifiers(type);
      if (qualifiers != 0) {
        fprintf(file, ", ");
        
        if (qualifiers & TYPE_QUALIFIER_CONST) {
          fprintf(file, "const");
        } else if (qualifiers & TYPE_QUALIFIER_VOLATILE) {
          fprintf(file, "volatile");
        } else if (qualifiers & TYPE_QUALIFIER_RESTRICT) {
          fprintf(file, "restrict");
        }
      }
      
      fprintf(file, ">");
      break;
    }
      
    case TYPE_CATEGORY_VECTOR: {
      coil_type_t elem_type = coil_get_element_type(module->type_registry, type);
      uint8_t elem_count = coil_get_type_width(type);
      
      fprintf(file, "vec<");
      dump_type(file, module, elem_type, 0);
      fprintf(file, ", %u>", elem_count);
      break;
    }
      
    case TYPE_CATEGORY_ARRAY: {
      coil_type_t elem_type = coil_get_element_type(module->type_registry, type);
      uint8_t elem_count = coil_get_type_width(type);
      
      fprintf(file, "array<");
      dump_type(file, module, elem_type, 0);
      
      if (elem_count > 0) {
        fprintf(file, ", %u>", elem_count);
      } else {
        fprintf(file, ">");
      }
      break;
    }
      
    case TYPE_CATEGORY_STRUCT: {
      const struct_type_t* struct_info = coil_get_struct_info(module->type_registry, type);
      
      if (struct_info != NULL && struct_info->name != NULL) {
        fprintf(file, "%s", struct_info->name);
      } else {
        fprintf(file, "struct");
      }
      break;
    }
      
    case TYPE_CATEGORY_FUNCTION: {
      const function_type_t* func_info = coil_get_function_info(module->type_registry, type);
      
      fprintf(file, "function(");
      
      if (func_info != NULL) {
        for (uint32_t i = 0; i < func_info->param_count; i++) {
          if (i > 0) {
            fprintf(file, ", ");
          }
          
          dump_type(file, module, func_info->params[i].type, 0);
        }
        
        if (func_info->is_variadic) {
          if (func_info->param_count > 0) {
            fprintf(file, ", ");
          }
          fprintf(file, "...");
        }
        
        fprintf(file, ") -> ");
        dump_type(file, module, func_info->return_type, 0);
      } else {
        fprintf(file, ") -> void");
      }
      break;
    }
      
    default:
      fprintf(file, "unknown_type");
      break;
  }
}

/**
 * @brief Dump an operand to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module
 * @param operand Operand to dump
 */
static void dump_operand(FILE* file, module_t* module, instruction_operand_t* operand) {
  if (file == NULL || module == NULL || operand == NULL) {
    return;
  }
  
  switch (operand->kind) {
    case OPERAND_REGISTER:
      fprintf(file, "r%u", operand->value.reg);
      break;
      
    case OPERAND_IMMEDIATE:
      fprintf(file, "%lld", (long long)operand->value.imm);
      break;
      
    case OPERAND_BASIC_BLOCK:
      fprintf(file, "%s", operand->value.block);
      break;
      
    case OPERAND_FUNCTION:
      fprintf(file, "%s", operand->value.func);
      break;
      
    case OPERAND_GLOBAL:
      fprintf(file, "%s", operand->value.global);
      break;
      
    case OPERAND_MEMORY:
      fprintf(file, "[r%u", operand->value.mem.base);
      
      if (operand->value.mem.index != 0) {
        fprintf(file, " + r%u * %u",
               operand->value.mem.index,
               operand->value.mem.scale);
      }
      
      if (operand->value.mem.offset != 0) {
        fprintf(file, " + %d", operand->value.mem.offset);
      }
      
      fprintf(file, "]");
      break;
      
    default:
      fprintf(file, "unknown_operand");
      break;
  }
}

/**
 * @brief Dump an instruction to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module
 * @param instr Instruction to dump
 * @param indent Indentation level
 */
static void dump_instruction(FILE* file, module_t* module, instruction_t* instr, int indent) {
  if (file == NULL || module == NULL || instr == NULL) {
    return;
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  // Get opcode mnemonic
  const char* mnemonic = coil_get_opcode_mnemonic((uint8_t)instr->opcode);
  
  // Print destination if present
  if (instr->dest.kind != OPERAND_REGISTER || instr->dest.value.reg != 0) {
    if (instr->result_name != NULL) {
      fprintf(file, "%s = ", instr->result_name);
    } else {
      fprintf(file, "result = ");
    }
  }
  
  // Print opcode
  fprintf(file, "%s", mnemonic ? mnemonic : "UNKNOWN");
  
  // Print operands
  if (instr->operand_count > 0) {
    fprintf(file, " ");
    
    for (uint8_t i = 0; i < instr->operand_count; i++) {
      if (i > 0) {
        fprintf(file, ", ");
      }
      
      dump_operand(file, module, &instr->operands[i]);
    }
  }
  
  fprintf(file, ";\n");
}

/**
 * @brief Dump a basic block to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module
 * @param block Basic block to dump
 * @param indent Indentation level
 */
static void dump_basic_block(FILE* file, module_t* module, basic_block_t* block, int indent) {
  if (file == NULL || module == NULL || block == NULL) {
    return;
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  // Print block name
  fprintf(file, "%s:\n", block->name);
  
  // Print instructions
  for (uint32_t i = 0; i < block->instr_count; i++) {
    dump_instruction(file, module, block->instructions[i], indent + 1);
  }
}

/**
 * @brief Dump a function to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module
 * @param function Function to dump
 * @param indent Indentation level
 */
static void dump_function(FILE* file, module_t* module, function_t* function, int indent) {
  if (file == NULL || module == NULL || function == NULL) {
    return;
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  // Print function declaration
  if (function->is_external) {
    fprintf(file, "EXTERN ");
  }
  
  fprintf(file, "FUNCTION %s(", function->name);
  
  // Print parameters
  for (uint32_t i = 0; i < function->param_count; i++) {
    if (i > 0) {
      fprintf(file, ", ");
    }
    
    fprintf(file, "%s: ", function->parameters[i].name);
    dump_type(file, module, function->parameters[i].type, 0);
  }
  
  if (function->is_variadic) {
    if (function->param_count > 0) {
      fprintf(file, ", ");
    }
    fprintf(file, "...");
  }
  
  fprintf(file, ") -> ");
  dump_type(file, module, function->return_type, 0);
  
  if (function->is_external) {
    fprintf(file, ";\n");
    return;
  }
  
  fprintf(file, " {\n");
  
  // Print basic blocks
  for (uint32_t i = 0; i < function->block_count; i++) {
    dump_basic_block(file, module, function->blocks[i], indent + 1);
    
    if (i < function->block_count - 1) {
      fprintf(file, "\n");
    }
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  fprintf(file, "}\n");
}

/**
 * @brief Dump a global variable to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module
 * @param global Global variable to dump
 * @param indent Indentation level
 */
static void dump_global(FILE* file, module_t* module, global_variable_t* global, int indent) {
  if (file == NULL || module == NULL || global == NULL) {
    return;
  }
  
  // Print indentation
  for (int i = 0; i < indent; i++) {
    fprintf(file, "  ");
  }
  
  // Print global declaration
  if (global->is_constant) {
    fprintf(file, "CONSTANT ");
  } else {
    fprintf(file, "GLOBAL ");
  }
  
  fprintf(file, "%s: ", global->name);
  dump_type(file, module, global->type, 0);
  
  if (global->is_external) {
    fprintf(file, ";\n");
    return;
  }
  
  if (global->initializer != NULL) {
    fprintf(file, " = /* initializer */");
  }
  
  fprintf(file, ";\n");
}

/**
 * @brief Dump a module to a file (for debugging)
 * 
 * @param file Output file
 * @param module Module to dump
 */
static void dump_module(FILE* file, module_t* module) {
  if (file == NULL || module == NULL) {
    return;
  }
  
  // Print module declaration
  fprintf(file, "MODULE \"%s\";\n\n", module->name ? module->name : "");
  
  // Print target requirements
  if (module->target.required_count > 0 || 
      module->target.preferred_count > 0 || 
      module->target.device_class != NULL) {
    
    fprintf(file, "TARGET {\n");
    
    if (module->target.required_count > 0) {
      fprintf(file, "  required_features = [");
      
      for (uint32_t i = 0; i < module->target.required_count; i++) {
        if (i > 0) {
          fprintf(file, ", ");
        }
        
        fprintf(file, "\"%s\"", module->target.required_features[i]);
      }
      
      fprintf(file, "]\n");
    }
    
    if (module->target.preferred_count > 0) {
      fprintf(file, "  preferred_features = [");
      
      for (uint32_t i = 0; i < module->target.preferred_count; i++) {
        if (i > 0) {
          fprintf(file, ", ");
        }
        
        fprintf(file, "\"%s\"", module->target.preferred_features[i]);
      }
      
      fprintf(file, "]\n");
    }
    
    if (module->target.device_class != NULL) {
      fprintf(file, "  device_class = \"%s\"\n", module->target.device_class);
    }
    
    fprintf(file, "}\n\n");
  }
  
  // Print global variables
  for (uint32_t i = 0; i < module->global_count; i++) {
    dump_global(file, module, module->globals[i], 0);
  }
  
  if (module->global_count > 0) {
    fprintf(file, "\n");
  }
  
  // Print functions
  for (uint32_t i = 0; i < module->function_count; i++) {
    dump_function(file, module, module->functions[i], 0);
    
    if (i < module->function_count - 1) {
      fprintf(file, "\n");
    }
  }
}

module_t* hoil_compile_string(
  hoil_compiler_t* compiler,
  const char* source,
  size_t source_length,
  const char* filename
) {
  if (compiler == NULL || source == NULL) {
    return NULL;
  }
  
  compiler->had_error = false;
  
  // Create lexer
  if (compiler->lexer != NULL) {
    hoil_free_lexer(compiler->lexer);
  }
  
  compiler->lexer = hoil_create_lexer(
    source,
    source_length,
    filename,
    compiler->error_context
  );
  
  if (compiler->lexer == NULL) {
    compiler->had_error = true;
    return NULL;
  }
  
  // Create parser
  if (compiler->parser != NULL) {
    hoil_free_parser(compiler->parser);
  }
  
  compiler->parser = hoil_create_parser(
    compiler->lexer,
    compiler->error_context
  );
  
  if (compiler->parser == NULL) {
    compiler->had_error = true;
    return NULL;
  }
  
  // Parse module
  if (compiler->module != NULL) {
    coil_free_module(compiler->module);
    compiler->module = NULL;
  }
  
  compiler->module = hoil_parse_module(compiler->parser);
  
  if (compiler->module == NULL) {
    compiler->had_error = true;
    return NULL;
  }
  
  // Validate module
  if (compiler->validate) {
    if (hoil_analyze_module(compiler->analyzer, compiler->module) != 0) {
      compiler->had_error = true;
      return NULL;
    }
  }
  
  // Dump AST
  if (compiler->dump_ast) {
    dump_module(stdout, compiler->module);
  }
  
  // TODO: Optimize if requested
  
  return compiler->module;
}

module_t* hoil_compile_file(
  hoil_compiler_t* compiler,
  const char* filename
) {
  if (compiler == NULL || filename == NULL) {
    return NULL;
  }
  
  compiler->had_error = false;
  
  // Open file
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_SYSTEM,
                  ERROR_SYSTEM_IO, "Failed to open file", filename, 0, 0);
    }
    
    compiler->had_error = true;
    return NULL;
  }
  
  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (file_size <= 0) {
    fclose(file);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_SYSTEM,
                  ERROR_SYSTEM_IO, "Failed to get file size or empty file",
                  filename, 0, 0);
    }
    
    compiler->had_error = true;
    return NULL;
  }
  
  // Allocate buffer
  char* source = (char*)malloc(file_size + 1);
  if (source == NULL) {
    fclose(file);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_GENERAL,
                  ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory", NULL, 0, 0);
    }
    
    compiler->had_error = true;
    return NULL;
  }
  
  // Read file
  size_t bytes_read = fread(source, 1, file_size, file);
  fclose(file);
  
  if (bytes_read != (size_t)file_size) {
    free(source);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_SYSTEM,
                  ERROR_SYSTEM_IO, "Failed to read file", filename, 0, 0);
    }
    
    compiler->had_error = true;
    return NULL;
  }
  
  // Null-terminate
  source[file_size] = '\0';
  
  // Compile string
  module_t* module = hoil_compile_string(
    compiler,
    source,
    file_size,
    filename
  );
  
  free(source);
  
  return module;
}

int hoil_write_binary(hoil_compiler_t* compiler, const char* filename) {
  if (compiler == NULL || filename == NULL || compiler->module == NULL) {
    return -1;
  }
  
  // Open file
  FILE* file = fopen(filename, "wb");
  if (file == NULL) {
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_SYSTEM,
                  ERROR_SYSTEM_IO, "Failed to open file for writing",
                  filename, 0, 0);
    }
    
    return -1;
  }
  
  // Allocate buffer for binary data
  uint8_t* buffer = (uint8_t*)malloc(1024);  // Initial size
  if (buffer == NULL) {
    fclose(file);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_GENERAL,
                  ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory", NULL, 0, 0);
    }
    
    return -1;
  }
  
  // Convert module to binary
  int size = coil_module_to_binary(compiler->module, buffer, 1024);
  if (size < 0) {
    free(buffer);
    fclose(file);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_CODEGEN,
                  ERROR_CODEGEN_INVALID_IR, "Failed to convert module to binary",
                  NULL, 0, 0);
    }
    
    return -1;
  }
  
  // Write to file
  if (fwrite(buffer, 1, size, file) != (size_t)size) {
    free(buffer);
    fclose(file);
    
    if (compiler->error_context != NULL) {
      REPORT_ERROR(compiler->error_context, ERROR_ERROR, ERROR_CATEGORY_SYSTEM,
                  ERROR_SYSTEM_IO, "Failed to write to file", filename, 0, 0);
    }
    
    return -1;
  }
  
  free(buffer);
  fclose(file);
  
  return 0;
}

bool hoil_had_error(hoil_compiler_t* compiler) {
  if (compiler == NULL) {
    return true;
  }
  
  return compiler->had_error;
}