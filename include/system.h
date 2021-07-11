#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  int TotalMem();

  // TODO: Define any necessary private members
 private:
  void set_Kernel(const std::string);
  void set_OperatingSytem(const std::string);
  void set_TotalMem(const int);
  
  Processor cpu_;
  std::vector<Process> processes_ = {};
  std::string knl;
  std::string os;
  int total_mem = 0;
};

#endif
