#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "data_structures.h"

/*
 * Write filename.ob from the code and data images. filename is a basename
 * without an extension; icf and dcf describe the used image lengths. Returns
 * SUCCESS on success or ERROR when arguments or file creation are invalid.
 * The algorithm writes the header, code bytes, and data bytes in order.
 */
int write_ob_file(const char *filename, unsigned int *code_image, int icf, unsigned char *data_image, int dcf);

/*
 * Write filename.ent for symbols marked as entries. Returns SUCCESS when no
 * entries exist or writing succeeds, and ERROR if the file cannot be opened.
 * The function assumes symbol_table_head points to the linked symbol table.
 */
int write_ent_file(const char *filename, SymbolNode *symbol_table_head);


/*
 * Write filename.ext from the external-usage list. Returns SUCCESS when the
 * list is empty or writing succeeds, and ERROR if the file cannot be opened.
 * Each node is written in traversal order with its symbol and address.
 */
int write_ext_file(const char *filename, ExtUsageNode *ext_usage_head);

#endif