#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
// contains the 
struct cpuData{
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
    long guest;
    long guest_nice;
};
class Processor {
 public:
  double Utilization();  // 

  // Declare any necessary private members
 private:
 std::vector<cpuData*> cpudata_;
};

#endif