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
  2018-10-02: Added the Dataset feature, making this function callable 
              without having to load the entire dataset each time. 
  2018-10-03: Introducing four and five dimensional tables for tracking the 
              bin category and haprad data. 

*/

// Standard c++ Libraries 
#include <iostream>
#include <map>
#include <vector>

// Included files from this project 
#include "constants.cpp"
#include "dataset.cpp"
#include "histograms.cpp"
#include "tables.cpp"
#include "utils.cpp"

void processOneBinSystematics(Dataset & dataset, BinCategoryTable & binCategoryTable, 
			      HapradTable & hapradTable, 
			      int xBin = 0, int QQBin = 0, int zBin = 3, 
			      int PT2Bin = 4, string hadronType = "pip"){

  const bool SAVE_ROOT_FILE = 1;
  TFile *rootFile;
  if(SAVE_ROOT_FILE){
    rootFile = new TFile("Systematics_v2.root", "update");
  }
 
  const string MESSAGE("[ProcessOneBinSystematics] "); 
  const string BASE_DIRECTORY("/volatile/clas12/dmriser/farm_out"); 
  const string PROJECT_NAME("sidis_batch_11"); 
  const string PATH_TO_REQUIRED_FILES("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles"); 

  const string SOURCE_NAME[constants::n_sources] = {
    "e_zvert", "e_ECsamp", "e_ECoVi", "e_ECgeo", "e_CCthMatch", 
    "e_R1fid", "e_R3fid", "e_CCfid", "pi_vvp", "pi_R1fid", 
    "phih_fid", "accModel", "hapModel"
  };
   
  float M_sysErrorPiece[constants::n_sources]; 
  float Ac_sysErrorPiece[constants::n_sources];
  float Acc_sysErrorPiece[constants::n_sources];

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
					  constants::n_sources, 0, constants::n_sources);
  TH1F *hAc_sysEcontributions  = new TH1F(AcString.c_str(), AcString.c_str(), 
					  constants::n_sources, 0, constants::n_sources);
  TH1F *hAcc_sysEcontributions = new TH1F(AccString.c_str(), AccString.c_str(), 
					  constants::n_sources, 0, constants::n_sources);
  TH1F *hCategory              = new TH1F(catString.c_str(), catString.c_str(), 
					  1, 0, 1);
 
  // Read into memory the category file for this bin. 
  /* 
  string categoryFilename = Form("%s/binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", 
				 PATH_TO_REQUIRED_FILES.c_str(), hadronType.c_str(), 
				 xBin, QQBin, zBin, PT2Bin); 
  int defaultCategory = Utils::getBinCategory(categoryFilename, MESSAGE);
  int category = defaultCategory; 
  */

  int defaultCategory = binCategoryTable.querySafe(xBin, QQBin, zBin, PT2Bin);
  int category = defaultCategory; 

  //-----------------------------------------------------------
  //--------------- do the nominal case -----------------------
  //-----------------------------------------------------------
  cout << MESSAGE << "Starting calculation for nominal case..." << endl; 

  HistogramPack nominalHistos = createPack(hadronType, xBin, QQBin, zBin, PT2Bin); 
  nominalHistos.data = (TH1F*) dataset.fDataNominalFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));

  //  TH1F *hdataphihModified = (TH1F*) dataset.fDataNominalFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));

  // A little bit of protection because the 
  // code is dying. 
  /* 
  if(hdataphihModified){
    cout << MESSAGE << "Opened nominal data histogram with entries = " << hdataphihModified->GetEntries() << endl;
  } else {
    cout << MESSAGE << "Dying because the nominal data histogram could not be opened." << endl; 
    return; 
  }
  */

  Utils::removeBinsWithCountsLower(nominalHistos.data, 10); 
  Utils::removeBinsInCentralPhi(nominalHistos.data, category); 
  int numberOfEmptyPhiBins = Utils::countEmptyBins(nominalHistos.data); 

  // update/redefine category here:
  // bad statistics and bad coverage, don't use this bin
  // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, Ac, and Acc for this bin
  // Measure only M for all other bins
  // note: category is further updated at the top of the corrRC section
  
  // Category codes below (as far as I could workout from the comments above). 
  //     -99 : Bad statistics and bad coverage, DO NOT use this bin.
  //       1 : Measure the values for this bin. 
  //     -13 : Measure only the multiplcity M for this bin.  
  category = Utils::updateCategoryBasedOnEmptyBins(nominalHistos.data, 
						   numberOfEmptyPhiBins, 
						   category); 
  
  //--------------------- MC: ---------------------------------
  //  TH1F *hgenphih, *hrecphih, *haccphih;
  /*
  hgenphih = (TH1F*) dataset.fMCNominalFile->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", 
						      hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  hrecphih = (TH1F*) dataset.fMCNominalFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", 
						      hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  haccphih = new TH1F(Form("haccphih_z%iPT2%i", zBin, PT2Bin), 
		      Form("haccphih_z%iPT2%i", zBin, PT2Bin), 
		      constants::n_phi_bins, -180, 180);
  haccphih->Sumw2();
  haccphih->Divide(hrecphih, hgenphih);
  */

  nominalHistos.gen = (TH1F*) dataset.fMCNominalFile->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", 
							       hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  nominalHistos.rec = (TH1F*) dataset.fMCNominalFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", 
							       hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  nominalHistos.acc->Divide(nominalHistos.rec, nominalHistos.gen); 

  //------------------ haprad: --------------------------------

  TH1F *hsig = new TH1F("hsig", "hsig", constants::n_phi_bins, constants::phi_min, constants::phi_max);
  TH1F *hsib = new TH1F("hsib", "hsib", constants::n_phi_bins, constants::phi_min, constants::phi_max);
  TH1F *hRC  = new TH1F( "hRC",  "hRC", constants::n_phi_bins, constants::phi_min, constants::phi_max);
  hsig->Sumw2();
  hsib->Sumw2();
  hRC ->Sumw2();

  // This small block loads the radiative correction histograms 
  // from the haprad table provided in the input. 
  for (int i = 0; i < constants::n_phi_bins; i++) {
    HapradDataEntry entry = hapradTable.querySafe(xBin, QQBin, zBin, PT2Bin, i); 

    if (hadronType == "pim"){
      entry.sig -= entry.tail; 
      entry.tail = 0; 
    }

    hsig->SetBinContent(i + 1, entry.sig);
    hsig->SetBinError(i + 1, 0);
    hsib->SetBinContent(i + 1, entry.sib);
    hsib->SetBinError(i + 1, 0);
  }

  nominalHistos.rc->Divide(hsig, hsib);

  /* 
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
  */

  //------------------- corr: ---------------------------------
  //  TH1F *hcorr = new TH1F("hcorr", "hcorr", constants::n_phi_bins, -180, 180);
  //  hcorr->Sumw2();
  //  hcorr->Divide(hdataphihModified, haccphih);
  nominalHistos.corrAcc->Divide(nominalHistos.data, nominalHistos.acc); 

  //------------------- corrRC: -------------------------------
  // defining category = -2 here means no haprad results
  if(hsib->Integral() < constants::tiny || hsig->Integral() < constants::tiny){ 
    category = -2; 
  }

  /* 
  TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", constants::n_phi_bins, -180, 180);
  hcorrRC->Sumw2();
  hcorrRC->Divide(hcorr, hRC);
  hcorrRC->GetYaxis()->SetRangeUser(0, 1.1*hcorrRC->GetMaximum());
  hcorrRC->GetXaxis()->SetTitle("phi_h (deg.)");
  hcorrRC->SetTitle("acc. corr. and rad. corr. data");
  */
  nominalHistos.corrRC->Divide(nominalHistos.data, nominalHistos.rc);
  nominalHistos.corr->Divide(nominalHistos.corrAcc, nominalHistos.rc);

  TF1 *ffcorrRC = new TF1("ffcorrRC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", 
			  constants::phi_min, constants::phi_max);
  ffcorrRC->SetLineColor(kBlue);
  ffcorrRC->SetParameters(nominalHistos.corr->GetMaximum(), 0.0, 0.0);

  if(category == 1){
    ffcorrRC->SetParLimits(1, -0.99, 0.99);
    ffcorrRC->SetParLimits(2, -0.99, 0.99);
  } else {
    ffcorrRC->SetParLimits(1, -0.30, 0.30);
    ffcorrRC->SetParLimits(2, -0.30, 0.30);
  }

  if(nominalHistos.corr->Integral() > 36){ 
    nominalHistos.corr->Fit("ffcorrRC", "q", "", constants::phi_min, constants::phi_max); 
  }

  std::cout << MESSAGE << "nominal M: " << ffcorrRC->GetParameter(0) << " +/- " << ffcorrRC->GetParError(0) << std::endl; 

  int finalNomCategory = category;
  //-----------------------------------------------------------
  //----------------- do the variations ----------------------- checkpoint
  //-----------------------------------------------------------

  //  TH1F *hdataphihModifiedS[constants::n_sources][constants::n_variations_per_source]; // S for Systematics
  //  TH1F *hgenphihS[constants::n_sources][constants::n_variations_per_source], *hrecphihS[constants::n_sources][constants::n_variations_per_source], *haccphihS[constants::n_sources][constants::n_variations_per_source];
  //  TH1F *hsigS[constants::n_sources][constants::n_variations_per_source], *hsibS[constants::n_sources][constants::n_variations_per_source], *hRCS[constants::n_sources][constants::n_variations_per_source];
  TH1F *hsigS[constants::n_sources][constants::n_variations_per_source], *hsibS[constants::n_sources][constants::n_variations_per_source];
  //  TH1F *hcorrS[constants::n_sources][constants::n_variations_per_source];
  //  TH1F *hcorrRCS[constants::n_sources][constants::n_variations_per_source];
  TF1 *ffcorrRCS[constants::n_sources][constants::n_variations_per_source];

  int count = 0;
  for(int s = 0; s < constants::n_sources; s++) {
    for(int v = 0; v < constants::n_variations_per_source; v++) {
      if(!((s == 11 && v == 1) || (s == 12 && v == 1))) {
	count++;

	// Reset to the original category. 
	category = defaultCategory; 
	if(category > 0.5 && s == 10 && v == 0){ 
	  category -= 10; 
	}
	if(category > 0.5 && s == 10 && v == 1){
	  category += 10;
	}
	//-------------------- data: --------------------------------

	hdataphihModifiedS[s][v] = (TH1F*) dataset.fDataFiles[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hdataphihModifiedS[s][v]->SetName(Form("hdataMS_%i_%i", s, v));

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
	hgenphihS[s][v] = (TH1F*) dataset.fMCFiles[s][v]->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", 
								   hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hgenphihS[s][v]->SetName(Form("hgenphihS_%i_%i", s, v));
	//	cout << MESSAGE << "Entries (generated) = " << hgenphihS[s][v]->GetEntries() << endl; 

	hrecphihS[s][v] = (TH1F*) dataset.fMCFiles[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
	hrecphihS[s][v]->SetName(Form("hrecphihS_%i_%i", s, v));
	//	cout << MESSAGE << "Entries (reconstructed) = " << hrecphihS[s][v]->GetEntries() << endl; 

	// Might want to set the errors directly using manual loop over bins.  Sometimes this division does weird things 
	// even when Sumw2 is called first.  Perhaps it doesn't matter anymore. 
	haccphihS[s][v] = new TH1F(Form("haccphihS_z%iPT2%i_%i_%i", zBin, PT2Bin, s, v), Form("haccphihS_z%iPT2%i", zBin, PT2Bin), constants::n_phi_bins, -180, 180);
	haccphihS[s][v]->Sumw2();
	haccphihS[s][v]->Divide(hrecphihS[s][v], hgenphihS[s][v]);
	//	cout << MESSAGE << "Entries (acceptance) = " << haccphihS[s][v]->GetEntries() << endl; 

	//------------------ haprad: --------------------------------
	hsigS[s][v] = new TH1F(Form("hsigS_%i_%i", s, v), Form("hsigS_%i_%i", s, v), constants::n_phi_bins, -180, 180);
	hsigS[s][v]->Sumw2();
	hsibS[s][v] = new TH1F(Form("hsibS_%i_%i", s, v), Form("hsibS_%i_%i", s, v), constants::n_phi_bins, -180, 180);
	hsibS[s][v]->Sumw2();
	hRCS[s][v] = new TH1F(Form("hRCS_%i_%i", s, v), Form("hRCS_%i_%i", s, v), constants::n_phi_bins, -180, 180);
	hRCS[s][v]->Sumw2();

	// This small block loads the radiative correction histograms 
	// from the haprad table provided in the input. 
	for (int i = 0; i < constants::n_phi_bins; i++) {
	  HapradDataEntry entry = hapradTable.querySafe(xBin, QQBin, zBin, PT2Bin, i); 

	  if (hadronType == "pim"){
	    entry.sig -= entry.tail; 
	    entry.tail = 0; 
	  }

	  hsigS[s][v]->SetBinContent(i + 1, entry.sig);
	  hsigS[s][v]->SetBinError(i + 1, 0);
	  hsibS[s][v]->SetBinContent(i + 1, entry.sib);
	  hsibS[s][v]->SetBinError(i + 1, 0);
	}

	hRCS[s][v]->Divide(hsigS[s][v], hsibS[s][v]);

	// Apply acceptance correction. 
	hcorrS[s][v] = new TH1F(Form("hcorrS_%i_%i", s, v), Form("hcorrS_%i_%i", s, v), constants::n_phi_bins, -180, 180);
	hcorrS[s][v]->Sumw2();
	hcorrS[s][v]->Divide(hdataphihModifiedS[s][v], haccphihS[s][v]);


	// Apply radiative correction. 
	if(hsibS[s][v]->Integral() < constants::tiny || hsigS[s][v]->Integral() < constants::tiny){
	  category = -2;
	}

	hcorrRCS[s][v] = new TH1F(Form("hcorrRCS_%i_%i", s, v), Form("hcorrRCS_%i_%i", s, v), constants::n_phi_bins, -180, 180);
	hcorrRCS[s][v]->Sumw2();
	hcorrRCS[s][v]->Divide(hcorrS[s][v], hRCS[s][v]);
	hcorrRCS[s][v]->GetYaxis()->SetRangeUser(0, 1.1*hcorrRCS[s][v]->GetMaximum());
	hcorrRCS[s][v]->GetXaxis()->SetTitle("phi_h (deg.)");
	hcorrRCS[s][v]->SetTitle(SOURCE_NAME[s].c_str());

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

  for (int sourceIndex = 0; sourceIndex < constants::n_sources; sourceIndex++){
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

  if(SAVE_ROOT_FILE){
    rootFile->Write();
  }
} 
