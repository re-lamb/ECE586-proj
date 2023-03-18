# test slt/i/u
    li t0, -1       # init pass/fail result
    li a7, 0        # result reg
    li a0, 0x00000081
    li a1, 0x00000080
    li a2, 0xfffff801
    li a3, 0xfffff800
# slt
    slt a7, a1, a0      # a < b, true    (+operands)
    beq a7, zero, fail
    slt a7, a0, a1      # a > b, false
    bne a7, zero, fail
    slt a7, a0, a0      # a = b, false
    bne a7, zero, fail
    slt a7, a3, a2      # a < b, true     (-operands)
    beq a7, zero, fail
    slt a7, a2, a3      # a > b, false
    bne a7, zero, fail
    slt a7, a2, a2      # a = b, false
    bne a7, zero, fail
    slt a7, a2, a1      # a < b, true    (-/+)
    beq a7, zero, fail
    slt a7, a1, a2      # a > b, false    (+/-)
    bne a7, zero, fail
    
# sltu    
    sltu a7, a1, a0    # a < b, true
    beq a7, zero, fail
    sltu a7, a0, a1    # a > b, false
    bne a7, zero, fail
    sltu a7, a0, a0    # a = b, false
    bne a7, zero, fail
    sltu a7, a3, a2    # a < b, true
    beq a7, zero, fail
    sltu a7, a2, a3    # a > b, false
    bne a7, zero, fail
    sltu a7, a2, a2    # a = b, false
    bne a7, zero, fail
    sltu a7, a1, a2    # a < b, true
    beq a7, zero, fail
    sltu a7, a2, a1    # a > b, false
    bne a7, zero, fail
    
# slti
    slti a7, a1, 0x81     # a < b, true    (+operands)
    beq a7, zero, fail
    slti a7, a0, 0x80     # a > b, false
    bne a7, zero, fail
    slti a7, a0, 0x81     # a = b, false
    bne a7, zero, fail
    slti a7, a3, 0xfffff801     # a < b, true     (-operands)
    beq a7, zero, fail
    slti a7, a2, 0xfffff800     # a > b, false
    bne a7, zero, fail
    slti a7, a2, 0xfffff800     # a = b, false
    bne a7, zero, fail
    slti a7, a2, 1      # a < b, true    (-/+)
    beq a7, zero, fail
    slti a7, a1, -1      # a > b, false    (+/-)
    bne a7, zero, fail
    
# sltiu    
    sltiu a7, a1, 0x7ff    # a < b, true
    beq a7, zero, fail
    sltiu a7, a0, 0x7f    # a > b, false
    bne a7, zero, fail
    sltiu a7, a0, 0x80    # a = b, false
    bne a7, zero, fail
    sltiu a7, a3, 0xfffff801    # a < b, true
    beq a7, zero, fail
    sltiu a7, a2, 0xfffff800    # a > b, false
    bne a7, zero, fail
    sltiu a7, a2, 0xfffff800    # a = b, false
    bne a7, zero, fail
    sltiu a7, a1, -1    # a < b, true
    beq a7, zero, fail
    sltiu a7, a2, 1    # a > b, true
    bne a7, zero, fail
        
pass:
    li t0, 0xcafebabe
fail:
    jr zero
