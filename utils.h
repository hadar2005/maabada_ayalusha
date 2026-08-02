#ifndef UTILS_H
#define UTILS_H

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
