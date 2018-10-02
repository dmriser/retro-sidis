#include "dataset.cpp"
#include "processOneBinSystematics.cpp"

void variations(){
  std::string baseDirectory("/volatile/clas12/dmriser/farm_out");
  std::string projectName("sidis_batch_11"); 

  // Load the entire dataset. 
  Dataset dataset = loadDataset(baseDirectory, projectName); 

  // Ensure that all files opened correctly. 
  if (verifyDataset(dataset)){
    std::cout << "The dataset is opened correctly." << std::endl; 

    for (int i = 0; i < constants::n_x_bins; i++){
      for (int j = 0; j < constants::n_q2_bins; j++){
	if ( i != 4 && j != 1 ){
	  processOneBinSystematics(dataset, i, j, 0, 0, "pip"); 
	  processOneBinSystematics(dataset, i, j, 0, 0, "pim"); 
	}
      }
    }
  } else {
    std::cout << "The dataset is not opened correctly." << std::endl; 
  }
}
