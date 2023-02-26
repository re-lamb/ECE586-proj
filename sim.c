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

char *instnames[] = {
    "add",  "sub",  "xor",  "or",   "and",  "sll",  "srl",  "sra",
    "slt",  "sltu", "addi", "xori", "ori",  "andi", "slli", "srli",
    "srai", "slti", "sltiu","lb",   "lh",   "lw",   "lbu",  "lhu",
    "sb",   "sh",   "sw",   "beq",  "bne",  "blt",  "bge",  "bltu",
    "bgeu", "jal",  "jalr", "lui",  "auipc","ecall","ebreak",
    "unknown"
};


int reg[32];

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
            if (value & 0x80000000) this.imm |= 0xfffff000;
            
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
            if (value & 0x80000000) this.imm |= 0xfffff000;  /* sign extended */

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
            if (funct3 == 0)
                this.func = jalr;
            break;

        case envcall:
            // I format (ecall/ebreak)
            this.imm = (value & 0xfff00000) >> 20;
            if (value & 0x80000000) this.imm |= 0xfffff000;  /* sign extended */

            if (funct3 == 0)
                this.func = (this.imm == 1) ? ebreak :
                            (this.imm == 0) ? ecall : unknown;
            break;

        case store:
            // S format (stores)
            tmp = (value & 0xfe000000) >> 20;   // bits 31:25
            if (value & 0x80000000) tmp |= 0xfffff000;
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
            tmp = (value & 0x80000000) ? 0xfffff000 : 0;
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
            tmp = (value & 0x80000000) ? 0xfff00000 : 0;
            this.imm = tmp | ((value & 0x7fe00000) >> 20 |
                              (value & 0x00100000) >> 9 |
                              (value & 0x000ff000));

            if (funct3 == 0)
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

void execute(Instruction inst)
{
    switch (inst.opcode)
    {
        case aluRtype: 
            reg[inst.rd] = aluop(reg[inst.rs1], reg[inst.rs2], inst.func);
            break;
        
        case aluItype: 
            reg[inst.rd] = aluop(reg[inst.rs1], inst.imm, inst.func);
            break;

        case load:
            reg[inst.rd] = loadop(reg[inst.rs1], inst.imm, inst.func);
            break;

        case store:
            storeop(reg[inst.rs1], inst.imm, inst.func, reg[inst.rs2]);
            break;

        default:
            printf("Unimplemented: %s op\n", instnames[inst.func]);
            break;
    }
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

void run(uint pc, uint sp /*, uint8_t *mem */)
{
    printf("Starting simulation at pc=0x%X, sp=0x%X\n", pc, sp);

    for (; pc < 0xff; pc += 4)
    {
        Instruction inst = decode(memload(/*mem, */ pc, 4, false));

        printf("Instruction at 0x%08X: 0x%08X\n", pc, memload(/*mem, */ pc, 4, false));
        printf("    op: 0x%2X  func: %d (%s)", inst.opcode, inst.func, instnames[inst.func]);
        printf("    rd: %s=%d  rs1: %s=%d  rs2: %s=%d\n", regnames[inst.rd], reg[inst.rd],
                                                          regnames[inst.rs1], reg[inst.rs1],
                                                          regnames[inst.rs2], reg[inst.rs2]);
        printf("   imm: %d (0x%08X)\n", inst.imm, inst.imm);

        execute(inst);
    }
    /*
    inst = memload(mem, pc, 4, false);

    decode(inst);

    sw

    */
}
