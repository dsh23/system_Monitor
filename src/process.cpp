#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unistd.h>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i) : pid(i) {
  stat_data = LinuxParser::ProcStats(Pid());
  proc_time = LinuxParser::UpTime();
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  float hertz = sysconf(_SC_CLK_TCK);
  float time = 0;
  //long tmp;
  for(int i = 13; i < 17; i++) {
    time += std::stol(stat_data[i]);
  }
    float seconds = proc_time - (std::stol(stat_data[21]) / hertz);
    return ((time / hertz) / seconds);
}

// TODO: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(this->pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  const int mbyte = 1024; 
  int  raw_mem = std::stoi(LinuxParser::Ram(this->pid));
  int mb_mem;
  int kb_mem;
  
  mb_mem = raw_mem / mbyte;
  kb_mem = raw_mem % mbyte;
  // basic precision limit, this could be done with sstream
  // and precision() but that seems heavy?
  kb_mem = (kb_mem / static_cast<float> (mbyte)) * 1000;

  return std::to_string(mb_mem) + "." + std::to_string(kb_mem);
}

// TODO: Return the user (name) that generated this proces
string Process::User() {
  return LinuxParser::User(LinuxParser::Uid(this->pid));
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return proc_time - (std::stol(stat_data[21]) / sysconf(_SC_CLK_TCK));
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}
