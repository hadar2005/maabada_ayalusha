#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#define MAX_FILENAME_LEN 256
#define MAX_LINE_LEN 81
#define MAX_WORD_LENGTH 50

struct MacroLine {
    char line[MAX_LINE_LEN];
    struct MacroLine *next;     
};

struct MacroNode {
    char *name;
    struct MacroLine *lines_head;  
    struct MacroLine *lines_tail;  
    struct MacroNode *next;
};

#endif

struct MacroNode* add_macro(struct MacroNode **head, char *macro_name) {
    struct MacroNode *curr;
    struct MacroNode *new_macro = (struct MacroNode *)malloc(sizeof(struct MacroNode));
    
    new_macro->name = (char *)malloc(strlen(macro_name) + 1);
    strcpy(new_macro->name, macro_name);
    
    new_macro->lines_head = NULL;
    new_macro->lines_tail = NULL;
    new_macro->next = NULL;        

    if (*head == NULL) {
        *head = new_macro;
    } else {
        curr = *head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_macro;
    }

    return new_macro; 
}

void insert_macro_lines(FILE *file_ptr, struct MacroNode *curr_node) {
    char line[MAX_LINE_LEN];
    char first_word[MAX_WORD_LENGTH];
    struct MacroLine *new_line;

    while (fgets(line, MAX_LINE_LEN, file_ptr) != NULL) {
        
        if (sscanf(line, "%49s", first_word) == 1) {
            if (strcmp(first_word, "mcroend") == 0) {
                break;
            }
        }

        new_line = (struct MacroLine *)malloc(sizeof(struct MacroLine));
        strcpy(new_line->line, line);
        new_line->next = NULL;
        
        if (curr_node->lines_head == NULL) {
            curr_node->lines_head = new_line;
            curr_node->lines_tail = new_line;
        } 
        else {
            curr_node->lines_tail->next = new_line;
            curr_node->lines_tail = new_line;
        }
    }
}

void free_macro_table(struct MacroNode *head) {
    struct MacroNode *curr_macro = head;
    struct MacroNode *next_macro;
    struct MacroLine *curr_line;
    struct MacroLine *next_line;

    while (curr_macro != NULL) {
        curr_line = curr_macro->lines_head;
        while (curr_line != NULL) {
            next_line = curr_line->next;
            free(curr_line);
            curr_line = next_line;
        }

        next_macro = curr_macro->next;
        free(curr_macro->name);
        free(curr_macro);
        curr_macro = next_macro;
    }
}

struct MacroNode* get_existing_macro(char *macro_name, struct MacroNode *head) {
    struct MacroNode *curr_macro = head;
    while (curr_macro != NULL) {
        if (strcmp(curr_macro->name, macro_name) == 0) {
            return curr_macro; 
        }
        curr_macro = curr_macro->next;
    }
    return NULL; 
}

void print_macro_lines(struct MacroNode *macro, FILE *output_file) {
    struct MacroLine *curr_line = macro->lines_head;
    while (curr_line != NULL) {
            fputs(curr_line->line, output_file);
            curr_line = curr_line->next;
        }
}

int main(int argc, char *argv[]) 
{
    int i;
    int sizeof_name;
    FILE *input_file_ptr;
    FILE *output_file_ptr;
    char line[MAX_LINE_LEN];
    char first_word[MAX_WORD_LENGTH];
    char macro_name[MAX_WORD_LENGTH];
    char *output_file_name;
    char *input_file_name;
    
    struct MacroNode *macro_head = NULL; 
    struct MacroNode *curr_macro = NULL;

    if (argc == 1) {
        printf("You must enter files to assemble\n");
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        sizeof_name = strlen(argv[i]) + strlen(".am") + 1;
        output_file_name = (char *)malloc(sizeof_name);
        input_file_name = (char *)malloc(sizeof_name);
        sprintf(output_file_name, "%s.am", argv[i]);
        sprintf(input_file_name, "%s.as", argv[i]);
        input_file_ptr = fopen(input_file_name, "r");
        
        if (input_file_ptr == NULL) {
            perror(argv[i]);
            free(output_file_name);
            free(input_file_name);
            continue;
        }

        printf("=== Processing file: %s ===\n", argv[i]);
        
        output_file_ptr = fopen(output_file_name, "w");
        if (output_file_ptr == NULL) {
            printf("Error: couldn't create output file %s\n", output_file_name);
            fclose(input_file_ptr);
            free(output_file_name);
            return 1; 
        }

        while (fgets(line, MAX_LINE_LEN, input_file_ptr) != NULL) {
            int result = sscanf(line, "%49s %49s", first_word, macro_name);
            
            if (result >= 1) {
                if (strcmp(first_word, "mcro") == 0) {
                    curr_macro = add_macro(&macro_head, macro_name);
                    insert_macro_lines(input_file_ptr, curr_macro);
                    continue; 
                }
                
                struct MacroNode *found_macro = get_existing_macro(first_word, macro_head);
                if (found_macro != NULL) {
                    print_macro_lines(found_macro, output_file_ptr);
                    continue; 
                }
            }
            
            fputs(line, output_file_ptr);
        }

        fclose(input_file_ptr);
        fclose(output_file_ptr); 
        free_macro_table(macro_head);
        free(output_file_name);
        free(input_file_name);
        macro_head = NULL; 
    }

    return 0;
}
