#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unistd.h>
#include "linux_parser.h"
#include "process.h"
#include <iostream>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i) : pid(i) {
  set_Data();
  user = LinuxParser::User(LinuxParser::Uid(this->pid));
  command = LinuxParser::Command(this->pid);  
}

// Return this process's ID
int Process::Pid() const { return pid; }

// Update process data
void Process::set_Data() {
  stat_data = LinuxParser::ProcStats(this->pid);
  uptime = LinuxParser::UpTime();
  set_CpuUtilization(); // 1st call
  status = Process::alive;
}

// Return this process's CPU utilization
float Process::CpuUtilization() { //const {
  return proc_utilization;
}

void Process::set_CpuUtilization() {
  float process_current;
  float hertz = sysconf(_SC_CLK_TCK); // Make static
  float seconds = uptime - (std::stol(stat_data[21]) / hertz);
  int seconds_delta = uptime - old_uptime;
  long proc_time = 0;
  //long tmp;

  for(int i = 13; i < 17; i++) {
    proc_time += std::stol(stat_data[i]);
  }

  int proc_delta = proc_time - old_proc_time;

  if(proc_delta == 0) {  // No processing has occured
    process_current = 0;
  }
  else {
    if(old_uptime == 0) { //insufficeint time shift or error
      process_current = ((static_cast<float>(proc_time) / hertz) / static_cast<float> (seconds));
    } else {
      process_current = ((static_cast<float>(proc_delta) / hertz) / static_cast<float> (seconds_delta));
    }
  }
  
  old_uptime = uptime;
  old_proc_time = proc_time;

  proc_utilization = process_current;

}

// Return the command that generated this process
std::string Process::Command() {
  return command;
}

// Return this process's memory utilization
string Process::Ram() {
  const int mbyte = 1024; 
  int  raw_mem = std::stoi(LinuxParser::Ram(this->pid));
  
  int mb_mem = raw_mem / mbyte;
  int kb_mem = raw_mem % mbyte;
  // basic precision limit, this could be done with sstream
  // and precision() but that seems heavy?
  kb_mem = (kb_mem / static_cast<float> (mbyte)) * 1000;

  return std::to_string(mb_mem) + "." + std::to_string(kb_mem);
}

// Return the user (name) that generated this proces
string Process::User() {
  return user;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  return uptime - (std::stol(stat_data[21]) / sysconf(_SC_CLK_TCK));
}

Process::proc_state Process::Status() {
  return this->status;
}

void Process::set_Status(Process::proc_state ps) {
  this->status = ps;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->proc_utilization > a.proc_utilization;
}

// Comparator predicate
bool Process::operator==(Process const& a) const {
  return this->Pid() == a.Pid();
}
