#include "tables.h"
/*this is the opcode table from page 24*/
const Command commands_table[27] = 
{
  /* ------------ R commands -------- */
  {"add", 0, 1, R_TYPE},
  {"sub", 0, 2, R_TYPE},
  {"and", 0, 3, R_TYPE},
  {"or", 0, 4, R_TYPE},
  {"nor", 0, 5, R_TYPE},
  {"move", 1, 1, R_TYPE},
  {"mvhi", 1, 2, R_TYPE},
  {"mvlo", 1, 3, R_TYPE},
  
  /* ------------ I commands -------- */
  {"addi", 10, 0, I_TYPE},
  {"subi", 11, 0, I_TYPE},
  {"andi", 12, 0, I_TYPE},
  {"ori", 13, 0, I_TYPE},
  {"nori", 14, 0, I_TYPE},
  {"bne", 15, 0, I_TYPE},
  {"beq", 16, 0, I_TYPE},
  {"blt", 17, 0, I_TYPE},
  {"bgt", 18, 0, I_TYPE},
  {"lb", 19, 0, I_TYPE},
  {"sb", 20, 0, I_TYPE},
  {"lw", 21, 0, I_TYPE},
  {"sw", 22, 0, I_TYPE},
  {"lh", 23, 0, I_TYPE},
  {"sh", 24, 0, I_TYPE},
  
  
  /* ------------ J commands -------- */
  {"jmp", 30, 0, J_TYPE},
  {"la", 31, 0, J_TYPE},
  {"call", 32, 0, J_TYPE},
  {"hlt", 63, 0, J_TYPE},
  
};
