lis $10                 
.word 4
lis $11           
.word 1

add $4, $1, $0
sub $5, $2, $11 

sw $31, -4($30)
sub $30, $30, $10

lis $6
.word compare

loop: 
  lw $1, 0($4)
  add $4, $4, $10
  lw $2, 0($4)
  sub $5, $5, $11
  jalr $6
  beq $3, $0, end
  bne $5, $0, loop

end: 
  add $30, $30, $10
  lw $31, -4($30)
  jr $31
