# test blt/u
    li t0, -1		# init pass/fail result
    li a0, 0x00008001
    li a1, 0x00008000
    li a2, 0xffff8001
    li a3, 0xffff8000
# blt
    blt a1, a0, L1    	# a < b, true
    j fail
L1:
    blt a0, a1, fail	# a > b, false
    blt a0, a0, fail	# a = b, false
    blt a3, a2, L2	# a < b, true
    j fail
L2:
    blt a2, a3, fail	# a > b, false
    blt a2, a2, fail	# a = b, false
    blt a2, a1, L3	# a < b, true
    j fail
L3: 
    blt a1, a2, fail	# a > b, false
    
# bltu    
    bltu a1, a0, L4	# a < b, true
    j fail
L4:
    bltu a0, a1, fail	# a > b, false
    bltu a0, a0, fail	# a = b, false
    bltu a3, a2, L5	# a < b, true
    j fail
L5:
    bltu a2, a3, fail	# a > b, false
    bltu a2, a2, fail	# a = b, false
    bltu a1, a2, L6	# a < b, true
    j fail
L6: 
    bltu a2, a1, fail	# a > b, false
pass:
    li t0, 0xcafebabe
fail:
    jr zero
