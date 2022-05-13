#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Main.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
	
	
	
	unsigned int hwreg[IOREGS] = { 0 };//////////////////////////////////////////////


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
			//updating interuption station
			irqstat = ((ioregisters[0] & ioregisters[3]) | (ioregisters[1] & ioregisters[4]) | (ioregisters[2] & ioregisters[5]));
			
			//////////////switch comes here!!!!//////////

			updatepc("R", "sw", cycles);//update cycles count
			//ioregisters[8]++;//update current num of clock cycles


			// Logs and end of process stage
			LedLog(cycles, argv[LEDS]);
			sevensegmentLog(cycles,argv[DISPLAY7SEG]);
			MonitorManager();

			// check irq
			irqhandler(&pc, cycles);//complete!!!!!!!!!!!!!!!!!!!

		} while (pc != -1);

		shutdownmethods(argv, cycles);
}	

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
void updatepc(char type, char* cmd, int* cycles)
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
void MonitorManager()
{

}
void irqhandler(int pc, int *cycles)
{

}
void shutdownmethods(char* argv[], unsigned long long cycles)
{
	CyclesLog(cycles, argv[CYCLES]);
}
