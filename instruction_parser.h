#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include "tables.h" /*using the Command struct*/

/*finds details about a command from its name and the table*/
const Command* find_command(const char *cmd_name);

/*builds binary code for R type*/
unsigned int build_r_instruction(int opcode, int funct, int rs, int rt, int rd);

/*builds binary code for I type*/
unsigned int build_i_instruction(int opcode, int rs, int rt, int immed);

/*builds binary code for J type*/
unsigned int build_j_instruction(int opcode, int reg_flag, int address_or_reg);

#endif


