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
SOURCES=main.c sim.c mem.c alu.c
INCLUDES=rvdefs.h
OBJS=main.o sim.o mem.o alu.o

CC=gcc -Wall

sim: $(OBJS) $(INCLUDES)
	$(CC) -o $(PROG) $(OBJS)

test:
	@echo "todo: run the test(s) through the simulator"

#
# Generate .mem files from the various test programs
#
TESTPROG=prog

testgen:
	$(RVGCC) -fpic -march=rv32i -mabi=ilp32 -S $(TESTPROG).c
	$(RVAS) -ahld $(TESTPROG).s -o $(TESTPROG).o
	$(RVOBJDUMP) -d $(TESTPROG).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTPROG).mem


# Clean up
clean:
	@rm -f $(OBJS) $(PROG)
