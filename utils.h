#ifndef UTILS_H
#define UTILS_H

/*
 * Return a pointer to the first character in line that is not a space or
 * tab. The input must be a null-terminated string; no characters are moved.
 */
char* skip_spaces(char *line);

/*
 * Remove leading and trailing whitespace from str in place and return the
 * resulting start pointer. The caller must provide a writable string buffer.
 */
char* trim_whitespace(char *str);

/*
 * Return the next comma-separated parameter from line_segment after trimming
 * it. The string is tokenized in place and NULL is returned when no parameter
 * remains; therefore line_segment must be writable.
 */
char* get_next_comma_param(char *line_segment);

/*creating a struct that will represent a label,
using typedef so we dont have to write 'struct' every time*/
typedef struct {
  /*max name length is 31 + one char to end the string*/
  char name[32];
  
  int address;
  
  /*using int as a boolean
  0 = false
  1 = true*/
  int is_code;
  int is_data;
  int is_external;
  int is_entry;

} Label;

#endif
