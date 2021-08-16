#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  set_OperatingSystem(LinuxParser::OperatingSystem());
  set_Kernel(LinuxParser::Kernel());
  set_TotalMem(LinuxParser::TotalMem());
} 

// Done: Return the system's CPU
Processor& System::Cpu() {
  return cpu_;
}

/* Return a container composed of the system's processes
 * Very basic and ugly implementation
 * a map or alternative data structure may be more suitable here
 * Todo: If we know we will only display 10 processes can we optimise
 * prior to the display functions
 */
vector<Process>& System::Processes() {

  int count = 0;
  if(processes_.empty()) {   // Build the initial list
    for(auto pid : LinuxParser::Pids()) {
      //std::cout << "Instantiation of process. calling CpuUtilisation\n";
      Process process(pid);
      processes_.push_back(process);
    }
  }
  else {  // Mark all processes as old
    for(auto& i : processes_) {
      i.set_Status(Process::dead);
    }
    for(int pid : LinuxParser::Pids()) { //maintain list of *active* processes
      std::vector<Process>::iterator it = std::find(processes_.begin(), processes_.end(), Process(pid)); //call set_Data. Shouldnt matter as it is discarded
      if(it != processes_.end()) {
	it->set_Data();
	count++;
      } 
      else { //add new processes
	Process process(pid);
	processes_.push_back(process);
      }
    }
    // prune dead processes
    for(int i = 0; i < static_cast<int>(processes_.size()); i++) {
      if(processes_[i].Status() == Process::dead) {
	processes_.erase(processes_.begin() + i);	
      }
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
  
// Done: Return the system's memory utilization
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
    set_OperatingSystem(LinuxParser::OperatingSystem());
  }
    return os;
}

void System::set_OperatingSystem(const std::string os_string) {
  os = os_string;
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// Done: Return the total number of processes on the system
int System::TotalProcesses() {
  return static_cast<int>(LinuxParser::Pids().size());
}

// Done: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime(); 
}
