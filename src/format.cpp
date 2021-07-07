#include <string>

#include "format.h"

using std::string;

#define DAY (24 * 60 * 60)
#define HOUR (DAY / 24)
#define MIN (HOUR / 60)

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds, bool proc_win) {
 if(seconds <= 59) {
    return std::to_string(seconds) + "s";
  }

  long raw = seconds;
    
  int days = seconds / DAY;
  raw = raw % DAY;
  
  int hours = raw / HOUR;
  raw = raw % HOUR;
  
  int mins = raw / MIN;
  raw = raw % MIN;

  int secs = raw;
   
  if(proc_win) {
    return std::to_string(days) + "d:" + std::to_string(hours) + "h:" + std::to_string(mins) + "m";    
  }
  else {
    return std::to_string(days) + "d:" + std::to_string(hours) + "h:" + std::to_string(mins) + "m:"  + std::to_string(secs) + "s";
  }
}
