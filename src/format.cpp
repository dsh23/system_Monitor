#include <string>

#include "format.h"

using std::string;

#define DAY (24 * 60 * 60)
#define HOUR (DAY / 24)
#define MIN (HOUR / 60)


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds, bool proc_win) {

  string ts_string;
  
  if(seconds < 0) {
    ts_string = "Error"; // TODO: Implement better error control e.g try/catch
  } else if(seconds <= 59) {
    ts_string = std::to_string(seconds) + "s"; 
  } else {

    long raw = seconds;
    
    int days = seconds / DAY;
    raw = raw % DAY;
  
    int hours = raw / HOUR;
    raw = raw % HOUR;
  
    int mins = raw / MIN;
    raw = raw % MIN;

    int secs = raw;
   
    if(proc_win) { //always display seconds in the top window 
      ts_string = std::to_string(days) + "d:" + std::to_string(hours) + "h:" + std::to_string(mins) + "m";    
    }
    else {
      ts_string = std::to_string(days) + "d:" + std::to_string(hours) + "h:" + std::to_string(mins) + "m:"  + std::to_string(secs) + "s";
    }
  }
  return ts_string;
}
