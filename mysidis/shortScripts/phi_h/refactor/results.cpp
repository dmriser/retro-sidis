#ifndef results_cpp 
#define results_cpp 

struct FourDResult {
  int bin_category;
  float m, m_err; 
  float ac, ac_err; 
  float acc, acc_err; 
  float chi2, confidence; 
};

struct FiveDResult {
  int phi_bin, counts; 
  float acceptance, acceptance_err; 
  float rad_corr, rad_corr_err; 
};

#endif 
