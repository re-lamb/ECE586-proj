/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Memory subsystem
 */

#include "rvdefs.h"

int loadop(int addr, int offset, InstNum func)
{
    switch (func)
    {
        case lb:
            return memload(addr + offset, 1, true);

        case lh:
            return memload(addr + offset, 2, true);

        case lw:
            return memload(addr + offset, 4, false);

        case lbu:
            return memload(addr + offset, 1, false);

        case lhu:
            return memload(addr + offset, 2, false);

        default:
            fprintf(stderr, "Illegal load op: %d\n", func);
            exit(-1);
            break;
    }
}

void storeop(int addr, int offset, InstNum func, int value)
{
    switch (func)
    {
        case sb:
            memstore(addr + offset, 1, value);
            break;

        case sh:
            memstore(addr + offset, 2, value);
            break;

        case sw:
            memstore(addr + offset, 4, value);
            break;
    
        default:
            fprintf(stderr, "Illegal store op: %d\n", func);
            exit(-1);
            break;
    }
}

int memload(uint addr, uint8_t size, bool signxt)
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
            if (!signxt) 
            {
                ret = (uint)mem[addr];
            }
            else 
            {
                ret = *(int8_t*)&mem[addr];
            }
            break;

        case 2:
            if (!signxt)
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

void memstore(uint addr, uint8_t size, int value)
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
