/*
 * ECE586 Winter 2023 Final Project
 * R.E. Lamb
 *
 * Simulator guts
 */

#include "rvdefs.h"

char *regnames[] = {
    "zer", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};

char *instnames[] = {
    "add",  "sub",  "xor",  "or",   "and",  "sll",  "srl",  "sra",
    "slt",  "sltu", "addi", "xori", "ori",  "andi", "slli", "srli",
    "srai", "slti", "sltiu","lb",   "lh",   "lw",   "lbu",  "lhu",
    "sb",   "sh",   "sw",   "beq",  "bne",  "blt",  "bge",  "bltu",
    "bgeu", "jal",  "jalr", "lui",  "auipc","ecall","ebreak",
    "exit", "unknown"
};


int reg[32];

int regread(int r)
{
    if (r >= 0 && r <= 31)
    {
        if (debug) printf("Read  x%d (%s) = %d (0x%X)\n", r, regnames[r], reg[r], reg[r]);
        return reg[r];
    }
    else
    {
        fprintf(stderr, "Register %d read out of range\n", r);
        exit(-1);
    }
}

void regwrite(int r, int val)
{
    if (r > 0 && r <= 31)
    {
        reg[r] = val;
        if (debug) printf("Write x%d (%s) = %d (0x%X)\n", r, regnames[r], reg[r], reg[r]);
    }
    else if (r == 0)
    {
        if (debug) printf("Write x0 (zero) ignored\n");
    }
    else
    {
        fprintf(stderr, "Register %d write out of range\n", r);
        exit(-1);
    }
}

bool branchop(int rs1, int rs2, InstNum func)
{
    switch (func)
    {
        case beq:
            return (rs1 == rs2);

        case bne:
            return (rs1 != rs2);

        case blt:
            return (rs1 < rs2);

        case bge:
            return (rs1 >= rs2);

        case bltu:
            return ((uint)rs1 < (uint)rs2);

        case bgeu:
            return ((uint)rs1 >= (uint)rs2);

        default:
            fprintf(stderr, "Illegal branch op %d\n", func);
            exit(-1);
            break;
    }
}

Instruction decode(uint value)
{
    Instruction this;

    int tmp;    /* for assembling the immediate field */

    int funct3 = (value >> 12) & 0x7;
    int funct7 = (value >> 25) & 0xfe;

    this.opcode = value & 0x7f;
    this.func = unknown;
    this.rd =  (value & 0x0000f80) >> 7;
    this.rs1 = (value & 0x00f8000) >> 15;
    this.rs2 = (value & 0x1f00000) >> 20;
    this.imm = 0;

    switch (this.opcode)
    {
        case aluRtype:
            // R format
            switch (funct3)
            {
                case 0:
                    this.func = (funct7 == 0x20) ? sub :
                                (funct7 == 0x00) ? add : unknown;
                    break;

                case 1:
                    this.func = sll;
                    break;

                case 2:
                    this.func = slt;
                    break;

                case 3:
                    this.func = sltu;
                    break;

                case 4:
                    this.func = xor;
                    break;

                case 5:
                    this.func = (funct7 == 0x20) ? sra :
                                (funct7 == 0x00) ? srl : unknown;
                    break;

                case 6:
                    this.func = or;
                    break;

                case 7:
                    this.func = and;
                    break;
            }
            break;


        case aluItype:
            // I format (ALU ops)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & SIGNBIT) this.imm |= 0xfffff000;
            
            switch (funct3)
            {
                case 0:
                    this.func = addi;
                    break;

                case 1:
                    this.func = slli;
                    break;

                case 2:
                    this.func = slti;
                    // todo: bits 11:5 must be 0?
                    break;

                case 3:
                    this.func = sltiu;
                    break;

                case 4:
                    this.func = xori;
                    break;

                case 5:
                    /* NB: imm[11:5] are same bits as funct7! */
                    this.func = (funct7 == 0x20) ? srai :
                                (funct7 == 0x00) ? srli : unknown;
                    break;

                case 6:
                    this.func = ori;
                    break;

                case 7:
                    this.func = andi;
                    break;
            }  
            break;

        case load:
            // I format (loads)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & SIGNBIT) this.imm |= 0xfffff000;  /* sign extended */

            switch (funct3)
            {
                case 0:
                    this.func = lb;
                    break;

                case 1:
                    this.func = lh;
                    break;

                case 2:
                    this.func = lw;
                    break;

                case 4:
                    this.func = lbu;
                    break;

                case 5:
                    this.func = lhu;
                    break;
            }
            break;

        case jumpItype:
            // I format (jalr)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & SIGNBIT) this.imm |= 0xfffff000;
            
            if (funct3 == 0)
            {
                this.func = jalr;

                if (this.rd == 0 && this.rs1 == 0 && this.imm == 0)
                    this.func = exitprog;
            }
            break;

        case envcall:
            // I format (ecall/ebreak)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & SIGNBIT) this.imm |= 0xfffff000;  /* sign extended */

            if (funct3 == 0)
                this.func = (this.imm == 1) ? ebreak :
                            (this.imm == 0) ? ecall : unknown;
            break;

        case store:
            // S format (stores)
            tmp = (value & 0xfe000000) >> 20;   // bits 31:25
            if (value & SIGNBIT) tmp |= 0xfffff000;
            this.imm = tmp | this.rd;           // bits 11:7

            switch (funct3)
            {
                case 0:
                    this.func = sb;
                    break;

                case 1:
                    this.func = sh;
                    break;

                case 2:
                    this.func = sw;
                    break;
            }
            break;

        case branch:
            // B format (branches)
            tmp = (value & SIGNBIT) ? 0xfffff000 : 0;
            this.imm = tmp | ((value & 0x80) << 4 |
                              (value & 0x7e000000) >> 20 |
                              (value & 0xf00) >> 7);

            switch (funct3)
            {
                case 0:
                    this.func = beq;
                    break;

                case 1:
                    this.func = bne;
                    break;

                case 4:
                    this.func = blt;
                    break;

                case 5:
                    this.func = bge;
                    break;

                case 6:
                    this.func = bltu;
                    break;

                case 7:
                    this.func = bgeu;
                    break;
            }
            break;

        case jumpJtype:
            // J format (jump and link)
            tmp = (value & SIGNBIT) ? 0xfff00000 : 0;
            this.imm = tmp | ((value & 0x7fe00000) >> 20 |
                              (value & 0x00100000) >> 9 |
                              (value & 0x000ff000));
            this.func = jal;    
            break;

        case loadUPC:
        case loadUpper:
            // U format
            this.imm = (value & 0xfffff000);
            this.func = (this.opcode == loadUpper) ? lui : auipc;
            break;

        default:
            fprintf(stderr, "Unimplemented opcode 0x%02X\n", this.opcode);
            exit(-1);
    }

    if (this.func == unknown)
    {
        fprintf(stderr, "Unknown instruction 0x%08X!\n", value);
        exit(-1);
    }
    return this;
}

uint execute(uint pc, Instruction inst)
{
    int rs1 = regread(inst.rs1);
    int rs2 = regread(inst.rs2);
    int nextpc = pc + 4;

    switch (inst.opcode)
    {
        case aluRtype: 
            regwrite(inst.rd, aluop(rs1, rs2, inst.func));
            break;
        
        case aluItype: 
            regwrite(inst.rd, aluop(rs1, inst.imm, inst.func));
            break;

        case load:
            regwrite(inst.rd, loadop(rs1, inst.imm, inst.func));
            break;

        case store:
            storeop(rs1, inst.imm, inst.func, rs2);
            break;

        case branch:
            if (branchop(rs1, rs2, inst.func))
            {
                if (debug) printf("Branch taken\n");
                nextpc = (int)pc + inst.imm;
            }
            break;

        case jumpJtype:
            regwrite(inst.rd, nextpc);
            nextpc = (int)pc + inst.imm;
            break;

        case jumpItype:
            regwrite(inst.rd, nextpc);
            nextpc = rs1 + inst.imm;
            break;
        
        case loadUpper:
            regwrite(inst.rd, inst.imm);
            break;

        case loadUPC:
            regwrite(inst.rd, (int)pc + inst.imm);
            break;

        case envcall:
		if (inst.func == ebreak)
		{
			interactive = 1;
			printf("EBREAK @ 0x%08X\n", pc);
		}
		else
		{
			regwrite(10, envop(inst)); /* result left in a0 */
		}
            break;

        default:
            printf("Unimplemented: %s op\n", instnames[inst.func]);
            break;
    }

    return (uint)nextpc;
}

void dumpregs()
{
    printf("Registers:");

    for (int i = 0; i < 32; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("x%02d (%3s) 0x%08X  ", i, regnames[i], reg[i]);
    }

    printf("\n");
}

/* Debugging */
void dumpmem(uint8_t *mem)
{
    uint a;

    printf("Memory dump:\n");
    for (a = 0; a < 0x10; a++)
    {
    /*
        printf("byte u[%08X] = %08X\n", a, memload(mem, a, 1, true));
        printf("byte  [%08X] = %08X\n", a, memload(mem, a, 1, false));
        printf("half u[%08X] = %08X\n", a, memload(mem, a, 2, true));
        printf("half  [%08X] = %08X\n", a, memload(mem, a, 2, false));
        printf("word  [%08X] = %08X\n", a, memload(mem, a, 4, true));
    */
    }  
}

void run(uint startPC, uint initialSP)
{
    char c;
    int done;

    /* Initialize the zero register explicitly */
    reg[0] = 0;
    reg[RA] = 0;
    reg[SP] = initialSP;

    uint pc = startPC;

    printf("Starting simulation @ PC=0x%X, SP=0x%X\n", pc, reg[SP]);

    while (1)
    {
        /* Check for misaligned pc */
        if ((pc & 0x3) != 0)
        {
            fprintf(stderr, "Misaligned access @ PC=0x%08X, aborting\n", pc);
            exit(-1);
        }

        Instruction inst = decode(memload(pc, 4, false));

        if (verbose || debug)
        {
            printf("\nInstruction @ PC=0x%08X: 0x%08X\n", pc, memload(pc, 4, false));

            if (debug)
            {
                printf("    op: 0x%2X  func: %d (%s)", inst.opcode, inst.func, instnames[inst.func]);
                printf("    rd: x%d  rs1: x%d  rs2: x%d\n", inst.rd, inst.rs1, inst.rs2);
                printf("   imm: %d (0x%08X)\n", inst.imm, inst.imm);
            }
        }

        if (breakpoint && (breakAddr == pc))
        {
            interactive = 1;
            debug = 1;
            printf("Breakpoint at 0x%08X\n", pc);
        }

        if (interactive)
        {
            done = 0;

            while (!done)
            {
                printf("\n-- More? c)ontinue, d)isable bkpt, r)egs, q)uit: ");
                fflush(stdout);
                
                read(STDIN_FILENO, &c, 1);

                switch (c)
                {
                    case '?':
                    case 'h':
                        printf("Help\n");
                        printf("c - continue execution (no stepping)\n");
                        printf("d - disable the breakpoint if set\n");
                        printf("r - dump the current registers\n");
                        printf("q - quit the program immediately\n");
                        printf("\nPress space or enter to step.\n");
                        break;

                    case 'q':
                        printf("Quit\n");
                        inst.func = exitprog;
                        done = 1;
                        break;

                    case ' ':
                    case '\n':
                        printf("\n");
                        done = 1;
                        break;

                    case 'd':
                        breakpoint = 0;
                        printf("\nBreakpoint disabled.\n");
                        break;

                    case 'r':
                        dumpregs();
                        break;

                    case 'c':
                        interactive = 0;
                        printf("\nContinuing...\n");
                        done = 1;
                        break;

                    default:
                        printf("???\n");
                        break;
                }
            }
        }

        if (inst.func == exitprog)
        {
            /* Dump the stats */
            printf("Execution stopped @ PC=0x%08X\n", pc);
            dumpregs();
            return;
        }

        pc = execute(pc, inst);

        if (verbose && !debug) dumpregs();

    }
}
