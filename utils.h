#ifndef UTILS_H
#define UTILS_H

/*declaring the skip_spaces func*/
char* skip_spaces(char *line);

/*declaring the func that trims whitespaces from both ends*/
char* trim_whitespace(char *str);

/*declaring function to recognize next parameter by comma*/
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
