#include "dataset.cpp"
#include "processOneBinSystematics.cpp"
#include "tables.cpp"

/*

  File:   variations.cpp 
  Author: David Riser 
  Date:   October 3, 2018 
  
  This file replaces processOneBinSystematics as the 
  main workhorse for calculating systematic uncertainties.

*/

void variations(int startIndex = 0, int stopIndex = 4){
  std::string baseDirectory("/volatile/clas12/dmriser/farm_out");
  std::string projectName("sidis_batch_11"); 

  // Load the entire dataset. 
  std::cout << "Loading data... "; 
  Dataset dataset = loadDataset(baseDirectory, projectName); 
  std::cout << "done!" << std::endl; 

  // Load bin category tables and haprad tables. 
  std::cout << "Loading bin categories... ";
  BinCategoryTable pipBinCategoryTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories", "pip");
  BinCategoryTable pimBinCategoryTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories", "pim");
  std::cout << "done!" << std::endl; 

  std::cout << "Loading haprad tables... "; 
  HapradTable pipHapradTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPipModel", "pip");
  HapradTable pimHapradTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPimModel", "pim");
  std::cout << "done!" << std::endl; 
  
  if (verifyDataset(dataset)){
    std::cout << "The dataset is opened correctly." << std::endl; 

    FourDimensionalResultsTable fourResults(".", "pip"); 
    FiveDimensionalResultsTable fiveResults(".", "pip");

    int index = 0; 
    for(int i = 0; i < constants::n_x_bins; i++){
      for(int j = 0; j < constants::n_q2_bins; j++){
	for(int k = 0; k < constants::n_z_bins; k++){
	  for(int m = 0; m < constants::n_pt2_bins; m++){

	    // A simple way to divide jobs up. 
	    if (index >= startIndex && index < stopIndex){
	      std::cout << "\rProcessing (" << index-startIndex << "/" << stopIndex-startIndex << ")" << std::flush; 

	      std::pair<FourDResult, std::vector<FiveDResult> > nominalResult = calculateResult("nominal", 
												dataset.fDataNominalFile, 
												dataset.fMCNominalFile, 
												pipBinCategoryTable,
												pipHapradTable,
												i, j, k, m, "pip");

	      for (int sourceIndex = 0; sourceIndex < constants::n_sources; sourceIndex++){

		std::string looseName(Form("source%d_var0", sourceIndex));
		std::pair<FourDResult, std::vector<FiveDResult> > looseResults = calculateResult(looseName,
												 dataset.fDataFiles[sourceIndex][0],
												 dataset.fMCFiles[sourceIndex][0],
												 pipBinCategoryTable,
												 pipHapradTable,
												 i, j, k, m, "pip");
		std::string tightName(Form("source%d_var1", sourceIndex));
		std::pair<FourDResult, std::vector<FiveDResult> > tightResults = calculateResult(tightName,
												 dataset.fDataFiles[sourceIndex][1],
												 dataset.fMCFiles[sourceIndex][1],
												 pipBinCategoryTable,
												 pipHapradTable,
												 i, j, k, m, "pip");
		
		nominalResult.first.m_sys_err[sourceIndex] = Utils::combineVariations(nominalResult.first.m, 
										      looseResults.first.m, 
										      tightResults.first.m); 
		nominalResult.first.ac_sys_err[sourceIndex] = Utils::combineVariations(nominalResult.first.ac, 
										       looseResults.first.ac, 
										       tightResults.first.ac); 
		nominalResult.first.acc_sys_err[sourceIndex] = Utils::combineVariations(nominalResult.first.acc, 
											looseResults.first.acc, 
											tightResults.first.acc); 
	      }
	      
	      fourResults.insert(nominalResult.first, i, j, k, m); 										  
	      
	      for (int phi_bin = 0; phi_bin < constants::n_phi_bins; phi_bin++){
		fiveResults.insert(nominalResult.second.at(phi_bin), i, j, k, m, phi_bin); 
	      }

	    }

	    index++; 
	  }
	}
      }
    }

    fourResults.write(); 
    fiveResults.write(); 

  } else {
    std::cout << "The dataset is not opened correctly." << std::endl; 
  }
}
