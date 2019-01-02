
struct BinIndex {
  int x, q2, z, pt2;

  BinIndex(int xi, int q2i, int zi, int pt2i){
    x = xi; 
    q2 = q2i; 
    z = zi; 
    pt2 = pt2i;
  }
};

struct HistogramPack {
  TH1F *rec, *gen, *acc; 
};

HistogramPack *getHistograms(TFile *file, BinIndex currentBin, std::string hadronType){

  HistogramPack *pack = new HistogramPack();

  if (file && file->IsOpen()){
    
    pack->rec = (TH1F*) file->Get(Form("rec_%s_phih_x%d_QQ%d_z%d_PT2%d", hadronType.c_str(),
				       currentBin.x, currentBin.q2, currentBin.z, currentBin.pt2));
    pack->gen = (TH1F*) file->Get(Form("gen_%s_phih_x%d_QQ%d_z%d_PT2%d", hadronType.c_str(),
				       currentBin.x, currentBin.q2, currentBin.z, currentBin.pt2));
    
    // setup accepatance and plot 
    pack->acc = (TH1F*) pack->rec->Clone();
    pack->acc->SetTitle(Form("acc_%s_phih_x%d_QQ%d_z%d_PT2%d", hadronType.c_str(),
			     currentBin.x, currentBin.q2, currentBin.z, currentBin.pt2));
    pack->acc->SetName(Form("acc_%s_phih_x%d_QQ%d_z%d_PT2%d", hadronType.c_str(),
			    currentBin.x, currentBin.q2, currentBin.z, currentBin.pt2));
    pack->acc->Divide(pack->gen);
  } 
  else {
    std::cerr << "The file isn't open! " << std::endl; 
  }
  
  return pack; 
}

namespace constants {
  const int n_bins_x   = 5; 
  const int n_bins_q2  = 2;
  const int n_bins_z   = 18;
  const int n_bins_pt2 = 20; 
};

void acceptance(){

  /* Open a bin and plot the acceptance. */

  // setup 
  const std::string inputFile("/home/dmriser/mysidis-histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");
  const std::string hadronType("pip");

  // init files and histograms 
  TFile *file = TFile::Open(inputFile.c_str());

  // define bins to cover 
  int startBinZ = 2; 
  int stopBinZ = 10; 
  int startBinPt2 = 4; 
  int stopBinPt2 = 8; 
  int nBinsPlottedZ   = stopBinZ - startBinZ; 
  int nBinsPlottedPt2 = stopBinPt2 - startBinPt2; 

  std::cout << "Setup canvas (" << nBinsPlottedZ << ", " << nBinsPlottedPt2 << ") total = " << nBinsPlottedZ * nBinsPlottedPt2 << std::endl;

  TCanvas *can = new TCanvas("can", "can", nBinsPlottedZ * 400, nBinsPlottedPt2 * 300);
  can->Divide(nBinsPlottedZ, nBinsPlottedPt2);

  // labels 
  TLatex *label = new TLatex();
  label->SetNDC();
  label->SetTextSize(0.05);
  label->SetTextFont(42);

  // styling options 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int canvasIndex = 1; 
  for (int ptbin = startBinPt2; ptbin < stopBinPt2; ptbin++){
    for (int zbin = startBinZ; zbin < stopBinZ; zbin++){
      can->cd(canvasIndex);
      
      BinIndex currentBin(0, 0, zbin, ptbin); 
      HistogramPack *currentPack = getHistograms(file, currentBin, hadronType);      

      currentPack->acc->SetLineColor(kBlack);
      currentPack->acc->SetMarkerStyle(8);
      currentPack->acc->SetLineStyle(1);
      currentPack->acc->SetLineWidth(1);
      currentPack->acc->SetMinimum(0.0);
      currentPack->acc->SetMaximum(1.1);
      currentPack->acc->Draw();
  
      currentPack->rec->Scale( 1.0 / currentPack->gen->GetMaximum() );
      currentPack->gen->Scale( 1.0 / currentPack->gen->GetMaximum() );
  
      currentPack->rec->SetLineColor(99);
      currentPack->rec->SetMarkerStyle(8);
      currentPack->rec->SetMarkerColor(99);
      currentPack->rec->SetLineStyle(1);
      currentPack->rec->SetLineWidth(1);
      currentPack->rec->SetMinimum(0.0);
      currentPack->rec->SetMaximum(1.1);
      currentPack->rec->Draw("same");
  
      currentPack->gen->SetLineColor(78);
      currentPack->gen->SetMarkerStyle(8);
      currentPack->gen->SetMarkerColor(78);
      currentPack->gen->SetLineStyle(1);
      currentPack->gen->SetLineWidth(1);
      currentPack->gen->SetMinimum(0.0);
      currentPack->gen->SetMaximum(1.1);
      currentPack->gen->Draw("same");

      label->DrawLatex(0.34, 0.85, Form("(0, 0, %d, %d)", zbin, ptbin));
      label->DrawLatex(0.38, 0.02, "#phi_{h} [degrees]");

      canvasIndex++; 
    }    
  }

  can->Print("/volatile/clas12/dmriser/plots/nathan/acceptance.pdf");
}
