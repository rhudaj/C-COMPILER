# Overview 
* Assembler for MIPS assembly language 
* correctly assembles all valid programs, and correctly reject all invalid programs.
* Written in C++ 

**Steps:**

1. Scanning the input
3. Identify the instructions (and the register values or immediate values)
4. do error checking (output error messages if invalid) 
5. instructions into binary and output them to stdout

**Note:** A scanner need to be used in this step for the assembler to work. However, only the assembler part is what we care about here. 

## Features

* Scanner provides an input to the program: tokens (without whitespace or comments) 
* the assembler produces an error message when it receives an invalid program
* O(n) time where n is the "size" of the input (ie: # tokens)
* Uses only C++ std library 

**Instructions it works for: **

Works for all labels and the following instructions: 

Instruction |Example
------------|------------------
add         | add $3, $1, $1 
sub         | sub $3, $1, $2
mult        | mult $3, $2
multu       | multu $3, $2
div         | div $1, $2
divu        | divu $1, $2
mfhi        | mfhi $1
mflo        | mflo $1
lis         | lis $1
lw          | lw $3, 4($2)
sw          | sw $3, 8($2) 
slt         | slt $3, $1, $2
sltu        | sltu $3, $1, $2
beq         | beq $3, $1, $2
bne         | bne $3, $1, $2
jr          | jr -4
jalr        | jalr $31
.word       | .word 4

