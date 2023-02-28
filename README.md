# C-COMPILER
C++ project: compiles C-code to MIPS assembly language. Includes all stages of compilation in separate executable files and as a single project. 

This project was developped for CS241: Foundations of Sequential Programs over the course of the Winter 2023 semester. 

## Features 
* Efficient: each step runs in O(n) time
* High Cohesion and minimized coupling 
  * Each file performs one function
  * Steps of the compiler are modularized/seperated so that it is easily modifiable to other languages
* Each step (folder in this repo) can be ran individually
* All steps can be ran at once for the full compilation process

## Steps
### Scanning
Transforming code from a sequence of individual characters into meaningful chunks called tokens for easier processing
  
  
### Assembler

[ASSEMBLER-step](https://github.com/Romanhudaj11/C-COMPILER/tree/main/ASSEMBLER)

[ASSEMBLER-ReadMe](https://github.com/Romanhudaj11/C-COMPILER/tree/main/ASSEMBLER) - for more info

  *  Assembly Language ==> Machine Language
  *  Given an assembly program, it translates the file into binary (machine language) that the CPU can understand directly 

## MIPS?

MIPS Assembly Langage (MAL) is the assembly language for MIPS processors. It is currently one of the major processors in the embedded processor market.

Although ARM is a more standardly used, the foundations of MIPS and ARM are the same. And for the purpose of creating a compiler, the techniques are roughly the same. 

You can check out some examples of MIPS programs that I created by following this link: 

  [MIPS-examples](https://github.com/Romanhudaj11/C-COMPILER/tree/main/MIPS%20examples)
