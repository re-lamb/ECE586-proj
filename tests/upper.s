# test lui/auipc
    addi t0, zero, -1   # init pass/fail result
    
    addi a1, zero, 1    # make a 1
    slli a2, a1, 12     # 0x00001000
    slli a3, a1, 31     # 0x80000000
    mv a4, t0
    slli a4, a4, 12     # 0xfffff000
    slli a5, a4, 1      # 0xffffe000
    srli a5, a5, 1      # 0x7ffff000

# test lui
    lui a0, 0
    bne a0, zero, fail
    lui a0, 1
    bne a0, a2, fail
    lui a0, 0x80000
    bne a0, a3, fail
    lui a0, 0xfffff
    bne a0, a4, fail
    lui a0, 0x7ffff
    bne a0, a5, fail
    
    auipc a0, 1        # add 0x1000 to current pc
    sub a0, a0, a2     # sub that off again
    addi a0, a0, 20    # now jump based on the expected pc
    jr a0
    j fail
    
    add a4, a2, a2    # test again at 0x2000
    auipc a0, 2
    sub a0, a0, a4
    addi a0, a0, 20
    jr a0
    j fail
    
    auipc a0, 0xfffff    # should be 0xfffff<pc>
    add a0, a0, a2        # recover pc
    addi a0, a0, 20
    jr a0
    j fail

    auipc a0, 0x7ffff    # add largest positive offset
    sub a0, a0, a5        # recover pc
    addi a0, a0, 20
    jr a0
    j fail
    
    auipc a0, 0x80000    # add most negatory offset
    add a0, a0, a3
    addi a0, a0, 20
    jr a0
    j fail
    
pass:
    li t0, 0xcafebabe
fail:
    jr zero
