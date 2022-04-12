#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Main.h"

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

	#pragma region fpRegion
		//file pointers definition
		FILE *file_imemin = NULL, *file_dmemin = NULL, *file_diskin = NULL, *file_irq2in = NULL, *file_dmemout = NULL, *file_regout = NULL, 
			*file_trace = NULL,		*file_hwregtrace = NULL, *file_cycles = NULL, *file_leds = NULL, *file_display7seg = NULL, 
			*file_diskout = NULL, *file_monitor_txt = NULL, *file_monitor_yuv = NULL;
	#pragma endregion

	//read the input files
		read_file(file_hwregtrace, argv[HWREGTRACE], HWREGTRACEC);
		read_file(file_trace, argv[CYCLES], CYCLESC);
		read_file(file_leds, argv[LEDS], LEDSC);
		read_file(file_cycles, argv[CYCLESC], CYCLES);
		read_file(file_dmemout, argv[DMEMOUT], DMEMOUTC);
		read_file(file_regout, argv[REGOUTC], REGOUT);
		read_file(file_display7seg, argv[DISPLAY7SEG], DISPLAY7SEGC);
		read_file(file_diskout, argv[DISKOUTC], DISKOUT);
		read_file(file_monitor_txt, argv[MONITORC], MONITOR);
		read_file(file_monitor_yuv, argv[MONITOR_YUV], MONITOR_YUVC);
		read_file(file_irq2in, argv[IRQ2INC], IRQ2IN);
		read_file(file_diskin, argv[DISKIN], DISKINC);
		read_file(file_dmemin, argv[DMEMIN], DMEMINC);
		read_file(file_imemin, argv[IMEMIN], IMEMINC);
	// close input files
		fclose(file_hwregtrace);
		fclose(file_trace);
		fclose(file_leds);
		fclose(file_cycles);
		fclose(file_dmemout);
		fclose(file_regout);
		fclose(file_display7seg);
		fclose(file_diskout);
		fclose(file_monitor_txt);
		fclose(file_monitor_yuv);
		fclose(file_irq2in);
		fclose(file_diskin);
		fclose(file_dmemin);
		fclose(file_imemin);
		//main work loop
		do
		{
			//updating interuption station
			irqstat = ((ioregisters[0] & ioregisters[3]) | (ioregisters[1] & ioregisters[4]) | (ioregisters[2] & ioregisters[5]));
			ioregisters[8]++;//update current num of clock cycles
		} while (pc != -1);
}	

//function to read input files
int read_file(FILE *fpointer,  char chmod, char filename[])
{
	fpointer = fopen(filename, chmod);
	if (fpointer == NULL)
	{
		printf("Error: reading file %s was problematic.",filename);
		return 1;
	}
}
