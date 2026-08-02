/*in this file we have all the helping functions that will ease our work*/

char *skip_spaces(char *line) {
  /*gets a pointer to the beginning of a string 
  and returns a pointer to the first non-space part of the string*/
  while (*line == ' ' || *line == '\t') {
    line++; /*increases pointer by one unit - length of char*/
  }
  return line;
}


