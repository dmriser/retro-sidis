
// c++ standard libraries 
#include <fstream>
#include <iostream>

// Root standard libraries 
#include "TCanvas.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TTree.h"
#include "TVector3.h"

// This project. 
#include "programFiles/functions.C"
#include "programFiles/eID.C"
#include "programFiles/getGenIndices.C"
#include "programFiles/hadronID.C"
#include "programFiles/hadronIDFast.C"
#include "programFiles/MomCorr.C"

/* -----------------------------------------------------------

   Author:    Nathan Harrison 
   Revisions: David Riser 

   Intent: 
   This function processes the E1-F dataset 
   for one configuration of systematic uncertainty 
   values, defaults are all zero.  The output is a 
   file that contains the 5-dimensional counts.  This 
   file needs to be run over data and MC in different 
   runs. 
   
   General Notes: 
   (1) expOrSim follows the convention (0, MC) and (1, data). 
   (2) The tree variables tl3_x, tl1_cx used to be called 
   dc_xsc and dc_cxsc in h10 tree format.

   Change log: 
   08-10-2018: Creating this log.  Reformatting spacing.
   08-13-2018: Continuing refactoring constants. 
   08-14-2018: Beginning to refactor the hadronic identificaiton 
       functions.  Before doing so I took a benchmark, which is below. 
       Before 1 -> Event rate: 10714.6 events/sec.
       Before 2 -> Event rate: 7207.22 events/sec.    
       Before 3 -> Event rate: 9004.69 events/sec.
       After 1  -> Event rate: 32495.9 events/sec.
       After 2  -> Event rate: 38732.2 events/sec.
       After 3  -> Event rate: 41106.9 events/sec.
       After 4  -> Event rate: 51929.8 events/sec.

   08-17-2018: Added configuration file setup to programFiles/configuration.C.  
       changing the input of this function to accept just a few arguments. 
   09-13-2018: Changing the path for parameter files. 
   -----------------------------------------------------------
*/

 
//void mysidis(std::string inputListOfFiles, int accIterationN = 0, int Nfiles = 5, int expOrSim = 1, 
//	     bool do_momCorr_e = 1, bool do_momCorr_pions = 1, int e_zvertex_strict = 0, 
//	     int e_ECsampling_strict = 0, int e_ECoutVin_strict = 0, 
//	     int e_ECgeometric_strict = 0, int e_CCthetaMatching_strict = 0, 
//	     int e_R1fid_strict = 0, int e_R3fid_strict = 0, int e_CCphiMatching_strict = 0, 
//	     int e_CCfiducial_strict = 0, int yCut_strict = 0, int pip_vvp_strict = 0, 
//	     int pip_R1fid_strict = 0, int pip_MXcut_strict = 0, int pim_vvp_strict = 0, 
//	     int pim_R1fid_strict = 0, int pim_MXcut_strict = 0){

void mysidis(std::string inputFileList, int numberOfFiles, bool expOrSim, 
	     int acceptanceIteration, bool correctElectronP, bool correctPionP,
	     bool changeStrictness, int strictToChange, int strictness){
  
  // Constant declaration block (reserved for anything
  // that does not change throughout the run of the code.)
  // By convention, these variables are named with all 
  // capitol letters. 
  const static int BUFFER_SIZE   = 16; 
  const static int MC_VERSION    = 12;
  const static int INVALID_INDEX = -123; 
  
  // Floats, this convention may be different 
  // when declaring constant floats in c++11. 
  const Float_t e_mass         = 0.000511;      // GeV
  const Float_t prot_mass      = 0.938272;      // GeV
  const Float_t pip_mass       = 0.13957;       // GeV
  const Float_t pim_mass       = 0.13957;       // GeV
  const Float_t speed_of_light = 29.9792458;    // cm/ns
  const Float_t Beam_Energy    = 5.498;         // GeV
  const Float_t pi             = TMath::Pi();   // Should be replaced everywhere, this is a quick fix. 
  const Float_t pi180          = pi/180.0;      // Really not sure why root decided to make it a function and not a constant.
  const Float_t pi180_inv      = 180.0/pi;
  
  // Setup the strictness values based on the input 
  // of this module. Sixteen different flags are given 
  // an index between 0-15. 
  int e_zvertex_strict         = 0;//  0
  int e_ECsampling_strict      = 0;//  1
  int e_ECoutVin_strict        = 0;//  2
  int e_ECgeometric_strict     = 0;//  3
  int e_CCthetaMatching_strict = 0;//  4
  int e_R1fid_strict           = 0;//  5
  int e_R3fid_strict           = 0;//  6
  int e_CCphiMatching_strict   = 0;//  7
  int e_CCfiducial_strict      = 0;//  8
  int yCut_strict              = 0;//  9
  int pip_vvp_strict           = 0;// 10
  int pip_R1fid_strict         = 0;// 11
  int pip_MXcut_strict         = 0;// 12
  int pim_vvp_strict           = 0;// 13
  int pim_R1fid_strict         = 0;// 14
  int pim_MXcut_strict         = 0;// 15

  // If we're asked to change one of the 
  // strictnesses, let's do it and go on 
  // with our lives!
  // As Nathan points out, we can change 
  // pi+ and pi- cuts at the same time.  
  if (changeStrictness){
    std::cout << "Changing strictness: " << strictToChange << " to level " << strictness << std::endl; 
    
    // Hideous block. 
    if (strictToChange == 0){
      e_zvertex_strict = strictness; 
    } 
    else if (strictToChange == 1){
      e_ECsampling_strict = strictness; 
    } 
    else if (strictToChange == 2){
      e_ECoutVin_strict = strictness; 
    } 
    else if (strictToChange == 3){
      e_ECgeometric_strict = strictness; 
    } 
    else if (strictToChange == 4){
      e_CCthetaMatching_strict = strictness; 
    } 
    else if (strictToChange == 5){
      e_R1fid_strict = strictness; 
    } 
    else if (strictToChange == 6){
      e_R3fid_strict = strictness; 
    } 
    else if (strictToChange == 7){
      e_CCphiMatching_strict = strictness; 
    } 
    else if (strictToChange == 8){
      e_CCfiducial_strict = strictness; 
    } 
    else if (strictToChange == 9){
      yCut_strict = strictness; 
    } 
    else if (strictToChange == 10){
      pip_vvp_strict = strictness; 
      pim_vvp_strict = strictness; 
    } 
    else if (strictToChange == 11){
      pip_R1fid_strict = strictness; 
      pim_R1fid_strict = strictness; 
    } 
    else if (strictToChange == 12){
      pip_MXcut_strict = strictness; 
      pim_MXcut_strict = strictness; 
    } 
  }

  // Time tracking and benchmarking. 
  TStopwatch runtimeCounter;
  runtimeCounter.Reset(); 
  runtimeCounter.Start(); 

  // Class produced by Marco Mirazita.
  MomCorr_e1f *MomCorr = new MomCorr_e1f("requiredFiles/parameters/momentumCorrections/");

  // Classes for hadronic identification rely on the 
  // same login as the original function.  The only 
  // difference is the single load of the parameters. 
  BetaPTable *pipTable = new BetaPTable("requiredFiles/parameters/pip/", "pip"); 
  BetaPTable *pimTable = new BetaPTable("requiredFiles/parameters/pim/", "pim"); 

  // %%%%% some cut values %%%%%
  float WMin = 2.05;
  float yMax = 0.85; // default value (for when yCut_strict = 0)
  if(yCut_strict == 1) yMax = 0.8;
  // %%%%% end some cut values %%%%%
  
  // %%%%% x, QQ, z, PT2 bin definitions %%%%%
  // When you change this, there are two spots below that you have to update. 
  //
  // I haven't found those places yet.  I hope this doesn't introduce 
  // unreproducible results. 
  int binSchemeOpt = 5; 
  
  int NxBins, NQQBins, NzBins_pip, NPT2Bins_pip, NzBins_pim, NPT2Bins_pim;
  int NphihBins;
  float phihMin, phihMax;
  
  if(binSchemeOpt == 5){
    NxBins       = 5;
    NQQBins      = 2;
    NzBins_pip   = 18;
    NPT2Bins_pip = 20;
    NzBins_pim   = 18;
    NPT2Bins_pim = 20;
    NphihBins    = 36;
    phihMin      = -180;
    phihMax      = 180;
  }
  
  // consistent with Osipenko's binning
  // exception here, Osipenko uses 0-360, but -180-180 is better 
  // so I'll convert his results to this convention
  else if(binSchemeOpt == 6){
    NxBins       = 19;
    NQQBins      = 10;
    NzBins_pip   = 22;
    NPT2Bins_pip = 10;
    NzBins_pim   = 1;
    NPT2Bins_pim = 1;
    NphihBins    = 18;
    phihMin      = -180;
    phihMax      = 180;
  }
  
  float xLimits[NxBins + 1], QQLimits[NxBins][NQQBins + 1], zLimits_pip[NzBins_pip + 1], PT2Limits_pip[NPT2Bins_pip + 1], zLimits_pim[NzBins_pim + 1], PT2Limits_pim[NPT2Bins_pim + 1];
  
  if(binSchemeOpt == 5){
    xLimits[0] = 0.1; xLimits[1] = 0.2; xLimits[2] = 0.3; xLimits[3] = 0.4; xLimits[4] = 0.5; xLimits[5] = 0.6;
    QQLimits[0][0] = 1.0; QQLimits[0][1] = 1.3; QQLimits[0][2] = 5.0; QQLimits[1][0] = 1.0; QQLimits[1][1] = 1.7; QQLimits[1][2] = 5.0; QQLimits[2][0] = 1.0; QQLimits[2][1] = 2.2; QQLimits[2][2] = 5.0; QQLimits[3][0] = 1.0; QQLimits[3][1] = 2.9; QQLimits[3][2] = 5.0; QQLimits[4][0] = 1.0; QQLimits[4][1] = 5.0; QQLimits[4][2] = 99.9;
    zLimits_pip[0] = 0.0; zLimits_pip[1] = 0.05; zLimits_pip[2] = 0.10; zLimits_pip[3] = 0.15; zLimits_pip[4] = 0.20; zLimits_pip[5] = 0.25; zLimits_pip[6] = 0.30; zLimits_pip[7] = 0.35; zLimits_pip[8] = 0.40; zLimits_pip[9] = 0.45; zLimits_pip[10] = 0.50; zLimits_pip[11] = 0.55; zLimits_pip[12] = 0.60; zLimits_pip[13] = 0.65; zLimits_pip[14] = 0.70; zLimits_pip[15] = 0.75; zLimits_pip[16] = 0.80; zLimits_pip[17] = 0.85; zLimits_pip[18] = 0.90;
    PT2Limits_pip[0] = 0.0; PT2Limits_pip[1] = 0.05; PT2Limits_pip[2] = 0.10; PT2Limits_pip[3] = 0.15; PT2Limits_pip[4] = 0.20; PT2Limits_pip[5] = 0.25; PT2Limits_pip[6] = 0.30; PT2Limits_pip[7] = 0.35; PT2Limits_pip[8] = 0.40; PT2Limits_pip[9] = 0.45; PT2Limits_pip[10] = 0.50; PT2Limits_pip[11] = 0.55; PT2Limits_pip[12] = 0.60; PT2Limits_pip[13] = 0.65; PT2Limits_pip[14] = 0.70; PT2Limits_pip[15] = 0.75; PT2Limits_pip[16] = 0.80; PT2Limits_pip[17] = 0.85; PT2Limits_pip[18] = 0.90; PT2Limits_pip[19] = 0.95; PT2Limits_pip[20] = 1.00;
    zLimits_pim[0] = 0.0; zLimits_pim[1] = 0.05; zLimits_pim[2] = 0.10; zLimits_pim[3] = 0.15; zLimits_pim[4] = 0.20; zLimits_pim[5] = 0.25; zLimits_pim[6] = 0.30; zLimits_pim[7] = 0.35; zLimits_pim[8] = 0.40; zLimits_pim[9] = 0.45; zLimits_pim[10] = 0.50; zLimits_pim[11] = 0.55; zLimits_pim[12] = 0.60; zLimits_pim[13] = 0.65; zLimits_pim[14] = 0.70; zLimits_pim[15] = 0.75; zLimits_pim[16] = 0.80; zLimits_pim[17] = 0.85; zLimits_pim[18] = 0.90;
    PT2Limits_pim[0] = 0.0; PT2Limits_pim[1] = 0.05; PT2Limits_pim[2] = 0.10; PT2Limits_pim[3] = 0.15; PT2Limits_pim[4] = 0.20; PT2Limits_pim[5] = 0.25; PT2Limits_pim[6] = 0.30; PT2Limits_pim[7] = 0.35; PT2Limits_pim[8] = 0.40; PT2Limits_pim[9] = 0.45; PT2Limits_pim[10] = 0.50; PT2Limits_pim[11] = 0.55; PT2Limits_pim[12] = 0.60; PT2Limits_pim[13] = 0.65; PT2Limits_pim[14] = 0.70; PT2Limits_pim[15] = 0.75; PT2Limits_pim[16] = 0.80; PT2Limits_pim[17] = 0.85; PT2Limits_pim[18] = 0.90; PT2Limits_pim[19] = 0.95; PT2Limits_pim[20] = 1.00;
  }
  
  else if(binSchemeOpt == 6){
    xLimits[0] = 0.128; xLimits[1] = 0.161; xLimits[2] = 0.1905; xLimits[3] = 0.221; xLimits[4] = 0.2525; xLimits[5] = 0.2855; xLimits[6] = 0.32; xLimits[7] = 0.356; xLimits[8] = 0.394; xLimits[9] = 0.4335; xLimits[10] = 0.4745; xLimits[11] = 0.5175; xLimits[12] = 0.5625; xLimits[13] = 0.6095; xLimits[14] = 0.6585; xLimits[15] = 0.7095; xLimits[16] = 0.763; xLimits[17] = 0.8185; xLimits[18] = 0.8765; xLimits[19] = 0.9425;
    
    QQLimits[0][0] = 1.38; QQLimits[0][1] = 1.62; QQLimits[0][2] = 1.88; QQLimits[0][3] = 2.19; QQLimits[0][4] = 2.65; QQLimits[0][5] = 3.18; QQLimits[0][6] = 3.76; QQLimits[0][7] = 4.47; QQLimits[0][8] = 5.28; QQLimits[0][9] = 6.165; QQLimits[0][10] = 7.05;
    
    QQLimits[1][0] = 1.38; QQLimits[1][1] = 1.62; QQLimits[1][2] = 1.88; QQLimits[1][3] = 2.19; QQLimits[1][4] = 2.65; QQLimits[1][5] = 3.18; QQLimits[1][6] = 3.76; QQLimits[1][7] = 4.47; QQLimits[1][8] = 5.28; QQLimits[1][9] = 6.165; QQLimits[1][10] = 7.05;
    
    QQLimits[2][0] = 1.38; QQLimits[2][1] = 1.62; QQLimits[2][2] = 1.88; QQLimits[2][3] = 2.19; QQLimits[2][4] = 2.65; QQLimits[2][5] = 3.18; QQLimits[2][6] = 3.76; QQLimits[2][7] = 4.47; QQLimits[2][8] = 5.28; QQLimits[2][9] = 6.165; QQLimits[2][10] = 7.05;
    
    QQLimits[3][0] = 1.38; QQLimits[3][1] = 1.62; QQLimits[3][2] = 1.88; QQLimits[3][3] = 2.19; QQLimits[3][4] = 2.65; QQLimits[3][5] = 3.18; QQLimits[3][6] = 3.76; QQLimits[3][7] = 4.47; QQLimits[3][8] = 5.28; QQLimits[3][9] = 6.165; QQLimits[3][10] = 7.05;
    
    QQLimits[4][0] = 1.38; QQLimits[4][1] = 1.62; QQLimits[4][2] = 1.88; QQLimits[4][3] = 2.19; QQLimits[4][4] = 2.65; QQLimits[4][5] = 3.18; QQLimits[4][6] = 3.76; QQLimits[4][7] = 4.47; QQLimits[4][8] = 5.28; QQLimits[4][9] = 6.165; QQLimits[4][10] = 7.05;
    
    QQLimits[5][0] = 1.38; QQLimits[5][1] = 1.62; QQLimits[5][2] = 1.88; QQLimits[5][3] = 2.19; QQLimits[5][4] = 2.65; QQLimits[5][5] = 3.18; QQLimits[5][6] = 3.76; QQLimits[5][7] = 4.47; QQLimits[5][8] = 5.28; QQLimits[5][9] = 6.165; QQLimits[5][10] = 7.05;
    
    QQLimits[6][0] = 1.38; QQLimits[6][1] = 1.62; QQLimits[6][2] = 1.88; QQLimits[6][3] = 2.19; QQLimits[6][4] = 2.65; QQLimits[6][5] = 3.18; QQLimits[6][6] = 3.76; QQLimits[6][7] = 4.47; QQLimits[6][8] = 5.28; QQLimits[6][9] = 6.165; QQLimits[6][10] = 7.05;
    
    QQLimits[7][0] = 1.38; QQLimits[7][1] = 1.62; QQLimits[7][2] = 1.88; QQLimits[7][3] = 2.19; QQLimits[7][4] = 2.65; QQLimits[7][5] = 3.18; QQLimits[7][6] = 3.76; QQLimits[7][7] = 4.47; QQLimits[7][8] = 5.28; QQLimits[7][9] = 6.165; QQLimits[7][10] = 7.05;
    
    QQLimits[8][0] = 1.38; QQLimits[8][1] = 1.62; QQLimits[8][2] = 1.88; QQLimits[8][3] = 2.19; QQLimits[8][4] = 2.65; QQLimits[8][5] = 3.18; QQLimits[8][6] = 3.76; QQLimits[8][7] = 4.47; QQLimits[8][8] = 5.28; QQLimits[8][9] = 6.165; QQLimits[8][10] = 7.05;
        
    QQLimits[9][0] = 1.38; QQLimits[9][1] = 1.62; QQLimits[9][2] = 1.88; QQLimits[9][3] = 2.19; QQLimits[9][4] = 2.65; QQLimits[9][5] = 3.18; QQLimits[9][6] = 3.76; QQLimits[9][7] = 4.47; QQLimits[9][8] = 5.28; QQLimits[9][9] = 6.165; QQLimits[9][10] = 7.05;
    
    QQLimits[10][0] = 1.38; QQLimits[10][1] = 1.62; QQLimits[10][2] = 1.88; QQLimits[10][3] = 2.19; QQLimits[10][4] = 2.65; QQLimits[10][5] = 3.18; QQLimits[10][6] = 3.76; QQLimits[10][7] = 4.47; QQLimits[10][8] = 5.28; QQLimits[10][9] = 6.165; QQLimits[10][10] = 7.05;
    
    QQLimits[11][0] = 1.38; QQLimits[11][1] = 1.62; QQLimits[11][2] = 1.88; QQLimits[11][3] = 2.19; QQLimits[11][4] = 2.65; QQLimits[11][5] = 3.18; QQLimits[11][6] = 3.76; QQLimits[11][7] = 4.47; QQLimits[11][8] = 5.28; QQLimits[11][9] = 6.165; QQLimits[11][10] = 7.05;
    
    QQLimits[12][0] = 1.38; QQLimits[12][1] = 1.62; QQLimits[12][2] = 1.88; QQLimits[12][3] = 2.19; QQLimits[12][4] = 2.65; QQLimits[12][5] = 3.18; QQLimits[12][6] = 3.76; QQLimits[12][7] = 4.47; QQLimits[12][8] = 5.28; QQLimits[12][9] = 6.165; QQLimits[12][10] = 7.05;
    
    QQLimits[13][0] = 1.38; QQLimits[13][1] = 1.62; QQLimits[13][2] = 1.88; QQLimits[13][3] = 2.19; QQLimits[13][4] = 2.65; QQLimits[13][5] = 3.18; QQLimits[13][6] = 3.76; QQLimits[13][7] = 4.47; QQLimits[13][8] = 5.28; QQLimits[13][9] = 6.165; QQLimits[13][10] = 7.05;
    
    QQLimits[14][0] = 1.38; QQLimits[14][1] = 1.62; QQLimits[14][2] = 1.88; QQLimits[14][3] = 2.19; QQLimits[14][4] = 2.65; QQLimits[14][5] = 3.18; QQLimits[14][6] = 3.76; QQLimits[14][7] = 4.47; QQLimits[14][8] = 5.28; QQLimits[14][9] = 6.165; QQLimits[14][10] = 7.05;
    
    QQLimits[15][0] = 1.38; QQLimits[15][1] = 1.62; QQLimits[15][2] = 1.88; QQLimits[15][3] = 2.19; QQLimits[15][4] = 2.65; QQLimits[15][5] = 3.18; QQLimits[15][6] = 3.76; QQLimits[15][7] = 4.47; QQLimits[15][8] = 5.28; QQLimits[15][9] = 6.165; QQLimits[15][10] = 7.05;
    
    QQLimits[16][0] = 1.38; QQLimits[16][1] = 1.62; QQLimits[16][2] = 1.88; QQLimits[16][3] = 2.19; QQLimits[16][4] = 2.65; QQLimits[16][5] = 3.18; QQLimits[16][6] = 3.76; QQLimits[16][7] = 4.47; QQLimits[16][8] = 5.28; QQLimits[16][9] = 6.165; QQLimits[16][10] = 7.05;
        
    QQLimits[17][0] = 1.38; QQLimits[17][1] = 1.62; QQLimits[17][2] = 1.88; QQLimits[17][3] = 2.19; QQLimits[17][4] = 2.65; QQLimits[17][5] = 3.18; QQLimits[17][6] = 3.76; QQLimits[17][7] = 4.47; QQLimits[17][8] = 5.28; QQLimits[17][9] = 6.165; QQLimits[17][10] = 7.05;
    
    QQLimits[18][0] = 1.38; QQLimits[18][1] = 1.62; QQLimits[18][2] = 1.88; QQLimits[18][3] = 2.19; QQLimits[18][4] = 2.65; QQLimits[18][5] = 3.18; QQLimits[18][6] = 3.76; QQLimits[18][7] = 4.47; QQLimits[18][8] = 5.28; QQLimits[18][9] = 6.165; QQLimits[18][10] = 7.05;
    
    zLimits_pip[0] = 0.059; zLimits_pip[1] = 0.081; zLimits_pip[2] = 0.1065; zLimits_pip[3] = 0.133; zLimits_pip[4] = 0.161; zLimits_pip[5] = 0.1905; zLimits_pip[6] = 0.221; zLimits_pip[7] = 0.2525; zLimits_pip[8] = 0.2855; zLimits_pip[9] = 0.32; zLimits_pip[10] = 0.356; zLimits_pip[11] = 0.394; zLimits_pip[12] = 0.4335; zLimits_pip[13] = 0.4745; zLimits_pip[14] = 0.5175; zLimits_pip[15] = 0.5625; zLimits_pip[16] = 0.6095; zLimits_pip[17] = 0.6585; zLimits_pip[18] = 0.7095; zLimits_pip[19] = 0.763; zLimits_pip[20] = 0.8185; zLimits_pip[21] = 0.8765; zLimits_pip[22] = 0.93;
    PT2Limits_pip[0] = 0.0; PT2Limits_pip[1] = 0.0153; PT2Limits_pip[2] = 0.0459; PT2Limits_pip[3] = 0.0972; PT2Limits_pip[4] = 0.1728; PT2Limits_pip[5] = 0.279; PT2Limits_pip[6] = 0.4239; PT2Limits_pip[7] = 0.6246; PT2Limits_pip[8] = 0.9081; PT2Limits_pip[9] = 1.314; PT2Limits_pip[10] = 1.75;
    zLimits_pim[0] = 0.89; zLimits_pim[1] = 0.9;
    PT2Limits_pim[0] = 0.89; PT2Limits_pim[1] = 0.9;
  }
  // %%%%% end x, QQ, z, PT2 bin definitions %%%%%
  
  ifstream filelist;
  filelist.open(inputFileList.c_str()); 

  TChain *h22chain = new TChain("h22");
  
  if (filelist.is_open()){
      std::string filename;
      
      int currentFileNumber = 0; 
      while( getline(filelist, filename) && currentFileNumber < numberOfFiles){
	h22chain->Add(filename.c_str()); 
	currentFileNumber++; 
      }
  } else {
    cerr << "Fatal: The list of files from " << inputFileList << " was not opened successfully!" << endl; 
    return; 
  }

  // Define variables that are common 
  // to the data and MC tree structure. 
  Int_t gpart;    
  
  // Short types stored in arrays. 
  // All of which (for data) are length gpart.
  // The MC banks are length mcnentr (be careful).
  Int_t    q[BUFFER_SIZE];
  UChar_t  cc_sect[BUFFER_SIZE];
  UChar_t  dc_sect[BUFFER_SIZE];
  UChar_t  ec_sect[BUFFER_SIZE];
  UChar_t  sc_sect[BUFFER_SIZE];
  UChar_t  sc_pd[BUFFER_SIZE];    
  UShort_t cc_segm[BUFFER_SIZE];
  UShort_t nphe[BUFFER_SIZE];
  
  // Float tree branches for both data and MC. 
  Float_t p[BUFFER_SIZE];
  Float_t cx[BUFFER_SIZE];
  Float_t cy[BUFFER_SIZE];
  Float_t cz[BUFFER_SIZE];
  Float_t vz[BUFFER_SIZE];
  Float_t vy[BUFFER_SIZE];
  Float_t vx[BUFFER_SIZE];
  Float_t b[BUFFER_SIZE];
  Float_t tl1_x[BUFFER_SIZE];
  Float_t tl1_y[BUFFER_SIZE];
  Float_t tl3_x[BUFFER_SIZE]; 
  Float_t tl3_y[BUFFER_SIZE];
  Float_t tl3_z[BUFFER_SIZE];
  Float_t tl3_cx[BUFFER_SIZE];
  Float_t tl3_cy[BUFFER_SIZE];
  Float_t tl3_cz[BUFFER_SIZE];
  Float_t etot[BUFFER_SIZE];
  Float_t ec_ei[BUFFER_SIZE];
  Float_t ec_eo[BUFFER_SIZE];
  Float_t ec_t[BUFFER_SIZE];
  Float_t ech_x[BUFFER_SIZE];
  Float_t ech_y[BUFFER_SIZE];
  Float_t ech_z[BUFFER_SIZE];
  Float_t sc_t[BUFFER_SIZE];
  Float_t sc_r[BUFFER_SIZE];
  
  // Variables used for MC files that contain 
  // the additional truth information. 
  Int_t   mcnentr;
  Int_t   mcid[BUFFER_SIZE];
  Float_t mctheta[BUFFER_SIZE];
  Float_t mcphi[BUFFER_SIZE];
  Float_t mcp[BUFFER_SIZE];
  
  // Conditionally activate the branches that 
  // are associated with monte carlo.  If you 
  // load them when processing data there is no 
  // harm, just a recurrent message that there
  // exists no branch called "mcid" in the file.
  // This might also cause a slight slowdown. 
  if(expOrSim == 0){
    h22chain->SetBranchAddress("mcnentr", &mcnentr);
    h22chain->SetBranchAddress("mcid", mcid);
    h22chain->SetBranchAddress("mctheta", mctheta);
    h22chain->SetBranchAddress("mcphi", mcphi);
    h22chain->SetBranchAddress("mcp", mcp);
  }
  
  h22chain->SetBranchAddress("gpart", &gpart);
  h22chain->SetBranchAddress("p", p);
  h22chain->SetBranchAddress("q", q);
  h22chain->SetBranchAddress("cx", cx);
  h22chain->SetBranchAddress("cy", cy);
  h22chain->SetBranchAddress("cz", cz);
  h22chain->SetBranchAddress("vz", vz);
  h22chain->SetBranchAddress("vy", vy);
  h22chain->SetBranchAddress("vx", vx);
  h22chain->SetBranchAddress("b", b);
  
  h22chain->SetBranchAddress("dc_sect", dc_sect);
  h22chain->SetBranchAddress("tl1_x", tl1_x);
  h22chain->SetBranchAddress("tl1_y", tl1_y);
  h22chain->SetBranchAddress("tl3_x", tl3_x);
  h22chain->SetBranchAddress("tl3_y", tl3_y);
  h22chain->SetBranchAddress("tl3_z", tl3_z);
  h22chain->SetBranchAddress("tl3_cx", tl3_cx);
  h22chain->SetBranchAddress("tl3_cy", tl3_cy);
  h22chain->SetBranchAddress("tl3_cz", tl3_cz);
  
  h22chain->SetBranchAddress("ec_sect", ec_sect);
  h22chain->SetBranchAddress("etot", etot);
  h22chain->SetBranchAddress("ec_ei", ec_ei);
  h22chain->SetBranchAddress("ec_eo", ec_eo);
  h22chain->SetBranchAddress("ec_t", ec_t);
  h22chain->SetBranchAddress("ech_x", ech_x);
  h22chain->SetBranchAddress("ech_y", ech_y);
  h22chain->SetBranchAddress("ech_z", ech_z);
  
  h22chain->SetBranchAddress("sc_sect", sc_sect);
  h22chain->SetBranchAddress("sc_t", sc_t);
  h22chain->SetBranchAddress("sc_r", sc_r);
  h22chain->SetBranchAddress("sc_pd", sc_pd);
  
  h22chain->SetBranchAddress("cc_sect", cc_sect);
  h22chain->SetBranchAddress("cc_segm", cc_segm);
  h22chain->SetBranchAddress("nphe", nphe);
  // %%%%% end set branch addresses %%%%%
  
  // %%%%% stuff for output root files %%%%%
  string outfilename;
  TFile *outputfile;
  /* 
  if(expOrSim == 1){ 
    outfilename = Form("data.n%i.BiSc%i.MoCo%i%i.__%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i__.root", 
		       numberOfFiles, binSchemeOpt, correctElectronP, 
		       correctPionP, e_zvertex_strict, e_ECsampling_strict, 
		       e_ECoutVin_strict, e_ECgeometric_strict, e_CCthetaMatching_strict, 
		       e_R1fid_strict, e_R3fid_strict, e_CCphiMatching_strict, 
		       e_CCfiducial_strict, yCut_strict, pip_vvp_strict, pip_R1fid_strict, 
		       pip_MXcut_strict, pim_vvp_strict, pim_R1fid_strict, pim_MXcut_strict);
  }
  else if(expOrSim == 0){ 
    outfilename = Form("MonteCarlo_v%i.it%i.n%i.BiSc%i.__%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i__.root", 
		       MC_VERSION, acceptanceIteration, numberOfFiles, binSchemeOpt, 
		       e_zvertex_strict, e_ECsampling_strict, e_ECoutVin_strict, 
		       e_ECgeometric_strict, e_CCthetaMatching_strict, e_R1fid_strict, 
		       e_R3fid_strict, e_CCphiMatching_strict, e_CCfiducial_strict, 
		       yCut_strict, pip_vvp_strict, pip_R1fid_strict, pip_MXcut_strict, 
		       pim_vvp_strict, pim_R1fid_strict, pim_MXcut_strict);
  }
  */

  // Trying this for ease of operation. 
  outfilename = "out.root"; 
  cout << "Opening output: " << outfilename << endl;
  outputfile = new TFile(outfilename.c_str(), "recreate");

  /* --------------------------------------------
    
    Declaration and initialization of histograms. 

   ---------------------------------------------- */
  int NthetaCCBins = 1; // keep these small to save space unless you really need to do another thorough study
  int Nrel_phiBins = 1;
  TH1F *h_nphe[NthetaCCBins][Nrel_phiBins];
  for(int k = 0; k < NthetaCCBins; k++) {
    for(int j = 0; j < Nrel_phiBins; j++) {
      h_nphe[k][j] = new TH1F(Form("h_nphe_thCC%i_relph%i", k, j), Form("h_nphe_thCC%i_relph%i", k, j), 300, 0, 300);
    }
  }
  
  TH2F *QQvsW = new TH2F("QQvsW", "QQvsW", 200, 0, 3.5, 200, 0, 5.5);
  TH2F *yvsx = new TH2F("yvsx", "yvsx", 200, 0.05, 0.65, 200, 0, 1);
  TH2F *yvsx_QQgt1_Wgt2p05 = new TH2F("yvsx_QQgt1_Wgt2p05", "yvsx_QQgt1_Wgt2p05", 200, 0.05, 0.65, 200, 0, 1);
  TH2F *ethvsx = new TH2F("ethvsx", "ethvsx", 200, 0.05, 0.65, 200, 0, 50);
  TH2F *ethvsx_QQgt1_Wgt2p05 = new TH2F("ethvsx_QQgt1_Wgt2p05", "ethvsx_QQgt1_Wgt2p05", 200, 0.05, 0.65, 200, 0, 50);
  
  TH1F *hpip_PT2[2]; // 2 for gen(0) or rec(1)
  if(expOrSim == 0) hpip_PT2[0] = new TH1F("hgen_pip_PT2", "hgen_pip_PT2", 200, 0, 1);
  hpip_PT2[1] = new TH1F("hrec_pip_PT2", "hrec_pip_PT2", 200, 0, 1);
  
  TH2F *pip_QQvsx[2]; // 2 for gen(0) or rec(1)
  if(expOrSim == 0) pip_QQvsx[0] = new TH2F("gen_pip_QQvsx", "gen_pip_QQvsx", 200, 0.08, 0.64, 200, 0.5, 5);
  pip_QQvsx[1] = new TH2F("rec_pip_QQvsx", "rec_pip_QQvsx", 200, 0.08, 0.64, 200, 0.5, 5);
  
  TH2F *pim_QQvsx[2]; // 2 for gen(0) or rec(1)
  if(expOrSim == 0) pim_QQvsx[0] = new TH2F("gen_pim_QQvsx", "gen_pim_QQvsx", 200, 0.08, 0.64, 200, 0.5, 5);
  pim_QQvsx[1] = new TH2F("rec_pim_QQvsx", "rec_pim_QQvsx", 200, 0.08, 0.64, 200, 0.5, 5);
  
  TH2F *pip_PT2vsz[2]; // 2 for gen(0) or rec(1)
  if(expOrSim == 0) pip_PT2vsz[0] = new TH2F("gen_pip_PT2vsz", "gen_pip_PT2vsz", 200, 0, 1.05, 200, 0, 1.05);
  pip_PT2vsz[1] = new TH2F("rec_pip_PT2vsz", "rec_pip_PT2vsz", 200, 0, 1.05, 200, 0, 1.05);
  
  TH2F *pim_PT2vsz[2]; // 2 for gen(0) or rec(1)
  if(expOrSim == 0) pim_PT2vsz[0] = new TH2F("gen_pim_PT2vsz", "gen_pim_PT2vsz", 200, 0, 1.05, 200, 0, 1.05);
  pim_PT2vsz[1] = new TH2F("rec_pim_PT2vsz", "rec_pim_PT2vsz", 200, 0, 1.05, 200, 0, 1.05);
  
  TH2F *pip_PT2vsz_xQQbins[NxBins][NQQBins][2]; // 2 for gen(0) or rec(1)
  TH2F *pim_PT2vsz_xQQbins[NxBins][NQQBins][2]; // 2 for gen(0) or rec(1)
  
  int NphihBBins = 1; // the extra "B" is because this binning scheme is for a different purpose than the other one, ... keep this small unless you really need it
  int phihBMin = 0;
  int phihBMax = 180;
  TH2F *pip_PT2vsz_xQQphihBbins[NxBins][NQQBins][NphihBBins][2]; // 2 for gen(0) or rec(1)
  TH2F *pim_PT2vsz_xQQphihBbins[NxBins][NQQBins][NphihBBins][2]; // 2 for gen(0) or rec(1)
  
  TH1F *pip_MX_xQQbins[NxBins][NQQBins][2]; // 2 for gen(0) or rec(1)
  TH1F *pim_MX_xQQbins[NxBins][NQQBins][2]; // 2 for gen(0) or rec(1)
  for(int x = 0; x < NxBins; x++){
    for(int QQ = 0; QQ < NQQBins; QQ++){
      
      if(expOrSim == 0){
	pip_PT2vsz_xQQbins[x][QQ][0] = new TH2F(Form("gen_pip_PT2vsz_x%iQQ%i", x, QQ), 
						Form("gen_pip_PT2vsz_x%iQQ%i", x, QQ), 
						200, 0, 1.05, 200, 0, 1.1);
	pim_PT2vsz_xQQbins[x][QQ][0] = new TH2F(Form("gen_pim_PT2vsz_x%iQQ%i", x, QQ), 
						Form("gen_pim_PT2vsz_x%iQQ%i", x, QQ), 
						200, 0, 1.05, 200, 0, 1.1);
	pip_MX_xQQbins[x][QQ][0] = new TH1F(Form("gen_pip_MX_x%iQQ%i", x, QQ), 
					    Form("gen_pip_MX_x%iQQ%i", x, QQ), 
					    200, 0, 4);
	pim_MX_xQQbins[x][QQ][0] = new TH1F(Form("gen_pim_MX_x%iQQ%i", x, QQ), 
					    Form("gen_pim_MX_x%iQQ%i", x, QQ), 
					    200, 0, 4);
      }
      
      pip_PT2vsz_xQQbins[x][QQ][1] = new TH2F(Form("rec_pip_PT2vsz_x%iQQ%i", x, QQ),
					      Form("rec_pip_PT2vsz_x%iQQ%i", x, QQ),
					      200, 0, 1.05, 200, 0, 1.1);
      pim_PT2vsz_xQQbins[x][QQ][1] = new TH2F(Form("rec_pim_PT2vsz_x%iQQ%i", x, QQ), 
					      Form("rec_pim_PT2vsz_x%iQQ%i", x, QQ), 
					      200, 0, 1.05, 200, 0, 1.1);
      
      pip_MX_xQQbins[x][QQ][1] = new TH1F(Form("rec_pip_MX_x%iQQ%i", x, QQ), 
					  Form("rec_pip_MX_x%iQQ%i", x, QQ), 
					  200, 0, 4);	  
      pim_MX_xQQbins[x][QQ][1] = new TH1F(Form("rec_pim_MX_x%iQQ%i", x, QQ), 
					  Form("rec_pim_MX_x%iQQ%i", x, QQ), 
					  200, 0, 4);
      
      // Not sure what this phihB is.
      for(int phihB = 0; phihB < NphihBBins; phihB++){
	if(expOrSim == 0){ 
	  pip_PT2vsz_xQQphihBbins[x][QQ][phihB][0] = new TH2F(Form("gen_pip_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							      Form("gen_pip_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							      200, 0, 1.05, 200, 0, 1.1);
	  pim_PT2vsz_xQQphihBbins[x][QQ][phihB][0] = new TH2F(Form("gen_pim_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							      Form("gen_pim_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							      200, 0, 1.05, 200, 0, 1.1);
	}
	
	pip_PT2vsz_xQQphihBbins[x][QQ][phihB][1] = new TH2F(Form("rec_pip_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							    Form("rec_pip_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							    200, 0, 1.05, 200, 0, 1.1);
	pim_PT2vsz_xQQphihBbins[x][QQ][phihB][1] = new TH2F(Form("rec_pim_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							    Form("rec_pim_PT2vsz_x%iQQ%iphihB%i", x, QQ, phihB), 
							    200, 0, 1.05, 200, 0, 1.1);
      }
    }
  }
  
  TH1F *h_pip_phih[NxBins][NQQBins][NzBins_pip][NPT2Bins_pip][2]; // 2 for gen(0) or rec(1)
  TH1F *pip_phih_pure[NxBins][NQQBins][NzBins_pip][NPT2Bins_pip];
  TH1F *pip_phih_impure[NxBins][NQQBins][NzBins_pip][NPT2Bins_pip];
  
  TH1F *h_pim_phih[NxBins][NQQBins][NzBins_pim][NPT2Bins_pim][2];
  TH1F *pim_phih_pure[NxBins][NQQBins][NzBins_pim][NPT2Bins_pim];
  TH1F *pim_phih_impure[NxBins][NQQBins][NzBins_pim][NPT2Bins_pim];
  
  int NmomBins = 5; // to bin the resolutions in momentum
  float momMin = 0;
  float momMax = 4;
  TH1F *pip_mom_res[NmomBins];
  TH1F *pip_theta_res[NmomBins];
  TH1F *pip_phi_res[NmomBins];
  for(int N = 0; N < NmomBins; N++){
    if(expOrSim == 0){
      pip_mom_res[N] = new TH1F(Form("pip_mom_res%i",N), Form("pip_mom_res%i",N), 100, -0.3, 0.3);
      pip_theta_res[N] = new TH1F(Form("pip_theta_res%i",N), Form("pip_theta_res%i",N), 100, -1, 1);
      pip_phi_res[N] = new TH1F(Form("pip_phi_res%i",N), Form("pip_phi_res%i",N), 100, -7, 7);
    }
  }
  
  TH1F *pip_PT2_res[NxBins][NQQBins][NzBins_pip][NPT2Bins_pip]; // MC only. res = gen - rec
  
  for(int x = 0; x < NxBins; x++){
    for(int QQ = 0; QQ < NQQBins; QQ++){
      for(int z = 0; z < NzBins_pip; z++){
	for(int PT2 = 0; PT2 < NPT2Bins_pip; PT2++){
	  if(expOrSim == 0){
	    h_pip_phih[x][QQ][z][PT2][0] = new TH1F(Form("gen_pip_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("gen_pip_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	    h_pip_phih[x][QQ][z][PT2][0]->Sumw2();
	    pip_phih_pure[x][QQ][z][PT2] = new TH1F(Form("pip_phih_pure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("pip_phih_pure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	    pip_phih_impure[x][QQ][z][PT2] = new TH1F(Form("pip_phih_impure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("pip_phih_impure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	    pip_PT2_res[x][QQ][z][PT2] = new TH1F(Form("pip_PT2_res_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("PT2 resolution (gen - rec) x%i QQ%i z%i PT2%i",x,QQ,z,PT2), 100, -0.2, 0.2);
	  }
	  
	  h_pip_phih[x][QQ][z][PT2][1] = new TH1F(Form("rec_pip_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("rec_pip_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	  h_pip_phih[x][QQ][z][PT2][1]->Sumw2();
	}
      }
    }
  }
  
  for(int x = 0; x < NxBins; x++){
    for(int QQ = 0; QQ < NQQBins; QQ++){
      for(int z = 0; z < NzBins_pim; z++){
	for(int PT2 = 0; PT2 < NPT2Bins_pim; PT2++){
	  if(expOrSim == 0){
	    h_pim_phih[x][QQ][z][PT2][0] = new TH1F(Form("gen_pim_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("gen_pim_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	    h_pim_phih[x][QQ][z][PT2][0]->Sumw2();
	    pim_phih_pure[x][QQ][z][PT2] = new TH1F(Form("pim_phih_pure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("pim_phih_pure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	    pim_phih_impure[x][QQ][z][PT2] = new TH1F(Form("pim_phih_impure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("pim_phih_impure_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	  }
	  
	  h_pim_phih[x][QQ][z][PT2][1] = new TH1F(Form("rec_pim_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), Form("rec_pim_phih_x%i_QQ%i_z%i_PT2%i",x,QQ,z,PT2), NphihBins, phihMin, phihMax);
	  h_pim_phih[x][QQ][z][PT2][1]->Sumw2();
	}
      }
    }
  }

  for(int i = 0; i < h22chain->GetEntries(); i++){
    h22chain->GetEntry(i);
    
    bool genExclusiveEvent = 0; // if the generated event is an exclusive event, i.e. ep --> e pi+ n, we don't want to analyze it at all (the generator give unrealistic kinematics)
    if(expOrSim == 0 && mcnentr == 3){
      if((mcid[1] == 211 && mcid[2] == 2112) || (mcid[1] == 2112 && mcid[2] == 211)){
	genExclusiveEvent = 1; // the first (zeroth) generated particle is always the scattered electron
      }
    }
    
    if(genExclusiveEvent == 0){
      
      Int_t currentRunNumber = 0; 
      if(expOrSim == 1){
	TString currentFilename = h22chain->GetCurrentFile()->GetName(); 
	currentRunNumber = getRunNumberFromFilename(currentFilename); 
      }
      
      // Some things. 
      TLorentzVector V4k(0.0, 0.0, Beam_Energy, Beam_Energy); // x, y, z, t
      TLorentzVector V4ISproton(0.0, 0.0, 0.0, prot_mass); // IS = Initial State
      int e_index[2]    = {INVALID_INDEX, INVALID_INDEX};
      int pip_index[2]  = {INVALID_INDEX, INVALID_INDEX};
      int pim_index[2]  = {INVALID_INDEX, INVALID_INDEX};
      int prot_index[2] = {INVALID_INDEX, INVALID_INDEX};
      
      // %%%%% get gen. particle indices  %%%%%
      if(expOrSim == 0){
	e_index[0] = 0; // the first (zeroth) generated particle is always the scattered electron
	vector<int> genIndices = getGenIndices(mcnentr, mcid, mcp);
	pip_index[0] = genIndices[0];
	pim_index[0] = genIndices[1];
	prot_index[0] = genIndices[2];
      }
      // %%%%% end get gen. particle indices  %%%%%
      
      // Electron identification, returns -123 if 
      // no electron is found. 
      e_index[1] = eID(gpart, q, p, cc_sect, sc_sect, ec_sect, dc_sect, cx, cy, cz, 
		       tl1_x, tl1_y, tl3_x, tl3_y, tl3_z, tl3_cx, tl3_cy, tl3_cz, 
		       e_zvertex_strict, vz, vy, vx, e_ECsampling_strict, expOrSim, 
		       etot, e_ECoutVin_strict, ec_ei, ech_x, ech_y, ech_z, 
		       e_CCthetaMatching_strict, cc_segm, e_ECgeometric_strict, 
		       e_R1fid_strict, e_R3fid_strict, e_CCphiMatching_strict, sc_pd, 
		       e_CCfiducial_strict);
      
      // %%%%% set e- 3 and 4 vectors %%%%%
      // can't set the vectors in a loop because gen uses p, theta, phi but rec uses p, cx, cy, cz
      TVector3 V3_e[2]; // 2 for gen(0) and rec(1)
      TLorentzVector V4_e[2];
      
      if(e_index[0] > INVALID_INDEX){
	V3_e[0].SetXYZ(mcp[e_index[0]]*cos(pi180*mcphi[e_index[0]])*sin(pi180*mctheta[e_index[0]]), mcp[e_index[0]]*sin(pi180*mcphi[e_index[0]])*sin(pi180*mctheta[e_index[0]]), mcp[e_index[0]]*cos(pi180*mctheta[e_index[0]]));
	V4_e[0].SetXYZT(V3_e[0].X(), V3_e[0].Y(), V3_e[0].Z(), sqrt(V3_e[0].Mag2() + pow(e_mass,2)));
      }
      
      if(e_index[1] > INVALID_INDEX){
	V3_e[1].SetXYZ(p[e_index[1]]*cx[e_index[1]], p[e_index[1]]*cy[e_index[1]], p[e_index[1]]*cz[e_index[1]]);
	V4_e[1].SetXYZT(V3_e[1].X(), V3_e[1].Y(), V3_e[1].Z(), sqrt(V3_e[1].Mag2() + pow(e_mass,2)));

	if(correctElectronP && expOrSim == 1){ 
	  V4_e[1] = MomCorr->PcorN(V4_e[1], -1, 11); 
	}
      }
      
      TVector3 V3_H[2];
      TLorentzVector V4_q[2], V4_H[2];
      
      for(int j = 1; j >= expOrSim; j--){
	V4_q[j] = V4k - V4_e[j];
	V4_H[j] = V4_q[j] + V4ISproton;
	V3_H[j].SetXYZ(V4_H[j].X(), V4_H[j].Y(), V4_H[j].Z());
      }
      // %%%%% end set e- 3 and 4 vectors %%%%%
      
      // %%%%% hadron ID %%%%%
      if(e_index[1] >= 0){
	//	vector<int> hadronIndices = hadronID(gpart, e_index, q, p, sc_sect, 
	//					     dc_sect, sc_t, sc_r, sc_pd, pip_vvp_strict, 
	//					     pip_R1fid_strict, pip_MXcut_strict, expOrSim, 
	//					     ec_ei, ec_sect, cc_sect, nphe, ec_eo, cx, cy, 
	//					     cz, b, tl1_x, tl1_y, mcp, mcphi, mctheta, V4_H, 
	//					     currentRunNumber, pim_vvp_strict, pim_R1fid_strict, 
	//					     pim_MXcut_strict);

	vector<int> hadronIndices = hadronIDFast(gpart, e_index, q, p, sc_sect, 
						 dc_sect, sc_t, sc_r, sc_pd, pip_vvp_strict, 
						 pip_R1fid_strict, pip_MXcut_strict, expOrSim, 
						 ec_ei, ec_sect, cc_sect, nphe, ec_eo, cx, cy, 
						 cz, b, tl1_x, tl1_y, mcp, mcphi, mctheta, V4_H, 
						 currentRunNumber, pim_vvp_strict, pim_R1fid_strict, 
						 pim_MXcut_strict, pipTable, pimTable);
	
	// Should be changed to return a std::tuple<int, int, int>. 
	pip_index[1]  = hadronIndices[0];
	pim_index[1]  = hadronIndices[1];
	prot_index[1] = hadronIndices[2];
      }
      // %%%%% end hadron ID %%%%%
      
      // %%%%%%%%% set remaining 4-vectors (and 3-vectors)  %%%%%%%%%%%
      // can't set the vectors in a loop because gen uses p, theta, phi but rec uses p, cx, cy, cz
      TVector3 V3_pip[2], V3_pim[2], V3_prot[2]; // 2 for gen(0) and rec(1)
      TLorentzVector V4_pip[2], V4_pim[2], V4_prot[2];
      
      if(pip_index[0] > INVALID_INDEX){
	V3_pip[0].SetXYZ(mcp[pip_index[0]]*cos(pi180*mcphi[pip_index[0]])*sin(pi180*mctheta[pip_index[0]]), mcp[pip_index[0]]*sin(pi180*mcphi[pip_index[0]])*sin(pi180*mctheta[pip_index[0]]), mcp[pip_index[0]]*cos(pi180*mctheta[pip_index[0]]));
	V4_pip[0].SetXYZT(V3_pip[0].X(), V3_pip[0].Y(), V3_pip[0].Z(), sqrt(V3_pip[0].Mag2() + pow(pip_mass,2)));
      }

      if(pim_index[0] > INVALID_INDEX){
	V3_pim[0].SetXYZ(mcp[pim_index[0]]*cos(pi180*mcphi[pim_index[0]])*sin(pi180*mctheta[pim_index[0]]), mcp[pim_index[0]]*sin(pi180*mcphi[pim_index[0]])*sin(pi180*mctheta[pim_index[0]]), mcp[pim_index[0]]*cos(pi180*mctheta[pim_index[0]]));
	V4_pim[0].SetXYZT(V3_pim[0].X(), V3_pim[0].Y(), V3_pim[0].Z(), sqrt(V3_pim[0].Mag2() + pow(pim_mass,2)));
      }

      if(prot_index[0] > INVALID_INDEX){
	V3_prot[0].SetXYZ(mcp[prot_index[0]]*cos(pi180*mcphi[prot_index[0]])*sin(pi180*mctheta[prot_index[0]]), mcp[prot_index[0]]*sin(pi180*mcphi[prot_index[0]])*sin(pi180*mctheta[prot_index[0]]), mcp[prot_index[0]]*cos(pi180*mctheta[prot_index[0]]));
	V4_prot[0].SetXYZT(V3_prot[0].X(), V3_prot[0].Y(), V3_prot[0].Z(), sqrt(V3_prot[0].Mag2() + pow(prot_mass,2)));
      }
      
      if(pip_index[1] > INVALID_INDEX){
	V3_pip[1].SetXYZ(p[pip_index[1]]*cx[pip_index[1]], p[pip_index[1]]*cy[pip_index[1]], p[pip_index[1]]*cz[pip_index[1]]);
	V4_pip[1].SetXYZT(V3_pip[1].X(), V3_pip[1].Y(), V3_pip[1].Z(), sqrt(V3_pip[1].Mag2() + pow(pip_mass,2)));
	if(correctPionP && expOrSim == 1){ 
	  V4_pip[1] = MomCorr->PcorN(V4_pip[1], 1, 211); 
	}
      }

      if(pim_index[1] > INVALID_INDEX){
	V3_pim[1].SetXYZ(p[pim_index[1]]*cx[pim_index[1]], p[pim_index[1]]*cy[pim_index[1]], p[pim_index[1]]*cz[pim_index[1]]);
	V4_pim[1].SetXYZT(V3_pim[1].X(), V3_pim[1].Y(), V3_pim[1].Z(), sqrt(V3_pim[1].Mag2() + pow(pim_mass,2)));
	if(correctPionP && expOrSim == 1){ 
	  V4_pim[1] = MomCorr->PcorN(V4_pim[1], -1, -211); 
	}
      }

      if(prot_index[1] > INVALID_INDEX){
	V3_prot[1].SetXYZ(p[prot_index[1]]*cx[prot_index[1]], p[prot_index[1]]*cy[prot_index[1]], p[prot_index[1]]*cz[prot_index[1]]);
	V4_prot[1].SetXYZT(V3_prot[1].X(), V3_prot[1].Y(), V3_prot[1].Z(), sqrt(V3_prot[1].Mag2() + pow(prot_mass,2)));
      }
      
      TLorentzVector V4_pip_prime[2], V4_pim_prime[2], V4_prot_prime[2];
      TLorentzVector V4_X_epipX[2];
      TLorentzVector V4_X_epimX[2];
      
      for(int j = 1; j >= expOrSim; j--){
	V4_q[j] = V4k - V4_e[j];
	V4_H[j] = V4_q[j] + V4ISproton;
	V3_H[j].SetXYZ(V4_H[j].X(), V4_H[j].Y(), V4_H[j].Z());
	
	V4_pip_prime[j] = V4_pip[j];
	V4_pip_prime[j].RotateZ(-1.0*V4_q[j].Phi() - pi);
	V4_pip_prime[j].RotateY(V4_q[j].Theta());
	V4_pip_prime[j].Boost(0.0, 0.0, V3_H[j].Mag()/V4_H[j].T());
        
	V4_pim_prime[j] = V4_pim[j];
	V4_pim_prime[j].RotateZ(-1.0*V4_q[j].Phi() - pi);
	V4_pim_prime[j].RotateY(V4_q[j].Theta());
	V4_pim_prime[j].Boost(0.0, 0.0, V3_H[j].Mag()/V4_H[j].T());
        
	V4_prot_prime[j] = V4_prot[j];
	V4_prot_prime[j].RotateZ(-1.0*V4_q[j].Phi() - pi);
	V4_prot_prime[j].RotateY(V4_q[j].Theta());
	V4_prot_prime[j].Boost(0.0, 0.0, V3_H[j].Mag()/V4_H[j].T());
        
	V4_X_epipX[j] = V4_H[j] - V4_pip[j];
	V4_X_epimX[j] = V4_H[j] - V4_pim[j];
      }

      // %%%%%%%%% analysis of kinematics %%%%%%%%%%%
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      float W[2], y[2], QQ[2], x[2], pip_z[2], pip_PT2[2], pip_phih[2], MX_epipX[2];
      int xBin[2] = {-1234, -1234};
      int QQBin[2] = {-1234, -1234};
      int pip_zBin[2] = {-1234, -1234};
      int pip_PT2Bin[2] = {-1234, -1234};
      int pip_phihBin[2] = {-1234, -1234};
      int pip_phihBBin[2] = {-1234, -1234};
      
      float pim_z[2], pim_PT2[2], pim_phih[2], MX_epimX[2];
      int pim_zBin[2] = {-1234, -1234};
      int pim_PT2Bin[2] = {-1234, -1234};
      int pim_phihBin[2] = {-1234, -1234};
      int pim_phihBBin[2] = {-1234, -1234};
      
      // set some values:
      for(int j = 1; j >= expOrSim; j--){
	W[j]  = V4_H[j].Mag();
	y[j]  = V4_q[j].T()/Beam_Energy;
	QQ[j] = -V4_q[j]*V4_q[j];
	x[j]  = QQ[j]/(2.0*V4_q[j].T()*prot_mass);
	
	pip_z[j]    = V4_pip[j].T()/V4_q[j].T();
	pip_PT2[j]  = V4_pip_prime[j].Perp()*V4_pip_prime[j].Perp();
	pip_phih[j] = pi180_inv*V4_pip_prime[j].Phi();
	MX_epipX[j] = V4_X_epipX[j].Mag();
        
	pim_z[j]    = V4_pim[j].T()/V4_q[j].T();
	pim_PT2[j]  = V4_pim_prime[j].Perp()*V4_pim_prime[j].Perp();
	pim_phih[j] = pi180_inv*V4_pim_prime[j].Phi();
	MX_epimX[j] = V4_X_epimX[j].Mag();
        
	if(pip_index[j] >= 0 || pim_index[j] >= 0){
	    
	  if(x[j] >= xLimits[0] && x[j] < xLimits[NxBins]){

	    xBin[j] = 0;
	    while(x[j] >= xLimits[xBin[j]+1]){ 
	      xBin[j]++; 
	    }
	    
	    if(QQ[j] >= QQLimits[xBin[j]][0] && QQ[j] < QQLimits[xBin[j]][NQQBins]){

	      QQBin[j] = 0;
	      while(QQ[j] >= QQLimits[xBin[j]][QQBin[j]+1]){ 
		QQBin[j]++; 
	      }
                    
		    if(pip_index[j] >= 0 && pip_z[j] >= zLimits_pip[0] && pip_z[j] < zLimits_pip[NzBins_pip] && pip_PT2[j] >= PT2Limits_pip[0] && pip_PT2[j] < PT2Limits_pip[NPT2Bins_pip]){

		      pip_zBin[j] = 0;
		      while(pip_z[j] >= zLimits_pip[pip_zBin[j]+1]){ 
			pip_zBin[j]++; 
		      }

			pip_PT2Bin[j] = 0;
			while(pip_PT2[j] >= PT2Limits_pip[pip_PT2Bin[j]+1]){
			  pip_PT2Bin[j]++; 
			}
                        
			pip_phihBin[j]  = static_cast<int>((pip_phih[j] - phihMin)/((phihMax - phihMin)/NphihBins));
			pip_phihBBin[j] = static_cast<int>((fabs(pip_phih[j]) - phihBMin)/((phihBMax - phihBMin)/NphihBBins));
		    }
		    
		    if(pim_index[j] >= 0 && pim_z[j] >= zLimits_pim[0] && pim_z[j] < zLimits_pim[NzBins_pim] && pim_PT2[j] >= PT2Limits_pim[0] && pim_PT2[j] < PT2Limits_pim[NPT2Bins_pim]){

			pim_zBin[j] = 0;
			while(pim_z[j] >= zLimits_pim[pim_zBin[j]+1]){ 
			  pim_zBin[j]++; 
			}

			pim_PT2Bin[j] = 0;
			while(pim_PT2[j] >= PT2Limits_pim[pim_PT2Bin[j]+1]){ 
			  pim_PT2Bin[j]++; 
			}
                        
			pim_phihBin[j] = static_cast<int>((pim_phih[j] - phihMin)/((phihMax - phihMin)/NphihBins));
			pim_phihBBin[j] = static_cast<int>((fabs(pim_phih[j]) - phihBMin)/((phihBMax - phihBMin)/NphihBBins));
		    }
	    }
	  }	    
	}
      }
	
	// see if the event is a keeper for my final sample
	bool pip_keeper[2] = {0, 0};
	bool pim_keeper[2] = {0, 0};
	for(int j = 1; j >= expOrSim; j--)
	  {
	    if(xBin[j] > -1 && QQBin[j] > -1 && pip_zBin[j] > -1 && pip_PT2Bin[j] > -1 && W[j] > WMin && y[j] < yMax) pip_keeper[j] = 1;
	    if(xBin[j] > -1 && QQBin[j] > -1 && pim_zBin[j] > -1 && pim_PT2Bin[j] > -1 && W[j] > WMin && y[j] < yMax) pim_keeper[j] = 1;
	  }
	
	/////// get weight factors: ///////
	///////////////////////////////////
	float pip_weight = 1.0;
	float pim_weight = 1.0;
        
	// It is the acceptance correction to the phi distributions, 
	// and currently the value is set to 0 so this part never 
	// executes. 
	// iteration 1 = default model in haprad:
	// pip:
	if(expOrSim == 0 && acceptanceIteration == 1 && xBin[0] > -1 && QQBin[0] > -1 && W[0] > WMin && y[0] < yMax && pip_index[0] >= 0)
	  {
	    ifstream term0file(Form("requiredFiles/haprad/term0/dflt_term0_fromHAPRAD_BiSc%i_x%iQQ%iz%iPT2%i.txt", binSchemeOpt, xBin[0], QQBin[0], pip_zBin[0], pip_PT2Bin[0]));
	    if(term0file)
	      {
		float term0;
		term0file>>term0;
                
		// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
		float ac = 1.2025E-10;
		float bc = -5.2703E-02;
		float cc = 3.7467E-01;
		float dc = 6.5397E-02;
		float ec = -2.2136E-01;
		float fc = -1.0621E-01;
                
		float sgmpt = ac + bc*x[0] + cc*pip_z[0] + dc*x[0]*x[0] + ec*pip_z[0]*pip_z[0] + fc*x[0]*pip_z[0];
		if(sgmpt < 0.02) sgmpt = 0.02;
		if(sgmpt > 0.15) sgmpt = 0.15;
		float GTMD = exp((-(pip_PT2[0] + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
		float pi_thresh = sqrt(1.0 - ((prot_mass + pip_mass)*(prot_mass + pip_mass))/pow(MX_epipX[0], 2.0));
		float rt = 1.0 - y[0] - (prot_mass*x[0]*y[0])/(2.0*Beam_Energy);
		float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x[0])/(y[0]*Beam_Energy))));
		float h3 = -0.36544E-03*pow(x[0], -2.1855)*pow(1.0 - x[0], 3.4176)*pow(pip_z[0], -1.7567)*pow(1.0 - pip_z[0], 1.1272)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 8.9985);
		float h4 = 0.10908E-02*pow(x[0], -0.35265E-06)*pow(1.0 - x[0], 0.30276E-07)*pow(pip_z[0], -0.66787)*pow(1.0 - pip_z[0], 3.5868)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 6.8777/x[0]);
		float Ac = (h3*GTMD*pi_thresh*sqrt(pip_PT2[0]/QQ[0])*(2.0 - y[0])*rtz)/term0;
		float Acc = (h4*GTMD*pi_thresh*(pip_PT2[0]/QQ[0])*rtz*rtz)/term0;
                
		if(Ac > 0.45) Ac = 0.45; // don't want huge moments
		if(Ac < -0.45) Ac = -0.45;
		if(Acc > 0.45) Acc = 0.45;
		if(Acc < -0.45) Acc = -0.45;
                
		pip_weight = 1.0 + Ac*cos(pip_phih[0]*pi180) + Acc*cos(2.0*pip_phih[0]*pi180);
		if(pip_weight != pip_weight) pip_weight = 1.0; // to remove the occasional nan
	      }
	    term0file.close();
	  }
	
	// iteration 2 = from Nick
	// pip:
	if(expOrSim == 0 && acceptanceIteration == 2 && xBin[0] > -1 && QQBin[0] > -1 && W[0] > WMin && y[0] < yMax && pip_index[0] >= 0)
	  {
	    ifstream term0file(Form("requiredFiles/haprad/term0/dflt_term0_fromHAPRAD_BiSc%i_x%iQQ%iz%iPT2%i.txt", binSchemeOpt, xBin[0], QQBin[0], pip_zBin[0], pip_PT2Bin[0]));
	    if(term0file)
	      {
		float term0;
		term0file>>term0;
                
		// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
		float ac = 1.2025E-10;
		float bc = -5.2703E-02;
		float cc = 3.7467E-01;
		float dc = 6.5397E-02;
		float ec = -2.2136E-01;
		float fc = -1.0621E-01;
                
		float sgmpt = ac + bc*x[0] + cc*pip_z[0] + dc*x[0]*x[0] + ec*pip_z[0]*pip_z[0] + fc*x[0]*pip_z[0];
		if(sgmpt < 0.02) sgmpt = 0.02;
		if(sgmpt > 0.15) sgmpt = 0.15;
		float GTMD = exp((-(pip_PT2[0] + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
		float pi_thresh = sqrt(1.0 - ((prot_mass + pip_mass)*(prot_mass + pip_mass))/pow(MX_epipX[0], 2.0));
		float rt = 1.0 - y[0] - (prot_mass*x[0]*y[0])/(2.0*Beam_Energy);
		float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x[0])/(y[0]*Beam_Energy))));
		float h3 = -0.36544E-03*pow(x[0], -2.1855)*pow(1.0 - x[0], 3.4176)*pow(pip_z[0], -1.7567)*pow(1.0 - pip_z[0], 1.1272)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 8.9985);
		h3 = h3*12.0*(-0.03 + x[0]); // from Nick: H3_Updated = h3(default)* 12*(-0.03 + x)
		float h4 = 0.10908E-02*pow(x[0], -0.35265E-06)*pow(1.0 - x[0], 0.30276E-07)*pow(pip_z[0], -0.66787)*pow(1.0 - pip_z[0], 3.5868)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 5.1777/x[0]);
		h4 = h4*6.0*(2.0 - x[0]); // from Nick: H4_Updated = 6*(2-x)*h4[default, bb = 5.1777] ... note that bb is changed above
		float Ac = (h3*GTMD*pi_thresh*sqrt(pip_PT2[0]/QQ[0])*(2.0 - y[0])*rtz)/term0;
		float Acc = (h4*GTMD*pi_thresh*(pip_PT2[0]/QQ[0])*rtz*rtz)/term0;
                
		if(Ac > 0.45) Ac = 0.45; // don't want huge moments
		if(Ac < -0.45) Ac = -0.45;
		if(Acc > 0.45) Acc = 0.45;
		if(Acc < -0.45) Acc = -0.45;
                
		pip_weight = 1.0 + Ac*cos(pip_phih[0]*pi180) + Acc*cos(2.0*pip_phih[0]*pi180);
		if(pip_weight != pip_weight) pip_weight = 1.0; // to remove the occasional nan
	      }
	    term0file.close();
	  }
	
	// iteration 1 = default model in haprad:
	// pim:
	if(expOrSim == 0 && acceptanceIteration == 1 && xBin[0] > -1 && QQBin[0] > -1 && W[0] > WMin && y[0] < yMax && pim_index[0] >= 0)
	  {
	    ifstream term0file(Form("requiredFiles/haprad/term0/dflt_term0_fromHAPRAD_BiSc%i_x%iQQ%iz%iPT2%i.txt", binSchemeOpt, xBin[0], QQBin[0], pim_zBin[0], pim_PT2Bin[0]));
	    if(term0file)
	      {
		float term0;
		term0file>>term0;
                
		// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
		float ac = 1.2025E-10;
		float bc = -5.2703E-02;
		float cc = 3.7467E-01;
		float dc = 6.5397E-02;
		float ec = -2.2136E-01;
		float fc = -1.0621E-01;
                
		float sgmpt = ac + bc*x[0] + cc*pim_z[0] + dc*x[0]*x[0] + ec*pim_z[0]*pim_z[0] + fc*x[0]*pim_z[0];
		if(sgmpt < 0.02) sgmpt = 0.02;
		if(sgmpt > 0.15) sgmpt = 0.15;
		float GTMD = exp((-(pim_PT2[0] + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
		float pi_thresh = sqrt(1.0 - ((prot_mass + pim_mass)*(prot_mass + pim_mass))/pow(MX_epimX[0], 2.0));
		float rt = 1.0 - y[0] - (prot_mass*x[0]*y[0])/(2.0*Beam_Energy);
		float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x[0])/(y[0]*Beam_Energy))));
		float h3 = -0.36544E-03*pow(x[0], -2.1855)*pow(1.0 - x[0], 3.4176)*pow(pim_z[0], -1.7567)*pow(1.0 - pim_z[0], 1.1272)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 8.9985);
		float h4 = 0.10908E-02*pow(x[0], -0.35265E-06)*pow(1.0 - x[0], 0.30276E-07)*pow(pim_z[0], -0.66787)*pow(1.0 - pim_z[0], 3.5868)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 6.8777/x[0]);
		float Ac = (h3*GTMD*pi_thresh*sqrt(pim_PT2[0]/QQ[0])*(2.0 - y[0])*rtz)/term0;
		float Acc = (h4*GTMD*pi_thresh*(pim_PT2[0]/QQ[0])*rtz*rtz)/term0;
                
		if(Ac > 0.45) Ac = 0.45; // don't want huge moments
		if(Ac < -0.45) Ac = -0.45;
		if(Acc > 0.45) Acc = 0.45;
		if(Acc < -0.45) Acc = -0.45;
                
		pim_weight = 1.0 + Ac*cos(pim_phih[0]*pi180) + Acc*cos(2.0*pim_phih[0]*pi180);
		if(pim_weight != pim_weight) pim_weight = 1.0; // to remove the occasional nan
	      }
	    term0file.close();
	  }
	
	// iteration 2 = from Nick
	// pim:
	if(expOrSim == 0 && acceptanceIteration == 2 && xBin[0] > -1 && QQBin[0] > -1 && W[0] > WMin && y[0] < yMax && pim_index[0] >= 0)
	  {
	    ifstream term0file(Form("requiredFiles/haprad/term0/dflt_term0_fromHAPRAD_BiSc%i_x%iQQ%iz%iPT2%i.txt", binSchemeOpt, xBin[0], QQBin[0], pim_zBin[0], pim_PT2Bin[0]));
	    if(term0file)
	      {
		float term0;
		term0file>>term0;
                
		// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
		float ac = 1.2025E-10;
		float bc = -5.2703E-02;
		float cc = 3.7467E-01;
		float dc = 6.5397E-02;
		float ec = -2.2136E-01;
		float fc = -1.0621E-01;
                
		float sgmpt = ac + bc*x[0] + cc*pim_z[0] + dc*x[0]*x[0] + ec*pim_z[0]*pim_z[0] + fc*x[0]*pim_z[0];
		if(sgmpt < 0.02) sgmpt = 0.02;
		if(sgmpt > 0.15) sgmpt = 0.15;
		float GTMD = exp((-(pim_PT2[0] + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
		float pi_thresh = sqrt(1.0 - ((prot_mass + pim_mass)*(prot_mass + pim_mass))/pow(MX_epimX[0], 2.0));
		float rt = 1.0 - y[0] - (prot_mass*x[0]*y[0])/(2.0*Beam_Energy);
		float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x[0])/(y[0]*Beam_Energy))));
		float h3 = -0.36544E-03*pow(x[0], -2.1855)*pow(1.0 - x[0], 3.4176)*pow(pim_z[0], -1.7567)*pow(1.0 - pim_z[0], 1.1272)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 8.9985);
		// from Nick: For H3 default values look reasonable.
		float h4 = 0.10908E-02*pow(x[0], -0.35265E-06)*pow(1.0 - x[0], 0.30276E-07)*pow(pim_z[0], -0.66787)*pow(1.0 - pim_z[0], 3.5868)*pow(log(QQ[0]/(0.25*0.25))/log(1.0/(0.25*0.25)), 2.077/x[0]);
		h4 = h4*75.0*(2.0 - x[0]); // from Nick: H4_updated = 75*(2-x)*h4_default[bb = 2.077]
		float Ac = (h3*GTMD*pi_thresh*sqrt(pim_PT2[0]/QQ[0])*(2.0 - y[0])*rtz)/term0;
		float Acc = (h4*GTMD*pi_thresh*(pim_PT2[0]/QQ[0])*rtz*rtz)/term0;
                
		if(Ac > 0.45) Ac = 0.45; // don't want huge moments
		if(Ac < -0.45) Ac = -0.45;
		if(Acc > 0.45) Acc = 0.45;
		if(Acc < -0.45) Acc = -0.45;
                
		pim_weight = 1.0 + Ac*cos(pim_phih[0]*pi180) + Acc*cos(2.0*pim_phih[0]*pi180);
		if(pim_weight != pim_weight) pim_weight = 1.0; // to remove the occasional nan
	      }
	    term0file.close();
	  }
        
	// Some phase space plots. 
	if(e_index[1] > -1){
	  QQvsW->Fill(W[1], QQ[1]);
	  yvsx ->Fill(x[1], y[1]);
	  ethvsx->Fill(x[1], pi180_inv*V3_e[1].Theta());

	  if(QQ[1] > 1 && W[1] > 2.05){
	    yvsx_QQgt1_Wgt2p05  ->Fill(x[1], y[1]);
	    ethvsx_QQgt1_Wgt2p05->Fill(x[1], pi180_inv*V3_e[1].Theta());
	  }
	}
            
	if(e_index[1] > -1 && QQ[1] > 1 && W[1] > 2.05){
	  float tCC = 57.2957795*get_thetaCC(tl3_x[e_index[1]], tl3_y[e_index[1]], tl3_z[e_index[1]], tl3_cx[e_index[1]], tl3_cy[e_index[1]], tl3_cz[e_index[1]]);
	  float prel = get_rel_phi(pi180_inv*V4_e[1].Phi());
	  int thetaCCbin = static_cast<int>((tCC - 5.0)/((50.0 - 5.0)/NthetaCCBins));
	  int relphibin = static_cast<int>((prel - -30.0)/((30.0 - -30.0)/Nrel_phiBins));

	  if(thetaCCbin < NthetaCCBins && thetaCCbin >= 0 && relphibin < Nrel_phiBins && relphibin >= 0){
	    h_nphe[thetaCCbin][relphibin]->Fill(nphe[e_index[1]]);
	  }
	}
	
	// Clever way to use the boolean indexing variable 
	// but I don't think a good practice.  Right now I 
	// don't see a good solution so it stays as is.  
	// 
	// Keep in mind that flipping the convention for 
	// simulation and data would break this entire 
	// block. 
	for(int j = 1; j >= expOrSim; j--){
	  if(pip_keeper[j] && pip_z[j] > 0.4 && pip_z[j] < 0.7){
	    hpip_PT2[j]->Fill(pip_PT2[j]); 
	  }

	  if(e_index[j] > -1 && pip_index[j] > -1){
	    pip_QQvsx[j]->Fill(x[j], QQ[j]); 

	    if(QQ[j] > 1.0 && W[j] > WMin && y[j] < yMax){
	      pip_PT2vsz[j]->Fill(pip_z[j], pip_PT2[j]);

	      if(xBin[j] > -1 && QQBin[j] > -1){
		pip_PT2vsz_xQQbins[xBin[j]][QQBin[j]][j]->Fill(pip_z[j], pip_PT2[j]);
		pip_MX_xQQbins[xBin[j]][QQBin[j]][j]->Fill(MX_epipX[j]);

		if(pip_phihBBin[j] > -1){
		  pip_PT2vsz_xQQphihBbins[xBin[j]][QQBin[j]][pip_phihBBin[j]][j]->Fill(pip_z[j], pip_PT2[j], pip_weight);
		}
	      }
	    }
	  }

	  if(e_index[j] > -1 && pim_index[j] > -1){ 
	    pim_QQvsx[j]->Fill(x[j], QQ[j]); 

	    if (QQ[j] > 1.0 && W[j] > WMin && y[j] < yMax){
	      pim_PT2vsz[j]->Fill(pim_z[j], pim_PT2[j]);

	      if(xBin[j] > -1 && QQBin[j] > -1){
		pim_PT2vsz_xQQbins[xBin[j]][QQBin[j]][j]->Fill(pim_z[j], pim_PT2[j]);
		pim_MX_xQQbins[xBin[j]][QQBin[j]][j]->Fill(MX_epimX[j]);

		if(pim_phihBBin[j] > -1){
		  pim_PT2vsz_xQQphihBbins[xBin[j]][QQBin[j]][pim_phihBBin[j]][j]->Fill(pim_z[j], pim_PT2[j], pim_weight);
		}
	      }
	    }
	  }
	}
        
	// phih histos:
	for(int j = 1; j >= expOrSim; j--){
	  if(pip_keeper[j]){ 
	    h_pip_phih[xBin[j]][QQBin[j]][pip_zBin[j]][pip_PT2Bin[j]][j]->Fill(pip_phih[j], pip_weight); 
	  }
	  if(pim_keeper[j]){ 
	    h_pim_phih[xBin[j]][QQBin[j]][pim_zBin[j]][pim_PT2Bin[j]][j]->Fill(pim_phih[j], pim_weight); 
	  }
	}
	
	// purity:
	if(pip_keeper[0] && pip_keeper[1]){
	  if(pip_phihBin[0] == pip_phihBin[1]){ 
	    pip_phih_pure[xBin[0]][QQBin[0]][pip_zBin[0]][pip_PT2Bin[0]]->Fill(pip_phih[0], pip_weight); 
	  }
	  else {
	    pip_phih_impure[xBin[0]][QQBin[0]][pip_zBin[0]][pip_PT2Bin[0]]->Fill(pip_phih[0], pip_weight);
	  }
	}
        
	if(pim_keeper[0] && pim_keeper[1]){
	  if(pim_phihBin[0] == pim_phihBin[1]){
	    pim_phih_pure[xBin[0]][QQBin[0]][pim_zBin[0]][pim_PT2Bin[0]]->Fill(pim_phih[0], pim_weight); 
	  } 
	  else{
	    pim_phih_impure[xBin[0]][QQBin[0]][pim_zBin[0]][pim_PT2Bin[0]]->Fill(pim_phih[0], pim_weight); 
	  }
	}
	
	// resolution:
	if(expOrSim == 0 && pip_index[0] >= 0 && pip_index[1] >= 0 && V3_pip[0].Mag() > momMin && V3_pip[0].Mag() < momMax){
	  int momBin = static_cast<int>((V3_pip[0].Mag() - momMin)/((momMax - momMin)/NmomBins));
	  pip_mom_res[momBin]  ->Fill(V3_pip[0].Mag() - V3_pip[1].Mag());
	  pip_theta_res[momBin]->Fill(pi180_inv*V3_pip[0].Theta() - pi180_inv*V3_pip[1].Theta());
	  pip_phi_res[momBin]  ->Fill(pi180_inv*V3_pip[0].Phi() - pi180_inv*V3_pip[1].Phi());
	}
            
	// more resolution:
	if(expOrSim == 0 && xBin[0] >= 0 && QQBin[0] >=0 && pip_zBin[0] >= 0 && pip_PT2Bin[0] >= 0 && xBin[1] >= 0 && QQBin[1] >=0 && pip_zBin[1] >= 0 && pip_PT2Bin[1] >= 0){ 
	  float mom = V3_pip[0].Mag();
	  float Dmom = V3_pip[0].Mag() - V3_pip[1].Mag();
	  float Dtheta = V3_pip[0].Theta() - V3_pip[1].Theta();
	  float Dphi = V3_pip[0].Phi() - V3_pip[1].Phi();
	  if(fabs(Dmom) < 3.0*(0.00424 + 0.001184*mom + 0.001945*mom*mom) && fabs(Dtheta) < 3.0*(0.15 + -0.0676*mom + 0.013*mom*mom) && fabs(Dphi) < 3.0*(0.6011 + 0.1176*mom + -0.005821*mom*mom)){
	    pip_PT2_res[xBin[0]][QQBin[0]][pip_zBin[0]][pip_PT2Bin[0]]->Fill(pip_PT2[0] - pip_PT2[1]); // see https://clasweb.jlab.org/rungroups/e1/wiki/index.php/Pip_Resolution_Study_-_June_24,_2014 for the above values
	  }
	}
	
        // Print the status at some points. 
	//	if (i % 10000){
	//	  cout << "\rDone processing (" << i << "/" << h22chain->GetEntries() << ")." << flush; 
	//	}

    } // end if(genExclusiveEvent == 0)
  } // end of loop over entries

  // Clear the output of above. 
  cout << endl; 

  // Calculate runtime and event rate.
  float loopTime  = runtimeCounter.RealTime(); 
  float eventRate = h22chain->GetEntries() / loopTime;  
  cout << "Total time: " << loopTime << " seconds, Event rate: " 
       << eventRate << " events/sec. " << endl; 

  outputfile->Write();
  cout << endl << " Analysis complete. "<< endl;
} 
