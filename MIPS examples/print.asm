print:

; print.asm prints the given decimal number (negative or not)
; Registers 
; 	$1 = input number
; 	$2 = input number, divided over and over by powers of 10
;		$3 = quotient of the division of powers of 10 
; 	$4 = divisor (starts at 1'000'000'000)
; 	$5 = isNegative (1 if input is negative)
; 	$10 = constant (1)
; 	$11 = constant (ASCII 0)
; 	$12 = constant (ASCII MINUS)
;		$13 = constant (ASCII NEWLINE)
; 	$14 = constant (OUTPUT ADDRESS)
; 	$15 = constant (MAX (2^32 -1))
; 	$16 = constant (10)
; ………………………………………………………………………………………………………………………………………………………………………………………………
; Saving the register values
sw $1, -4($30)
sw $2, -8($30)
sw $3, -12($30)
sw $4, -16($30)
sw $5, -20($30)
sw $10, -24($30)
sw $11, -28($30)
sw $12, -32($30)
sw $13, -36($30)
sw $14, -40($30)
sw $15, -44($30)
sw $16, -48($30)
; ………………………………………………………………………………………………………………………………………………………………………………………………
lis $10							; one
.word 1
lis $11							; ASCII 0
.word 48		
lis $12							; ASCII_MINUS
.word 45		
lis $13							; ASCII_NEWLINE
.word 0x0A
lis $14							; OUTPUT ADDRESS
.word 0xFFFF000C
lis $15							; MAX (2^32 -1)
.word 4294967295  
lis $16 						; FACTOR (10)
.word 10
; ………………………………………………………………………………………………………………………………………………………………………………………………

lis $4							; DIVISOR (1'000'000'000)
.word 1000000000
add $5, $0, $0			; isNegative
add $3, $0, $0 			; result
add $2, $0, $0 			; number

; ………………………………………………………………………………………………………………………………………………………………………………………………
beq $1, $0, outputZero
; ………………………………………………………………………………………………………………………………………………………………………………………………
add $2, $1, $0   				; number = input
; ………………………………………………………………………………………………………………………………………………………………………………………………
slt $5, $1, $0          ; if (input < 0): isNegative = 1

beq $5, $0, zerosLoop		; if(isNegative == 0) => zerosLoop	

sub $2, $15, $1					; number = (2^32 -1) - input 
add $2, $2, $10					; number += 1

sw $12, 0($14)					; output “-” 
; ………………………………………………………………………………………………………………………………………………………………………………………………

zerosLoop: 	

	div $2, $4		
		mflo $3						; result = quotient(number / divisor)

	bne $3, $0, outputLoop

	div $4, $16					
		mflo $4						; divisor = quotient(divisor/10)

	beq $0, $0, zerosLoop

outputLoop: 
	
	div $2, $4					; number / divisor
		mflo $3						; $3 = lo 
		mfhi $2						; $2 = $2 - ($4 * $3)

	add $3, $3, $11 		; $3 => ascii-code
	sw $3, 0($14)				; output “$3” 

	div $4, $16					; divisor = quotient(divisor/10)
		mflo $4					

	bne $4, $0, outputLoop			; if($4 == 0) => outputLoop

end: 

	sw $13, 0($14)	

; ………………………………………………………………………………………………………………………………………………………………………………………………
; Restoring register values
lw $1, -4($30)
lw $2, -8($30)
lw $3, -12($30)
lw $4, -16($30)
lw $5, -20($30)
lw $10, -24($30)
lw $11, -28($30)
lw $12, -32($30)
lw $13, -36($30)
lw $14, -40($30)
lw $15, -44($30)
lw $16, -48($30)
; ………………………………………………………………………………………………………………………………………………………………………………………………

	jr $31 

outputZero: 

	sw $11, 0($14)

	beq $0, $0, end
