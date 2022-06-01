#define _CRT_SECURE_NO_WARNINGS
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
void changecell(int i, int j, char val)
{
	mon[i][j] = val;
}


void writeLogMon(char* y_file, char* data_file) 
	{
		//open files and check validity
		FILE* mon_data;
		mon_data = fopen(data_file, "w+");
		if (mon_data == NULL)
		{
			exit(-1);
		}
		FILE* mon_y;
		mon_y = fopen(y_file, "wb+");
		if (mon_y == NULL)
		{
		exit(-1);
		}

		for (int i = 0; i < MON_SIZE; i++)
		{
			for (int j = 0; j < MON_SIZE; j++)
		{
			//pixel to binary in monitor.yuv
			char data = mon[i][j] & 0xFF;/////////////////////////////////check FF
			fwrite(&data, sizeof(data), 1, mon_y);
			// each pixel to separate line in monitor.txt
			fprintf(mon_data,"%02X\n",mon[i][j]&0xFF);/////////////////////////////////check FF
			}
		}
		//close 
		fclose(mon_y);
		fclose(mon_data);
	}