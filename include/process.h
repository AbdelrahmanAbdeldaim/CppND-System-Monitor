#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  long ActiveJiffies() const;                               
  Process(int pid):pid_(pid){};
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;                  
  std::string Ram();                     
  long int UpTime() const;                       
  bool operator<(Process const& a) const;  

  // TODO: Declare any necessary private members
 private:
 int pid_ {0};
 std::string user, ram,cmd;

};

#endif