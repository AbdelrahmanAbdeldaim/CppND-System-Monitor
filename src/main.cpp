#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"
#include<iostream>
int main() {
  System system;
  // Process process(4561);
  // {
  //   std::cout << process.CpuUtilization() *100<< std::endl;
  // }
  NCursesDisplay::Display(system);
}