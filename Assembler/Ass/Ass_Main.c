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
	// Test to validate of file num from input command.
	/*if (argc != 4) {
		printf("Error: incorrect number of input files - 4 required\nExiting.");
		exit(1);
	}*/
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
}

#pragma region shit

//needed in order to remove commas
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

//think about changing inputs
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
#pragma endregion