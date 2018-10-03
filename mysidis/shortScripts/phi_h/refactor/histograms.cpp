#ifndef HISTOGRAMS_CPP
#define HISTOGRAMS_CPP

#include "constants.cpp"

struct HistogramPack {
  TH1F *data, *rec, *gen; 
  TH1F *acc, *rc; 
  TH1F *corrAcc, *corrRC, *corr; 
};

TH1F * buildHistogram(std::string base, std::string hadronType, int x_bin, 
		      int q2_bin, int z_bin, int pt2_bin){

  std::string name = Form("%s_%s_%d_%d_%d_%d", base.c_str(), hadronType.c_str(), 
			  x_bin, q2_bin, z_bin, pt2_bin);
  return new TH1F(name.c_str(), name.c_str(), constants::n_phi_bins, 
		  constants::phi_min, constants::phi_max); 
}

HistogramPack createPack (std::string hadronType, int x_bin, int q2_bin, int z_bin, int pt2_bin) {
  HistogramPack histos; 
  histos.data = buildHistogram("data", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.rec = buildHistogram("rec", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.gen = buildHistogram("gen", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.acc = buildHistogram("acc", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.rc = buildHistogram("rc", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corrAcc = buildHistogram("corrAcc", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corrRC = buildHistogram("corrRC", hadronType, x_bin, q2_bin, z_bin, pt2_bin);
  histos.corr = buildHistogram("corr", hadronType, x_bin, q2_bin, z_bin, pt2_bin);

  histos.data->Sumw2(); 
  histos.rec->Sumw2(); 
  histos.gen->Sumw2(); 
  histos.acc->Sumw2(); 
  histos.rc->Sumw2(); 
  histos.corrAcc->Sumw2(); 
  histos.corrRC->Sumw2(); 
  histos.corr->Sumw2(); 

  return histos; 
}

#endif
