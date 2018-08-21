#include "mysidis.C"
#include "programFiles/configuration.C"

void run(){

  gROOT->LoadMacro("mysidis.C"); 

  RunConfiguration testingConfig = loadRunConfiguration("example.conf"); 

  mysidis(testingConfig.inputFileList, testingConfig.numberOfFiles, 
	  testingConfig.expOrSim, testingConfig.acceptanceIteration, 
	  testingConfig.correctElectronP, testingConfig.correctPionP,
	  testingConfig.changeStrictness, testingConfig.strictToChange,
	  testingConfig.strictness);

}
