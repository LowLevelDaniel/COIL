/**
 * @file binary_parser.c
 * @brief COIL binary format parser implementation
 */

#include "coil_assembler/binary_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define COIL_MAGIC 0x434F494C  /* "COIL" in ASCII */

/**
 * @brief Binary parser structure
 */
struct binary_parser {
  error_context_t* error_context;  /**< Error context */
  const uint8_t*   data;           /**< Binary data */
  size_t           size;           /**< Data size */
  section_t*       sections;       /**< Sections */
  uint32_t         section_count;  /**< Number of sections */
  bool             had_error;      /**< True if an error occurred */
};

binary_parser_t* coil_create_binary_parser(error_context_t* error_context) {
  binary_parser_t* parser = (binary_parser_t*)malloc(sizeof(binary_parser_t));
  if (parser == NULL) {
    return NULL;
  }
  
  parser->error_context = error_context;
  parser->data = NULL;
  parser->size = 0;
  parser->sections = NULL;
  parser->section_count = 0;
  parser->had_error = false;
  
  return parser;
}

void coil_free_binary_parser(binary_parser_t* parser) {
  if (parser == NULL) {
    return;
  }
  
  free(parser->sections);
  free(parser);
}

/**
 * @brief Report a binary parser error
 * 
 * @param parser The parser
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  binary_parser_t* parser,
  uint32_t code,
  const char* message
) {
  if (parser->error_context != NULL) {
    REPORT_ERROR(parser->error_context, ERROR_ERROR, ERROR_CATEGORY_BINARY,
                code, message, NULL, 0, 0);
  }
  
  parser->had_error = true;
}

/**
 * @brief Parse the COIL binary header
 * 
 * @param parser The parser
 * @return true if successful, false otherwise
 */
static bool parse_header(binary_parser_t* parser) {
  if (parser->size < 16) {
    report_error(parser, ERROR_BINARY_INVALID_FORMAT, "File too small");
    return false;
  }
  
  // Check magic number
  uint32_t magic = *(uint32_t*)parser->data;
  if (magic != COIL_MAGIC) {
    report_error(parser, ERROR_BINARY_INVALID_FORMAT, "Invalid magic number");
    return false;
  }
  
  // Get version
  uint32_t version = *(uint32_t*)(parser->data + 4);
  uint8_t major = (version >> 24) & 0xFF;
  uint8_t minor = (version >> 16) & 0xFF;
  uint8_t patch = (version >> 8) & 0xFF;
  
  // For now, we only support version 1.0.x
  if (major != 1 || minor != 0) {
    char error_msg[64];
    snprintf(error_msg, sizeof(error_msg), 
             "Unsupported version: %u.%u.%u", major, minor, patch);
    report_error(parser, ERROR_BINARY_UNSUPPORTED_VER, error_msg);
    return false;
  }
  
  // Get section count
  uint32_t section_count = *(uint32_t*)(parser->data + 8);
  
  if (section_count == 0) {
    report_error(parser, ERROR_BINARY_INVALID_FORMAT, "No sections in file");
    return false;
  }
  
  // Allocate sections
  parser->sections = (section_t*)malloc(section_count * sizeof(section_t));
  if (parser->sections == NULL) {
    report_error(parser, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return false;
  }
  
  parser->section_count = section_count;
  
  // Parse section table
  uint32_t section_table_offset = 16;
  for (uint32_t i = 0; i < section_count; i++) {
    uint32_t offset = section_table_offset + i * 12;
    
    if (offset + 12 > parser->size) {
      report_error(parser, ERROR_BINARY_CORRUPT, "Section table truncated");
      return false;
    }
    
    section_t* section = &parser->sections[i];
    section->type = (section_type_t)*(uint32_t*)(parser->data + offset);
    section->offset = *(uint32_t*)(parser->data + offset + 4);
    section->size = *(uint32_t*)(parser->data + offset + 8);
    
    // Validate section
    if (section->offset + section->size > parser->size) {
      report_error(parser, ERROR_BINARY_CORRUPT, "Section extends beyond file");
      return false;
    }
    
    section->data = parser->data + section->offset;
  }
  
  return true;
}

/**
 * @brief Parse the type section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_type_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_TYPE);
  if (section == NULL) {
    // Type section is optional
    return true;
  }
  
  // TODO: Implement type section parsing
  // For now, we'll skip this since our initial implementation
  // only needs to support basic types
  
  return true;
}

/**
 * @brief Parse the function section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_function_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_FUNCTION);
  if (section == NULL) {
    return true;  // Optional section
  }
  
  // TODO: Implement function section parsing
  // For now, we'll skip this for our initial implementation
  
  return true;
}

/**
 * @brief Parse the global section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_global_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_GLOBAL);
  if (section == NULL) {
    return true;  // Optional section
  }
  
  // TODO: Implement global section parsing
  // For now, we'll skip this for our initial implementation
  
  return true;
}

/**
 * @brief Parse the constant section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_constant_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_CONSTANT);
  if (section == NULL) {
    return true;  // Optional section
  }
  
  // TODO: Implement constant section parsing
  // For now, we'll skip this for our initial implementation
  
  return true;
}

/**
 * @brief Parse the code section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_code_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_CODE);
  if (section == NULL) {
    // Code section is required if there are functions
    if (module->function_count > 0) {
      report_error(parser, ERROR_BINARY_MISSING_SECTION, "Missing code section");
      return false;
    }
    return true;
  }
  
  // TODO: Implement code section parsing
  // For now, we'll skip this for our initial implementation
  
  return true;
}

/**
 * @brief Parse the relocation section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_relocation_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_RELOC);
  if (section == NULL) {
    return true;  // Optional section
  }
  
  // TODO: Implement relocation section parsing
  // For now, we'll skip this for our initial implementation
  
  return true;
}

/**
 * @brief Parse the metadata section
 * 
 * @param parser The parser
 * @param module The module
 * @return true if successful, false otherwise
 */
static bool parse_metadata_section(binary_parser_t* parser, module_t* module) {
  const section_t* section = coil_get_section(parser, SECTION_TYPE_METADATA);
  if (section == NULL) {
    return true;  // Optional section
  }
  
  // The metadata section contains the module name and other metadata
  // Module name is a null-terminated string at the beginning of the section
  if (section->size > 0) {
    const char* name = (const char*)section->data;
    
    // Ensure the string is properly terminated
    bool found_terminator = false;
    for (uint32_t i = 0; i < section->size; i++) {
      if (name[i] == '\0') {
        found_terminator = true;
        break;
      }
    }
    
    if (!found_terminator) {
      report_error(parser, ERROR_BINARY_CORRUPT, "Unterminated module name");
      return false;
    }
    
    // Set module name (replacing any existing name)
    if (module->name) {
      free((void*)module->name);
    }
    module->name = strdup(name);
  }
  
  return true;
}

module_t* coil_parse_binary(binary_parser_t* parser, const uint8_t* data, size_t size) {
  if (parser == NULL || data == NULL || size == 0) {
    return NULL;
  }
  
  // Reset parser state
  free(parser->sections);
  parser->sections = NULL;
  parser->section_count = 0;
  parser->had_error = false;
  
  // Set binary data
  parser->data = data;
  parser->size = size;
  
  // Parse header and section table
  if (!parse_header(parser)) {
    return NULL;
  }
  
  // Create empty module
  module_t* module = coil_create_module(NULL);
  if (module == NULL) {
    report_error(parser, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return NULL;
  }
  
  // Parse sections
  if (!parse_type_section(parser, module) ||
      !parse_function_section(parser, module) ||
      !parse_global_section(parser, module) ||
      !parse_constant_section(parser, module) ||
      !parse_code_section(parser, module) ||
      !parse_relocation_section(parser, module) ||
      !parse_metadata_section(parser, module)) {
    coil_free_module(module);
    return NULL;
  }
  
  return module;
}

module_t* coil_parse_binary_file(binary_parser_t* parser, const char* filename) {
  if (parser == NULL || filename == NULL) {
    return NULL;
  }
  
  // Open file
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    report_error(parser, ERROR_SYSTEM_IO, "Failed to open file");
    return NULL;
  }
  
  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (file_size <= 0) {
    fclose(file);
    report_error(parser, ERROR_SYSTEM_IO, "Failed to get file size or empty file");
    return NULL;
  }
  
  // Allocate buffer
  uint8_t* data = (uint8_t*)malloc(file_size);
  if (data == NULL) {
    fclose(file);
    report_error(parser, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return NULL;
  }
  
  // Read file
  size_t bytes_read = fread(data, 1, file_size, file);
  fclose(file);
  
  if (bytes_read != file_size) {
    free(data);
    report_error(parser, ERROR_SYSTEM_IO, "Failed to read file");
    return NULL;
  }
  
  // Parse binary
  module_t* module = coil_parse_binary(parser, data, file_size);
  
  // Free data buffer
  free(data);
  
  return module;
}

const section_t* coil_get_section(binary_parser_t* parser, section_type_t type) {
  if (parser == NULL) {
    return NULL;
  }
  
  for (uint32_t i = 0; i < parser->section_count; i++) {
    if (parser->sections[i].type == type) {
      return &parser->sections[i];
    }
  }
  
  return NULL;
}

bool coil_is_valid_binary(const uint8_t* data, size_t size) {
  if (data == NULL || size < 4) {
    return false;
  }
  
  uint32_t magic = *(uint32_t*)data;
  return magic == COIL_MAGIC;
}

bool coil_get_binary_version(
  const uint8_t* data,
  size_t size,
  uint8_t* major,
  uint8_t* minor,
  uint8_t* patch
) {
  if (!coil_is_valid_binary(data, size) || size < 8) {
    return false;
  }
  
  uint32_t version = *(uint32_t*)(data + 4);
  
  if (major) *major = (version >> 24) & 0xFF;
  if (minor) *minor = (version >> 16) & 0xFF;
  if (patch) *patch = (version >> 8) & 0xFF;
  
  return true;
}