#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>


double Processor::Utilization() { 
    double utilization(0);
    long totalJiffiesBefore=LinuxParser::Jiffies();
    long activeJiffiesBefore=LinuxParser::Jiffies();

    usleep(1000000); // sleep 100 milliseconds    

    long totalJiffiesAfter=LinuxParser::Jiffies(); 
    long activeJiffiesAfter=LinuxParser::Jiffies(); 
    
    long tDelta = totalJiffiesAfter - totalJiffiesBefore; // total Jiffies delta
    long aDelta = activeJiffiesAfter - activeJiffiesBefore; // active Jiffies  delta

    if (tDelta == 0) {
      return 0.0;
    }

    utilization=static_cast<double>(aDelta/tDelta);

    return utilization;
}
