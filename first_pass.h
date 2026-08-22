#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "data_structures.h"
#include "symbol_table.h"

/* 
 * Executes the first pass on the .am file.
 * Builds the symbol table and initial data/code images.
 */
int run_first_pass(const char *filename, struct SymbolNode **symbol_table_head, unsigned int *code_image, unsigned char *data_image, int *icf, int *dcf);

#endif