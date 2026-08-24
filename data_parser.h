#ifndef DATA_PARSER_H
#define DATA_PARSER_H
#include "data_structures.h"

/*
 * Parse a .db directive, store each value as one byte in data_image, and
 * advance DC by one for every value. The input line and data_image must be
 * valid writable buffers, and DC must point to the current data position.
 */
void parse_db_directive(char *line, unsigned char *data_image, int *DC);

/*
 * Parse a .dh directive, store each value as two little-endian bytes in
 * data_image, and advance DC by two for every value. The line is tokenized
 * in place, so it must be writable; DC must point to a valid data position.
 */
void parse_dh_directive(char *line, unsigned char *data_image, int *DC);

/*
 * Parse a .dw directive, store each value as four little-endian bytes in
 * data_image, and advance DC by four for every value. The caller supplies a
 * writable line buffer and a pointer to the current data position.
 */
void parse_dw_directive(char *line, unsigned char *data_image, int *DC);

/*
 * Validate a quoted .asciz string, copy its characters and terminating null
 * byte into data_image, and update DC. Return TRUE for valid syntax and FALSE
 * otherwise. The line must contain only optional whitespace after the closing
 * quote; invalid input leaves data_image and DC unchanged.
 */
int parse_asciz_directive(char *line, unsigned char *data_image, int *DC);

#endif
