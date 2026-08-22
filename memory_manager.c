#include "data_structures.h"
#include <stdlib.h>

/*frees the memory allocated for the symbol table*/
void free_symbol_table(SymbolNode *head) {

    SymbolNode *current = head;
    SymbolNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/*frees the memory allocated for the external usage list*/
void free_ext_list(ExtUsageNode *head) {

    ExtUsageNode *current = head;
    ExtUsageNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

/*frees the memory allocated for the macro table*/
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
