#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int char_in_hex_to_int(char ch);
int string_in_hex_to_int_minus(char* str);
int string_in_hex_to_int(char* str);
void add(int* registers, int rd, int rs, int rt);
void sub(int* registers, int rd, int rs, int rt);
void mul(int* registers, int rd, int rs, int rt);
void andn(int* registers, int rd, int rs, int rt);
void orn (int* registers, int rd, int rs, int rt);
void xorn(int* registers, int rd, int rs, int rt);
void sll(int* registers, int rd, int rs, int rt);
void sra(int* registers, int rd, int rs, int rt);
void srl(int* registers, int rd, int rs, int rt);
void beq(int* registers, int rd, int rs, int rt, int* pc);
void bne(int* registers, int rd, int rs, int rt, int* pc);
void blt(int* registers, int rd, int rs, int rt, int* pc);
void bgt(int* registers, int rd, int rs, int rt, int* pc);
void ble(int* registers, int rd, int rs, int rt, int* pc);
void bge(int* registers, int rd, int rs, int rt, int* pc);
void jal(int* registers, int rd, int rs, int rt, int* pc);
void lw(int* registers, int* MEM, int rd, int rs, int rt);
void sw(int* registers, int* MEM, int rd, int rs, int rt);
void reti(int* registers, int* IORegister, int rd, int rs, int rt, int* pc);
void in(int* registers, int* IORegister, int rd, int rs, int rt);
void out(int* registers, int* IORegister, int rd, int rs, int rt);
void halt(int pc);
void recognize_opcode(char* file_name, int* ioregisters, int* registers, int* MEM, int* pc, int inetrruptflag);

