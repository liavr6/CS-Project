//regular inclusions
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

// CL arguments order defenition
#define IMEMIN 1
//#define DMEMIN 2
#define DISKIN 2
#define IRQ2IN 3
#define MEMOUT 4
#define REGOUT 5
#define TRACE 6
#define HWREGTRACE 7
#define CYCLES 8
#define LEDS 9
#define DISPLAY7SEG 10
#define DISKOUT 11
#define MONITOR 12
#define MONITOR_YUV 13

// CL arguments chmod
#define IMEMINC 'r'
#define MEMINC 'r'
#define DISKINC 'r'
#define IRQ2INC 'r'
#define DMEMOUTC 'w'
#define REGOUTC 'w'
#define TRACEC 'w'
#define HWREGTRACEC 'w'
#define CYCLESC 'w'
#define LEDSC 'w'
#define DISPLAY7SEGC 'w'
#define DISKOUTC 'w'
#define MONITORC 'w'
#define MONITOR_YUVC 'wb'

#define IRQ0STS 3
#define IRQ1STS 4
#define IRQ2STS 5
#define IRQHANDLER 6
#define IRQRETURN 7
#define LEDS 9
#define TIMERENB 11
#define TIMERCURR 12
#define TIMERMAX 13
#define DISKCMD 14
#define DISKSECTOR 15
#define DISKBFR 16
#define DISKSTS 17
#define MONITORADDR 20
#define MONITORDATA 21
#define MONITORCMD 22

//default parameter from class report requirments

#define IOREGS 23
#define DATA_SIZE 8
#define INT_SIZE 32
#define REGS 16
#define SECTORS 128
#define MAX_INPUT_LINE 500
#define INST_SIZE 12
#define MONITOR_PIXELS (256*256)
#define MON_SIZE 256
#define DISK_CYCLES 1024
#define LINES_MAX_SIZE 4096
#define SECTOR_SIZE	5		
#define SECTOR_COUNT 128
unsigned int oldsegval = 0;
unsigned int oldledstate = 0;
int irqstat = 0;

//int ioregisters[IOREGS] = { 0 };

// define structs for main
typedef struct instruction
{
	int opcode, rd, rs, rt, imm;
}	INSTRUCTION;

//function to read input files
FILE* read_file(char filename[], char chmod);
void write_file(char *filename, char *strtowrite);
char* substr(const char *src, int strt, int end);
void updatecyc(char type, char* cmd, int* cycles);
void LedLog(int *cycles, char *filename);
void triggermon();
void irqhandler(int pc, int *cycles);
void shutdownmethods(char* argv[], int* cycles);
void CyclesLog(int *cycles, char *filename);
void triggertimer();
void writeval2mon();
void set_irq2_arr(char* file_path);
void check_irq2arr(unsigned long long cycles);
//void outputdisc(char* outputfile);
void inputdisc(char* inputfile);
void logdrivetofile(char* fileName);
int hdmanager(int rammemory[], int diskcyc);
void logmemout(int data[], char* fileName);
int lastindexinmem(int memory[]);
void logregout(int hwreg[], char* fileName);
void sevensegmenttoLog(int *cycles, char *filename);
void touppers(char *str);