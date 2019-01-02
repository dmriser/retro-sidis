
void xq2(){

  // constants 
  const int nType = 2;
  const std::string baseFilePath("/u/home/dmriser/mysidis-histos");
  std::string fileName[nType] = {"data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root", 
				 "MonteCarlo_v12.it0.s1.n32255.BiSc5.__0000000000000000__.root"};
  std::string typeName[nType] = {"Data", "Monte Carlo"};

  // root objects that persist 
  TFile *file[nType];
  TH2F  *xq2[nType][2];

  // stuff from Nathan to draw bin edges 
  const int NxBins = 5;
  const int NQQBins = 2;
  float xLimits[NxBins + 1] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
  float QQLimits[NxBins][NQQBins + 1] = {{1.0, 1.3, 5.0}, {1.0, 1.7, 5.0}, {1.0, 2.2, 5.0}, {1.0, 2.9, 5.0}, {1.0, 5.0, 99.9}};
  TLine *xLine[NxBins + 1];
  TLine *QQLine[NxBins][NQQBins + 1];

  // setup lines 
  for(int k = 0; k < NxBins + 1; k++){
      xLine[k] = new TLine(xLimits[k], 0.5, xLimits[k], 5.0);
      xLine[k]->SetLineColor(kGray+1);
      xLine[k]->SetLineWidth(1);

      if(k < NxBins){
	  for(int j = 0; j < NQQBins + 1; j++){
	    QQLine[k][j] = new TLine(xLimits[k], QQLimits[k][j], xLimits[k+1], QQLimits[k][j]);
	    QQLine[k][j]->SetLineColor(kGray+1);
	    QQLine[k][j]->SetLineWidth(1);
	    }
	}
    }

  for (int type = 0; type < nType; type++){

    // open files 
    file[type] = TFile::Open(Form("%s/%s", baseFilePath.c_str(), fileName[type].c_str()));

    // get histos 
    xq2[type][0] = (TH2F*) file[type]->Get("rec_pip_QQvsx");
    xq2[type][1] = (TH2F*) file[type]->Get("rec_pim_QQvsx");

    xq2[type][0]->GetXaxis()->SetTitle("x");
    xq2[type][1]->GetXaxis()->SetTitle("x");
    xq2[type][0]->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
    xq2[type][1]->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
  }


  // style options 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(kViridis);

  // something to write with 
  TLatex *label = new TLatex();
  label->SetNDC();
  label->SetTextFont(42);
  label->SetTextSize(0.05);

  TCanvas *can = new TCanvas("can", "can", 800, 600);
  can->Divide(2,2);

  can->cd(1);
  xq2[0][0]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.4, 0.92, "Data (#pi^{+})");

  // add bin lines 
  for(int k = 0; k < NxBins + 1; k++){
    xLine[k]->Draw();
    if(k < NxBins){
      for(int j = 0; j < NQQBins + 1; j++){
	if(!(j == 0 || j == NQQBins)){ 
	  QQLine[k][j]->Draw(); 
	}
      }
    }
  }
  
  can->cd(2);
  xq2[0][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.4, 0.92, "Data (#pi^{-})");

  // add bin lines 
  for(int k = 0; k < NxBins + 1; k++){
    xLine[k]->Draw();
    if(k < NxBins){
      for(int j = 0; j < NQQBins + 1; j++){
	if(!(j == 0 || j == NQQBins)){ 
	  QQLine[k][j]->Draw(); 
	}
      }
    }
  }

  can->cd(3);
  xq2[1][0]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{+})");

  // add bin lines 
  for(int k = 0; k < NxBins + 1; k++){
    xLine[k]->Draw();
    if(k < NxBins){
      for(int j = 0; j < NQQBins + 1; j++){
	if(!(j == 0 || j == NQQBins)){ 
	  QQLine[k][j]->Draw(); 
	}
      }
    }
  }

  can->cd(4);
  xq2[1][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{-})");

  // add bin lines 
  for(int k = 0; k < NxBins + 1; k++){
    xLine[k]->Draw();
    if(k < NxBins){
      for(int j = 0; j < NQQBins + 1; j++){
	if(!(j == 0 || j == NQQBins)){ 
	  QQLine[k][j]->Draw(); 
	}
      }
    }
  }

  can->Print("/volatile/clas12/dmriser/plots/nathan/xq2.pdf");
}
