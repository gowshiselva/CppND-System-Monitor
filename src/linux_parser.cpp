#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include <sys/time.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;






// define helper template functions

template <typename T>
T getValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream filestream(LinuxParser::kProcDirectory + filename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key="Linux";
  std::string value;
  std::ifstream filestream(LinuxParser::kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
//  Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{ 
  float memoryutil{0}; 
  float Total = getValueByKey<float>(filterMemTotalString, kMeminfoFilename);// "/proc/memInfo"
  float Free = getValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  memoryutil= (Total - Free) / Total;
  return memoryutil;
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{   
  return getValueOfFile<long>(LinuxParser::kUptimeFilename);  
}


LinuxParser::processData LinuxParser::extractProcessData(int pid)
{
  std::stringstream filename;
  filename << kProcDirectory << "/" << pid << "/" << kStatFilename;
  std::ifstream filestream(filename.str());
    LinuxParser::processData data;
    if (filestream.is_open())
     {
      std::string line;
      std::getline(filestream, line);
      std::istringstream linestream(line);
      std::string dummy;
      long   usertime, systemtime, cutime, cstime, starttime;
      for(int i = 0; i < 13; i++) {
        linestream >> dummy;
      }
      linestream >> usertime >> systemtime >> cutime >> cstime;
      for(int i = 0; i < 4; i++) linestream >> dummy;
      linestream >> starttime;
        data.seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
        data.totalTime = (usertime + systemtime + cutime + cstime)/sysconf(_SC_CLK_TCK);

    }
    return data;
}

//Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization()
{
 std::vector<std::string> cpuUtils{};
 std::string line;
 std::string key;
 std::string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

 std::ifstream filestream(kProcDirectory + kStatFilename);

 if (filestream.is_open())
  {
   while (std::getline(filestream, line)) 
   {
    std::istringstream linestream(line);
    while (linestream >> key >> user >> nice >> system >> idle >> iowait 
                        >> irq >> softirq >> steal >> guest >>  guest_nice)
       {
          if (key == "cpu")
            {
          cpuUtils = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
          break;
            }
       }
   } 
  }
  return cpuUtils;
 }
//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open())
  {
    std::string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    long  cpu, usertime, nicetime, systemtime, idletime;
    long  ioWait, irq, softIrq, steal, guest, guestnice;
    ioWait = irq = softIrq = steal = guest = guestnice = 0;
    linestream >> cpu >> usertime >> nicetime >> systemtime >> idletime >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;

  // Guest time is already accounted in usertime
    usertime = usertime - guest;
    nicetime = nicetime - guestnice;

      
    long  idlealltime = idletime + ioWait;
    long  systemalltime = systemtime + irq + softIrq;
    long  virtalltime = guest + guestnice;
    //long  totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
    long  jiffies = usertime + nicetime+ idlealltime + systemalltime +virtalltime;
    return jiffies; 
    }
  return 0;


}

// Read and return the number of active jiffies for a PID

//refer https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
//refer:https://github.com/htop-dev/htop/blob/15652e7b8102e86b3405254405d8ee5d2a239004/linux/LinuxProcessList.c
//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open())
  {
  std::string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  std::string dummy;
  long uTime, sTime, cUtime, cStime, startTime;

  for(auto i=0; i<13; ++i) linestream >> dummy;
  linestream >> uTime >> sTime >> cUtime >> cStime;
  for(auto i=0; i<4; ++i) linestream >> dummy;
  linestream >> startTime;
  return (uTime + sTime + cUtime + cStime +startTime);
  }  
  
  return 0;
} 

long LinuxParser::ActiveJiffies(int pid)
{
  std::stringstream filename;
  filename << kProcDirectory << "\\" << pid << kStatFilename ;
  std::ifstream filestream(filename.str());

  if(filestream.is_open())
  {
    std::string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  std::string dummy;
  long uTime, sTime, cUtime, cStime, startTime;

  for(auto i=0; i<13; ++i) linestream >> dummy;
  linestream >> uTime >> sTime >> cUtime >> cStime;
  for(auto i=0; i<4; ++i) linestream >> dummy;
  linestream >> startTime;
  return (uTime + sTime + cUtime + cStime +startTime);
    }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  std::string os, kernel, line;  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) 
  {      
    std::istringstream linestream(line);

    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal, guess, guessnice;
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guess >> guessnice;
  
    long totalIdleTime = idle + iowait;
  
    return totalIdleTime;
  }
 
  return 0; 
}

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return getValueByKey<int>(filterProcesses, kStatFilename); 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return getValueByKey<int>(filterRunningProcesses, kStatFilename); 
  }

// Read and return the command associated with a process
//cmdline :Shows the parameters that were passed to the kernel at boot time
std::string LinuxParser::Command(int pid)
 { 
  return std::string(getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}


//Read and return the memory used by a process
std::string LinuxParser::Ram(int pid)
{ 
  return std::to_string(std::stol(getValueByKey<std::string>(filterProcMem, std::to_string(pid) + kStatusFilename)) / 1024); 
}

// Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) 
{  
 return getValueByKey<std::string>(filterUID, std::to_string(pid) + kStatusFilename);
}

// Read and return the user associated with a process
//  once you define the function
std::string LinuxParser::User(int pid)
{ 
  std::string uid = Uid(pid);
  std::string userName="No Username";
  std::ifstream filestream(kPasswordPath);
 // long runningProcesses = 0;
  
  if (filestream.is_open())
  {
    std::string line;
    while(std::getline(filestream,line))
    {
      std::replace(line.begin(), line.end(),' ', '_');
      std::replace(line.begin(), line.end(),':', ' ');
      std::istringstream linestream(line);
      std::string pwd, currentUID;
      linestream >> userName >> pwd >> currentUID;
      if (currentUID == uid)
        {          
             return userName; 
        }
    }
  }
   return userName; 
}

// Read and return the uptime of a process
//A short file that has only two numbers: how many seconds your box has been up, and how many seconds it has been idle.
long LinuxParser::UpTime(int pid) { 
  std::string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    for (int i = 0; i < 22; ++i) filestream >> value;
      return LinuxParser::UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
  }
  return 0;
  
}