#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MON_SIZE 256

void initMon();
void changecell(int row, int col, char val);
void writeLogMon(char* y_file, char* data_file);

