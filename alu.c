/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * ALU functions
 */

#include "rvdefs.h"

int alu(int a, int b, int op)
{
    int f;
    int mask = 0xffffffff;      /* all ones for sign ext */
    int shamt = (b & 0x1f);     /* shift amount */

    switch (op)
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
            f = a >> shamt;
            break;

        case sra:
        case srai:
            f = a >> shamt;
            if (a & 0x80000000)
            {
                f |= mask << (32 - shamt);  /* add ext bits */
            }
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
            fprintf(stderr, "Bad ALU op: %d\n", op);
    }

    printf("ALU: %s %d, %d => %d\n", instnames[op], a, b, f);
    return f;
}