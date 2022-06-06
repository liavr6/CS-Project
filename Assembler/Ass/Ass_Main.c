#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Ass_Main.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

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
	argc = 3;
	argv[0] = "program.exe";
	argv[1] = "..\\TestThis.asm";
	argv[2]="memin.txt";
	if (argc != 3) {
		printf("Error: incorrect number of input files - 3 required\nExiting.");
		exit(1);
	}

	label** labelsarray = (label**)malloc(0);
	if (labelsarray == NULL) { exit(1); }
	int labelnum = find_labels(argv[1], &labelsarray);

	
	create_memin(opcodes, registers, argv[1],argv[2], labelsarray, labelnum);

	free_array(labelsarray, labelnum);


}

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

	fpointer = fopen(filename, chmod);
	if (fpointer == NULL)
	{
		printf("Error: reading file %s was problematic.", filename);
		return NULL;
	}
	return fpointer;
}

//function to write output files
void write_file(FILE *fp, char *strtowrite)
{

	// write to text file
	fprintf(fp, "%s\n", strtowrite);

	// close file
}

//adds a new label structs to the existing array of labels
label** AddLabelToArray(label** labelarray, int array_size, char* labelname, int pc) {
	// create a new label struct
	label* nlabel = (label*)malloc(sizeof(label));
	nlabel->pc = pc;
	nlabel->name = (char*)malloc(strlen(labelname)+1);
	strcpy(nlabel->name, labelname);
	labelarray = (label**)realloc(labelarray, sizeof(label*)*(array_size+1));
	labelarray[array_size] = nlabel;
	return labelarray;
}

//adds a new word structs to the existing array of words
word** AddWordToArray(word** wordarr, int array_size, int place, int val) {
	// create a new word struct
	word* nword = (word*)malloc(sizeof(word));
	nword->place = place;
	nword->val = val;

	wordarr = (word**)realloc(wordarr, sizeof(label*)*(array_size + 1));
	wordarr[array_size] = nword;
	return wordarr;
}
//func to find labels in the text file
int find_labels(char* file_name, label*** labels)
{
	FILE *fp1;
	int line_counter = 0;
	int len = 0, a;
	int labels_count = 0;
	char line[1024];
	fp1 = fopen(file_name, "r");

	if (fp1 == NULL) {
		printf("Error");
		return;
	}
	while (fgets(line, 100, fp1) != NULL)
	{
		//line_counter++;
		char **temp_line = split(line, &a,',');
		for (int i = 0; i < a; i++)
		{
			//counts the curr pc in regard with r/i format

			if (strcmp(temp_line[i], "$imm") == 0)
			{
				line_counter += 2;
				break;
			}
			if (strpbrk(temp_line[0], ":") == NULL && (i == (a - 1)))
			{
				line_counter += 1;
			}
			if (strpbrk(temp_line[i], ":") != NULL) // checks if the string has ":" in it.
			{
				remove_char(temp_line[i], ':');
				remove_char(temp_line[i], '\n');
				*labels = AddLabelToArray(*labels, labels_count, temp_line[i], line_counter);
				labels_count++;
			}

		}
	}
	return labels_count;
}

//func to find out if a opcode exists in the opcode array
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

//func to find out if a lable exists in the label array
int is_lbl_in_array(label** labels, char* str, int length) //return the index value of str if it's indeed an label, -1 otherwise.
{
	for (int i = 0; i < length; i++)
	{
		if (strcmp(labels[i]->name, str) == 0) {
			return i;
		}
	}
	return -1;
}

//func to create data for the final output file and writes it to the file
void create_memin(char* opcodes[22], char* registers[16], char* in_file_name, char* out_file_name, label** labels,int labelnum)
{
	int size;
	char line[500] = { "0" };
	char memin_line[1000] = { NULL };
	char temp[1000] = { NULL };
	char* imm;
	int writtenlines = 0;
	FILE *fp1;
	fp1 = fopen(in_file_name, "r");
	if (fp1 == NULL) {
		printf("Error");
		return;
	}

	FILE *fp2 = fopen(out_file_name, "w");
	if (fp2 == NULL) {
		fclose(fp1);
		printf("Error");
		return;
	}
	int word = 0;
	char str[6] = "00000";
	strcat(memin_line, str);
	char** splited_line = split(line, &size, ',');
	bool alpha = false;
	int labelindex = 0;
	static char hexVal[3];
	static char nhexVal[5];

	int resultop=0;
	int isresr = -1;
	int resultreg = 0;
	char resultopstr[1000] = "";
	bool terf = false;
	label** wordsa = (label**)malloc(0);
	if (wordsa == NULL) { exit(1); }
	//main loop
	while (fgets(line, 100, fp1) != NULL)
	{
		strcpy(temp, "00000");
		splited_line = split(line, &size, ',');
		terf = false;
		isresr = -1;
			terf = strstr(line, ".word");
			if (terf)//checks if word
			{
				fflush(fp2);
				int left, right;
				if (strstr(splited_line[1], "0x"))
				{
					left = strtol((cleanhex(splited_line[1])), NULL, 16);
				}
				else
				{
					left = atoi(splited_line[1]);
				}
				if (strstr(splited_line[2], "0x"))
				{
					right = strtol((cleanhex(splited_line[2])), NULL, 16);
				}
				else
				{
					right = atoi(splited_line[2]); 
				}
				wordhandle(out_file_name, left, right);
					word++;
				
			}
			if (!terf)//if not word
			{
				if (size > 1)
				{
					resultop = is_str_in_array(opcodes, splited_line[0], 22); 
					isresr = is_str_in_array(Rop, splited_line[0], 12);
					if (resultop == 18)
					{
						bool a = true;
					}
					if (resultop != -1)//valid
					{
						sprintf(resultopstr, "%d", resultop);
						sprintf(hexVal, "%02X", resultop);
						strcpy(temp, hexVal); // copies the decimal value of opcode to temp
						 // copies the decimal value of opcode to temp
						strcpy(memin_line, temp); // concatanate the register number to memin_line
					}
					for (int i = 1; i < (size - 1); i++)
					{
						if (is_str_in_array(registers, splited_line[i], 16) != -1)
						{
							resultreg = is_str_in_array(registers, splited_line[i], 16);
							strcpy(temp, _itoa(resultreg, temp, 10)); // copies the decimal value of register to temp
							sprintf(hexVal, "%01X", resultreg);
							strcpy(temp, hexVal);
							strcat(memin_line, temp);	// concatanate the register number to memin_line
						}
					}

					if (!strstr(line, ".word"))
					{
						write_file(fp2, memin_line);//added writing to file - check if can avoid opening file over and over
					}
					writtenlines++;
					char *ret;
					ret = strstr(splited_line[size - 1], "0x");
					for (int ind = 0; ind < strlen(splited_line[size - 1]); ind++)
					{
						if (isalpha(splited_line[size - 1][ind])&&!ret)
						{
							alpha = true;
							break;
						}
					}
					if (!alpha)
					{
				
						if (ret)
						{
							imm = cleanhex(splited_line[size - 1]);
						}
						else
						{
							imm = tohex(atoi(splited_line[size - 1]));
						}
					
						for (int i = 0; i < size; i++)
						{
							if (isresr != -1)
							{
								if (i == 1)
								{
									continue;//r type
								}
							}
							if (strcmp(splited_line[i], "$imm") == 0)
							{
								strcpy(memin_line, "");//i type
								strcpy(memin_line, imm);

								if (!strstr(line, ".word"))
								{
									write_file(fp2, memin_line);//added writing to file - check if can avoid opening file over and over
								}
								writtenlines++;
								break;
							}
						}
					}
					if (alpha)//theres a label at the final cell
					{
						if (labels[0] != NULL)
						{
							labelindex = is_lbl_in_array(labels, splited_line[size - 1], labelnum);
							if (labelindex != -1)
							{
								for (int i = 0; i < size; i++)
								{
									if (isresr != -1)
									{
										if (i == 1)
										{
											continue;//r type
										}
									}
									if (strcmp(splited_line[i], "$imm") == 0)
									{
										strcpy(memin_line, "");//i type
										break;
									}
								}
								imm = tohex(labels[labelindex]->pc);
								strcat(memin_line, imm);


								if (!strstr(line, ".word"))
								{
									write_file(fp2, memin_line);//added writing to file - check if can avoid opening file over and over
								}
								writtenlines++;
							}
							else
								return -1;
						}
						else
							return -1;
					}
					alpha = false;

				}
			}
	}

	fclose(fp2);
	fclose(fp1);

}


char* substr(const char *src, int strt, int end)
{
	// get length destination string
	int len = end - strt;
	// allocate len + 1 chars for dst
	char *dst = (char*)malloc(sizeof(char) * (len + 1));
	strncpy(dst, (src + strt), len);
	dst += '\0';
	return dst;
}
//turns num to hex - assuming it isnt bigger than 5digit output
char* tohex(int num)
{
	static char hexVal[9];
	static char shorthexVal[6];
	if (num < 0) //deal if num is negatived
	{
		sprintf(hexVal, "%08X", num);
		strncpy(shorthexVal, hexVal + 3, 8);
		return shorthexVal;
	}
	else//if positive
	{
		sprintf(hexVal, "%05X", num);
		return hexVal;
	}
}
int wordhandle(char* file_name, int place, int val)
{
	//manages the placement of wanted word in place
	FILE *file = fopen(file_name, "r+");

	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
	int number_of_lines = file_size / 7;

	while (number_of_lines < place)
	{
		fseek(file, 0, SEEK_END);
		fprintf(file, "%05X\n", 0);
		number_of_lines += 1;
	}

	if (place <= number_of_lines)
	{
		fseek(file, place * 7, SEEK_SET);
		fprintf(file,"%05X\n", val);
	}

	fclose(file);
	return val;
}

void free_array(label** labels, int labelnum)
{
	for (int i = 0; i < labelnum; i++) {
		free(labels[i]->name);
		free(labels[i]);
	}
	free(labels);
}

char* cleanhex(char* num)
{
	//turns hex to hex in wanted format

	int targetStrLen = 5;           //output length  
	const char *padding = "00000";
	
	size_t n = sizeof(num) / sizeof(num[0]);
	char *finalhexVal = (char*)malloc(sizeof(char) * (6));
	char cleanhexVal[8];
	memcpy(cleanhexVal, &num[2], n - 1);
	touppers(cleanhexVal);
	int padLen = targetStrLen - strlen(cleanhexVal); //Padding length
	if (padLen < 0)
	{
		padLen = 0;
	}
	sprintf(finalhexVal,"%*.*s%s", padLen, padLen, padding, cleanhexVal);  // Left Padding
	finalhexVal[5] = '\0';
	return finalhexVal;
}
void touppers(char *str)
{
	//converts strings to uppercase
	int i = 0;

	while (str[i])
	{
		if (isalpha(str[i]))
		{
			str[i] = toupper(str[i]);
		}
		i++;
	}

}
