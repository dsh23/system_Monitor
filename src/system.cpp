#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  set_OperatingSytem(LinuxParser::OperatingSystem());
  set_Kernel(LinuxParser::Kernel());
  set_TotalMem(LinuxParser::TotalMem());
} 

// TODO: Return the system's CPU
Processor& System::Cpu() {
  return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  //a map is waay more suitable here
  //processes_.clear();
  // mark all process as dead
  for(auto i : processes_) {
    i.set_Status(Process::proc_state::dead);
  }
  for(int pid : LinuxParser::Pids()) {
    std::vector<Process>::iterator it;
    it = std::find(processes_.begin(), processes_.end(), Process(pid));
    if(it != processes_.end()) {
      it->set_Data();     
    }
    else {
      Process process(pid);
      processes_.push_back(process);
    }
  }
  //prune processes
  for(int i = 0; i < static_cast<int>(processes_.size()); i++) {
    if(processes_[i].Status() == Process::proc_state::dead) {
      processes_.erase(processes_.begin() + i);
    }
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

/* Return the system's kernel identifier (string)
   Store the string as this changes infrquently.
   There's probably some more sophisticated method for detecting
   a kernel change. In almost every circumstance this require a reboot
   so happy with this for now.
*/
std::string System::Kernel() {
  if(knl.empty()) {
    set_Kernel(LinuxParser::Kernel());
  }
  return knl;
}

void System::set_Kernel(const std::string k_string) {
  knl = k_string;
}

int System::TotalMem() {
  if(total_mem == 0) {
    set_TotalMem(LinuxParser::TotalMem());
  }
  return total_mem;
}

void System::set_TotalMem(const int total) {
  total_mem = total;
}
  
// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  return (total_mem - LinuxParser::MemoryUtilization()) / total_mem;
}

/* Return the operating system name
   Similar to the kernel case this changes infrequently
   However it is much eaier to update.
   I will assume if the user needs to know the instantaneous
   os state then they will use the appropraie tools.
*/
std::string System::OperatingSystem() {
  if(os.empty()) {
    set_OperatingSytem(LinuxParser::OperatingSystem());
  }
    return os;
}

void System::set_OperatingSytem(const std::string os_string) {
  os = os_string;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return static_cast<int>(LinuxParser::Pids().size());
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime(); 
}
