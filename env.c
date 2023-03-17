/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * env functions
 */

#include "rvdefs.h"

#define SYSREAD     63
#define SYSWRITE    64
#define SYSEXIT     94

int envop(Instruction inst)
{
    int call = regread(17); /* fetch syscall number */
    int fd = regread(10);   /* file destricptor */
    int buffaddr = regread(11); /* address of buffer */
    int len = regread(12);

    int cnt = 0;
    int res = 0;
    uint8_t c;

    if (debug) printf("Syscall %d: fd=%d, addr=0x%08X, len=%d\n", call, fd, buffaddr, len);
    
    switch (call)
    {
        case SYSREAD:
            /* Always print so we know to type something! */
            printf("Read expects %d bytes:\n", len);
 
            while (cnt < len)
            {
                res = read(fd, &c, 1);

                if (res < 0)
                {
                    if (debug) printf("\nRead error %d after %d bytes\n", res, cnt);
                    return -1;
                }
                else if (res == 0)
                {
                    if (debug) printf("\nEOF on read\n");
                    break;
                }
                else if (res > 0)
                {
                    cnt++;
                    printf("%c", c);
                    fflush(stdout);
                    memstore(buffaddr++, 1, c);
                }
            }
            printf("\n");
            if (debug) printf("Read %d bytes\n", cnt);
            return cnt;

        case SYSWRITE:
            if (debug) printf("Writing %d bytes: ", len);

            while (cnt < len)
            {
                c = (uint8_t)memload(buffaddr++, 1, false);
                printf("%c", c);
                cnt++;
            }
            printf("\n");
            if (debug) printf("Wrote %d bytes\n", cnt);
            return cnt;

        case SYSEXIT:
            printf("Execution stopped by exit call\n");
            dumpregs();
            exit(fd);

        default:
            fprintf(stderr, "Unimplemented syscall %d\n", call);
            break;
    }

    return -1;
}