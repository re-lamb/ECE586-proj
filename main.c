/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * usage: rvsim [-h] [-d] [-v] [-i] [-p pc] [-s stack] [[-f] filename]
 */

#include "rvdefs.h"
#include <unistd.h>

#define MAXLINELEN  40

uint8_t *mem;

/* args */
int debug = 0;
int verbose = 0;
int interactive = 0;

char defmem[] = "program.mem";
char *memfile = NULL;


/*
 * Allocate memory to load the program memory input file, Bails if it can't.
 */
uint8_t *loadfile(FILE *fp)
{
    char *line;
    char buf[MAXLINELEN];

    uint addr, value;
    int lc = 0;

    /* Allocate the (currently) fixed memory block */
    if ((mem = malloc(MEMSZ)) == NULL)
    {
        fprintf(stderr, "could not allocate %d bytes\n", MEMSZ);
        exit(-1);
    }

    /* Read and parse one line at a time */
    while ((line = fgets(buf, MAXLINELEN, fp)) != NULL)
    {
        lc++;

        if (sscanf(line, " %x: %x", &addr, &value) != 2)
        {
            fprintf(stderr, "Bad input at line %d ignored: '%s'\n", lc, line);
            continue;
        }
    
        /* Make sure the address is in range */
        if (addr >= MEMSZ-4)
        {
            fprintf(stderr, "Address 0x%X out of range at line %d\n", addr, lc);
            continue;
        }

        /* Use the store function to copy into main mem */
        memstore(/*mem, */addr, 4, value);
    }

    return mem;
}


int main(int argc, char *argv[])
{   
    int opt;
    uint pc = 0;
    uint sp = MEMSZ;

    FILE *file;
    char *memfile = NULL;

    while ((opt = getopt(argc, argv, "hdvip:s:f:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = 1;
                break;

            case 'd':
                debug = 1;
                break;

            case 'i':
                interactive = 1;
                verbose = 1;
                break;

            case 'p':
                pc = (uint)strtol(optarg, NULL, 0);   /* auto-detect radix */
                break;

            case 's':
                sp = (uint)strtol(optarg, NULL, 0);   /* auto-detect radix */
                break;

            case 'f':
                memfile = optarg;
                break;
        
            case 'h':
            default:
                fprintf(stderr, "Usage: rvsim [-h] [-d] [-v] [-i] [-p pc] [-s stack] [[-f] filename]\n");
                exit(1);
        }
    }

    if (debug)
    {
        printf("argc=%d, optind=%d, memfile=%s\n", argc, optind, memfile);
        printf("debug=%d, verbose=%d, interactive=%d, pc=0x%X, sp=0x%X\n", debug, verbose, interactive, pc, sp);
    }

    /* Any unparsed options? Assume it's a filename... */
    if (optind < argc)
    {
        memfile = argv[optind];
    }

    /* If no file given, read from the default */
    if (memfile == NULL)
    {
        memfile = defmem;
    }
    
    /* Verify input is readable */
    file = fopen(memfile, "r");
    
    if (!file)
    {
        fprintf(stderr, "Could not open file %s\n", memfile);
        exit(1);
    }
    
    mem = loadfile(file);
    fclose(file);

    run(pc, sp /*, mem */);
}