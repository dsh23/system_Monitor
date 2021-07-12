#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  enum proc_state {
    dead = 0,
    alive = 1
  };
  
  Process(int i);
  int Pid() const;     
  std::string User();
  //void set_User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  proc_state Status();
  void set_Status(proc_state);
  void set_Data();
  bool operator<(Process const& a) const;
  bool operator==(Process const& a) const;

 private:
  int pid;
  long uptime;
  std::string user;
  std::string command;
  std::vector<std::string> stat_data;
  proc_state status = alive;
};

#endif
