#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
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
float LinuxParser::MemoryUtilization() { 
  return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  long upTime=0;
  long idleTime=0;
  if (filestream.is_open())
  {
    std::string line;
    std::getline(filestream,line);

    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;

  }
  return upTime;
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
long LinuxParser::ActiveJiffies() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream,line);
    std::istringstream linestream;


    
  }

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  long uptime=0;
  long idletime=0;

  if(filestream.is_open())
  {
  std::string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  std::string dummy;
  long uTime, sTime, cUtime, cStime, startTime;

  for(auto i=0; i<13; ++i) linestream >> dummy;
  linestream >> uTime >> sTime >> cUtime >> cStime;
  for(auto i = 0; i < 4; i++) linestream >> ignore;
  linestream >> startTime;
  return (uTime + sTime + cUtime + cStime +starttime);
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
  long user, nice, system, idle, iowait, irq, softirg, steal, guess, guessnice;
  linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guess >> guessnice;
  long totalUserTime = user - guess;
  long totalNiceTime = nice - guessnice;
  long totalIdleTime = idle + iowait;
  long totalSystem = system + irq + softirq;
  long totalVirtualTime = guess + guessnice;
    return totalIdleTime;
  }
 
  return 0; 
}

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  std::ifstream filestream (kProcDirectory + kStatFilename);
  long totalProcess=0;

  if(filestream.is_open())
  {
    std::string line;
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      std::string key;
      
      linestream >> key;

      if (key=="processes")
      {
        return totalProcess;
        break;
      }
    }
  }
  return totalProcess; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int proces_running=0;

  if(filestream.is_open()){
    std::string line;
    while(std::getline(filestream,line)) {
      std::istringstream linestream(line);
      std::string key;
      linestream >> key >> proces_running;

      if(key=="procs_running"){        
        return proces_running
    }
    }
  }
  return 0; }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::stringstream filename;
  filename << kProcDirectory << "/" << pid << "/" <<  kCmdlineFilename;
  std::ifstream (filename.str());

  std::string cmd;
  if (filestream.is_open()){
    std::getline(filstream,cmd)
  }
  
  return cmd; 
  }

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::stringstream filename;
  filename << kProcDirectory << "/" << pid << "/" << kStatusFilename;
  std::ifstream filestream(filename.str());
  long memory=1;
  std::string unit="";
  
  if(filestream.is_open()){
    std::string line;
    while(std::getline(filestream, line)){
    std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "VmSize:") {
          linestream >> memory >> unit;
          break;
        }
          
    }
  }
  std::ostringstream ostream;
  ostream << memory/1024 ;
  return ostream.str(); 
  }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string filename;
  filename << kProcDirectory << "/" << pid << "/" << kStatusFilename;
  std::ifstream filestream(filename.str());
  std::string uid = " ";

  if(filestream.is_open()){
    std::string line;

    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "Uid:") {
          linestream >> uid;          
        }
    }

  }
  return uid; 
  }

// Read and return the user associated with a process
//  once you define the function
string LinuxParser::User(int pid) { 
  std::string uid = Uid(pid);
  std::string userName="No Username";
  std::ifstream filestream(kPasswordPath);
  long runningProcesses = 0;
  
  if (filestream.is_open()){
    std::string line;

    while(std::getline(filestream,line)){
      std::replace(line,begin()m line.end(),' ', '_');
      std::replace(line,begin()m line.end(),':', ' ');

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
long LinuxParser::UpTime(int pid) { 
  std::stringstream filename;
  filename << kProcDirectory << "/" << pid << "/" << kStatFilename;
  std::ifstream filestream(filename.str());
  long startTime=0;
  if(filename.is_open()){
    std::string line;
    std::getline(filestream,line);
    std::string dummy;
      for(int i = 0; i < 21; i++) linestream >> dummy;
      linestream >> startTime;
      struct timeval tv;
      gettimeofday(&tv, 0);
      std::time_t now = std::time(0);
      std::time_t elapsedTime = LinuxParser::UpTime() - (startTime/sysconf(_SC_CLK_TCK));
      return elapsedTime;
  }
  return startTime;
}