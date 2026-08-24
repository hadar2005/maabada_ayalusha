#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include "data_structures.h"

/*
 * Release every node in the symbol table linked list. head may be NULL; the
 * function returns no value and assumes each node was dynamically allocated.
 */
void free_symbol_table(SymbolNode *head);

/*
 * Release every node in the external-usage linked list. head may be NULL;
 * each node is traversed and freed exactly once.
 */
void free_ext_list(ExtUsageNode *head);

/*
 * Release every macro, its dynamically allocated name, and all lines linked
 * to it. head may be NULL; traversal frees inner line lists before macro
 * nodes so no allocated memory is leaked.
 */
void free_macro_table(MacroNode *head);

#endif /* MEMORY_MANAGER_H */