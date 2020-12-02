#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"}; // not in use
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

//define usefull datas
const std::string filterProcesses{"processes"};
const std::string filterRunningProcesses{"procs_running"};
const std::string filterMemTotalString{"MemTotal:"};
const std::string filterMemFreeString{"MemFree:"};
const std::string filterCpu{"cpu"};
const std::string filterUID{"Uid:"};
const std::string filterProcMem{"VmData:"}; // The string can be VmSize As well


// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
struct processorData{
  float user;
  float nice;
  float system;
  float idle;
  float iowait;
  float irq;
  float softirq;
  float steal;
  float guest;
  float guest_nice;
};

//define process string metrics
const std::string key{};
const std::string cpu{};
const std::string user{}; // time spent in the user mode
const std::string nice{}; // time spent in usermode with the low priority
const std::string system{}; // Time spent in system mode.
const std::string idle{}; //  Time spent in the idle task.  This value 
                        //should be USER_HZ times the second entry in the
                        ///proc/uptime pseudo-file.
const std::string iowit{}; // Time waiting for I/O to complete.  This value is not reliable, for the following rea‐sons:
const std::string irq{}; // Time servicing interrupts.

const std::string softirq{}; // Time servicing softirqs.

const std::string steal{}; // Stolen time, which is the time spent in operating systems under the onctrol of the linux
const std::string guest_nice{}; // Time spent running a niced guest (visrtual CPU guest for guest operating systems under the control of the linux)



struct processData
{
   float seconds;
   float totalTime;
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
processData extractProcessData(int pid);
};  // namespace LinuxParser
#endif