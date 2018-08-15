#ifndef HADRON_FAST_CXX
#define HADRON_FAST_CXX

#include "functions.C"
#include "pipIDsubroutines.C"
#include "pimIDsubroutines.C"
#include "protIDsubroutines.C"

vector<int> hadronIDFast(Int_t gpart, int e_index[], Int_t q[], Float_t p[], 
			 UChar_t sc_sect[], UChar_t dc_sect[], Float_t sc_t[],
			 Float_t sc_r[], UChar_t sc_pd[], int pip_vvp_strict, 
			 int pip_R1fid_strict, int pip_MXcut_strict, int ExpOrSim, 
			 Float_t ec_ei[], UChar_t ec_sect[], UChar_t cc_sect[], 
			 UShort_t nphe[], Float_t ec_eo[], Float_t cx[], Float_t cy[], 
			 Float_t cz[], Float_t b[], Float_t tl1_x[], Float_t tl1_y[], 
			 Float_t mcp[], Float_t mcphi[], Float_t mctheta[], 
			 TLorentzVector V4_H[], int currentrunno, int pim_vvp_strict, 
			 int pim_R1fid_strict, int pim_MXcut_strict, BetaPTable *pipTable, 
			 BetaPTable *pimTable){

  Float_t speed_of_light = 29.9792458; // cm/ns
  Float_t pip_mass       = 0.13957; // GeV
  Float_t pim_mass       = 0.13957; // GeV
  Float_t prot_mass      = 0.938272; // GeV
  Float_t pi             = 3.14159265359;
  Float_t pi180          = pi/180.0;
  
  vector<int> Vpipindex;
  vector<int> Vpimindex;
  vector<int> Vprotindex;
  
  for(int k = 0; k < gpart; k++){
    // pip ID
    if(q[k] == 1 && sc_sect[k] != 0 && dc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1){
      float corrStartTime = e_sctimeCorr(ExpOrSim, sc_t[e_index[1]], dc_sect[e_index[1]], 
					 sc_pd[e_index[1]], currentrunno) - (sc_r[e_index[1]]/speed_of_light);
      float ehtcorrMeasuredTime = h_sctimeCorr(ExpOrSim, sc_t[k], dc_sect[k], sc_pd[k], currentrunno) - corrStartTime;
      float CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(pip_mass,2));
      float ehtcorrDt = ehtcorrMeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);
      float velocity = (sc_r[k]/ehtcorrMeasuredTime)/speed_of_light;
      float rot_tl1_x = tl1_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + tl1_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
      float rot_tl1_y = tl1_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - tl1_x[k]*sin((dc_sect[k]-1)*pi180*60.0);
	
      TVector3 V3_pip;
      V3_pip.SetXYZ(p[k]*cx[k], p[k]*cy[k], p[k]*cz[k]);
      TLorentzVector V4_pip;
      V4_pip.SetXYZT(V3_pip.X(), V3_pip.Y(), V3_pip.Z(), sqrt(V3_pip.Mag2() + pow(pip_mass,2))); // pip candidate, assuming pip mass
      TLorentzVector V4_X_epipX = V4_H[1] - V4_pip;
	
      // check if particle passes pipID cuts:
      bool vvp_pass, R1fid_pass, MXcut_pass;
      vvp_pass = R1fid_pass = MXcut_pass = 0;
	
      vvp_pass = pip_vvp_pass_table(pip_vvp_strict, ExpOrSim, dc_sect[e_index[1]], velocity, p[k], pipTable);
      if(vvp_pass){ 
	R1fid_pass = pip_R1fid_pass(pip_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]); 
      }
      if(vvp_pass && R1fid_pass){ 
	MXcut_pass = pip_MXcut_pass(pip_MXcut_strict, V4_X_epipX.Mag()); 
      }
	
      if(vvp_pass && R1fid_pass && MXcut_pass){ 
	Vpipindex.push_back(k); 
      }
    }
    // end pip ID
    
    // pim ID
    if(q[k] == -1 && sc_sect[k] != 0 && dc_sect[k] != 0 && k != e_index[1] && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1){
      float corrStartTime = e_sctimeCorr(ExpOrSim, sc_t[e_index[1]], dc_sect[e_index[1]], 
					 sc_pd[e_index[1]], currentrunno) - (sc_r[e_index[1]]/speed_of_light);
      float ehtcorrMeasuredTime = h_sctimeCorr(ExpOrSim, sc_t[k], dc_sect[k], sc_pd[k], currentrunno) - corrStartTime;
      float CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(pim_mass,2));
      float ehtcorrDt = ehtcorrMeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);
      float velocity = (sc_r[k]/ehtcorrMeasuredTime)/speed_of_light;
      float rot_tl1_x = tl1_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + tl1_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
      float rot_tl1_y = tl1_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - tl1_x[k]*sin((dc_sect[k]-1)*pi180*60.0);
      
      TVector3 V3_pim;
      V3_pim.SetXYZ(p[k]*cx[k], p[k]*cy[k], p[k]*cz[k]);
      TLorentzVector V4_pim;
      V4_pim.SetXYZT(V3_pim.X(), V3_pim.Y(), V3_pim.Z(), sqrt(V3_pim.Mag2() + pow(pim_mass,2))); // pim candidate, assuming pim mass
      TLorentzVector V4_X_epimX = V4_H[1] - V4_pim;
	
      // check if particle passes pimID cuts:
      bool vvp_pass, R1fid_pass, MXcut_pass;
      vvp_pass = R1fid_pass = MXcut_pass = 0;
      
      vvp_pass = pim_vvp_pass_table(pim_vvp_strict, ExpOrSim, dc_sect[e_index[1]], velocity, p[k], pimTable);
      if(vvp_pass){ 
	R1fid_pass = pim_R1fid_pass(pim_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]); 
      }
      if(vvp_pass && R1fid_pass){ 
	MXcut_pass = pim_MXcut_pass(pim_MXcut_strict, V4_X_epimX.Mag()); 
      }
      
      if(vvp_pass && R1fid_pass && MXcut_pass){ 
	Vpimindex.push_back(k); 
      }
    }
    // end pim ID
    
    // prot ID
    if(q[k] == 1 && sc_sect[k] != 0 && dc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1){
      Float_t StartTime = sc_t[e_index[1]] - (sc_r[e_index[1]]/speed_of_light);
      Float_t MeasuredTime = sc_t[k] - StartTime;
      Float_t CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(prot_mass,2));
      Float_t Dt = MeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);
	
      Bool_t DtVp_pass = prot_DtVp_pass(Dt);
	
      if(DtVp_pass == 1){ 
	Vprotindex.push_back(k); 
      }
    }
    // end prot ID
    
  } // end of loop over particles
  
  // %%%%% find highest/lowest momentum particles %%%%%
  
  // find highest momentum pip
  int pip_index = -123;
  if(Vpipindex.size() > 0){ 
    pip_index = Vpipindex[0]; 
  }
  if(Vpipindex.size() > 1){
    for(unsigned int v = 1; v < Vpipindex.size(); v++){
      if(p[Vpipindex[v]] > p[pip_index]){ 
	pip_index = Vpipindex[v]; 
      }
    }
  }
  // end find highest momentum pip

  // find highest momentum pim
  int pim_index = -123;
  if(Vpimindex.size() > 0){ 
    pim_index = Vpimindex[0]; 
  }
  if(Vpimindex.size() > 1){
    for(unsigned int v = 1; v < Vpimindex.size(); v++){
      if(p[Vpimindex[v]] > p[pim_index]){ 
	pim_index = Vpimindex[v]; 
      }
    }
  }
  // end find highest momentum pim

  // find lowest momentum prot
  int prot_index = -123;
  if(Vprotindex.size() > 0){ 
    prot_index = Vprotindex[0]; 
  }
  if(Vprotindex.size() > 1){
    for(unsigned int v = 1; v < Vprotindex.size(); v++){
      if(p[Vprotindex[v]] < p[prot_index]){ 
	prot_index = Vprotindex[v]; 
      }
    }
  }
  // end find lowest momentum prot

  vector<int> result;
  result.push_back(pip_index);
  result.push_back(pim_index);
  result.push_back(prot_index);
  
  return result;
}

#endif
