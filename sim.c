/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Simulator guts
 */

#include "rvdefs.h"

char *regnames[] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0/fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};


uint loadword(uint8_t *mem, uint addr)
{
    uint*foo = (uint*)&mem[addr];
    return *foo;
}

uint8_t loadbyte(uint8_t *mem, uint addr)
{
    return mem[addr];
}

void run(uint pc, uint sp, uint8_t *mem)
{
    uint a;

    printf("Starting simulation at pc=0x%X, sp=0x%X\n", pc, sp);

    printf("Memory dump:\n");
    for (a = 0; a < 0x10; a++)
    {
        printf("byte mem[%08X] = %02X\n", a, (uint8_t)loadbyte(mem, a));

        if (a % 4 == 0) printf("word mem[%08X] = %08X\n", a, loadword(mem, a));
    }
}