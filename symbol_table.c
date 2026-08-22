#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "memory_manager.h"

/*  Searches for a symbol by its name in the symbol table.
 Returns a pointer to the symbolif found, or NULL if it doesn't exist. */

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

/*  Allocates and inserts a new symbol into the symbol table.
 * Returns SUCCESS (0) if added, or ERROR (-1) if it already exists.*/
 
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

/* Updates the address of all data symbols at the end of Pass 1.
  Adds the final Instruction Counter (ICF) to their current address */
  
void update_data_symbols_address(struct SymbolNode *head, int ICF) {
    struct SymbolNode *curr = head;
    
    while (curr != NULL) {
        if (curr->is_data == TRUE) {
            curr->address += ICF;
        }
        curr = curr->next;
    }
}


