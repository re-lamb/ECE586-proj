#
# ECE 586 Winter 2023 - Final project
# R.E. Lamb
#
# Makefile for the full test suite
#

# RISC-V toolchain

RISCV=/pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain
RVGCC=$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc
RVOBJDUMP=$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump
RVAS=$(RISCV)/bin/riscv64-unknown-linux-gnu-as

#
# Generate .mem file from an assembler .s file
#
TESTSRC=addsub beq bge blt ecall logical shift slt upper load store jump
TESTS=$(TESTSRC:%=%.mem)

# Assembler flags for building the .s files
ASFLAGS=-fpic -march=rv32i -mabi=ilp32

# Override the default rule to build assembler sources
%.o : %.s
	@$(RVAS) $(ASFLAGS) -ahld=$(@:%.o=%.list) $< -o $@

%.mem : %.o
	@$(RVOBJDUMP) -d $< | \
	grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*' > $@
	@echo wrote $@

all: $(TESTS)

# Clean up
clean:
	@rm -f *.mem *.list *.o

