# test beq/bne
    li t0, -1		# init pass/fail value
    li a0, 0x01234567
    li a1, 0x76543210
    li a2, 0x01234567
    li a3, 0xffffffff
# beq
    beq a0, a0, L1	# a == b, true
    j fail
L1:
    beq a0, a1, fail	# a != b, false
    beq a0, zero, fail	# a != b, false
    beq a0, a2, L2	# a == b, true
    j fail
L2:
    beq a3, t0, L3	# a == b, true
    j fail
# bne
L3: 
    bne a0, a0, fail	# a == b, false
    bne a1, a0, L4	# a != b, true
    j fail
L4:
    bne a0, a2, fail	# a > b, false
    bne a3, t0, fail	# a == b, false
pass:
    li t0, 0xcafebabe
fail:
    jr zero
