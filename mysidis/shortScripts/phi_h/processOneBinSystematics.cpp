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

*/



// Nathan's Comment
// 
// same as v1 but saving to root file w/ more details instead of saving only final values to txt file
// uses "update" instead of "recreate" in TFile to add histos one by one... 
// loop over all bins must be done externally
// be careful because this can cause duplicate copies of the same histogram in the same root file
// safest thing is to delete file and start over if any uncertainty occurs
//
// this is a long script... search "Checkpoint" (w/ lowercase C) for key locations
// a lot of things are hard-coded, be very careful if you change anything
// this works for BiSc5

#include <iostream>
#include <map>

string createFilename(string baseDirectory, string projectName, 
		      string dataType, int variation, bool isTight){

  string tightName = Form("%s/%s/%s/variation_%d/tight.root",
			  baseDirectory.c_str(), projectName.c_str(),
			  dataType.c_str(), variation); 
  string looseName = Form("%s/%s/%s/variation_%d/loose.root",
			  baseDirectory.c_str(), projectName.c_str(),
			  dataType.c_str(), variation); 
  return (isTight == true ? tightName : looseName); 
}

//void systematics_v2(int xBin = 0, int QQBin = 0, int zBin = 5, int PT2Bin = 5, string pipORpim = "pip")
//void systematics_v2(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 3, string pipORpim = "pip")
void processOneBinSystematics(int xBin = 0, int QQBin = 0, int zBin = 3, 
			      int PT2Bin = 0, string pipORpim = "pip"){
  gStyle->SetOptStat(0);
  
  bool doSaveRoot = 1;
  TFile *rootFile;
  if(doSaveRoot){
      rootFile = new TFile("Systematics_v2.root", "update");
    }

  const int NphihBins           = 36;
  const int Nsources            = 13;
  const int variationsPerSource = 2;

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

  const string message("[ProcessOneBinSystematics] "); 
  const string baseDirectory("/volatile/clas12/dmriser/farm_out"); 
  const string projectName("sidis_batch_11"); 
  const string pathToRequiredFiles("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles"); 

  string sourceName[Nsources] = {
    "e_zvert", "e_ECsamp", "e_ECoVi", "e_ECgeo", "e_CCthMatch", 
    "e_R1fid", "e_R3fid", "e_CCfid", "pi_vvp", "pi_R1fid", 
    "phih_fid", "accModel", "hapModel"
  };

  float M_sysErrorPiece[Nsources]; 
  float Ac_sysErrorPiece[Nsources];
  float Acc_sysErrorPiece[Nsources];

  float M_sumOfSquaredErrors   = 0;
  float Ac_sumOfSquaredErrors  = 0;
  float Acc_sumOfSquaredErrors = 0;

  float M_sysErrorTotal;
  float Ac_sysErrorTotal;
  float Acc_sysErrorTotal;

  string MString   = Form("hM_sysEcontributions_%s_%i_%i_%i_%i", 
			  pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);
  string AcString  = Form("hAc_sysEcontributions_%s_%i_%i_%i_%i", 
			  pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);
  string AccString = Form("hAcc_sysEcontributions_%s_%i_%i_%i_%i", 
			  pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);
  string catString = Form("hCategory_%s_%i_%i_%i_%i", 
			  pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);
  TH1F *hM_sysEcontributions   = new TH1F(MString.c_str(), MString.c_str(), 
					  Nsources, 0, Nsources);
  TH1F *hAc_sysEcontributions  = new TH1F(AcString.c_str(), AcString.c_str(), 
					  Nsources, 0, Nsources);
  TH1F *hAcc_sysEcontributions = new TH1F(AccString.c_str(), AccString.c_str(), 
					  Nsources, 0, Nsources);
  TH1F *hCategory              = new TH1F(catString.c_str(), catString.c_str(), 
					  1, 0, 1);

  

  TFile *tfdata[Nsources][variationsPerSource];

  // 
  // This part needs to be done carefully, and I believe
  // that I have done so.  The variation number changes at
  // times by more than one, and the array index is just 
  // incremented by one.  This is the way that it was done 
  // by Nathan and I don't want to change that. 
  // 
  // Code Incoming to replace megablock. 
  for (int i = 0; i < 10; i++){
    tfdata[i][0] = new TFile(createFilename(baseDirectory, projectName, 
					    "data", arrayIndexToVariation[i], false).c_str()); 
    tfdata[i][1] = new TFile(createFilename(baseDirectory, projectName, 
					    "data", arrayIndexToVariation[i],  true).c_str()); 

    cout << message << "File " << i << " for data (loose) is " << (tfdata[i][0]->IsOpen() ? "open." : "not open.") << endl; 
    cout << message << "File " << i << " for data (tight) is " << (tfdata[i][1]->IsOpen() ? "open." : "not open.") << endl; 
  }

  // Is it safe to have 6 pointers to an open file?  How many times is the file open? 
  // [10] is for testing phih cuts (bins with a phih cut only)... don't change the number!
  // for acc. model dependence. See note below (at tfmc[11][0])!!
  // for hap. model dependence. See note below (at tfmc[12][0])!!

  string nominalFilenameData = Form("%s/%s/data/variation_nominal/merged.root", 
				baseDirectory.c_str(), projectName.c_str());
  tfdata[10][0] = new TFile(nominalFilenameData.c_str());
  tfdata[10][1] = new TFile(nominalFilenameData.c_str());
  tfdata[11][0] = new TFile(nominalFilenameData.c_str());
  tfdata[11][1] = new TFile(nominalFilenameData.c_str());
  tfdata[12][0] = new TFile(nominalFilenameData.c_str());
  tfdata[12][1] = new TFile(nominalFilenameData.c_str());

  TFile *tfmc[Nsources][variationsPerSource];
  for (int i = 0; i < 10; i++){
    tfmc[i][0] = new TFile(createFilename(baseDirectory, projectName, 
					  "mc", arrayIndexToVariation[i], false).c_str()); 
    tfmc[i][1] = new TFile(createFilename(baseDirectory, projectName, 
					  "mc", arrayIndexToVariation[i],  true).c_str()); 

    cout << message << "File " << i << " for MC (loose) is " << (tfmc[i][0]->IsOpen() ? "open." : "not open.") << endl; 
    cout << message << "File " << i << " for MC (tight) is " << (tfmc[i][1]->IsOpen() ? "open." : "not open.") << endl; 
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
				  baseDirectory.c_str(), projectName.c_str());
  tfmc[10][0] = new TFile(nominalFilenameMC.c_str()); 
  tfmc[10][1] = new TFile(nominalFilenameMC.c_str()); 
  tfmc[11][0] = new TFile(nominalFilenameMC.c_str()); 
  tfmc[11][1] = new TFile(nominalFilenameMC.c_str()); 
  tfmc[12][0] = new TFile(nominalFilenameMC.c_str()); 
  tfmc[12][1] = new TFile(nominalFilenameMC.c_str()); 

  TLatex *tlat = new TLatex();
  tlat->SetNDC();
  tlat->SetTextSize(0.06);

  // Read into memory the category file for this bin. 
  //
  // If the file does not exist, then the bin passes.  There are 
  // more cuts (according to Nathan) that come later to take care
  // of these cases. 
  int category;
  ifstream categoryFile;
  string categoryFilename = Form("%s/binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", 
				 pathToRequiredFiles.c_str(), pipORpim.c_str(), 
				 xBin, QQBin, zBin, PT2Bin); 
  categoryFile.open(categoryFilename.c_str());

  if(categoryFile){ 
    categoryFile >> category; 
    categoryFile.close();
    cout << message << "Found category file." << endl; 
  } else { 
    category = 0; 
    cout << message << "Did not find category file: " << categoryFilename << endl; 
  } 


  //-----------------------------------------------------------
  //--------------- do the nominal case -----------------------
  //-----------------------------------------------------------
  TCanvas *nomcan  = new TCanvas();
  TFile *tfdataNom = new TFile(nominalFilenameData.c_str());
  TFile *tfmcNom   = new TFile(nominalFilenameMC.c_str());

  
  // Simple safety checks on the file. 
  if (tfdataNom && tfdataNom->IsOpen()){
    cout << message << "Opening sucessfully: " << nominalFilenameData << endl; 
  } else {
    cerr << message << "Dying on not being able to open " << nominalFilenameData << endl; 
  }

  
  if (tfmcNom && tfmcNom->IsOpen()){
    cout << message << "Opening sucessfully: " << nominalFilenameMC << endl; 
  } else {
    cerr << message << "Dying on not being able to open " << nominalFilenameMC << endl; 
  }

  TH1F *hdataphihModified = (TH1F*) tfdataNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

  // apply some modifications and further define the bin category:
  int NemptyPhihBins = 0;
  for(int phih = 0; phih < NphihBins; phih++){
    if(hdataphihModified->GetBinContent(phih+1) < 10){
      hdataphihModified->SetBinContent(phih+1, 0);
      hdataphihModified->SetBinError(phih+1, 0);
    }
    if(category > 0.5 && fabs(hdataphihModified->GetXaxis()->GetBinCenter(phih+1)) < category){
      hdataphihModified->SetBinContent(phih+1, 0);
      hdataphihModified->SetBinError(phih+1, 0);
    }
    if(hdataphihModified->GetBinContent(phih+1) < 0.1){ 
      NemptyPhihBins++;
    }
  }

  // update/redefine category here:
  // bad statistics and bad coverage, don't use this bin
  // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, Ac, and Acc for this bin
  // Measure only M for all other bins
  // note: category is further updated at the top of the corrRC section
  
  // Category codes below (as far as I could workout from the comments above). 
  //     -99 : Bad statistics and bad coverage, DO NOT use this bin.
  //       1 : Measure the values for this bin. 
  //     -13 : Measure only the multiplcity M for this bin.  
  if(hdataphihModified->Integral() < 180 && NemptyPhihBins >= 26){
    category = -99; 
  }
  else if(category != -1 && hdataphihModified->Integral() >= 360 && NemptyPhihBins <= 6){ 
    category = 1; 
  }
  else{
    category = -13; 
  }

  
  //--------------------- MC: ---------------------------------
  TH1F *hgenphih, *hrecphih, *haccphih;

  hgenphih = (TH1F*) tfmcNom->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
  hrecphih = (TH1F*) tfmcNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
  haccphih = new TH1F(Form("haccphih_z%iPT2%i", zBin, PT2Bin), Form("haccphih_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
  haccphih->Sumw2();
  haccphih->Divide(hrecphih, hgenphih);

  //------------------ haprad: --------------------------------

  TH1F *hsig = new TH1F("hsig", "hsig", NphihBins, -180, 180);
  hsig->Sumw2();
  TH1F *hsib = new TH1F("hsib", "hsib", NphihBins, -180, 180);
  hsib->Sumw2();
  TH1F *hRC = new TH1F("hRC", "hRC", NphihBins, -180, 180);
  hRC->Sumw2();

  string happath;
  if(pipORpim == "pip"){
    happath = Form("%s/haprad/hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", 
		   pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin); 
  }
  else if(pipORpim == "pim"){ 
    happath = Form("%s/haprad/hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat", 
		   pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin); 
  }
  ifstream hapfile(happath.c_str());

  if(hapfile){
    for(int phih = 0; phih < NphihBins; phih++){
      float sig, sib, tail;
      hapfile >> sig >> sib >> tail;
      if(pipORpim == "pim"){
	sig = sig - tail;
	tail = 0;
      }
      hsig->SetBinContent(phih+1, sig);
      hsig->SetBinError(phih+1, 0);
      hsib->SetBinContent(phih+1, sib);
      hsib->SetBinError(phih+1, 0);
    }
    hapfile.close();
    cout << message << "Found haprad file." << endl; 
  } else {
    cout << message << "Didn't find haprad file." << happath << endl; 
  }


  hRC->Divide(hsig, hsib);

  //------------------- corr: ---------------------------------

  TH1F *hcorr = new TH1F("hcorr", "hcorr", NphihBins, -180, 180);
  hcorr->Sumw2();
  hcorr->Divide(hdataphihModified, haccphih);

  //------------------- corrRC: -------------------------------
  // defining category = -2 here means no haprad results
  if(hsib->Integral() < 0.00000001 || hsig->Integral() < 0.00000001){ 
    category = -2; 
  }

  TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", NphihBins, -180, 180);
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

  int finalNomCategory = category;
  //-----------------------------------------------------------
  //----------------- do the variations ----------------------- checkpoint
  //-----------------------------------------------------------

  TCanvas *can = new TCanvas("can", "can", 5, 5, 1600, 1000);
  can->Divide(6, 4, 0.00001, 0.00001); // may need manual update

  TH1F *hdataphihModifiedS[Nsources][variationsPerSource]; // S for Systematics
  TH1F *hgenphihS[Nsources][variationsPerSource], *hrecphihS[Nsources][variationsPerSource], *haccphihS[Nsources][variationsPerSource];
  TH1F *hsigS[Nsources][variationsPerSource], *hsibS[Nsources][variationsPerSource], *hRCS[Nsources][variationsPerSource];
  TH1F *hcorrS[Nsources][variationsPerSource];
  TH1F *hcorrRCS[Nsources][variationsPerSource];
  TF1 *ffcorrRCS[Nsources][variationsPerSource];

  int count = 0;
  for(int s = 0; s < Nsources; s++) {
    for(int v = 0; v < variationsPerSource; v++) {
      if(!((s == 11 && v == 1) || (s == 12 && v == 1))) // part of the two work-arounds mentioned above
	{
	  count++;
	  can->cd(count);

	  //----------- read in the initial bin category --------------

	  categoryFile.open(Form("%s/binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", 
				 pathToRequiredFiles.c_str(), pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
	  if(categoryFile){ 
	    categoryFile >> category; 
	    categoryFile.close();
	  } else{ 
	    category = 0; 
	  }


	  if(category > 0.5 && s == 10 && v == 0) category = category - 10;
	  if(category > 0.5 && s == 10 && v == 1) category = category + 10;

	  //-------------------- data: --------------------------------

	  hdataphihModifiedS[s][v] = (TH1F*) tfdata[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
	  hdataphihModifiedS[s][v]->SetName(Form("hdataMS_%i_%i", s, v));

	  // apply some modifications and further define the bin category:
	  NemptyPhihBins = 0;
	  for(int phih = 0; phih < NphihBins; phih++){
	    if(hdataphihModifiedS[s][v]->GetBinContent(phih+1) < 10){
	      hdataphihModifiedS[s][v]->SetBinContent(phih+1, 0);
	      hdataphihModifiedS[s][v]->SetBinError(phih+1, 0);
	    }
	    if(category > 0.5 && fabs(hdataphihModifiedS[s][v]->GetXaxis()->GetBinCenter(phih+1)) < category){
	      hdataphihModifiedS[s][v]->SetBinContent(phih+1, 0);
	      hdataphihModifiedS[s][v]->SetBinError(phih+1, 0);
	    } 
	    if(hdataphihModifiedS[s][v]->GetBinContent(phih+1) < 0.1){
	      NemptyPhihBins++; 
	    }
	  }
	  
	  // update/redefine category here:
	  if(hdataphihModifiedS[s][v]->Integral() < 180 && NemptyPhihBins >= 26){ 
	    category = -99; 
	  } else if(category != -1 && hdataphihModifiedS[s][v]->Integral() >= 360 && NemptyPhihBins <= 6){ 
	    category = 1; 
	  } else { 
	    category = -13; 
	  }

	  //--------------------- MC: ---------------------------------
	  hgenphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
	  hgenphihS[s][v]->SetName(Form("hgenphihS_%i_%i", s, v));
	  hrecphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
	  hrecphihS[s][v]->SetName(Form("hrecphihS_%i_%i", s, v));
	  haccphihS[s][v] = new TH1F(Form("haccphihS_z%iPT2%i_%i_%i", zBin, PT2Bin, s, v), Form("haccphihS_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
	  haccphihS[s][v]->Sumw2();
	  haccphihS[s][v]->Divide(hrecphihS[s][v], hgenphihS[s][v]);

	  //------------------ haprad: --------------------------------
	  hsigS[s][v] = new TH1F(Form("hsigS_%i_%i", s, v), Form("hsigS_%i_%i", s, v), NphihBins, -180, 180);
	  hsigS[s][v]->Sumw2();
	  hsibS[s][v] = new TH1F(Form("hsibS_%i_%i", s, v), Form("hsibS_%i_%i", s, v), NphihBins, -180, 180);
	  hsibS[s][v]->Sumw2();
	  hRCS[s][v] = new TH1F(Form("hRCS_%i_%i", s, v), Form("hRCS_%i_%i", s, v), NphihBins, -180, 180);
	  hRCS[s][v]->Sumw2();

	  string happathS;
	  if(s == 12 && v == 0){
	      happathS = Form("%s/hapradResults/hapDefault/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", 
			      pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin); 
	    } else {
	    if(pipORpim == "pip"){
	      happathS = Form("%s/hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", 
			      pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin);
	    } else if(pipORpim == "pim"){
	      happathS = Form("%s/hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat", 
			      pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin);
	    }
	  }
	  ifstream hapfileS(happathS.c_str());

	  if(hapfileS){
	    for(int phih = 0; phih < NphihBins; phih++){
	      float sig, sib, tail;
	      hapfileS >> sig >> sib >> tail;
	      if(pipORpim == "pim"){
		sig = sig - tail;
		tail = 0;
	      }
	      hsigS[s][v]->SetBinContent(phih+1, sig);
	      hsigS[s][v]->SetBinError(phih+1, 0);
	      hsibS[s][v]->SetBinContent(phih+1, sib);
	      hsibS[s][v]->SetBinError(phih+1, 0);
	    }
	  }
	  hapfileS.close();

	  hRCS[s][v]->Divide(hsigS[s][v], hsibS[s][v]);

	  //------------------- corr: ---------------------------------

	  hcorrS[s][v] = new TH1F(Form("hcorrS_%i_%i", s, v), Form("hcorrS_%i_%i", s, v), NphihBins, -180, 180);
	  hcorrS[s][v]->Sumw2();
	  hcorrS[s][v]->Divide(hdataphihModifiedS[s][v], haccphihS[s][v]);
	  hcorrS[s][v]->Draw();

	  //------------------- corrRC: -------------------------------

	  if(hsibS[s][v]->Integral() < 0.00000001 || hsigS[s][v]->Integral() < 0.00000001){
	    category = -2;
	  }

	  hcorrRCS[s][v] = new TH1F(Form("hcorrRCS_%i_%i", s, v), Form("hcorrRCS_%i_%i", s, v), NphihBins, -180, 180);
	  hcorrRCS[s][v]->Sumw2();
	  hcorrRCS[s][v]->Divide(hcorrS[s][v], hRCS[s][v]);
	  hcorrRCS[s][v]->GetYaxis()->SetRangeUser(0, 1.1*hcorrRCS[s][v]->GetMaximum());
	  hcorrRCS[s][v]->GetXaxis()->SetTitle("phi_h (deg.)");
	  hcorrRCS[s][v]->SetTitle(sourceName[s].c_str());
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
	    hcorrRCS[s][v]->Fit(Form("ffcorrRCS_%i_%i", s, v), "q", "", -180, 180);
	  }

	  tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRCS[s][v]->GetParameter(0), ffcorrRCS[s][v]->GetParError(0)));
	  tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(1), ffcorrRCS[s][v]->GetParError(1)));
	  tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(2), ffcorrRCS[s][v]->GetParError(2)));

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

    cout << s << " " << sourceName[s] << " " << M_sysErrorPiece[s] << " " 
	 << Ac_sysErrorPiece[s] << " " << Acc_sysErrorPiece[s] << endl;

    hM_sysEcontributions  ->SetBinContent(s+1, M_sysErrorPiece[s]);
    hAc_sysEcontributions ->SetBinContent(s+1, Ac_sysErrorPiece[s]);
    hAcc_sysEcontributions->SetBinContent(s+1, Acc_sysErrorPiece[s]);
 
    M_sumOfSquaredErrors   = M_sumOfSquaredErrors + M_sysErrorPiece[s]*M_sysErrorPiece[s];
    Ac_sumOfSquaredErrors  = Ac_sumOfSquaredErrors + Ac_sysErrorPiece[s]*Ac_sysErrorPiece[s];
    Acc_sumOfSquaredErrors = Acc_sumOfSquaredErrors + Acc_sysErrorPiece[s]*Acc_sysErrorPiece[s];

  } 

  hCategory->SetBinContent(1, finalNomCategory);
  M_sysErrorTotal   = sqrt(M_sumOfSquaredErrors);
  Ac_sysErrorTotal  = sqrt(Ac_sumOfSquaredErrors);
  Acc_sysErrorTotal = sqrt(Acc_sumOfSquaredErrors);

  cout << endl << M_sysErrorTotal << " " << Ac_sysErrorTotal << " " << Acc_sysErrorTotal<<endl;

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

  if(doSaveRoot){
    rootFile->Write();
  }
} 
