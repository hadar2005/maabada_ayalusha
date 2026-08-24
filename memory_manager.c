/*
 * Memory-management module. It frees the linked lists allocated while
 * assembling a file: symbols, external usages, and macros with nested body
 * lines. Each routine assumes list nodes were allocated dynamically and is
 * safe to call with a NULL head.
 */

#include "data_structures.h"
#include <stdlib.h>

/* Traverse and free every symbol-table node exactly once. */
void free_symbol_table(SymbolNode *head) {

    SymbolNode *current = head;
    SymbolNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/* Traverse and free every external-usage node exactly once. */
void free_ext_list(ExtUsageNode *head) {

    ExtUsageNode *current = head;
    ExtUsageNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/* Free each macro body before freeing its name and macro-table node. */
void free_macro_table(MacroNode *head) {
    MacroNode *current = head;
    MacroNode *temp;
    MacroLine *line_current;
    MacroLine *line_temp;

    while (current != NULL) {
        temp = current;

        /* Free the lines associated with the macro */
        line_current = current->lines_head;
        while (line_current != NULL) {
            line_temp = line_current;
            line_current = line_current->next;
            free(line_temp);
        }

        current = current->next;

        /* Free macro name */
        if (temp->name != NULL) {
            free(temp->name);
        }

        free(temp);
    }
}
