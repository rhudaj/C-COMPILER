; variables

lis $10						; input address
.word 0xFFFF0004

lis $11						; output address
.word 0xFFFF000C

add $3, $0, $0		; byte count

lis $20						; end of input value
.word -1

lis $21						; constant (1)
.word 1

lis $22 					; constant (4)
.word 4

inputLoop:

	lw $1, 0($10) 	

	beq $1, $20, inputEnd

	add $3, $3, $21

	sub $30, $30, $22 	

	sw $1, 0($30)

	beq $0, $0, inputLoop

inputEnd:

	add $2, $3, $0

outputLoop: 

	lw $1, 0($30)

	add $30, $30, $22 

	sw $1, 0($11)

	sub $2, $2, $21

	bne $2, $0, outputLoop
end:

	jr $31
