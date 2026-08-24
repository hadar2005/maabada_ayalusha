/*
 * Symbol-table module. It maintains the linked list of code, data, external,
 * and entry symbols used by both assembler passes. Names are copied into
 * owned storage in each node; callers must provide valid labels and release
 * the resulting list through the memory manager.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "memory_manager.h"

/*
 * Search the symbol list linearly for name. Return the first matching node or
 * NULL when the name is absent; the list is not modified and name must be a
 * valid null-terminated string.
 */

struct SymbolNode* get_symbol(struct SymbolNode *head, char *name) {
    struct SymbolNode *curr = head;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            return curr; /* Symbol found */
        }
        curr = curr->next;
    }
    return NULL; /* Symbol not found */
}

/*
 * Reject duplicate names, allocate and initialize a symbol, set the flag
 * selected by the caller, and append the node to the list. Return SUCCESS on
 * insertion or ERROR for duplicates/allocation failure. The name must fit
 * the configured label buffer and head must be writable.
 */
 
int insert_to_symbol_table(struct SymbolNode **head, char *name, int address, int flag) {
    struct SymbolNode *curr;
    struct SymbolNode *new_node;

    /* Check if the symbol already exists to prevent duplicates */
    if (get_symbol(*head, name) != NULL) {
        return ERROR; 
    }

    /* Allocate memory for the new symbol */
    new_node = (struct SymbolNode *)malloc(sizeof(struct SymbolNode));
    if (new_node == NULL) {
        printf("Memory allocation failed for symbol %s\n", name);
        return ERROR;
    }

    strcpy(new_node->name, name);
    new_node->address = address;
    
    /* Set all flags to FALSE initially */
    new_node->is_code = FALSE;
    new_node->is_data = FALSE;
    new_node->is_external = FALSE;
    new_node->is_entry = FALSE;

    /* Turn on the specific flag based on the input */
    switch (flag) {
        case FLAG_CODE:
            new_node->is_code = TRUE;
            break;
        case FLAG_DATA:
            new_node->is_data = TRUE;
            break;
        case FLAG_EXTERN:
            new_node->is_external = TRUE;
            /* address is expected to be 0 for externals */
            break;
        case FLAG_ENTRY:
            new_node->is_entry = TRUE;
            break;
    }

    new_node->next = NULL;

    /* Insert at the end of the linked list */
    if (*head == NULL) {
        *head = new_node;
    } else {
        curr = *head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }

    return SUCCESS;
}

/*
 * Convert each data-symbol offset into its final address by adding ICF. The
 * algorithm traverses the list once and changes only nodes marked as data;
 * ICF must be the final instruction counter from Pass 1.
 */
  
void update_data_symbols_address(struct SymbolNode *head, int ICF) {
    struct SymbolNode *curr = head;
    
    while (curr != NULL) {
        if (curr->is_data == TRUE) {
            curr->address += ICF;
        }
        curr = curr->next;
    }
}


