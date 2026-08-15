#include <stdio.h>
#include <string.h>
#include "instruction_parser.h"

void run_comma_param_tests() {

    /* כל המשתנים של הפונקציה מוצהרים כאן בהתחלה המוחלטת - חובה לפי תקן C90 */
    char *input1[] = "add";
    char input1_copy[50];
    char *input2[] = "ori";
    char input2_copy[50];
    char *input3[] = "shit";
    char input3_copy[50];
    char *result;
    int index;

    /* מקרה בדיקה 1 */
    printf("=== בדיקה 1 ===\n");
    printf("הקלט המקורי: [%s]\n", *input1);
    printf(find_command(*input1);
    strcpy(input1_copy, input1);
    index = 1;

    

    /* מקרה בדיקה 2 */
    printf("=== בדיקה 2: קלט יחיד בלי פסיקים ===\n");
    printf("הקלט המקורי: [%s]\n", input2);
    strcpy(input2_copy, input2);
    index = 1;

    result = get_next_comma_param(input2_copy);
    while (result != NULL) {
        printf("  פרמטר %d: [%s]\n", index++, result);
        result = get_next_comma_param(NULL);
    }
    printf("\n");

    /* מקרה בדיקה 3 */
    printf("=== בדיקה 3: מחרוזות ריקות בין פסיקים ===\n");
    printf("הקלט המקורי: [%s]\n", input3);
    strcpy(input3_copy, input3);
    index = 1;

    result = get_next_comma_param(input3_copy);
    while (result != NULL) {
        printf("  פרמטר %d: [%s]\n", index++, result);
        result = get_next_comma_param(NULL);
    }
    printf("\n");
    
}

int main() {
    run_comma_param_tests();
    return 0;
}
