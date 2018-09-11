#ifndef LOGGING_H
#define LOGGING_H

#include <fstream>
#include <iostream>

enum logLevel {
  INFO,
  DEBUG,
  WARN,
  ERROR
};

class Logger {

 public:
 Logger(std::string outputFilename, int logLevel) : fLogfileName(outputFilename), fLogLevel(logLevel) {
    fOutputStream.open(fLogfileName, ios::out); 
  }

  ~Logger(){
    if (fOutputStream.is_open()){
      fOutputStream.close(); 
    }
  }

  void log(int level, std::string logMessage) {
    if (level <= fLogLevel){
      fOutputStream << logMessage << std::endl; 
    }

    // The program is about to die. 
    if (level == logLevel::ERROR){
      fOutputStream.close(); 
    }
  }

 protected:
  std::ofstream fOutputStream; 
  std::string   fLogfileName; 
  int           fLogLevel; 

};

#endif 
