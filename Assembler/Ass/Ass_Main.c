#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Ass_Main.h"
#include <stdlib.h>

label* create_label()
{
	label* result = NULL;
	
	result = (label*)malloc(sizeof(*result));
	if (NULL == result)
	{
		return NULL;
	}

	result->name = NULL;
	result->pc = 0;

	return result;
}

void destry_label(label* x)
{
	if (x != NULL)
	{
		free(x);
	}
}


int main(int argc, char *argv[])
{
#pragma region poc

	// Test to validate of file num from input command.
	/*if (argc != 4) {
		printf("Error: incorrect number of input files - 4 required\nExiting.");
		exit(1);
	}*/

	/*

	char a[] = "	add $t2, $zero, $imm, 1				# $t2 = 1";
	char test_label[] = "fib:allaakbar";
	char *copy_this = test_label;

	size_t size2 = strlen(copy_this);

	//find label poc
	char* chartest = (char*)malloc(sizeof(*chartest)*(size2 + 1));
	strcpy(chartest, copy_this);

	int pc = 0;	
	remove_char(test_label, ':');

	label* test = (label*)malloc(sizeof(label));
	memset(test, 0, sizeof*(test));

	test->name = chartest;
	test->pc = pc;

	label** testa = (label**)malloc(sizeof(label*));
	if (testa == NULL)
	{
		exit(1);
	}
	testa = malloc(0);
	for (int i = 0; i < 10; i++)
	{
		testa = AddLabelToArray(testa, i, test_label, i);
	}

	//read file poc
	FILE* fp = NULL;
	char rfname[] = "..\\fib.asm";//relative path of file to read
	fp = read_file(rfname,"r");

	char line[500];
	char fname[] = "test1.txt";
	fclose(fopen(fname, "w+"));
	while (fgets(line, sizeof(line), fp)) {
		printf("%s", line);
		//write to file poc
		write_file(fname, line);
	}
	fclose(fp);

	//split poc
	int size;
	char** splits1 = split(a, &size, ',');
	for (int i = 0; i < size; i++)
	{
		printf("%s ",splits1[i]);
	}
	//dont forget to free the splits later

	*/
#pragma endregion
		// Test to validate of file num from input command.
	if (argc != 4) {
		printf("Error: incorrect number of input files - 4 required\nExiting.");
		exit(1);
	}
	char rfname[] = { argv[1] };
	label** labelsarray = (label**)malloc(sizeof(label*));
	if (labelsarray == NULL){exit(1);}
	labelsarray = malloc(0);
	find_labels(rfname, labelsarray);
	create_memin(opcodes[22], registers[16], rfname);
	free_array(labelsarray);


	/*1.read_file(asm) v
	2.find_labels(file) v
	3.create_memin(file,labels) v
	4.free memory v */
}

#pragma region shit

//removes unwanted chars from the strin
void remove_char(char *str, char target)
{
	char *src, *dst;
	for (src = dst = str; *src != '\0'; src++)
	{
		*dst = *src;
		if (*dst != target)
		{
			dst++;
		}
	}
	*dst = '\0';
}

//split the command line to its parts
char** split(char* mainstring, int* size_top_arr, char del)
{
	//string to substrings allocations
	(*size_top_arr) = 1;
	char** splits;
	splits = (char**)malloc(sizeof(char*) * (*size_top_arr));
	// seperation of first token
	char* symb = strtok(mainstring, " \t");
	// seperation of other token
	while (symb != NULL && symb[0] != '#')
	{
		remove_char(symb, del);
		splits[(*size_top_arr) - 1] = symb;
		(*size_top_arr)++;
		splits = (char**)realloc(splits, sizeof(char*) * (*size_top_arr));
		symb = strtok(NULL, " \t");
	}
	(*size_top_arr)--;
	return splits;
}

//function to read input files
FILE* read_file(char filename[], char chmod)
{
	static FILE *fpointer;
	fpointer = fopen(filename, "r");
	if (fpointer == NULL)
	{
		printf("Error: reading file %s was problematic.", filename);
		return NULL;
	}
	return fpointer;
}

//function to write output files
void write_file(char *filename, char *strtowrite)
{
	// open file for writing
	FILE *fp = fopen(filename, "a+");
	//if (fp == NULL)
	//{
	//	fp = fopen(filename, "wb");
	//	//printf("Error opening file %s", filename);
	//	//return 1;
	//}
	// write to text file
	fprintf(fp, strtowrite);

	// close file
	fclose(fp);
}

//adds a new label structs to the existing array of labels
label** AddLabelToArray(label** labelarray, int array_size, char* labelname, int pc) {
	// create a new label struct
	label* nlabel = (label*)malloc(sizeof(label));
	nlabel->name = (char*)malloc(sizeof(labelname));
	nlabel->pc = pc;
	strcpy(nlabel->name, labelname);
	labelarray = (label**)realloc(labelarray, sizeof(label*)*(array_size+1));
	labelarray[array_size] = nlabel;
	return labelarray;
}

//func to find labels in the text file
void find_labels(char* file_name, label** labels)
{
	FILE *fp1;
	int line_counter = 0;
	int len = 0, a;
	int labels_count = 0;
	char line[500];
	fp1 = fopen(file_name, "r");
	if (fp1 == NULL) {
		printf("kaki");
		return;
	}
	while (fgets(line, 100, fp1) != NULL)
	{
		line_counter++;
		char **temp_line = split(line, &a,',');
		for (int i = 0; i < a; i++)
		{
			if (strpbrk(temp_line[i], ":") != NULL) // checks if the string has ":" in it.
			{
				remove_char(temp_line[i], ':');
				strcpy(labels[labels_count]->name, temp_line[i]);
				labels[labels_count]->pc = line_counter;
				labels_count++;
			}
		}
	}
}

//func to find out if a lable exists in the label array
int is_str_in_array(char* array[], char* str, int length) //return the opcode/register value of str if it's indeed an opcode/register, -1 otherwise.
{ // array is always the opcodes/registers array from main function.
	for (int i = 0; i < length; i++)
	{
		if (strcmp(array[i], str) == 0) {
			return i;
		}
	}
	return -1;
}

//func to create data for the final output file and writes it to the file
void create_memin(char* opcodes[22], char* registers[16], char* file_name)
{
	int size;
	char line[500], memin_line[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };//initialize again in the while******
	char temp[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };//initialize again in the while******
	FILE *fp1;
	fp1 = fopen(file_name, "r");
	if (fp1 == NULL) {
		printf("kaki");
		return;
	}
	strcat(memin_line, "0");//remove and add formating to length of 8 letters***********************
	while (fgets(line, 100, fp1) != NULL)
	{
		char** splited_line = split(line, &size,',');
		if (is_str_in_array(opcodes, splited_line[0], 22) != -1)
		{
			strcpy(temp, _itoa(is_str_in_array(opcodes, splited_line[0], 22), temp, 10)); // copies the decimal value of opcode to temp
			strcat(memin_line, temp);													  // concatanate the register number to memin_line
			printf("%s\n", memin_line);
		}
		for (int i = 1; i < size; i++)
		{
			if (is_str_in_array(registers, splited_line[i], 16) != -1)
			{
				strcpy(temp, _itoa(is_str_in_array(registers, splited_line[i], 16), temp, 10)); // copies the decimal value of register to temp
				strcat(memin_line, temp);														// concatanate the register number to memin_line
				printf("%s\n", memin_line);
			}
		}
		/*1.format the second line of the input to 8 letters
		2.initialize the arrays in the while
		3.remove cat line 236
		4.add func part to address labels and add their pc in the text
		5.adapt to work with new code: read file and write file*/


		// write memin_line to file.
		// initialize memin_line,temp at the end of a while iteration.
	}
}

void free_array(label** labels)
{
	size_t n = sizeof(labels) / sizeof(labels[0]);
	for (int i = 0; i < n; i++) {
		free(labels[i]->name);
		free(labels[i]);
	}
	free(labels);
}
#pragma endregion