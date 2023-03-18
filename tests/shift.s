.local L3

# test sll/i
    li t0, -1           # init pass/fail result
    li a1, 0x00000001   # test vector
    li a2, 0x00000001   
    li a3, 0x00100000   # immediate increment
    li a4, 0x00061513   # slli a0, a2, 0 instruction
    li a5, 0x94		# set address of the instruction (L3)

# shift left
    li t1, 32           # test shift amount mask 32 -> 0 
    sll a0, a2, t1
    bne a0, a1, fail
    li t1, 0
L1:
    sll a0, a2, t1      # shift by t1 bits
    bne a0, a1, fail    # compare against expected
    add a1, a1, a1
    addi t1, t1, 1
    slti t2, t1, 32     # any bits left
    bne t2, zero, L1
    li t1, 0
    li a2, -2
    li a1, -2     
L2:
    sll a0, a2, t1      # do it again, with a negative
    bne a0, a1, fail
    add a1, a1, a1
    addi t1, t1, 1
    slti t2, t1, 32 
    bne t2, zero, L2 
    li a1, 0x55555555
    li a2, 0xaaaaaaaa
    li t1, 1
    sll a1, a1, t1
    bne a1, a2, fail
# shift left imm
    li t1, 0
    li a2, 1
    li a1, 1
L3:
    slli a0, a2, 0      # shift by n bits ** THIS INSTRUCTION IS MODIFIED **
    bne a0, a1, fail    # compare against expected
    add a1, a1, a1      # shift the test vector
    addi t1, t1, 1
    add a4, a4, a3      # increment the immediate
    sw a4, 0(a5)        # store the modified instruction
    slti t2, t1, 32     # any bits left?
    bne t2, zero, L3
    li t1, 0
    li a2, -2
    addi a1, a2, 0      
# test srl/a
    li t1, 31
    li t2, 0
    li a1, 1
    li t3, 1
    li a2, 0x80000000
L4:
    srl a0, a2, t2
    sra a6, a2, t2
    sll a1, t3, t1
    sll a7, t0, t1
    bne a0, a1, fail
    bne a6, a7, fail
    addi t1, t1, -1
    addi t2, t2, 1
    bne t1, t0, L4
    
    li a1, 0x55555555    
    li a2, 0xaaaaaaaa    
    li t1, 1
    li t2, 0x80000000
    srl a0, a2, t1      # test that no bits are swapped
    bne a0, a1, fail    # with zero fill
    sra a0, a2, t1      # and test sign extend
    xor a0, a0, a1
    bne a0, t2, fail
  
# test srai    
    li a4, 0x40065813   # srai a6, a2, 0 instruction
    li t4, 0x140        # instruction to modify (L5)
    li t1, 31           # 
    li t2, 0
    li a1, 1
    li t3, 1
    li a2, 0x80000000
L5:
    srai a6, a2, 0
    sll a7, t0, t1
    bne a6, a7, fail
    add a4, a4, a3      # increment the immediate
    sw a4, 0(t4)        # store the modified instruction
    addi t1, t1, -1
    addi t2, t2, 1
    bne t1, t0, L5
    
    li a1, 0x55555555   # test srai with sign extend 
    li a2, 0xaaaaaaaa   # against a pattern
    li t2, 0x80000000   # and then without
    srai a0, a2, 1   
    xor a0, a0, a1    
    bne a0, t2, fail
    srai a0, a1, 1
    li a1, 0x2aaaaaaa
    bne a0, a1, fail 
pass:
    li t0, 0xcafebabe
fail:
    jr zero
