#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include<queue>
#include <map>
#include <set>
#include<iterator>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
int count(const std::string&, const char&);
// void gencombs(std::queue<char>, std::string, std::set<std::string>&);
std::string getUnpermuted(std::string curr, int dashesToAdd);
void stringToCharQueue(const std::string& unpermuted, std::queue<char>& q, int i);
void permute(std::queue<char> q, int size, string curr, std::vector<std::string>& perms);
void alpha(std::string full, std::set<std::string>& allcombos);
void allcombs(std::string full, std::string perm, std::set<std::string>& allcombos, int dashpos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
  // Add your code here
  std::set<std::string> retset, allcombos;
  std::queue<char> q;
  std::string unpermuted = getUnpermuted(floating, count(in, '-') - floating.length());
  stringToCharQueue(unpermuted, q, 0);
  std::vector<std::string> permutations;
  permute(q, q.size(), "", permutations); // recursive helper function to generate all permutations of floating + dashes
  for(unsigned i = 0; i < permutations.size(); i++)
    allcombs(in, permutations[i], allcombos, 0);
  std::set<std::string>::iterator it = allcombos.begin();
  for(; it != allcombos.end(); ++it){ // transfer every dictionary-verified word into return set
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

std::string getUnpermuted(std::string curr, int dashesToAdd){
  if(!dashesToAdd) return curr;
  return getUnpermuted(curr +'-', dashesToAdd - 1);
}

void stringToCharQueue(const std::string& unpermuted, std::queue<char>& q, int i){
  if(i == unpermuted.length()) return;
  q.push(unpermuted[i]);
  stringToCharQueue(unpermuted, q, i + 1);
}

void permute(std::queue<char> q, int size, string curr, std::vector<std::string>& perms){
  if(curr.length() == size) perms.push_back(curr);
  for(int i = 0; i < q.size(); i++){
    char c = q.front();
    q.pop();
    curr += c;
    permute(q, size, curr, perms);
    curr.pop_back();
    q.push(c);
  }
}

void alpha(std::string full, std::set<std::string>& allcombos){
  if(full.find('-') == string::npos){
    allcombos.insert(full);
    return;
  }
  int nextdash = full.find('-');
  for(int i = 0; i < 26; i++){
    full[nextdash] = 'a' + i;
    alpha(full, allcombos);
  }
}

void allcombs(std::string full, std::string perm, std::set<std::string>& allcombos, int dashpos){
  for(int i = full.find('-'); i < full.size(); i = full.find('-', i + 1))
    full[i] = perm[dashpos++];
  alpha(full, allcombos);
}

// void gencombs(std::queue<char> letterstoiterate, std::string curr, std::set<std::string>& combs){
//   if(curr.find('-') == std::string::npos) combs.insert(curr); // word complete
//   else{
//     int i = curr.find('-');
//     // for(int i = curr.find('-'); i < curr.size(); i = curr.find('-', i + 1)){ // iterate through "--x-"
//       if(letterstoiterate.size()){ // if there are still floating letters to use
//         for(int j = 0; j < letterstoiterate.size(); j++){ // iterate over floaters
//           char c = letterstoiterate.front();
//           letterstoiterate.pop();
//           curr[i] = c; // replace '-' with floater
//           gencombs(letterstoiterate, curr, combs); // recurse with '-' replaced and the replacing floater removed
//           curr[i] = '-';
//           letterstoiterate.push(c);
//         }
//       } else{
//         for(int j = 0; j < 26; j++){
//           string temp = curr;
//           temp[i] = 'a' + j;
//           gencombs(letterstoiterate, temp, combs);
//         }
//       }
//     // }
//   }
// }

// void gencombs(std::string letterstoiterate, std::string curr, std::set<std::string>& combs){
//   if(curr.find('-') == std::string::npos) combs.insert(curr); // word complete
//   else{
//     for(int i = curr.find('-'); i < curr.size(); i = curr.find('-', i + 1)){ // iterate through "--x-"
//       if(letterstoiterate.size()){ // if there are still floating letters to use
//         for(int j = 0; j < letterstoiterate.size(); j++){ // iterate over floaters
//           string newlets = letterstoiterate;
//           newlets.erase(newlets.begin() + j); // remove replacing floater from floating list
//           string temp = curr;
//           temp[i] = letterstoiterate[j]; // replace '-' with floater
//           gencombs(newlets, temp, combs); // recurse with '-' replaced and the replacing floater removed
//         }
//       } else{
//         for(int j = 0; j < 26; j++){
//           string temp = curr;
//           temp[i] = 'a' + j;
//           gencombs("", temp, combs);
//         }
//       }
//     }
//   }
// }

// void gencombs(std::string letterstoiterate, std::string curr, std::set<std::string>& combs){
//   if(!count(curr, '-')) combs.insert(curr); // word complete
//   else{
//     for(int i = 0; i < curr.size(); i++){ // iterate through "--x-"
//       if(curr[i] == '-'){ // need to recurse down this path
//         if(letterstoiterate.size()){ // if there are still floating letters to use
//           for(int j = 0; j < letterstoiterate.size(); j++){ // iterate over floaters
//             string newlets = letterstoiterate;
//             newlets.erase(newlets.begin() + j); // remove replacing floater from floating list
//             string temp = curr;
//             temp[i] = letterstoiterate[j]; // replace '-' with floater
//             gencombs(newlets, temp, combs); // recurse with '-' replaced and the replacing floater removed
//           }
//         } else alpha(curr, combs, i, 0); // no floating letters to use, so cycle a-z
//         }
//       }
//     }
//   }
// }

// void alpha(std::string curr, std::set<std::string>& s, int index, int offset){
//   if(offset == 26) return; // 'z' generated in previous call
//   string temp = curr;
//   temp[index] = 'a' + offset; // replace the '-' with a letter a-z
//   gencombs("", temp, s); // generate combinations with the current setup (no floating left)
//   alpha(curr, s, index, offset + 1); // cycle a-z
// }