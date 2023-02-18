/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * usage: rvsim [-d] [-v] [-pc=n] [-sp=n] [[-f] filename]
 */

#include "rvdefs.h"
#include <unistd.h>

#define MEMSZ 0x10000


unsigned int pc = 0;
unsigned int sp = MEMSZ;
char *mem;

/* args */
int debug = 1;
int verbose = 1;

char defmem[] = "program.mem";
char *memfile = NULL;


char *load(FILE *fp)
{
    /* Allocate the (currently) fixed memory block */
    if ((mem = malloc(MEMSZ)) == NULL)
    {
        fprintf(stderr, "could not allocate %d bytes\n", MEMSZ);
        exit(-1);
    }

    return mem;
}


int main(int argc, char *argv[])
{   
    int opt;

    FILE *file;
    char *memfile = NULL;

    while ((opt = getopt(argc, argv, "dvp:s:f:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = 1;
                break;

            case 'd':
                debug = 1;
                break;

            case 'p':
                pc = (unsigned int)strtol(optarg, NULL, 0);   /* auto-detect radix */
                break;

            case 's':
                sp = (unsigned int)strtol(optarg, NULL, 0);   /* auto-detect radix */
                break;

            case 'f':
                memfile = optarg;
                break;
        
            default:
                fprintf(stderr, "Usage: rvsim [-d] [-v] [-p pc] [-s stack] [[-f] filename]\n");
                exit(1);
        }
    }

    printf("argc=%d, optind=%d, memfile=%s\n", argc, optind, memfile);
    printf("debug=%d, verbose=%d, pc=0x%X, sp=0x%X\n", debug, verbose, pc, sp);

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
    
    mem = load(file);
    close(file);

    run(pc, sp, mem);
}