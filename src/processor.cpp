#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <fstream>
#include <string>

// TODO: Return the aggregate CPU utilization
Processor::Processor() {
  set_Utilization(LinuxParser::CpuUtilization());
}

const std::vector<float> Processor::Utilization() {
  set_Utilization(LinuxParser::CpuUtilization());
  std::vector<float> cpu_data{lifetime, current}; 
  return cpu_data;
}

/*  NB. Implementation based on discussion here
    https://stackoverflow.com/a/23376195
*/

void Processor::set_Utilization(const std::vector<std::string> u_stats) {
  
  long idle = std::stol(u_stats[kIdle_]) + std::stol(u_stats[kIOwait_]);
  long work  = std::stol(u_stats[kUser_]) + std::stol(u_stats[kNice_]) + std::stol(u_stats[kSystem_]) + std::stol(u_stats[kIRQ_]) + std::stol(u_stats[kSoftIRQ_]) + std::stol(u_stats[kSteal_]);

  long old_total = old_work + old_idle;
  long total = work + idle;

  lifetime = work / static_cast<float> (total);

  int total_delta = total - old_total;
  int idle_delta = idle - old_idle;

  if(total_delta > 0) {
    current = (total_delta - idle_delta) / static_cast<float> (total_delta);   
  }
  else {
    current = lifetime; // fallback value for div 0 expressions etc
  }
  old_work = work;
  old_idle = idle;
}
