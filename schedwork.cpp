#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedHelper(
  const AvailabilityMatrix& avail,
  const size_t n,
  const size_t d,
  const size_t m,
  const size_t k,
  const size_t i,
  DailySchedule& sched,
  std::map<Worker_T, int>& shiftmap
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
  const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule& sched
)
{
  std::map<Worker_T, int> shiftmap;
  if(avail.size() == 0U){
    return false;
  }
  sched.clear();
  // Add your code below
  int n = avail.size(), d = dailyNeed, m = maxShifts, k = avail[0].size();
  for(int i = 0; i < n; i++){ // initialize "blank" sched (completely invalid)
    vector<Worker_T> v;
    sched.push_back(v);
    for(int j = 0; j < d; j++){
      sched[i].push_back(INVALID_ID);
    }
  }
  return schedHelper(avail, n, d, m, k, 0, sched, shiftmap); // kick off recursion
}

bool schedHelper(
  const AvailabilityMatrix& avail,
  const size_t n,
  const size_t d,
  const size_t m,
  const size_t k,
  const size_t i,
  DailySchedule& sched,
  std::map<Worker_T, int>& shiftmap
){
  if(i == n * d) return true; // reached the end!
  if(sched[i / d][i % d] == INVALID_ID){
    for(int w = 0; w < k; w++){ // cycle through options (each worker)
      sched[i / d][i % d] = w; // 1d to 2d
      if((shiftmap[w] + 1 <= m) and (avail[i / d][w] == 1)){ // check these before making the changes to save a line or two
        shiftmap[w]++;
        if(schedHelper(avail, n, d, m, k, i + 1, sched, shiftmap)) return true; // recurse down sched
      }
      sched[i / d][i % d] = INVALID_ID; // backtrack if invalid
    }
  } else{
    std::cout << "this shouldn't print" << endl; // as you might have guessed, this shouldn't print
    return schedHelper(avail, n, d, m, k, i + 1, sched, shiftmap);
  }
  return false; // made it through all the options and none were valid
}