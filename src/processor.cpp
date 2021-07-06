#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() {
  //do I need Processor:: here
  set_Utilization(LinuxParser::CpuUtilization());
}

float Processor::Utilization() { return utilization; }

/*  NB. Implementation based on discussion here
    https://stackoverflow.com/a/23376195
*/
void Processor::set_Utilization(std::vector<std::string> u_stats) {
  
  long idle = std::stol(u_stats[kIdle_]) + std::stol(u_stats[kIOwait_]);
  long usertime = std::stol(u_stats[kUser_]) - std::stol(u_stats[kGuest_]);
  long nicetime = std::stol(u_stats[kNice_]) - std::stol(u_stats[kGuestNice_]);
  long sysall = std::stol(u_stats[kSystem_]) +
    std::stol(u_stats[kIRQ_]) + std::stol(u_stats[kSoftIRQ_]);
  long virttime = std::stol(u_stats[kGuest_]) + std::stol(u_stats[kGuestNice_]);

  float total = static_cast<float> (usertime + nicetime + sysall + virttime);
  if(total == 0.0) {
    //fix - clean
    utilization = -1;
  }
  utilization = total / (total + static_cast<float> (idle));  
}
