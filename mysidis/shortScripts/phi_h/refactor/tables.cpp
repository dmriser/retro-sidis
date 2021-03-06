#ifndef TABLES_CPP 
#define TABLES_CPP

#include "constants.cpp"
#include "encoder.cpp"
#include "results.cpp"
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

class FourDimensionalResultsTable : public FourDimensionalTable<FourDResult> {
public:
  FourDimensionalResultsTable(std::string path, std::string hadronType) : FourDimensionalTable(path), fHadronType(hadronType) {
    
  }

  void insert(FourDResult result, int x_bin, int q2_bin, int z_bin, int pt2_bin){
    if ( inBounds(x_bin, q2_bin, z_bin, pt2_bin) ){
      table[x_bin][q2_bin][z_bin][pt2_bin] = result; 
    }
  }

  void write(){
    std::string filename(Form("%s/four_dimensional_table_%s.dat", fPath.c_str(), fHadronType.c_str()));
    std::ofstream output(filename.c_str());
    
    if (output && output.is_open()){
      for (int i = 0; i < constants::n_x_bins; i++){
	for (int j = 0; j < constants::n_q2_bins; j++){
	  for (int k = 0; k < constants::n_z_bins; k++){
	    for (int m = 0; m < constants::n_pt2_bins; m++){
	      writeRow(output, i, j, k, m); 
	    }
	  }
	}
      }
    }
    
    output.close(); 
  }
  
protected:
  std::string fHadronType; 

  void writeRow(std::ofstream & output, int i, int j, int k, int m){
    output << i << ",";
    output << j << ",";
    output << k << ",";
    output << m << ",";
    output << table[i][j][k][m].bin_category << ",";
    output << table[i][j][k][m].chi2 << ",";
    output << table[i][j][k][m].m << ",";
    output << table[i][j][k][m].m_err << ",";
    output << table[i][j][k][m].ac << ",";
    output << table[i][j][k][m].ac_err << ",";
    output << table[i][j][k][m].acc << ",";
    output << table[i][j][k][m].acc_err << ",";

    for (int s = 0; s < constants::n_sources; s++){
      output << table[i][j][k][m].m_sys_err[s] << ","; 
      output << table[i][j][k][m].ac_sys_err[s] << ","; 
      output << table[i][j][k][m].acc_sys_err[s]; 
      
      if (s != constants::n_sources - 1){
	output << ",";
      }
    }

    output << std::endl; 
  }

};


class FiveDimensionalResultsTable {
public:
  FiveDimensionalResultsTable(std::string path, std::string hadronType) : fPath(path), fHadronType(hadronType) {
    std::vector<int> dimensions = {constants::n_x_bins, constants::n_q2_bins, 
				   constants::n_z_bins, constants::n_pt2_bins, constants::n_phi_bins};
    table = new SparseContainer<FiveDResult>(dimensions); 
  }

  void insert(FiveDResult result, int x_bin, int q2_bin, int z_bin, int pt2_bin, int phi_bin){
    if ( inBounds(x_bin, q2_bin, z_bin, pt2_bin, phi_bin) ){
      if (result.counts != 0){
	// constantly re-allocating a vector is probably slow,
	// maybe this class shoudl accept arrays and then current
	// point should be a member of this class which is updated. 
	std::vector<int> point = {x_bin, q2_bin, z_bin, pt2_bin, phi_bin};
	table->insert(result, point); 
      }
    }
  }

  void write(){
    std::string filename(Form("%s/five_dimensional_table_%s.dat", fPath.c_str(), fHadronType.c_str()));
    std::ofstream output(filename.c_str());
    
    if (output && output.is_open()){
      for (int i = 0; i < constants::n_x_bins; i++){
	for (int j = 0; j < constants::n_q2_bins; j++){
	  for (int k = 0; k < constants::n_z_bins; k++){
	    for (int m = 0; m < constants::n_pt2_bins; m++){
	      for (int n = 0; n < constants::n_phi_bins; n++){
		writeRow(output, i, j, k, m, n); 
	      }
	    }
	  }
	}
      }
    }
    
    output.close(); 
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
  
protected:
  std::string fHadronType, fPath; 
  SparseContainer<FiveDResult> *table; 

  void writeRow(std::ofstream & output, int i, int j, int k, int m, int n){
    output << i << ",";
    output << j << ",";
    output << k << ",";
    output << m << ",";
    output << n << ",";

    std::vector<int> point = {i, j, k, m, n};
    FiveDResult r = table->query(point);

    output << r.counts << ",";
    output << r.acceptance << ",";
    output << r.acceptance_err << ",";
    output << r.rad_corr << ",";
    output << r.rad_corr_err << ",";
    output << r.stat_err << ",";

    for (int s = 0; s < constants::n_sources; s++){
      output << r.sys_err[s]; 
      
      if (s != constants::n_sources - 1){
	output << ",";
      }
    }

    output << std::endl; 
  }

};

#endif 
