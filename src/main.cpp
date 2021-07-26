#include "ncurses_display.h"
#include "system.h"
//Test harness
#include <string>
#include <chrono>
#include "format.h"
#include <thread>
#include <iostream>
using namespace::std;

int main() {
  //int n = 10;
  System system;
  /*    for(int i =0; i < 100; i++) {
  //test harness;
  string os = system.OperatingSystem();
  string kern = system.Kernel();
  float current_cpu =  system.Cpu().Utilization().current;
  float mem = system.MemoryUtilization();
  int t_procs = system.TotalProcesses();
  int r_procs = system.RunningProcesses();
  string uptime = Format::ElapsedTime(system.UpTime(), false);
  cout << os << "  " << kern << "  " << uptime << endl;
  cout << current_cpu << "  " << mem << "  " << t_procs << "  " << r_procs << endl;

  vector<Process>& processes = system.Processes();
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  for (int i = 0; i < num_processes; ++i) {
    int pid = processes[i].Pid();
    string user = processes[i].User();
    float cpu = processes[i].CpuUtilization() * 100;
    string ram = processes[i].Ram();
    cout << pid << "  " << user << "  " << cpu << " " << ram << endl;
  }
  
  std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  */
  NCursesDisplay::Display(system);
}
