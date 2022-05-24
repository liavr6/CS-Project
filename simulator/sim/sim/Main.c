#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "monitor.c"

int main(int argc, char *argv[])
{
	// Test to validate of file num from input command.
	if (argc != 14) {
		printf("Error: incorrect number of input files - 14 required\nExiting.");
		exit(1);
	}
	// initialize components
	int registers[REGS] = { 0 };
	int ioregisters[IOREGS] = { 0 };
	int pc = 0; int irqstat = 0;
	int* cycles = 0;
	
	
	
	//unsigned int hwreg[IOREGS] = { 0 };//////////////////////////////////////////////


	unsigned int oldsegval = 0;
	unsigned int oldledstate = 0;


#pragma region shit
//#pragma region fpRegion
//	//file pointers definition
//	FILE *file_imemin = NULL, *file_dmemin = NULL, *file_diskin = NULL, *file_irq2in = NULL, *file_dmemout = NULL, *file_regout = NULL,
//		*file_trace = NULL, *file_hwregtrace = NULL, *file_cycles = NULL, *file_leds = NULL, *file_display7seg = NULL,
//		*file_diskout = NULL, *file_monitor_txt = NULL, *file_monitor_yuv = NULL;
//#pragma endregion
	////read the input files
	//	read_file(file_hwregtrace, argv[HWREGTRACE], HWREGTRACEC);
	//	read_file(file_trace, argv[CYCLES], CYCLESC);
	//	read_file(file_leds, argv[LEDS], LEDSC);
	//	read_file(file_cycles, argv[CYCLESC], CYCLES);
	//	read_file(file_dmemout, argv[DMEMOUT], DMEMOUTC);
	//	read_file(file_regout, argv[REGOUTC], REGOUT);
	//	read_file(file_display7seg, argv[DISPLAY7SEG], DISPLAY7SEGC);
	//	read_file(file_diskout, argv[DISKOUTC], DISKOUT);
	//	read_file(file_monitor_txt, argv[MONITORC], MONITOR);
	//	read_file(file_monitor_yuv, argv[MONITOR_YUV], MONITOR_YUVC);
	//	read_file(file_irq2in, argv[IRQ2INC], IRQ2IN);
	//	read_file(file_diskin, argv[DISKIN], DISKINC);
	//	read_file(file_dmemin, argv[DMEMIN], DMEMINC);
	//	read_file(file_imemin, argv[IMEMIN], IMEMINC);
	//// close input files
	//	fclose(file_hwregtrace);
	//	fclose(file_trace);
	//	fclose(file_leds);
	//	fclose(file_cycles);
	//	fclose(file_dmemout);
	//	fclose(file_regout);
	//	fclose(file_display7seg);
	//	fclose(file_diskout);
	//	fclose(file_monitor_txt);
	//	fclose(file_monitor_yuv);
	//	fclose(file_irq2in);
	//	fclose(file_diskin);
	//	fclose(file_dmemin);
	//	fclose(file_imemin);
#pragma endregion
		//main work loop
		do
		{

			//////////////switch comes here!!!!//////////

			updatecyc("R", "sw", cycles);//update cycles count
			//ioregisters[8]++;//update current num of clock cycles


			// Logs and end of process stage
			LedLog(cycles, argv[LEDS]);
			sevensegmentLog(cycles,argv[DISPLAY7SEG]);
			triggermon();

			// check irq
			irqhandler(&pc, cycles);//complete!!!!!!!!!!!!!!!!!!!

		} while (pc != -1);

		shutdownmethods(argv, cycles);
}	
int interuptflag = 0;
//function to read input files
FILE* read_file(char filename[], char chmod)
{
	static FILE *fpointer;
	//fpointer = fopen(filename, "r");

	fpointer = fopen(filename, chmod);
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
	// write to text file
	fprintf(fp, strtowrite);
	// close file
	fclose(fp);
}
char* substr(const char *src, int strt, int end)
{
	// get length destination string
	int len = end - strt;
	// allocate len + 1 chars for dst
	char *dst = (char*)malloc(sizeof(char) * (len + 1));
	strncpy(dst, (src + strt), len);
	return dst;
}
void updatecyc(char type, char* cmd, int* cycles)
{
	if (type == "R")
	{
		*cycles += 1;
	}
	if (type == "I")
	{
		*cycles += 2;
	}
	if (cmd == "lw" || cmd == "sw")
	{
		*cycles += 1;
	}
}
void LedLog(unsigned long long cycle, char *filename)
{
	unsigned int curledstate = ioregisters[9];
	if (curledstate != oldsegval)
	{
		char* line = (char*)malloc(sizeof(char) * 500);///////change 500?!!!!!!
		if (line == NULL) {
			exit(1);
		}
		sprintf(line, "%d %08x", cycle, curledstate);
		write_file(filename, line);
		oldledstate = curledstate;
		free(line);
	}
}
void CyclesLog(unsigned long long cycle, char *filename)
{
	char line[100];
		printf(line, "%d", cycle);
		write_file(filename, line);
		//free(line);
}
void sevensegmenttoLog(unsigned long long cycle, char *filename)
{
	unsigned int cursegval = ioregisters[10];
	if (cursegval != oldsegval)
	{
		char* line = (char*)malloc(sizeof(char) * 500);///////change 500?!!!!!!
		if (line == NULL) {
			exit(1);
		}
		sprintf(line, "%d %08x", cycle, cursegval);
		write_file(filename, line);
		oldsegval = cursegval;
		free(line);
	}
}
void writeval2mon()
{
	unsigned char val = ioregisters[MONITORDATA];
	unsigned int ofst = ioregisters[MONITORADDR];
	//int col = ofst%MON_SIZE;
	//int row = ofst / MON_SIZE;

	changecell(ofst/MON_SIZE, ofst%MON_SIZE, val);
}
void triggermon()
{
	//check if need to write val to mon
	unsigned int boolwritemon = ioregisters[MONITORCMD];//// check again unsigned!!!!!!!!!
	if (boolwritemon)
	{
		writeval2mon();
		ioregisters[MONITORCMD] = 0;//////is it ok in the if?????????????????
	}

//
//	char screen[SCREEN_SIZE][SCREEN_SIZE];
//
//	void initializeScreen() {
//		// Set all pixels to 0.
//		for (int i = 0; i < SCREEN_SIZE; i++) {
//			for (int j = 0; j < SCREEN_SIZE; j++) {
//				screen[i][j] = 0;
//			}
//		}
//	}
//
//	void updatePixel(int row, int col, char color) {
//		screen[row][col] = color;
//	}
//
//	void DumpMonitorFiles(char* txtFileName, char* yuvFileName) {
//		FILE* monTxt;
//		FILE* monYuv;
//
//		monYuv = fopen(yuvFileName, "wb+");
//		monTxt = fopen(txtFileName, "w+");
//
//		if (monYuv == NULL || monTxt == NULL) {
//			printf("[ERROR] Failed to open monitor output files for writing.\nExiting.");
//			exit(-1);
//		}
//
//		for (int row = 0; row < SCREEN_SIZE; row++)
//		{
//			for (int col = 0; col < SCREEN_SIZE; col++)
//			{
//				// For monitor.txt, write every pixel to a separate line.
//				fprintf(monTxt, "%02x\n", 0xFF & screen[row][col]);
//				// For monitor.yuv, write every pixel to a binary
//				char byte_to_write = 0xFF & screen[row][col];
//				fwrite(&byte_to_write, sizeof(byte_to_write), 1, monYuv);
//			}
//		}
//
//		// Close files.
//		fclose(monTxt);
//		fclose(monYuv);
//	}
//	void HandleMonitor() {
//		// If we need to write to monitor, do it now.
//		unsigned int shouldWriteToMonitor = hw_reg[MONITORCMD];
//
//		if (shouldWriteToMonitor) {
//			WriteToMonitor();
//		}
//		hw_reg[MONITORCMD] = 0;
//	}
//
//	void WriteToMonitor() {
//		unsigned char color = hw_reg[MONITORDATA];
//		unsigned int offset = hw_reg[MONITOROFFSET];
//
//		int row = offset / SCREEN_SIZE;
//		int col = offset % SCREEN_SIZE;
//
//		updatePixel(row, col, color);
//	}

}
void shutdownmethods(char* argv[], unsigned long long cycles)
{
	CyclesLog(cycles, argv[CYCLES]);
}
void triggertimer() 
{
	//check if timer enabled
	if (ioregisters[TIMERENB] == 1) 
	{
		//check if val is valid
		if (ioregisters[TIMERCURR] < ioregisters[TIMERMAX]) 
		{
			ioregisters[TIMERCURR]++;
		}
		else
		{
			ioregisters[IRQ0STS] = 1;
			ioregisters[TIMERCURR] = 0;
		}
	}
}

int* irq2arr;
int irq2arrlen;
//void set_irq2_arr(char* file_path)
//{
//	FILE* fp;
//	char* line = NULL;
//	size_t len = 0;
//	size_t read;
//	irq2arr = (int*)malloc(sizeof(int));
//	irq2arrlen = 0;
//	fp = fopen(file_path, "r");
//	if (fp == NULL)
//	{
//		exit(1);
//	}
//
//	while ((read = getline(&line, &len, fp)) != -1)
//	{
//		printf("Retrieved line of length %zu:\n", read);
//		printf("%s", line);
//		irq2arr = (int*)realloc(irq2arr,(irq2arrlen + 1)*sizeof(int));
//		int val = strtoul(line, NULL, 0);
//		irq2arr[irq2arrlen] = val;
//		irq2arrlen++;
//	}
//
//	fclose(fp);
//	if (line)
//	{
//		free(line);
//	}
//}
void set_irq2_arr(char* file_path)
{
	FILE* fp;
	fopen(&fp, file_path, "r");    //fopen_s!!!!!!********************************************

	size_t line_size = 255;
	char* line = malloc(line_size);
	irq2arr = (int*)malloc(sizeof(int));
	irq2arrlen = 0;
	//check if exists
	if (fp == NULL) {
		exit(1);
	}
	//read lines
	while (fgets(line, line_size, fp) != NULL)
	{
		irq2arr = (int*)realloc(irq2arr, (irq2arrlen + 1) * sizeof(int));
		int val = strtoul(line, NULL, 0);
		irq2arr[irq2arrlen] = val;
		irq2arrlen++;
	}
	free(line);
}
void check_irq2arr(unsigned long long cycles)
{
	for (size_t i = 0; i < irq2arrlen; i++)
	{
		if (irq2arr[i] == cycles)
		{
			ioregisters[IRQ2STS] = 1;
			break;
		}
	}
}
void irqhandler(int pc, int *cycles)
{
///////////////////////////////////////////remember to add to retri!!!!*****************
	if (interuptflag == 1)
	{
		return;
	}
	triggertimer();
	check_irq2arr(cycles);
	//updating interuption station
	irqstat = ((ioregisters[0] & ioregisters[3]) | (ioregisters[1] & ioregisters[4]) | (ioregisters[2] & ioregisters[5]));
	if (irqstat)
	{
		interuptflag = 1;
		ioregisters[IRQRETURN] = pc;
		pc = 0xFFF&ioregisters[IRQHANDLER]; //12 bits so we don't overflow.
		////////////////////////////////////////////////////////////////////////////////check FFF!!!!!!!!!!!!
	}
}
