#include "constants.cpp"
#include "tables.cpp"

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

void test_tables(){
  
  BinCategoryTable pipTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories/", "pip");
  BinCategoryTable pimTable("/u/home/dmriser/clas/retro-sidis/mysidis/requiredFiles/binCategories/", "pip");

  print(pipTable); 
  print(pimTable);
}
