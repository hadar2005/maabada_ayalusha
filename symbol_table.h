#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "data_structures.h"

/* declare functions to be imported from symbol_table.c */
struct SymbolNode* get_symbol(struct SymbolNode *head, char *name);
int insert_to_symbol_table(struct SymbolNode **head, char *name, int address, int flag);
void update_data_symbols_address(struct SymbolNode *head, int ICF);
void free_symbol_table(struct SymbolNode *head);

#endif
