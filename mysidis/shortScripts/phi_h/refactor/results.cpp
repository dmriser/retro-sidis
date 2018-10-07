#ifndef results_cpp 
#define results_cpp 

#include "constants.cpp"

struct FourDResult {
  int bin_category;
  float m, m_err; 
  float ac, ac_err; 
  float acc, acc_err; 
  float chi2, confidence; 
  float m_sys_err[constants::n_sources];
  float ac_sys_err[constants::n_sources];
  float acc_sys_err[constants::n_sources];
};

struct FiveDResult {
  int counts; 
  float acceptance, acceptance_err; 
  float rad_corr, rad_corr_err; 
  float stat_err; 
  float sys_err[constants::n_sources];
};

class CollectionOfFiveDResults {
public:
  CollectionOfFiveDResults(){
  }
  ~CollectionOfFiveDResults(){
  }

  void set(const FiveDResult & result, int index){
    fData[index] = result; 
  }

  FiveDResult get(int index) const {
    return fData[index];
  }

protected:
  FiveDResult fData[constants::n_phi_bins];
};

#endif 
