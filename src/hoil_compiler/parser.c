/**
 * @file parser.c
 * @brief Parser implementation for the HOIL compiler
 */

#include "hoil_compiler/parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Parser structure
 */
struct parser {
  lexer_t*         lexer;           /**< Lexer */
  error_context_t* error_context;   /**< Error context */
  token_t          current_token;   /**< Current token */
  bool             had_error;       /**< True if an error occurred */
};

parser_t* hoil_create_parser(lexer_t* lexer, error_context_t* error_context) {
  if (lexer == NULL) {
    return NULL;
  }
  
  parser_t* parser = (parser_t*)malloc(sizeof(parser_t));
  if (parser == NULL) {
    return NULL;
  }
  
  parser->lexer = lexer;
  parser->error_context = error_context;
  parser->had_error = false;
  
  // Initialize current token
  parser->current_token = hoil_lexer_next_token(lexer);
  
  return parser;
}

void hoil_free_parser(parser_t* parser) {
  if (parser == NULL) {
    return;
  }
  
  // No need to free lexer - it's owned by the caller
  
  free(parser);
}

/**
 * @brief Report a parser error
 * 
 * @param parser The parser
 * @param token The token where the error occurred
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  parser_t* parser,
  token_t token,
  uint32_t code,
  const char* message
) {
  if (parser->error_context != NULL) {
    source_location_t location = hoil_lexer_get_location(parser->lexer, token);
    
    REPORT_ERROR(parser->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                code, message, location.filename, location.line, location.column);
  }
  
  parser->had_error = true;
}

/**
 * @brief Check if the current token has the expected type
 * 
 * @param parser The parser
 * @param type Expected token type
 * @return True if the token has the expected type
 */
static bool check(parser_t* parser, token_type_t type) {
  return parser->current_token.type == type;
}

/**
 * @brief Match and consume a token of the expected type
 * 
 * @param parser The parser
 * @param type Expected token type
 * @return True if the token was matched and consumed
 */
static bool match(parser_t* parser, token_type_t type) {
  if (check(parser, type)) {
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return true;
  }
  return false;
}

/**
 * @brief Consume a token of the expected type
 * 
 * @param parser The parser
 * @param type Expected token type
 * @param error_message Error message if the token doesn't match
 * @return True if the token was consumed
 */
static bool consume(
  parser_t* parser,
  token_type_t type,
  const char* error_message
) {
  if (check(parser, type)) {
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return true;
  }
  
  report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN, error_message);
  return false;
}

/**
 * @brief Synchronize the parser after an error
 * 
 * Skips tokens until a statement boundary is found
 * 
 * @param parser The parser
 */
static void synchronize(parser_t* parser) {
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  while (parser->current_token.type != TOKEN_EOF) {
    // Statement boundary found
    if (parser->current_token.type == TOKEN_SEMICOLON) {
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return;
    }
    
    // Potential statement/declaration start
    switch (parser->current_token.type) {
      case TOKEN_MODULE:
      case TOKEN_TYPE:
      case TOKEN_CONSTANT:
      case TOKEN_GLOBAL:
      case TOKEN_FUNCTION:
      case TOKEN_EXTERN:
      case TOKEN_TARGET:
      case TOKEN_ENTRY:
        return;
      default:
        break;
    }
    
    parser->current_token = hoil_lexer_next_token(parser->lexer);
  }
}

/**
 * @brief Parse a basic COIL type
 * 
 * @param parser The parser
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
static coil_type_t parse_basic_type(parser_t* parser) {
  switch (parser->current_token.type) {
    case TOKEN_VOID:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_VOID;
    case TOKEN_BOOL:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_BOOL;
    case TOKEN_I8:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_INT8;
    case TOKEN_I16:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_INT16;
    case TOKEN_I32:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_INT32;
    case TOKEN_I64:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_INT64;
    case TOKEN_U8:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_UINT8;
    case TOKEN_U16:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_UINT16;
    case TOKEN_U32:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_UINT32;
    case TOKEN_U64:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_UINT64;
    case TOKEN_F16:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_FLOAT16;
    case TOKEN_F32:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_FLOAT32;
    case TOKEN_F64:
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      return COIL_TYPE_FLOAT64;
    default:
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected type");
      return COIL_TYPE_VOID;
  }
}

/**
 * @brief Parse a pointer type
 * 
 * @param parser The parser
 * @param module The module (for type registry)
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
static coil_type_t parse_pointer_type(parser_t* parser, module_t* module) {
  if (!consume(parser, TOKEN_LESS, "Expected '<' after 'ptr'")) {
    return COIL_TYPE_VOID;
  }
  
  coil_type_t base_type = hoil_parse_type(parser, module);
  
  // Parse memory space qualifier (optional)
  memory_space_t memory_space = MEMORY_SPACE_GLOBAL;
  uint8_t qualifiers = 0;
  
  if (match(parser, TOKEN_COMMA)) {
    if (check(parser, TOKEN_IDENTIFIER)) {
      const char* qualifier = parser->current_token.value.string_value;
      
      if (strcmp(qualifier, "global") == 0) {
        memory_space = MEMORY_SPACE_GLOBAL;
      } else if (strcmp(qualifier, "local") == 0) {
        memory_space = MEMORY_SPACE_LOCAL;
      } else if (strcmp(qualifier, "shared") == 0) {
        memory_space = MEMORY_SPACE_SHARED;
      } else if (strcmp(qualifier, "constant") == 0) {
        memory_space = MEMORY_SPACE_CONSTANT;
      } else if (strcmp(qualifier, "private") == 0) {
        memory_space = MEMORY_SPACE_PRIVATE;
      } else if (strcmp(qualifier, "const") == 0) {
        qualifiers |= TYPE_QUALIFIER_CONST;
      } else if (strcmp(qualifier, "volatile") == 0) {
        qualifiers |= TYPE_QUALIFIER_VOLATILE;
      } else if (strcmp(qualifier, "restrict") == 0) {
        qualifiers |= TYPE_QUALIFIER_RESTRICT;
      } else {
        report_error(parser, parser->current_token, ERROR_PARSER_INVALID_SYNTAX,
                    "Invalid pointer qualifier");
      }
      
      parser->current_token = hoil_lexer_next_token(parser->lexer);
    } else {
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected memory space or type qualifier after comma");
    }
  }
  
  if (!consume(parser, TOKEN_GREATER, "Expected '>' after pointer type")) {
    return COIL_TYPE_VOID;
  }
  
  return coil_create_pointer_type(base_type, memory_space, qualifiers);
}

/**
 * @brief Parse a vector type
 * 
 * @param parser The parser
 * @param module The module (for type registry)
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
static coil_type_t parse_vector_type(parser_t* parser, module_t* module) {
  if (!consume(parser, TOKEN_LESS, "Expected '<' after 'vec'")) {
    return COIL_TYPE_VOID;
  }
  
  coil_type_t element_type = hoil_parse_type(parser, module);
  
  if (!consume(parser, TOKEN_COMMA, "Expected ',' after vector element type")) {
    return COIL_TYPE_VOID;
  }
  
  if (!check(parser, TOKEN_INTEGER)) {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected integer for vector element count");
    return COIL_TYPE_VOID;
  }
  
  uint8_t element_count = (uint8_t)parser->current_token.value.int_value;
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  if (!consume(parser, TOKEN_GREATER, "Expected '>' after vector element count")) {
    return COIL_TYPE_VOID;
  }
  
  return coil_create_vector_type(element_type, element_count);
}

/**
 * @brief Parse an array type
 * 
 * @param parser The parser
 * @param module The module (for type registry)
 * @return The parsed type, or COIL_TYPE_VOID on error
 */
static coil_type_t parse_array_type(parser_t* parser, module_t* module) {
  if (!consume(parser, TOKEN_LESS, "Expected '<' after 'array'")) {
    return COIL_TYPE_VOID;
  }
  
  coil_type_t element_type = hoil_parse_type(parser, module);
  
  uint32_t element_count = 0;
  
  if (match(parser, TOKEN_COMMA)) {
    if (!check(parser, TOKEN_INTEGER)) {
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected integer for array element count");
      return COIL_TYPE_VOID;
    }
    
    element_count = (uint32_t)parser->current_token.value.int_value;
    parser->current_token = hoil_lexer_next_token(parser->lexer);
  }
  
  if (!consume(parser, TOKEN_GREATER, "Expected '>' after array type")) {
    return COIL_TYPE_VOID;
  }
  
  return coil_create_array_type(module->type_registry, element_type, element_count);
}

coil_type_t hoil_parse_type(parser_t* parser, module_t* module) {
  if (parser == NULL || module == NULL) {
    return COIL_TYPE_VOID;
  }
  
  // Basic types
  if (hoil_token_is_type_keyword(parser->current_token.type)) {
    return parse_basic_type(parser);
  }
  
  // Pointer type: ptr<base_type>
  if (check(parser, TOKEN_PTR)) {
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return parse_pointer_type(parser, module);
  }
  
  // Vector type: vec<element_type, count>
  if (check(parser, TOKEN_VEC)) {
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return parse_vector_type(parser, module);
  }
  
  // Array type: array<element_type> or array<element_type, count>
  if (check(parser, TOKEN_ARRAY)) {
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return parse_array_type(parser, module);
  }
  
  // Named struct type (reference to previously defined struct)
  if (check(parser, TOKEN_IDENTIFIER)) {
    // TODO: Look up struct type by name
    report_error(parser, parser->current_token, ERROR_TYPE_NOT_FOUND,
                "Named struct types not yet implemented");
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    return COIL_TYPE_VOID;
  }
  
  report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
              "Expected type");
  return COIL_TYPE_VOID;
}

/**
 * @brief Parse a struct element
 * 
 * @param parser The parser
 * @param module The module
 * @param elements Array to store elements
 * @param count Pointer to element count
 * @param capacity Pointer to element capacity
 * @return 0 on success, -1 on error
 */
static int parse_struct_element(
  parser_t* parser,
  module_t* module,
  struct_element_t** elements,
  uint32_t* count,
  uint32_t* capacity
) {
  // Parse element name
  if (!check(parser, TOKEN_IDENTIFIER)) {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected struct element name");
    return -1;
  }
  
  const char* name = parser->current_token.value.string_value;
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  if (!consume(parser, TOKEN_COLON, "Expected ':' after struct element name")) {
    return -1;
  }
  
  // Parse element type
  coil_type_t type = hoil_parse_type(parser, module);
  
  // Ensure capacity
  if (*count >= *capacity) {
    *capacity = *capacity == 0 ? 8 : (*capacity * 2);
    *elements = (struct_element_t*)realloc(
      *elements,
      (*capacity) * sizeof(struct_element_t)
    );
    
    if (*elements == NULL) {
      report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                  "Out of memory");
      return -1;
    }
  }
  
  // Add element
  (*elements)[*count].name = strdup(name);
  (*elements)[*count].type = type;
  (*elements)[*count].offset = 0;  // Will be calculated later
  (*count)++;
  
  // Check for comma
  match(parser, TOKEN_COMMA);  // Optional trailing comma
  
  return 0;
}

coil_type_t hoil_parse_struct_type(parser_t* parser, module_t* module, const char* name) {
  if (parser == NULL || module == NULL) {
    return COIL_TYPE_VOID;
  }
  
  if (!consume(parser, TOKEN_LBRACE, "Expected '{' after struct name")) {
    return COIL_TYPE_VOID;
  }
  
  // Parse struct elements
  struct_element_t* elements = NULL;
  uint32_t element_count = 0;
  uint32_t element_capacity = 0;
  
  while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
    if (parse_struct_element(parser, module, &elements, &element_count, &element_capacity) != 0) {
      // Error already reported
      free(elements);
      return COIL_TYPE_VOID;
    }
  }
  
  if (!consume(parser, TOKEN_RBRACE, "Expected '}' after struct elements")) {
    free(elements);
    return COIL_TYPE_VOID;
  }
  
  // Create struct type
  coil_type_t type = coil_create_struct_type(
    module->type_registry,
    name,
    elements,
    element_count
  );
  
  // Free elements array (they've been copied into the struct type)
  free(elements);
  
  return type;
}

/**
 * @brief Parse a function parameter
 * 
 * @param parser The parser
 * @param module The module
 * @param params Array to store parameters
 * @param count Pointer to parameter count
 * @param capacity Pointer to parameter capacity
 * @return 0 on success, -1 on error
 */
static int parse_function_parameter(
  parser_t* parser,
  module_t* module,
  function_parameter_t** params,
  uint32_t* count,
  uint32_t* capacity
) {
  // Parse parameter name
  if (!check(parser, TOKEN_IDENTIFIER)) {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected parameter name");
    return -1;
  }
  
  const char* name = parser->current_token.value.string_value;
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  if (!consume(parser, TOKEN_COLON, "Expected ':' after parameter name")) {
    return -1;
  }
  
  // Parse parameter type
  coil_type_t type = hoil_parse_type(parser, module);
  
  // Ensure capacity
  if (*count >= *capacity) {
    *capacity = *capacity == 0 ? 8 : (*capacity * 2);
    *params = (function_parameter_t*)realloc(
      *params,
      (*capacity) * sizeof(function_parameter_t)
    );
    
    if (*params == NULL) {
      report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                  "Out of memory");
      return -1;
    }
  }
  
  // Add parameter
  (*params)[*count].name = strdup(name);
  (*params)[*count].type = type;
  (*count)++;
  
  return 0;
}

/**
 * @brief Parse function parameters
 * 
 * @param parser The parser
 * @param module The module
 * @param params_out Output parameter array
 * @param count_out Output parameter count
 * @param is_variadic_out Output whether the function is variadic
 * @return 0 on success, -1 on error
 */
static int parse_function_parameters(
  parser_t* parser,
  module_t* module,
  function_parameter_t** params_out,
  uint32_t* count_out,
  bool* is_variadic_out
) {
  function_parameter_t* params = NULL;
  uint32_t param_count = 0;
  uint32_t param_capacity = 0;
  bool is_variadic = false;
  
  if (!consume(parser, TOKEN_LPAREN, "Expected '(' after function name")) {
    return -1;
  }
  
  // Empty parameter list
  if (match(parser, TOKEN_RPAREN)) {
    *params_out = params;
    *count_out = param_count;
    *is_variadic_out = false;
    return 0;
  }
  
  // Parse parameters
  while (true) {
    // Check for variadic marker
    if (check(parser, TOKEN_DOT)) {
      // Must be "..."
      if (!match(parser, TOKEN_DOT) || !match(parser, TOKEN_DOT) || !match(parser, TOKEN_DOT)) {
        report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                    "Expected '...' for variadic function");
        free(params);
        return -1;
      }
      
      is_variadic = true;
      break;
    }
    
    // Parse parameter
    if (parse_function_parameter(parser, module, &params, &param_count, &param_capacity) != 0) {
      // Error already reported
      free(params);
      return -1;
    }
    
    // Check for comma or end of list
    if (match(parser, TOKEN_RPAREN)) {
      break;
    }
    
    if (!consume(parser, TOKEN_COMMA, "Expected ',' after parameter")) {
      free(params);
      return -1;
    }
  }
  
  if (is_variadic && !consume(parser, TOKEN_RPAREN, "Expected ')' after variadic marker")) {
    free(params);
    return -1;
  }
  
  *params_out = params;
  *count_out = param_count;
  *is_variadic_out = is_variadic;
  return 0;
}

/**
 * @brief Parse a function return type
 * 
 * @param parser The parser
 * @param module The module
 * @return The parsed return type, or COIL_TYPE_VOID on error
 */
static coil_type_t parse_function_return_type(parser_t* parser, module_t* module) {
  if (!consume(parser, TOKEN_ARROW, "Expected '->' after parameter list")) {
    return COIL_TYPE_VOID;
  }
  
  return hoil_parse_type(parser, module);
}

/**
 * @brief Parse a function body
 * 
 * @param parser The parser
 * @param module The module
 * @param function The function
 * @return 0 on success, -1 on error
 */
static int parse_function_body(parser_t* parser, module_t* module, function_t* function) {
  if (!consume(parser, TOKEN_LBRACE, "Expected '{' after return type")) {
    return -1;
  }
  
  // Parse basic blocks
  while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
    // Parse block label
    if (!check(parser, TOKEN_IDENTIFIER)) {
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected block label");
      return -1;
    }
    
    const char* label = parser->current_token.value.string_value;
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    
    if (!consume(parser, TOKEN_COLON, "Expected ':' after block label")) {
      return -1;
    }
    
    // Create basic block
    basic_block_t* block = coil_create_basic_block(function, label);
    if (block == NULL) {
      report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                  "Out of memory");
      return -1;
    }
    
    // Parse block instructions (not yet implemented)
    // TODO: Parse block instructions
    
    // For now, just skip to the next block
    while (!check(parser, TOKEN_IDENTIFIER) && 
           !check(parser, TOKEN_RBRACE) && 
           !check(parser, TOKEN_EOF)) {
      parser->current_token = hoil_lexer_next_token(parser->lexer);
    }
  }
  
  if (!consume(parser, TOKEN_RBRACE, "Expected '}' at end of function body")) {
    return -1;
  }
  
  return 0;
}

function_t* hoil_parse_function(parser_t* parser, module_t* module, bool is_external) {
  if (parser == NULL || module == NULL) {
    return NULL;
  }
  
  if (!consume(parser, TOKEN_FUNCTION, "Expected 'FUNCTION' keyword")) {
    return NULL;
  }
  
  // Parse function name
  if (!check(parser, TOKEN_IDENTIFIER)) {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected function name");
    return NULL;
  }
  
  const char* name = parser->current_token.value.string_value;
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  // Parse parameters
  function_parameter_t* params = NULL;
  uint32_t param_count = 0;
  bool is_variadic = false;
  
  if (parse_function_parameters(parser, module, &params, &param_count, &is_variadic) != 0) {
    // Error already reported
    return NULL;
  }
  
  // Parse return type
  coil_type_t return_type = parse_function_return_type(parser, module);
  
  // Create function
  function_t* function = coil_create_function(
    module,
    name,
    return_type,
    params,
    param_count,
    is_variadic,
    is_external
  );
  
  if (function == NULL) {
    report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                "Out of memory");
    free(params);
    return NULL;
  }
  
  // Parse function body if not external
  if (!is_external) {
    if (parse_function_body(parser, module, function) != 0) {
      // Error already reported
      return NULL;
    }
  } else {
    // Expect semicolon after external function declaration
    if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' after external function declaration")) {
      return NULL;
    }
  }
  
  return function;
}

module_t* hoil_parse_module(parser_t* parser) {
  if (parser == NULL) {
    return NULL;
  }
  
  // Parse MODULE directive
  if (!consume(parser, TOKEN_MODULE, "Expected 'MODULE' at start of file")) {
    return NULL;
  }
  
  // Parse module name
  const char* module_name = NULL;
  
  if (check(parser, TOKEN_STRING)) {
    module_name = parser->current_token.value.string_value;
    parser->current_token = hoil_lexer_next_token(parser->lexer);
  } else {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected module name");
    return NULL;
  }
  
  if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' after module name")) {
    return NULL;
  }
  
  // Create module
  module_t* module = coil_create_module(module_name);
  if (module == NULL) {
    report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                "Out of memory");
    return NULL;
  }
  
  // Parse module items
  while (!check(parser, TOKEN_EOF)) {
    // Check for different kinds of declarations
    if (check(parser, TOKEN_TYPE)) {
      // TYPE declaration
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      
      if (!check(parser, TOKEN_IDENTIFIER)) {
        report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                    "Expected type name");
        synchronize(parser);
        continue;
      }
      
      const char* type_name = parser->current_token.value.string_value;
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      
      coil_type_t type = hoil_parse_struct_type(parser, module, type_name);
      
      if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' after type definition")) {
        synchronize(parser);
        continue;
      }
    } else if (check(parser, TOKEN_GLOBAL)) {
      // GLOBAL declaration
      global_variable_t* global = hoil_parse_global(parser, module, false);
      if (global == NULL) {
        synchronize(parser);
        continue;
      }
    } else if (check(parser, TOKEN_CONSTANT)) {
      // CONSTANT declaration
      global_variable_t* global = hoil_parse_global(parser, module, true);
      if (global == NULL) {
        synchronize(parser);
        continue;
      }
    } else if (check(parser, TOKEN_EXTERN)) {
      // External declaration
      parser->current_token = hoil_lexer_next_token(parser->lexer);
      
      if (check(parser, TOKEN_FUNCTION)) {
        // External function declaration
        function_t* function = hoil_parse_function(parser, module, true);
        if (function == NULL) {
          synchronize(parser);
          continue;
        }
      } else {
        report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                    "Expected 'FUNCTION' after 'EXTERN'");
        synchronize(parser);
        continue;
      }
    } else if (check(parser, TOKEN_FUNCTION)) {
      // Function definition
      function_t* function = hoil_parse_function(parser, module, false);
      if (function == NULL) {
        synchronize(parser);
        continue;
      }
    } else if (check(parser, TOKEN_TARGET)) {
      // Target requirements
      if (hoil_parse_target(parser, module) != 0) {
        synchronize(parser);
        continue;
      }
    } else {
      // Unexpected token
      report_error(parser, parser->current_token, ERROR_PARSER_UNEXPECTED_TOKEN,
                  "Unexpected token at module level");
      synchronize(parser);
      continue;
    }
  }
  
  return module;
}

global_variable_t* hoil_parse_global(parser_t* parser, module_t* module, bool is_constant) {
  if (parser == NULL || module == NULL) {
    return NULL;
  }
  
  token_type_t keyword = is_constant ? TOKEN_CONSTANT : TOKEN_GLOBAL;
  
  if (!consume(parser, keyword, is_constant ? "Expected 'CONSTANT'" : "Expected 'GLOBAL'")) {
    return NULL;
  }
  
  // Parse global name
  if (!check(parser, TOKEN_IDENTIFIER)) {
    report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                "Expected global variable name");
    return NULL;
  }
  
  const char* name = parser->current_token.value.string_value;
  parser->current_token = hoil_lexer_next_token(parser->lexer);
  
  if (!consume(parser, TOKEN_COLON, "Expected ':' after global variable name")) {
    return NULL;
  }
  
  // Parse global type
  coil_type_t type = hoil_parse_type(parser, module);
  
  // Parse initializer (optional)
  void* initializer = NULL;
  
  if (match(parser, TOKEN_EQUAL)) {
    // TODO: Parse initializer
    report_error(parser, parser->current_token, ERROR_PARSER_NOT_IMPLEMENTED,
                "Global initializers not yet implemented");
    return NULL;
  }
  
  if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' after global variable declaration")) {
    return NULL;
  }
  
  // TODO: Parse alignment
  uint32_t alignment = 8;  // Default alignment
  
  // Create global variable
  global_variable_t* global = coil_add_global(
    module,
    name,
    type,
    is_constant,
    false,  // Not external
    initializer,
    alignment
  );
  
  if (global == NULL) {
    report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                "Out of memory");
    return NULL;
  }
  
  return global;
}

/**
 * @brief Parse a string array
 * 
 * @param parser The parser
 * @param strings_out Output string array
 * @param count_out Output string count
 * @return 0 on success, -1 on error
 */
static int parse_string_array(
  parser_t* parser,
  const char*** strings_out,
  uint32_t* count_out
) {
  if (!consume(parser, TOKEN_LBRACKET, "Expected '[' for array")) {
    return -1;
  }
  
  // Empty array
  if (match(parser, TOKEN_RBRACKET)) {
    *strings_out = NULL;
    *count_out = 0;
    return 0;
  }
  
  // Parse strings
  const char** strings = NULL;
  uint32_t count = 0;
  uint32_t capacity = 0;
  
  while (true) {
    if (!check(parser, TOKEN_STRING)) {
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected string literal");
      free(strings);
      return -1;
    }
    
    // Ensure capacity
    if (count >= capacity) {
      capacity = capacity == 0 ? 8 : capacity * 2;
      strings = (const char**)realloc(strings, capacity * sizeof(const char*));
      
      if (strings == NULL) {
        report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                    "Out of memory");
        return -1;
      }
    }
    
    // Add string
    strings[count++] = strdup(parser->current_token.value.string_value);
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    
    // Check for comma or end of array
    if (match(parser, TOKEN_RBRACKET)) {
      break;
    }
    
    if (!consume(parser, TOKEN_COMMA, "Expected ',' after string")) {
      for (uint32_t i = 0; i < count; i++) {
        free((void*)strings[i]);
      }
      free(strings);
      return -1;
    }
  }
  
  *strings_out = strings;
  *count_out = count;
  return 0;
}

int hoil_parse_target(parser_t* parser, module_t* module) {
  if (parser == NULL || module == NULL) {
    return -1;
  }
  
  if (!consume(parser, TOKEN_TARGET, "Expected 'TARGET'")) {
    return -1;
  }
  
  if (!consume(parser, TOKEN_LBRACE, "Expected '{' after 'TARGET'")) {
    return -1;
  }
  
  // Parse target requirements
  const char** required_features = NULL;
  uint32_t required_count = 0;
  const char** preferred_features = NULL;
  uint32_t preferred_count = 0;
  const char* device_class = NULL;
  
  while (!check(parser, TOKEN_RBRACE) && !check(parser, TOKEN_EOF)) {
    if (!check(parser, TOKEN_IDENTIFIER)) {
      report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                  "Expected target requirement field name");
      goto error;
    }
    
    const char* field = parser->current_token.value.string_value;
    parser->current_token = hoil_lexer_next_token(parser->lexer);
    
    if (!consume(parser, TOKEN_EQUAL, "Expected '=' after field name")) {
      goto error;
    }
    
    if (strcmp(field, "required_features") == 0) {
      if (parse_string_array(parser, &required_features, &required_count) != 0) {
        goto error;
      }
    } else if (strcmp(field, "preferred_features") == 0) {
      if (parse_string_array(parser, &preferred_features, &preferred_count) != 0) {
        goto error;
      }
    } else if (strcmp(field, "device_class") == 0) {
      if (!check(parser, TOKEN_STRING)) {
        report_error(parser, parser->current_token, ERROR_PARSER_EXPECTED_TOKEN,
                    "Expected string for device_class");
        goto error;
      }
      
      device_class = strdup(parser->current_token.value.string_value);
      parser->current_token = hoil_lexer_next_token(parser->lexer);
    } else {
      report_error(parser, parser->current_token, ERROR_PARSER_INVALID_SYNTAX,
                  "Unknown target requirement field");
      goto error;
    }
  }
  
  if (!consume(parser, TOKEN_RBRACE, "Expected '}' at end of target requirements")) {
    goto error;
  }
  
  // Set target requirements
  if (coil_set_target_requirements(
      module,
      required_features,
      required_count,
      preferred_features,
      preferred_count,
      device_class
  ) != 0) {
    report_error(parser, parser->current_token, ERROR_GENERAL_OUT_OF_MEMORY,
                "Out of memory");
    goto error;
  }
  
  return 0;
  
error:
  // Clean up
  for (uint32_t i = 0; i < required_count; i++) {
    free((void*)required_features[i]);
  }
  free(required_features);
  
  for (uint32_t i = 0; i < preferred_count; i++) {
    free((void*)preferred_features[i]);
  }
  free(preferred_features);
  
  if (device_class != NULL) {
    free((void*)device_class);
  }
  
  return -1;
}