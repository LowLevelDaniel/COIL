/**
* @file binary_parser.h
* @brief COIL binary format parser
*
* This file defines the binary parser component for the COIL assembler,
* which parses COIL binary files into an in-memory module representation.
*/

#ifndef COIL_BINARY_PARSER_H
#define COIL_BINARY_PARSER_H

#include "common/module.h"
#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
* @brief Binary parser structure
*/
typedef struct binary_parser binary_parser_t;

/**
* @brief Create a new binary parser
* 
* @param error_context Error context
* @return The new binary parser, or NULL on error
*/
binary_parser_t* coil_create_binary_parser(error_context_t* error_context);

/**
* @brief Free a binary parser
* 
* @param parser The binary parser to free
*/
void coil_free_binary_parser(binary_parser_t* parser);

/**
* @brief Parse a COIL binary file
* 
* @param parser The binary parser
* @param data Binary data
* @param size Data size
* @return The parsed module, or NULL on error
*/
module_t* coil_parse_binary(binary_parser_t* parser, const uint8_t* data, size_t size);

/**
* @brief Parse a COIL binary file from a file
* 
* @param parser The binary parser
* @param filename File name
* @return The parsed module, or NULL on error
*/
module_t* coil_parse_binary_file(binary_parser_t* parser, const char* filename);

/**
* @brief COIL binary file section types
*/
typedef enum {
  SECTION_TYPE_HEADER    = 0,  /**< Header section */
  SECTION_TYPE_TYPE      = 1,  /**< Type section */
  SECTION_TYPE_FUNCTION  = 2,  /**< Function section */
  SECTION_TYPE_GLOBAL    = 3,  /**< Global section */
  SECTION_TYPE_CONSTANT  = 4,  /**< Constant section */
  SECTION_TYPE_CODE      = 5,  /**< Code section */
  SECTION_TYPE_RELOC     = 6,  /**< Relocation section */
  SECTION_TYPE_METADATA  = 7   /**< Metadata section */
} section_type_t;

/**
* @brief COIL binary file section
*/
typedef struct {
  section_type_t type;     /**< Section type */
  uint32_t       offset;   /**< Offset from start of file */
  uint32_t       size;     /**< Section size */
  const uint8_t* data;     /**< Section data */
} section_t;

/**
* @brief Get a section by type
* 
* @param parser The binary parser
* @param type Section type
* @return The section, or NULL if not found
*/
const section_t* coil_get_section(binary_parser_t* parser, section_type_t type);

/**
* @brief Check if a binary file is valid COIL format
* 
* @param data Binary data
* @param size Data size
* @return true if valid, false otherwise
*/
bool coil_is_valid_binary(const uint8_t* data, size_t size);

/**
* @brief Get the COIL binary version
* 
* @param data Binary data
* @param size Data size
* @param major Major version output
* @param minor Minor version output
* @param patch Patch version output
* @return true if valid, false otherwise
*/
bool coil_get_binary_version(
  const uint8_t* data,
  size_t size,
  uint8_t* major,
  uint8_t* minor,
  uint8_t* patch
);

#endif /* COIL_BINARY_PARSER_H */