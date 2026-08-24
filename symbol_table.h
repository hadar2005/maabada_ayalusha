#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "data_structures.h"

/*
 * Search the linked symbol table for name. Returns the matching node or NULL
 * when no match exists. The list is traversed from head to its end.
 */
struct SymbolNode* get_symbol(struct SymbolNode *head, char *name);

/*
 * Allocate and append a symbol with the supplied address and type flag.
 * Returns SUCCESS when inserted or ERROR for a duplicate/allocation failure.
 * head must point to the table pointer, and name must fit the label limit.
 */
int insert_to_symbol_table(struct SymbolNode **head, char *name, int address, int flag);

/*
 * Add the final instruction counter ICF to every data-symbol address. This
 * mutates the existing list in place and assumes data symbols currently store
 * offsets relative to the beginning of the data image.
 */
void update_data_symbols_address(struct SymbolNode *head, int ICF);

/*
 * Release all nodes in the symbol table. This declaration is retained here
 * for compatibility; the implementation is provided by memory_manager.c.
 */
void free_symbol_table(struct SymbolNode *head);

#endif
