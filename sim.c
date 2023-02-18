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


void run(unsigned int pc, unsigned int sp, char *mem)
{
    printf("Starting simulation at pc=0x%X, sp=0x%X\n", pc, sp);
}