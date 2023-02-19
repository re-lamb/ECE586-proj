/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Global definitions
 */

#ifndef _RVDEFS_
#define _RVDEFS_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MEMSZ       0x10000

typedef enum instnum
{
    add,    sub,    xor,    or,     and,    sll,    srl,    sra,
    slt,    sltu,   addi,   xori,   ori,    andi,   slli,   srli,
    srai,   slti,   sltiu,  lb,     lh,     lw,     lbu,    lhu,
    sb,     sh,     sw,     beq,    bne,    blt,    bge,    bltu,
    bgeu,   jal,    jalr,   lui,    auipc,  ecall,  ebreak,
    unknown
} InstNum;

typedef struct decodedInst
{
    InstNum op;
    int rd;
    int rs1;
    int rs2;
    int imm;
} Instruction;

extern char *instnames[];
extern char *regnames[];

int memload(uint8_t *mem, uint addr, uint8_t size, bool signxt);
void memstore(uint8_t *mem, uint addr, uint8_t size, int value);
void run(uint pc, uint sp, uint8_t *mem);

int alu(int a, int b, int op);

#endif
