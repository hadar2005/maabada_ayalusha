/*
 * First-pass assembler module. It reads macro-expanded .am input, records
 * code/data/external symbols, parses data directives, and creates provisional
 * instruction and data images. Unresolved labels remain zero for Pass 2;
 * the input follows the syntax and fixed-size limits in data_structures.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structures.h"
#include "data_parser.h"
#include "symbol_table.h"
#include "instruction_parser.h"
#include "tables.h"
#include "first_pass.h"

/*
 * Scan the expanded file once, classify each line, update IC/DC, and encode
 * every instruction whose operands are immediately known. Labels are added
 * to the symbol table and unresolved operands are left for Pass 2. Return
 * TRUE if any input error is detected, otherwise FALSE. output pointers must
 * refer to valid writable tables, images, and counters.
 */
int run_first_pass(const char *filename, struct SymbolNode **symbol_table_head, unsigned int *code_image, unsigned char *data_image, int *icf, int *dcf)
{
    char line[MAX_LINE_LEN];
    char word1[MAX_WORD_LENGTH];
    char word2[MAX_WORD_LENGTH];
    char ext_symbol[MAX_WORD_LENGTH];
    char *command_word;
    char full_filename[MAX_FILE_NAME];
    FILE *file_ptr;

    int IC = 100;
    int DC = 0;
    int has_label = FALSE;
    int has_errors = FALSE;
    int line_num = 0; 
  
    /* Open the .am file to read the macro-expanded assembly */
    sprintf(full_filename, "%s.am", filename);
    file_ptr = fopen(full_filename, "r");
    if (file_ptr == NULL) {
        printf("Error: Cannot open file %s\n", full_filename);
        return TRUE; 
    }
  
    while (fgets(line, MAX_LINE_LEN, file_ptr) != NULL)
    {
        line_num++;

        /* Zero out strings to prevent garbage data from previous iterations */
        word1[0] = '\0';
        word2[0] = '\0';

        if (line[0] == '\n' || line[0] == '\0') {
            continue; /* Filter empty lines */
        }

        if (line[0] == ';') {
            continue; /* Ignore comment lines */
        }
      
        /* Read up to two words. If empty, continue */
        if (sscanf(line, "%s %s", word1, word2) < 1) {
            continue;
        }
      
        /* Check if the first word is a label (ends with ':') */
        if (word1[strlen(word1) - 1] == ':')
        {
            has_label = TRUE;
            word1[strlen(word1) - 1] = '\0'; /* Remove the colon */
            command_word = word2;
        }
        else
        {
            has_label = FALSE;
            command_word = word1;
        }

        /* ----- HANDLE DATA DIRECTIVES ----- */
        if (strcmp(command_word, ".db") == 0 || strcmp(command_word, ".dh") == 0 ||
            strcmp(command_word, ".dw") == 0 || strcmp(command_word, ".asciz") == 0) 
        { 
            if (has_label == TRUE) {
                /* Insert data label with current DC address */
                if (insert_to_symbol_table(symbol_table_head, word1, DC, FLAG_DATA) == ERROR) {
                    printf("Error in line %d: Duplicate label '%s'\n", line_num, word1);
                    has_errors = TRUE;
                }
            }

            /* Parse values and advance DC according to data type */
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
                if (parse_asciz_directive(line, data_image, &DC) == 0) {
                    has_errors = TRUE;
                }
            }
        }
      
        /* ----- HANDLE .ENTRY DIRECTIVE ----- */
        else if (strcmp(command_word, ".entry") == 0) 
        {
            if (has_label == TRUE) {
                printf("Warning in line %d: Label '%s' before .entry is meaningless and will be ignored.\n", line_num, word1);
            }
            continue; /* Entries are handled strictly in the second pass */
        }

        /* ----- HANDLE .EXTERN DIRECTIVE ----- */
        else if (strcmp(command_word, ".extern") == 0)
        {
            if (has_label == TRUE) {
                printf("Warning in line %d: Label '%s' before .extern is meaningless and will be ignored.\n", line_num, word1);
                sscanf(line, "%*s %*s %s", ext_symbol); 
            } else {
                strcpy(ext_symbol, word2);
            }
          
            /* Insert extern label with address 0 */
            if (insert_to_symbol_table(symbol_table_head, ext_symbol, 0, FLAG_EXTERN) == ERROR) {
                printf("Error in line %d: Duplicate label '%s'\n", line_num, ext_symbol);
                has_errors = TRUE;
            }
        }

        /* ----- HANDLE INSTRUCTIONS ----- */
        else 
        {
            const Command *cmd;
            unsigned int final_instruction = 0;
            char temp_args[MAX_LINE_LEN];
            char *args;
            int rs = 0, rt = 0, rd = 0, immed = 0, reg_flag = 0, address_or_reg = 0;
          
            if (has_label == TRUE) {
                /* Insert code label with current IC address */
                if (insert_to_symbol_table(symbol_table_head, word1, IC, FLAG_CODE) == ERROR) {
                    printf("Error in line %d: Duplicate label '%s'\n", line_num, word1);
                    has_errors = TRUE;
                }
            }
          
            cmd = find_command(command_word);
            if (cmd == NULL) {
                printf("Error in line %d: Unknown instruction '%s'\n", line_num, command_word);
                has_errors = TRUE;
            } else {
                /* Locate arguments after the command */
                args = strstr(line, command_word) + strlen(command_word);
                strcpy(temp_args, args);
              
                if (cmd->type == R_TYPE) {
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
                else if (cmd->type == I_TYPE) {
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
                else if (cmd->type == J_TYPE) {
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
              
                /* Store the entire 32-bit instruction into the code array index */
                code_image[(IC - 100) / 4] = final_instruction;
            }
          
            IC += 4; /* Advance IC by 4 bytes per instruction */
        }
    }

    /* Update data symbols with the final Instruction Counter (ICF) */
    update_data_symbols_address(*symbol_table_head, IC);
    
    /* Pass the final memory counters back to the main file */
    *icf = IC;
    *dcf = DC;
    
    fclose(file_ptr);
    return has_errors;
}