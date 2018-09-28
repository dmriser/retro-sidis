#ifndef UTILS_CPP 
#define UTILS_CPP 

// Standard c++ Includes 
#include <iostream>

// CERN Root Includes 
#include "TH1.h"

namespace Utils {

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
    int category;
    ifstream categoryFile;
    categoryFile.open(categoryFilename.c_str());

    if(categoryFile){
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

}
#endif 
