#include "dataset.cpp"
#include "encoder.cpp"
#include "processOneBinSystematics.cpp"
#include "tables.cpp"

/*

  File:   variations.cpp 
  Author: David Riser 
  Date:   October 3, 2018 
  
  This file replaces processOneBinSystematics as the 
  main workhorse for calculating systematic uncertainties.

  Please Do: 
  1. Check that the variations are done correctly, perhaps one needs 
     to pass a different haprad table into one of the variations. 
  2. Run a complete run with Nathan's good run list.
  3. Compare in an automated way the results produced by myself and Nathan. 
  4. Add support for pi- and pi+ with some flag. 

*/

void variations(int startIndex = 0, int stopIndex = 4){

  const std::string baseDirectory("/volatile/clas12/dmriser/farm_out");
  const std::string projectName("sidis_with_nathans_files"); 
  const std::string hadronType("pip");

  // Load the entire dataset, these are the output room files 
  // from all the different variations. 
  std::cout << "Loading data... "; 
  Dataset dataset = loadDataset(baseDirectory, projectName); 
  std::cout << "done!" << std::endl; 

  // Load bin category tables and haprad tables. 
  std::cout << "Loading bin categories... ";
  BinCategoryTable binCategoryTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories", hadronType);
  std::cout << "done!" << std::endl; 

  std::cout << "Loading haprad tables... "; 
  std::string hapradPath = (hadronType == "pip" ? "/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPipModel"
			    : "/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPimModel");
  HapradTable hapradTable(hapradPath, hadronType);
  HapradTable hapradDefaultTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/hapDefault", hadronType);
  std::cout << "done!" << std::endl; 

  if (verifyDataset(dataset)){
    std::cout << "The dataset is opened correctly." << std::endl; 

    FourDimensionalResultsTable fourResults(".", hadronType); 
    FiveDimensionalResultsTable fiveResults(".", hadronType);

    int index = 0; 
    for(int i = 0; i < constants::n_x_bins; i++){
      for(int j = 0; j < constants::n_q2_bins; j++){
	for(int k = 0; k < constants::n_z_bins; k++){
	  for(int m = 0; m < constants::n_pt2_bins; m++){

	    // A simple way to divide jobs up. 
	    if (index >= startIndex && index < stopIndex){
	      std::cout << "\rProcessing (" << index-startIndex << "/" << stopIndex-startIndex << ")" << std::flush; 

	      std::pair<FourDResult, CollectionOfFiveDResults> nominalResult = calculateResult("nominal", 
											       dataset.fDataNominalFile, 
											       dataset.fMCNominalFile, 
											       binCategoryTable,
											       hapradTable,
											       i, j, k, m, 
											       hadronType);

	      for (int sourceIndex = 0; sourceIndex < constants::n_sources; sourceIndex++){

		// For the 13th source we change the haprad table. 
		HapradTable hap = (sourceIndex == 12 ? hapradDefaultTable : hapradTable);

		std::string looseName(Form("source%d_var0", sourceIndex));
		std::pair<FourDResult, CollectionOfFiveDResults> looseResults = calculateResult(looseName,
												 dataset.fDataFiles[sourceIndex][0],
												 dataset.fMCFiles[sourceIndex][0],
												 binCategoryTable,
												 hap,
												 i, j, k, m, hadronType);
		std::string tightName(Form("source%d_var1", sourceIndex));
		std::pair<FourDResult, CollectionOfFiveDResults> tightResults = calculateResult(tightName,
												 dataset.fDataFiles[sourceIndex][1],
												 dataset.fMCFiles[sourceIndex][1],
												 binCategoryTable,
												 hapradTable,
												 i, j, k, m, hadronType);
		
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
		fiveResults.insert(nominalResult.second.get(phi_bin), i, j, k, m, phi_bin); 
	      }
	      
	    }

	    index++; 
	  }
	}
      }
    }
    
    std::cout << std::endl; 
    std::cout << "Writing tables... "; 
    fourResults.write(); 
    fiveResults.write(); 
    std::cout << "done!" << std::endl; 

  } else {
    std::cout << "The dataset is not opened correctly." << std::endl; 
  }

}
