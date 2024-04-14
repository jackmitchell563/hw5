#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include<iterator>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
int count(const std::string&, const char&);
void gencombs(std::string, std::string, std::set<std::string>&);
void alpha(std::string, std::set<std::string>&, int, int);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
  // Add your code here
  std::set<std::string> retset, temp;
  gencombs(floating, in, temp); // recursive helper function to generate all combinations
  std::set<std::string>::iterator it = temp.begin();
  for(; it != temp.end(); ++it){ // transfer every dictionary-verified word into return set
    if(dict.find(*it) != dict.end()) retset.insert(*it);
  }
  return retset;
}

// Define any helper functions here
int count(const std::string& s, const char& c){
  int i = 0, count = (s[0] == c) - 1; // loop guaranteed to run once, so subtract one to compensate
  while(i != std::string::npos){
    i = s.find(c, i + 1); // update index to 1 past next occurrence of the character
    count++;
  }
  return count;
}

void gencombs(std::string letterstoiterate, std::string curr, std::set<std::string>& combs){
  if(!count(curr, '-')) combs.insert(curr); // word complete
  else{
    for(int i = 0; i < curr.size(); i++){ // iterate through "--x-"
      if(curr[i] == '-'){ // need to recurse down this path
        if(letterstoiterate.size()){ // if there are still floating letters to use
          for(int j = 0; j < letterstoiterate.size(); j++){ // iterate over floaters
            string newlets = letterstoiterate;
            newlets.erase(newlets.begin() + j); // remove replacing floater from floating list
            string temp = curr;
            temp[i] = letterstoiterate[j]; // replace '-' with floater
            gencombs(newlets, temp, combs); // recurse with '-' replaced and the replacing floater removed
          }
        } else alpha(curr, combs, i, 0); // no floating letters to use, so cycle a-z
      }
    }
  }
}

void alpha(std::string curr, std::set<std::string>& s, int index, int offset){
  if(offset == 26) return; // 'z' done in last call
  string temp = curr;
  temp[index] = 'a' + offset; // replace the '-' with a letter a-z
  gencombs("", temp, s); // generate combinations with the current setup (no floating left)
  alpha(curr, s, index, offset + 1); // cycle a-z
}