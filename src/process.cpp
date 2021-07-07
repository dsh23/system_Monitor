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
  long mem = std::stol(LinuxParser::Ram(this->pid));
  mem = mem/1000;
  return std::to_string(mem);
}

// TODO: Return the user (name) that generated this process
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
