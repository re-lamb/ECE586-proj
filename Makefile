#
# ECE 586 Winter 2023 - Final project
# R.E. Lamb
#

# RISC-V toolchain

RISCV=/pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain
RVGCC=$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc
RVOBJDUMP=$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump
RVAS=$(RISCV)/bin/riscv64-unknown-linux-gnu-as

# Sources for the simulator

PROG=rvsim
SOURCES=main.c sim.c mem.c alu.c env.c
INCLUDES=rvdefs.h
OBJS=main.o sim.o mem.o alu.o env.o

CC=gcc -Wall

sim: $(OBJS) $(INCLUDES)
	$(CC) -o $(PROG) $(OBJS)

# Generate .mem files from a C function
TESTPROG=prog

# Generate .mem file from an assembler .s file
TESTDIR=tests
TESTS=addsub beq bge blt logical shift slt upper load store

ASFLAGS=-ahld -fpic -march=rv32i -mabi=ilp32

testgen: $(TESTPROG).c
	$(RVGCC) -fpic -march=rv32i -mabi=ilp32 -S $(TESTPROG).c
	$(RVAS) -ahld $(TESTPROG).s -o $(TESTPROG).o
	$(RVOBJDUMP) -d $(TESTPROG).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTPROG).mem

ecall:
	$(RVGCC) -fpic -march=rv32i -mabi=ilp32 -S $(TESTDIR)/$(@F).s
	$(RVAS) $(ASFLAGS) $(TESTDIR)/$(@F).s -o $(TESTDIR)/$(@F).o
	$(RVOBJDUMP) -d $(TESTDIR)/$(@F).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTDIR)/$(@F).mem

$(TESTS):
	$(RVAS) $(ASFLAGS) $(TESTDIR)/$(@F).s -o $(TESTDIR)/$(@F).o
	$(RVOBJDUMP) -d $(TESTDIR)/$(@F).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTDIR)/$(@F).mem

check:
	@./$(PROG) tests/addsub.mem
	@./$(PROG) tests/beq.mem
	@./$(PROG) tests/bge.mem
	@./$(PROG) tests/blt.mem
	@./$(PROG) tests/logical.mem
	@./$(PROG) tests/shift.mem
	@./$(PROG) tests/slt.mem
	@./$(PROG) tests/upper.mem
	@./$(PROG) tests/load.mem
	@./$(PROG) tests/store.mem

all: sim $(TESTS)

# Clean up
clean:
	@rm -f $(OBJS) $(PROG)

