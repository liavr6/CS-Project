	add $t2, $zero, $imm, 1
	jal $ra, $imm, $zero, fib
	add $t2, $zero, $imm, 1
fib:
	jal $ra, $imm, $zero, da
da:
	add $t2, $zero, $imm, 1