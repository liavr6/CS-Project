#pragma once
char** split(char* mainstring, int* size_top_arr, char del);
void remove_char(char *str, char target);
void write_file(char *filename, char *strtowrite);
int read_file(FILE *fpointer, char chmod, char filename[]);

typedef struct s_label {
	char* name;
	int pc;
} label;

label** AddLabelToArray(label** labelarray, int array_size, char* labelname, int pc);
label* create_label();
void destry_label(label* x);