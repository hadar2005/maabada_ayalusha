#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include "data_structures.h"

/*frees the memory allocated for the symbol table*/
void free_symbol_table(SymbolNode *head);

/*frees the memory allocated for the external usage list*/
void free_ext_list(ExtUsageNode *head);

/*frees the memory allocated for the macro table*/
void free_macro_table(MacroNode *head);

#endif /* MEMORY_MANAGER_H */