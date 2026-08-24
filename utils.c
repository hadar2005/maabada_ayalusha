/*
 * Text-processing utilities used by the assembler. The routines operate on
 * null-terminated strings, and the trimming/tokenizing functions modify the
 * caller's writable buffer rather than allocating a new string.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"
/*
 * Advance through leading spaces and tabs and return the first non-space
 * character. The algorithm changes only the returned pointer and does not
 * modify the input string.
 */
char *skip_spaces(char *line) {
  while (*line == ' ' || *line == '\t') {
    line++; /*increases pointer by one unit - length of char*/
  }
  return line;
}


/*
 * Remove leading and trailing spaces, tabs, and line terminators in place.
 * Return the adjusted start pointer; the caller must provide writable text.
 */
char *trim_whitespace(char *str) {
  char *end; /*pointer to end of string, we will fill it later*/
  
  str = skip_spaces(str); /*trimming start of string*/
  
  if (*str == '\0')/*if the string was only whitespaces*/
    return str;
    
  end = str + strlen(str) - 1; /*pointer to end of str*/
  /*removing all spaces from the end*/
  while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') ) {
    end--;
  }
  
  *(end + 1) = '\0'; /*this terminates the string after cleaning spaces from the end*/
  
  return str;
}

/*
 * Extract the next comma-delimited token with strtok, trim its whitespace,
 * and return it. Return NULL when no token remains; tokenization modifies the
 * supplied buffer and uses strtok's internal traversal state.
 */
char *get_next_comma_param(char *line_segment) {
  char *curr_param = strtok(line_segment, ",");
  
  if (curr_param == NULL) { /*if we reached the end*/
    return NULL;
  }
  
  return trim_whitespace(curr_param); /*return cleaned current parameter*/
}

