# test and/or/xor
    li t0, -1
    li a0, 0
    li a1, -1
    li a2, 0x55555555
    li a3, 0xaaaaaaaa
    li a4, 0x00000fff
    li a5, 0x000007ff
    li a6, 1
    li a7, 0x00000800
# and
    and a0, a0, a0        # 0 & 0 = 0
    bne a0, zero, fail
    and a0, a0, a1        # 0 & 1 = 0
    bne a0, zero, fail
    and a0, a1, a1        # 1 & 1 = 1
    bne a0, a1, fail      # a0 should contain -1
    and a0, a2, a3        # 5 & a = 0
    bne a0, zero, fail
# andi  
    andi a0, a0, 0        # 0 & 0 = 0
    bne a0, zero, fail
    andi a0, a0, 1        # 0 & 1 = 0
    bne a0, zero, fail
    andi a0, a1, 1        # -1 & 1 = 1
    bne a0, a6, fail      # a6 should contain 1
# test immediate extents and sign ext.
    andi a0, a4, 0xfffff800    # a0 should contain a7 pattern
    bne a0, a7, fail
    andi a0, a7, 0x7ff    # a0 should contain 0
    bne a0, zero, fail
    andi a0, a5, 0x7ff    # a0 should contain 7ff
    bne a0, a5, fail
# or
    or a0, zero, zero    # 0 | 0 = 0
    bne a0, zero, fail
    or a0, a1, zero    # 0 | 1s = 1
    bne a0, a1, fail
    or a0, a2, a3     # 5 | a = f
    bne a0, a1, fail
    or a0, a1, a1    # 1 | 1 = 1
    bne a0, a1, fail
# ori
    ori a0, a7, 0x7ff    # 800 | 7ff = fff
    bne a0, a4, fail
    ori a0, a5, 0xfffff800    # 7ff | 800 (sign ext) = -1
    bne a0, a1, fail
    ori a0, a2, 0     # anything | 0 = anything
    bne a0, a2, fail
    li a6, 0x00000aaa
    ori a0, a6, 0x555    # 555 | aaa = fff
    bne a0, a4, fail
# xor
    xor a0, zero, zero    # 0 ^ 0 = 0
    bne a0, zero, fail
    xor a0, a1, a1        # 1s ^ 1s = 0
    bne a0, zero, fail
    xor a0, a2, a2        # 5s ^ 5s = 0
    bne a0, zero, fail
    xor a0, a2, a3        # 5s ^ as = 1s
    bne a0, a1, fail
# xori
    xori a0, zero, 0    # 0 ^ 0 = 0
    bne a0, zero, fail
    xori a0, a1, -1    # 1s ^ 1s = 0
    bne a0, zero, fail
    xori a0, a5, 0x7ff    # 7ff ^ 7ff (imm) = 0
    bne a0, zero, fail
    xori a0, a5, 0xfffff800    # 7ff ^ 800 = -1
    bne a0, a1, fail
    xori a0, a6, 0x555    # aaa ^ 555 = fff
    bne a0, a4, fail
    xori a0, a4, 0x555    # fff ^ 555 = aaa
    bne a0, a6, fail
    li a6, 0x55555000
    xori a0, a3, 0xfffffaaa    # as ^ aaa (imm) = 55555000
    bne a0, a6, fail
pass:
    li t0, 0xcafebabe
fail:
    jr zero
