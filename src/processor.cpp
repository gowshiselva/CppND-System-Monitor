#include "processor.h"
#include "linux_parser.h"


float Processor::Utilization() { 
    float utilization(0);
    long activeJiffies=LinuxParser::ActiveJiffies();
    long idleJiffies=LinuxParser::IdleJiffies();
   // long  active_duration{activeJiffies-cached_activeJiffies};
   // long  idle_duration{idleJiffies-cached_activeJiffies};
   // long  duration{active_duration_idle_duration};
   // utilization=static_cast<float>(active_duration*100/idle_duration)
   utilization=static_cast<float>(activeJiffies*100/idleJiffies);
   // cached_activeJiffies=activeJiffies;
    //cached_idleJiffies=idleJiffies;
    return utilization;
}
