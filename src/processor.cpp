#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>


float Processor::Utilization() { 
    float totalJiffies = LinuxParser::Jiffies();
    float activeJiffies = LinuxParser::ActiveJiffies();
    return activeJiffies/totalJiffies ;}