#ifndef TABLES_CPP 
#define TABLES_CPP

#include "constants.cpp"
#include "utils.cpp"

template <class T>
class FourDimensionalTable {
public:

  FourDimensionalTable(std::string path) : fPath(path) { 
    loadTable(); 
  }
  
  virtual ~FourDimensionalTable(){

  }
  
  virtual void loadTable(){
    return; 
  }

  T query(int i, int j, int k, int m){
    return table[i][j][k][m]; 
  }

  bool inBounds(int i, int j, int k, int m){
    return ( 
	    (i > -1) && (i <   constants::n_x_bins) &&
	    (j > -1) && (j <  constants::n_q2_bins) &&
	    (k > -1) && (k <   constants::n_z_bins) &&
	    (m > -1) && (m < constants::n_pt2_bins) 
	     ); 
  }

  T querySafe(int i, int j, int k, int m){
    if (inBounds(i,j,k,m)) {
      return query(i,j,k,m); 
    } else {
      std::cerr << "Querying a bin that isn't part of the table" << std::endl; 
      return constants::tiny; 
    }
  }

protected:
  std::string fPath; 
  T table[constants::n_x_bins][constants::n_q2_bins][constants::n_z_bins][constants::n_pt2_bins]; 
};

class BinCategoryTable : public FourDimensionalTable<int> {
public: 
  BinCategoryTable (std::string path, std::string hadronType) : FourDimensionalTable(path), fHadronType(hadronType) {
    loadTable(); 
  }
  
  ~BinCategoryTable(){
  }

  void loadTable(){
    std::string filename(Form("%s/%s.dat", fPath.c_str(), fHadronType.c_str())); 
    std::ifstream input(filename);
    
    if (input && input.is_open()){
      std::string currentLine; 
      while(getline(input, currentLine)){
	std::vector<std::string> tokens = Utils::split(currentLine, ','); 

	// Take the tokenized strings and 
	// create ints. 
	std::vector<int> intTokens; 
	for (std::string token : tokens){
	  intTokens.push_back(atoi(token.c_str())); 
	}
	
	if (intTokens.size() == 5){
	  table[intTokens.at(0)][intTokens.at(1)][intTokens.at(2)][intTokens.at(3)] = intTokens.at(4); 
	} else {
	  std::cout << "Issue in line of file " << filename << std::endl; 
	}
      }
    }

  }

protected:
  std::string fHadronType; 
};

// These are the names that Nathan used, i'm not really 
// sure what they mean. 
struct HapradDataEntry {
  TH1F *sig, *sib, *tail; 
};

// This is really now a five dimensional table.  Four 
// dimensional phase space but for each point there are 
// 3 histograms which contain constants::n_phi_bins each. 
// 
// Perhaps this should just be FiveDimensionalTable. 
class HapradTable : public FourDimensionalTable<HapradDataEntry> {
public:
  HapradTable(std::string path, std::string hadronType) : FourDimensionalTable(path), fHadronType(hadronType) {
    loadTable(); 
  }

  ~HapradTable(){
  }

  void loadTable(){
    for (int i = 0; i < constants::n_x_bins; i++){
      for (int j = 0; j < constants::n_q2_bins; j++){
	for (int k = 0; k < constants::n_z_bins; k++){
	  for (int m = 0; m < constants::n_pt2_bins; m++){
	    HapradDataEntry tableEntry; 
	    tableEntry.sig = new TH1F("", "", 
				      constants::n_phi_bins, constants::phi_min, constants::phi_max); 
	    tableEntry.sib = new TH1F("", "", 
				      constants::n_phi_bins, constants::phi_min, constants::phi_max); 
	    tableEntry.tail = new TH1F("", "", 
				       constants::n_phi_bins, constants::phi_min, constants::phi_max); 

	    table[i][j][k][m] = tableEntry; 
	  }
	}
      }
    }

  }

protected:
  std::string fHadronType;
};

#endif 