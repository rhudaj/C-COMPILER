#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std; 

const std::string ALPHABET    = ".ALPHABET";
const std::string STATES      = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT       = ".INPUT";
const std::string EMPTY       = ".EMPTY";

bool isChar(std::string s) 
{
  return (s.length() == 1);
}
bool isRange(std::string s) 
{
  return s.length() == 3 && s[1] == '-';
}

bool isAlphabetSymbol(char c)
{
  //valid if it's in [33, 126]  [!,~]
  return ( c >= 33 && c <= 126 ); 
}

class DFA_Error
{
  std::string message; 

  public:

    DFA_Error(std::string message): message{message} {}

    std::string what()  
    {
      return message; 
    }
}; 

// Locations in the program that you should modify to store the
// DFA information have been marked with four-slash comments:

class DFA
{
  private: 

    std::vector<char>           alphabet; 
    std::map<std::string, bool> states; 
    std::string                 startState; 
    std::map< std::pair<std::string, char>, std::string> transitions; //map: (fromState, symbol) pair => toState

  public: 

    DFA() {}

    ~DFA() {}
  
  public:

    void output()
    {
      cout << "DFA OUTPUT" << endl; 

      cout << "\t" << "ALPHABET: "; 
      for (auto c : alphabet) cout << c << ", ";
      cout << endl; 

      cout << "\t" << "STATES: " << endl; 
      for (auto state : states) cout << "\t\t" << state.first << ", " << state.second << endl; 

      cout << "\t" << "start state: " << getStartState() << endl; 

      cout << "\t" << "TRANSITIONS: " << endl; 
      for (auto t : transitions) cout << "\t\t" << "(" << t.first.first << ", " << t.first.second << ") => " << t.second << endl; 

      cout << "DFA OUTPUT" << endl; 
    }

    //Checks whether the state returned by transition is an accepting state
    bool isAcceptState(std::string & state) const 
    {
      return states.at(state); 
    }

    void setStartState(std::string state)
    {
      startState = state; 
    }

    std::string getStartState() const 
    { 
      return startState; 
    }

    void add2Alphabet(char c)
    {
      alphabet.emplace_back(c); 
    }

    void add2States(std::string & state, bool isAccept)
    {      
      states.insert(states.end(), {state, isAccept}); 
    }

    void addTransition(std::string fromState, char symbol, std::string toState)
    {
      transitions[std::make_pair(fromState, symbol)] = toState; 
    }

    bool stateFromTransition(std::string & state, char symbol) const
    {
      std::string originalState = state; 
      try
      {
        state = transitions.at(std::make_pair(state, symbol)); 
      }
      catch(const std::out_of_range &e) //transition does not exist
      {
        state = originalState; 
        return false; 
      }
      return true; 
    }

    bool simplifiedMaximalMunch(const std::string & input, std::vector<std::string> & tokens) const 
    {
      //cout << "simplifiedMaximalMunch() on " << input << endl; 

      std::string currState = getStartState(); 
      int inputSize = input.size(); 

      std::string currToken = ""; 
      char c; 

      for (int i = 0; i < inputSize; ++i) 
      {
        c = input[i];

        //cout << "\t" << "Curr State: " << state << endl; 
        //cout << "\t" << "Curr Token: " << currToken << endl; 
        //cout << "\t" << "Input: " << c << endl; 

        if(!stateFromTransition(currState, c))  //does currState become to a new state? 
        {
          //std::cout << "\t\t" << "NO TRANSITION FOR: " << "(" << state << ", " << c << ")" << std::endl; 
          
          if(isAcceptState(currState)) //if we're in an accept state
          {
            //std::cout << "\t\t" << "adding " << currToken << " to tokens" << std::endl; 

            tokens.emplace_back(currToken); 
            currToken = ""; 

            currState = getStartState(); 
            --i;                      
            continue; 
          } 
          else
          {
            tokens.clear(); 
            return false; 
          }
        }
        else 
        {
          currToken += c; 
        }
      }
      
      if(isAcceptState(currState)) 
      {
        tokens.emplace_back(currToken); 
        return true; 
      }
      else
      {
        tokens.clear(); 
        return false; 
      }                     
    }
}; 

int main() 
{
  DFA dfa; 
  std::vector<std::string> tokens; 

  std::istream& in = std::cin;
  std::string s;

  std::getline(in, s); // Alphabet section (skip header)
  
  //Read characters or ranges separated by whitespace
  while(in >> s) 
  {
    if (s == STATES) 
    { 
      break; 
    } 
    else 
    {
      if (isChar(s))                //single character
      {
        dfa.add2Alphabet(s[0]);       //Variable 's[0]' is an alphabet symbol
      } 
      else if (isRange(s))          //a range 
      {          
        for(char c = s[0]; c <= s[2]; ++c) 
        {
          dfa.add2Alphabet(c); 
        }
      } 
    }
  }

  std::getline(in, s); //States section (skip header)

  //Read states separated by whitespace
  bool initial = true; 
  while(in >> s) 
  {
    if (s == TRANSITIONS) 
    { 
      break; 
    } 
    else 
    {
      bool accepting = false;
      
      if(s.back() == '!' && !isChar(s)) 
      {
        accepting = true;
        s.pop_back();
      }

      if(initial) 
      {
        dfa.setStartState(s); 
        initial = false; 
      }
      
      dfa.add2States(s, accepting);   //If accepting = true, then it will be add to acceptStates
    }
  }

  std::getline(in, s); // Transitions section (skip header)
  
  // Read transitions line-by-line
  while(std::getline(in, s)) 
  {
    //cout << s << endl; 
    if (s == INPUT) 
    { 
      break; 
    } 
    else 
    {
      std::string fromState = "", symbols = "", toState = "";
      std::istringstream line(s);

      line >> fromState;

      while(line >> s) 
      {
        if(line.peek() == EOF)  // If we reached the last item on the line
        { 
          toState = s;          // Then it's the to-state
        } 
        else                    // Otherwise, there is more stuff on the line
        {
          if(isChar(s))        // We expect a character or range
          {
            //cout << "Adding a symbol:  " << s << endl; 
            symbols += s;
          } 
          else if (isRange(s)) 
          {
            //cout << "Adding a range:  " << endl; 
            for(char c = s[0]; c <= s[2]; ++c)  
            {
              //cout << "\t" << c << endl; 
              symbols += c;
            }
          }
        }
      }
      //cout << "symbols: " << symbols << endl; 
      for (char c : symbols) 
      {
        //cout << "adding transition: " << "(" << fromState << ", " << c << ") => " << toState << endl; 
        dfa.addTransition(fromState, c, toState); 
      }
    }
  }

  // Input section (already skipped header)

  //dfa.output();

  while(in >> s) //s = input string for the DFA: 
  {
    bool result = dfa.simplifiedMaximalMunch(s, tokens); 

    if(result)
    {
      for(auto s : tokens) cout << s << endl; 
    }
    else
    {
      cerr << "ERROR" << endl; 
    }
  }
}