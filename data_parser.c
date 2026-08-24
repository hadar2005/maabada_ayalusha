/*
 * Parses data directives and stores their values in the data image.
 * The input line must be writable, and the image must have enough space.
 * Multi-byte values are stored in little-endian order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structures.h"
#include "data_parser.h" 

/*
 * Reads the numbers after .db and stores one byte for each number.
 * The line is split into tokens, and DC is increased after every byte.
 */
void parse_db_directive(char *line, unsigned char *data_image, int *DC) {
    char *token; 
    char *args = strstr(line, ".db");
    
    if (args == NULL) return;
    args += 3; /* Skip .db */
    
    /* Separate the values by commas or whitespace. */
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        int value = atoi(token);
        
        /* Keep only the lowest byte. */
        data_image[*DC] = (unsigned char)(value & 0xFF);
        (*DC) += 1;
        
        token = strtok(NULL, " ,\t\n");
    }
}

/*
 * Reads the numbers after .dh and stores each one in two bytes,
 * low byte first. DC is increased by two for every value.
 */
void parse_dh_directive(char *line, unsigned char *data_image, int *DC) {
    char *token;
    char *args = strstr(line, ".dh");
    
    if (args == NULL) return;
    args += 3; /* Skip .dh */
    
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        int value = atoi(token);
        
        /* Store the low byte before the high byte. */
        data_image[*DC] = (unsigned char)(value & 0xFF);
        data_image[*DC + 1] = (unsigned char)((value >> 8) & 0xFF);
        
        (*DC) += 2;
        token = strtok(NULL, " ,\t\n");
    }
}

/*
 * Reads the numbers after .dw and stores each one in four bytes,
 * low byte first. DC is increased by four for every value.
 */
void parse_dw_directive(char *line, unsigned char *data_image, int *DC) {
    char *token;
    char *args = strstr(line, ".dw");
    
    if (args == NULL) return;
    args += 3; /* Skip .dw */
    
    token = strtok(args, " ,\t\n");
    while (token != NULL) {
        long value = atol(token);
        
        /* Store the value one byte at a time. */
        data_image[*DC] = (unsigned char)(value & 0xFF);
        data_image[*DC + 1] = (unsigned char)((value >> 8) & 0xFF);
        data_image[*DC + 2] = (unsigned char)((value >> 16) & 0xFF);
        data_image[*DC + 3] = (unsigned char)((value >> 24) & 0xFF);
        
        (*DC) += 4;
        token = strtok(NULL, " ,\t\n");
    }
}

/*
 * Check for a single quoted string after .asciz, then copy its characters and
 * add a null byte to the data image. Only spaces, tabs, and a line ending may
 * follow the closing quote. Return TRUE when the line is valid and FALSE
 * after printing an error; invalid input does not change DC or the image.
 */
int parse_asciz_directive(char *line, unsigned char *data_image, int *DC) {
    char *directive;
    char *start_quote;
    char *end_quote;
    char *curr;

    directive = strstr(line, ".asciz");
    if (directive == NULL) {
        printf("Error: Invalid .asciz directive format. Expected .asciz \"string\".\n");
        return FALSE;
    }

    start_quote = directive + strlen(".asciz");
    while (*start_quote == ' ' || *start_quote == '\t') {
        start_quote++;
    }

    if (*start_quote != '"') {
        printf("Error: Invalid .asciz directive format. Expected .asciz \"string\".\n");
        return FALSE;
    }

    start_quote++;
    end_quote = strchr(start_quote, '"');
    if (end_quote == NULL) {
        printf("Error: Invalid .asciz directive format. Missing closing quote.\n");
        return FALSE;
    }

    curr = end_quote + 1;
    while (*curr == ' ' || *curr == '\t' || *curr == '\n' || *curr == '\r') {
        curr++;
    }
    if (*curr != '\0') {
        printf("Error: Invalid .asciz directive format. Unexpected text after string.\n");
        return FALSE;
    }
    
    /* Copy the string characters. */
    for (curr = start_quote; curr < end_quote; curr++) {
        data_image[*DC] = (unsigned char)(*curr);
        (*DC) += 1;
    }
    
    /* Add the string terminator. */
    data_image[*DC] = '\0';
    (*DC) += 1;

    return TRUE;
}
