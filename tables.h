#ifndef TABLES_H
#define TABLES_H

/*defining InstructionType as only R, J or I*/
typedef enum { R_TYPE, I_TYPE, J_TYPE } InstructionType;

/*creating struct for an assembly command - name, opcode, funct, and type*/
typedef struct {
  char name[5]; /*longest command name in assembly is 4, + end char*/
  int opcode;
  int funct;
  InstructionType type; /*is it R, J or I?*/
} Command;

/*the array of commands needs to be external, we will fill it in table.c
there 27 unique instructions in assembly*/
extern const Command commands_table[27]; 

#endif
