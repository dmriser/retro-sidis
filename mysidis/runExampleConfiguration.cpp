#include "mysidis.C"
#include "programFiles/configuration.C"

void runExampleConfiguration(){

  gROOT->LoadMacro("mysidis.C"); 

  RunConfiguration testingConfig = loadRunConfiguration("config/example.conf"); 

  mysidis(testingConfig.inputFileList, testingConfig.numberOfFiles, 
	  testingConfig.expOrSim, testingConfig.acceptanceIteration, 
	  testingConfig.correctElectronP, testingConfig.correctPionP,
	  testingConfig.changeStrictness, testingConfig.strictToChange,
	  testingConfig.strictness, testingConfig.selectSector);

}
