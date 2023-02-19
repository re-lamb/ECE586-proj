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

typedef enum instnum
{
    add,    sub,    xor,    or,     and,    sll,    srl,    sra,
    slt,    sltu,   addi,   xori,   ori,    andi,   slli,   srli,
    srai,   slti,   sltiu,  lb,     lh,     lw,     lbu,    lhu,
    sb,     sh,     sw,     beq,    bne,    blt,    bge,    bltu,
    bgeu,   jal,    jalr,   lui,    auipc,  ecall,  ebreak,
    unknown
} InstNum;

char *instnames[] = {
    "add",  "sub",  "xor",  "or",   "and",  "sll",  "srl",  "sra",
    "slt",  "sltu", "addi", "xori", "ori",  "andi", "slli", "srli",
    "srai", "slti", "sltiu","lb",   "lh",   "lw",   "lbu",  "lhu",
    "sb",   "sh",   "sw",   "beq",  "bne",  "blt",  "bge",  "bltu",
    "bgeu", "jal",  "jalr", "lui",  "auipc","ecall","ebreak",
    "unknown"
};

typedef struct decodedInst
{
    InstNum op;
    int rd;
    int rs1;
    int rs2;
    int imm;
} Instruction;

Instruction decode(uint value)
{
    Instruction this;

    int tmp;    /* for assembling the immediate field */

    int format = value & 0x7f;
    int funct3 = (value >> 12) & 0x7;
    int funct7 = (value >> 25) & 0xfe;

    this.op = unknown;
    this.rd =  (value & 0x0000f80) >> 7;
    this.rs1 = (value & 0x00f8000) >> 15;
    this.rs2 = (value & 0x1f00000) >> 20;
    this.imm = 0;

    switch (format)
    {
        case 0x33:
            // R format
            switch (funct3)
            {
                case 0:
                    this.op = (funct7 == 0x20) ? sub :
                              (funct7 == 0x00) ? add : unknown;
                    break;

                case 1:
                    this.op = sll;
                    break;

                case 2:
                    this.op = slt;
                    break;

                case 3:
                    this.op = sltu;
                    break;

                case 4:
                    this.op = xor;
                    break;

                case 5:
                    this.op = (funct7 == 0x20) ? sra :
                              (funct7 == 0x00) ? srl : unknown;
                    break;

                case 6:
                    this.op = or;
                    break;

                case 7:
                    this.op = and;
                    break;
            }
            break;


        case 0x13:
            // I format (ALU ops)
            printf("I format: value is %d : 0x%08X\n", value, value);
            this.imm = (value & 0xfff00000) >> 20;
            if (value & 0x80000000)
            {
                this.imm |= 0xfffff000;
            }
            printf("I format: imm12 is %d : 0x%08X\n", this.imm, this.imm);

            switch (funct3)
            {
                case 0:
                    this.op = addi;
                    break;

                case 1:
                    this.op = slli;
                    break;

                case 2:
                    this.op = slti;
                    // todo: bits 11:5 must be 0?
                    break;

                case 3:
                    this.op = sltiu;
                    break;

                case 4:
                    this.op = xori;
                    break;

                case 5:
                    /* NB: imm[11:5] are same bits as funct7! */
                    this.op = (funct7 == 0x20) ? srai :
                              (funct7 == 0x00) ? srli : unknown;
                    break;

                case 6:
                    this.op = ori;
                    break;

                case 7:
                    this.op = andi;
                    break;
            }  
            break;

        case 0x03:
            // I format (loads)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & 0x80000000) this.imm |= 0xfffff000;  /* sign extended */

            switch (funct3)
            {
                case 0:
                    this.op = lb;
                    break;

                case 1:
                    this.op = lh;
                    break;

                case 2:
                    this.op = lw;
                    break;

                case 4:
                    this.op = lbu;
                    break;

                case 5:
                    this.op = lhu;
                    break;
            }
            break;

        case 0x67:
            // I format (jalr)
            if (funct3 == 0)
                this.op = jalr;
            break;

        case 0x73:
            // I format (ecall/ebreak)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & 0x80000000) this.imm |= 0xfffff000;  /* sign extended */

            if (funct3 == 0)
                this.op = (this.imm == 1) ? ebreak :
                          (this.imm == 0) ? ecall : unknown;
            break;

        case 0x23:
            // S format (stores)
            tmp = (value & 0xfe000000) >> 20;   // bits 31:25
            if (value & 0x80000000) tmp |= 0xfffff000;
            this.imm = tmp | this.rd;           // bits 11:7

            switch (funct3)
            {
                case 0:
                    this.op = sb;
                    break;

                case 1:
                    this.op = sh;
                    break;

                case 2:
                    this.op = sw;
                    break;
            }
            break;

        case 0x63:
            // B format (branches)
            tmp = (value & 0x80000000) ? 0xfffff000 : 0;
            this.imm = tmp | ((value & 0x80) << 4 |
                              (value & 0x7e000000) >> 20 |
                              (value & 0xf00) >> 7);

            switch (funct3)
            {
                case 0:
                    this.op = beq;
                    break;

                case 1:
                    this.op = bne;
                    break;

                case 4:
                    this.op = blt;
                    break;

                case 5:
                    this.op = bge;
                    break;

                case 6:
                    this.op = bltu;
                    break;

                case 7:
                    this.op = bgeu;
                    break;
            }
            break;

        case 0x6f:
            // J format (jump and link)
            tmp = (value & 0x80000000) ? 0xfff00000 : 0;
            this.imm = tmp | ((value & 0x7fe00000) >> 20 |
                              (value & 0x00100000) >> 9 |
                              (value & 0x000ff000));

            if (funct3 == 0)
                this.op = jal;    
            break;

        case 0x17:
        case 0x37:
            // U format
            this.imm = (value & 0xfffff000);
            this.op = (format == 0x37) ? lui : auipc;
            break;

        default:
            fprintf(stderr, "Unimplemented opcode 0x%02X\n", format);
            exit(-1);
    }

    if (this.op == unknown)
    {
        fprintf(stderr, "Unknown instruction 0x%08X!\n", value);
        exit(-1);
    }
    return this;
}

/* Debugging */
void dumpmem(uint8_t *mem)
{
    uint a;

    printf("Memory dump:\n");
    for (a = 0; a < 0x10; a++)
    {
        printf("byte u[%08X] = %08X\n", a, memload(mem, a, 1, true));
        printf("byte  [%08X] = %08X\n", a, memload(mem, a, 1, false));
        printf("half u[%08X] = %08X\n", a, memload(mem, a, 2, true));
        printf("half  [%08X] = %08X\n", a, memload(mem, a, 2, false));
        printf("word  [%08X] = %08X\n", a, memload(mem, a, 4, true));
    }  
}

void run(uint pc, uint sp, uint8_t *mem)
{
    printf("Starting simulation at pc=0x%X, sp=0x%X\n", pc, sp);

    for (; pc < 0xf; pc += 4)
    {
        Instruction inst = decode(memload(mem, pc, 4, false));

        printf("Instruction at 0x%08X: 0x%08X\n", pc, memload(mem, pc, 4, false));
        printf("    op: %d (%s)", inst.op, instnames[inst.op]);
        printf("    rd: %d  rs1: %d  rs2: %d\n", inst.rd, inst.rs1, inst.rs2);
        printf("   imm: %d (0x%08X)\n", inst.imm, inst.imm);
    }
    /*
    inst = memload(mem, pc, 4, false);

    decode(inst);

    sw

    */
}
