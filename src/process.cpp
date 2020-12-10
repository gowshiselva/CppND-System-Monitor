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
   //total_time = utime + stime
   std::unordered_map<std::string, long> cpu_utilization = LinuxParser::CpuUtilization(pid_);
   float cpu_usage{0};
   if(cpu_utilization.empty())
   {   
       cpu_usage = 0;
       return cpu_usage;
   }
   //long total_time = cpu_utilization["utime"] + cpu_utilization["stime"] + cpu_utilization["cutime"] + cpu_utilization["cstime"];
   float total_time = static_cast<float>(cpu_utilization["utime"] + cpu_utilization["stime"] + cpu_utilization["cutime"] + cpu_utilization["cstime"]);

   // seconds = uptime - (starttime / Hertz)
   float up_time = static_cast<float>(Process::UpTime());
   float herts = static_cast<float>(sysconf(_SC_CLK_TCK));
   float seconds = up_time - (static_cast<float>(cpu_utilization["starttime"]) / herts);
   
   // cpu_usage = 100 * ((total_time / Hertz) / seconds)
   //cpu_usage =  ((static_cast<float>(total_time) /static_cast<float>(herts) ) / static_cast<float>(seconds));
    cpu_usage =  (total_time / herts) / seconds;


    //cpu_usage = 100 * ((total_time / Hertz) / seconds)
  return cpu_usage;
}

// Return the uptime
long int Process::UpTime(){
  return LinuxParser::UpTime();
}

// overload the "less" than operator 
bool Process::operator<(Process const& a) const {
    return cpuUtilization_ < a.cpuUtilization_;
}