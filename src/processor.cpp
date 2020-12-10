#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <math.h>       /* floor */


float Processor::Utilization() { 
    float utilization(0);
    float totalJiffiesBefore=static_cast<float>(LinuxParser::Jiffies());
    float activeJiffiesBefore=static_cast<float>(LinuxParser::ActiveJiffies());

    usleep(100); // sleep 100 milliseconds    

    float totalJiffiesAfter=static_cast<float>(LinuxParser::Jiffies()); 
    float activeJiffiesAfter=static_cast<float>(LinuxParser::ActiveJiffies()); 
    
    float tDelta = totalJiffiesAfter - totalJiffiesBefore; // total Jiffies delta
    float aDelta = activeJiffiesAfter - activeJiffiesBefore; // active Jiffies  delta

    if (tDelta == 0) {
      return 0.0;
    }
    utilization=floor(aDelta / tDelta);

    return utilization;
}
