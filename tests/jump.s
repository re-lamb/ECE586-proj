# test jal/jalr
    addi t0, zero, -1   # init pass/fail result
    addi t1, zero, 1
    addi t2, zero, 3
    auipc a0, 0         # am i storing the correct pc?
    jal a1, L0
L0:
    addi a0, a0, 8
    bne a0, a1, fail
    beq a0, a1, L2      # if predicted pc matched
    j fail              # do a forward jump
L1:
    addi t1, t1, 1      # ..and then return. forwards.
    jr ra
L2:
    addi t1, zero, 2    # now give backwards a go..
    jal L1
    bne t1, t2, fail
 
    li a3, 2060         # test largest possible negative imm
    jal a2, L3
L3:
    add a2, a2, a3
    jalr ra, a2, -2048
    j fail
    
    li a3, -2035        # test largest possible positive imm
    jal a2, L4          # also using a half align immediate!
L4:
    add a2, a2, a3
    jalr ra, a2, 2047
    j fail
    
    li a3, -2034        # test that LSB is discarded after 
    jal a2, L5          # addition
L5:
    add a2, a2, a3
    jalr ra, a2, 2047
    j fail
    
pass:
    li t0, 0xcafebabe
fail:
    jr zero
