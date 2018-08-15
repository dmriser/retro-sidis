#include "mysidis.C"

void runTests(){

  gROOT->LoadMacro("mysidis.C"); 

  // We're go for run. 
  // Incomprehensible. 

  /* 
  mysidis(0, 1, 1, 1, 1, 1, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0);
  */

      
  mysidis("files.dat", 0, 2, 1, 0, 1, 1, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 
	  0, 0, 0, 0);

}
