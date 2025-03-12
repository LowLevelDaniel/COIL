/**
 * @file lexer.h
 * @brief Lexer for the HOIL compiler
 *
 * This file defines the lexer component for the HOIL compiler,
 * which tokenizes HOIL source code.
 */

#ifndef HOIL_LEXER_H
#define HOIL_LEXER_H

#include "common/error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Token types
 */
typedef enum {
  TOKEN_EOF,              /**< End of file */
  
  /* Literals */
  TOKEN_IDENTIFIER,       /**< Identifier */
  TOKEN_INTEGER,          /**< Integer literal */
  TOKEN_FLOAT,            /**< Floating-point literal */
  TOKEN_STRING,           /**< String literal */
  TOKEN_CHAR,             /**< Character literal */
  
  /* Keywords */
  TOKEN_MODULE,           /**< MODULE */
  TOKEN_TYPE,             /**< TYPE */
  TOKEN_CONSTANT,         /**< CONSTANT */
  TOKEN_GLOBAL,           /**< GLOBAL */
  TOKEN_FUNCTION,         /**< FUNCTION */
  TOKEN_EXTERN,           /**< EXTERN */
  TOKEN_TARGET,           /**< TARGET */
  TOKEN_ENTRY,            /**< ENTRY */
  TOKEN_BR,               /**< BR */
  TOKEN_ALWAYS,           /**< ALWAYS */
  TOKEN_RET,              /**< RET */
  TOKEN_CALL,             /**< CALL */
  
  /* Data types */
  TOKEN_VOID,             /**< void */
  TOKEN_BOOL,             /**< bool */
  TOKEN_I8,               /**< i8 */
  TOKEN_I16,              /**< i16 */
  TOKEN_I32,              /**< i32 */
  TOKEN_I64,              /**< i64 */
  TOKEN_U8,               /**< u8 */
  TOKEN_U16,              /**< u16 */
  TOKEN_U32,              /**< u32 */
  TOKEN_U64,              /**< u64 */
  TOKEN_F16,              /**< f16 */
  TOKEN_F32,              /**< f32 */
  TOKEN_F64,              /**< f64 */
  TOKEN_PTR,              /**< ptr */
  TOKEN_ARRAY,            /**< array */
  TOKEN_VEC,              /**< vec */
  TOKEN_FUNCTION_TYPE,    /**< function */
  
  /* Operators */
  TOKEN_PLUS,             /**< + */
  TOKEN_MINUS,            /**< - */
  TOKEN_STAR,             /**< * */
  TOKEN_SLASH,            /**< / */
  TOKEN_PERCENT,          /**< % */
  TOKEN_AMP,              /**< & */
  TOKEN_PIPE,             /**< | */
  TOKEN_CARET,            /**< ^ */
  TOKEN_TILDE,            /**< ~ */
  TOKEN_BANG,             /**< ! */
  TOKEN_EQUAL,            /**< = */
  TOKEN_LESS,             /**< < */
  TOKEN_GREATER,          /**< > */
  TOKEN_DOT,              /**< . */
  TOKEN_ARROW,            /**< -> */
  
  /* Punctuation */
  TOKEN_LPAREN,           /**< ( */
  TOKEN_RPAREN,           /**< ) */
  TOKEN_LBRACKET,         /**< [ */
  TOKEN_RBRACKET,         /**< ] */
  TOKEN_LBRACE,           /**< { */
  TOKEN_RBRACE,           /**< } */
  TOKEN_COMMA,            /**< , */
  TOKEN_COLON,            /**< : */
  TOKEN_SEMICOLON,        /**< ; */
  
  /* Error */
  TOKEN_ERROR             /**< Error token */
} token_type_t;

/**
 * @brief Token structure
 */
typedef struct {
  token_type_t      type;     /**< Token type */
  const char*       text;     /**< Token text */
  uint32_t          length;   /**< Token length */
  uint32_t          line;     /**< Line number */
  uint32_t          column;   /**< Column number */
  
  union {
    int64_t         int_value;   /**< Integer value */
    double          float_value; /**< Floating-point value */
    const char*     string_value; /**< String value */
  } value;                     /**< Token value */
} token_t;

/**
 * @brief Lexer structure
 */
typedef struct lexer lexer_t;

/**
 * @brief Create a new lexer
 * 
 * @param source Source code
 * @param source_length Source code length
 * @param filename Source file name (for error reporting)
 * @param error_context Error context
 * @return The new lexer, or NULL on error
 */
lexer_t* hoil_create_lexer(
  const char* source,
  size_t source_length,
  const char* filename,
  error_context_t* error_context
);

/**
 * @brief Free a lexer
 * 
 * @param lexer The lexer to free
 */
void hoil_free_lexer(lexer_t* lexer);

/**
 * @brief Get the next token
 * 
 * @param lexer The lexer
 * @return The next token
 */
token_t hoil_lexer_next_token(lexer_t* lexer);

/**
 * @brief Peek at the next token without consuming it
 * 
 * @param lexer The lexer
 * @return The next token
 */
token_t hoil_lexer_peek_token(lexer_t* lexer);

/**
 * @brief Get the token type name
 * 
 * @param type Token type
 * @return The token type name
 */
const char* hoil_token_type_name(token_type_t type);

/**
 * @brief Check if a token is a keyword
 * 
 * @param type Token type
 * @return True if the token is a keyword
 */
bool hoil_token_is_keyword(token_type_t type);

/**
 * @brief Check if a token is a type keyword
 * 
 * @param type Token type
 * @return True if the token is a type keyword
 */
bool hoil_token_is_type_keyword(token_type_t type);

/**
 * @brief Get the source location for a token
 * 
 * @param lexer The lexer
 * @param token The token
 * @return The source location
 */
source_location_t hoil_lexer_get_location(lexer_t* lexer, token_t token);

#endif /* HOIL_LEXER_H */