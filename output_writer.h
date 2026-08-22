#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "data_structures.h"

/*create the output object file - the machine code .ob*/
int write_ob_file(const char *filename, unsigned int *code_image, int icf, unsigned char *data_image, int dcf);

/*create the entry file - the symbol table .ent*/
int write_ent_file(const char *filename, SymbolNode *symbol_table_head);


/*create the extern file - the symbol table .ext*/
int write_ext_file(const char *filename, ExtUsageNode *ext_usage_head);

#endif