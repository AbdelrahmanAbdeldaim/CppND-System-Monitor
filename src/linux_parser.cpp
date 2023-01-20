#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() { 
  string line, key, value,kB;
  float MemoryUtilization;
  float MemTotal, MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      line.erase(remove(line.begin(), line.end(), ' '), line.end());
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'k', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value>>kB) {
        if (key == "MemTotal") {
          MemTotal = stof(value);
        }
        else if(key == "MemFree"){
          MemFree = stof(value);
        }
        else if(key == "MemAvailable"){
          MemoryUtilization = (MemTotal-MemFree)/MemTotal;
          return MemoryUtilization;
        }
      }
    }
  }
  return MemoryUtilization;
}

long LinuxParser::UpTime() { 
  string upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return std::stol(upTime);
 }


long LinuxParser::Jiffies() { 
  string line, value;
  long Jiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while (linestream >> value) {
        if(value != "cpu")
          Jiffies+= stol(value);
      }
  }
  return Jiffies;
}

long LinuxParser::ActiveJiffies() {  
  string line, value;
  long ActiveJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> value && count <4) {
        if(value != "cpu")
          ActiveJiffies+= stol(value);
        count++;
      } 
  }
  return ActiveJiffies;
}

long LinuxParser::IdleJiffies() { 
  string line, value;
  long IdleJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> value) {
        if(count == 4){
          IdleJiffies= stol(value);
          break;
        }   
        count++;
      } 
  }
  return IdleJiffies;
 }

vector<string> LinuxParser::CpuUtilization() {return {};}

long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  long ActiveJiffies = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> value && count <15) {
        if(count == 13 || count == 14)
          ActiveJiffies+= stol(value);
        count++;
      } 
  }
  return ActiveJiffies;
} 

int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}
 
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value); 
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
  std::getline(filestream, line);
  } 
  return line;
}

string LinuxParser::Ram(int pid) { 
  string key, value, kb, line;
  int vmSize;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value >> kb) {
        if (key == "VmRSS:") {
          vmSize = std::stoi(value); 
          vmSize = vmSize/1024;
          return std::to_string(vmSize);
        }
      }
    }
  }
  return value;      
}

string LinuxParser::Uid(int pid) { 
  string line,value, key;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key>> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;      
}

string LinuxParser::User(int pid) { 
  string usr, passwd, uid, line;
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
    	while (linestream >> usr >> passwd >> uid) {
               if (uid==uid_){
                return usr;
               }
        	}
      	}
    }
  return usr;
} 

long LinuxParser::UpTime(int pid) { 
  string line, value;
  long clock = sysconf(_SC_CLK_TCK);
  long startTime = 0;
  long systemUpTime = UpTime();
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> value) {
        if(count == 21)
        {
          startTime = stol(value)/clock;
          return systemUpTime-startTime;
        }
        count++;
      } 
  }
  return startTime;
 }

// float LinuxParser::CpuUtilization(int pid) {
//   string line;
//   vector<string> columns;
//   string column;
//   float util{0.0};
//   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
//   if(stream.is_open()) {
//     getline(stream, line);
//     std::istringstream linestream(line);
//     while(linestream.good()) {
//       getline(linestream, column, ' ');
//       columns.push_back(column);
//     }
//     //totalTime = utime + stime
//     // with child processes totalTime += cutime + cstime
//     int totalProcessTicks = stoi(columns[13]) + stoi(columns[14]) + stoi(columns[15]) + stoi(columns[16]);
//     float totalProcessTime = totalProcessTicks / (float)sysconf(_SC_CLK_TCK);
//     long totalSeconds = UpTime() - UpTime(pid);
//     util = totalSeconds != 0 ? (totalProcessTime/(float)totalSeconds) : 0.0;
//   }
//   return util;
// }