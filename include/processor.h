#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <unordered_map>
#include<string>
// contains the 

typedef std::unordered_map<std::string, long> cpu_utilization;

class Processor {
 public:
  float Utilization();  
  
  
 private:
   cpu_utilization cpu_;
   
};

#endif