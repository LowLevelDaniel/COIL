/**
* @file error.h
* @brief Error handling for COIL/HOIL
*
* This file defines the error handling system used by both the HOIL compiler
* and the COIL assembler.
*/

#ifndef COIL_ERROR_H
#define COIL_ERROR_H

#include <stdint.h>

/**
* @brief Error severity levels
*/
typedef enum {
  ERROR_INFO,     /**< Informational message */
  ERROR_WARNING,  /**< Warning (non-fatal) */
  ERROR_ERROR,    /**< Error (fatal) */
  ERROR_INTERNAL  /**< Internal error (fatal) */
} error_severity_t;

/**
* @brief Error categories
*/
typedef enum {
  ERROR_CATEGORY_GENERAL,     /**< General errors */
  ERROR_CATEGORY_PARSER,      /**< Parser errors */
  ERROR_CATEGORY_TYPE,        /**< Type system errors */
  ERROR_CATEGORY_SEMANTIC,    /**< Semantic analysis errors */
  ERROR_CATEGORY_CODEGEN,     /**< Code generation errors */
  ERROR_CATEGORY_BINARY,      /**< Binary format errors */
  ERROR_CATEGORY_ASSEMBLER,   /**< Assembler errors */
  ERROR_CATEGORY_SYSTEM       /**< System errors (I/O, memory, etc.) */
} error_category_t;

/**
* @brief Source location information
*/
typedef struct {
  const char* filename;  /**< Source file name */
  uint32_t    line;      /**< Line number */
  uint32_t    column;    /**< Column number */
} source_location_t;

/**
* @brief Error message
*/
typedef struct {
  error_severity_t  severity;  /**< Error severity */
  error_category_t  category;  /**< Error category */
  uint32_t          code;      /**< Error code */
  const char*       message;   /**< Error message */
  source_location_t location;  /**< Source location */
} error_message_t;

/**
* @brief Error context
*/
typedef struct error_context error_context_t;

/**
* @brief Error callback function type
*/
typedef void (*error_callback_t)(const error_message_t* error, void* user_data);

/**
* @brief Create a new error context
* 
* @return The new error context
*/
error_context_t* coil_create_error_context(void);

/**
* @brief Free an error context
* 
* @param context The error context to free
*/
void coil_free_error_context(error_context_t* context);

/**
* @brief Register an error callback
* 
* @param context The error context
* @param callback The callback function
* @param user_data User data to pass to the callback
* @return 0 on success, -1 on error
*/
int coil_register_error_callback(
  error_context_t* context,
  error_callback_t callback,
  void* user_data
);

/**
* @brief Report an error
* 
* @param context The error context
* @param severity Error severity
* @param category Error category
* @param code Error code
* @param message Error message
* @param filename Source file name
* @param line Line number
* @param column Column number
*/
void coil_report_error(
  error_context_t* context,
  error_severity_t severity,
  error_category_t category,
  uint32_t code,
  const char* message,
  const char* filename,
  uint32_t line,
  uint32_t column
);

/**
* @brief Get the number of errors of a given severity
* 
* @param context The error context
* @param severity Error severity
* @return Number of errors
*/
uint32_t coil_get_error_count(error_context_t* context, error_severity_t severity);

/**
* @brief Clear all errors
* 
* @param context The error context
*/
void coil_clear_errors(error_context_t* context);

/**
* @brief Get the last error message
* 
* @param context The error context
* @return The last error message, or NULL if none
*/
const error_message_t* coil_get_last_error(error_context_t* context);

/**
* @brief Get the error message for an error code
* 
* @param category Error category
* @param code Error code
* @return The error message
*/
const char* coil_get_error_message(error_category_t category, uint32_t code);

/* Common error codes */

/* General errors (0-99) */
#define ERROR_GENERAL_UNKNOWN          0
#define ERROR_GENERAL_NOT_IMPLEMENTED  1
#define ERROR_GENERAL_INVALID_ARGUMENT 2
#define ERROR_GENERAL_OUT_OF_MEMORY    3

/* Parser errors (100-199) */
#define ERROR_PARSER_UNEXPECTED_TOKEN  100
#define ERROR_PARSER_EXPECTED_TOKEN    101
#define ERROR_PARSER_INVALID_SYNTAX    102
#define ERROR_PARSER_UNTERMINATED      103
#define ERROR_PARSER_UNEXPECTED_EOF    104

/* Type errors (200-299) */
#define ERROR_TYPE_UNKNOWN             200
#define ERROR_TYPE_MISMATCH            201
#define ERROR_TYPE_INCOMPATIBLE        202
#define ERROR_TYPE_INVALID             203
#define ERROR_TYPE_NOT_FOUND           204

/* Semantic errors (300-399) */
#define ERROR_SEMANTIC_UNDEFINED       300
#define ERROR_SEMANTIC_REDEFINED       301
#define ERROR_SEMANTIC_INVALID_OPERAND 302
#define ERROR_SEMANTIC_INVALID_CONTROL 303
#define ERROR_SEMANTIC_UNREACHABLE     304

/* Code generation errors (400-499) */
#define ERROR_CODEGEN_INVALID_IR       400
#define ERROR_CODEGEN_UNSUPPORTED      401
#define ERROR_CODEGEN_TOO_COMPLEX      402

/* Binary format errors (500-599) */
#define ERROR_BINARY_INVALID_FORMAT    500
#define ERROR_BINARY_UNSUPPORTED_VER   501
#define ERROR_BINARY_CORRUPT           502
#define ERROR_BINARY_MISSING_SECTION   503

/* Assembler errors (600-699) */
#define ERROR_ASSEMBLER_UNSUPPORTED    600
#define ERROR_ASSEMBLER_NO_TARGET      601
#define ERROR_ASSEMBLER_BAD_MAPPING    602

/* System errors (700-799) */
#define ERROR_SYSTEM_IO                700
#define ERROR_SYSTEM_MEMORY            701
#define ERROR_SYSTEM_RESOURCE          702

/* Convenience macros */
#define REPORT_ERROR(ctx, sev, cat, code, msg, file, line, col) \
  coil_report_error(ctx, sev, cat, code, msg, file, line, col)

#define REPORT_WARNING(ctx, cat, code, msg, file, line, col) \
  coil_report_error(ctx, ERROR_WARNING, cat, code, msg, file, line, col)

#define REPORT_FATAL(ctx, cat, code, msg, file, line, col) \
  coil_report_error(ctx, ERROR_ERROR, cat, code, msg, file, line, col)

#define REPORT_INTERNAL(ctx, msg, file, line, col) \
  coil_report_error(ctx, ERROR_INTERNAL, ERROR_CATEGORY_GENERAL, \
                  ERROR_GENERAL_UNKNOWN, msg, file, line, col)

#endif /* COIL_ERROR_H */