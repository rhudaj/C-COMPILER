# Overview 
* Scanner: generates tokens for the assembler from input C-code
* Written in C++ 

# 2 Parts 

## 1. Simplified Maximal Munch

Takes a DFA as input (start state, states, transitions) and determines wether or not an input word is in the language specified by the DFA. 


**Note:** A scanner need to be used in this step for the assembler to work. However, only the assembler part is what we care about here. 

## Features

* Scanner provides an input to the program: tokens (without whitespace or comments) 
* the assembler produces an error message when it receives an invalid program
* O(n) time where n is the "size" of the input (ie: # tokens)
* Uses only C++ std library 
* Utilizes 2 passes: 
    * first pass: construct a map (label-table) 
    * second pass: analyze tokens and generate output

**Instructions it works for:**

Works for all labels, comments and the following instructions: 
