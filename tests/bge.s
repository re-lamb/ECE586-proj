# test bge/u
    li t0, -1		# pass/fail result
    li a0, 0x00008001
    li a1, 0x00008000
    li a2, 0xffff8001
    li a3, 0xffff8000
# bge
    bge a0, a1, L1	# a > b, true (same MSB)
    j fail
L1:
    bge a1, a0, fail	# a < b, false (same MSB)
    bge a0, a0, L2	# a = b, false (same MSB)
    j fail
L2:
    bge a3, a2, fail	# a < b, false (same MSB, negative #s)
    bge a2, a3, L3	# a > b, true
    j fail
L3: 
    bge a2, a2, L4	# a = b, true
    j fail
L4:
    bge a2, a0, fail	# a < b, false (diff MSB)
    bge a0, a2, L5	# a > b, true
    j fail

# bgeu    
L5:
    bgeu a0, a1, L6	# a > b, true (same MSB)
    j fail
L6:
    bgeu a1, a0, fail	# a < b, false (same MSB)
    bgeu a0, a0, L7	# a = b, false (same MSB)
    j fail
L7:
    bgeu a3, a2, fail	# a < b, false (same MSB, negative #s)
    bgeu a2, a3, L8	# a > b, true
    j fail
L8: 
    bgeu a2, a2, L9	# a = b, true
    j fail
L9:
    bgeu a0, a2, fail	# a < b, false (diff MSB)
    bgeu a2, a0, pass	# a > b, true
    j fail
pass:
    li t0, 0xcafebabe
fail:
    jr zero
