#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include <unistd.h>
using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_ ; }

// float Process::CpuUtilization() const { return LinuxParser::CpuUtilization(pid_); }
float Process::CpuUtilization() const{ 
    long clock = sysconf(_SC_CLK_TCK);
    float activeJiffies = ActiveJiffies() /clock;
    return activeJiffies/UpTime() ; }

string Process::Command() { return LinuxParser::Command(pid_); }

long Process::ActiveJiffies() const {return LinuxParser::ActiveJiffies(pid_);};

string Process::Ram() { return LinuxParser::Ram(pid_) ; }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() const{ return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }