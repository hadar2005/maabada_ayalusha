#ifndef DATA_PARSER_H
#define DATA_PARSER_H
#include "data_structures.h"

/* declare functions to be imported from data_parser.c */
void parse_db_directive(char *line, unsigned char *data_image, int *DC);
void parse_dh_directive(char *line, unsigned char *data_image, int *DC);
void parse_dw_directive(char *line, unsigned char *data_image, int *DC);
void parse_asciz_directive(char *line, unsigned char *data_image, int *DC);

#endif
