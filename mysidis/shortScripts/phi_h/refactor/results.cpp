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

#endif 
