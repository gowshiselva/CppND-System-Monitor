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
    //int p=pid();
    return std::stof(LinuxParser::CpuUtilization()[0]); 
}

// Return the uptime
long int Process::UpTime(){
  return LinuxParser::UpTime();
}

// overload the "less" than operator 
bool Process::operator<(Process const& a) const {
    return cpuUtilization_ < a.cpuUtilization_;
}