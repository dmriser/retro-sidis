#include "dataset.cpp"
#include "processOneBinSystematics.cpp"
#include "tables.cpp"

void variations(){
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

  // Ensure that all files opened correctly. 
  if (verifyDataset(dataset)){
    std::cout << "The dataset is opened correctly." << std::endl; 

    for(int i = 0; i < constants::n_z_bins; i++){
      processOneBinSystematics(dataset, pipBinCategoryTable, pipHapradTable, 0, 0, i, 4, "pip"); 
      processOneBinSystematics(dataset, pimBinCategoryTable, pimHapradTable, 0, 0, i, 4, "pim"); 
    }

  } else {
    std::cout << "The dataset is not opened correctly." << std::endl; 
  }
}
