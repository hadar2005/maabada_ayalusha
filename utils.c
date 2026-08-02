#include <stdio.h>
#include <string.h>
#include "utils.h"
/*in this file we have all the helping functions that will ease our work, reding text etc*/


/*gets a pointer to the beginning of a string 
  and returns a pointer to the first non-space part of the string*/
char *skip_spaces(char *line) {
  while (*line == ' ' || *line == '\t') {
    line++; /*increases pointer by one unit - length of char*/
  }
  return line;
}


/*removes spaces from beginning to end*/
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

/*returns start of next parameter by comma*/
char *get_next_comma_param(char *line_segment) {
  char *curr_param = strtok(line_segment, ",");
  
  if (curr_param == NULL) { /*if we reached the end*/
    return NULL;
  }
  
  return trim_whitespace(curr_param); /*return cleaned current parameter*/
}

