/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Memory subsystem
 */

#include "rvdefs.h"

int memload(uint8_t *mem, uint addr, uint8_t size, bool signxt)
{
    // TODO: Add stats and debug
    
    int ret;

    if ((addr + size-1) >= MEMSZ)
    {
        fprintf(stderr, "Load from 0x%08X out of range\n", addr);
        exit(-1);
    }

    /* Abuse the compiler instead of shifting and masking */
    switch (size) 
    {
        case 1:
            if (signxt) 
            {
                ret = (uint)mem[addr];
            }
            else 
            {
                ret = *(int8_t*)&mem[addr];
            }
            break;

        case 2:
            if (signxt)
            {
                ret = *(uint16_t*)&mem[addr];
            }
            else 
            {
                ret = *(int16_t*)&mem[addr];
            }
            break;

        case 4:
            ret = *(uint*)&mem[addr];
            break;

        default:
            fprintf(stderr, "Illegal load request (%d bytes) at 0x%08X\n", size, addr);
            exit(-1);
    }

    return (int)ret;
}

void memstore(uint8_t *mem, uint addr, uint8_t size, int value)
{
    // TODO: Add stats and debug

    if ((addr + size-1) >= MEMSZ)
    {
        fprintf(stderr, "Store to 0x%08X out of range\n", addr);
        exit(-1);
    }

    switch (size) 
    {
        case 1:
            mem[addr] = (uint8_t)value;
            break;

        case 2:
            *(uint16_t*)&mem[addr] = (uint16_t)value;
            break;

        case 4:
            *(uint32_t*)&mem[addr] = (uint32_t)value;
            break;

        default:
            fprintf(stderr, "Illegal store request (%d bytes) at 0x%08X\n", size, addr);
            exit(-1);
    }
}
