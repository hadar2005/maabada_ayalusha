#include <stdio.h>
#include <string.h>
#include "output_writer.h"
#include "data_structures.h"

/*create the output object file - the machine code .ob*/
int write_ob_file(const char *filename, unsigned int *code_image, int icf, unsigned char *data_image, int dcf) {
    
    /*----declare variables----*/
    FILE *file_ptr;

    int address;
    int index;
    int i;
    int j;
    unsigned int inst;
    unsigned char b0, b1, b2, b3;

    /*----------open the file + null check-----------*/

    
    char full_filename[MAX_FILE_NAME];

    /*input check: nulls */
    if (filename == NULL || code_image == NULL || data_image == NULL) {
        return ERROR;
    }

    /*create the full filename - with .ob*/
    sprintf(full_filename, "%s.ob", filename);


    file_ptr = fopen(full_filename, "w");
    if (file_ptr == NULL) {
        return ERROR;
    }

    /*write the header line with icf and dcf*/
    fprintf(file_ptr, "%d %d\n", icf - IC_INIT_VALUE, dcf);

    /*----------write the code image-----------*/

    for (address = IC_INIT_VALUE, index = 0; address < icf; address += INSTRUCTION_SIZE, index++) {
        inst = code_image[index];

        /*extract bytes in little-endian order
          0xFF is a mask to get the least significant byte
          >> 8 shifts to the next byte - 8 bits*/
        b0 = (unsigned char)(inst & 0xFF);
        b1 = (unsigned char)((inst >> 8) & 0xFF);
        b2 = (unsigned char)((inst >> 16) & 0xFF);
        b3 = (unsigned char)((inst >> 24) & 0xFF);

        fprintf(file_ptr, "%04d %02X %02X %02X %02X\n", address, b0, b1, b2, b3);
    }

    /*----------write the data image-----------*/
    /*iterate through the data image*/
    for (i = 0; i < dcf; i += INSTRUCTION_SIZE) {

        /*current address - after instructions*/
        address = icf + i;
        fprintf(file_ptr, "%04d", address);
        
        /*print each byte of the current instruction*/
        for (j = 0; j < INSTRUCTION_SIZE && (i + j) < dcf; j++) {
            fprintf(file_ptr, " %02X", data_image[i + j]);
        }

        fprintf(file_ptr, "\n");
    }

    /*----------close the file-----------*/
    fclose(file_ptr);
    return SUCCESS;
}

/*create the entry file - the symbol table .ent*/
int write_ent_file(const char *filename, SymbolNode *symbol_table_head) {
    FILE *file_ptr;
    char full_filename[MAX_FILE_NAME];
    SymbolNode *current = symbol_table_head;
    int has_entries;

    /*-------if there are no entries - no file-------*/
    /*input check: nulls */
    if (filename == NULL || symbol_table_head == NULL) {
        return SUCCESS; /* No entries to write, but not an error */
    }

    /*check if there are any entries*/
    has_entries = FALSE;
    current = symbol_table_head;
    while (current != NULL) {
        /*if we find an entry in the symbol table, mark has_entries as TRUE*/
        if (current->is_entry) {
            has_entries = TRUE;
            break;
        }
        current = current->next;
    }

    if (!has_entries) {
        return SUCCESS; /* No entries to write, but not an error */
    }

    /*-------if there are entries - create the file-------*/

    /*create the full filename with .ent*/
    sprintf(full_filename, "%s.ent", filename);

    file_ptr = fopen(full_filename, "w");
    if (file_ptr == NULL) {
        printf("Error: Could not open file %s for writing\n", full_filename);
        return ERROR;
    }

    
    /*iterate through the symbol table and write entries*/
    current = symbol_table_head;
    
    while (current != NULL) {
        /*check if current is an entry*/
        if (current->is_entry) {
            fprintf(file_ptr, "%s %04d\n", current->name, current->address);
        }
        current = current->next;
    }

    fclose(file_ptr);
    return SUCCESS;
}

/*create the extern file - the symbol table .ext*/
int write_ext_file(const char *filename, ExtUsageNode *ext_usage_head) {

    /*----declare variables----*/
    FILE *file_ptr;
    char full_filename[MAX_FILE_NAME];
    ExtUsageNode *current;

    /*-------if there are no externs - no file-------*/
    /*input check: nulls */
    if (filename == NULL || ext_usage_head == NULL) {
        return SUCCESS; /* No externs to write, but not an error */
    }

    /*-------if there are externs - create the file-------*/
    
    /*create the full filename with .ext*/
    sprintf(full_filename, "%s.ext", filename);
    file_ptr = fopen(full_filename, "w");
    if (file_ptr == NULL) {
        printf("Error: Could not open file %s for writing\n", full_filename);
        return ERROR;
    }
    
    /*iterate through the ext table and write their addresses*/
    current = ext_usage_head;
    while (current != NULL) {
        fprintf(file_ptr, "%s %04d\n", current->name, current->address);
        current = current->next;
    }

    fclose(file_ptr);
    return SUCCESS;
}