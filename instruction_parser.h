#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include "tables.h" /*using the Command struct*/

/*
 * Find the command-table entry named cmd_name. Returns a pointer to the
 * read-only table entry, or NULL when cmd_name is NULL or unknown. The
 * algorithm performs a linear search through the fixed command table.
 */
const Command* find_command(const char *cmd_name);

/*
 * Encode an R-type instruction from its opcode, function code, and registers.
 * Returns the 32-bit instruction, or zero when an argument is outside its
 * legal range. Register numbers, opcode, and function code are assumed to be
 * integer values supplied by the assembler; fields are placed by bit shifts.
 */
unsigned int build_r_instruction(int opcode, int funct, int rs, int rt, int rd);

/*
 * Encode an I-type instruction from its opcode, registers, and signed
 * immediate. Returns the 32-bit instruction, or zero for an invalid range.
 * The algorithm validates inputs, shifts the opcode/register fields, and
 * masks the immediate to 16 bits.
 */
unsigned int build_i_instruction(int opcode, int rs, int rt, int immed);

/*
 * Encode a J-type instruction using either a 25-bit address or register,
 * selected by reg_flag. Returns the 32-bit instruction, or zero for invalid
 * input. The caller must pass reg_flag as FALSE for an address and TRUE for
 * a register; the fields are assembled with shifts and a 25-bit mask.
 */
unsigned int build_j_instruction(int opcode, int reg_flag, int address_or_reg);

#endif


