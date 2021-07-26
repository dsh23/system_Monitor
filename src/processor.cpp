#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>

// TODO: Return the aggregate CPU utilization
Processor::Processor() {
  set_Utilization(LinuxParser::CpuUtilization());
}

const Processor::UtilData& Processor::Utilization() {
  set_Utilization(LinuxParser::CpuUtilization());
  return util_data;
}

/*  NB. Implementation based on discussion here
    https://stackoverflow.com/a/23376195
*/
void Processor::set_Utilization(const std::vector<std::string> u_stats) {
/*  std::ofstream myfile;
  myfile.open("./build/processor.txt", std::ofstream::app);
  myfile << std::to_string(util_data.old_work) + ", " +  std::to_string(util_data.old_idle) + "\n";
  myfile.close();
*/  
  
  long idle = std::stol(u_stats[kIdle_]) + std::stol(u_stats[kIOwait_]);
  long work  = std::stol(u_stats[kUser_]) + std::stol(u_stats[kNice_]) + std::stol(u_stats[kSystem_]) + std::stol(u_stats[kIRQ_]) + std::stol(u_stats[kSoftIRQ_]) + std::stol(u_stats[kSteal_]);
  long old_total;
  long total;
  int total_delta;
  int idle_delta;

  old_total = util_data.old_work + util_data.old_idle;
  total = work + idle;

  util_data.lifetime = work / static_cast<float> (total);
  total_delta = total - old_total;
  idle_delta = idle - util_data.old_idle;

  if(total_delta > 0) {
    util_data.current = (total_delta -idle_delta) / static_cast<float> (total_delta);   
  }
  else {
    util_data.current = util_data.lifetime;
  }
  util_data.old_work = work;
  util_data.old_idle = idle;
}
