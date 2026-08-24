#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "data_structures.h"
#include "symbol_table.h"

/*
 * Process filename.am during the first assembler pass. The function reads
 * directives and instructions, builds the symbol table, writes initial code
 * and data images, and returns the final instruction/data counters through
 * icf and dcf. It returns TRUE when an error is found and FALSE otherwise.
 * filename is the input basename without .am; all output pointers must be
 * valid, writable storage. The algorithm scans the file once and postpones
 * unresolved labels for the second pass.
 */
int run_first_pass(const char *filename, struct SymbolNode **symbol_table_head, unsigned int *code_image, unsigned char *data_image, int *icf, int *dcf);

#endif