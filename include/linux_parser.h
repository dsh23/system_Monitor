#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
  // Paths
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};

  // System
  int TotalMem();  
  float MemoryUtilization();
  long UpTime();
  std::vector<int> Pids();
  int TotalProcesses();
  int RunningProcesses();
  std::string OperatingSystem();
  std::string Kernel();

  /*CPU //use? where?
    enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
    };
  */
  const std::vector<std::string> CpuUtilization();
  // Jiffies just not needed and probably more confusing to your students.
  // consult man (5) proc
  long Jiffies();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();

  // Processes
  std::string Command(int pid);
  std::string Ram(int pid);
  std::string Uid(int pid);
  std::string User(std::string uid);
  long int UpTime(int pid);
  std::vector<std::string> ProcStats(int);
};  // namespace LinuxParser

#endif
