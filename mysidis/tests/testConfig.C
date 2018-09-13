#include "../programFiles/configuration.C"

void testConfig(){
  RunConfiguration runConfig = loadRunConfiguration("../config/example.conf"); 
  std::cout << runConfig.inputFileList << std::endl; 
  std::cout << runConfig.numberOfFiles << std::endl; 
}
