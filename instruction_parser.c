#include <string.h>
#include "instruction_parser.h"
#include "tables.h"

#define NUM_COMMANDS 27

/*finds details about a command from its name and the table*/
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

/*builds binary code for R type*/
unsigned int build_r_instruction(int opcode, int funct, int rs, int rt, int rd) {
  unsigned int instruction = 0;
  
  /*input check: registers should be in range 0-31*/
  if (rs < 0 || rs > 31 || rt < 0 || rt > 31 || rd < 0 || rd > 31) {
    printf("Error: Register number out of range (0-31)\n");
    return 0;
  }

  /*input check: opcode should be in range 0-63
                  funct should be in range 0-31*/
  if (opcode < 0 || opcode > 63 || funct < 0 || funct > 31) {
    printf("Error: Opcode or Funct out of range (0-63)\n");
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

/*builds binary code for I type*/
unsigned int build_i_instruction(int opcode, int rs, int rt, int immed) {
  unsigned int instruction = 0;
  
  /*input check: registers should be in range 0-31*/
  if (rs < 0 || rs > 31 || rt < 0 || rt > 31) {
    printf("Error: Register number out of range (0-31)\n");
    return 0;
  }

  /*input check: opcode should be in range 0-63*/
  if (opcode < 0 || opcode > 63) {
    printf("Error: Opcode out of range (0-63)\n");
    return 0;
  }
  
  /*input check: immed should be in range -32768 to 32767*/
  if (immed < -32768 || immed > 32767) {
    printf("Error: Immediate value %d out of range (-32768 to 32767)\n", immed);
    return 0;
  }
  
  /*the opcode is bits 26-31*/
  instruction |= ((unsigned int)(opcode) << 26);
  
  /*the rs register is bits 21-25*/
  instruction |= ((unsigned int)(rs) << 21);
  
  /*the rt register is bits 16-20*/
  instruction |= ((unsigned int)(rt) << 16);
  
  /*the immed is bits 0-15 with mask to ensure only the lower 16 bits are used*/
  instruction |= (unsigned int)(immed) & 0xFFFF;
  
  
  return instruction;
}

/*builds binary code for J type*/
unsigned int build_j_instruction(int opcode, int reg_flag, int address_or_reg) {
  unsigned int instruction = 0;
  
  /*input check: opcode should be in range 0-63*/
  if (opcode < 0 || opcode > 63) {
    printf("Error: Opcode out of range (0-63)\n");
    return 0;
  }

  /*input check: reg_flag should be 0 or 1*/
  if (reg_flag < 0 || reg_flag > 1) {
    printf("Error: Register flag out of range (0 or 1)\n");
    return 0;
  }

  if (reg_flag == 0) { /*if we are using a label*/

    /*input check: address should be in range 0-33554431 (25 bits)*/
    if (address_or_reg < 0 || address_or_reg > 0x1FFFFFF) {
      printf("Error: Address out of range (0-33554431)\n");
      return 0;
    }
  } else { /*if we are using a register*/
    /*input check: register should be in range 0-31*/
    if (address_or_reg < 0 || address_or_reg > 31) {
      printf("Error: Register number out of range (0-31)\n");
      return 0;
    }
  }

  
  /*the opcode is bits 26-31*/
  instruction |= ((unsigned int)(opcode) << 26);
  
  /*the reg_flag is bit 25*/
  instruction |= ((unsigned int)(reg_flag) << 25);
  
  /*the address_or_reg is bits 0-24 with mask to ensure only the lower 25 bits are used*/
  instruction |= (unsigned int)(address_or_reg) & 0x1FFFFFF;
  
  return instruction;
}



