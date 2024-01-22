# C-COMPILER
C++ project: compiles C-code to MIPS assembly language. Includes all stages of compilation in separate executable files and as a single project. 

## Features 
* Efficient: each step runs in O(n) time
* High Cohesion and minimized coupling 
  * Each file performs one function
  * Steps of the compiler are modularized/seperated so that it is easily modifiable to other languages
* Each step (folder in this repo) can be ran individually
* All steps can be ran at once for the full compilation process

## Steps
### Scanning

[SCANNING-step](https://github.com/Romanhudaj11/C-COMPILER/tree/main/SCANNER)

[SCANNING-ReadMe](https://github.com/Romanhudaj11/C-COMPILER/blob/main/SCANNER/SCANNER-ReadMe.md) - for more info

Transforming code from a sequence of individual characters into meaningful chunks called tokens for easier processing
  
### Parser

[PARSER-step](https://github.com/Romanhudaj11/C-COMPILER/tree/main/PARSER)

[PARSER-ReadMe](https://github.com/Romanhudaj11/C-COMPILER/blob/main/PARSER/PARSER-ReadMe.md)- for more info

* Parses C-code
* Operates on the output of the scanner
* implements the LR(1) parsing algorithm based on a DFA
  
### Assembler

[ASSEMBLER-step](https://github.com/Romanhudaj11/C-COMPILER/tree/main/ASSEMBLER)

[ASSEMBLER-ReadMe](https://github.com/Romanhudaj11/C-COMPILER/blob/main/ASSEMBLER/ASSEMBLER-ReadMe.md) - for more info

  *  Assembly Language ==> Machine Language
  *  Given an assembly program, it translates the file into binary (machine language) that the CPU can understand directly 

## MIPS?

MIPS Assembly Langage (MAL) is the assembly language for MIPS processors. It is currently one of the major processors in the embedded processor market.

Although ARM is a more standardly used, the foundations of MIPS and ARM are the same. And for the purpose of creating a compiler, the techniques are roughly the same. 

You can check out some examples of MIPS programs that I created by following this link: 

  [MIPS-examples](https://github.com/Romanhudaj11/C-COMPILER/tree/main/MIPS%20examples)
