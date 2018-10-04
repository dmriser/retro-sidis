#include "constants.cpp"
#include "utils.cpp"

void test_global_index(){

  std::vector<int> axisSizes; 
  std::vector<int> binIndex; 

  axisSizes.push_back(constants::n_x_bins); 
  axisSizes.push_back(constants::n_q2_bins); 
  axisSizes.push_back(constants::n_z_bins); 
  axisSizes.push_back(constants::n_pt2_bins); 

  int trueIndex = 0; 
  int index = 0; 
  int pass = 0; 
  int fail = 0; 
  for (int i = 0; i < constants::n_pt2_bins; i++){
    for (int j = 0; j < constants::n_z_bins; j++){
      for (int k = 0; k < constants::n_q2_bins; k++){
	for (int m = 0; m < constants::n_x_bins; m++){
 	  
	  binIndex.clear(); 
	  binIndex.push_back(m); 
	  binIndex.push_back(k); 
	  binIndex.push_back(j); 
	  binIndex.push_back(i); 
	  index = Utils::BinEncoder::encodeBinCoordinates(binIndex, axisSizes); 

	  if (index == trueIndex){
	    pass++; 
	  } else {
	    fail++; 
	  }

	  trueIndex++; 
	}
      }
    }
  }

  std::cout << "Passed " << pass << ", Failed " << fail << std::endl; 
}
