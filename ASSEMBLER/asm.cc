#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "scanner.h"

//type1 == add, sub, slt, sltu
//type2 == beq, bne

void outputAscii(const int & instr) 
{
  unsigned char c = instr >> 24;    //get the MSB (byte 1)
  std::cout << c; 
  
  c = instr >> 16;                  //get byte 2
  std::cout << c; 
  
  c = instr >> 8;                   //get byte 3
  std::cout << c; 
  
  c = instr;                        //get the LSB (byte 4)
  std::cout << c;
}

bool intInRange(int64_t & i)
{
  int64_t max = 4294967295;   //max 32-bit (unsigned) integer
  int64_t min = -2147483648;  //min 32-bit (signed) integer

  if((i > max) || (i < min)) 
  {
    return false;
  }
  else 
  {
    return true; 
  }
}

bool regInRange(int & regNum)
{
  return (0 <= regNum && regNum <= 31); 
}

int beq2Int(int & reg1, int & reg2, int & offset, std::string & lexeme)
{
  int opcode = 4; 
  if(lexeme == "bne") opcode = 5; 

  int instr = (opcode << 26 ) | (reg1 << 21) | (reg2 << 16) | (offset & 0xFFFF); 
  return instr; 
}

int add2Int(int & reg1, int & reg2, int & reg3, std::string & lexeme)
{
  int funct = 32; 

  if(lexeme == "sub")         funct = 34;    
  else if(lexeme == "slt")    funct = 42; 
  else if(lexeme == "sltu")   funct = 43; 

  int instr = (0 << 26 ) | (reg2 << 21) | (reg3 << 16) | (reg1 << 11) | funct; 
  return instr; 
}

int mult2Int(int & reg1, int & reg2, std::string & lexeme)
{
  int funct = 24; 

  if(lexeme == "multu")         funct = 25;    
  else if(lexeme == "div")      funct = 26; 
  else if(lexeme == "divu")     funct = 27; 

  int instr = (0 << 26 ) | (reg1 << 21) | (reg2 << 16) | funct; 
  return instr; 
}

int mfhi2Int(int & reg1, std::string & lexeme)
{
  int funct = 16; 

  if(lexeme == "mflo")         funct = 18;    
  else if(lexeme == "lis")      funct = 20; 

  int instr = (0 << 26 ) | (0 << 21) | (0 << 16) | (reg1 << 11) | funct; 
  return instr; 
}

int lw2Int(int & reg1, int & reg2, int & offset, std::string & lexeme)
{
  int opcode = 35; 

  if(lexeme == "sw")  opcode = 43;   

  int instr = (opcode << 26) | (reg2 << 21) | (reg1 << 16) | (offset & 0xFFFF); 
  return instr; 
}

int jr2Int(int & reg1, std::string & lexeme)
{
  int funct = 8; 

  if(lexeme == "jalr")  funct = 9;    

  int instr = (0 << 26 ) | (reg1 << 21) | funct; 
  return instr; 
}

int address2offset(int & PC, int & labelAddress)
{
  return ( (labelAddress - PC) / 4 ) - 1; 
}

bool offsetInRange(int & offset)
{
  return ( (-32768 <= offset) && (offset <= 32767) ); 
}

//Pass 1
//*builds the symbol table passed in.
//*Also builds a 2d array of tokens for the second pass.
int pass1(std::map<std::string, int> & symbolTable, std::vector<std::vector<Token>> & tokenLines) 
{
  try 
  {
    int PC = 0;
    bool canIncrementPC = false; 
    std::string line;
    int lineNum = 0; 

    while (getline(std::cin, line))                 //linear in terms of lines & insructions
    {
      tokenLines.emplace_back( scan(line) );        //scan the line. Get the tokens into a vector. Put that in a vector of vectors. 
      canIncrementPC = false; 

      for(Token currToken : tokenLines[lineNum])
      {
        Token::Kind kind = currToken.getKind(); 

        if(kind == Token::LABEL)                            //there is a label being defined
        {
          std::string label = currToken.getLexeme();        //add (label-address) pair to map 
          label.pop_back();                                 //remove colon at the end

          if( ! symbolTable.insert({label, PC}).second)     //if nothing was inserted because it already existed!
          {
            throw ScanningFailure("ERROR : multiple label definitions!"); 
          }
        }
        else if(kind == Token::ID || kind == Token::WORD)   
        {
          canIncrementPC = true; 
        }
      }

      if(canIncrementPC) PC += 4;  //only if there was an instruction defined on this line
      ++lineNum; 
    }
  }
  catch (ScanningFailure &f) 
  {
    std::cerr << f.what() << std::endl;
    return 0;
  }
  return 1; 
} 

int pass2(std::map<std::string, int> & symbolTable, std::vector<std::vector<Token>> & tokenLines)
{
  try 
  {
    int PC = 0;
    bool canIncrementPC = false;    
    int numLines = tokenLines.size();                                     //while there are still lines to read (linear in terms of # lines) 

    for(int lineNum{0}; lineNum < numLines; ++lineNum)                              
    {
      std::vector<Token> tokenLine = tokenLines[lineNum];                 //get the next line
      canIncrementPC = false; 
      int lineSize = tokenLine.size(); 
      
      for (int tokenNum{0}; tokenNum < lineSize; ++tokenNum)              //check all tokens on the line (linear in number of tokens)
      {
        Token currToken = tokenLine[tokenNum];                            //get the i'th token 
          Token::Kind kind = currToken.getKind(); 
          std::string lexeme = currToken.getLexeme(); 

        if(kind == Token::LABEL)                           //skip to the next token
        {
          continue; 
        }
        if(kind == Token::WORD)                            //If the token is a WORD token
        {
          if( (tokenNum + 2) != lineSize) throw ScanningFailure("ERROR : not enough arguments for .word"); 

          ++tokenNum; 
          currToken = tokenLine[tokenNum];                //get the next token

          int64_t decimal{0}; 

          if(currToken.getKind() == Token::INT || currToken.getKind() == Token::HEXINT)     
          { 
            decimal = currToken.toNumber();  
          }
          else if(currToken.getKind() == Token::ID)                  //the ID has to be a label:              
          {
            std::map<std::string, int>::iterator it = symbolTable.find(currToken.getLexeme()); 
            
            if(it == symbolTable.end())
            {
              throw ScanningFailure("ERROR : .word operand (label), does not exist"); 
            }
            else 
            {
              decimal = it->second; 
            }
          }
          else 
          {
            throw ScanningFailure("ERROR : invalid .word operand. Must be either: INT, HEX or a valid Label"); 
          }

          if(intInRange(decimal))
          {
            outputAscii(decimal); 
          }
          else
          {
            ScanningFailure rangeError{"ERROR : integer out of range"}; 
            throw rangeError; 
          }

          canIncrementPC = true; 
        }
        else if(kind == Token::ID)                        //id can be a label, or an operation (beq, add, sub, ...)
        {
          if(lexeme == "beq" || lexeme == "bne")
          {
            if( (tokenNum + 6) != lineSize) throw ScanningFailure("ERROR : beq/bne, not the right amont of arguments"); 

            int reg1{0}, reg2{0}, offset{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : beq/bne not followed by a reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : beq/bne, reg1 not in range"); 

            currToken = tokenLine[tokenNum+2]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : beq/bne missing comma 1"); 

            currToken = tokenLine[tokenNum+3]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : beq/bne not followed by a reg2"); 
                reg2 = currToken.toNumber(); 
                  if(!regInRange(reg2)) throw ScanningFailure("ERROR : beq/bne, reg2 not in range"); 

            currToken = tokenLine[tokenNum+4]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : beq/bne missing comma 2"); 

            currToken = tokenLine[tokenNum+5];    //offset token

            tokenNum += 5; 

            if(currToken.getKind() == Token::ID)                              //must be a label
            {
              std::map<std::string, int>::iterator it = symbolTable.find(currToken.getLexeme()); 
            
              if(it == symbolTable.end())
              {
                throw ScanningFailure("ERROR : beq offset operand (label), does not exist"); 
              }
              else 
              {
                offset = address2offset(PC, it->second);
              }
            }
            else if(currToken.getKind() == Token::INT)                //must be an integer/hex
            {
              offset = currToken.toNumber(); 
              if(!offsetInRange(offset)) throw ScanningFailure("ERROR : beq offset out of range");
            }
            else if(currToken.getKind() == Token::HEXINT)
            {
              offset = currToken.toNumber(); 
              if(!(0 <= offset && offset < 65535)) throw ScanningFailure("ERROR : beq offset out of range");
            }
            else 
            {
              throw ScanningFailure("ERROR : beq offset is not a LABEL, INT or HEX"); 
            }
            
            int instr = beq2Int(reg1, reg2, offset, lexeme);  //opcode changes based on the lexeme (bne/beq)
            outputAscii(instr); 
          }
          else if(lexeme == "add" || lexeme == "sub" || lexeme == "slt" || lexeme == "sltu")
          {
            if( (tokenNum + 6) != lineSize) throw ScanningFailure("ERROR : expected REG1 COMMA REG2 COMMA REG3");   // #commas(2) + #reg(3) + 1 = 6

            int reg1{0}, reg2{0}, reg3{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : misssing reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : reg1 not in range"); 

            currToken = tokenLine[tokenNum+2]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : missing comma 1"); 

            currToken = tokenLine[tokenNum+3]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : missing reg2"); 
                reg2 = currToken.toNumber(); 
                  if(!regInRange(reg2)) throw ScanningFailure("ERROR : reg2 not in range"); 
              
            currToken = tokenLine[tokenNum+4]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : missing comma 2"); 

            currToken = tokenLine[tokenNum+5];      
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : missingreg3"); 
                reg3 = currToken.toNumber(); 
                  if(!regInRange(reg3)) throw ScanningFailure("ERROR : reg3 not in range"); 
            
            tokenNum += 5; 

            int instr = add2Int(reg1, reg2, reg3, lexeme);      //different integer depending on the lexem (funct bits change) 

            outputAscii(instr);         
          }
          else if(lexeme == "mult" || lexeme == "multu" || lexeme == "div" || lexeme == "divu")
          {
            if( (tokenNum + 4) != lineSize) throw ScanningFailure("ERROR : expected REG1 COMMA REG2");  //#commas(1) + #reg(2) + 1 = 4

            int reg1{0}, reg2{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : misssing reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : reg1 not in range"); 

            currToken = tokenLine[tokenNum+2]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : missing comma 1"); 

            currToken = tokenLine[tokenNum+3]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : missing reg2"); 
                reg2 = currToken.toNumber(); 
                  if(!regInRange(reg2)) throw ScanningFailure("ERROR : reg2 not in range"); 
              
            tokenNum += 3; 

            int instr = mult2Int(reg1, reg2, lexeme);      //different integer depending on the lexem (funct bits change) 
            outputAscii(instr);         
          }
          else if(lexeme == "mfhi" || lexeme == "mflo" || lexeme == "lis")
          {
            if( (tokenNum + 2) != lineSize) throw ScanningFailure("ERROR : mfhi/mflo/lis expects 1 REG");  //#reg(1) + 1 = 2

            int reg1{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : misssing reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : reg1 not in range"); 
            
            tokenNum+=1; 

            int instr = mfhi2Int(reg1, lexeme);      //different integer depending on the lexem (funct bits change) 
            outputAscii(instr);      
          }
          else if(lexeme == "lw" || lexeme == "sw") //opcode REG1 COMMA [HEX|INT] LPAREN REG2 RPAREN
          {
            if( (tokenNum + 7) != lineSize) throw ScanningFailure("ERROR : LW/SW wrong # tokens");  

            int reg1{0}, reg2{0}, offset{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : misssing reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : reg1 not in range"); 

            currToken = tokenLine[tokenNum+2]; 
              if(currToken.getKind() != Token::COMMA) throw ScanningFailure("ERROR : missing comma 1"); 

            currToken = tokenLine[tokenNum+3]; 

            if(currToken.getKind() == Token::INT)
            {
              offset = currToken.toNumber(); 
              if(!offsetInRange(offset)) throw ScanningFailure("ERROR : lw/sw offset out of range");
            }
            else if(currToken.getKind() == Token::HEXINT)
            {
              offset = currToken.toNumber(); 
              if(!(0 <= offset && offset < 65535)) throw ScanningFailure("ERROR : lw/sw offset out of range");
            }
            else 
            {
              throw ScanningFailure("ERROR : lw/sw immediate must be an INT or HEXINT"); 
            }

            currToken = tokenLine[tokenNum+4]; 
              if(currToken.getKind() != Token::LPAREN) throw ScanningFailure("ERROR : lw/sw missing LPAREN");

            currToken = tokenLine[tokenNum+5]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : lw/sw misssing reg2"); 
                reg2 = currToken.toNumber(); 
                  if(!regInRange(reg2)) throw ScanningFailure("ERROR : lw/sw reg2 not in range"); 

            currToken = tokenLine[tokenNum+6]; 
              if(currToken.getKind() != Token::RPAREN) throw ScanningFailure("ERROR : lw/sw missing RPAREN");

            tokenNum += 6; 

            int instr = lw2Int(reg1, reg2, offset, lexeme);  //opcode changes based on the lexeme (bne/beq)
            outputAscii(instr); 
          }
          else if(lexeme == "jr" || lexeme == "jalr")
          {
            if( (tokenNum + 2) != lineSize) throw ScanningFailure("ERROR : jr/jalr, needs only 1 REG"); 

            int reg1{0}; 

            currToken = tokenLine[tokenNum+1]; 
              if(currToken.getKind() != Token::REG) throw ScanningFailure("ERROR : jr/jalr not followed by a reg1"); 
                reg1 = currToken.toNumber(); 
                  if(!regInRange(reg1)) throw ScanningFailure("ERROR : jr/jalr, reg1 not in range"); 

            tokenNum += 1; 

            int instr = jr2Int(reg1, lexeme);  //opcode changes based on the lexeme (bne/beq)
            outputAscii(instr); 
          }
          else 
          {
            throw ScanningFailure("ERROR : not a valid instruction"); 
          }
          
          canIncrementPC = true;                                 
        }
      
        //At this point. We read a valid instruction. We should be at the end of the line. If theres anything after. It's an ERROR.  
        if(tokenNum != (lineSize-1)) throw ScanningFailure("ERROR : Extra stuff at the end of an instruction"); 
      }

      if(canIncrementPC) PC += 4;  //only if there was an instruction defined on this line (not a label or comment)
    }
  } 
  catch (ScanningFailure &f) 
  {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  
  return 0; 
}

int main() 
{
  std::map<std::string, int> symbolTable; 

  std::vector<std::vector<Token>> tokenLines;           //a 2d vector: stores each line of tokens, which is itself a vector of tokens. 
  tokenLines.reserve(10000);                            //reserve space ahead of time so that the vector does not have to keep resizing. 

  if(pass1(symbolTable, tokenLines))                       //create the symbol table  (consumes input from cin). pass2() only runs if there was no error thrown in pass 2
  {
    return pass2(symbolTable, tokenLines);
  }
}

