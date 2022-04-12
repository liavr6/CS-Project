#pragma once
char** split(char* mainstring, int* size_top_arr);
void remove_char(char *str, char target);
typedef struct s_label {char* name;int pc;} label;