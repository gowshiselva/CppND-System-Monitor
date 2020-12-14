#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <math.h>       /* floor */


float Processor::Utilization() { 
    cpu_ = LinuxParser::CpuUtilization();

    float utilization(0);
          

    
    float prev_total_jiffies = 0;
    float pre_active_jiffies = 0;

    usleep(1000); // sleep 100 milliseconds    

    float post_total_jiffies = static_cast<float>(LinuxParser::Jiffies());
    float post_active_jiffies = static_cast<float>(LinuxParser::ActiveJiffies());

    float diff_active_jiffies = post_active_jiffies - pre_active_jiffies;
    float diff_total_jiffies = post_total_jiffies - prev_total_jiffies;
    
    utilization =  diff_active_jiffies / diff_total_jiffies;
    
    return utilization;
}
