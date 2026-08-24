/*
 * Instruction-encoding module. It looks up supported mnemonics and packs
 * validated R-, I-, and J-type operands into 32-bit machine instructions.
 * The implementation assumes the architecture field widths defined below;
 * invalid operands produce zero and an explanatory diagnostic.
 */

#include <string.h>
#include <stdio.h>
#include "instruction_parser.h"
#include "tables.h"
#include "data_structures.h"

/* Encoding limits and field masks for the target instruction format. */
#define NUM_COMMANDS 27
#define MIN_REGISTER 0
#define MAX_REGISTER 31
#define MIN_OPCODE 0
#define MAX_OPCODE 63
#define MIN_FUNCT 0
#define MAX_FUNCT 31
#define MIN_IMMED -32768
#define MAX_IMMED 32767
#define MASK_16_BITS 0xFFFF
#define MAX_J_ADDRESS 0x1FFFFFF
#define MASK_25_BITS 0x1FFFFFF

/*
 * Search the fixed command table linearly and return the matching descriptor.
 * NULL input or an unknown mnemonic returns NULL; the table itself is not
 * modified.
 */
const Command* find_command(const char *cmd_name) {
  int i; /*index for itirating*/
  
  /*input check*/
  if (cmd_name == NULL) {
    return NULL;
  }
  
  /*going through the command table*/
  for (i = 0; i < NUM_COMMANDS; i++) {
  
    /*compare the current command to the input*/
    if (strcmp(commands_table[i].name, cmd_name) == 0) {
      return &commands_table[i];
    }
    
  }

  return NULL; /*if we didnt find the command*/
}

/*
 * Validate R-type fields and place opcode, registers, and function code into
 * their designated bit ranges. Return the packed instruction or zero when a
 * supplied field is outside its architectural range.
 */
unsigned int build_r_instruction(int opcode, int funct, int rs, int rt, int rd) {
  unsigned int instruction = 0;
  
  /*input check: registers should be in range 0-31*/
  if (rs < MIN_REGISTER || rs > MAX_REGISTER || rt < MIN_REGISTER || rt > MAX_REGISTER || rd < MIN_REGISTER || rd > MAX_REGISTER) {
    printf("Error: Register number out of range (%d-%d)\n", MIN_REGISTER, MAX_REGISTER);
    return 0;
  }

  /*input check: opcode should be in range 0-63
                  funct should be in range 0-31*/
  if (opcode < MIN_OPCODE || opcode > MAX_OPCODE || funct < MIN_FUNCT || funct > MAX_FUNCT) {
    printf("Error: Opcode or Funct out of range (%d-%d)\n", MIN_OPCODE, MAX_OPCODE);
    return 0;
  }
  
  /*the opcode is bits 26-31*/
  instruction |= ((unsigned int)(opcode) << 26);
  
  /*the rs register is bits 21-25*/
  instruction |= ((unsigned int)(rs) << 21);
  
  /*the rt register is bits 16-20*/
  instruction |= ((unsigned int)(rt) << 16);
  
  /*the rd register is bits 11-15*/
  instruction |= ((unsigned int)(rd) << 11);
  
  /*the funct is bits 6-10*/
  instruction |= ((unsigned int)(funct) << 6);
  
  /*bits 0-5 are unused - will stay set to 0*/

  
  return instruction;
  
}

/*
 * Validate I-type operands, shift opcode and registers into position, and
 * mask the signed immediate to 16 bits. Return the packed instruction or
 * zero when any input is outside its permitted range.
 */
unsigned int build_i_instruction(int opcode, int rs, int rt, int immed) {
  unsigned int instruction = 0;
  
  /*input check: registers should be in range 0-31*/
  if (rs < MIN_REGISTER || rs > MAX_REGISTER || rt < MIN_REGISTER || rt > MAX_REGISTER) {
    printf("Error: Register number out of range (%d-%d)\n", MIN_REGISTER, MAX_REGISTER);
    return 0;
  }

  /*input check: opcode should be in range 0-63*/
  if (opcode < MIN_OPCODE || opcode > MAX_OPCODE) {
    printf("Error: Opcode out of range (%d-%d)\n", MIN_OPCODE, MAX_OPCODE);
    return 0;
  }
  
  /*input check: immed should be in range -32768 to 32767*/
  if (immed < MIN_IMMED || immed > MAX_IMMED) {
    printf("Error: Immediate value %d out of range (%d to %d)\n", immed, MIN_IMMED, MAX_IMMED);
    return 0;
  }
  
  /*the opcode is bits 26-31*/
  instruction |= ((unsigned int)(opcode) << 26);
  
  /*the rs register is bits 21-25*/
  instruction |= ((unsigned int)(rs) << 21);
  
  /*the rt register is bits 16-20*/
  instruction |= ((unsigned int)(rt) << 16);
  
  /*the immed is bits 0-15 with mask to ensure only the lower 16 bits are used*/
  instruction |= (unsigned int)(immed) & MASK_16_BITS;
  
  
  return instruction;
}

/*
 * Validate a J-type opcode and either a register or address operand, then set
 * the opcode, register-mode flag, and 25-bit operand fields. Return zero on
 * invalid input; reg_flag selects whether address_or_reg is an address or a
 * register number.
 */
unsigned int build_j_instruction(int opcode, int reg_flag, int address_or_reg) {
  unsigned int instruction = 0;
  
  /*input check: opcode should be in range 0-63*/
  if (opcode < MIN_OPCODE || opcode > MAX_OPCODE) {
    printf("Error: Opcode out of range (%d-%d)\n", MIN_OPCODE, MAX_OPCODE);
    return 0;
  }

  /*input check: reg_flag should be 0 or 1*/
  if (reg_flag != FALSE && reg_flag != TRUE) {
    printf("Error: Register flag out of range (%d or %d)\n", FALSE, TRUE);
    return 0;
  }

  if (reg_flag == FALSE) { /*if we are using a label*/

    /*input check: address should be in range 0-33554431 (25 bits)*/
    if (address_or_reg < 0 || address_or_reg > MAX_J_ADDRESS) {
      printf("Error: Address out of range (0-%d)\n", MAX_J_ADDRESS);
      return 0;
    }
  } else { /*if we are using a register*/
    /*input check: register should be in range 0-31*/
    if (address_or_reg < 0 || address_or_reg > MAX_REGISTER) {
      printf("Error: Register number out of range (%d-%d)\n", MIN_REGISTER, MAX_REGISTER);
      return 0;
    }
  }

  
  /*the opcode is bits 26-31*/
  instruction |= ((unsigned int)(opcode) << 26);
  
  /*the reg_flag is bit 25*/
  instruction |= ((unsigned int)(reg_flag) << 25);
  
  /*the address_or_reg is bits 0-24 with mask to ensure only the lower 25 bits are used*/
  instruction |= (unsigned int)(address_or_reg) & MASK_25_BITS;
  
  return instruction;
}



