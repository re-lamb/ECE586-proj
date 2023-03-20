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

#
# Build the simulator
#
sim: $(OBJS) $(INCLUDES)
	$(CC) -o $(PROG) $(OBJS)

tests: FORCE
	@cd $(TESTDIR) && $(MAKE) all

#
# Generate .mem files from a C function
#
TESTPROG=program

testgen: $(TESTPROG).c
	$(RVGCC) -fpic -march=rv32i -mabi=ilp32 -S $(TESTPROG).c
	$(RVAS) -ahld $(TESTPROG).s -o $(TESTPROG).o
	$(RVOBJDUMP) -d $(TESTPROG).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTPROG).mem

#
# Generate .mem file from an assembler .s file
#
TESTDIR=tests
TESTS=addsub beq bge blt logical shift slt upper load store jump

# this one is special (requires an input file)
ecall:
	@echo ""
	@echo -n "Running test '"$(@F)"'..."
	@./$(PROG) -d $(TESTDIR)/ecall.mem < tests/ecall.input >> test.log || echo "ok"

$(TESTS):
	@echo ""
	@echo -n "Running test '"$(@F)"'..."
	@./$(PROG) -d $(TESTDIR)/$(@F).mem >> test.log && echo "ok"

# build the sim and tests
all: sim tests

# run the tests (ecall is special)
check:  $(TESTS) ecall

# clean up
clean:
	@rm -f $(OBJS) $(PROG) $(TESTPROG) test.log
	@cd $(TESTDIR) && $(MAKE) clean

FORCE:

