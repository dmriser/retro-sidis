#ifndef HISTOGRAMS_CPP
#define HISTOGRAMS_CPP

// root 
#include "TFile.h"
#include "TH1.h"

// this projects 
#include "constants.cpp"

struct HistogramPack {
  TH1F *data, *rec, *gen; 
  TH1F *acc, *rc, *sig, *sib; 
  TH1F *corrAcc, *corrRC, *corr; 
};

TH1F * buildHistogram(std::string base, std::string hadronType, int x_bin, 
		      int q2_bin, int z_bin, int pt2_bin){

  std::string name = Form("%s_%s_%d_%d_%d_%d", base.c_str(), hadronType.c_str(), 
			  x_bin, q2_bin, z_bin, pt2_bin);
  return new TH1F(name.c_str(), name.c_str(), constants::n_phi_bins, 
		  constants::phi_min, constants::phi_max); 
}

HistogramPack createPack (std::string base, std::string hadronType, 
			  int x_bin, int q2_bin, int z_bin, int pt2_bin) {
  HistogramPack histos; 
  histos.data = buildHistogram(Form("data_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.rec = buildHistogram(Form("rec_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.gen = buildHistogram(Form("gen_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.acc = buildHistogram(Form("acc_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.rc = buildHistogram(Form("rc_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.sig = buildHistogram(Form("sig_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.sib = buildHistogram(Form("sib_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corrAcc = buildHistogram(Form("corrAcc_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corrRC = buildHistogram(Form("corrRC_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corr = buildHistogram(Form("corr_%s", base.c_str()), hadronType, x_bin, q2_bin, z_bin, pt2_bin);

  histos.data->Sumw2(); 
  histos.rec->Sumw2(); 
  histos.gen->Sumw2(); 
  histos.acc->Sumw2(); 
  histos.rc->Sumw2(); 
  histos.sig->Sumw2(); 
  histos.sib->Sumw2(); 
  histos.corrAcc->Sumw2(); 
  histos.corrRC->Sumw2(); 
  histos.corr->Sumw2(); 

  return histos; 
}

void writeHistogramPackToFile(TFile *file, HistogramPack & histos){
  if (file && file->IsOpen()){
    histos.data->Write(); 
    histos.rec->Write(); 
    histos.gen->Write(); 
    histos.acc->Write(); 
    histos.rc->Write(); 
    histos.sig->Write(); 
    histos.sib->Write(); 
    histos.corrAcc->Write(); 
    histos.corrRC->Write(); 
    histos.corr->Write();     
  } else {
    std::cerr << "Trouble writing to file! " << std::endl; 
  }
}

#endif
