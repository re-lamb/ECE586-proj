# test env calls

.equ STDIN, 0
.equ STDOUT, 1
.equ SYS_READ, 63
.equ SYS_WRITE, 64
.equ SYS_EXIT, 94

.equ RDLEN, 20

.org 0
.align 2

# read 5 bytes   
    li t0, -1
    li a0, STDIN
    la a1, buffer
    li a2, RDLEN
    li a7, SYS_READ
    sb zero, 0(a1)	# write a zero to make sure it gets stomped
    ecall
    
    bne a0, a2, fail    # did we get n back?
     
# write what we just read
    li a0, STDOUT
    la a1, msg
    lbu a2, len
    li a7, SYS_WRITE
    ecall
    
    bne a0, a2, fail    # did we write 'em all
    
# test sysexit
    li a0, 0xcafebabe
    li a7, SYS_EXIT
    ecall

fail:
    jr zero

# move the data sections

.org 0x2000
.align 2

# output for the write test
msg: .asciz "This is a test"
len: .byte .-msg

# space for the read test
.org 0x3000
.align 2

buffer: .space 256, 0

