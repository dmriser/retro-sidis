#include "mysidis.C"

void runTests(){

  gROOT->LoadMacro("mysidis.C"); 

  // We're go for run. 
  // Incomprehensible. 

  mysidis(0, 1, 10, 1, 1, 1, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0);

  /*    
  mysidis(0, 1, 1, 0, 1, 1, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0);
  */
}
