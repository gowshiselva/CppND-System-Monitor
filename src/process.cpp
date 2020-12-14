#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

//Process Consrtructor
Process::Process(int pid):pid_(pid)
{
    std::unordered_map<std::string, long> cpu_utilization = LinuxParser::CpuUtilization(pid_);

    utime_ = static_cast<float>(cpu_utilization["utime"]);
    stime_ = static_cast<float>(cpu_utilization["stime"]);
    cutime_ = static_cast<float>(cpu_utilization["cutime"]);
    cstime_ = static_cast<float>(cpu_utilization["cstime"]);
    starttime_ = static_cast<float>(cpu_utilization["start_time"]);
    hertz_ = static_cast<float>(sysconf(_SC_CLK_TCK));
}

// Return the pid of the process
int Process::Pid(){
    return pid_;
}

// Return the user of the process
std::string Process::User(){
    return LinuxParser::User(pid_);
}

// Return the Command 
std::string Process::Command(){
    return LinuxParser::Command(pid_);
}

// Return the cpuUtilization of this process
std::string Process::Ram(){
    return LinuxParser::Ram(pid_);
}
//  Return the aggregate CPU utilization
float Process:: CpuUtilization() { 
   
   float cpu_usage{0};
  
   long up_time = LinuxParser::UpTime();
   float total_time = utime_ + stime_ + cutime_ + cstime_;
   float seconds = up_time - (starttime_ / hertz_);


   // seconds = uptime - (starttime / Hertz)
   //float up_time = static_cast<float>(Process::UpTime());
   
   
   // cpu_usage = 100 * ((total_time / Hertz) / seconds)
   ///cpu_usage = 100 * ((total_time / Hertz) / seconds)
    cpu_usage =  (total_time / hertz_) / seconds;


    
  return cpu_usage;
}

// Return the uptime
long int Process::UpTime(){
  long up_time = LinuxParser::UpTime();
  long seconds = up_time -( starttime_ / hertz_);
  return seconds;
}

// overload the "less" than operator 
bool Process::operator<(Process const& a) const {
    return cpuUtilization_ < a.cpuUtilization_;
}