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

typedef enum opnum
{
    aluRtype = 0x33,
    aluItype = 0x13,
    load = 0x03,
    store = 0x23,
    branch = 0x63,
    jumpJtype = 0x6f,
    jumpItype = 0x67,
    loadUpper = 0x37,
    loadUPC = 0x17,
    envcall = 0x73
} Opcode;

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
    int opcode;
    InstNum func;
    int rd;
    int rs1;
    int rs2;
    int imm;
} Instruction;

extern uint8_t *mem;
extern char *instnames[];
extern char *regnames[];

int memload(uint addr, uint8_t size, bool signxt);
void memstore(uint addr, uint8_t size, int value);
void run(uint pc, uint sp /*, uint8_t *mem */);

int aluop(int a, int b, InstNum func);
int loadop(int addr, int offset, InstNum func);
void storeop(int addr, int offset, InstNum func, int value);

#endif
