.equ base, 0x3000

# test sw
    addi t0, zero, -1     # init pass/fail result
    li s0, base
    li a0, 0xaaaaaaaa
    
    sw a0, -4(s0)         # init some mem
    sw a0, 0(s0)
    sw a0, 4(s0)
    
    lw a1, -4(s0)         # make sure mem is initialized
    bne a1, a0, fail    
    lw a1, 0(s0)
    bne a1, a0, fail
    lw a1, 4(s0)
    bne a1, a0, fail
    
    li a2, 0xdeadbeef     # store test vector
    sw a2, 0(s0)

    lw a1, -4(s0)         # make sure not to clobber neighbors
    bne a1, a0, fail
    lw a1, 0(s0)
    bne a1, a2, fail      # did we get our word back?
    lw a1, 4(s0)
    bne a1, a0, fail
        
    sw a0, 0(s0)
    
    li a3, 0xaadeadbe
    li a4, 0xefaaaaaa            
    sw a2, -1(s0)        # check misaligned stores
    lw a1, -4(s0)        # making sure not to clobber
    bne a1, a4, fail     # neighboring values
    lw a1, 0(s0)
    bne a1, a3, fail
    lw a1, -1(s0)
    bne a1, a2, fail
    
    sw a0, -4(s0)        # re-init some mem
    sw a0, 0(s0)
    sw a0, 4(s0)
    
    li a3, 0xaaaaaade
    li a4, 0xadbeefaa           
    sw a2, 1(s0)        # check misaligned stores
    lw a1, 0(s0)        # making sure not to clobber
    bne a1, a4, fail    # neighboring values
    lw a1, 4(s0)
    bne a1, a3, fail
    lw a1, 1(s0)
    bne a1, a2, fail
    
    addi s1, s0, -2048    # test largest negative store offset
    sw a2, -2048(s0)
    lw a1, 0(s1)
    bne a1, a2, fail
    
    addi s1, s0, 2047     # test largest positive  store offset
    sw a2, 2047(s0)
    lw a1, 0(s1)
    bne a1, a2, fail
    
# test sh
    sw a0, -4(s0)        
    sw a0, 0(s0)        # re-re-init some mem
    sw a0, 4(s0)
    
    li a3, 0xaaaaaaaa
    li a4, 0xaabeefaa
    li a5, 0xaaaabeef           
    sh a2, 1(s0)        # check misaligned stores
    lw a1, 0(s0)        # making sure not to clobber
    bne a1, a4, fail    # neighboring values
    lw a1, 4(s0)
    bne a1, a3, fail
    lw a1, 1(s0)
    bne a1, a5, fail
    lw a1, -4(s0)
    bne a1, a3, fail
         
    sw a0, 0(s0)        # re-re-re-init some mem
    sw a0, 4(s0)
    
    li a4, 0xbeefaaaa          
    sh a2, 2(s0)        # check misaligned stores
    lw a1, 0(s0)        # making sure not to clobber
    bne a1, a4, fail    # neighboring values
    lw a1, 4(s0)
    bne a1, a3, fail
    lw a1, 2(s0)
    bne a1, a5, fail
    lw a1, -4(s0)
    bne a1, a0, fail
          
    sw a0, 0(s0)        # re-re-re-init this should be a function
    sw a0, 4(s0)
    
    li a3, 0xaaaaaabe
    li a4, 0xefaaaaaa         
    sh a2, 3(s0)        # check misaligned stores
    lw a1, 0(s0)        # making sure not to clobber
    bne a1, a4, fail    # neighboring values
    lw a1, 4(s0)
    bne a1, a3, fail
    lw a1, 3(s0)
    bne a1, a5, fail
    lw a1, -4(s0)
    bne a1, a0, fail
    
    addi s1, s0, -2048    # test largest negative store offset
    sw a0, 0(s1)        
    sh a2, -2048(s0)
    lw a1, 0(s1)
    bne a1, a5, fail
    
    addi s1, s0, 2047     # test largest positive  store offset
    sw a0, 0(s1)
    sh a2, 2047(s0)
    lw a1, 0(s1)
    bne a1, a5, fail
    
# test sb
    sw a0, -4(s0)        
    sw a0, 0(s0)        # re-re-init some mem
    sw a0, 4(s0)
    
    li a3, 0xaaaaaaaa    # marching byte
    li a4, 0xaaaaaaef
    li a5, 0xaaaaefaa
    li a6, 0xaaefaaaa
    li a7, 0xefaaaaaa
    sb a2, 0(s0)         # aligned byte store
    lw a1, 0(s0)
    bne a1, a4, fail
    
    sw a0, -4(s0)
    sb a2, -1(s0)       # check misaligned byte stores
    lw a1, -4(s0)        # making sure not to clobber
    bne a1, a7, fail    # neighboring values
    lw a1, 1(s0)
    bne a1, a3, fail
    
    sw a0, -4(s0)
    sb a2, -2(s0)       # check misaligned byte stores
    lw a1, -4(s0)        # making sure not to clobber
    bne a1, a6, fail    # neighboring values
    lw a1, 1(s0)
    bne a1, a3, fail
    
    sw a0, -4(s0)
    sb a2, -3(s0)       # check misaligned byte stores
    lw a1, -4(s0)        # making sure not to clobber
    bne a1, a5, fail    # neighboring values
    lw a1, 1(s0)
    bne a1, a3, fail

    addi s1, s0, -2048    # test largest negative store offset
    sw a0, 0(s1)        
    sb a2, -2048(s0)
    lw a1, 0(s1)
    bne a1, a4, fail
    
    addi s1, s0, 2047     # test largest positive  store offset
    sw a0, 0(s1)
    sb a2, 2047(s0)
    lw a1, 0(s1)
    bne a1, a4, fail
 
pass:
    li t0, 0xcafebabe
fail:
    jr zero
