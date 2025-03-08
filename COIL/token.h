#ifndef BY_GLOBAL_IN_LEX_IMPL
#define BY_GLOBAL_IN_LEX_IMPL

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>


/* Bytey Tokens
* The bytey tokens are generally seperated into 5 main categories depending on the implementation
* this is the basic int and float, complex types (struct), complex objects (class), extensions (vectors), os (abstract system calls (stdlib))
* 
*/

/* Token Types
* Directive Token      (Direct Compiler Communication)
* Value Tokens         (Runtime values)
* Expression Tokens    (Expressions like function calls, operands, for loops, etc...)
* Compiler Time Tokens (A inline expression either to ensure better optimization in header files and for bytey compiler expression compatibility)
* Type Tokens          (Also refered to as template tokens, describe the expected token format for some given parameter)
*/

/*
* The following token types should be able to be used in all bytey files
* including the include files, optimized binary files and object files.
* Optimization should be kept in mind for each token and sub token type
*/

// --- Base Token --- //
struct Token {
  BY_OPCODE_TYPE type;
};

// --- Directive Tokens --- //

// --- Value Tokens --- //
enum BY_VALUE_TYPE {
  BY_VALUE_TYPE_SI,
  BY_VALUE_TYPE_UI,
  BY_VALUE_TYPE_FL,     // float
  BY_VALUE_TYPE_VEC,    // hardware vector
  BY_VALUE_TYPE_FPTR,   // fat pointer (pointer with some size)
  BY_VALUE_TYPE_PTR,    // pointer
  BY_VALUE_TYPE_PACK,   // no padding structure
  BY_VALUE_TYPE_STRUCT  // a collection of types under different offsets
};

struct TokenValOptBaseVal {
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
};

struct TokenValImm {
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
  enum BY_VALUE_COMPONENT component;
  size_t component_information;
  union {
    int64_t i;
    uint64_t u;
    long double f;
  } val;

  size_t bitcount; // the amount of seperate values
  size_t bits; // the amount of bits in a singular of these values
};

// optimized values
struct TokenValOptimVar { // register / stack
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
  unsigned long int reg;
  long int offset;
};
struct TokenValOptimAddr {
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
  unsigned long long int data; // program offset or location in symbol table
  bool issymbol;
};

// meta values
struct TokenValMetaVar { // register / stack
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
  uint64_t ID; // some unique identifier in the scope
};
struct TokenValMetaAddr {
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
};

// --- Expression Tokens --- //
struct TokenRtCtrl {
  BY_OPCODE_TYPE type;
  struct Token *address;
};
struct TokenRtOperand {
  BY_OPCODE_TYPE type;
  struct Token *operand1;
  struct Token *operand2;
  struct Token *result;
};

// --- Compiler Time Tokens --- //
struct TokenCtVariable {
  BY_OPCODE_TYPE type;
  enum BY_VALUE_TYPE valtype;
  uint64_t ID;
};

#ifdef __cplusplus
}
#endif

#endif // BY_GLOBAL_IN_LEX_IMPL