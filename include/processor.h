#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:

  float lifetime;
  float current;

  Processor();
  const std::vector<float> Utilization(); 


  // Done: Declare any necessary private members
  private:
  long old_work = 0;
  long old_idle = 0;

  void set_Utilization(const std::vector<std::string>);  
    
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
