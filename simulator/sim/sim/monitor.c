#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "monitor.h"



char mon[MON_SIZE][MON_SIZE];

// intializes all the monitor to 0
void initMon()
{
	for (int i = 0; i < MON_SIZE; i++) 
	{
		for (int j = 0; j < MON_SIZE; j++) 
		{
			mon[i][j] = 0;
		}
	}
}

//changes cell value when triggered
void changecell(int row, int col, char val)
{
	mon[row][col] = val;
}






	//void DumpMonitorFiles(char* txtFileName, char* yuvFileName) 
	//{
	//	FILE* monTxt;
	//	FILE* monYuv;

	//	monYuv = fopen(yuvFileName, "wb+");
	//	monTxt = fopen(txtFileName, "w+");

	//	if (monYuv == NULL || monTxt == NULL) {
	//		printf("[ERROR] Failed to open monitor output files for writing.\nExiting.");
	//		exit(-1);
	//	}

	//	for (int row = 0; row < SCREEN_SIZE; row++)
	//	{
	//		for (int col = 0; col < SCREEN_SIZE; col++)
	//		{
	//			// For monitor.txt, write every pixel to a separate line.
	//			fprintf(monTxt, "%02x\n", 0xFF & screen[row][col]);
	//			// For monitor.yuv, write every pixel to a binary
	//			char byte_to_write = 0xFF & screen[row][col];
	//			fwrite(&byte_to_write, sizeof(byte_to_write), 1, monYuv);
	//		}
	//	}

	//	// Close files.
	//	fclose(monTxt);
	//	fclose(monYuv);
	//}