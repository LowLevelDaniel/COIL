/**
 * @file instruction_decoder.c
 * @brief COIL instruction decoder implementation
 */

#include "coil_assembler/instruction_decoder.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Instruction decoder structure
 */
struct instruction_decoder {
  error_context_t* error_context;  /**< Error context */
  bool             had_error;      /**< True if an error occurred */
};

/**
 * @brief Opcode mnemonic table
 */
static const char* opcode_mnemonics[] = {
  /* 0x00 */ NULL,
  /* 0x01 */ "ADD",
  /* 0x02 */ "SUB",
  /* 0x03 */ "MUL",
  /* 0x04 */ "DIV",
  /* 0x05 */ "REM",
  /* 0x06 */ "NEG",
  /* 0x07 */ "ABS",
  /* 0x08 */ "MIN",
  /* 0x09 */ "MAX",
  /* 0x0A */ "FMA",
  /* 0x0B */ NULL,
  /* 0x0C */ NULL,
  /* 0x0D */ NULL,
  /* 0x0E */ NULL,
  /* 0x0F */ NULL,
  
  /* 0x10 */ "AND",
  /* 0x11 */ "OR",
  /* 0x12 */ "XOR",
  /* 0x13 */ "NOT",
  /* 0x14 */ "SHL",
  /* 0x15 */ "SHR",
  /* 0x16 */ NULL,
  /* 0x17 */ NULL,
  /* 0x18 */ NULL,
  /* 0x19 */ NULL,
  /* 0x1A */ NULL,
  /* 0x1B */ NULL,
  /* 0x1C */ NULL,
  /* 0x1D */ NULL,
  /* 0x1E */ NULL,
  /* 0x1F */ NULL,
  
  /* 0x20 */ "CMP_EQ",
  /* 0x21 */ "CMP_NE",
  /* 0x22 */ "CMP_LT",
  /* 0x23 */ "CMP_LE",
  /* 0x24 */ "CMP_GT",
  /* 0x25 */ "CMP_GE",
  /* 0x26 */ NULL,
  /* 0x27 */ NULL,
  /* 0x28 */ NULL,
  /* 0x29 */ NULL,
  /* 0x2A */ NULL,
  /* 0x2B */ NULL,
  /* 0x2C */ NULL,
  /* 0x2D */ NULL,
  /* 0x2E */ NULL,
  /* 0x2F */ NULL,
  
  /* 0x30 */ "LOAD",
  /* 0x31 */ "STORE",
  /* 0x32 */ "ATOMIC_OP",
  /* 0x33 */ "FENCE",
  /* 0x34 */ "LEA",
  /* 0x35 */ NULL,
  /* 0x36 */ NULL,
  /* 0x37 */ NULL,
  /* 0x38 */ NULL,
  /* 0x39 */ NULL,
  /* 0x3A */ NULL,
  /* 0x3B */ NULL,
  /* 0x3C */ NULL,
  /* 0x3D */ NULL,
  /* 0x3E */ NULL,
  /* 0x3F */ NULL,
  
  /* 0x40 */ "BR",
  /* 0x41 */ "SWITCH",
  /* 0x42 */ "CALL",
  /* 0x43 */ "RET",
  /* 0x44 */ NULL,
  /* 0x45 */ NULL,
  /* 0x46 */ NULL,
  /* 0x47 */ NULL,
  /* 0x48 */ NULL,
  /* 0x49 */ NULL,
  /* 0x4A */ NULL,
  /* 0x4B */ NULL,
  /* 0x4C */ NULL,
  /* 0x4D */ NULL,
  /* 0x4E */ NULL,
  /* 0x4F */ NULL,
  
  /* 0x50 */ "CONVERT",
  /* 0x51 */ "TRUNC",
  /* 0x52 */ "EXTEND",
  /* 0x53 */ NULL,
  /* 0x54 */ NULL,
  /* 0x55 */ NULL,
  /* 0x56 */ NULL,
  /* 0x57 */ NULL,
  /* 0x58 */ NULL,
  /* 0x59 */ NULL,
  /* 0x5A */ NULL,
  /* 0x5B */ NULL,
  /* 0x5C */ NULL,
  /* 0x5D */ NULL,
  /* 0x5E */ NULL,
  /* 0x5F */ NULL,
  
  /* 0x60 */ "VADD",
  /* 0x61 */ "VSUB",
  /* 0x62 */ "VMUL",
  /* 0x63 */ "VDIV",
  /* 0x64 */ "VDOT",
  /* 0x65 */ "VCROSS",
  /* 0x66 */ "VSPLAT",
  /* 0x67 */ "VEXTRACT",
  /* 0x68 */ "VINSERT",
  /* 0x69 */ NULL,
  /* 0x6A */ NULL,
  /* 0x6B */ NULL,
  /* 0x6C */ NULL,
  /* 0x6D */ NULL,
  /* 0x6E */ NULL,
  /* 0x6F */ NULL,
  
  /* 0x70 */ "LOAD_I8",
  /* 0x71 */ "LOAD_I16",
  /* 0x72 */ "LOAD_I32",
  /* 0x73 */ "LOAD_I64",
  /* 0x74 */ "LOAD_F32",
  /* 0x75 */ "LOAD_F64",
  /* 0x76 */ "UNDEF",
  /* 0x77 */ NULL,
  /* 0x78 */ NULL,
  /* 0x79 */ NULL,
  /* 0x7A */ NULL,
  /* 0x7B */ NULL,
  /* 0x7C */ NULL,
  /* 0x7D */ NULL,
  /* 0x7E */ NULL,
  /* 0x7F */ NULL,
  
  /* 0x80-0xEF */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  
  /* 0xF0 */ "HLT",
  /* 0xF1 */ "NOP",
  /* 0xF2 */ "TRAP",
  /* 0xF3 */ "UNREACHABLE",
  /* 0xF4-0xFF */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL
};

/**
 * @brief Expected operand counts for each opcode
 * A value of 255 means variable operand count
 */
static const uint8_t opcode_operand_counts[] = {
  /* 0x00 */ 0,
  /* 0x01 */ 2,  // ADD
  /* 0x02 */ 2,  // SUB
  /* 0x03 */ 2,  // MUL
  /* 0x04 */ 2,  // DIV
  /* 0x05 */ 2,  // REM
  /* 0x06 */ 1,  // NEG
  /* 0x07 */ 1,  // ABS
  /* 0x08 */ 2,  // MIN
  /* 0x09 */ 2,  // MAX
  /* 0x0A */ 3,  // FMA
  /* 0x0B */ 0,
  /* 0x0C */ 0,
  /* 0x0D */ 0,
  /* 0x0E */ 0,
  /* 0x0F */ 0,
  
  /* 0x10 */ 2,  // AND
  /* 0x11 */ 2,  // OR
  /* 0x12 */ 2,  // XOR
  /* 0x13 */ 1,  // NOT
  /* 0x14 */ 2,  // SHL
  /* 0x15 */ 2,  // SHR
  /* 0x16 */ 0,
  /* 0x17 */ 0,
  /* 0x18 */ 0,
  /* 0x19 */ 0,
  /* 0x1A */ 0,
  /* 0x1B */ 0,
  /* 0x1C */ 0,
  /* 0x1D */ 0,
  /* 0x1E */ 0,
  /* 0x1F */ 0,
  
  /* 0x20 */ 2,  // CMP_EQ
  /* 0x21 */ 2,  // CMP_NE
  /* 0x22 */ 2,  // CMP_LT
  /* 0x23 */ 2,  // CMP_LE
  /* 0x24 */ 2,  // CMP_GT
  /* 0x25 */ 2,  // CMP_GE
  /* 0x26 */ 0,
  /* 0x27 */ 0,
  /* 0x28 */ 0,
  /* 0x29 */ 0,
  /* 0x2A */ 0,
  /* 0x2B */ 0,
  /* 0x2C */ 0,
  /* 0x2D */ 0,
  /* 0x2E */ 0,
  /* 0x2F */ 0,
  
  /* 0x30 */ 1,  // LOAD
  /* 0x31 */ 2,  // STORE
  /* 0x32 */ 255,// ATOMIC_OP (variable)
  /* 0x33 */ 1,  // FENCE
  /* 0x34 */ 2,  // LEA
  /* 0x35 */ 0,
  /* 0x36 */ 0,
  /* 0x37 */ 0,
  /* 0x38 */ 0,
  /* 0x39 */ 0,
  /* 0x3A */ 0,
  /* 0x3B */ 0,
  /* 0x3C */ 0,
  /* 0x3D */ 0,
  /* 0x3E */ 0,
  /* 0x3F */ 0,
  
  /* 0x40 */ 255,// BR (variable, 1 or 3)
  /* 0x41 */ 255,// SWITCH (variable)
  /* 0x42 */ 255,// CALL (variable)
  /* 0x43 */ 255,// RET (0 or 1)
  /* 0x44 */ 0,
  /* 0x45 */ 0,
  /* 0x46 */ 0,
  /* 0x47 */ 0,
  /* 0x48 */ 0,
  /* 0x49 */ 0,
  /* 0x4A */ 0,
  /* 0x4B */ 0,
  /* 0x4C */ 0,
  /* 0x4D */ 0,
  /* 0x4E */ 0,
  /* 0x4F */ 0,
  
  /* 0x50 */ 2,  // CONVERT
  /* 0x51 */ 2,  // TRUNC
  /* 0x52 */ 2,  // EXTEND
  /* 0x53 */ 0,
  /* 0x54 */ 0,
  /* 0x55 */ 0,
  /* 0x56 */ 0,
  /* 0x57 */ 0,
  /* 0x58 */ 0,
  /* 0x59 */ 0,
  /* 0x5A */ 0,
  /* 0x5B */ 0,
  /* 0x5C */ 0,
  /* 0x5D */ 0,
  /* 0x5E */ 0,
  /* 0x5F */ 0,
  
  /* 0x60 */ 2,  // VADD
  /* 0x61 */ 2,  // VSUB
  /* 0x62 */ 2,  // VMUL
  /* 0x63 */ 2,  // VDIV
  /* 0x64 */ 2,  // VDOT
  /* 0x65 */ 2,  // VCROSS
  /* 0x66 */ 2,  // VSPLAT
  /* 0x67 */ 2,  // VEXTRACT
  /* 0x68 */ 3,  // VINSERT
  /* 0x69 */ 0,
  /* 0x6A */ 0,
  /* 0x6B */ 0,
  /* 0x6C */ 0,
  /* 0x6D */ 0,
  /* 0x6E */ 0,
  /* 0x6F */ 0,
  
  /* 0x70 */ 1,  // LOAD_I8
  /* 0x71 */ 1,  // LOAD_I16
  /* 0x72 */ 1,  // LOAD_I32
  /* 0x73 */ 1,  // LOAD_I64
  /* 0x74 */ 1,  // LOAD_F32
  /* 0x75 */ 1,  // LOAD_F64
  /* 0x76 */ 1,  // UNDEF
  /* 0x77 */ 0,
  /* 0x78 */ 0,
  /* 0x79 */ 0,
  /* 0x7A */ 0,
  /* 0x7B */ 0,
  /* 0x7C */ 0,
  /* 0x7D */ 0,
  /* 0x7E */ 0,
  /* 0x7F */ 0,
  
  /* 0x80-0xEF */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  
  /* 0xF0 */ 0,  // HLT
  /* 0xF1 */ 0,  // NOP
  /* 0xF2 */ 0,  // TRAP
  /* 0xF3 */ 0,  // UNREACHABLE
  /* 0xF4-0xFF */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

instruction_decoder_t* coil_create_instruction_decoder(error_context_t* error_context) {
  instruction_decoder_t* decoder = (instruction_decoder_t*)malloc(sizeof(instruction_decoder_t));
  if (decoder == NULL) {
    return NULL;
  }
  
  decoder->error_context = error_context;
  decoder->had_error = false;
  
  return decoder;
}

void coil_free_instruction_decoder(instruction_decoder_t* decoder) {
  if (decoder == NULL) {
    return;
  }
  
  free(decoder);
}

/**
 * @brief Report an instruction decoder error
 * 
 * @param decoder The decoder
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  instruction_decoder_t* decoder,
  uint32_t code,
  const char* message
) {
  if (decoder->error_context != NULL) {
    REPORT_ERROR(decoder->error_context, ERROR_ERROR, ERROR_CATEGORY_BINARY,
                code, message, NULL, 0, 0);
  }
  
  decoder->had_error = true;
}

/**
 * @brief Decode an operand from binary data
 * 
 * @param decoder The decoder
 * @param data Binary data
 * @param size Data size
 * @param offset Offset into data
 * @param module The module
 * @param function The function
 * @param operand Output operand
 * @return Size of the operand in bytes, or 0 on error
 */
static uint32_t decode_operand(
  instruction_decoder_t* decoder,
  const uint8_t* data,
  size_t size,
  uint32_t offset,
  module_t* module,
  function_t* function,
  instruction_operand_t* operand
) {
  if (offset + 1 > size) {
    report_error(decoder, ERROR_BINARY_CORRUPT, "Operand truncated");
    return 0;
  }
  
  uint8_t kind = data[offset];
  
  switch (kind) {
    case OPERAND_REGISTER: {
      if (offset + 4 > size) {
        report_error(decoder, ERROR_BINARY_CORRUPT, "Register operand truncated");
        return 0;
      }
      
      uint32_t reg = data[offset + 1] |
                    (data[offset + 2] << 8) |
                    (data[offset + 3] << 16);
      
      // For now, use a placeholder type
      *operand = coil_create_reg_operand(reg, COIL_TYPE_INT32);
      
      return 4;
    }
    
    case OPERAND_IMMEDIATE: {
      if (offset + 4 > size) {
        report_error(decoder, ERROR_BINARY_CORRUPT, "Immediate operand truncated");
        return 0;
      }
      
      int32_t imm = data[offset + 1] |
                   (data[offset + 2] << 8) |
                   (data[offset + 3] << 16);
      
      // For now, use a placeholder type
      *operand = coil_create_imm_operand(imm, COIL_TYPE_INT32);
      
      return 4;
    }
    
    case OPERAND_BASIC_BLOCK: {
      // Basic block references are stored as string indices
      // For now, we'll use a placeholder
      *operand = coil_create_block_operand("block");
      
      return 4;
    }
    
    case OPERAND_FUNCTION: {
      // Function references are stored as string indices
      // For now, we'll use a placeholder
      *operand = coil_create_func_operand("function", COIL_TYPE_VOID);
      
      return 4;
    }
    
    case OPERAND_GLOBAL: {
      // Global references are stored as string indices
      // For now, we'll use a placeholder
      *operand = coil_create_global_operand("global", COIL_TYPE_INT32);
      
      return 4;
    }
    
    case OPERAND_MEMORY: {
      if (offset + 8 > size) {
        report_error(decoder, ERROR_BINARY_CORRUPT, "Memory operand truncated");
        return 0;
      }
      
      uint32_t base = data[offset + 1];
      int32_t offset_val = *(int32_t*)(data + offset + 2);
      uint8_t index = data[offset + 6];
      uint8_t scale = data[offset + 7];
      
      // For now, use a placeholder type
      *operand = coil_create_mem_operand(base, offset_val, index, scale, COIL_TYPE_INT32);
      
      return 8;
    }
    
    default:
      report_error(decoder, ERROR_BINARY_CORRUPT, "Invalid operand kind");
      return 0;
  }
}

instruction_t* coil_decode_instruction(
  instruction_decoder_t* decoder,
  const uint8_t* data,
  size_t size,
  module_t* module,
  function_t* function
) {
  if (decoder == NULL || data == NULL || size == 0) {
    return NULL;
  }
  
  decoder->had_error = false;
  
  // Basic format:
  // [opcode:8][flags:8][operand count:8][destination:8][operands...]
  
  // Check minimum instruction size
  if (size < 4) {
    report_error(decoder, ERROR_BINARY_CORRUPT, "Instruction too small");
    return NULL;
  }
  
  // Decode header
  uint8_t opcode = data[0];
  uint8_t flags = data[1];
  uint8_t operand_count = data[2];
  uint8_t dest_reg = data[3];
  
  // Validate opcode
  if (!coil_is_valid_instruction(opcode, operand_count)) {
    char error_msg[64];
    snprintf(error_msg, sizeof(error_msg), 
             "Invalid instruction: opcode=%02X, operand_count=%u",
             opcode, operand_count);
    report_error(decoder, ERROR_BINARY_CORRUPT, error_msg);
    return NULL;
  }
  
  // Decode destination operand (register)
  instruction_operand_t dest = coil_create_reg_operand(dest_reg, COIL_TYPE_INT32);
  
  // Decode operands
  instruction_operand_t* operands = NULL;
  if (operand_count > 0) {
    operands = (instruction_operand_t*)malloc(operand_count * sizeof(instruction_operand_t));
    if (operands == NULL) {
      report_error(decoder, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
      return NULL;
    }
    
    uint32_t offset = 4;
    for (uint8_t i = 0; i < operand_count; i++) {
      uint32_t operand_size = decode_operand(
        decoder,
        data,
        size,
        offset,
        module,
        function,
        &operands[i]
      );
      
      if (operand_size == 0) {
        // Error already reported
        free(operands);
        return NULL;
      }
      
      offset += operand_size;
    }
  }
  
  // Create instruction
  instruction_t* instr = coil_create_instruction(
    (opcode_t)opcode,
    flags,
    dest,
    operands,
    operand_count,
    COIL_TYPE_INT32,  // Placeholder type
    NULL
  );
  
  // Free operands array (they've been copied into the instruction)
  free(operands);
  
  return instr;
}

int coil_decode_function_instructions(
  instruction_decoder_t* decoder,
  const uint8_t* data,
  size_t size,
  module_t* module,
  function_t* function
) {
  if (decoder == NULL || data == NULL || function == NULL) {
    return -1;
  }
  
  // Not yet implemented
  report_error(decoder, ERROR_GENERAL_NOT_IMPLEMENTED,
              "Function instruction decoding not yet implemented");
  return -1;
}

uint32_t coil_get_instruction_size(const uint8_t* data, size_t size) {
  if (data == NULL || size < 4) {
    return 0;
  }
  
  // We only have a simplified implementation for now
  // In a real implementation, we would compute the actual size
  // based on the operand types
  
  return 4;  // Minimum instruction size (header only)
}

bool coil_is_valid_instruction(uint8_t opcode, uint8_t operand_count) {
  // Check if opcode has a mnemonic
  if (opcode >= sizeof(opcode_mnemonics) / sizeof(opcode_mnemonics[0]) ||
      opcode_mnemonics[opcode] == NULL) {
    return false;
  }
  
  // Check operand count
  uint8_t expected_count = opcode_operand_counts[opcode];
  if (expected_count == 255) {
    // Variable operand count - we'll assume it's valid
    return true;
  }
  
  return expected_count == operand_count;
}

const char* coil_get_opcode_mnemonic(uint8_t opcode) {
  if (opcode >= sizeof(opcode_mnemonics) / sizeof(opcode_mnemonics[0])) {
    return NULL;
  }
  
  return opcode_mnemonics[opcode];
}

int coil_print_instruction(
  const instruction_t* instr,
  char* buffer,
  size_t size
) {
  if (instr == NULL || buffer == NULL || size == 0) {
    return -1;
  }
  
  // Get opcode mnemonic
  const char* mnemonic = coil_get_opcode_mnemonic(instr->opcode);
  if (mnemonic == NULL) {
    snprintf(buffer, size, "<unknown opcode: %02X>", instr->opcode);
    return strlen(buffer);
  }
  
  // Format based on instruction type
  int len = 0;
  
  // We'll need a more sophisticated implementation for a real assembler
  // This is just a simplified version for demonstration
  
  if (instr->dest.kind == OPERAND_REGISTER && instr->dest.value.reg != 0) {
    // Instructions that produce a result
    len = snprintf(buffer, size, "%s = %s ", 
                 instr->result_name ? instr->result_name : "result", 
                 mnemonic);
  } else {
    // Instructions that don't produce a result
    len = snprintf(buffer, size, "%s ", mnemonic);
  }
  
  if (len < 0 || (size_t)len >= size) {
    return -1;
  }
  
  // Add operands
  int remaining = size - len;
  char* pos = buffer + len;
  
  for (uint8_t i = 0; i < instr->operand_count; i++) {
    const instruction_operand_t* op = &instr->operands[i];
    int op_len = 0;
    
    switch (op->kind) {
      case OPERAND_REGISTER:
        op_len = snprintf(pos, remaining, "r%u", op->value.reg);
        break;
        
      case OPERAND_IMMEDIATE:
        op_len = snprintf(pos, remaining, "%d", (int)op->value.imm);
        break;
        
      case OPERAND_BASIC_BLOCK:
        op_len = snprintf(pos, remaining, "%s", op->value.block);
        break;
        
      case OPERAND_FUNCTION:
        op_len = snprintf(pos, remaining, "%s", op->value.func);
        break;
        
      case OPERAND_GLOBAL:
        op_len = snprintf(pos, remaining, "%s", op->value.global);
        break;
        
      case OPERAND_MEMORY:
        if (op->value.mem.index == 0) {
          // Simple memory operand
          op_len = snprintf(pos, remaining, "[r%u + %d]", 
                         op->value.mem.base, op->value.mem.offset);
        } else {
          // Memory operand with index
          op_len = snprintf(pos, remaining, "[r%u + r%u * %u + %d]", 
                         op->value.mem.base, op->value.mem.index, 
                         op->value.mem.scale, op->value.mem.offset);
        }
        break;
        
      default:
        op_len = snprintf(pos, remaining, "<unknown>");
        break;
    }
    
    if (op_len < 0 || op_len >= remaining) {
      return -1;
    }
    
    pos += op_len;
    remaining -= op_len;
    
    // Add comma if not the last operand
    if (i < instr->operand_count - 1) {
      if (remaining < 2) {
        return -1;
      }
      *pos++ = ',';
      *pos++ = ' ';
      remaining -= 2;
    }
  }
  
  return pos - buffer;
}