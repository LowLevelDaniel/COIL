/**
 * @file hoilc.c
 * @brief HOIL compiler command-line tool
 * 
 * This is the main entry point for the HOIL compiler, which converts
 * HOIL source code to COIL binary format.
 */

#include "common/error.h"
#include "common/module.h"
#include "hoil_compiler/lexer.h"
#include "hoil_compiler/parser.h"
#include "hoil_compiler/hoil_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Program options
 */
typedef struct {
  const char* input_file;       /**< Input file path */
  const char* output_file;      /**< Output file path */
  bool        verbose;          /**< Verbose output */
  int         optimization;     /**< Optimization level (0-3) */
} options_t;

/**
 * @brief Print usage information
 * 
 * @param program_name Name of the program
 */
static void print_usage(const char* program_name) {
  printf("Usage: %s [options] <input.hoil>\n", program_name);
  printf("Options:\n");
  printf("  -o <file>     Output file (default: input.coil)\n");
  printf("  -O<level>     Optimization level (0-3, default: 0)\n");
  printf("  -v            Verbose output\n");
  printf("  -h, --help    Show this help message\n");
}

/**
 * @brief Parse command-line arguments
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @param options Output options
 * @return true if successful, false otherwise
 */
static bool parse_arguments(int argc, char** argv, options_t* options) {
  // Initialize options with defaults
  options->input_file = NULL;
  options->output_file = NULL;
  options->verbose = false;
  options->optimization = 0;
  
  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      // Option
      if (strcmp(argv[i], "-o") == 0) {
        // Output file
        if (i + 1 >= argc) {
          fprintf(stderr, "Error: Missing output file after -o\n");
          return false;
        }
        options->output_file = argv[++i];
      } else if (strncmp(argv[i], "-O", 2) == 0) {
        // Optimization level
        if (argv[i][2] >= '0' && argv[i][2] <= '3') {
          options->optimization = argv[i][2] - '0';
        } else {
          fprintf(stderr, "Error: Invalid optimization level: %c\n", argv[i][2]);
          return false;
        }
      } else if (strcmp(argv[i], "-v") == 0) {
        // Verbose output
        options->verbose = true;
      } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        // Help
        print_usage(argv[0]);
        exit(0);
      } else {
        fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
        return false;
      }
    } else {
      // Input file
      if (options->input_file != NULL) {
        fprintf(stderr, "Error: Multiple input files not supported\n");
        return false;
      }
      options->input_file = argv[i];
    }
  }
  
  // Check required arguments
  if (options->input_file == NULL) {
    fprintf(stderr, "Error: No input file specified\n");
    return false;
  }
  
  // Generate default output file name if not specified
  if (options->output_file == NULL) {
    size_t input_len = strlen(options->input_file);
    char* output_file = malloc(input_len + 2); // +2 for possible extension change and null terminator
    
    if (output_file == NULL) {
      fprintf(stderr, "Error: Out of memory\n");
      return false;
    }
    
    strcpy(output_file, options->input_file);
    
    // Replace .hoil extension with .coil, or append .coil if no extension
    char* ext = strrchr(output_file, '.');
    if (ext != NULL && strcmp(ext, ".hoil") == 0) {
      strcpy(ext, ".coil");
    } else {
      strcat(output_file, ".coil");
    }
    
    options->output_file = output_file;
  }
  
  return true;
}

/**
 * @brief Error callback function
 * 
 * @param error Error message
 * @param user_data User data
 */
static void error_callback(const error_message_t* error, void* user_data) {
  const char* severity = "Unknown";
  
  switch (error->severity) {
    case ERROR_INFO:
      severity = "Info";
      break;
    case ERROR_WARNING:
      severity = "Warning";
      break;
    case ERROR_ERROR:
      severity = "Error";
      break;
    case ERROR_INTERNAL:
      severity = "Internal Error";
      break;
  }
  
  if (error->location.filename != NULL) {
    fprintf(stderr, "%s: %s:%u:%u: %s\n", 
            severity, error->location.filename, 
            error->location.line, error->location.column, 
            error->message);
  } else {
    fprintf(stderr, "%s: %s\n", severity, error->message);
  }
}

/**
 * @brief Read file contents into a buffer
 * 
 * @param filename File to read
 * @param size Output buffer size
 * @return File contents, or NULL on error
 */
static char* read_file(const char* filename, size_t* size) {
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file: %s\n", filename);
    return NULL;
  }
  
  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (file_size <= 0) {
    fclose(file);
    fprintf(stderr, "Error: Empty file: %s\n", filename);
    return NULL;
  }
  
  // Allocate buffer
  char* buffer = (char*)malloc(file_size + 1);
  if (buffer == NULL) {
    fclose(file);
    fprintf(stderr, "Error: Out of memory\n");
    return NULL;
  }
  
  // Read file
  size_t bytes_read = fread(buffer, 1, file_size, file);
  fclose(file);
  
  if (bytes_read != (size_t)file_size) {
    free(buffer);
    fprintf(stderr, "Error: Failed to read file: %s\n", filename);
    return NULL;
  }
  
  // Null terminate the buffer
  buffer[file_size] = '\0';
  
  *size = file_size;
  return buffer;
}

/**
 * @brief Write buffer to file
 * 
 * @param filename File to write
 * @param buffer Buffer to write
 * @param size Buffer size
 * @return true if successful, false otherwise
 */
static bool write_file(const char* filename, const uint8_t* buffer, size_t size) {
  FILE* file = fopen(filename, "wb");
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file for writing: %s\n", filename);
    return false;
  }
  
  size_t bytes_written = fwrite(buffer, 1, size, file);
  fclose(file);
  
  if (bytes_written != size) {
    fprintf(stderr, "Error: Failed to write to file: %s\n", filename);
    return false;
  }
  
  return true;
}

/**
 * @brief Compile a HOIL source file to COIL binary
 * 
 * @param input_file Input file path
 * @param output_file Output file path
 * @param options Compiler options
 * @return true if successful, false otherwise
 */
static bool compile_file(const char* input_file, const char* output_file, options_t* options) {
  // Create error context
  error_context_t* error_context = coil_create_error_context();
  if (error_context == NULL) {
    fprintf(stderr, "Error: Failed to create error context\n");
    return false;
  }
  
  // Register error callback
  coil_register_error_callback(error_context, error_callback, NULL);
  
  // Read input file
  size_t source_size;
  char* source = read_file(input_file, &source_size);
  if (source == NULL) {
    coil_free_error_context(error_context);
    return false;
  }
  
  if (options->verbose) {
    printf("Compiling %s to %s\n", input_file, output_file);
  }
  
  // Create lexer
  lexer_t* lexer = hoil_create_lexer(source, source_size, input_file, error_context);
  if (lexer == NULL) {
    fprintf(stderr, "Error: Failed to create lexer\n");
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  // Create parser
  parser_t* parser = hoil_create_parser(lexer, error_context);
  if (parser == NULL) {
    fprintf(stderr, "Error: Failed to create parser\n");
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  // Parse module
  module_t* module = hoil_parse_module(parser);
  if (module == NULL) {
    fprintf(stderr, "Error: Failed to parse module\n");
    hoil_free_parser(parser);
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  // Check for errors
  if (coil_get_error_count(error_context, ERROR_ERROR) > 0) {
    fprintf(stderr, "Error: Compilation failed with %u errors\n",
            coil_get_error_count(error_context, ERROR_ERROR));
    coil_free_module(module);
    hoil_free_parser(parser);
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  // Convert to COIL binary
  uint8_t* binary_buffer = (uint8_t*)malloc(1024 * 1024); // 1MB initial buffer
  if (binary_buffer == NULL) {
    fprintf(stderr, "Error: Out of memory\n");
    coil_free_module(module);
    hoil_free_parser(parser);
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  int binary_size = coil_module_to_binary(module, binary_buffer, 1024 * 1024);
  if (binary_size < 0) {
    fprintf(stderr, "Error: Failed to convert module to binary\n");
    free(binary_buffer);
    coil_free_module(module);
    hoil_free_parser(parser);
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  // Write binary to output file
  if (!write_file(output_file, binary_buffer, binary_size)) {
    free(binary_buffer);
    coil_free_module(module);
    hoil_free_parser(parser);
    hoil_free_lexer(lexer);
    free(source);
    coil_free_error_context(error_context);
    return false;
  }
  
  if (options->verbose) {
    printf("Successfully compiled to %s (%d bytes)\n", output_file, binary_size);
  }
  
  // Clean up
  free(binary_buffer);
  coil_free_module(module);
  hoil_free_parser(parser);
  hoil_free_lexer(lexer);
  free(source);
  coil_free_error_context(error_context);
  
  return true;
}

/**
 * @brief Main entry point
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 on success, non-zero on error
 */
int main(int argc, char** argv) {
  // Parse command-line arguments
  options_t options;
  if (!parse_arguments(argc, argv, &options)) {
    print_usage(argv[0]);
    return 1;
  }
  
  // Compile the file
  if (!compile_file(options.input_file, options.output_file, &options)) {
    return 1;
  }
  
  return 0;
}