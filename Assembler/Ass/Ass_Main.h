#pragma once
char** split(char* mainstring, int* size_top_arr, char del);
void remove_char(char *str, char target);
void write_file(FILE *fp, char *strtowrite);
FILE* read_file(char filename[], char chmod);
typedef struct s_label {
	char* name;
	int pc;
} label;
typedef struct s_word {
	int place;
	int val;
} word;
char *Rop[12] = { "add", "sub", "mul", "and", "or", "xor", "sll", "sra", "srl", "jal", "lw", "in" };
int wordhandle(char* file_name, int place,int val);
word** AddWordToArray(word** wordarr, int array_size, int place, int val);
char *opcodes[22] = { "add", "sub", "mul", "and", "or", "xor", "sll", "sra", "srl", "beq", "bne", "blt", "bgt", "ble", "bge", "jal", "lw", "sw", "reti", "in", "out", "halt" };
char *registers[16] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$s0", "$s1", "$s2", "$gp", "$sp", "$ra" };
label** AddLabelToArray(label** labelarray, int array_size, char* labelname, int pc);
label* create_label();
void destry_label(label* x);
int find_labels(char* file_name, label*** labels);
void create_memin(char* opcodes[22], char* registers[16], char* in_file_name, char* out_file_name, label** labels, int labelnum);
void free_array(label** labels, int labelnum);
char* substr(const char *src, int strt, int end);
char* tohex(int num);
int is_lbl_in_array(label** labels, char* str, int length);
char* cleanhex(char* num);
void touppers(char *str);