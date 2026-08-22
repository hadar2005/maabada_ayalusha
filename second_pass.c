#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structures.h"
#include "symbol_table.h"
#include "second_pass.h"

#define J_TYPE_MASK 0x01FFFFFF
#define I_TYPE_MASK 0x0000FFFF

int run_second_pass(const char *filename, struct SymbolNode *symbol_table_head, unsigned int *code_image, struct ExtUsageNode **ext_list_head) {
    FILE *file_ptr;
    char line[MAX_LINE_LEN];
    char full_filename[MAX_FILE_NAME];
    int IC = IC_INIT_VALUE;
    int line_number = 0;
    int has_errors = FALSE;

    sprintf(full_filename, "%s.am", filename);
    file_ptr = fopen(full_filename, "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file %s for second pass.\n", full_filename);
        return TRUE;
    }

    while (fgets(line, MAX_LINE_LEN, file_ptr) != NULL) {
        char temp_line[MAX_LINE_LEN];
        char word1[MAX_WORD_LENGTH] = {0};
        char word2[MAX_WORD_LENGTH] = {0};
        char *command_word = NULL;
        char *rest_of_line = NULL;
        int result;

        line_number++;
        strcpy(temp_line, line);
        
        result = sscanf(temp_line, "%s %s", word1, word2);
        if (result < 1) continue;
        if (word1[0] == ';') continue;

        if (word1[strlen(word1) - 1] == ':') {
            command_word = word2;
            rest_of_line = strstr(temp_line, word2) + strlen(word2);
        } else {
            command_word = word1;
            rest_of_line = strstr(temp_line, word1) + strlen(word1);
        }

        if (command_word == NULL || command_word[0] == '\0') {
            continue;
        }

        if (strcmp(command_word, ".db") == 0 || strcmp(command_word, ".dh") == 0 ||
            strcmp(command_word, ".dw") == 0 || strcmp(command_word, ".asciz") == 0 ||
            strcmp(command_word, ".extern") == 0) {
            continue;
        }
        else if (strcmp(command_word, ".entry") == 0) {
            char entry_label[MAX_WORD_LENGTH] = {0};
            struct SymbolNode *sym;
            sscanf(rest_of_line, "%s", entry_label);

            sym = get_symbol(symbol_table_head, entry_label);
            if (sym == NULL) {
                printf("Error in line %d: Entry label '%s' not defined.\n", line_number, entry_label);
                has_errors = TRUE;
            } else {
                sym->is_entry = TRUE;
            }
        }
        else {
            int is_j_type = (strcmp(command_word, "jmp") == 0 || strcmp(command_word, "la") == 0 || strcmp(command_word, "call") == 0);
            int is_branch = (strcmp(command_word, "beq") == 0 || strcmp(command_word, "bne") == 0 || strcmp(command_word, "blt") == 0 || strcmp(command_word, "bgt") == 0);

            if (is_j_type || is_branch) {
                char label_operand[MAX_WORD_LENGTH] = {0};
                struct SymbolNode *sym;

                if (is_j_type) {
                    sscanf(rest_of_line, "%s", label_operand);
                    
                    /* Skip register operands (e.g., jmp $4) */
                    if (label_operand[0] == '$') {
                        IC += 4;
                        continue;
                    }
                } else if (is_branch) {
                    char *token = strtok(rest_of_line, ",");
                    token = strtok(NULL, ",");
                    token = strtok(NULL, " \t\n\r");
                    if (token) {
                        strcpy(label_operand, token);
                    }
                }

                if (label_operand[0] != '\0') {
                    sym = get_symbol(symbol_table_head, label_operand);
                    if (sym == NULL) {
                        printf("Error in line %d: Label '%s' used but not defined.\n", line_number, label_operand);
                        has_errors = TRUE;
                    } else {
                        int instruction_index = (IC - IC_INIT_VALUE) / 4;

                        if (sym->is_external) {
                            struct ExtUsageNode *new_ext = (struct ExtUsageNode *)malloc(sizeof(struct ExtUsageNode));
                            strcpy(new_ext->name, label_operand);
                            new_ext->address = IC;
                            new_ext->next = NULL;

                            if (*ext_list_head == NULL) {
                                *ext_list_head = new_ext;
                            } else {
                                struct ExtUsageNode *curr = *ext_list_head;
                                while (curr->next != NULL) curr = curr->next;
                                curr->next = new_ext;
                            }
                        } else {
                            if (is_j_type) {
                                code_image[instruction_index] |= (sym->address & J_TYPE_MASK);
                            } else if (is_branch) {
                                int distance = sym->address - IC;
                                code_image[instruction_index] |= (distance & I_TYPE_MASK);
                            }
                        }
                    }
                }
            }
            IC += 4;
        }
    }

    fclose(file_ptr);
    return has_errors;
}