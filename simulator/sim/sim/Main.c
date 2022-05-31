#define _CRT_SECURE_NO_WARNINGS

#include "Main.h"
#include "monitor.h"


int interuptflag = 0;
int hardrive[SECTOR_COUNT][SECTOR_SIZE] = { 0 };
int rammemory[LINES_MAX_SIZE] = { 0 };
int registers[REGS] = { 0 };
int ioregisters[IOREGS] = { 0 };
int* irq2arr;
int irq2arrlen;

int main(int argc, char *argv[])
{
	argc = 14;
	argv[0] = "program.exe";
	argv[1] = "..\\memin.txt";
	argv[2] = "..\\diskin.txt";
	argv[3] = "..\\irq2in.txt";
	argv[4] = "..\\memout.txt";
	argv[5] = "..\\regout.txt";
	argv[6] = "..\\trace.txt";
	argv[7] = "..\\hwregtrace.txt";
	argv[8] = "..\\cycles.txt";
	argv[9] = "..\\leds.txt";
	argv[10] = "..\\display7seg.txt";
	argv[11] = "..\\diskout.txt";
	argv[12] = "..\\monitor.txt";
	argv[13] = "..\\monitor.yuv";
	// Test to validate of file num from input command.
	if (argc != 14) {
		printf("Error: incorrect number of input files - 14 required\nExiting.");
		exit(1);
	}
	// initialize components

	int pc = 0; 
	int irqstat = 0;
	int* cycles;
	cycles = (int*)malloc(sizeof(int));
	*cycles = 0;
	int diskcyc = 0;
	
	inputdisc(argv[DISKIN]);//read data from harddrive
	set_irq2_arr(argv[IRQ2IN]);
	//unsigned int hwreg[IOREGS] = { 0 };//////////////////////////////////////////////


	unsigned int oldsegval = 0;
	unsigned int oldledstate = 0;


		//main work loop
		do
		{
			// check irq
			irqhandler(&pc, cycles);//complete!!!!!!!!!!!!!!!!!!!
			diskcyc = hdmanager(rammemory, diskcyc);

			//////////////switch comes here!!!!//////////

			updatecyc("R", "sw", cycles);//update cycles count
			triggertimer();

			// Logs and end of process stage
			LedLog(cycles, argv[LEDS]);
			sevensegmenttoLog(cycles,argv[DISPLAY7SEG]);
			triggermon();


			pc = -1;
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
void LedLog(int *cycles, char *filename)
{
	//unsigned int curledstate = ioregisters[9];///////////check if works
	if (ioregisters[LEDS] != oldsegval)
	{
		char* line = (char*)malloc(sizeof(char) * 500);///////change 500?!!!!!!
		if (line == NULL) {
			exit(1);
		}
		sprintf(line, "%d %08X\n", *cycles, ioregisters[LEDS]);
		//fprintf(fp_leds, "%u %08x\n", ioreg[8], ioreg[9]);
		write_file(filename, line);
		oldledstate = ioregisters[LEDS];
		free(line);
	}
}
void CyclesLog(int *cycles, char *filename)
{
	char line[100];
	FILE *fp = fopen(filename, "w+");

		sprintf(line, "%d", *cycles);
		// write to text file
		fprintf(fp, line);
		// close file
		fclose(fp);
		//write_file(filename, line);
		//free(line);
}
void sevensegmenttoLog(int *cycles, char *filename)
{
	unsigned int cursegval = ioregisters[DISPLAY7SEG];
	if (cursegval != oldsegval)
	{
		char* line = (char*)malloc(sizeof(char) * 500);///////change 500?!!!!!!
		if (line == NULL) {
			exit(1);
		}
		sprintf(line, "%d %08X\n", *cycles, cursegval);
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

}
void shutdownmethods(char* argv[], unsigned long long cycles)
{
	CyclesLog(cycles, argv[CYCLES]);
	logdrivetofile(argv[DISKOUT]);
	writeLogMon(argv[MONITOR], argv[MONITOR_YUV]);
	logmemout(rammemory,argv[MEMOUT]);
	logregout(registers, argv[REGOUT]);
	//////////remember to open and close file add file pointers!!!!!!!!!11!!!!!!!!!!@#$%^&
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
void set_irq2_arr(char* file_path)
{
	FILE* fp=fopen(file_path, "r");    //fopen_s!!!!!!********************************************

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
	if (irqstat==1)////check one!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		interuptflag = 1;
		ioregisters[IRQRETURN] = pc;
		pc = 0xFFF&ioregisters[IRQHANDLER]; //12 bits so we don't overflow
		////////////////////////////////////////////////////////////////////////////////check FFF!!!!!!!!!!!!
	}
}
//////////check if redundant!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void outputdisc(char* outputfile) 
{
	// write disc data to file
	int i = 0;int j = 0;
	FILE* f = fopen(outputfile, "w+");
	for (int i = 0; i < SECTOR_SIZE; i++)
	{
		for (int j = 0; j < SECTOR_COUNT; j++)
			{
				fprintf(f,"%05x\n",hardrive[i][j]);/////////////////////////////5 or 2?
			}
	}
	fclose(f);
}////////////////////////redundant??!!!!!
void inputdisc(char* inputfile) {
	// read memory from file into an array 
	int i = 0;
	int j = 0;
	int val;
	FILE* f = fopen(inputfile, "r");
	char buf_array[MAX_INPUT_LINE];
	while (fgets(buf_array,(MAX_INPUT_LINE-1), f))
	{
		buf_array[strcspn(buf_array,"\n")]=0;
		val = strtoul(buf_array, NULL, 0);
		hardrive[i][j] = val;
		j = ((j+1)%SECTOR_COUNT);
		if (j == 0)
		{
			i = ((i+1)%SECTOR_SIZE);
		}
	}
	fclose(f);
}
void logdrivetofile(char* fileName) 
{
	// write drive to file
	int totsize = SECTOR_SIZE*SECTOR_COUNT;
	FILE* fp;
	fp = fopen(fileName, "w+");
	for (int i = 0; i<totsize;i++)
	{
		fprintf(fp,"%05X\n",hardrive[i/SECTOR_SIZE][i%SECTOR_SIZE]);
	}
	fclose(fp);
}
int hdmanager(int rammemory[], int diskcyc)
{
	int disksectorid;
	int disksubsectorid;
	int  memory_id;
	//check if command exists
	if (ioregisters[DISKCMD] != 0)
	{
		// sets disk to bussy. diskstatus to 1
		ioregisters[DISKSTS] = 1;
		// 1024/128 = 8 cycles
		// read or write every 8 cycles 
		//needs to wait 1024 cycles and each sector size is 128
		if (diskcyc%8==0)
		{
			memory_id=(ioregisters[DISKBFR]+(diskcyc/8))%LINES_MAX_SIZE;
			disksectorid = ioregisters[DISKSECTOR]%SECTORS;
			disksubsectorid =(diskcyc /8)%SECTOR_SIZE;
			// if diskcmd = 1 its a read command
			if (ioregisters[DISKCMD] == 1)
			{
				rammemory[memory_id] = hardrive[disksectorid][disksubsectorid];
			}
			// if diskcmd = 2 its a write command
			else				
			{
				hardrive[disksectorid][disksubsectorid] = rammemory[memory_id];
			}
		}
		if (diskcyc < (DISK_CYCLES - 1))
		{
			diskcyc++;
		}
		else
		{
			//reseting everything
			diskcyc = 0;
			// set irq1sts = 1 and trigger irq
			ioregisters[IRQ1STS] = 1;	
			// set diskcmd to 0 so there is no active command
			ioregisters[DISKCMD] = 0;	
			// set diskstatus to 0 and free disk to work
			ioregisters[DISKSTS] = 0;	
		}
	}
	return diskcyc;
}
//getting the index of last line in memory which !=0
int lastindexinmem(int memory[])
{
	int last = 0;
	for (int i = 0; i < LINES_MAX_SIZE; i++)
	{
		if (memory[i] != 0)
		{
			last = i;
		}
	}
	return last;
}
// output memout file
void logmemout(int data[], char* fileName)
{
	FILE* fmemout = fopen(fileName, "w+");
	
	int lastval = lastindexinmem(data);
	for (int i = 0;i<=lastval;i++)
	{
		fprintf(fmemout, "%08X\n", data[i]);
	}
	fclose(fmemout);
}
//outputregout file 
void logregout(int hwreg[], char* fileName)
{
	FILE* fregout = fopen(fileName, "w+");
	for (int i = 2; i < REGS; i++)
	{
		fprintf(fregout, "%08X\n", hwreg[i]);
	}
	fclose(fregout);
}