#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "data_structures.h"
#include "symbol_table.h"

/*
 * Process filename.am during the second assembler pass. Resolve label
 * operands in code_image, mark .entry symbols, and append external references
 * to ext_list_head. Returns TRUE when an undefined label or file error is
 * found, and FALSE otherwise. filename is a basename without .am; the
 * symbol table and output pointers must remain valid for the whole scan.
 * The algorithm scans the expanded file and patches each instruction after
 * looking up its referenced label.
 */
int run_second_pass(const char *filename, struct SymbolNode *symbol_table_head, unsigned int *code_image, struct ExtUsageNode **ext_list_head);

#endif