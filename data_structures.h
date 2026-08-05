#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

/* ==========================================
 * 1. מגבלות גודל של מחרוזות וקלט (String Limits)
 * ========================================== */
#define MAX_LINE_LEN 81       /* אורך מקסימלי של שורת קלט: 80 תווים + תו סיום '\0' */
#define MAX_LABEL_LEN 32      /* אורך מקסימלי של תווית: 31 תווים + תו סיום '\0' */
#define MAX_FILE_NAME 256     /* אורך מקסימלי נפוץ ובטוח לשם של קובץ */
#define MAX_WORD_LENGTH 50    /* אורך מקסימלי למילה/שם מאקרו */

/* ==========================================
 * 2. נתוני זיכרון של המחשב הדמיוני (Memory Constants)
 * ========================================== */
#define IC_INIT_VALUE 100     /* הכתובת ההתחלתית בזיכרון שבה מתחילות ההוראות */
#define INSTRUCTION_SIZE 4    /* גודל של כל פקודה בזיכרון (4 בתים = 32 ביט) */

/* ==========================================
 * 3. קבועי אמת ושקר (Booleans)
 * ========================================== */
#define FALSE 0
#define TRUE 1

/* ==========================================
 * 4. דגלים של טבלת הסמלים (Symbol Table Flags)
 * ========================================== */
#define FLAG_CODE 1
#define FLAG_DATA 2
#define FLAG_ENTRY 3
#define FLAG_EXTERN 4

/* ==========================================
 * 5. קבועי מעברים (Passes Constants)
 * ========================================== */
#define SUCCESS 0
#define ERROR -1

/* ==========================================
 * 6. מבני נתונים (Data Structures)
 * ========================================== */

/* --- מבני נתונים עבור פרישת המאקרו --- */

/* מבנה עבור שורה בודדת בתוך מאקרו */
struct MacroLine {
    char line[MAX_LINE_LEN];
    struct MacroLine *next;     
};

/* מבנה עבור מאקרו שלם בטבלת המאקרואים (רשימה מקושרת) */
struct MacroNode {
    char *name;
    struct MacroLine *lines_head;  
    struct MacroLine *lines_tail;  
    struct MacroNode *next;
};

/* --- מבני נתונים עבור טבלת הסמלים (התוויות) --- */

/* מבנה עבור תווית בטבלת הסמלים (רשימה מקושרת) */
struct SymbolNode {
    char name[MAX_LABEL_LEN];
    int address;
    
    /* דגלים בוליאניים המציינים את סוג התווית */
    int is_code;
    int is_data;
    int is_external;
    int is_entry;
    
    struct SymbolNode *next;
};

#endif
