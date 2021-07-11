#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:

    struct UtilData {
    // will keep duration between measurements in sync with the render
    // refresh. Longer durationsn could be kept via the instantiation
    // of a static timer based on a real clock or just a loop
    // counter % some value
    public:
      float lifetime;
      float current;
      long old_work = 0;
      long old_idle = 0;
  };

  UtilData util_data;

  Processor();
 const UtilData& Utilization();  // TODO: See src/processor.cpp


  // TODO: Declare any necessary private members
  private:
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
