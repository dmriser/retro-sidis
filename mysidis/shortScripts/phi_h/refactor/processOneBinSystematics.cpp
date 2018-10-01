/*

  File: processOneBinSystematics.cpp

  Author: Nathan Harrison 
  Revisions: David Riser 
  Date: September 20, 2018

  Order of Systematics 
  --------------------
  00 - Electron Z Vertex 
  01 - Electron Sampling Fraction (ECAL)
  02 - Electron EC Inner vs. Outer Energy Deposition 
  03 - Electron ECAL U,V,W  
  04 - Electron Cherenkov Theta / Segment Matching 
  05 - Electron DC Region 1 Fiducial 
  06 - Electron DC Region 3 Fiducial 
  07 - Electron CC Fiducial 
  08 - Pions Beta vs. P 
  09 - Pions DC Region 1 Fiducial 
  10 - Hadron Phi Fiducial  
  11 - Acceptance Iteration 
  12 - HAPRAD Model 

  Changelog:
  ---------
  2018-09-20: Created this entry, starting modifications. Fixed indentation. 
  2018-09-21: Fixing if statements, formatting to my style.  Running initial
              tests on one bin. Adding more print statements, just for the 
              fun of it.  It helps me see which files are opened successfully. 
  2018-09-25: Various refactorings while waiting for batch farm test jobs.
  2018-09-27: Creating utilities file. Modularizing operations throughout the code. 
  2018-09-28: Adding functions for loading radiative corrections and generating
              haprad file names.  Fitter starting giving results. 
*/

#include <iostream>
#include <map>
#include <vector>

// Included files from this project 
#include "utils.cpp"

namespace constants { 
  const int n_phi_bins = 36; 
  const int n_sources = 13; 
  const int n_variations_per_source = 2; 
}

void processOneBinSystematics(int xBin = 0, int QQBin = 0, int zBin = 3, 
			      int PT2Bin = 0, string hadronType = "pip"){


  //  gStyle->SetOptStat(0);
  
  const bool SAVE_ROOT_FILE = 1;
  TFile *rootFile;
  if(SAVE_ROOT_FILE){
    rootFile = new TFile("Systematics_v2.root", "update");
  }

  //  const int N_PHI_BINS            = 36;
  //  const int N_SOURCES             = 13;
  //  const int VARIATIONS_PER_SOURCE = 2;

  const int N_PHI_BINS = constants::n_phi_bins;
  const int N_SOURCES = constants::n_sources;
  const int VARIATIONS_PER_SOURCE = constants::n_variations_per_source;

  const string MESSAGE("[ProcessOneBinSystematics] "); 
  const string BASE_DIRECTORY("/volatile/clas12/dmriser/farm_out"); 
  const string PROJECT_NAME("sidis_batch_11"); 
  const string PATH_TO_REQUIRED_FILES("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles"); 

  const string SOURCE_NAME[N_SOURCES] = {
    "e_zvert", "e_ECsamp", "e_ECoVi", "e_ECgeo", "e_CCthMatch", 
    "e_R1fid", "e_R3fid", "e_CCfid", "pi_vvp", "pi_R1fid", 
    "phih_fid", "accModel", "hapModel"
  };

  map<int, int> arrayIndexToVariation; 
  arrayIndexToVariation[0] =  0; 
  arrayIndexToVariation[1] =  1; 
  arrayIndexToVariation[2] =  2; 
  arrayIndexToVariation[3] =  3; 
  arrayIndexToVariation[4] =  4; 
  arrayIndexToVariation[5] =  5; 
  arrayIndexToVariation[6] =  6; 
  arrayIndexToVariation[7] =  8; 
  arrayIndexToVariation[8] = 10; 
  arrayIndexToVariation[9] = 11; 

  
  float M_sysErrorPiece[N_SOURCES]; 
  float Ac_sysErrorPiece[N_SOURCES];
  float Acc_sysErrorPiece[N_SOURCES];

  float M_sumOfSquaredErrors   = 0;
  float Ac_sumOfSquaredErrors  = 0;
  float Acc_sumOfSquaredErrors = 0;

  float M_sysErrorTotal;
  float Ac_sysErrorTotal;
  float Acc_sysErrorTotal;


  string MString   = Form("hM_sysEcontributions_%s_%i_%i_%i_%i", 
			  hadronType.c_str(), xBin, QQBin, zBin, PT2Bin);
  string AcString  = Form("hAc_sysEcontributions_%s_%i_%i_%i_%i", 
			  hadronType.c_str(), xBin, QQBin, zBin, PT2Bin);
  string AccString = Form("hAcc_sysEcontributions_%s_%i_%i_%i_%i", 
			  hadronType.c_str(), xBin, QQBin, zBin, PT2Bin);
  string catString = Form("hCategory_%s_%i_%i_%i_%i", 
			  hadronType.c_str(), xBin, QQBin, zBin, PT2Bin);
  TH1F *hM_sysEcontributions   = new TH1F(MString.c_str(), MString.c_str(), 
					  N_SOURCES, 0, N_SOURCES);
  TH1F *hAc_sysEcontributions  = new TH1F(AcString.c_str(), AcString.c_str(), 
					  N_SOURCES, 0, N_SOURCES);
  TH1F *hAcc_sysEcontributions = new TH1F(AccString.c_str(), AccString.c_str(), 
					  N_SOURCES, 0, N_SOURCES);
  TH1F *hCategory              = new TH1F(catString.c_str(), catString.c_str(), 
					  1, 0, 1);

  // At this point declarations that persist have stopped and the 
  // loading of files begins.  More declarations occur inside of 
  // the loop that are out of scope afterward. 
 
  TFile *tfdata[N_SOURCES][VARIATIONS_PER_SOURCE];
  // 
  // This part needs to be done carefully, and I believe
  // that I have done so.  The variation number changes at
  // times by more than one, and the array index is just 
  // incremented by one.  This is the way that it was done 
  // by Nathan and I don't want to change that. 
  // 
  for (int i = 0; i < N_SOURCES; i++){
    string looseFilename = Utils::createFilename(BASE_DIRECTORY, PROJECT_NAME, 
						 "data", arrayIndexToVariation[i], false);
    string tightFilename = Utils::createFilename(BASE_DIRECTORY, PROJECT_NAME, 
						 "data", arrayIndexToVariation[i], true);

    tfdata[i][0] = new TFile(looseFilename.c_str()); 
    tfdata[i][1] = new TFile(tightFilename.c_str());

    // Safety first. 
    if ( !tfdata[i][0]->IsOpen() ){
      cerr << MESSAGE << "Couldn't open " << looseFilename << endl; 
      return;
    }
    if ( !tfdata[i][1]->IsOpen() ){
      cerr << MESSAGE << "Couldn't open " << tightFilename << endl; 
      return;
    }
 
  }

  // Is it safe to have 6 pointers to an open file?  How many times is the file open? 
  // [10] is for testing phih cuts (bins with a phih cut only)... don't change the number!
  // for acc. model dependence. See note below (at tfmc[11][0])!!
  // for hap. model dependence. See note below (at tfmc[12][0])!!

  string nominalFilenameData = Form("%s/%s/data/variation_nominal/merged.root", 
				    BASE_DIRECTORY.c_str(), PROJECT_NAME.c_str());

  tfdata[10][0] = new TFile(nominalFilenameData.c_str());
  tfdata[10][1] = new TFile(nominalFilenameData.c_str());
  tfdata[11][0] = new TFile(nominalFilenameData.c_str());
  tfdata[11][1] = new TFile(nominalFilenameData.c_str());
  tfdata[12][0] = new TFile(nominalFilenameData.c_str());
  tfdata[12][1] = new TFile(nominalFilenameData.c_str());

  TFile *tfmc[N_SOURCES][VARIATIONS_PER_SOURCE];
  for (int i = 0; i < 10; i++){
    string looseFilename = Utils::createFilename(BASE_DIRECTORY, PROJECT_NAME, 
						 "mc", arrayIndexToVariation[i], false);
    string tightFilename = Utils::createFilename(BASE_DIRECTORY, PROJECT_NAME, 
						 "mc", arrayIndexToVariation[i], true);

    tfmc[i][0] = new TFile(looseFilename.c_str()); 
    tfmc[i][1] = new TFile(tightFilename.c_str());

    // Safety first. 
    if ( !tfmc[i][0]->IsOpen() ){
      cerr << MESSAGE << "Couldn't open " << looseFilename << endl; 
      return;
    }
    if ( !tfmc[i][1]->IsOpen() ){
      cerr << MESSAGE << "Couldn't open " << tightFilename << endl; 
      return;
    }

  }

  // [10] is for testing phih cuts (bins with a phih cut only)... don't change the number!
  //
  // [11] This comment was left by Nathan on the 11th systematic. 
  // need to do a work-around here since there's only one variation... 
  // the "loose" one is the variation and the "tight" one is just the nominal case so it won't 
  // contribute anything to the error (nominal - tight = 0)... just remember to divide by sqrt(1) 
  // instead of sqrt(2)!!
  //
  // [12] for hap. model dependence. similar work-around here as above. 
  // see below where the haprad files are read in for details

  string nominalFilenameMC = Form("%s/%s/mc/variation_nominal/merged.root", 
				  BASE_DIRECTORY.c_str(), PROJECT_NAME.c_str());
  string acceptanceFilenameMC = Form("%s/%s/mc/variation_acceptance/merged.root", 
				     BASE_DIRECTORY.c_str(), PROJECT_NAME.c_str());
  tfmc[10][0] = new TFile(   nominalFilenameMC.c_str()); 
  tfmc[10][1] = new TFile(   nominalFilenameMC.c_str()); 
  tfmc[11][0] = new TFile(acceptanceFilenameMC.c_str()); 
  tfmc[11][1] = new TFile(   nominalFilenameMC.c_str()); 
  tfmc[12][0] = new TFile(   nominalFilenameMC.c_str()); 
  tfmc[12][1] = new TFile(   nominalFilenameMC.c_str()); 

  TLatex *tlat = new TLatex();
  tlat->SetNDC();
  tlat->SetTextSize(0.06);

  // Read into memory the category file for this bin. 
  string categoryFilename = Form("%s/binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", 
				 PATH_TO_REQUIRED_FILES.c_str(), hadronType.c_str(), 
				 xBin, QQBin, zBin, PT2Bin); 
  int defaultCategory = Utils::getBinCategory(categoryFilename, MESSAGE);
  int category = defaultCategory; 

  //-----------------------------------------------------------
  //--------------- do the nominal case -----------------------
  //-----------------------------------------------------------
  cout << MESSAGE << "Starting calculation for nominal case..." << endl; 

  TCanvas *nomcan  = new TCanvas();
  TFile *tfdataNom = new TFile(nominalFilenameData.c_str());
  TFile *tfmcNom   = new TFile(nominalFilenameMC.c_str());

  
  // Simple safety checks on the file. 
  if (tfdataNom && tfdataNom->IsOpen()){
    cout << MESSAGE << "Opening sucessfully: " << nominalFilenameData << endl; 
  } else {
    cerr << MESSAGE << "Dying on not being able to open " << nominalFilenameData << endl; 
    return; 
  }
  
  if (tfmcNom && tfmcNom->IsOpen()){
    cout << MESSAGE << "Opening sucessfully: " << nominalFilenameMC << endl; 
  } else {
    cerr << MESSAGE << "Dying on not being able to open " << nominalFilenameMC << endl; 
    return; 
  }

  TH1F *hdataphihModified = (TH1F*) tfdataNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));

  // A little bit of protection because the 
  // code is dying. 
  if(hdataphihModified){
    cout << MESSAGE << "Opened nominal data histogram with entries = " << hdataphihModified->GetEntries() << endl;
  } else {
    cout << MESSAGE << "Dying because the nominal data histogram could not be opened." << endl; 
    return; 
  }

  // apply some modifications and further define the bin category:
  Utils::removeBinsWithCountsLower(hdataphihModified, 10); 
  Utils::removeBinsInCentralPhi(hdataphihModified, category); 
  int numberOfEmptyPhiBins = Utils::countEmptyBins(hdataphihModified); 

  // update/redefine category here:
  // bad statistics and bad coverage, don't use this bin
  // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, Ac, and Acc for this bin
  // Measure only M for all other bins
  // note: category is further updated at the top of the corrRC section
  
  // Category codes below (as far as I could workout from the comments above). 
  //     -99 : Bad statistics and bad coverage, DO NOT use this bin.
  //       1 : Measure the values for this bin. 
  //     -13 : Measure only the multiplcity M for this bin.  
  category = Utils::updateCategoryBasedOnEmptyBins(hdataphihModified, 
						   numberOfEmptyPhiBins, 
						   category); 
  
  //--------------------- MC: ---------------------------------
  TH1F *hgenphih, *hrecphih, *haccphih;

  hgenphih = (TH1F*) tfmcNom->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  hrecphih = (TH1F*) tfmcNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  haccphih = new TH1F(Form("haccphih_z%iPT2%i", zBin, PT2Bin), Form("haccphih_z%iPT2%i", zBin, PT2Bin), N_PHI_BINS, -180, 180);
  haccphih->Sumw2();
  haccphih->Divide(hrecphih, hgenphih);

  //------------------ haprad: --------------------------------

  TH1F *hsig = new TH1F("hsig", "hsig", N_PHI_BINS, -180, 180);
  TH1F *hsib = new TH1F("hsib", "hsib", N_PHI_BINS, -180, 180);
  TH1F *hRC  = new TH1F( "hRC",  "hRC", N_PHI_BINS, -180, 180);
  hsig->Sumw2();
  hsib->Sumw2();
  hRC ->Sumw2();

  // Build path to HAPRAD output files for radiative corrections 
  // and proceed to find and open the file for this bin.  If there 
  // is no file, a message is printed and the calculation continues. 
  //
  std::string happath = Utils::buildHapradPath(PATH_TO_REQUIRED_FILES, hadronType, 
					       xBin, QQBin, zBin, PT2Bin, -1, -1);

  ifstream hapfile(happath.c_str());
  if(hapfile){
    Utils::loadRadiativeCorrection(hapfile, hsig, hsib, hRC, hadronType);

    hapfile.close();
    cout << MESSAGE << "Found haprad file: " << happath << endl; 
  } else {
    cout << MESSAGE << "Didn't find haprad file: " << happath << endl; 
  }

  hRC->Divide(hsig, hsib);

  //------------------- corr: ---------------------------------
  TH1F *hcorr = new TH1F("hcorr", "hcorr", N_PHI_BINS, -180, 180);
  hcorr->Sumw2();
  hcorr->Divide(hdataphihModified, haccphih);

  //------------------- corrRC: -------------------------------
  // defining category = -2 here means no haprad results
  if(hsib->Integral() < 0.00000001 || hsig->Integral() < 0.00000001){ 
    category = -2; 
  }

  TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", N_PHI_BINS, -180, 180);
  hcorrRC->Sumw2();
  hcorrRC->Divide(hcorr, hRC);
  hcorrRC->GetYaxis()->SetRangeUser(0, 1.1*hcorrRC->GetMaximum());
  hcorrRC->GetXaxis()->SetTitle("phi_h (deg.)");
  hcorrRC->SetTitle("acc. corr. and rad. corr. data");
  hcorrRC->Draw();

  TF1 *ffcorrRC = new TF1("ffcorrRC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
  ffcorrRC->SetLineColor(kBlue);
  ffcorrRC->SetParameters(hcorrRC->GetMaximum(), 0.0, 0.0);

  if(category == 1){
    ffcorrRC->SetParLimits(1, -0.99, 0.99);
    ffcorrRC->SetParLimits(2, -0.99, 0.99);
  } else {
    ffcorrRC->SetParLimits(1, -0.30, 0.30);
    ffcorrRC->SetParLimits(2, -0.30, 0.30);
  }

  if(hcorrRC->Integral() > 36){ 
    hcorrRC->Fit("ffcorrRC", "q", "", -180, 180); 
  }

  tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRC->GetParameter(0), ffcorrRC->GetParError(0)));
  tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRC->GetParameter(1), ffcorrRC->GetParError(1)));
  tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRC->GetParameter(2), ffcorrRC->GetParError(2)));

  std::cout << MESSAGE << "nominal M: " << ffcorrRC->GetParameter(0) << " +/- " << ffcorrRC->GetParError(0) << std::endl; 

  int finalNomCategory = category;
  //-----------------------------------------------------------
  //----------------- do the variations ----------------------- checkpoint
  //-----------------------------------------------------------

  TCanvas *can = new TCanvas("can", "can", 5, 5, 1600, 1000);
  can->Divide(6, 4, 0.00001, 0.00001); // may need manual update

  TH1F *hdataphihModifiedS[N_SOURCES][VARIATIONS_PER_SOURCE]; // S for Systematics
  TH1F *hgenphihS[N_SOURCES][VARIATIONS_PER_SOURCE], *hrecphihS[N_SOURCES][VARIATIONS_PER_SOURCE], *haccphihS[N_SOURCES][VARIATIONS_PER_SOURCE];
  TH1F *hsigS[N_SOURCES][VARIATIONS_PER_SOURCE], *hsibS[N_SOURCES][VARIATIONS_PER_SOURCE], *hRCS[N_SOURCES][VARIATIONS_PER_SOURCE];
  TH1F *hcorrS[N_SOURCES][VARIATIONS_PER_SOURCE];
  TH1F *hcorrRCS[N_SOURCES][VARIATIONS_PER_SOURCE];
  TF1 *ffcorrRCS[N_SOURCES][VARIATIONS_PER_SOURCE];


  int count = 0;
  for(int s = 0; s < N_SOURCES; s++) {
    for(int v = 0; v < VARIATIONS_PER_SOURCE; v++) {
      if(!((s == 11 && v == 1) || (s == 12 && v == 1))) {
	count++;
	can->cd(count);

	// Reset to the original category. 
	category = defaultCategory; 
	if(category > 0.5 && s == 10 && v == 0){ 
	  category -= 10; 
	}
	if(category > 0.5 && s == 10 && v == 1){
	  category += 10;
	}
	//-------------------- data: --------------------------------

	hdataphihModifiedS[s][v] = (TH1F*) tfdata[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hdataphihModifiedS[s][v]->SetName(Form("hdataMS_%i_%i", s, v));

	//	cout << MESSAGE << "Entries (data) = " << hdataphihModifiedS[s][v]->GetEntries() << endl; 

	// apply some modifications and further define the bin category:	  
	Utils::removeBinsWithCountsLower(hdataphihModifiedS[s][v], 10);
	Utils::removeBinsInCentralPhi(hdataphihModifiedS[s][v], category);
	int numberOfEmptyPhiBins = Utils::countEmptyBins(hdataphihModifiedS[s][v]);

	// Category codes below (as far as I could workout from the comments above).
	//     -99 : Bad statistics and bad coverage, DO NOT use this bin.
	//       1 : Measure the values for this bin.
	//     -13 : Measure only the multiplcity M for this bin.
	category = Utils::updateCategoryBasedOnEmptyBins(hdataphihModifiedS[s][v],
							 numberOfEmptyPhiBins,
							 category);


	//--------------------- MC: ---------------------------------
	hgenphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hgenphihS[s][v]->SetName(Form("hgenphihS_%i_%i", s, v));
	//	cout << MESSAGE << "Entries (generated) = " << hgenphihS[s][v]->GetEntries() << endl; 

	hrecphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hrecphihS[s][v]->SetName(Form("hrecphihS_%i_%i", s, v));
	//	cout << MESSAGE << "Entries (reconstructed) = " << hrecphihS[s][v]->GetEntries() << endl; 

	// Might want to set the errors directly using manual loop over bins.  Sometimes this division does weird things 
	// even when Sumw2 is called first.  Perhaps it doesn't matter anymore. 
	haccphihS[s][v] = new TH1F(Form("haccphihS_z%iPT2%i_%i_%i", zBin, PT2Bin, s, v), Form("haccphihS_z%iPT2%i", zBin, PT2Bin), N_PHI_BINS, -180, 180);
	haccphihS[s][v]->Sumw2();
	haccphihS[s][v]->Divide(hrecphihS[s][v], hgenphihS[s][v]);
	//	cout << MESSAGE << "Entries (acceptance) = " << haccphihS[s][v]->GetEntries() << endl; 

	//------------------ haprad: --------------------------------
	hsigS[s][v] = new TH1F(Form("hsigS_%i_%i", s, v), Form("hsigS_%i_%i", s, v), N_PHI_BINS, -180, 180);
	hsigS[s][v]->Sumw2();
	hsibS[s][v] = new TH1F(Form("hsibS_%i_%i", s, v), Form("hsibS_%i_%i", s, v), N_PHI_BINS, -180, 180);
	hsibS[s][v]->Sumw2();
	hRCS[s][v] = new TH1F(Form("hRCS_%i_%i", s, v), Form("hRCS_%i_%i", s, v), N_PHI_BINS, -180, 180);
	hRCS[s][v]->Sumw2();

	string happathS = Utils::buildHapradPath(PATH_TO_REQUIRED_FILES, hadronType, 
						 xBin, QQBin, zBin, PT2Bin, s, v); 
	ifstream hapfileS(happathS.c_str());

	if(hapfileS){
	  Utils::loadRadiativeCorrection(hapfileS, hsigS[s][v], hsibS[s][v], hRCS[s][v], hadronType);
	} else {
	  std::cerr << MESSAGE << "Unable to locate HAPRAD file: " << happathS << std::endl;
	}


	hRCS[s][v]->Divide(hsigS[s][v], hsibS[s][v]);
	//	cout << MESSAGE << "Entries (sigS) = " << hsigS[s][v]->GetEntries() << endl; 
	//	cout << MESSAGE << "Entries (sibS) = " << hsibS[s][v]->GetEntries() << endl; 
	//	cout << MESSAGE << "Entries (hRCS) = " << hRCS[s][v]->GetEntries() << endl; 

	//------------------- corr: ---------------------------------

	hcorrS[s][v] = new TH1F(Form("hcorrS_%i_%i", s, v), Form("hcorrS_%i_%i", s, v), N_PHI_BINS, -180, 180);
	hcorrS[s][v]->Sumw2();
	hcorrS[s][v]->Divide(hdataphihModifiedS[s][v], haccphihS[s][v]);
	hcorrS[s][v]->Draw();
	//	cout << MESSAGE << "Entries (hcorrS) = " << hcorrS[s][v]->GetEntries() << endl; 	  

	//------------------- corrRC: -------------------------------

	if(hsibS[s][v]->Integral() < 0.00000001 || hsigS[s][v]->Integral() < 0.00000001){
	  category = -2;
	}

	hcorrRCS[s][v] = new TH1F(Form("hcorrRCS_%i_%i", s, v), Form("hcorrRCS_%i_%i", s, v), N_PHI_BINS, -180, 180);
	hcorrRCS[s][v]->Sumw2();
	hcorrRCS[s][v]->Divide(hcorrS[s][v], hRCS[s][v]);
	hcorrRCS[s][v]->GetYaxis()->SetRangeUser(0, 1.1*hcorrRCS[s][v]->GetMaximum());
	hcorrRCS[s][v]->GetXaxis()->SetTitle("phi_h (deg.)");
	hcorrRCS[s][v]->SetTitle(SOURCE_NAME[s].c_str());
	hcorrRCS[s][v]->Draw();

	ffcorrRCS[s][v] = new TF1(Form("ffcorrRCS_%i_%i", s, v), "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
	ffcorrRCS[s][v]->SetLineColor(kBlue);
	ffcorrRCS[s][v]->SetParameters(hcorrRCS[s][v]->GetMaximum(), 0.0, 0.0);

	if(category == 1){
	  ffcorrRCS[s][v]->SetParLimits(1, -0.99, 0.99);
	  ffcorrRCS[s][v]->SetParLimits(2, -0.99, 0.99);
	} else {
	  ffcorrRCS[s][v]->SetParLimits(1, -0.30, 0.30);
	  ffcorrRCS[s][v]->SetParLimits(2, -0.30, 0.30);
	}

	if(hcorrRCS[s][v]->Integral() > 36){
	  //	  cout << MESSAGE << "Fitting (" << s << "," << v << ")" << endl;
	  hcorrRCS[s][v]->Fit(Form("ffcorrRCS_%i_%i", s, v), "q", "", -180, 180);
	} else {
	  cout << MESSAGE << "Not Fitting (" << s << "," << v 
	       << ") with integral = " << hcorrRCS[s][v]->Integral() 
	       << " and entries = " << hcorrRCS[s][v]->GetEntries() << endl;
	}
      
	//	  tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRCS[s][v]->GetParameter(0), ffcorrRCS[s][v]->GetParError(0)));
	//	  tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(1), ffcorrRCS[s][v]->GetParError(1)));
	//	  tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(2), ffcorrRCS[s][v]->GetParError(2)));

	//tlat->DrawLatex(0.25, 0.14, Form("category %i", category));
      } // endif
    } // end v loop
    

    // This is part of the "work-around" Nathan mentions above. 
    if(s <= 10){
      M_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(0) - ffcorrRC->GetParameter(0), 2.0) + 
				pow(ffcorrRCS[s][1]->GetParameter(0) - ffcorrRC->GetParameter(0), 2.0))/sqrt(2.0);
      Ac_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(1) - ffcorrRC->GetParameter(1), 2.0) + 
				 pow(ffcorrRCS[s][1]->GetParameter(1) - ffcorrRC->GetParameter(1), 2.0))/sqrt(2.0);
      Acc_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(2) - ffcorrRC->GetParameter(2), 2.0) + 
				  pow(ffcorrRCS[s][1]->GetParameter(2) - ffcorrRC->GetParameter(2), 2.0))/sqrt(2.0);
    }

    else {
      M_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(0) - ffcorrRC->GetParameter(0)); // one variation, so it simplifies to this
      Ac_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(1) - ffcorrRC->GetParameter(1));
      Acc_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(2) - ffcorrRC->GetParameter(2));
    }

    hM_sysEcontributions  ->SetBinContent(s+1, M_sysErrorPiece[s]);
    hAc_sysEcontributions ->SetBinContent(s+1, Ac_sysErrorPiece[s]);
    hAcc_sysEcontributions->SetBinContent(s+1, Acc_sysErrorPiece[s]);

    // Add in quadrature
    M_sumOfSquaredErrors   += pow(  M_sysErrorPiece[s], 2); 
    Ac_sumOfSquaredErrors  += pow( Ac_sysErrorPiece[s], 2);
    Acc_sumOfSquaredErrors += pow(Acc_sysErrorPiece[s], 2); 

  } 

  hCategory->SetBinContent(1, finalNomCategory);
  M_sysErrorTotal   = sqrt(M_sumOfSquaredErrors);
  Ac_sysErrorTotal  = sqrt(Ac_sumOfSquaredErrors);
  Acc_sysErrorTotal = sqrt(Acc_sumOfSquaredErrors);

  // Print out a table summarizing what happened. 
  // Print table header. 

  cout << MESSAGE << setw(8) << "Source #"
       << setw(16) << "Source Name" << setw(12) << "dM" 
       << setw(12) << "dAc" << setw(12) << "dAcc" << endl; 

  for (int sourceIndex = 0; sourceIndex < N_SOURCES; sourceIndex++){
    std::cout << MESSAGE 
	      << setw(8)  << sourceIndex 
	      << setw(16) << SOURCE_NAME[sourceIndex] 
	      << setw(12) << std::setprecision(2) << std::scientific << M_sysErrorPiece[sourceIndex] 
	      << setw(12) << std::setprecision(2) << std::scientific << Ac_sysErrorPiece[sourceIndex] 
	      << setw(12) << std::setprecision(2) << std::scientific << Acc_sysErrorPiece[sourceIndex] 
	      << std::endl;
  }

  cout << MESSAGE << setw(12) << M_sysErrorTotal 
       << setw(12) << Ac_sysErrorTotal << setw(12) << Acc_sysErrorTotal<<endl;

  TCanvas *can2 = new TCanvas("can2", "can2", 15, 15, 1400, 700);
  can2->Divide(3, 1, 0.00001, 0.00001);
  can2->cd(1);
  hM_sysEcontributions->GetXaxis()->SetTitle("source");
  hM_sysEcontributions->SetTitle("A_{0} systematic errors");
  hM_sysEcontributions->Draw();
  can2->cd(2);
  hAc_sysEcontributions->GetXaxis()->SetTitle("source");
  hAc_sysEcontributions->SetTitle("A^{cos#phi}_{UU} systematic errors");
  hAc_sysEcontributions->Draw();
  can2->cd(3);
  hAcc_sysEcontributions->GetXaxis()->SetTitle("source");
  hAcc_sysEcontributions->SetTitle("A^{cos2#phi}_{UU} systematic errors");
  hAcc_sysEcontributions->Draw();

  if(SAVE_ROOT_FILE){
    rootFile->Write();
  }
} 
