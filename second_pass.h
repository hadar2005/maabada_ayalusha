#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "data_structures.h"
#include "symbol_table.h"

/*
 * Executes the second pass on the .am file.
 * Completes missing label addresses in the code image and updates the external usage list (.ext).
 * Returns TRUE if errors were found, or FALSE if successful.
 */
int run_second_pass(const char *filename, struct SymbolNode *symbol_table_head, unsigned int *code_image, struct ExtUsageNode **ext_list_head);

#endif