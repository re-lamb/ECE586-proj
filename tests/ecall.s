.equ SYS_READ 63
.equ SYS_WRITE 64
.equ SYS_EXIT 94

msg: .ascii "This is a test"
len: .byte .-msg

# read 5 bytes   
    li t0, -1
    li a0, 0
    li a1, 0x1000    # inside the base 64K
#    mv a1, sp        # break it on purpose
    li a2, 5
    li a7, SYS_READ
    sb zero, 0(a1)
    ecall
    
    bne a0, a2, fail    # did we get 5 back?
     
# write what we just read
    li a0, 1
    li a7, SYS_WRITE
    ecall
    
    bne a0, a2, fail    # formality
    
# test sysexit
    li a0, 0xcafebabe
    li a7, SYS_EXIT
    ecall

fail:
    jr zero