#ifndef UTILS_CPP 
#define UTILS_CPP 

// Standard c++ Includes 
#include <iostream>

// CERN Root Includes 
#include "TH1.h"

namespace Utils {

  void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
  }

  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }

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

  int getBinCategory(string categoryFilename, string message){
    // If the file does not exist, then the bin passes.  There are
    // more cuts (according to Nathan) that come later to take care
    // of these cases.

    // This should be some default value...
    int category;
    ifstream categoryFile;
    categoryFile.open(categoryFilename.c_str());

    if(categoryFile && categoryFile.is_open()){
      categoryFile >> category;
      categoryFile.close();
      cout << message << "Found category file." << endl;
    } else {
      category = 0;
      cout << message << "Did not find category file: " << categoryFilename << endl;
    }

    return category;
  }

  int updateCategoryBasedOnEmptyBins(TH1F *histo, int category, int numberOfEmptyBins){
    // This function updates the category of the current kinematic bin
    // based on the number of empty bins in the phi distribution.

    if(histo->Integral() < 180 && numberOfEmptyBins >= 26){
      category = -99;
    } else if(category != -1 && histo->Integral() >= 360 && numberOfEmptyBins <= 6){
      category = 1;
    } else{
      category = -13;
    }

    return category;
  }

  int countEmptyBins(TH1F *histo){
    int numberOfEmptyBins = 0;

    for(int bin = 0; bin < histo->GetNbinsX(); bin++){
      if(histo->GetBinContent(bin+1) < 0.1){
	numberOfEmptyBins++;
      }
    }

    return numberOfEmptyBins;
  }

  void removeBinsWithCountsLower(TH1F *histo, int limit){
    for(int bin = 0; bin < histo->GetNbinsX(); bin++){

      if(histo->GetBinContent(bin+1) < limit){
	histo->SetBinContent(bin+1, 0);
	histo->SetBinError(bin+1, 0);
      }
    }
  }

  void removeBinsInCentralPhi(TH1F *histo, int binIndex){
    for(int bin = 0; bin < histo->GetNbinsX(); bin++){
      if(binIndex > 0.5 && fabs(histo->GetXaxis()->GetBinCenter(bin+1)) < binIndex){
	histo->SetBinContent(bin + 1, 0);
	histo->SetBinError(bin + 1, 0);
      }
    }
  }

  std::string buildHapradPath(std::string pathToRequiredFiles, std::string hadronType,
			      int xBin, int QQBin, int zBin, int PT2Bin,
			      int sourceIndex, int variationIndex){

    std::string path;
    if(sourceIndex == 12 && variationIndex == 0){
      path = Form("%s/haprad/hapradResults/hapDefault/pip_BiSc5_x%iQQ%iz%iPT2%i.dat",
		  pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin);
    }
    else {
      if(hadronType == "pip"){
	path = Form("%s/haprad/hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat",
		    pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin);
      }
      else if(hadronType == "pim"){
	path = Form("%s/haprad/hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat",
		    pathToRequiredFiles.c_str(), xBin, QQBin, zBin, PT2Bin);
      }
    }

    return path;
  }

  void loadRadiativeCorrection(std::ifstream & hapfile,
			       TH1F *hsig, TH1F *hsib, TH1F *hRC,
			       std::string hadronType){
    /* This method calculates the radiative correction as a function of
   phi for one kinematic bin.  Before calling this function, the file
   should be opened and null checked. */

    for(int phih = 0; phih < hsig->GetNbinsX(); phih++){
      float sig, sib, tail;
      hapfile >> sig >> sib >> tail;

      if(hadronType == "pim"){
	sig = sig - tail;
	tail = 0;
      }

      hsig->SetBinContent(phih+1, sig);
      hsig->SetBinError(phih+1, 0);
      hsib->SetBinContent(phih+1, sib);
      hsib->SetBinError(phih+1, 0);
    }

    hRC->Divide(hsig, hsib);
  }

  int convertIndexIntoVariation(int index){
    int id = -1; 

    // Less code than a switch with explicit cases,
    // but mayber longer than a switch with default. 
    if (index <= 6){
      id = index; 
    } 
    else if(index == 7){
      id = 8; 
    }
    else if (index == 8){
      id = 10;
    }
    else if (index == 9){
      id = 11; 
    }

  return id; 
  }

}
#endif 
