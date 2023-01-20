#include <string>

#include "format.h"

using std::string;
string Format::ElapsedTime(long seconds) { 
    long hours = seconds / 3600;
    seconds %= 3600;
    long minutes = seconds / 60 ;
    seconds %= 60;
    char time [50];
    sprintf( time, "%02ld:%02ld:%02ld", hours, minutes,seconds);
    return time;
}