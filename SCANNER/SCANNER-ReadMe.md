# Overview 
* Scanner: generates tokens for the assembler from input C-code
* Written in C++ 

# 2 Parts 

## 1. Simplified Maximal Munch

Takes a DFA as input (start state, states, transitions) and determines wether or not an input word is in the language specified by the DFA. 

[smm.cc](https://github.com/Romanhudaj11/C-COMPILER/blob/main/SCANNER/smm.cc) - tokenizes input using a provided DFA (for c-code) and the Simplified Maximal Munch algorithm

## 2. 
