#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp
  void set_Utilization(std::vector<std::string>);

  // TODO: Declare any necessary private members
 private:
  // prev_util should be passed to CPU utilization. This enables us to
  // 1. Get the all time system Utilization
  // 2. Get the n period utilzation
  float utilization;
  //static int prev_util;

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
};

#endif
