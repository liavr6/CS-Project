//regular inclusions
#include <stdlib.h> 
#include <string.h>

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

// CL arguments order defenition
#define IMEMIN 1
#define DMEMIN 2
#define DISKIN 3
#define IRQ2IN 4
#define DMEMOUT 5
#define REGOUT 6
#define TRACE 7
#define HWREGTRACE 8
#define CYCLES 9
#define LEDS 10
#define DISPLAY7SEG 11
#define DISKOUT 12
#define MONITOR 13
#define MONITOR_YUV 14

// CL arguments chmod
#define IMEMINC 'r'
#define DMEMINC 'r'
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


//default parameter from class report requirments
#define IOREGS 23
#define DATA_SIZE 8
#define INT_SIZE 32
#define REGS 16
#define SECTORS 128
#define MAX_INPUT_LINE 500
#define INST_SIZE 12
#define MONITOR_PIXELS (256*256)
#define DISK_CYCLES 1024
#define MAX_LINES 4096
#define SECTOR_SIZE	128		


// define structs for main
typedef struct instruction
{
	int opcode, rd, rs, rt, imm;
}	INSTRUCTION;

//function to read input files
FILE* read_file(char filename[], char chmod);
void write_file(char *filename, char *strtowrite);
char* substr(const char *src, int strt, int end);