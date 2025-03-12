/**
* @file error.c
* @brief Error handling implementation for COIL/HOIL
*/

#include "common/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define MAX_ERROR_CALLBACKS 8
#define MAX_ERROR_HISTORY 16

/**
* @brief Error callback entry
*/
typedef struct {
  error_callback_t callback;
  void*            user_data;
} error_callback_entry_t;

/**
* @brief Error context structure
*/
struct error_context {
  error_callback_entry_t callbacks[MAX_ERROR_CALLBACKS];
  uint32_t               callback_count;
  error_message_t        history[MAX_ERROR_HISTORY];
  uint32_t               history_count;
  uint32_t               error_count;
  uint32_t               warning_count;
  uint32_t               info_count;
};

/**
* @brief Error messages for common error codes
*/
static const char* error_messages[] = {
  /* General errors */
  "Unknown error",
  "Not implemented",
  "Invalid argument",
  "Out of memory",
  
  /* Parser errors */
  "Unexpected token",
  "Expected token not found",
  "Invalid syntax",
  "Unterminated construct",
  "Unexpected end of file",
  
  /* Type errors */
  "Unknown type",
  "Type mismatch",
  "Incompatible types",
  "Invalid type",
  "Type not found",
  
  /* Semantic errors */
  "Undefined identifier",
  "Redefined identifier",
  "Invalid operand",
  "Invalid control flow",
  "Unreachable code",
  
  /* Code generation errors */
  "Invalid IR",
  "Unsupported feature",
  "Expression too complex",
  
  /* Binary format errors */
  "Invalid binary format",
  "Unsupported version",
  "Corrupt binary data",
  "Missing section",
  
  /* Assembler errors */
  "Unsupported target architecture",
  "No target specified",
  "Bad instruction mapping",
  
  /* System errors */
  "I/O error",
  "Memory allocation error",
  "Resource unavailable"
};

error_context_t* coil_create_error_context(void) {
  error_context_t* context = (error_context_t*)malloc(sizeof(error_context_t));
  if (context == NULL) {
    return NULL;
  }
  
  memset(context, 0, sizeof(error_context_t));
  return context;
}

void coil_free_error_context(error_context_t* context) {
  if (context == NULL) {
    return;
  }
  
  // Free any dynamically allocated error messages
  for (uint32_t i = 0; i < context->history_count; i++) {
    free((void*)context->history[i].message);
    free((void*)context->history[i].location.filename);
  }
  
  free(context);
}

int coil_register_error_callback(
  error_context_t* context,
  error_callback_t callback,
  void* user_data
) {
  if (context == NULL || callback == NULL) {
    return -1;
  }
  
  if (context->callback_count >= MAX_ERROR_CALLBACKS) {
    return -1;
  }
  
  context->callbacks[context->callback_count].callback = callback;
  context->callbacks[context->callback_count].user_data = user_data;
  context->callback_count++;
  
  return 0;
}

static void add_error_to_history(
  error_context_t* context,
  const error_message_t* error
) {
  if (context->history_count >= MAX_ERROR_HISTORY) {
    // Remove the oldest error
    free((void*)context->history[0].message);
    free((void*)context->history[0].location.filename);
    
    // Shift all errors one position left
    memmove(&context->history[0], &context->history[1], 
            (MAX_ERROR_HISTORY - 1) * sizeof(error_message_t));
    
    context->history_count = MAX_ERROR_HISTORY - 1;
  }
  
  // Add new error at the end
  error_message_t* new_error = &context->history[context->history_count++];
  
  new_error->severity = error->severity;
  new_error->category = error->category;
  new_error->code = error->code;
  new_error->message = error->message ? strdup(error->message) : NULL;
  
  if (error->location.filename) {
    new_error->location.filename = strdup(error->location.filename);
  } else {
    new_error->location.filename = NULL;
  }
  
  new_error->location.line = error->location.line;
  new_error->location.column = error->location.column;
}

void coil_report_error(
  error_context_t* context,
  error_severity_t severity,
  error_category_t category,
  uint32_t code,
  const char* message,
  const char* filename,
  uint32_t line,
  uint32_t column
) {
  if (context == NULL) {
    return;
  }
  
  // Create error message
  error_message_t error;
  error.severity = severity;
  error.category = category;
  error.code = code;
  error.message = message;
  error.location.filename = filename;
  error.location.line = line;
  error.location.column = column;
  
  // Update error counts
  switch (severity) {
    case ERROR_INFO:
      context->info_count++;
      break;
    case ERROR_WARNING:
      context->warning_count++;
      break;
    case ERROR_ERROR:
    case ERROR_INTERNAL:
      context->error_count++;
      break;
  }
  
  // Add to history
  add_error_to_history(context, &error);
  
  // Call all registered callbacks
  for (uint32_t i = 0; i < context->callback_count; i++) {
    context->callbacks[i].callback(&error, context->callbacks[i].user_data);
  }
  
  // Default error handling if no callbacks are registered
  if (context->callback_count == 0) {
    const char* severity_str = "Unknown";
    switch (severity) {
      case ERROR_INFO:
        severity_str = "Info";
        break;
      case ERROR_WARNING:
        severity_str = "Warning";
        break;
      case ERROR_ERROR:
        severity_str = "Error";
        break;
      case ERROR_INTERNAL:
        severity_str = "Internal Error";
        break;
    }
    
    if (filename) {
      fprintf(stderr, "%s: %s:%u:%u: %s\n", 
              severity_str, filename, line, column, message);
    } else {
      fprintf(stderr, "%s: %s\n", severity_str, message);
    }
  }
}

uint32_t coil_get_error_count(error_context_t* context, error_severity_t severity) {
  if (context == NULL) {
    return 0;
  }
  
  switch (severity) {
    case ERROR_INFO:
      return context->info_count;
    case ERROR_WARNING:
      return context->warning_count;
    case ERROR_ERROR:
    case ERROR_INTERNAL:
      return context->error_count;
    default:
      return 0;
  }
}

void coil_clear_errors(error_context_t* context) {
  if (context == NULL) {
    return;
  }
  
  // Free any dynamically allocated error messages
  for (uint32_t i = 0; i < context->history_count; i++) {
    free((void*)context->history[i].message);
    free((void*)context->history[i].location.filename);
  }
  
  context->history_count = 0;
  context->error_count = 0;
  context->warning_count = 0;
  context->info_count = 0;
}

const error_message_t* coil_get_last_error(error_context_t* context) {
  if (context == NULL || context->history_count == 0) {
    return NULL;
  }
  
  return &context->history[context->history_count - 1];
}

const char* coil_get_error_message(error_category_t category, uint32_t code) {
  // Simplified lookup based on fixed error codes
  // In a real implementation, we'd use a more flexible approach
  
  if (category == ERROR_CATEGORY_GENERAL && code <= ERROR_GENERAL_OUT_OF_MEMORY) {
    return error_messages[code];
  } else if (category == ERROR_CATEGORY_PARSER && 
            code >= ERROR_PARSER_UNEXPECTED_TOKEN &&
            code <= ERROR_PARSER_UNEXPECTED_EOF) {
    return error_messages[code - ERROR_PARSER_UNEXPECTED_TOKEN + 4];
  } else if (category == ERROR_CATEGORY_TYPE &&
            code >= ERROR_TYPE_UNKNOWN &&
            code <= ERROR_TYPE_NOT_FOUND) {
    return error_messages[code - ERROR_TYPE_UNKNOWN + 9];
  } else if (category == ERROR_CATEGORY_SEMANTIC &&
            code >= ERROR_SEMANTIC_UNDEFINED &&
            code <= ERROR_SEMANTIC_UNREACHABLE) {
    return error_messages[code - ERROR_SEMANTIC_UNDEFINED + 14];
  } else if (category == ERROR_CATEGORY_CODEGEN &&
            code >= ERROR_CODEGEN_INVALID_IR &&
            code <= ERROR_CODEGEN_TOO_COMPLEX) {
    return error_messages[code - ERROR_CODEGEN_INVALID_IR + 19];
  } else if (category == ERROR_CATEGORY_BINARY &&
            code >= ERROR_BINARY_INVALID_FORMAT &&
            code <= ERROR_BINARY_MISSING_SECTION) {
    return error_messages[code - ERROR_BINARY_INVALID_FORMAT + 22];
  } else if (category == ERROR_CATEGORY_ASSEMBLER &&
            code >= ERROR_ASSEMBLER_UNSUPPORTED &&
            code <= ERROR_ASSEMBLER_BAD_MAPPING) {
    return error_messages[code - ERROR_ASSEMBLER_UNSUPPORTED + 26];
  } else if (category == ERROR_CATEGORY_SYSTEM &&
            code >= ERROR_SYSTEM_IO &&
            code <= ERROR_SYSTEM_RESOURCE) {
    return error_messages[code - ERROR_SYSTEM_IO + 29];
  }
  
  return "Unknown error";
}