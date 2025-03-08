/*
* Essentialy the grammar of COIL
* helpful when parsing and writing COIL files
*
*/
// --- COIL OPCODE --- //
// SECTION INTERNAL
#define COIL_SECTION_EOF ((uint8_t)0x00)

// SECTION DIRECTIVE
  // for an extra 255 directives if needed via another uint8_t immediatly following this
  #define COIL_DIR_CONT ((uint8_t)0x0F)
  // linker directives
  #define COIL_DIR_SECT ((uint8_t)0x01) // the linker section to place the following in
  #define COIL_DIR_LIB  ((uint8_t)0x02) // a binary file to be linked with
  #define COIL_DIR_INCL ((uint8_t)0x03) // a file which contains extended coil code
  // define system
  #define COIL_DIR_DEF  ((uint8_t)0x04)
  #define COIL_DIR_UDEF ((uint8_t)0x05)
  #define COIL_DIR_IF   ((uint8_t)0x06)
  #define COIL_DIR_ELIF ((uint8_t)0x07)
  #define COIL_DIR_ELSE ((uint8_t)0x08)
  #define COIL_DIR_EIF  ((uint8_t)0x09)
  // program
  #define COIL_DIR_INST ((uint8_t)0x0A) // insert some bytes directly into program memory
  #define COIL_DIR_FILL ((uint8_t)0x0B) // optimized call of repeated insert
  #define COIL_DIR_PADD ((uint8_t)0x0C) // padd program up to instruction byte
  // other
  #define COIL_DIR_ASM  ((uint8_t)0x0D) // parse some raw assembly
  #define COIL_DIR_MATH ((uint8_t)0x0E) // parse a mathify expression
// SECTION VALUE
  #define COIL_VAL_CONT  ((uint8_t)0x01F)
  #define COIL_VAL_IMM   ((uint8_t)0x10)
  #define COIL_VAL_VAR   ((uint8_t)0x11)
  #define COIL_VAL_SYMB  ((uint8_t)0x12)
// SECTION CONTROL FLOW
  #define COIL_CF_CONT   ((uint8_t)0x2F)
  #define COIL_CF_CALL   ((uint8_t)0x20)
  #define COIL_CF_JMP    ((uint8_t)0x21)
  #define COIL_CF_JMPC   ((uint8_t)0x22)
  #define COIL_CF_RET    ((uint8_t)0x23)
  #define COIL_CF_INT    ((uint8_t)0x24)
  #define COIL_CF_SYSC   ((uint8_t)0x25)
  #define COIL_CF_IRET   ((uint8_t)0x26)
  #define COIL_CF_IHLT   ((uint8_t)0x27)
  #define COIL_CF_SEV    ((uint8_t)0x28) // send event (arm sev)
  #define COIL_CF_EHLT   ((uint8_t)0x29) // wait for event (arm wfe)
// SECTION MEMORY
  #define COIL_MEM_CONT  ((uint8_t)0x3F)
  #define COIL_MEM_MOV   ((uint8_t)0x30) // for normal coil values
  #define COIL_MEM_MOVR  ((uint8_t)0x31) // for raw assembly
  #define COIL_MEM_PUSR  ((uint8_t)0x32)
  #define COIL_MEM_POPR  ((uint8_t)0x33)
  #define COIL_MEM_PUSF  ((uint8_t)0x34)
  #define COIL_MEM_POPF  ((uint8_t)0x35)
  #define COIL_MEM_PUSA  ((uint8_t)0x36)
  #define COIL_MEM_POPA  ((uint8_t)0x37)
// SECTION BYTE Operations
  #define COIL_MATH_CONT ((uint8_t)0x4F)
  #define COIL_MATH_ADD  ((uint8_t)0x40)
  #define COIL_MATH_SUB  ((uint8_t)0x41)
  #define COIL_MATH_MUL  ((uint8_t)0x42)
  #define COIL_MATH_DIV  ((uint8_t)0x43)
  #define COIL_MATH_MOD  ((uint8_t)0x44)
  #define COIL_MATH_CMP  ((uint8_t)0x61)
// SECTION BIT
  #define COIL_BIT_CONT  ((uint8_t)0x5F)
  #define COIL_BIT_SL    ((uint8_t)0x50)
  #define COIL_BIT_SR    ((uint8_t)0x51)
  #define COIL_BIT_OR    ((uint8_t)0x52)
  #define COIL_BIT_AND   ((uint8_t)0x53)
  #define COIL_BIT_XOR   ((uint8_t)0x54)
  #define COIL_BIT_TEST  ((uint8_t)0x55)
// other
#define COIL_OPCODE_TYPE uint8_t
#define COIL_OPCODE_PRINT "%02X"

// --- GLOBAL TOKENS --- //
  #define COIL_TOKENS_OF_TYPE_DEF 0xC0
  struct CoilTokType {
    // some magic word to differntiate different token types
    uint16_t magicword;
  };
// --- DEFAULT TOKENS --- //
// Auxiliary
  typedef struct CoilTokType CoilTokType_t;
  struct CoilTokHeader {
    COIL_OPCODE_TYPE type;
    uint64_t size;
  };
  typedef struct CoilTokHeader CoilTokHeader_t;
// Directive Tokens
// Control Tokens
// Value Operands
// Operand Tokens
// --- LEAFS --- //