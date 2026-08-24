/*
 * Assembler driver. The program converts one or more source files from the
 * assembly language into object, entry, and external-reference files. It
 * first expands macros into .am files, then performs two passes: Pass 1
 * builds symbols and provisional images, while Pass 2 resolves labels. The
 * input is supplied as command-line basenames and is assumed to use the
 * project's instruction and directive syntax; output files are written next
 * to the source files and dynamically allocated lists are released per file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_writer.h"
#include "memory_manager.h"

/*
 * Coordinate macro expansion, both assembler passes, output generation, and
 * cleanup for every command-line input. The algorithm processes files in
 * order, skips files with errors, and writes output only after both passes
 * succeed. argc/argv contain source basenames; the function returns zero
 * after processing the supplied arguments, or one when no input is given.
 */
int main(int argc, char *argv[]) 
{
    /* C90 standard requires declaring all variables at the beginning of the block */
    int i;
    int sizeof_name;
    FILE *input_file_ptr;
    FILE *output_file_ptr;
    char line[MAX_LINE_LEN];
    char first_word[MAX_WORD_LENGTH];
    char macro_name[MAX_WORD_LENGTH];
    char *output_file_name;
    char *input_file_name;
    
    struct MacroNode *macro_head = NULL; 
    struct MacroNode *curr_macro = NULL;
    struct MacroNode *found_macro = NULL;

    /* Variables for the first and second passes */
    unsigned int code_image[4096]; 
    unsigned char data_image[4096];
    int icf, dcf;
    struct ExtUsageNode *ext_list_head;
    struct SymbolNode *symbol_table_head;

    /* Validate command line arguments */
    if (argc == 1) {
        printf("You must enter files to assemble\n");
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    /* Iterate through all provided assembly files */
    for (i = 1; i < argc; i++) {
        /* Allocate memory for file names and open the input file (.as) */
        sizeof_name = strlen(argv[i]) + strlen(".am") + 1;
        output_file_name = (char *)malloc(sizeof_name);
        input_file_name = (char *)malloc(sizeof_name);
        sprintf(output_file_name, "%s.am", argv[i]);
        sprintf(input_file_name, "%s.as", argv[i]);
        input_file_ptr = fopen(input_file_name, "r");
        
        if (input_file_ptr == NULL) {
            perror(argv[i]);
            free(output_file_name);
            free(input_file_name);
            continue; /* Skip to the next file if this one cannot be opened */
        }

        printf("=== Processing file: %s ===\n", argv[i]);
        
        /* Open the output file for the macro expansion (.am) */
        output_file_ptr = fopen(output_file_name, "w");
        if (output_file_ptr == NULL) {
            printf("Error: couldn't create output file %s\n", output_file_name);
            fclose(input_file_ptr);
            free(output_file_name);
            free(input_file_name);
            return 1; 
        }

        /* ----- PRE-ASSEMBLER STAGE (Macro Expansion) ----- */
        while (fgets(line, MAX_LINE_LEN, input_file_ptr) != NULL) {
            int result = sscanf(line, "%49s %49s", first_word, macro_name);
            
            if (result >= 1) {
                /* If a new macro is defined, add it to the table */
                if (strcmp(first_word, "mcro") == 0) {
                    curr_macro = add_macro(&macro_head, macro_name);
                    insert_macro_lines(input_file_ptr, curr_macro);
                    continue; 
                }
                
                /* If a known macro is called, expand it in the output file */
                found_macro = get_existing_macro(first_word, macro_head);
                if (found_macro != NULL) {
                    print_macro_lines(found_macro, output_file_ptr);
                    continue; 
                }
            }
            
            /* If it's a regular line, just copy it to the .am file */
            fputs(line, output_file_ptr);
        }

        /* Close files and clean up macro memory for the current file */
        fclose(input_file_ptr);
        fclose(output_file_ptr); 
        free_macro_table(macro_head);
        macro_head = NULL; 

        /* ----- FIRST AND SECOND PASS STAGES ----- */
        
        /* Reset tracking variables and memory images for the current file */
        memset(code_image, 0, sizeof(code_image));
        memset(data_image, 0, sizeof(data_image));
        icf = 0;
        dcf = 0;
        ext_list_head = NULL;
        symbol_table_head = NULL;

        /* Execute the First Pass: builds symbol table and basic data/code images */
        if (run_first_pass(argv[i], &symbol_table_head, code_image, data_image, &icf, &dcf) == TRUE) {
            printf("Errors found in first pass. Skipping to next file.\n");
            free_symbol_table(symbol_table_head);
            free(output_file_name);
            free(input_file_name);
            continue; 
        }

        /* Execute the Second Pass: completes missing label addresses */
        if (run_second_pass(argv[i], symbol_table_head, code_image, &ext_list_head) == TRUE) {
            printf("Errors found in second pass. Skipping to next file.\n");
            free_symbol_table(symbol_table_head);
            free_ext_list(ext_list_head);
            free(output_file_name);
            free(input_file_name);
            continue;
        }

        /* ----- EXPORTING OUTPUT FILES ----- */
        
        /* If both passes were successful, generate the final files (.ob, .ent, .ext) */
        write_ob_file(argv[i], code_image, icf, data_image, dcf);
        write_ent_file(argv[i], symbol_table_head);
        write_ext_file(argv[i], ext_list_head);

        /* Final cleanup before moving to the next command-line argument */
        free_symbol_table(symbol_table_head);
        free_ext_list(ext_list_head);
        free(output_file_name);
        free(input_file_name);
    }

    return 0;
}