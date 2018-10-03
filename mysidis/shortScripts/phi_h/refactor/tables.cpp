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
      return T(); 
    }
  }

protected:
  std::string fPath; 
  T table[constants::n_x_bins][constants::n_q2_bins][constants::n_z_bins][constants::n_pt2_bins]; 
};

template <class T>
class FiveDimensionalTable {
public:

  FiveDimensionalTable(std::string path) : fPath(path) { 

  }
  
  virtual ~FiveDimensionalTable(){

  }
  
  virtual void loadTable(){
    return; 
  }

  T query(int i, int j, int k, int m, int n){
    return table[i][j][k][m][n]; 
  }

  bool inBounds(int i, int j, int k, int m, int n){
    return ( 
	    (i > -1) && (i <   constants::n_x_bins) &&
	    (j > -1) && (j <  constants::n_q2_bins) &&
	    (k > -1) && (k <   constants::n_z_bins) &&
	    (m > -1) && (m < constants::n_pt2_bins) && 
	    (n > -1) && (n < constants::n_phi_bins)
	     ); 
  }

  T querySafe(int i, int j, int k, int m, int n){
    if (inBounds(i,j,k,m,n)) {
      return query(i,j,k,m,n); 
    } else {
      std::cerr << "Querying a bin that isn't part of the table" << std::endl; 
      return T(); 
    }
  }

protected:
  std::string fPath; 
  T table[constants::n_x_bins][constants::n_q2_bins][constants::n_z_bins][constants::n_pt2_bins][constants::n_phi_bins]; 
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
  float sig, sib, tail; 
};


class HapradTable : public FiveDimensionalTable<HapradDataEntry> {
public:
  HapradTable(std::string path, std::string hadronType) : FiveDimensionalTable(path), fHadronType(hadronType) {
    loadTable(); 
  }

  void loadTable(){
    std::string filename(Form("%s/%s.dat", fPath.c_str(), fHadronType.c_str()));

    std::ifstream input(filename.c_str());
    if (input && input.is_open()){
      
      std::string currentLine; 
      while(getline(input, currentLine)){
	std::vector<std::string> tokens = Utils::split(currentLine, ','); 
	std::vector<int> intTokens; 
	std::vector<float> floatTokens; 

	for (std::string token : tokens){
	  intTokens.push_back(atoi(token.c_str()));
	  floatTokens.push_back(atof(token.c_str()));
	}

	if (tokens.size() == 8){
	  HapradDataEntry entry; 
	  entry.sig = floatTokens[5];
	  entry.sib = floatTokens[6];
	  entry.tail = floatTokens[7];
	  table[intTokens[0]][intTokens[1]][intTokens[2]][intTokens[3]][intTokens[4]] = entry; 
	} else {
	  std::cerr << "Something is wrong with file " << filename << std::endl; 
	}
      }
      
    } else {
      std::cerr << "Trouble opening file: " << filename << std::endl; 
    }

  }

protected:
  std::string fHadronType; 
};

#endif 
