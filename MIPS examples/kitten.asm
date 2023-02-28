	lis $1
input:	.word 0xFFFF0004

	lis $2
output:	.word 0xFFFF000C

	lis $4
	.word -1
	
bytes:	add $3, $0, $0
	
	lis $6
	.word 1
loop:
	lw $7, 0($1) 		; load from input
	beq $7, $4, endloop	; if no input, exit 
	add $3, $3, $6		; increment byte count
	sw $7, 0($2)		; send to output 
	bne $7, $4, loop

endloop:
	jr $31
