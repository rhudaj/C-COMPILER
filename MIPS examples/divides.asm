	divu $2, $1	
	add $3, $0, $0
	mfhi $4	
	bne $4, $0, jump
	lis $3
	.word 1
jump:   
	jr $31
