/**
 * @file lexer.c
 * @brief Lexer implementation for the HOIL compiler
 */

#include "hoil_compiler/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Keyword definition
 */
typedef struct {
  const char*   name;     /**< Keyword name */
  token_type_t  type;     /**< Token type */
} keyword_t;

/**
 * @brief Keyword table
 */
static const keyword_t keywords[] = {
  { "MODULE",   TOKEN_MODULE },
  { "TYPE",     TOKEN_TYPE },
  { "CONSTANT", TOKEN_CONSTANT },
  { "GLOBAL",   TOKEN_GLOBAL },
  { "FUNCTION", TOKEN_FUNCTION },
  { "EXTERN",   TOKEN_EXTERN },
  { "TARGET",   TOKEN_TARGET },
  { "ENTRY",    TOKEN_ENTRY },
  { "BR",       TOKEN_BR },
  { "ALWAYS",   TOKEN_ALWAYS },
  { "RET",      TOKEN_RET },
  { "CALL",     TOKEN_CALL },
  
  { "void",     TOKEN_VOID },
  { "bool",     TOKEN_BOOL },
  { "i8",       TOKEN_I8 },
  { "i16",      TOKEN_I16 },
  { "i32",      TOKEN_I32 },
  { "i64",      TOKEN_I64 },
  { "u8",       TOKEN_U8 },
  { "u16",      TOKEN_U16 },
  { "u32",      TOKEN_U32 },
  { "u64",      TOKEN_U64 },
  { "f16",      TOKEN_F16 },
  { "f32",      TOKEN_F32 },
  { "f64",      TOKEN_F64 },
  { "ptr",      TOKEN_PTR },
  { "array",    TOKEN_ARRAY },
  { "vec",      TOKEN_VEC },
  { "function", TOKEN_FUNCTION_TYPE },
  
  { NULL, TOKEN_EOF }  // Sentinel
};

/**
 * @brief Token type names for debug output
 */
static const char* token_names[] = {
  "EOF",
  
  /* Literals */
  "IDENTIFIER",
  "INTEGER",
  "FLOAT",
  "STRING",
  "CHAR",
  
  /* Keywords */
  "MODULE",
  "TYPE",
  "CONSTANT",
  "GLOBAL",
  "FUNCTION",
  "EXTERN",
  "TARGET",
  "ENTRY",
  "BR",
  "ALWAYS",
  "RET",
  "CALL",
  
  /* Data types */
  "VOID",
  "BOOL",
  "I8",
  "I16",
  "I32",
  "I64",
  "U8",
  "U16",
  "U32",
  "U64",
  "F16",
  "F32",
  "F64",
  "PTR",
  "ARRAY",
  "VEC",
  "FUNCTION_TYPE",
  
  /* Operators */
  "PLUS",
  "MINUS",
  "STAR",
  "SLASH",
  "PERCENT",
  "AMP",
  "PIPE",
  "CARET",
  "TILDE",
  "BANG",
  "EQUAL",
  "LESS",
  "GREATER",
  "DOT",
  "ARROW",
  
  /* Punctuation */
  "LPAREN",
  "RPAREN",
  "LBRACKET",
  "RBRACKET",
  "LBRACE",
  "RBRACE",
  "COMMA",
  "COLON",
  "SEMICOLON",
  
  /* Error */
  "ERROR"
};

/**
 * @brief Lexer structure
 */
struct lexer {
  const char*     source;         /**< Source code */
  size_t          source_length;  /**< Source code length */
  const char*     filename;       /**< Source file name */
  error_context_t* error_context; /**< Error context */
  
  const char*     current;        /**< Current position */
  uint32_t        line;           /**< Current line */
  uint32_t        column;         /**< Current column */
  
  token_t         peeked_token;   /**< Peeked token */
  bool            has_peeked;     /**< True if we have a peeked token */
};

lexer_t* hoil_create_lexer(
  const char* source,
  size_t source_length,
  const char* filename,
  error_context_t* error_context
) {
  if (source == NULL) {
    return NULL;
  }
  
  lexer_t* lexer = (lexer_t*)malloc(sizeof(lexer_t));
  if (lexer == NULL) {
    return NULL;
  }
  
  lexer->source = source;
  lexer->source_length = source_length;
  lexer->filename = filename ? strdup(filename) : NULL;
  lexer->error_context = error_context;
  
  lexer->current = source;
  lexer->line = 1;
  lexer->column = 1;
  
  lexer->has_peeked = false;
  
  return lexer;
}

void hoil_free_lexer(lexer_t* lexer) {
  if (lexer == NULL) {
    return;
  }
  
  if (lexer->filename) {
    free((void*)lexer->filename);
  }
  
  // Free peeked token if needed
  if (lexer->has_peeked) {
    if (lexer->peeked_token.type == TOKEN_STRING ||
        lexer->peeked_token.type == TOKEN_IDENTIFIER) {
      free((void*)lexer->peeked_token.value.string_value);
    }
  }
  
  free(lexer);
}

/**
 * @brief Check if we've reached the end of the source
 * 
 * @param lexer The lexer
 * @return True if at end of source
 */
static bool is_at_end(lexer_t* lexer) {
  return lexer->current >= lexer->source + lexer->source_length;
}

/**
 * @brief Get the current character without advancing
 * 
 * @param lexer The lexer
 * @return The current character, or '\0' if at end
 */
static char peek(lexer_t* lexer) {
  if (is_at_end(lexer)) {
    return '\0';
  }
  return *lexer->current;
}

/**
 * @brief Get the next character without advancing
 * 
 * @param lexer The lexer
 * @return The next character, or '\0' if at end
 */
static char peek_next(lexer_t* lexer) {
  if (is_at_end(lexer) || lexer->current + 1 >= lexer->source + lexer->source_length) {
    return '\0';
  }
  return lexer->current[1];
}

/**
 * @brief Advance by one character
 * 
 * @param lexer The lexer
 * @return The character advanced past
 */
static char advance(lexer_t* lexer) {
  char c = peek(lexer);
  
  if (!is_at_end(lexer)) {
    lexer->current++;
    
    if (c == '\n') {
      lexer->line++;
      lexer->column = 1;
    } else {
      lexer->column++;
    }
  }
  
  return c;
}

/**
 * @brief Match the next character
 * 
 * @param lexer The lexer
 * @param expected The expected character
 * @return True if the next character matches and was consumed
 */
static bool match(lexer_t* lexer, char expected) {
  if (is_at_end(lexer) || peek(lexer) != expected) {
    return false;
  }
  
  advance(lexer);
  return true;
}

/**
 * @brief Skip whitespace and comments
 * 
 * @param lexer The lexer
 */
static void skip_whitespace_and_comments(lexer_t* lexer) {
  for (;;) {
    char c = peek(lexer);
    
    switch (c) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        advance(lexer);
        break;
        
      case '/':
        if (peek_next(lexer) == '/') {
          // Line comment - skip until end of line
          while (peek(lexer) != '\n' && !is_at_end(lexer)) {
            advance(lexer);
          }
        } else if (peek_next(lexer) == '*') {
          // Block comment - skip until */
          advance(lexer);  // Skip /
          advance(lexer);  // Skip *
          
          while (!is_at_end(lexer) &&
                 !(peek(lexer) == '*' && peek_next(lexer) == '/')) {
            advance(lexer);
            
            if (is_at_end(lexer)) {
              // Unterminated block comment
              if (lexer->error_context) {
                REPORT_ERROR(lexer->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                            ERROR_PARSER_UNTERMINATED, "Unterminated block comment",
                            lexer->filename, lexer->line, lexer->column);
              }
              return;
            }
          }
          
          advance(lexer);  // Skip *
          advance(lexer);  // Skip /
        } else {
          // Division operator
          return;
        }
        break;
        
      default:
        return;
    }
  }
}

/**
 * @brief Check if a character is a valid identifier start
 * 
 * @param c The character
 * @return True if valid identifier start
 */
static bool is_identifier_start(char c) {
  return isalpha(c) || c == '_';
}

/**
 * @brief Check if a character is a valid identifier part
 * 
 * @param c The character
 * @return True if valid identifier part
 */
static bool is_identifier_part(char c) {
  return isalnum(c) || c == '_';
}

/**
 * @brief Scan an identifier or keyword
 * 
 * @param lexer The lexer
 * @return The scanned token
 */
static token_t scan_identifier(lexer_t* lexer) {
  const char* start = lexer->current - 1;  // Include the first character
  uint32_t start_column = lexer->column - 1;
  
  while (is_identifier_part(peek(lexer))) {
    advance(lexer);
  }
  
  uint32_t length = lexer->current - start;
  
  // Check if it's a keyword
  for (const keyword_t* keyword = keywords; keyword->name != NULL; keyword++) {
    if (strlen(keyword->name) == length &&
        strncmp(start, keyword->name, length) == 0) {
      // It's a keyword
      token_t token;
      token.type = keyword->type;
      token.text = start;
      token.length = length;
      token.line = lexer->line;
      token.column = start_column;
      return token;
    }
  }
  
  // It's an identifier
  token_t token;
  token.type = TOKEN_IDENTIFIER;
  token.text = start;
  token.length = length;
  token.line = lexer->line;
  token.column = start_column;
  token.value.string_value = strndup(start, length);
  return token;
}

/**
 * @brief Scan a number literal (integer or float)
 * 
 * @param lexer The lexer
 * @return The scanned token
 */
static token_t scan_number(lexer_t* lexer) {
  const char* start = lexer->current - 1;  // Include the first digit
  uint32_t start_column = lexer->column - 1;
  bool is_float = false;
  
  // Check for hexadecimal, binary, or octal prefix
  if (*start == '0') {
    if (peek(lexer) == 'x' || peek(lexer) == 'X') {
      // Hexadecimal
      advance(lexer);
      while (isxdigit(peek(lexer))) {
        advance(lexer);
      }
      
      token_t token;
      token.type = TOKEN_INTEGER;
      token.text = start;
      token.length = lexer->current - start;
      token.line = lexer->line;
      token.column = start_column;
      
      // Parse the value
      char* end;
      token.value.int_value = strtoll(start, &end, 16);
      
      return token;
    } else if (peek(lexer) == 'b' || peek(lexer) == 'B') {
      // Binary
      advance(lexer);
      while (peek(lexer) == '0' || peek(lexer) == '1') {
        advance(lexer);
      }
      
      token_t token;
      token.type = TOKEN_INTEGER;
      token.text = start;
      token.length = lexer->current - start;
      token.line = lexer->line;
      token.column = start_column;
      
      // Parse the value
      char* buffer = strndup(start + 2, token.length - 2);
      token.value.int_value = strtoll(buffer, NULL, 2);
      free(buffer);
      
      return token;
    }
  }
  
  // Decimal number
  while (isdigit(peek(lexer))) {
    advance(lexer);
  }
  
  // Check for decimal point
  if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
    is_float = true;
    advance(lexer);  // Skip the dot
    
    while (isdigit(peek(lexer))) {
      advance(lexer);
    }
  }
  
  // Check for exponent
  if (peek(lexer) == 'e' || peek(lexer) == 'E') {
    is_float = true;
    advance(lexer);
    
    if (peek(lexer) == '+' || peek(lexer) == '-') {
      advance(lexer);
    }
    
    if (!isdigit(peek(lexer))) {
      // Invalid exponent
      if (lexer->error_context) {
        REPORT_ERROR(lexer->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                    ERROR_PARSER_INVALID_SYNTAX, "Invalid exponent in number",
                    lexer->filename, lexer->line, lexer->column);
      }
      
      token_t token;
      token.type = TOKEN_ERROR;
      token.text = start;
      token.length = lexer->current - start;
      token.line = lexer->line;
      token.column = start_column;
      return token;
    }
    
    while (isdigit(peek(lexer))) {
      advance(lexer);
    }
  }
  
  token_t token;
  token.type = is_float ? TOKEN_FLOAT : TOKEN_INTEGER;
  token.text = start;
  token.length = lexer->current - start;
  token.line = lexer->line;
  token.column = start_column;
  
  // Parse the value
  if (is_float) {
    char* buffer = strndup(start, token.length);
    token.value.float_value = strtod(buffer, NULL);
    free(buffer);
  } else {
    char* buffer = strndup(start, token.length);
    token.value.int_value = strtoll(buffer, NULL, 10);
    free(buffer);
  }
  
  return token;
}

/**
 * @brief Scan a string literal
 * 
 * @param lexer The lexer
 * @return The scanned token
 */
static token_t scan_string(lexer_t* lexer) {
  const char* start = lexer->current - 1;  // Include the opening quote
  uint32_t start_column = lexer->column - 1;
  
  // Buffer for the string value
  char* buffer = NULL;
  size_t buffer_size = 0;
  size_t buffer_pos = 0;
  
  while (peek(lexer) != '"' && !is_at_end(lexer)) {
    char c = advance(lexer);
    
    // Handle escape sequences
    if (c == '\\') {
      char next = advance(lexer);
      
      switch (next) {
        case 'n': c = '\n'; break;
        case 'r': c = '\r'; break;
        case 't': c = '\t'; break;
        case '\\': c = '\\'; break;
        case '"': c = '"'; break;
        case '0': c = '\0'; break;
        default:
          // Invalid escape sequence
          if (lexer->error_context) {
            REPORT_ERROR(lexer->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                        ERROR_PARSER_INVALID_SYNTAX,
                        "Invalid escape sequence in string",
                        lexer->filename, lexer->line, lexer->column);
          }
          break;
      }
    }
    
    // Grow buffer if needed
    if (buffer_pos >= buffer_size) {
      buffer_size = buffer_size == 0 ? 16 : buffer_size * 2;
      buffer = realloc(buffer, buffer_size);
    }
    
    buffer[buffer_pos++] = c;
  }
  
  if (is_at_end(lexer)) {
    // Unterminated string
    if (lexer->error_context) {
      REPORT_ERROR(lexer->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                  ERROR_PARSER_UNTERMINATED, "Unterminated string",
                  lexer->filename, lexer->line, lexer->column);
    }
    
    token_t token;
    token.type = TOKEN_ERROR;
    token.text = start;
    token.length = lexer->current - start;
    token.line = lexer->line;
    token.column = start_column;
    free(buffer);
    return token;
  }
  
  // Consume the closing quote
  advance(lexer);
  
  // Finalize the string value
  if (buffer == NULL) {
    buffer = malloc(1);
  }
  buffer = realloc(buffer, buffer_pos + 1);
  buffer[buffer_pos] = '\0';
  
  token_t token;
  token.type = TOKEN_STRING;
  token.text = start;
  token.length = lexer->current - start;
  token.line = lexer->line;
  token.column = start_column;
  token.value.string_value = buffer;
  
  return token;
}

/**
 * @brief Scan a single character token
 * 
 * @param lexer The lexer
 * @param type Token type
 * @return The scanned token
 */
static token_t scan_simple_token(lexer_t* lexer, token_type_t type) {
  token_t token;
  token.type = type;
  token.text = lexer->current - 1;  // Point to the character
  token.length = 1;
  token.line = lexer->line;
  token.column = lexer->column - 1;
  return token;
}

/**
 * @brief Scan a token of the given type with the given length
 * 
 * @param lexer The lexer
 * @param type Token type
 * @param length Token length
 * @return The scanned token
 */
static token_t scan_token(lexer_t* lexer, token_type_t type, uint32_t length) {
  token_t token;
  token.type = type;
  token.text = lexer->current - length;
  token.length = length;
  token.line = lexer->line;
  token.column = lexer->column - length;
  return token;
}

token_t hoil_lexer_next_token(lexer_t* lexer) {
  if (lexer == NULL) {
    token_t token;
    token.type = TOKEN_ERROR;
    token.text = NULL;
    token.length = 0;
    token.line = 0;
    token.column = 0;
    return token;
  }
  
  // Return and clear the peeked token if we have one
  if (lexer->has_peeked) {
    token_t token = lexer->peeked_token;
    lexer->has_peeked = false;
    return token;
  }
  
  // Skip whitespace and comments
  skip_whitespace_and_comments(lexer);
  
  // Check for end of file
  if (is_at_end(lexer)) {
    token_t token;
    token.type = TOKEN_EOF;
    token.text = lexer->current;
    token.length = 0;
    token.line = lexer->line;
    token.column = lexer->column;
    return token;
  }
  
  // Get the next character
  char c = advance(lexer);
  
  // Identifiers
  if (is_identifier_start(c)) {
    return scan_identifier(lexer);
  }
  
  // Numbers
  if (isdigit(c)) {
    return scan_number(lexer);
  }
  
  // String literals
  if (c == '"') {
    return scan_string(lexer);
  }
  
  // Operators and punctuation
  switch (c) {
    case '(': return scan_simple_token(lexer, TOKEN_LPAREN);
    case ')': return scan_simple_token(lexer, TOKEN_RPAREN);
    case '[': return scan_simple_token(lexer, TOKEN_LBRACKET);
    case ']': return scan_simple_token(lexer, TOKEN_RBRACKET);
    case '{': return scan_simple_token(lexer, TOKEN_LBRACE);
    case '}': return scan_simple_token(lexer, TOKEN_RBRACE);
    case ',': return scan_simple_token(lexer, TOKEN_COMMA);
    case ':': return scan_simple_token(lexer, TOKEN_COLON);
    case ';': return scan_simple_token(lexer, TOKEN_SEMICOLON);
    case '+': return scan_simple_token(lexer, TOKEN_PLUS);
    case '*': return scan_simple_token(lexer, TOKEN_STAR);
    case '%': return scan_simple_token(lexer, TOKEN_PERCENT);
    case '&': return scan_simple_token(lexer, TOKEN_AMP);
    case '|': return scan_simple_token(lexer, TOKEN_PIPE);
    case '^': return scan_simple_token(lexer, TOKEN_CARET);
    case '~': return scan_simple_token(lexer, TOKEN_TILDE);
    case '!': return scan_simple_token(lexer, TOKEN_BANG);
    case '=': return scan_simple_token(lexer, TOKEN_EQUAL);
    case '<': return scan_simple_token(lexer, TOKEN_LESS);
    case '>': return scan_simple_token(lexer, TOKEN_GREATER);
    
    case '.': return scan_simple_token(lexer, TOKEN_DOT);
    
    case '-':
      if (match(lexer, '>')) {
        return scan_token(lexer, TOKEN_ARROW, 2);
      }
      return scan_simple_token(lexer, TOKEN_MINUS);
      
    case '/':
      // Division operator (comments already handled in skip_whitespace_and_comments)
      return scan_simple_token(lexer, TOKEN_SLASH);
      
    default:
      // Invalid character
      if (lexer->error_context) {
        char msg[32];
        snprintf(msg, sizeof(msg), "Unexpected character: '%c'", c);
        REPORT_ERROR(lexer->error_context, ERROR_ERROR, ERROR_CATEGORY_PARSER,
                    ERROR_PARSER_UNEXPECTED_TOKEN, msg,
                    lexer->filename, lexer->line, lexer->column - 1);
      }
      
      token_t token;
      token.type = TOKEN_ERROR;
      token.text = lexer->current - 1;
      token.length = 1;
      token.line = lexer->line;
      token.column = lexer->column - 1;
      return token;
  }
}

token_t hoil_lexer_peek_token(lexer_t* lexer) {
  if (lexer == NULL) {
    token_t token;
    token.type = TOKEN_ERROR;
    token.text = NULL;
    token.length = 0;
    token.line = 0;
    token.column = 0;
    return token;
  }
  
  if (lexer->has_peeked) {
    return lexer->peeked_token;
  }
  
  lexer->peeked_token = hoil_lexer_next_token(lexer);
  lexer->has_peeked = true;
  
  return lexer->peeked_token;
}

const char* hoil_token_type_name(token_type_t type) {
  if (type >= 0 && type <= TOKEN_ERROR) {
    return token_names[type];
  }
  return "UNKNOWN";
}

bool hoil_token_is_keyword(token_type_t type) {
  return type >= TOKEN_MODULE && type <= TOKEN_FUNCTION_TYPE;
}

bool hoil_token_is_type_keyword(token_type_t type) {
  return type >= TOKEN_VOID && type <= TOKEN_FUNCTION_TYPE;
}

source_location_t hoil_lexer_get_location(lexer_t* lexer, token_t token) {
  source_location_t location;
  location.filename = lexer->filename;
  location.line = token.line;
  location.column = token.column;
  return location;
}