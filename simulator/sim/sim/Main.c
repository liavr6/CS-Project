#define _CRT_SECURE_NO_WARNINGS

#include "Main.h"
#include "monitor.h"
#include "simulator_new.h" 

int interuptflag = 0;
int hardrive[SECTORS*SECTORS][SECTOR_SIZE] = { 0 };
int rammemory[LINES_MAX_SIZE] = { 0 };
int registers[REGS] = { 0 };
int ioregisters[IOREGS] = { 0 };
int* irq2arr;
int irq2arrlen;
FILE* fptrace;
FILE* ledlog;
FILE* sevenseg;

int oldledstate = 0;
FILE* hwfile;
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
	hwfile = fopen(argv[HWREGTRACE], "w");

	int* pc;
	pc = (int*)malloc(sizeof(int));
	*pc = 0;
	int irqstat = 0;
	int* cycles;
	cycles = (int*)malloc(sizeof(int));
	*cycles = 0;
	
	int* diskcyc;
	diskcyc = (int*)malloc(sizeof(int));
	*diskcyc = 0;
	inputdisc(argv[DISKIN]);//read data from harddrive
	set_irq2_arr(argv[IRQ2IN]);
	




	unsigned int oldsegval = 0;


	FILE* fp1;
	char line[100];
	char** lines = malloc(sizeof(char*) * (1));
	fp1 = fopen(argv[MEMIN], "r");
	if (fp1 == NULL)
	{
		printf("Error opening file\n");
		return;
	}
	fptrace = fopen(argv[TRACE], "w");
	if (fptrace == NULL)
	{
		printf("Error opening trace file\n");
		return;
	}
	ledlog = fopen(argv[LEDS], "w");
	if (ledlog == NULL)
	{
		printf("Error opening led file\n");
		return;
	}
	sevenseg = fopen(argv[DISPLAY7SEG], "w");
	if (sevenseg == NULL)
	{
		printf("Error opening sevensegment file\n");
		return;
	}
	while (fgets(line, 100, fp1) != NULL) // creating an array of the lines of memin.txt
	{
		char* temp = malloc(sizeof(char*) * (100));
		strcpy(temp, line);
		lines = (char**)realloc(lines, (sizeof(char*) * (*pc + 1)));
		lines[*pc] = temp;
		*pc += 1;
	}
	for (int i = 0; i < *pc; i++)
	{
		rammemory[i] = strtol(lines[i], NULL, 10);
	}
	*pc = 0;
	while (*pc != -1)
	{

		registers[1] = 0;
		int op_num, rd, rs, rt;
		char operation[4];
		strncpy(operation, lines[*pc], 2);
		operation[2] = '\n';
		operation[3] = '\0';
 
		op_num = string_in_hex_to_int(operation);
		rd = char_in_hex_to_int(lines[*pc][2]);
		rs = char_in_hex_to_int(lines[*pc][3]);
		rt = char_in_hex_to_int(lines[*pc][4]);
 
		if (op_num == 21) // opcode is halt
		{
			LogTrace(fptrace, lines[*pc], pc);
			*pc = -1;
			break;
		}

		if ((op_num >= 0 && op_num <= 8) || (op_num == 16) || (op_num == 19))
		{
			if (rd == 0 || rd == 1) // R[rd] is $zero or $imm and it can't be rewritten
			{
				if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
				{

					// we still need to read the next line ($imm value)
					registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 2;
					*cycles += 1;
					irqhandler(pc, cycles);
					*cycles += 1;
					irqhandler(pc, cycles);
				}
				else // R[rs] and R[rt] aren't $imm, so the instruction is in R-format
				{
					*cycles += 1;
					
					irqhandler(pc, cycles);
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 1;
				}
				continue; // $zero or $imm can't be rewritten, so we go to the next iteration
			}
			else // R[rd] is not $zero nor $imm
			{
				if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
				{
					// we still need to read the next line ($imm value)

				
					registers[1] = string_in_hex_to_int(lines[(*pc) + 1]); // put const value in $imm
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 2;
					*cycles += 1;
					irqhandler(pc, cycles);
					*cycles += 1;
					irqhandler(pc, cycles);
				}
				else // R[rs] and R[rt] aren't $imm, so the instruction is in R-format
				{
					*cycles += 1;
					
					irqhandler(pc, cycles);
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 1;
				}
			}
			switch (op_num)
			{
			case 0:
				
				add(registers, rd, rs, rt);
				break;
			case 1:
				
				sub(registers, rd, rs, rt);
				break;
			case 2:
				
				mul(registers, rd, rs, rt);
				break;
			case 3:
				
				andn(registers, rd, rs, rt);
				break;
			case 4:
				
				orn(registers, rd, rs, rt);
				break;
			case 5:
				
				xorn(registers, rd, rs, rt);
				break;
			case 6:
				
				sll(registers, rd, rs, rt);
				break;
			case 7:
				
				sra(registers, rd, rs, rt);
				break;
			case 8:
				
				srl(registers, rd, rs, rt);
				break;
			case 16:
				
				*cycles += 1;
				irqhandler(pc, cycles);
				lw(registers, rammemory, rd, rs, rt);
				break;
			case 19:
				
				in(registers, ioregisters, rd, rs, rt);
				fprintf(hwfile, "%d READ %s %08X\n", *cycles, ioregname(registers[rs] + registers[rt]), registers[rd]);
				break;
			default:
				break;
			}
		}
		if (op_num == 15) // opcode is jal
		{
			if (rd == 0)
			{
				if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
				{
					// we still need to read the next line ($imm value)

					registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 1; // the instruction is in I-format, so it's 2 lines long
					*cycles += 1;
					irqhandler(pc, cycles);
					*cycles += 1;
					irqhandler(pc, cycles);
				}
				else
				{
					*cycles += 1;
					irqhandler(pc, cycles);
					
					LogTrace(fptrace, lines[*pc], pc);
				}
				// rd is $zero so it can't be rewritten, so the jal will perform only athe jump
				*pc = registers[rs]; // jump execution
			}
			if (rd == 1)
			{
				if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
				{
					// we still need to read the next line ($imm value)

					registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 2; // the instruction is in I-format, so it's 2 lines long
					*cycles += 1;
					irqhandler(pc, cycles);
					*cycles += 1;
					irqhandler(pc, cycles);
				}
				else{ 
					LogTrace(fptrace, lines[*pc], pc); 
				
				}
				// rd is $zero so it can't be rewritten, so the jal will perform only athe jump
				*pc = registers[rs]; // jump execution
			}
			else
			{
				if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
				{
					// we still need to read the next line ($imm value)

					registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
					LogTrace(fptrace, lines[*pc], pc);
					*pc += 1; // the instruction is in I-format, so it's 2 lines long
					*cycles += 1;
					irqhandler(pc, cycles);
					*cycles += 1;
					irqhandler(pc, cycles);
				}
				else{ LogTrace(fptrace, lines[*pc], pc); 
				}
				jal(registers, rd, rs, rt, pc);
				continue;
			}
		}
		if (op_num >= 9 && op_num <= 14)
		{

			if (rs == 1 || rt == 1|| rd == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
			{
				// we still need to read the next line ($imm value)

				registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
				LogTrace(fptrace, lines[*pc], pc);
				*pc += 1;
				*cycles += 1;
				irqhandler(pc, cycles);
				*cycles += 1;
				irqhandler(pc, cycles);
			}
			else{ LogTrace(fptrace, lines[*pc], pc);
			}
			*pc += 1;
			switch (op_num)
			{
			case 9:
				
				beq(registers, rd, rs, rt, pc);
				break;
			case 10:
				
				bne(registers, rd, rs, rt, pc);
				break;
			case 11:
				
				blt(registers, rd, rs, rt, pc);
				break;
			case 12:
				
				bgt(registers, rd, rs, rt, pc);
				break;
			case 13:
				
				ble(registers, rd, rs, rt, pc);
				break;
			case 14:
				
				bge(registers, rd, rs, rt, pc);
				break;
			default:
				break;
			}
		}
		if (op_num == 17 || op_num == 18 || op_num == 20)
		{
			
			if (rs == 1 || rt == 1) // R[rs] or R[rt] are $imm, so the instruction is in I-format
			{
				// we still need to read the next line ($imm value)

				registers[1] = string_in_hex_to_int(lines[(*pc) + 1]);// put const value in $imm
				LogTrace(fptrace, lines[*pc], pc);
				*pc += 1;
				*cycles += 1;
				irqhandler(pc, cycles);
				*cycles += 1;
				irqhandler(pc, cycles);
			}
			else{ LogTrace(fptrace, lines[*pc], pc); 
			}
			*pc += 1;
			switch (op_num)
			{
			case 17:
				
				*cycles += 1;
				irqhandler(pc, cycles);
				sw(registers, rammemory, rd, rs, rt);
				break;
			case 18:
				
				
				reti(registers, ioregisters, rd, rs, rt, pc);
				break;
			case 20:
				
				out(registers, ioregisters, rd, rs, rt);
				fprintf(hwfile, "%d WRITE %s %08X\n", *cycles, ioregname(registers[rs] + registers[rt]), registers[rd]);
				break;
			default:
				break;
			}
		}

		irqhandler(pc, cycles);
		// Logs and end of process stage
		LedLog(cycles, ledlog);
		sevensegmenttoLog(cycles, sevenseg);
		triggermon();
		hdmanager(diskcyc);
	}
	
		shutdownmethods(argv, cycles,lines);
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

void LedLog(int *cycles, FILE* filename)
{
	if (ioregisters[LEDS] != oldledstate)//if value changed
	{
		char* line = (char*)malloc(sizeof(char) * 500);
		if (line == NULL) {
			exit(1);
		}
		fprintf(filename, "%d %08X\n", *cycles, ioregisters[LEDS]);

		oldledstate = ioregisters[LEDS];
		free(line);
	}
}
void CyclesLog(int *cycles, char *filename)
{
	//log cycles
	char line[100];
	FILE *fp = fopen(filename, "w+");

		sprintf(line, "%d", *cycles);
		// write to text file
		fprintf(fp, line);
		// close file
		fclose(fp);

}
void sevensegmenttoLog(int *cycles, FILE* filename)
{
	unsigned int cursegval = ioregisters[DISPLAY7SEG];
	if (cursegval != oldsegval)//if value changed
	{
		char* line = (char*)malloc(sizeof(char) * 500);
		if (line == NULL) {
			exit(1);
		}

		
		fprintf(filename, "%d %08X\n", *cycles, cursegval);
		
		oldsegval = cursegval;
		free(line);
	}
}
void writeval2mon()
{
	//manages monitor writing
	unsigned char val = ioregisters[MONITORDATA];
	unsigned int ofst = ioregisters[MONITORADDR];

	changecell(ofst/MON_SIZE, ofst%MON_SIZE, val);
}
void triggermon()
{
	//check if need to write val to mon
	unsigned int boolwritemon = ioregisters[MONITORCMD];
	if (boolwritemon)
	{
		writeval2mon();
		ioregisters[MONITORCMD] = 0;
	}

}
void shutdownmethods(char* argv[], unsigned long long cycles, char** lines)
{
	//logs and closings
	CyclesLog(cycles, argv[CYCLES]);
	logdrivetofile(argv[DISKOUT]);
	writeLogMon(argv[MONITOR_YUV], argv[MONITOR]);
	logmemout(rammemory,argv[MEMOUT]);
	logregout(registers, argv[REGOUT]);
	fclose(fptrace);
	fclose(hwfile);
	fclose(ledlog);
	fclose(sevenseg);
	
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
	//build list of irq2s
	FILE* fp=fopen(file_path, "r");    

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
void check_irq2arr(int *cycles)
{
	//checks if there is irq2
	for (int i = 0; i < irq2arrlen; i++)
	{
		if (irq2arr[i] == *cycles)
		{
			ioregisters[IRQ2STS] = 1;
			break;
		}
	}
}
void irqhandler(int* pc, int *cycles)
{
//manages irqs
	if (interuptflag == 1)
	{
		return;
	}

	check_irq2arr(cycles);
	triggertimer();
	
	//updating interuption station
	irqstat = ((ioregisters[0] && ioregisters[3]) || (ioregisters[1] && ioregisters[4]) || (ioregisters[2] && ioregisters[IRQ2STS]));
	if (irqstat==1)
	{
		interuptflag = 1;
		ioregisters[IRQRETURN] = *pc;
		*pc = 0xFFF&ioregisters[IRQHANDLER]; //12 bits so no overflow
	}
}

void inputdisc(char* inputfile) {
	// read memory from file into an array 
	int i = 0;
	int j = 0;
	int val;
	FILE* f = fopen(inputfile, "r");
	char buf_array[MAX_INPUT_LINE];
	
	while (fgets(buf_array,(MAX_INPUT_LINE +1), f))
	{
		buf_array[strcspn(buf_array,"\n")];
		touppers(buf_array);
		
		for (int i = 0; i < SECTOR_SIZE; i++)
		{
			
			val = char_in_hex_to_int(buf_array[i]); 
			hardrive[j][i] = val;
			
		}

		j += 1;

	
	}
	fclose(f);
}

void touppers(char *str) 
{
	//converts strings to uppercase
	int i = 0;

	while (str[i]) 
	{
		if (isalpha(str[i]))
		{
			str[i]=toupper(str[i]);
		}
		i++;
	}

}
void logdrivetofile(char* fileName) 
{
	// write drive to file
	int totsize = sizeof(hardrive) / sizeof(hardrive[0]);
	FILE* fp;
	fp = fopen(fileName, "w+");
	for (int i = 0; i<totsize;i++)
	{
		fprintf(fp,"%01X%01X%01X%01X%01X\n",hardrive[i][0], hardrive[i][1], hardrive[i][2], hardrive[i][3], hardrive[i][4]);
	}
	fclose(fp);
}
void hdmanager(int* diskcyc)
{
	int disksectorid;
	int disksubsectorid;
	int  memory_id;
	//check if command exists
	if ((ioregisters[DISKCMD] != 0) && (*diskcyc == 0))
	{
		if (ioregisters[DISKSTS] == 0)
		{
			// sets disk to bussy. diskstatus to 1
			ioregisters[DISKSTS] = 1;
			

				// if diskcmd = 1 its a read command
			if (ioregisters[DISKCMD] == 1)
			{
				for (int i = 0; i < 128; i++)
				{
					rammemory[ioregisters[DISKBFR] + i] = hardrive[ioregisters[DISKSECTOR]][i];
				}
			}
			// if diskcmd = 2 its a write command
			else
			{
				for (int i = 0; i < 128; i++)
				{
					hardrive[ioregisters[DISKSECTOR]][i] = rammemory[ioregisters[DISKBFR] + i];
				}
			}
		}
	}
		else if ((*diskcyc) < (DISK_CYCLES - 1))
		{
			(*diskcyc)++;
		}
		else
		{
			//reseting everything
			(*diskcyc) = 0;
			// set irq1sts = 1 and trigger irq
			ioregisters[IRQ1STS] = 1;	
			// set diskcmd to 0 so there is no active command
			ioregisters[DISKCMD] = 0;	
			// set diskstatus to 0 and free disk to work
			ioregisters[DISKSTS] = 0;	
		}
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

void LogTrace(FILE* file_name, char* inst, int* pc)
{
	fprintf(file_name, "%03X ", *pc);
	remove_char(inst, '\n');

	fprintf(file_name, "%s", inst);
	for (int i = 0; i < REGS; i++)
	{
		fprintf(file_name, " %08X", registers[i]);
	}
	fprintf(file_name, "\n");
	fflush(file_name);
}

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

void reti(int* registers, int* IORegister, int rd, int rs, int rt, int* pc)
{
	*pc = IORegister[7];
	interuptflag = 0;
}

//ioregister num return name
const char* ioregname(int num)
{
	switch (num)
	{
	case 0:	return "irq0enable";
	case 1:	return "irq1enable";
	case 2: return "irq2enable";
	case 3: return "irq0status";
	case 4: return "irq1status";
	case 5: return "irq2status";
	case 6: return "irqhandler";
	case 7: return "irqreturn";
	case 8: return "clks";
	case 9: return "leds";
	case 10: return "display7seg";
	case 11: return "timerenable";
	case 12: return "timercurrent";
	case 13: return "timermax";
	case 14: return "diskcmd";
	case 15: return "disksector";
	case 16: return "diskbuffer";
	case 17: return "diskstatus";
	case 18: return "reserved";
	case 19: return "reserved";
	case 20: return "monitoraddr";
	case 21: return "monitordata";
	case 22: return "monitorcmd";
	default: return "error";
	}
}