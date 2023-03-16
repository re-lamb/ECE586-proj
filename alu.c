/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * ALU functions
 */

#include "rvdefs.h"

int aluop(int a, int b, InstNum func)
{
    int f;
    int shamt = (b & 0x1f);     /* shift amount */

    if (debug) printf("Shift amt: %d\n", shamt);
    
    switch (func)
    {
        case add:
        case addi:
            f = a + b;
            break;

        case sub:
            f = a - b;
            break;

        case xor:
        case xori:
            f = a ^ b;
            break;

        case or:
        case ori:
            f = a | b;
            break;

        case and:
        case andi:
            f = a & b;
            break;

        case sll:
        case slli:
            /* shift amount clipped to 5 bits */
            f = a << shamt;
            break;

        case srl:
        case srli:
            /* coerce to uint, gcc won't sign extend */
            f = (uint)a >> shamt;
            break;

        case sra:
        case srai:
            /* gcc sign extends here! (int)a */
            f = a >> shamt;
            break;

        case slt:
        case slti:
            f = (a < b) ? 1 : 0;
            break;

        case sltu:
        case sltiu:
            f = ((uint)a < (uint)b) ? 1 : 0;
            break;

        default:
            fprintf(stderr, "Bad ALU function: %d\n", func);
    }

    if (debug) printf("ALU: %s %d, %d => %d\n", instnames[func], a, b, f);
    return f;
}