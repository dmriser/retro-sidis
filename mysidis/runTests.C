#include "mysidis.C"

void runTests(){

  gROOT->LoadMacro("mysidis.C"); 

  // Below are the input arguments in order. 
  //
  // Setup input files, could be a list of 
  // data files, or monte carlo files. 
  std::string inputListOfFiles("files.dat"); 

  int accIterationN = 0; 
  int Nfiles = 5; 
  int ExpOrSim = 1;
  

  bool do_momCorr_e = 1; 
  bool do_momCorr_pions = 1; 
  
  // Flags (-1,0,1) that control the 
  // tightness of different cuts and 
  // conditions applied during the analysis. 
  int e_zvertex_strict         = 0;
  int e_ECsampling_strict      = 0; 
  int e_ECoutVin_strict        = 0;
  int e_ECgeometric_strict     = 0; 
  int e_CCthetaMatching_strict = 0;
  int e_R1fid_strict           = 0; 
  int e_R3fid_strict           = 0;  
  int e_CCphiMatching_strict   = 0;
  int e_CCfiducial_strict      = 0;
  int yCut_strict              = 0;
  int pip_vvp_strict           = 0;
  int pip_R1fid_strict         = 0; 
  int pip_MXcut_strict         = 0; 
  int pim_vvp_strict           = 0;
  int pim_R1fid_strict         = 0; 
  int pim_MXcut_strict         = 0;
      
  mysidis(inputListOfFiles, accIterationN, Nfiles, ExpOrSim, do_momCorr_e, 
	  do_momCorr_pions, e_zvertex_strict, e_ECsampling_strict, 
	  e_CCthetaMatching_strict, e_R1fid_strict, e_R3fid_strict, 
	  e_CCphiMatching_strict, e_CCfiducial_strict, yCut_strict, 
	  pip_vvp_strict, pip_R1fid_strict, pip_MXcut_strict, pim_vvp_strict, 
	  pim_R1fid_strict, pim_MXcut_strict);

}
