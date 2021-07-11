#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

int LinuxParser::TotalMem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> key >> value;
  }
  return std::stoi(value);
}

// TODO: Read and return the system memory utilization
// change to memoryFree
float LinuxParser::MemoryUtilization() {
   string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (!filestream.is_open()) {
    return -1; }
  else {
    int rows = 0;
    while (rows < 1) {
      std::getline(filestream, line);
      rows++;
    }
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> key >> value;
  }
  return std::stof(value, nullptr);
  //float used = total - free;
  //return ((total - free) / total);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string raw_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> raw_time;
    return std::stol(raw_time, nullptr);  
  }
  // format error data in the format function: format.cpp
  return -1;
}

// TODO: Read and return CPU utilization

const vector<string> LinuxParser::CpuUtilization() {
  string col, line;
  vector<string> row;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> col) {
     row.push_back(col);
    } 
  } //else: should be some error checking here
  //return a formatted row 
  row.erase(row.begin());
  return row;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int procs = -1; //Default value in case of failure

  std::ifstream stream(kProcDirectory + kStatFilename);
  while(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
        if (key == "procs_running") {
	  stream.close();
          procs = std::stoi(value);
	  break;
        }
    }
  }
  return procs;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string proc = std::to_string(pid);
  std::string line;
  std::ifstream stream(kProcDirectory + proc + kCmdlineFilename);
  if(stream.is_open()) {
    if(!std::getline(stream, line)) {
      // Under some curcumstances this can be empty
      line = "No entry in /proc/" + proc + "/cmdline\n";
    }
  }
  return line;
}

// TODO: Read and return the memory used by a process

string LinuxParser::Ram(int pid) {
  // VmSize not allways recorded for a process e.g. see:
  // https://unix.stackexchange.com/questions/500212/
  std::string result = "0"; // fallback for above
  std::string key, value, line;
  std::string proc = std::to_string(pid);

  std::ifstream stream(kProcDirectory + proc + kStatusFilename);
  while(stream.is_open()) {
    if(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "VmSize") {          
          stream.close();
	  result = value;
	  break;
	}
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value;
  string kProcId = std::to_string(pid);
  std::ifstream stream(kProcDirectory + kProcId + kStatusFilename);
  while(stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
        if (key == "Uid") {          
          stream.close();
	  break;
        }
      }
  }
  return value;
}

// TODO: Read and return the user associated with a process uid
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(std::string uid) {

  string user, shad, uid_num;
  string line;
  std::ifstream stream(kPasswordPath);
  while(stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> user >> shad >> uid_num) {
      if (uid_num == uid) {
	stream.close();
	break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

/* pid.uptime and pid.cpuutilization both traverse the same
   file data. This utility helps cache that data to reduce load
*/
vector<string> LinuxParser::ProcStats(int pid) {
  //NB. Implementation based on discussion here
  // https://stackoverflow.com/a/23376195
  string proc = std::to_string(pid);
  string col, line;
  vector<string> row;
  
  std::ifstream stream(kProcDirectory + proc + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> col) {
     row.push_back(col);
    } 
   }
  //assert(row.size() > 22);
  return row;
}
