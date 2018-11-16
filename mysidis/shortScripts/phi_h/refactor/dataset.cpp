#ifndef DATASET_CPP
#define DATASET_CPP

/*
  
  File:   dataset.cpp
  Author: David Riser
  Date:   October 2, 2018

  This file describes the structure of the input data
  needed to perform the systematic uncertainty calculation.
  A dataset consists of two nominal files, where all cuts are 
  set to the standard values, one for data and one for MC. 
  The dataset additionally consists of 
  n_variations = (constants::n_sources * constants::n_variations_per_source) 
  additional files that are used to estimate the imact of each
  variation on the final result in each bin.  

*/

// This project 
#include "constants.cpp"
#include "utils.cpp"

struct Dataset {
  TFile *fDataNominalFile, *fMCNominalFile;
  TFile *fDataFiles[constants::n_sources][constants::n_variations_per_source];
  TFile *fMCFiles[constants::n_sources][constants::n_variations_per_source];
};

void loadNominal(Dataset & dataset, std::string baseDirectory,
		 std::string projectName){
  dataset.fDataNominalFile = new TFile(Form("%s/%s/data/variation_nominal/nominal.root",
					    baseDirectory.c_str(), projectName.c_str())); 
  dataset.fMCNominalFile = new TFile(Form("%s/%s/mc/variation_nominal/nominal.root",
					  baseDirectory.c_str(), projectName.c_str())); 
}

void loadVariation(Dataset & dataset, std::string baseDirectory, 
		   std::string projectName, std::string dataType, 
		   int index, int variationIndex){
  
  // Variation index is used as the boolean flag is tight, because 
  // this is the indexing.  0 = loose, 1 = tight.  It's not a perfect 
  // way to do it, but it will work. I don't forsee that constants::n_variations_per_source 
  // will ever change. 
  std::string filename = Utils::createFilename(baseDirectory, projectName,
					       dataType, 
					       Utils::convertIndexIntoVariation(index), 
					       variationIndex);
  if (dataType == "data"){
    dataset.fDataFiles[index][variationIndex] = new TFile(filename.c_str());
  } else if (dataType == "mc"){
    dataset.fMCFiles[index][variationIndex] = new TFile(filename.c_str());
  } else {
    std::cerr << "Unable to load data for " << filename << std::endl; 
    exit(0); 
  }
}

void loadVariations(Dataset & dataset, std::string baseDirectory,
                    std::string projectName){

  // Load the data
  for (int i = 0; i < constants::n_sources; i++){
    if (i < 10){
      loadVariation(dataset, baseDirectory, projectName, "data", i, 0); 
      loadVariation(dataset, baseDirectory, projectName, "data", i, 1); 
      loadVariation(dataset, baseDirectory, projectName,   "mc", i, 0); 
      loadVariation(dataset, baseDirectory, projectName,   "mc", i, 1); 
    } else {
      std::string filename = Form("%s/%s/data/variation_nominal/nominal.root",
                                             baseDirectory.c_str(), projectName.c_str());
      dataset.fDataFiles[i][0] = new TFile(filename.c_str());
      dataset.fDataFiles[i][1] = new TFile(filename.c_str());
 
      std::string nominalFilename = Form("%s/%s/mc/variation_nominal/nominal.root",
                                         baseDirectory.c_str(), projectName.c_str());
      
      std::string acceptanceFilename = Form("%s/%s/mc/variation_acceptance/acceptance.root",
                                            baseDirectory.c_str(), projectName.c_str());
      
      // Handle the acceptance variations separately.
      if (i == 11){
        dataset.fMCFiles[i][0] = new TFile(acceptanceFilename.c_str());
        dataset.fMCFiles[i][1] = new TFile(nominalFilename.c_str());
      } else {
        dataset.fMCFiles[i][0] = new TFile(nominalFilename.c_str());
        dataset.fMCFiles[i][1] = new TFile(nominalFilename.c_str());
      }
    }
  }
}

Dataset loadDataset(std::string baseDirectory, std::string projectName){
  Dataset dataset;
  loadNominal(dataset, baseDirectory, projectName);
  loadVariations(dataset, baseDirectory, projectName);
  return dataset;
}

bool verifyFile(TFile *file){
  bool status = true; 
  if (file){ 
    if (!file->IsOpen()){
      status = false; 
    }
  } else {
    status = false; 
  }

  return status; 
}

bool verifyDataset(Dataset & dataset){

  bool status = verifyFile(dataset.fDataNominalFile); 
  if( !verifyFile(dataset.fMCNominalFile) ){
    status = false; 
  }
  
  for (int index = 0; index < constants::n_sources; index++){
    if (!verifyFile(dataset.fDataFiles[index][0])){
      status = false; 
    }
    if (!verifyFile(dataset.fDataFiles[index][1])){
      status = false; 
    }
    if (!verifyFile(dataset.fMCFiles[index][0])){
      status = false; 
    }
    if (!verifyFile(dataset.fMCFiles[index][1])){
      status = false; 
    }
  }

  return status; 
}

#endif 
