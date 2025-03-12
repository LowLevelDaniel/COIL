/**
* @file instruction.c
* @brief Instruction set implementation for COIL/HOIL
*/

#include "common/instruction.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

instruction_operand_t coil_create_reg_operand(uint32_t reg, coil_type_t type) {
  instruction_operand_t operand;
  operand.kind = OPERAND_REGISTER;
  operand.value.reg = reg;
  operand.type = type;
  return operand;
}

instruction_operand_t coil_create_imm_operand(int64_t imm, coil_type_t type) {
  instruction_operand_t operand;
  operand.kind = OPERAND_IMMEDIATE;
  operand.value.imm = imm;
  operand.type = type;
  return operand;
}

instruction_operand_t coil_create_block_operand(const char* name) {
  instruction_operand_t operand;
  operand.kind = OPERAND_BASIC_BLOCK;
  operand.value.block = name ? strdup(name) : NULL;
  operand.type = COIL_TYPE_VOID;  // Basic blocks don't have a type
  return operand;
}

instruction_operand_t coil_create_func_operand(const char* name, coil_type_t type) {
  instruction_operand_t operand;
  operand.kind = OPERAND_FUNCTION;
  operand.value.func = name ? strdup(name) : NULL;
  operand.type = type;
  return operand;
}

instruction_operand_t coil_create_global_operand(const char* name, coil_type_t type) {
  instruction_operand_t operand;
  operand.kind = OPERAND_GLOBAL;
  operand.value.global = name ? strdup(name) : NULL;
  operand.type = type;
  return operand;
}

instruction_operand_t coil_create_mem_operand(
  uint32_t base,
  int32_t offset,
  uint32_t index,
  uint8_t scale,
  coil_type_t type
) {
  instruction_operand_t operand;
  operand.kind = OPERAND_MEMORY;
  operand.value.mem.base = base;
  operand.value.mem.offset = offset;
  operand.value.mem.index = index;
  operand.value.mem.scale = scale;
  operand.type = type;
  return operand;
}

static instruction_operand_t* clone_operands(
  const instruction_operand_t* operands,
  uint8_t operand_count
) {
  if (operand_count == 0 || operands == NULL) {
    return NULL;
  }
  
  instruction_operand_t* result = (instruction_operand_t*)malloc(
    operand_count * sizeof(instruction_operand_t)
  );
  
  if (result == NULL) {
    return NULL;
  }
  
  for (uint8_t i = 0; i < operand_count; i++) {
    result[i] = operands[i];
    
    // Deep copy strings
    switch (operands[i].kind) {
      case OPERAND_BASIC_BLOCK:
        if (operands[i].value.block) {
          result[i].value.block = strdup(operands[i].value.block);
        }
        break;
        
      case OPERAND_FUNCTION:
        if (operands[i].value.func) {
          result[i].value.func = strdup(operands[i].value.func);
        }
        break;
        
      case OPERAND_GLOBAL:
        if (operands[i].value.global) {
          result[i].value.global = strdup(operands[i].value.global);
        }
        break;
        
      default:
        break;
    }
  }
  
  return result;
}

instruction_t* coil_create_instruction(
  opcode_t opcode,
  uint8_t flags,
  instruction_operand_t dest,
  instruction_operand_t* operands,
  uint8_t operand_count,
  coil_type_t type,
  const char* result_name
) {
  instruction_t* instr = (instruction_t*)malloc(sizeof(instruction_t));
  if (instr == NULL) {
    return NULL;
  }
  
  instr->opcode = opcode;
  instr->flags = flags;
  instr->dest = dest;
  instr->operands = clone_operands(operands, operand_count);
  instr->operand_count = operand_count;
  instr->type = type;
  instr->result_name = result_name ? strdup(result_name) : NULL;
  
  return instr;
}

instruction_t* coil_create_binary_op(
  opcode_t opcode,
  uint8_t flags,
  uint32_t dest,
  instruction_operand_t lhs,
  instruction_operand_t rhs,
  coil_type_t type,
  const char* result_name
) {
  instruction_operand_t operands[2] = { lhs, rhs };
  instruction_operand_t dest_operand = coil_create_reg_operand(dest, type);
  
  return coil_create_instruction(
    opcode,
    flags,
    dest_operand,
    operands,
    2,
    type,
    result_name
  );
}

instruction_t* coil_create_unary_op(
  opcode_t opcode,
  uint8_t flags,
  uint32_t dest,
  instruction_operand_t src,
  coil_type_t type,
  const char* result_name
) {
  instruction_operand_t dest_operand = coil_create_reg_operand(dest, type);
  
  return coil_create_instruction(
    opcode,
    flags,
    dest_operand,
    &src,
    1,
    type,
    result_name
  );
}

instruction_t* coil_create_load(
  uint32_t dest,
  instruction_operand_t addr,
  coil_type_t type,
  uint8_t flags,
  const char* result_name
) {
  instruction_operand_t dest_operand = coil_create_reg_operand(dest, type);
  
  return coil_create_instruction(
    OPCODE_LOAD,
    flags,
    dest_operand,
    &addr,
    1,
    type,
    result_name
  );
}

instruction_t* coil_create_store(
  instruction_operand_t addr,
  instruction_operand_t value,
  uint8_t flags
) {
  // Store doesn't produce a result, so dest is a dummy register
  instruction_operand_t dest_operand = coil_create_reg_operand(0, COIL_TYPE_VOID);
  instruction_operand_t operands[2] = { addr, value };
  
  return coil_create_instruction(
    OPCODE_STORE,
    flags,
    dest_operand,
    operands,
    2,
    COIL_TYPE_VOID,
    NULL
  );
}

instruction_t* coil_create_branch(
  instruction_operand_t* cond,
  const char* true_block,
  const char* false_block
) {
  instruction_operand_t dest_operand = coil_create_reg_operand(0, COIL_TYPE_VOID);
  
  if (cond == NULL) {
    // Unconditional branch
    instruction_operand_t target_operand = coil_create_block_operand(true_block);
    
    return coil_create_instruction(
      OPCODE_BR,
      0,
      dest_operand,
      &target_operand,
      1,
      COIL_TYPE_VOID,
      NULL
    );
  } else {
    // Conditional branch
    instruction_operand_t operands[3];
    operands[0] = *cond;
    operands[1] = coil_create_block_operand(true_block);
    operands[2] = coil_create_block_operand(false_block);
    
    return coil_create_instruction(
      OPCODE_BR,
      0,
      dest_operand,
      operands,
      3,
      COIL_TYPE_VOID,
      NULL
    );
  }
}

instruction_t* coil_create_call(
  uint32_t dest,
  instruction_operand_t func,
  instruction_operand_t* args,
  uint8_t arg_count,
  coil_type_t type,
  uint8_t flags,
  const char* result_name
) {
  instruction_operand_t dest_operand = coil_create_reg_operand(dest, type);
  
  // Allocate space for function + args
  instruction_operand_t* operands = (instruction_operand_t*)malloc(
    (arg_count + 1) * sizeof(instruction_operand_t)
  );
  
  if (operands == NULL) {
    return NULL;
  }
  
  // Function is the first operand
  operands[0] = func;
  
  // Copy args
  for (uint8_t i = 0; i < arg_count; i++) {
    operands[i + 1] = args[i];
  }
  
  instruction_t* instr = coil_create_instruction(
    OPCODE_CALL,
    flags,
    dest_operand,
    operands,
    arg_count + 1,
    type,
    result_name
  );
  
  // Free temporary buffer
  free(operands);
  
  return instr;
}

instruction_t* coil_create_return(instruction_operand_t* value) {
  instruction_operand_t dest_operand = coil_create_reg_operand(0, COIL_TYPE_VOID);
  
  if (value == NULL) {
    // Return void
    return coil_create_instruction(
      OPCODE_RET,
      0,
      dest_operand,
      NULL,
      0,
      COIL_TYPE_VOID,
      NULL
    );
  } else {
    // Return value
    return coil_create_instruction(
      OPCODE_RET,
      0,
      dest_operand,
      value,
      1,
      value->type,
      NULL
    );
  }
}

void coil_free_instruction(instruction_t* instr) {
  if (instr == NULL) {
    return;
  }
  
  // Free operands
  for (uint8_t i = 0; i < instr->operand_count; i++) {
    instruction_operand_t* op = &instr->operands[i];
    
    // Free strings
    switch (op->kind) {
      case OPERAND_BASIC_BLOCK:
        if (op->value.block) free((void*)op->value.block);
        break;
        
      case OPERAND_FUNCTION:
        if (op->value.func) free((void*)op->value.func);
        break;
        
      case OPERAND_GLOBAL:
        if (op->value.global) free((void*)op->value.global);
        break;
        
      default:
        break;
    }
  }
  
  // Free strings in destination operand
  switch (instr->dest.kind) {
    case OPERAND_BASIC_BLOCK:
      if (instr->dest.value.block) free((void*)instr->dest.value.block);
      break;
      
    case OPERAND_FUNCTION:
      if (instr->dest.value.func) free((void*)instr->dest.value.func);
      break;
      
    case OPERAND_GLOBAL:
      if (instr->dest.value.global) free((void*)instr->dest.value.global);
      break;
      
    default:
      break;
  }
  
  // Free operands array
  if (instr->operands) {
    free(instr->operands);
  }
  
  // Free result name
  if (instr->result_name) {
    free((void*)instr->result_name);
  }
  
  // Free instruction
  free(instr);
}

int coil_instruction_to_binary(
  const instruction_t* instr,
  uint8_t* buffer,
  size_t buffer_size
) {
  // Basic format:
  // [opcode:8][flags:8][operand count:8][destination:8][operands...]
  
  if (instr == NULL || buffer == NULL || buffer_size < 4) {
    return -1;
  }
  
  // Write header
  buffer[0] = (uint8_t)instr->opcode;
  buffer[1] = instr->flags;
  buffer[2] = instr->operand_count;
  buffer[3] = instr->dest.kind == OPERAND_REGISTER ? instr->dest.value.reg : 0;
  
  // Calculate required size for operands (simplified version, real implementation would be more complex)
  size_t required_size = 4;  // Header
  
  // Add space for operands (simplified - in reality we'd need variable-length encoding)
  required_size += instr->operand_count * 4;
  
  if (buffer_size < required_size) {
    return -1;
  }
  
  // Write operands (simplified - real implementation would handle all operand types)
  for (uint8_t i = 0; i < instr->operand_count; i++) {
    const instruction_operand_t* op = &instr->operands[i];
    
    // Simple encoding for register/immediate operands
    if (op->kind == OPERAND_REGISTER) {
      buffer[4 + i * 4] = OPERAND_REGISTER;
      buffer[5 + i * 4] = op->value.reg & 0xFF;
      buffer[6 + i * 4] = (op->value.reg >> 8) & 0xFF;
      buffer[7 + i * 4] = (op->value.reg >> 16) & 0xFF;
    } else if (op->kind == OPERAND_IMMEDIATE) {
      buffer[4 + i * 4] = OPERAND_IMMEDIATE;
      buffer[5 + i * 4] = op->value.imm & 0xFF;
      buffer[6 + i * 4] = (op->value.imm >> 8) & 0xFF;
      buffer[7 + i * 4] = (op->value.imm >> 16) & 0xFF;
    }
    // Other operand types would be handled here
  }
  
  return required_size;
}

instruction_t* coil_instruction_from_binary(
  const uint8_t* buffer,
  size_t buffer_size,
  type_registry_t* registry
) {
  // Minimal size check
  if (buffer == NULL || buffer_size < 4) {
    return NULL;
  }
  
  // Read header
  opcode_t opcode = (opcode_t)buffer[0];
  uint8_t flags = buffer[1];
  uint8_t operand_count = buffer[2];
  uint8_t dest_reg = buffer[3];
  
  // Check buffer size for operands
  if (buffer_size < 4 + operand_count * 4) {
    return NULL;
  }
  
  // Create destination operand (assuming it's a register)
  instruction_operand_t dest = coil_create_reg_operand(dest_reg, COIL_TYPE_VOID);
  
  // Read operands
  instruction_operand_t* operands = NULL;
  if (operand_count > 0) {
    operands = (instruction_operand_t*)malloc(operand_count * sizeof(instruction_operand_t));
    if (operands == NULL) {
      return NULL;
    }
    
    for (uint8_t i = 0; i < operand_count; i++) {
      operand_kind_t kind = (operand_kind_t)buffer[4 + i * 4];
      
      if (kind == OPERAND_REGISTER) {
        uint32_t reg = buffer[5 + i * 4] |
                    (buffer[6 + i * 4] << 8) |
                    (buffer[7 + i * 4] << 16);
        operands[i] = coil_create_reg_operand(reg, COIL_TYPE_VOID);
      } else if (kind == OPERAND_IMMEDIATE) {
        int32_t imm = buffer[5 + i * 4] |
                    (buffer[6 + i * 4] << 8) |
                    (buffer[7 + i * 4] << 16);
        operands[i] = coil_create_imm_operand(imm, COIL_TYPE_INT32);
      }
      // Other operand types would be handled here
    }
  }
  
  // Create instruction
  instruction_t* instr = coil_create_instruction(
    opcode,
    flags,
    dest,
    operands,
    operand_count,
    COIL_TYPE_VOID,  // Type would be decoded from the binary
    NULL
  );
  
  // Free temporary operand buffer
  if (operands) {
    free(operands);
  }
  
  return instr;
}