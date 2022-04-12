#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Ass_Main.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
	// Test to validate of file num from input command.
	/*if (argc != 4) {
		printf("Error: incorrect number of input files - 4 required\nExiting.");
		exit(1);
	}*/
	char a[] = "	add $t2, $zero, $imm, 1				# $t2 = 1";
	char test_label[] = "fib:";

	//find label poc
	int pc = 0;
	label* test = (label*)malloc(sizeof(label));
	char* chartest = (char*)malloc(sizeof(char)*500);
	remove_char(test_label, ':');
	test->pc = pc;
	strcpy(chartest, test_label);//i don't like the implementation of test->name [255]
	test->name = chartest;
	
	
	//split poc
	int size;
	char** splits1 = split(a, &size, ',');
	for (int i = 0; i < size; i++)
	{
		printf("%s ",splits1[i]);
	}
	//dont forget to free the splits later
}
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
char** split(char* mainstring, int* size_top_arr,char del)
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
int read_file(FILE *fpointer, char chmod, char filename[])
{
	fpointer = fopen(filename, chmod);
	if (fpointer == NULL)
	{
		printf("Error: reading file %s was problematic.", filename);
		return 1;
	}
}
