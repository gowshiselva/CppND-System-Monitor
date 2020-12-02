#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// Return the system's CPU
Processor& System::Cpu() 
{ 
    LinuxParser::CpuUtilization().clear();
    return cpu_; }

// Return a container composed of the system's processes
std::vector<Process>& System::Processes() 
{ 
    std::vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (int pid: pids)
    {
    processes_.emplace_back(pid);
    }

    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem()
{ 
    return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); }

