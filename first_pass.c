#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structures.h"
#include "data_parser.h"
#include "symbol_table.h"
#include "instruction_parser.h"
#include "tables.h"

/* this function reads the am line by line. addresses each line (code/data) to its respective handler */
void run_first_pass(FILE *file_ptr)
{
  char line[MAX_LINE_LEN];
  char word1[MAX_WORD_LENGTH];
  char word2[MAX_WORD_LENGTH];
  char ext_symbol[MAX_WORD_LENGTH];
  char *command_word;
  
  int IC = 100;
  int DC = 0;
  int has_label = FALSE;
  
  /* Added for Error Handling */
  int has_errors = FALSE;
  int line_num = 0; 
  
  unsigned char data_image[4096];
  unsigned char code_image[4096]; 
  
  struct SymbolNode *symbol_table_head = NULL;
  
  while (fgets(line, MAX_LINE_LEN, file_ptr) != NULL)
  {
      line_num++;

      /* zero out first word */
      word1[0] = '\0';
      word2[0] = '\0';

      if (line[0] == '\n' || line[0] == '\0') {
        continue; /* filter empty lines */
      }

      if (line[0] == ';') {
        continue; /* check for comments and ignore them */
      }
      
      /* if there are no words (empty) - continue. else, handle each option. 
      read 2 words in the case the first word is a label*/
      if (sscanf(line, "%s %s", word1, word2) < 1) {
          continue;
      }
      
      if (word1[strlen(word1) - 1] == ':')
      {
        has_label = TRUE;
        word1[strlen(word1) - 1] = '\0';
        command_word = word2;
      }
      else
      {
        has_label = FALSE;
        command_word = word1;
      }

      if (strcmp(command_word, ".db") == 0 || strcmp(command_word, ".dh") == 0 ||
          strcmp(command_word, ".dw") == 0 || strcmp(command_word, ".asciz") == 0) 
      { /* is data */
      
          if (has_label == TRUE) {
              if (insert_to_symbol_table(&symbol_table_head, word1, DC, FLAG_DATA) == ERROR) {
                  printf("Error in line %d: Duplicate label '%s'\n", line_num, word1);
                  has_errors = TRUE;
              }
          }

          if (strcmp(command_word, ".db") == 0) 
          {
            parse_db_directive(line, data_image, &DC);
          }
          else if (strcmp(command_word, ".dh") == 0) 
          {
            parse_dh_directive(line, data_image, &DC);
          }
          else if (strcmp(command_word, ".dw") == 0) 
          {
            parse_dw_directive(line, data_image, &DC);
          }
          else if (strcmp(command_word, ".asciz") == 0) 
          {
            parse_asciz_directive(line, data_image, &DC);
          }
      }
      
      else if (strcmp(command_word, ".entry") == 0) 
      {
        if (has_label == TRUE) {
            printf("Warning in line %d: Label '%s' before .entry is meaningless and will be ignored.\n", line_num, word1);
        }
        continue; /* will be taken care of in the second pass */
      }

      else if (strcmp(command_word, ".extern") == 0)
      {
          if (has_label == TRUE) {
              printf("Warning in line %d: Label '%s' before .extern is meaningless and will be ignored.\n", line_num, word1);
              sscanf(line, "%*s %*s %s", ext_symbol); 
          } else {
              strcpy(ext_symbol, word2);
          }
          
          if (insert_to_symbol_table(&symbol_table_head, ext_symbol, 0, FLAG_EXTERN) == ERROR) {
              printf("Error in line %d: Duplicate label '%s'\n", line_num, ext_symbol);
              has_errors = TRUE;
          }
      }

      else 
      {
          /* is instruction */
          const Command *cmd;
          unsigned int final_instruction = 0;
          char temp_args[MAX_LINE_LEN];
          char *args;
          int rs = 0, rt = 0, rd = 0, immed = 0, reg_flag = 0, address_or_reg = 0;
          
          if (has_label == TRUE) {
              if (insert_to_symbol_table(&symbol_table_head, word1, IC, FLAG_CODE) == ERROR) {
                  printf("Error in line %d: Duplicate label '%s'\n", line_num, word1);
                  has_errors = TRUE;
              }
          }
          
          cmd = find_command(command_word);
          if (cmd == NULL) {
              printf("Error in line %d: Unknown instruction '%s'\n", line_num, command_word);
              has_errors = TRUE;
          } else {
              /* Find where the arguments start */
              args = strstr(line, command_word) + strlen(command_word);
              strcpy(temp_args, args);
              
              if (cmd->type == 'R') {
                  char *p = temp_args;
                  int reg_count = 0;
                  int regs[3] = {0};
                  
                  /* Extract registers */
                  while ((p = strchr(p, '$')) != NULL && reg_count < 3) {
                      p++;
                      regs[reg_count++] = atoi(p);
                  }
                  
                  if (reg_count == 3) {
                      rs = regs[0]; rt = regs[1]; rd = regs[2];
                  } else if (reg_count == 2) { 
                      rs = regs[0]; rd = regs[1]; rt = 0; 
                  }
                  final_instruction = build_r_instruction(cmd->opcode, cmd->funct, rs, rt, rd);
              }
              else if (cmd->type == 'I') {
                  char *tok = strtok(temp_args, " ,\t\n");
                  int arg_idx = 0;
                  
                  while (tok != NULL && arg_idx < 3) {
                      if (tok[0] == '$') {
                          if (arg_idx == 0) rs = atoi(tok + 1);
                          else rt = atoi(tok + 1);
                      } else {
                          if (isalpha(tok[0])) {
                              immed = 0; /* Label detected, leave 0 for second pass */
                          } else {
                              immed = atoi(tok);
                          }
                      }
                      tok = strtok(NULL, " ,\t\n");
                      arg_idx++;
                  }
                  final_instruction = build_i_instruction(cmd->opcode, rs, rt, immed);
              }
              else if (cmd->type == 'J') {
                  char *tok = strtok(temp_args, " ,\t\n");
                  if (tok != NULL) {
                      if (tok[0] == '$') {
                          reg_flag = 1;
                          address_or_reg = atoi(tok + 1);
                      } else {
                          reg_flag = 0;
                          address_or_reg = 0; /* Label detected, leave 0 for second pass */
                      }
                  }
                  final_instruction = build_j_instruction(cmd->opcode, reg_flag, address_or_reg);
              }
              
              /* Store the instruction as 4 distinct bytes in Little-Endian format */
              code_image[IC - 100]     = (unsigned char)(final_instruction & 0xFF);
              code_image[IC - 100 + 1] = (unsigned char)((final_instruction >> 8) & 0xFF);
              code_image[IC - 100 + 2] = (unsigned char)((final_instruction >> 16) & 0xFF);
              code_image[IC - 100 + 3] = (unsigned char)((final_instruction >> 24) & 0xFF);
          }
          
          IC += 4;
      }
  }

  if (has_errors == TRUE) {
      printf("Errors found in the file. Halting process to prevent invalid output.\n");
      free_symbol_table(symbol_table_head);
      return; 
  }

  update_data_symbols_address(symbol_table_head, IC);
}
