#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structures.h"
#include "data_parser.h" 

/* Parses .db directive (1 byte per integer) */
void parse_db_directive(char *line, unsigned char *data_image, int *DC) {
    char *token; 
    char *args = strstr(line, ".db");
    
    if (args == NULL) return;
    args += 3; /* Skip the ".db" part */
    
    /* split by commas, spaces, and tabs */
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        int value = atoi(token);
        
        /* Store 1 byte (8 bits) and increment DC */
        data_image[*DC] = (unsigned char)(value & 0xFF); /* this performs bitwise AND with FF, which takes the lowest byte of the number*/
        (*DC) += 1;
        
        token = strtok(NULL, " ,\t\n");
    }
}

/* Parses .dh directive (2 bytes per integer, Little-Endian) */
void parse_dh_directive(char *line, unsigned char *data_image, int *DC) {
    char *token;
    char *args = strstr(line, ".dh");
    
    if (args == NULL) return;
    args += 3; /* Skip the ".dh" part */
    
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        int value = atoi(token);
        
        /* Store 2 bytes in Little-Endian format (lower byte first) */
        data_image[*DC] = (unsigned char)(value & 0xFF);
        data_image[*DC + 1] = (unsigned char)((value >> 8) & 0xFF);
        
        (*DC) += 2;
        token = strtok(NULL, " ,\t\n");
    }
}

/* Parses .dw directive (4 bytes per integer, Little-Endian) */
void parse_dw_directive(char *line, unsigned char *data_image, int *DC) {
    char *token;
    char *args = strstr(line, ".dw");
    
    if (args == NULL) return;
    args += 3; /* Skip the ".dw" part */
    
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        long value = atol(token);
        
        /* Store 4 bytes in Little-Endian format */
        data_image[*DC] = (unsigned char)(value & 0xFF);
        data_image[*DC + 1] = (unsigned char)((value >> 8) & 0xFF);
        data_image[*DC + 2] = (unsigned char)((value >> 16) & 0xFF);
        data_image[*DC + 3] = (unsigned char)((value >> 24) & 0xFF);
        
        (*DC) += 4;
        token = strtok(NULL, " ,\t\n");
    }
}

/* Parses .asciz directive (stores string characters and adds '\0' at the end) */
void parse_asciz_directive(char *line, unsigned char *data_image, int *DC) {
    char *start_quote = strchr(line, '"');
    char *end_quote;
    char *curr;
    
    if (start_quote == NULL) return;
    
    start_quote++; /* Move past the first quote */
    end_quote = strrchr(start_quote, '"'); /* Find the closing quote */
    
    if (end_quote == NULL) return;
    
    /* Copy characters to data_image */
    for (curr = start_quote; curr < end_quote; curr++) {
        data_image[*DC] = (unsigned char)(*curr);
        (*DC) += 1;
    }
    
    /* Add the null terminator (\0) at the end of the string */
    data_image[*DC] = '\0';
    (*DC) += 1;
}
