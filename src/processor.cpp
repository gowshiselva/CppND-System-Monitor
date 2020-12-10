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
    
    double tDelta = static_cast<double>(totalJiffiesAfter - totalJiffiesBefore); // total Jiffies delta
    double aDelta = static_cast<double>(activeJiffiesAfter - activeJiffiesBefore); // active Jiffies  delta

    if (tDelta == 0) {
      return 0.0;
    }
    utilization=aDelta / tDelta;

    return utilization;
}
