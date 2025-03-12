/**
 * @file parser.h
 * @brief Parser for the HOIL compiler
 *
 * This file defines the parser component for the HOIL compiler,
 * which parses HOIL source code into an abstract syntax tree (AST).
 */

#ifndef HOIL_PARSER_H
#define HOIL_PARSER_H

#include "common/module.h"
#include "common/error.h"
#include "hoil_compiler/lexer.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Parser structure
 */
typedef struct parser parser_t;

/**
 * @brief Create a new parser
 * 
 * @param lexer The lexer to use
 * @param error_context Error context
 * @return The new parser, or NULL on error
 */
parser_t* hoil_create_parser(lexer_t* lexer, error_context_t* error_context);

/**
 * @brief Free a parser
 * 
 * @param parser The parser to free
 */
void hoil_free_parser(parser_t* parser);

/**
 * @brief Parse a module
 * 
 * @param parser The parser
 * @return The parsed module, or NULL on error
 */
module_t* hoil_parse_module(parser_t* parser);

/**
 * @brief Parse a type
 * 
 * @param parser The parser
 * @param module The module (for type registry)
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
coil_type_t hoil_parse_type(parser_t* parser, module_t* module);

/**
 * @brief Parse a function
 * 
 * @param parser The parser
 * @param module The module
 * @param is_external True if the function is external
 * @return The parsed function, or NULL on error
 */
function_t* hoil_parse_function(parser_t* parser, module_t* module, bool is_external);

/**
 * @brief Parse a global variable
 * 
 * @param parser The parser
 * @param module The module
 * @param is_constant True if the global is a constant
 * @return The parsed global variable, or NULL on error
 */
global_variable_t* hoil_parse_global(parser_t* parser, module_t* module, bool is_constant);

/**
 * @brief Parse a basic block
 * 
 * @param parser The parser
 * @param module The module
 * @param function The function
 * @return The parsed basic block, or NULL on error
 */
basic_block_t* hoil_parse_basic_block(parser_t* parser, module_t* module, function_t* function);

/**
 * @brief Parse a structure type
 * 
 * @param parser The parser
 * @param module The module
 * @param name Structure name (optional)
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
coil_type_t hoil_parse_struct_type(parser_t* parser, module_t* module, const char* name);

/**
 * @brief Parse target requirements
 * 
 * @param parser The parser
 * @param module The module
 * @return 0 on success, -1 on error
 */
int hoil_parse_target(parser_t* parser, module_t* module);

#endif /* HOIL_PARSER_H */