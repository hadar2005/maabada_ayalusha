#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include "data_structures.h"

/*
 * Allocate a macro named macro_name and append it to the table at head.
 * Returns the new node; head and macro_name must be valid, and the name is
 * copied into newly allocated storage. The algorithm traverses to the tail.
 */
struct MacroNode* add_macro(struct MacroNode **head, char *macro_name);

/*
 * Read macro body lines from file_ptr until mcroend and append them to
 * curr_node. The file must be positioned immediately after the macro header;
 * each line is copied into a linked MacroLine list and no value is returned.
 */
void insert_macro_lines(FILE *file_ptr, struct MacroNode *curr_node);

/*
 * Search head for macro_name and return the matching node, or NULL if absent.
 * The function performs a linear traversal and does not modify the table.
 */
struct MacroNode* get_existing_macro(char *macro_name, struct MacroNode *head);

/*
 * Write every stored line in macro to output_file in original order. Both
 * pointers must be valid; the function does not modify the macro or return a
 * status value.
 */
void print_macro_lines(struct MacroNode *macro, FILE *output_file);

#endif
