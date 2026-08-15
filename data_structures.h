#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

/* ==========================================
 * 1. String and Input Size Limits
 * ========================================== */
#define MAX_LINE_LEN 81       /* Maximum length of an input line: 80 characters + '\0' terminator */
#define MAX_LABEL_LEN 32      /* Maximum length of a label: 31 characters + '\0' terminator */
#define MAX_FILE_NAME 256     /* Common and safe maximum length for a file name */
#define MAX_WORD_LENGTH 50    /* Maximum length for a word/macro name */

/* ==========================================
 * 2. Imaginary Computer Memory Constants
 * ========================================== */
#define IC_INIT_VALUE 100     /* Initial memory address where instructions begin */
#define INSTRUCTION_SIZE 4    /* Size of each instruction in memory (4 bytes = 32 bits) */

/* ==========================================
 * 3. Booleans
 * ========================================== */
#define FALSE 0
#define TRUE 1

/* ==========================================
 * 4. Symbol Table Flags
 * ========================================== */
#define FLAG_CODE 1
#define FLAG_DATA 2
#define FLAG_ENTRY 3
#define FLAG_EXTERN 4

/* ==========================================
 * 5. Passes Constants
 * ========================================== */
#define SUCCESS 0
#define ERROR -1

/* ==========================================
 * 6. Data Structures
 * ========================================== */

/* --- Data Structures for Macro Expansion --- */

/* Structure for a single line inside a macro */
typedef struct MacroLine {
    char line[MAX_LINE_LEN];
    struct MacroLine *next;     
} MacroLine;

/* Structure for a complete macro in the macro table (linked list) */
typedef struct MacroNode {
    char *name;
    struct MacroLine *lines_head;  
    struct MacroLine *lines_tail;  
    struct MacroNode *next;
} MacroNode;

/* --- Data Structures for the Symbol Table (Labels) --- */

/* Structure for a label in the symbol table (linked list) */
typedef struct SymbolNode {
    char name[MAX_LABEL_LEN];
    int address;
    
    /* Boolean flags indicating the type of the label */
    int is_code;
    int is_data;
    int is_external;
    int is_entry;
    
    struct SymbolNode *next;
} SymbolNode;

/* Structure for external label usage tracking (.ext file) */
typedef struct ExtUsageNode {
    char name[MAX_LABEL_LEN];
    int address;
    struct ExtUsageNode *next;
} ExtUsageNode;

#endif
