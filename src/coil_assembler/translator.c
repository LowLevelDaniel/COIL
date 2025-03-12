/**
 * @file translator.c
 * @brief Translator implementation from COIL instructions to native code
 */

#include "coil_assembler/translator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Native code buffer structure
 */
struct native_buffer {
  uint8_t*  data;          /**< Buffer data */
  size_t    size;          /**< Current size */
  size_t    capacity;      /**< Buffer capacity */
};

/**
 * @brief X86-64 register mapping
 */
typedef enum {
  X86_REG_RAX = 0,
  X86_REG_RCX = 1,
  X86_REG_RDX = 2,
  X86_REG_RBX = 3,
  X86_REG_RSP = 4,
  X86_REG_RBP = 5,
  X86_REG_RSI = 6,
  X86_REG_RDI = 7,
  X86_REG_R8  = 8,
  X86_REG_R9  = 9,
  X86_REG_R10 = 10,
  X86_REG_R11 = 11,
  X86_REG_R12 = 12,
  X86_REG_R13 = 13,
  X86_REG_R14 = 14,
  X86_REG_R15 = 15
} x86_reg_t;

/**
 * @brief Mapping from virtual registers to physical registers
 */
typedef struct {
  uint32_t  vreg;    /**< Virtual register */
  x86_reg_t preg;    /**< Physical register */
  bool     assigned; /**< Whether the register is assigned */
} reg_mapping_t;

/**
 * @brief Translator structure
 */
struct translator {
  target_config_t*  target;         /**< Target configuration */
  error_context_t*  error_context;  /**< Error context */
  uint32_t          opt_level;      /**< Optimization level */
  reg_mapping_t*    reg_mappings;   /**< Register mappings */
  uint32_t          reg_count;      /**< Number of register mappings */
  bool              had_error;      /**< True if an error occurred */
};

/**
 * @brief Report a translator error
 * 
 * @param translator The translator
 * @param code Error code
 * @param message Error message
 */
static void report_error(
  translator_t* translator,
  uint32_t code,
  const char* message
) {
  if (translator->error_context != NULL) {
    REPORT_ERROR(translator->error_context, ERROR_ERROR, ERROR_CATEGORY_ASSEMBLER,
                code, message, NULL, 0, 0);
  }
  
  translator->had_error = true;
}

translator_t* coil_create_translator(
  target_config_t* target,
  error_context_t* error_context
) {
  if (target == NULL) {
    return NULL;
  }
  
  translator_t* translator = (translator_t*)malloc(sizeof(translator_t));
  if (translator == NULL) {
    return NULL;
  }
  
  translator->target = target;
  translator->error_context = error_context;
  translator->opt_level = 0;
  translator->reg_mappings = NULL;
  translator->reg_count = 0;
  translator->had_error = false;
  
  return translator;
}

void coil_free_translator(translator_t* translator) {
  if (translator == NULL) {
    return;
  }
  
  if (translator->reg_mappings != NULL) {
    free(translator->reg_mappings);
  }
  
  free(translator);
}

native_buffer_t* coil_create_native_buffer(
  translator_t* translator,
  size_t initial_capacity
) {
  if (translator == NULL || initial_capacity == 0) {
    return NULL;
  }
  
  native_buffer_t* buffer = (native_buffer_t*)malloc(sizeof(native_buffer_t));
  if (buffer == NULL) {
    return NULL;
  }
  
  buffer->data = (uint8_t*)malloc(initial_capacity);
  if (buffer->data == NULL) {
    free(buffer);
    return NULL;
  }
  
  buffer->size = 0;
  buffer->capacity = initial_capacity;
  
  return buffer;
}

void coil_free_native_buffer(native_buffer_t* buffer) {
  if (buffer == NULL) {
    return;
  }
  
  if (buffer->data != NULL) {
    free(buffer->data);
  }
  
  free(buffer);
}

const uint8_t* coil_get_buffer_data(const native_buffer_t* buffer) {
  if (buffer == NULL) {
    return NULL;
  }
  
  return buffer->data;
}

size_t coil_get_buffer_size(const native_buffer_t* buffer) {
  if (buffer == NULL) {
    return 0;
  }
  
  return buffer->size;
}

void coil_reset_buffer(native_buffer_t* buffer) {
  if (buffer == NULL) {
    return;
  }
  
  buffer->size = 0;
}

/**
 * @brief Ensure the buffer has enough capacity
 * 
 * @param buffer The buffer
 * @param additional_size Additional size needed
 * @return 0 on success, -1 on error
 */
static int ensure_buffer_capacity(
  native_buffer_t* buffer,
  size_t additional_size
) {
  if (buffer->size + additional_size <= buffer->capacity) {
    return 0;
  }
  
  size_t new_capacity = buffer->capacity * 2;
  while (buffer->size + additional_size > new_capacity) {
    new_capacity *= 2;
  }
  
  uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_capacity);
  if (new_data == NULL) {
    return -1;
  }
  
  buffer->data = new_data;
  buffer->capacity = new_capacity;
  
  return 0;
}

/**
 * @brief Append data to the buffer
 * 
 * @param buffer The buffer
 * @param data Data to append
 * @param size Data size
 * @return 0 on success, -1 on error
 */
static int append_to_buffer(
  native_buffer_t* buffer,
  const uint8_t* data,
  size_t size
) {
  if (ensure_buffer_capacity(buffer, size) != 0) {
    return -1;
  }
  
  memcpy(buffer->data + buffer->size, data, size);
  buffer->size += size;
  
  return 0;
}

/**
 * @brief Map a virtual register to a physical register
 * 
 * @param translator The translator
 * @param vreg Virtual register
 * @return Physical register, or -1 on error
 */
static int map_register(translator_t* translator, uint32_t vreg) {
  // Check if the register is already mapped
  for (uint32_t i = 0; i < translator->reg_count; i++) {
    if (translator->reg_mappings[i].vreg == vreg) {
      return translator->reg_mappings[i].preg;
    }
  }
  
  // Find an available physical register
  x86_reg_t preg = X86_REG_RAX;  // Default to RAX if no mapping
  
  // Simple round-robin allocation for now
  if (translator->reg_count > 0) {
    preg = (translator->reg_mappings[translator->reg_count - 1].preg + 1) % 16;
  }
  
  // Add the mapping
  reg_mapping_t* new_mappings = (reg_mapping_t*)realloc(
    translator->reg_mappings,
    (translator->reg_count + 1) * sizeof(reg_mapping_t)
  );
  
  if (new_mappings == NULL) {
    report_error(translator, ERROR_GENERAL_OUT_OF_MEMORY, "Out of memory");
    return -1;
  }
  
  translator->reg_mappings = new_mappings;
  translator->reg_mappings[translator->reg_count].vreg = vreg;
  translator->reg_mappings[translator->reg_count].preg = preg;
  translator->reg_mappings[translator->reg_count].assigned = true;
  translator->reg_count++;
  
  return preg;
}

/**
 * @brief Reset register mappings
 * 
 * @param translator The translator
 */
static void reset_register_mappings(translator_t* translator) {
  if (translator->reg_mappings != NULL) {
    free(translator->reg_mappings);
    translator->reg_mappings = NULL;
  }
  
  translator->reg_count = 0;
}

/**
 * @brief Encode a ModR/M byte
 * 
 * @param mod Mode (0-3)
 * @param reg Register operand (0-7)
 * @param rm R/M operand (0-7)
 * @return Encoded ModR/M byte
 */
static uint8_t encode_modrm(uint8_t mod, uint8_t reg, uint8_t rm) {
  return ((mod & 0x03) << 6) | ((reg & 0x07) << 3) | (rm & 0x07);
}

/**
 * @brief Encode a REX prefix
 * 
 * @param w 64-bit operand size (0 or 1)
 * @param r Extension of ModR/M.reg field (0 or 1)
 * @param x Extension of SIB.index field (0 or 1)
 * @param b Extension of ModR/M.rm or SIB.base field (0 or 1)
 * @return Encoded REX prefix
 */
static uint8_t encode_rex(uint8_t w, uint8_t r, uint8_t x, uint8_t b) {
  return 0x40 | (w << 3) | (r << 2) | (x << 1) | b;
}

/**
 * @brief Translate an ADD instruction to x86-64
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
static int translate_add(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
) {
  // ADD instruction has format: ADD dest, src1, src2
  // For x86-64, we'll map it to: dest = src1; ADD dest, src2
  
  if (instr->operand_count != 2) {
    report_error(translator, ERROR_CODEGEN_INVALID_IR,
                "ADD instruction must have 2 operands");
    return -1;
  }
  
  // Get operands
  const instruction_operand_t* dest = &instr->dest;
  const instruction_operand_t* src1 = &instr->operands[0];
  const instruction_operand_t* src2 = &instr->operands[1];
  
  // We only handle register operands for now
  if (dest->kind != OPERAND_REGISTER ||
      src1->kind != OPERAND_REGISTER ||
      src2->kind != OPERAND_REGISTER) {
    report_error(translator, ERROR_CODEGEN_UNSUPPORTED,
                "Only register operands are supported for ADD");
    return -1;
  }
  
  // Map virtual registers to physical registers
  int dest_reg = map_register(translator, dest->value.reg);
  int src1_reg = map_register(translator, src1->value.reg);
  int src2_reg = map_register(translator, src2->value.reg);
  
  if (dest_reg < 0 || src1_reg < 0 || src2_reg < 0) {
    return -1;  // Error already reported
  }
  
  // First, we need to move src1 to dest if they're different
  if (dest_reg != src1_reg) {
    // MOV dest, src1
    uint8_t mov_code[3];
    
    // REX prefix for 64-bit operation with register extensions
    mov_code[0] = encode_rex(1,
                            (dest_reg > 7) ? 1 : 0,
                            0,
                            (src1_reg > 7) ? 1 : 0);
    
    // MOV opcode
    mov_code[1] = 0x89;
    
    // ModR/M byte
    mov_code[2] = encode_modrm(3,
                              src1_reg & 0x07,
                              dest_reg & 0x07);
    
    if (append_to_buffer(buffer, mov_code, sizeof(mov_code)) != 0) {
      report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
      return -1;
    }
  }
  
  // Now, ADD dest, src2
  uint8_t add_code[3];
  
  // REX prefix for 64-bit operation with register extensions
  add_code[0] = encode_rex(1,
                          (src2_reg > 7) ? 1 : 0,
                          0,
                          (dest_reg > 7) ? 1 : 0);
  
  // ADD opcode
  add_code[1] = 0x01;
  
  // ModR/M byte
  add_code[2] = encode_modrm(3,
                            src2_reg & 0x07,
                            dest_reg & 0x07);
  
  if (append_to_buffer(buffer, add_code, sizeof(add_code)) != 0) {
    report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
    return -1;
  }
  
  return 0;
}

/**
 * @brief Translate a SUB instruction to x86-64
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
static int translate_sub(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
) {
  // SUB instruction has format: SUB dest, src1, src2
  // For x86-64, we'll map it to: dest = src1; SUB dest, src2
  
  if (instr->operand_count != 2) {
    report_error(translator, ERROR_CODEGEN_INVALID_IR,
                "SUB instruction must have 2 operands");
    return -1;
  }
  
  // Get operands
  const instruction_operand_t* dest = &instr->dest;
  const instruction_operand_t* src1 = &instr->operands[0];
  const instruction_operand_t* src2 = &instr->operands[1];
  
  // We only handle register operands for now
  if (dest->kind != OPERAND_REGISTER ||
      src1->kind != OPERAND_REGISTER ||
      src2->kind != OPERAND_REGISTER) {
    report_error(translator, ERROR_CODEGEN_UNSUPPORTED,
                "Only register operands are supported for SUB");
    return -1;
  }
  
  // Map virtual registers to physical registers
  int dest_reg = map_register(translator, dest->value.reg);
  int src1_reg = map_register(translator, src1->value.reg);
  int src2_reg = map_register(translator, src2->value.reg);
  
  if (dest_reg < 0 || src1_reg < 0 || src2_reg < 0) {
    return -1;  // Error already reported
  }
  
  // First, we need to move src1 to dest if they're different
  if (dest_reg != src1_reg) {
    // MOV dest, src1
    uint8_t mov_code[3];
    
    // REX prefix for 64-bit operation with register extensions
    mov_code[0] = encode_rex(1,
                            (dest_reg > 7) ? 1 : 0,
                            0,
                            (src1_reg > 7) ? 1 : 0);
    
    // MOV opcode
    mov_code[1] = 0x89;
    
    // ModR/M byte
    mov_code[2] = encode_modrm(3,
                              src1_reg & 0x07,
                              dest_reg & 0x07);
    
    if (append_to_buffer(buffer, mov_code, sizeof(mov_code)) != 0) {
      report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
      return -1;
    }
  }
  
  // Now, SUB dest, src2
  uint8_t sub_code[3];
  
  // REX prefix for 64-bit operation with register extensions
  sub_code[0] = encode_rex(1,
                          (src2_reg > 7) ? 1 : 0,
                          0,
                          (dest_reg > 7) ? 1 : 0);
  
  // SUB opcode
  sub_code[1] = 0x29;
  
  // ModR/M byte
  sub_code[2] = encode_modrm(3,
                            src2_reg & 0x07,
                            dest_reg & 0x07);
  
  if (append_to_buffer(buffer, sub_code, sizeof(sub_code)) != 0) {
    report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
    return -1;
  }
  
  return 0;
}

/**
 * @brief Translate a LOAD_I32 instruction to x86-64
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
static int translate_load_i32(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
) {
  // LOAD_I32 instruction has format: LOAD_I32 dest, imm
  
  if (instr->operand_count != 1) {
    report_error(translator, ERROR_CODEGEN_INVALID_IR,
                "LOAD_I32 instruction must have 1 operand");
    return -1;
  }
  
  // Get operands
  const instruction_operand_t* dest = &instr->dest;
  const instruction_operand_t* imm = &instr->operands[0];
  
  // We only handle register destination for now
  if (dest->kind != OPERAND_REGISTER) {
    report_error(translator, ERROR_CODEGEN_UNSUPPORTED,
                "Only register destinations are supported for LOAD_I32");
    return -1;
  }
  
  // We only handle immediate operand for now
  if (imm->kind != OPERAND_IMMEDIATE) {
    report_error(translator, ERROR_CODEGEN_UNSUPPORTED,
                "Only immediate operands are supported for LOAD_I32");
    return -1;
  }
  
  // Map virtual register to physical register
  int dest_reg = map_register(translator, dest->value.reg);
  
  if (dest_reg < 0) {
    return -1;  // Error already reported
  }
  
  // MOV dest, imm32
  uint8_t mov_code[6];
  
  // REX prefix for 64-bit operation with register extension
  mov_code[0] = encode_rex(1, 0, 0, (dest_reg > 7) ? 1 : 0);
  
  // MOV r64, imm32 (sign-extended) opcode
  mov_code[1] = 0xC7;
  
  // ModR/M byte
  mov_code[2] = encode_modrm(3, 0, dest_reg & 0x07);
  
  // Immediate value (32-bit)
  *(int32_t*)(mov_code + 3) = (int32_t)imm->value.imm;
  
  if (append_to_buffer(buffer, mov_code, sizeof(mov_code)) != 0) {
    report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
    return -1;
  }
  
  return 0;
}

/**
 * @brief Translate a RET instruction to x86-64
 * 
 * @param translator The translator
 * @param instr The COIL instruction
 * @param buffer The native code buffer
 * @return 0 on success, -1 on error
 */
static int translate_ret(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
) {
  // RET instruction has format: RET [value]
  
  // For simplicity, we're ignoring the return value for now
  // In a real implementation, we'd move the value to RAX
  
  // Simple RET instruction
  uint8_t ret_code = 0xC3;
  
  if (append_to_buffer(buffer, &ret_code, sizeof(ret_code)) != 0) {
    report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
    return -1;
  }
  
  return 0;
}

int coil_translate_instruction(
  translator_t* translator,
  const instruction_t* instr,
  native_buffer_t* buffer
) {
  if (translator == NULL || instr == NULL || buffer == NULL) {
    return -1;
  }
  
  translator->had_error = false;
  
  // Translate based on opcode
  switch (instr->opcode) {
    case OPCODE_ADD:
      return translate_add(translator, instr, buffer);
      
    case OPCODE_SUB:
      return translate_sub(translator, instr, buffer);
      
    case OPCODE_LOAD_I32:
      return translate_load_i32(translator, instr, buffer);
      
    case OPCODE_RET:
      return translate_ret(translator, instr, buffer);
      
    default:
      report_error(translator, ERROR_CODEGEN_UNSUPPORTED,
                  "Unsupported instruction");
      return -1;
  }
}

int coil_translate_function(
  translator_t* translator,
  const function_t* function,
  native_buffer_t* buffer
) {
  if (translator == NULL || function == NULL || buffer == NULL) {
    return -1;
  }
  
  translator->had_error = false;
  
  // Reset register mappings
  reset_register_mappings(translator);
  
  // Function prologue
  uint8_t prologue[] = {
    0x55,                     // PUSH RBP
    0x48, 0x89, 0xE5          // MOV RBP, RSP
  };
  
  if (append_to_buffer(buffer, prologue, sizeof(prologue)) != 0) {
    report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
    return -1;
  }
  
  // Translate each basic block
  for (uint32_t i = 0; i < function->block_count; i++) {
    const basic_block_t* block = function->blocks[i];
    
    // Translate each instruction
    for (uint32_t j = 0; j < block->instr_count; j++) {
      const instruction_t* instr = block->instructions[j];
      
      if (coil_translate_instruction(translator, instr, buffer) != 0) {
        // Error already reported
        return -1;
      }
    }
  }
  
  // Function epilogue (if not already added by RET instruction)
  if (buffer->size == 0 || buffer->data[buffer->size - 1] != 0xC3) {
    uint8_t epilogue[] = {
      0x5D,                     // POP RBP
      0xC3                      // RET
    };
    
    if (append_to_buffer(buffer, epilogue, sizeof(epilogue)) != 0) {
      report_error(translator, ERROR_SYSTEM_MEMORY, "Failed to append to buffer");
      return -1;
    }
  }
  
  return 0;
}

int coil_translate_module(
  translator_t* translator,
  const module_t* module,
  native_buffer_t* buffer
) {
  if (translator == NULL || module == NULL || buffer == NULL) {
    return -1;
  }
  
  translator->had_error = false;
  
  // Reset buffer
  coil_reset_buffer(buffer);
  
  // Check target compatibility
  if (!coil_target_satisfies_requirements(
      translator->target,
      module->target.required_features,
      module->target.required_count)) {
    report_error(translator, ERROR_ASSEMBLER_NO_TARGET,
                "Target does not satisfy module requirements");
    return -1;
  }
  
  // Translate each function
  for (uint32_t i = 0; i < module->function_count; i++) {
    const function_t* function = module->functions[i];
    
    // Skip external functions
    if (function->is_external) {
      continue;
    }
    
    if (coil_translate_function(translator, function, buffer) != 0) {
      // Error already reported
      return -1;
    }
  }
  
  return 0;
}

int coil_output_native_code(
  translator_t* translator,
  const native_buffer_t* buffer,
  FILE* file
) {
  if (translator == NULL || buffer == NULL || file == NULL) {
    return -1;
  }
  
  if (buffer->size == 0) {
    return 0;  // Nothing to output
  }
  
  size_t written = fwrite(buffer->data, 1, buffer->size, file);
  if (written != buffer->size) {
    report_error(translator, ERROR_SYSTEM_IO, "Failed to write to file");
    return -1;
  }
  
  return 0;
}

int coil_get_native_code(
  translator_t* translator,
  const instruction_t* instr,
  uint8_t* code,
  size_t code_size
) {
  if (translator == NULL || instr == NULL || code == NULL || code_size == 0) {
    return -1;
  }
  
  // Create a temporary buffer
  native_buffer_t* buffer = coil_create_native_buffer(translator, 16);
  if (buffer == NULL) {
    return -1;
  }
  
  // Translate the instruction
  if (coil_translate_instruction(translator, instr, buffer) != 0) {
    coil_free_native_buffer(buffer);
    return -1;
  }
  
  // Check if the code fits
  if (buffer->size > code_size) {
    coil_free_native_buffer(buffer);
    return -1;
  }
  
  // Copy the code
  memcpy(code, buffer->data, buffer->size);
  
  size_t size = buffer->size;
  coil_free_native_buffer(buffer);
  
  return size;
}

int coil_generate_assembly(
  translator_t* translator,
  const instruction_t* instr,
  char* buffer,
  size_t size
) {
  if (translator == NULL || instr == NULL || buffer == NULL || size == 0) {
    return -1;
  }
  
  // Simple version for now - just output instruction mnemonic
  const char* mnemonic = coil_get_opcode_mnemonic((uint8_t)instr->opcode);
  if (mnemonic == NULL) {
    snprintf(buffer, size, "# Unknown instruction: %02X", (uint8_t)instr->opcode);
    return strlen(buffer);
  }
  
  // Format based on instruction type
  switch (instr->opcode) {
    case OPCODE_ADD:
      snprintf(buffer, size, "    addq %%r%u, %%r%u",
              (unsigned)instr->operands[1].value.reg,
              (unsigned)instr->operands[0].value.reg);
      break;
      
    case OPCODE_SUB:
      snprintf(buffer, size, "    subq %%r%u, %%r%u",
              (unsigned)instr->operands[1].value.reg,
              (unsigned)instr->operands[0].value.reg);
      break;
      
    case OPCODE_LOAD_I32:
      snprintf(buffer, size, "    movl $%d, %%r%u",
              (int)instr->operands[0].value.imm,
              (unsigned)instr->dest.value.reg);
      break;
      
    case OPCODE_RET:
      snprintf(buffer, size, "    ret");
      break;
      
    default:
      snprintf(buffer, size, "# %s (not implemented)", mnemonic);
      break;
  }
  
  return strlen(buffer);
}

bool coil_can_translate_instruction(
  translator_t* translator,
  const instruction_t* instr
) {
  if (translator == NULL || instr == NULL) {
    return false;
  }
  
  // Check if we have an implementation for this instruction
  switch (instr->opcode) {
    case OPCODE_ADD:
    case OPCODE_SUB:
    case OPCODE_LOAD_I32:
    case OPCODE_RET:
      return true;
      
    default:
      return false;
  }
}

size_t coil_get_native_code_size(
  translator_t* translator,
  const instruction_t* instr
) {
  if (translator == NULL || instr == NULL) {
    return 0;
  }
  
  // Return the approximate size of the native code
  // This is a conservative estimate
  switch (instr->opcode) {
    case OPCODE_ADD:
    case OPCODE_SUB:
      return 6;  // REX + MOV + REX + ADD/SUB
      
    case OPCODE_LOAD_I32:
      return 6;  // REX + MOV + immediate
      
    case OPCODE_RET:
      return 1;  // RET
      
    default:
      return 0;  // Unknown
  }
}

int coil_set_optimization_level(
  translator_t* translator,
  uint32_t level
) {
  if (translator == NULL) {
    return -1;
  }
  
  if (level > 3) {
    return -1;
  }
  
  translator->opt_level = level;
  
  return 0;
}