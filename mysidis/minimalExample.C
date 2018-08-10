#include <fstream>
#include <iostream>
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TRegexp.h"
#include "TString.h"
#include "TVector3.h"
#include "programFiles/functions.C"
#include "programFiles/eID.C"
#include "programFiles/hadronID.C"
#include "programFiles/getGenIndices.C"
#include "MomCorr.C"

void minimalExample(){

  // Declaration of all constants 
  // that do not change throughout 
  // the execution of the program. 
  const static int ExpOrSim    = 1; 
  const static int files       = 1; 
  const static int BUFFER_SIZE = 16; 
  const static int INVALID_ID  = -123; 

  TStopwatch *timer = new TStopwatch();
  MomCorr_e1f *MomCorr = new MomCorr_e1f();
    
  ifstream filelist;
  filelist.open("../files.dat");
    
  TChain *h22chain = new TChain("h22");

  for(int k = 0; k < files; k++){
    string filename;
    filelist >> filename;
    h22chain->Add(filename.c_str());

    TString stringyFilename(filename); 
    TString runNumberWithStub = getRunNumberWithStubFromFilename(stringyFilename);

    cout << "Adding: " << runNumberWithStub << endl; 
  }

  Int_t mcnentr;
  Int_t mcid[BUFFER_SIZE];
  Float_t mctheta[BUFFER_SIZE];
  Float_t mcphi[BUFFER_SIZE];
  Float_t mcp[BUFFER_SIZE];
  
  Int_t gpart;
  
  Float_t p[BUFFER_SIZE];
  Int_t q[BUFFER_SIZE];
  Float_t cx[BUFFER_SIZE];
  Float_t cy[BUFFER_SIZE];
  Float_t cz[BUFFER_SIZE];
  Float_t vz[BUFFER_SIZE];
  Float_t vy[BUFFER_SIZE];
  Float_t vx[BUFFER_SIZE];
  Float_t b[BUFFER_SIZE];
  
  UChar_t dc_sect[BUFFER_SIZE];
  Float_t tl1_x[BUFFER_SIZE];
  Float_t tl1_y[BUFFER_SIZE];
  Float_t tl3_x[BUFFER_SIZE]; // used to be dc_xsc in h10
  Float_t tl3_y[BUFFER_SIZE];
  Float_t tl3_z[BUFFER_SIZE];
  Float_t tl3_cx[BUFFER_SIZE]; // used to be dc_cxsc in h10
  Float_t tl3_cy[BUFFER_SIZE];
  Float_t tl3_cz[BUFFER_SIZE];
  
  UChar_t ec_sect[BUFFER_SIZE];
  Float_t etot[BUFFER_SIZE];
  Float_t ec_ei[BUFFER_SIZE];
  Float_t ec_eo[BUFFER_SIZE];
  Float_t ec_t[BUFFER_SIZE];
  Float_t ech_x[BUFFER_SIZE];
  Float_t ech_y[BUFFER_SIZE];
  Float_t ech_z[BUFFER_SIZE];
  
  UChar_t sc_sect[BUFFER_SIZE];
  Float_t sc_t[BUFFER_SIZE];
  Float_t sc_r[BUFFER_SIZE];
  UChar_t sc_pd[BUFFER_SIZE];
  
  UChar_t cc_sect[BUFFER_SIZE];
  UShort_t cc_segm[BUFFER_SIZE];
  UShort_t nphe[BUFFER_SIZE];
    
  // Set branch addresses, connecting local 
  // variables to tree branch pointer variables. 
  h22chain->SetBranchAddress("gpart", &gpart);
  h22chain->SetBranchAddress("p", p);
  h22chain->SetBranchAddress("q", q);
  h22chain->SetBranchAddress("cx", cx);
  h22chain->SetBranchAddress("cy", cy);
  h22chain->SetBranchAddress("cz", cz);
  h22chain->SetBranchAddress("vz", vz);
  h22chain->SetBranchAddress("vy", vy);
  h22chain->SetBranchAddress("vx", vx);
  h22chain->SetBranchAddress("b", b);
  
  h22chain->SetBranchAddress("dc_sect", dc_sect);
  h22chain->SetBranchAddress("tl1_x", tl1_x);
  h22chain->SetBranchAddress("tl1_y", tl1_y);
  h22chain->SetBranchAddress("tl3_x", tl3_x);
  h22chain->SetBranchAddress("tl3_y", tl3_y);
  h22chain->SetBranchAddress("tl3_z", tl3_z);
  h22chain->SetBranchAddress("tl3_cx", tl3_cx);
  h22chain->SetBranchAddress("tl3_cy", tl3_cy);
  h22chain->SetBranchAddress("tl3_cz", tl3_cz);
  
  h22chain->SetBranchAddress("ec_sect", ec_sect);
  h22chain->SetBranchAddress("etot", etot);
  h22chain->SetBranchAddress("ec_ei", ec_ei);
  h22chain->SetBranchAddress("ec_eo", ec_eo);
  h22chain->SetBranchAddress("ec_t", ec_t);
  h22chain->SetBranchAddress("ech_x", ech_x);
  h22chain->SetBranchAddress("ech_y", ech_y);
  h22chain->SetBranchAddress("ech_z", ech_z);
  
  h22chain->SetBranchAddress("sc_sect", sc_sect);
  h22chain->SetBranchAddress("sc_t", sc_t);
  h22chain->SetBranchAddress("sc_r", sc_r);
  h22chain->SetBranchAddress("sc_pd", sc_pd);
  
  h22chain->SetBranchAddress("cc_sect", cc_sect);
  h22chain->SetBranchAddress("cc_segm", cc_segm);
  h22chain->SetBranchAddress("nphe", nphe);
  

  // Declaration of strictnesses for 
  // electron identification. 
  Int_t e_zvertex_strict         = 0; 
  Int_t e_ECsampling_strict      = 0; 
  Int_t e_ECoutVin_strict        = 0; 
  Int_t e_CCthetaMatching_strict = 0; 
  Int_t e_ECgeometric_strict     = 0; 
  Int_t e_R1fid_strict           = 0; 
  Int_t e_R3fid_strict           = 0;
  Int_t e_CCphiMatching_strict   = 0; 
  Int_t e_CCfiducial_strict      = 0; 


  // Begin processing all entries. 
  Int_t totalElectrons = 0; 
  for (int i = 0; i < h22chain->GetEntries(); i++){
    h22chain->GetEntry(i);

    Int_t currentRunNumber = INVALID_ID; 
    if(ExpOrSim == 1){
      currentRunNumber = getRunNumberFromFilename(h22chain->GetCurrentFile()->GetName());
    }
    
    Int_t electronIndex = eID(
			      gpart, q, p, cc_sect, sc_sect, ec_sect, 
			      dc_sect, cx, cy, cz, tl1_x, tl1_y, tl3_x, 
			      tl3_y, tl3_z, tl3_cx, tl3_cy, tl3_cz, 
			      e_zvertex_strict, vz, vy, vx, e_ECsampling_strict, 
			      ExpOrSim, etot, e_ECoutVin_strict, ec_ei, ech_x, 
			      ech_y, ech_z, e_CCthetaMatching_strict, cc_segm, 
			      e_ECgeometric_strict, e_R1fid_strict, e_R3fid_strict, 
			      e_CCphiMatching_strict, sc_pd, e_CCfiducial_strict
			      );
    
    if (electronIndex > INVALID_ID){
      totalElectrons++; 
    }

    if(i%10000 == 0){
      cout<<"\rFinished Event ("<< i << "/" << h22chain->GetEntries() << ")" << flush;
    }

    // This brace closes the loop over
    // all entries in the tree. 
  } 
  
  // Summarize things. 
  cout << endl << "Done!" << endl;
  cout << "Found electrons: " << totalElectrons << endl; 
  timer->Print();
} 
