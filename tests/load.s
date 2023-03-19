.equ base, 0x3000

# test lw
    addi t0, zero, -1     # init pass/fail result
    li s0, base

    
    addi a0, zero, 0      # zero some mem
    sw a0, -4(s0)
    sw a0, 0(s0)
    sw a0, 4(s0)
    
    lw a1, -4(s0)         # make sure mem is zero'd
    bne a1, zero, fail    
    lw a1, 0(s0)
    bne a1, zero, fail
    lw a1, 4(s0)
    bne a1, zero, fail
    
    li a0, 0xdeadbeef     # store test vector
    sw a0, 0(s0)
    lw a1, -4(s0)
    bne a1, zero, fail    # make sure not to clobber neighbors
    lw a1, 0(s0)
    bne a1, a0, fail      # did we get our word back?
    lw a1, 4(s0)
    bne a1, zero, fail
    
    slli a2, a0, 8        # check misaligned loads (neg)
    lw a1, -1(s0)
    bne a1, a2, fail
    
    slli a2, a0, 16
    lw a1, -2(s0)
    bne a1, a2, fail
    
    slli a2, a0, 24
    lw a1, -3(s0)
    bne a1, a2, fail

    srli a2, a0, 8        # check misaligned loads (pos)
    lw a1, 1(s0)
    bne a1, a2, fail
    
    srli a2, a0, 16
    lw a1, 2(s0)
    bne a1, a2, fail
    
    srli a2, a0, 24
    lw a1, 3(s0)
    bne a1, a2, fail
    
    addi s1, s0, -2048    # test largest negative load offset
    sw a0, 0(s1)
    lw a1, -2048(s0)
    bne a0, a1, fail
    
    addi s1, s0, 2047     # test largest positive load offset
    sw a0, 0(s1)
    lw a1, 2047(s0)
    bne a0, a1, fail
    
# lh
    li a3, 0xffffbeef    # lh, aligned, with sign-ext
    lh a1, 0(s0)
    bne a1, a3, fail
    
    li a3, 0xffffadbe    # lh, mis-aligned with sign ext
    lh a1, 1(s0) 
    bne a1, a3, fail
    
    li a2, 0x0dad
    li a3, 0xf00d0dad    # lh, now without sign ext
    sw a3, 0(s0)
    
    lh a1, 0(s0)
    bne a1, a2, fail  
    
    li a2, 0x0d0d        # lh, mis-aligned without sign ext
    lh a1, 1(s0)  
    bne a1, a2, fail
    
# lhu 
    sw a0, 0(s0)         
    
    li a3, 0x0000beef    # lhu, aligned, 1 msb
    lhu a1, 0(s0)
    bne a1, a3, fail
    
    li a3, 0x0000adbe    # lhu, mis-aligned, 1 msb
    lhu a1, 1(s0) 
    bne a1, a3, fail
    
    li a2, 0x0dad
    li a3, 0xf00d0dad    # lhu, 0 msb
    sw a3, 0(s0)
    
    lhu a1, 0(s0)
    bne a1, a2, fail  
    
    li a2, 0x0d0d        # lhu, mis-aligned, 0 msb
    lhu a1, 1(s0)  
    bne a1, a2, fail
    
# lb
    sw a0, 0(s0) 

    li a3, 0xffffffef    # lb, aligned, 1 msb
    lb a1, 0(s0)
    bne a1, a3, fail
    
    li a3, 0xffffffbe    # lb, mis-aligned 1, 1 msb
    lb a1, 1(s0) 
    bne a1, a3, fail
    
    li a3, 0xffffffad    # lb, mis-aligned 2, 1 msb
    lb a1, 2(s0) 
    bne a1, a3, fail
 
    li a3, 0xffffffde    # lb, mis-aligned 3, 1 msb
    lb a1, 3(s0) 
    bne a1, a3, fail
    
    li a3, 0xf00d0dad    # lb, now without sign ext
    sw a3, 0(s0)
    
    li a2, 0x0000000d    # lb, 0 msb, odd offset
    lb a1, 1(s0)
    bne a1, a2, fail  
           
    lb a1, 2(s0)         # lb, 0 msb, even offset
    bne a1, a2, fail
    
# lbu 
    sw a0, 0(s0)         
    
    li a3, 0xef          # lbu, aligned, 1 msb, unsigned
    lbu a1, 0(s0)
    bne a1, a3, fail
    
    li a3, 0xbe          # lbu, mis-aligned, 1 msb
    lbu a1, 1(s0) 
    bne a1, a3, fail
    
    li a2, 0x0d
    li a3, 0xf00d0dad    # lbu, 0 msb
    sw a3, 0(s0)
    
    lbu a1, 1(s0)        # lbu, 0 msb, odd offset
    bne a1, a2, fail  
          
    lbu a1, 2(s0)        # lbu, 0 msb, even offset
    bne a1, a2, fail
    
    
    
pass:
    li t0, 0xcafebabe
fail:
    jr zero
