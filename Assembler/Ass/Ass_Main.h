#pragma once
char** split(char* mainstring, int* size_top_arr, char del);
void remove_char(char *str, char target);
void write_file(char *filename, char *strtowrite);
FILE* read_file(char filename[], char chmod);
typedef struct s_label {
	char* name;
	int pc;
} label;
char *opcodes[22] = { "add", "sub", "mul", "and", "or", "xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" };
char *registers[16] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
label** AddLabelToArray(label** labelarray, int array_size, char* labelname, int pc);
label* create_label();
void destry_label(label* x);
void find_labels(char* file_name, label** labels);
void create_memin(char* opcodes[22], char* registers[16], char* file_name);
void free_array(label** labels);