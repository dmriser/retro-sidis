#include "../constants.cpp"
#include "../tables.cpp"

void print(BinCategoryTable & table){
  for (int i = 0; i < constants::n_x_bins; i++){
    for (int j = 0; j < constants::n_q2_bins; j++){
      for (int k = 0; k < constants::n_z_bins; k++){
	for (int m = 0; m < constants::n_pt2_bins; m++){
	  std::cout << std::setw(4) << i; 
	  std::cout << std::setw(4) << j; 
	  std::cout << std::setw(4) << k; 
	  std::cout << std::setw(4) << m; 
	  std::cout << std::setw(18) << std::scientific << table.querySafe(i,j,k,m) << std::endl; 
	}
      }
    }
  }
}

std::pair<int, int> countBins(BinCategoryTable & table){

  int filled = 0; 
  int empty = 0; 
  for (int i = 0; i < constants::n_x_bins; i++){
    for (int j = 0; j < constants::n_q2_bins; j++){
      for (int k = 0; k < constants::n_z_bins; k++){
	for (int m = 0; m < constants::n_pt2_bins; m++){
	  int value = table.querySafe(i, j, k, m);

	  if (value == -123){
	    empty++;
	  } else {
	    filled++; 
	  }
	}
      }
    }
  }
  return std::make_pair(filled, empty);
}

void test_tables(){
  
  BinCategoryTable pipTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories/", "pip");
  BinCategoryTable pimTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories/", "pip");

  print(pipTable); 
  print(pimTable);

  std::pair<int, int> pipCounts = countBins(pipTable); 
  std::pair<int, int> pimCounts = countBins(pimTable); 

  std::cout << "Pip empty " << pipCounts.second << " Pip full " << pipCounts.first << std::endl; 
  std::cout << "Pim empty " << pimCounts.second << " Pim full " << pimCounts.first << std::endl; 

  HapradTable pipHap("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPipModel/", "pip"); 
  HapradTable pimHap("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/haprad/hapradResults/NickPimModel/", "pim"); 

  for (int i = 0; i < constants::n_phi_bins; i++){
    HapradDataEntry entry = pipHap.querySafe(0, 0, 3, 4, i); 
    std::cout << std::setw(4) << i 
	      << std::setw(18) << std::scientific << entry.sig 
	      << std::setw(18) << std::scientific << entry.sib 
	      << std::setw(18) << std::scientific << entry.tail
	      << std::endl; 
  }

}
