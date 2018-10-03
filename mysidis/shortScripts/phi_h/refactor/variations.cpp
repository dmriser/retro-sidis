#include "dataset.cpp"
#include "processOneBinSystematics.cpp"
#include "tables.cpp"

void variations(){
  std::string baseDirectory("/volatile/clas12/dmriser/farm_out");
  std::string projectName("sidis_batch_11"); 

  // Load the entire dataset. 
  Dataset dataset = loadDataset(baseDirectory, projectName); 

  // Load bin category tables and haprad tables. 
  BinCategoryTable pipBinCategoryTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories", "pip");
  BinCategoryTable pimBinCategoryTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories", "pim");

  HapradTable pipHapradTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPipModel", "pip");
  HapradTable pimHapradTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPimModel", "pim");

  // Ensure that all files opened correctly. 
  if (verifyDataset(dataset)){
    std::cout << "The dataset is opened correctly." << std::endl; 

    processOneBinSystematics(dataset, pipBinCategoryTable, pipHapradTable, 0, 0, 3, 4, "pip"); 
    processOneBinSystematics(dataset, pimBinCategoryTable, pimHapradTable, 0, 0, 3, 4, "pim"); 

  } else {
    std::cout << "The dataset is not opened correctly." << std::endl; 
  }
}
