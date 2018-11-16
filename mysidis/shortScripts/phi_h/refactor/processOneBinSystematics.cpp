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
#include "results.cpp"
#include "tables.cpp"
#include "utils.cpp"

std::pair<FourDResult, CollectionOfFiveDResults> calculateResult(std::string name, TFile *dataFile, TFile *mcFile,
						    BinCategoryTable & binCategoryTable,
						    HapradTable & hapradTable,
						    int xBin, int QQBin, int zBin, int PT2Bin, 
						    std::string hadronType){
  
  // Setup the category for this bin. 
  int category = binCategoryTable.querySafe(xBin, QQBin, zBin, PT2Bin);
  if (name == "source10_var0" && category > 0.5){
    category -= 10; 
  } else if (name == "source10_var1" && category > 0.5){
    category += 10; 
  }

  HistogramPack histos = createPack(name, hadronType, xBin, QQBin, zBin, PT2Bin); 
  histos.data = (TH1F*) dataFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));

  // There is some need for protection here, the 
  // code can die anytime if even one of these 
  // histograms won't open or doesn't exist. 

  Utils::removeBinsWithCountsLower(histos.data, 10); 
  Utils::removeBinsInCentralPhi(histos.data, category); 
  int numberOfEmptyPhiBins = Utils::countEmptyBins(histos.data); 

  // Category codes below (as far as I could workout from the comments above). 
  //     -99 : Bad statistics and bad coverage, DO NOT use this bin.
  //       1 : Measure the values for this bin. 
  //     -13 : Measure only the multiplcity M for this bin.  
  category = Utils::updateCategoryBasedOnEmptyBins(histos.data, 
						   numberOfEmptyPhiBins, 
						   category); 
  
  histos.gen = (TH1F*) mcFile->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", 
					hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  histos.rec = (TH1F*) mcFile->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", 
					hadronType.c_str(), xBin, QQBin, zBin, PT2Bin));
  histos.acc->Divide(histos.rec, histos.gen); 

  // This small block loads the radiative correction histograms 
  // from the haprad table provided in the input. 
  for (int i = 0; i < constants::n_phi_bins; i++) {
    HapradDataEntry entry = hapradTable.querySafe(xBin, QQBin, zBin, PT2Bin, i); 

    if (hadronType == "pim"){
      entry.sig -= entry.tail; 
      entry.tail = 0; 
    }

    histos.sig->SetBinContent(i + 1, entry.sig);
    histos.sig->SetBinError(i + 1, 0);
    histos.sib->SetBinContent(i + 1, entry.sib);
    histos.sib->SetBinError(i + 1, 0);
  }

  histos.rc->Divide(histos.sig, histos.sib);
  histos.corrAcc->Divide(histos.data, histos.acc); 
  histos.corrRC->Divide(histos.data, histos.rc);
  histos.corr->Divide(histos.corrAcc, histos.rc);

  if(histos.sib->Integral() < constants::tiny || histos.sig->Integral() < constants::tiny){ 
    category = -2; 
  }

  TF1 *ffcorrRC = new TF1("ffcorrRC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", 
			  constants::phi_min, constants::phi_max);
  ffcorrRC->SetLineColor(kBlue);
  ffcorrRC->SetParameters(histos.corr->GetMaximum(), 0.0, 0.0);

  if(category == 1){
    ffcorrRC->SetParLimits(1, -0.99, 0.99);
    ffcorrRC->SetParLimits(2, -0.99, 0.99);
  } else {
    ffcorrRC->SetParLimits(1, -0.30, 0.30);
    ffcorrRC->SetParLimits(2, -0.30, 0.30);
  }

  if(histos.corr->Integral() > 36){ 
    histos.corr->Fit("ffcorrRC", "q", "", constants::phi_min, constants::phi_max); 
  }

  FourDResult fourDResult; 
  fourDResult.m = ffcorrRC->GetParameter(0);
  fourDResult.m_err = ffcorrRC->GetParError(0); 
  fourDResult.ac = ffcorrRC->GetParameter(1);
  fourDResult.ac_err = ffcorrRC->GetParError(1); 
  fourDResult.acc = ffcorrRC->GetParameter(2);
  fourDResult.acc_err = ffcorrRC->GetParError(2); 
  fourDResult.chi2 = ffcorrRC->GetChisquare(); 
  fourDResult.bin_category = category; 

  CollectionOfFiveDResults fiveDResults; 
  for(int i = 1; i <= constants::n_phi_bins; i++){
    FiveDResult fiveDResult;
    fiveDResult.counts = histos.corr->GetBinContent(i);
    fiveDResult.stat_err = histos.corr->GetBinError(i);
    fiveDResult.acceptance = histos.acc->GetBinContent(i);
    fiveDResult.acceptance_err = histos.acc->GetBinError(i);
    fiveDResult.rad_corr = histos.rc->GetBinContent(i);
    fiveDResult.rad_corr_err = histos.rc->GetBinError(i);    
    fiveDResults.set(fiveDResult, i);
  }

  return std::make_pair(fourDResult, fiveDResults); 
}

void processOneBinSystematics(Dataset & dataset, 
			      BinCategoryTable & binCategoryTable, 
			      HapradTable & hapradTable, 
			      int xBin = 0, int QQBin = 0, int zBin = 3, 
			      int PT2Bin = 4, string hadronType = "pip"){

  const bool SAVE_ROOT_FILE = false;
  TFile *rootFile;
  if(SAVE_ROOT_FILE){
    rootFile = new TFile("Systematics_v2.root", "update");
  }
 
  const string MESSAGE("[ProcessOneBinSystematics] "); 
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
 

  // Process Nominal Configuration 
  std::pair<FourDResult, CollectionOfFiveDResults> nominalResults = calculateResult("nominal", dataset.fDataNominalFile, 
										     dataset.fMCNominalFile, binCategoryTable, 
										     hapradTable, xBin, QQBin, zBin, PT2Bin, hadronType); 

  std::cout << "Nominal Results: " << nominalResults.first.m << " +/- " << nominalResults.first.m_err << std::endl;

  // Setup for variations 
  std::pair<FourDResult, CollectionOfFiveDResults> variationResults[constants::n_sources][constants::n_variations_per_source]; 

  int finalNomCategory = nominalResults.first.bin_category; 
  for(int s = 0; s < constants::n_sources; s++) {
    for(int v = 0; v < constants::n_variations_per_source; v++) {
      if(!((s == 11 && v == 1) || (s == 12 && v == 1))) {

	variationResults[s][v] = calculateResult(Form("source%d_var%d", s, v),
						 dataset.fDataFiles[s][v],
						 dataset.fMCFiles[s][v],
						 binCategoryTable,
						 hapradTable,
						 xBin, QQBin, zBin, PT2Bin,
						 hadronType);      
      }
    }
    
    // This is part of the work-around Nathan mentions above. 
    if (s < 10){
      M_sysErrorPiece[s] = sqrt( pow(variationResults[s][0].first.m - nominalResults.first.m, 2) + 
				 pow(variationResults[s][1].first.m - nominalResults.first.m, 2) ) / sqrt(2.0);
      Ac_sysErrorPiece[s] = sqrt( pow(variationResults[s][0].first.ac - nominalResults.first.ac, 2) + 
				  pow(variationResults[s][1].first.ac - nominalResults.first.ac, 2) ) / sqrt(2.0);
      Acc_sysErrorPiece[s] = sqrt( pow(variationResults[s][0].first.acc - nominalResults.first.acc, 2) + 
				   pow(variationResults[s][1].first.acc - nominalResults.first.acc, 2) ) / sqrt(2.0);
    }
    
    else {
      M_sysErrorPiece[s] = fabs(variationResults[s][0].first.m - nominalResults.first.m); 
      Ac_sysErrorPiece[s] = fabs(variationResults[s][0].first.ac - nominalResults.first.ac); 
      Acc_sysErrorPiece[s] = fabs(variationResults[s][0].first.acc - nominalResults.first.acc); 
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

    // The pieces of the calculation. 
    //    writeHistogramPackToFile(rootFile, nominalResults.first); 

    // The important results. 
    hCategory->Write(); 
    hM_sysEcontributions->Write();
    hAc_sysEcontributions->Write();
    hAcc_sysEcontributions->Write();

    // Maybe not needed. 
    rootFile->Write();
  }
} 
