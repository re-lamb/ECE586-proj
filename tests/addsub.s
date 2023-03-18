# test add/addi/sub
    li t0, -1
    li a0, 0
    li a1, 1
    li a2, -1
    li a3, 0x55555555
    li a4, 0xaaaaaaaa
    li a5, 0x40000000
    li a6, 0x80000000
    li a7, -2
# add
    add a0, a0, a0        # 0 + 0 = 0
    bne a0, zero, fail
    add a0, a0, a1        # 0 + 1 = 1
    bne a0, a1, fail
    add a0, a1, a2        # 1 + -1 = 0
    bne a0, zero, fail
    add a0, a0, a2        # 0 + -1 = -1
    bne a0, a2, fail
    add a0, a3, a4        # 5 & a = f (-1)
    bne a0, a2, fail
    add a0, a5, a5        # 40000000 + 40000000 = 80000000
    bne a0, a6, fail
    add a0, a6, a6        # 80000000 + 80000000 = 0
    bne a0, zero, fail

    add a0, a6, a7
    add a0, a0, a6        # same + 7ff = 7ff (high bit overflows)
    bne a0, a7, fail
    
    add a0, a4, a4        # add large - + large - = large + (underflow)
    add a0, a0, a1        # + 1 (:-)
    bne a0, a3, fail
    
# addi  
    addi a0, zero, 0        # 0 + 0 = 0
    bne a0, zero, fail
    addi a0, a0, 1        # 0 + 1 = 1
    bne a0, a1, fail
    addi a0, a1, -1        # 1 + -1 = 0
    bne a0, zero, fail
    addi a0, zero, -1        # 0 + -1 = -1
    bne a0, a2, fail
    addi a0, a2, -1        # -1 + -1 = -2
    bne a0, a7, fail
    li a7, 0x00000800
    addi a0, a1, 0x7ff    # 1 + largest +imm = 0x800
    bne a0, a7, fail
    addi a0, a0, -2048    # 0x800 + largest -imm = 0
    bne a0, zero, fail
    
# sub
    sub a0, zero, zero    # 0 - 0 = 0
    bne a0, zero, fail
    sub a0, a1, zero    # 1 - 0 = 1
    bne a0, a1, fail
    sub a0, a1, a1    # 1 - 1 = 0
    bne a0, zero, fail
    sub a0, zero, a1    # 0 - 1 = -1
    bne a0, a2, fail
    sub a0, a2, a2    # -1 - -1 = 0
    bne a0, zero, fail
    li a7, -2
    sub a0, a2, a1    # -1 - 1 = -2
    bne a0, a7, fail
    sub a0, a6, a6    # -biggest - -biggest = 0
    bne a0, zero, fail
    xori a7, a6, -1    # turn -biggest > +biggest
    sub a0, a7, a7    # +biggest - +biggest = 0
    bne a0, zero, fail
    sub a0, a6, a7    # -biggest - +biggest = 1 (underflow)
    bne a0, a1, fail
    sub a0, a7, a6    # +biggest - -biggest = -1 (overflow)
    bne a0, a2, fail
pass:
    li t0, 0xcafebabe
fail:
    jr zero
