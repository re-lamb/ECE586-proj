/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * usage: rvsim [-h] [-dvi] [-b bkpt] [-p pc] [-s stack] [[-f] filename]
 *
 * args:
 *      -h      print usage message
 *      -d      enable debug output
 *      -v      enable verbose output
 *      -i      interactive (single step)
 *      -b bkpt set a breakpoint
 *      -p pc   set the initial program counter
 *      -s sp   set the stack pointer
 *      -f file read from 'file'
 */

#include "rvdefs.h"
#include <termios.h>

#define MAXLINELEN  40

uint8_t *mem;

/* args */
int debug = 0;
int verbose = 0;
int interactive = 0;
int breakpoint = 0;

uint breakAddr = 0;

char defmem[] = "program.mem";
char *memfile = NULL;

/* for interactive mode */
struct termios orig_termios;

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

/* restore the original term settings */
void setcooked() 
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/* interactive sets raw mode without echo */
void setraw()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(setcooked);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(int argc, char *argv[])
{   
    int opt;
    uint pc = 0;
    uint sp = MEMSZ;

    FILE *file;
    char *memfile = NULL;

    while ((opt = getopt(argc, argv, "hdvib:p:s:f:")) != -1)
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

            case 'b':
                breakpoint = 1;
                breakAddr = (uint)strtol(optarg, NULL, 0);   /* auto-detect radix */
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
                fprintf(stderr, "Usage: rvsim [-h] [-idv] [-b bkpt] [-p pc] [-s stack] [[-f] filename]\n");
                exit(1);
        }
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
 
    if (debug)
    {
        printf("argc=%d, optind=%d, memfile=%s\n", argc, optind, memfile);
        printf("debug=%d, verbose=%d, interactive=%d, breakpoint=%d (%x)\n",
                debug, verbose, interactive, breakpoint, breakAddr);
        printf("pc=0x%X, sp=0x%X\n", pc, sp);
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

    setraw();
    run(pc, sp /*, mem */);
    setcooked();
}