#
# ECE 568 Winter 2023 - Final project
# R.E. Lamb
#

# risc-v toolchain for ece581 homework 3

RISCV=/pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain
GCC=$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc
OBJDUMP=$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump
AS=$(RISCV)/bin/riscv64-unknown-linux-gnu-as

TESTPROG=prog

codegen:
	$(GCC) -fpic -march=rv32i -mabi=ilp32 -S $(TESTPROG).c
	$(AS) -ahld $(TESTPROG).s -o $(TESTPROG).o
	$(OBJDUMP) -d $(TESTPROG).o | grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $(TESTPROG).mem

