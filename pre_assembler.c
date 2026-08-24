/*
 * Macro pre-assembler module. It stores macro definitions in a linked list,
 * reads their bodies until mcroend, and reproduces stored lines when a macro
 * invocation is encountered. The module assumes writable input buffers and
 * valid file pointers supplied by the assembler driver; memory ownership is
 * released later by free_macro_table in memory_manager.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "pre_assembler.h"

/*
 * Allocate a macro node, copy its name, initialize its line list, and append
 * it to the table. The table is traversed to its tail; the new node is
 * returned so its body can be filled by insert_macro_lines. The caller must
 * later release the table with free_macro_table.
 */
struct MacroNode* add_macro(struct MacroNode **head, char *macro_name) {
    struct MacroNode *curr;
    struct MacroNode *new_macro = (struct MacroNode *)malloc(sizeof(struct MacroNode));
    
    /* Allocate memory for the macro name and copy it */
    new_macro->name = (char *)malloc(strlen(macro_name) + 1);
    strcpy(new_macro->name, macro_name);
    
    new_macro->lines_head = NULL;
    new_macro->lines_tail = NULL;
    new_macro->next = NULL;        

    /* Insert as the first node if list is empty, otherwise append to the end */
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

/*
 * Read and copy source lines into curr_node until the first word is mcroend.
 * Each copied line becomes a MacroLine appended to the node's linked list.
 * The file must be positioned after the macro declaration and the node must
 * have been created by add_macro.
 */
void insert_macro_lines(FILE *file_ptr, struct MacroNode *curr_node) {
    char line[MAX_LINE_LEN];
    char first_word[MAX_WORD_LENGTH];
    struct MacroLine *new_line;

    while (fgets(line, MAX_LINE_LEN, file_ptr) != NULL) {
        
        /* Check if the macro definition ends */
        if (sscanf(line, "%49s", first_word) == 1) {
            if (strcmp(first_word, "mcroend") == 0) {
                break;
            }
        }

        /* Allocate memory for the new line and copy the content */
        new_line = (struct MacroLine *)malloc(sizeof(struct MacroLine));
        strcpy(new_line->line, line);
        new_line->next = NULL;
        
        /* Append the line to the macro's line list */
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

/*
 * Search the macro list from head to its end and compare each stored name with
 * macro_name. Return the matching node without modifying the list, or NULL
 * when no definition exists.
 */
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

/*
 * Traverse macro's stored body and write each line to output_file in its
 * original order. The macro data is read-only during this operation.
 */
void print_macro_lines(struct MacroNode *macro, FILE *output_file) {
    struct MacroLine *curr_line = macro->lines_head;
    while (curr_line != NULL) {
        fputs(curr_line->line, output_file);
        curr_line = curr_line->next;
    }
}
