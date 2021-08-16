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

/* General Notes:
 * - OO correctness: One could spend some considerable time debating the
 *   correctness of OO code in the exercise and in the final implementation.
 *   I have used what seems reasonable given time and existing code.
 * - Errors: There are many sources of potential failures. Read or bad variable errors
 *   are generally handled by passing a signal to the requesting method/class
 *   and leaving the implementation to be handled there.
 * - Jiffies seem to be an anachonism and IMO completely superfluos to this
 *   excercise. From the number of questions regarding this topic they also seem
 *   to be a barrier to understanding. 
 *  
 */


// DONE: AN EXAMPLE OF how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  string os = "No OS detected";

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
          os = value;
	  filestream.close();
	  break;
        }
      }
    }
  }
  return os;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, line, kernel;
  string current_kernel = "Error";
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
    current_kernel = kernel;
  } 
  return current_kernel;
}

// BONUS: Update this to use std::filesystem
// See separate branch for the alternative implementation
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  const float error = -1.0;
  float mem;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    int rows = 0;
    while (rows < 1) {
      std::getline(filestream, line);
      rows++;
    }
    if(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      mem = std::stof(value);
      filestream.close();
    } else { mem = error; }
  } else { mem = error; } //1

  return mem;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string raw_time;
  string line;
  long time = -1; // Default error value
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> raw_time;
    time = std::stol(raw_time);
    stream.close();
  }
  return time;
}

// Done: Read and return CPU utilization
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
// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int procs = -1; //Default value in case of failure

  std::ifstream stream(kProcDirectory + kStatFilename);
  while(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
        if (key == "procs_running") {
          procs = std::stoi(value);
	  stream.close();
	  break;
        }
    }
  }
  return procs;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string proc = std::to_string(pid);
  std::string line;
  std::string command = "No entry in /proc/" + proc + "/cmdline";
  std::ifstream stream(kProcDirectory + proc + kCmdlineFilename);
  if(stream.is_open()) {
    if(std::getline(stream, line)) {
      command = line;
    }
  }
  return command;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  // VmSize not allways recorded for a process e.g. see:
  // https://unix.stackexchange.com/questions/500212/
  // Similar effects observed in quemu/kvm/libvirt vms
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
	  result = value;
          stream.close();
	  break;
	}
      }
    } else {
      stream.close();
      break;
    }
  }
  return result;;
}

// Done: Read and return the user ID associated with a process
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

// Done: Read and return the user associated with a process uid
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

// REDUNDANT: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }


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
