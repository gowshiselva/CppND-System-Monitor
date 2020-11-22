#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// TODO: Return the system's CPU
Processor& System::Cpu() 
{ 
    LinuxParser::CpuUtilization().clear();
    return cpu_; }

// TODO: Return a container composed of the system's processes
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


    /*
processes_.clear()<
    std::vector<int> pids=LinuxParser::Pids();
    
    // create a set
    std::set<int> active_pids;

    for (Process const& process :processes_)
    {
        active_pids.insert(process.Pid());
    }
    
    //emplace all new processs
    for(pid:pids)
    {
        if(active_pids.find(pid) == active_pids.end())
        {
         processes_.emplace_back(pid);
        }         
    }

    //update CPU utilization
    for( auto& process:processes_)
    {
        Process.CpuUtilization_(LinuxParser::ActiveJiffies(process.Pid()/
                                        Linux::Parser::Jiffies());
    }
    
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>() )
    return processes_;
    */