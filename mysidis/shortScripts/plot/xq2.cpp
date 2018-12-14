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
  TH2F  *zpt[nType][2];

  for (int type = 0; type < nType; type++){

    // open files 
    file[type] = TFile::Open(Form("%s/%s", baseFilePath.c_str(), fileName[type].c_str()));

    // get histos 
    xq2[type][0] = (TH2F*) file[type]->Get("rec_pip_QQvsx");
    zpt[type][0] = (TH2F*) file[type]->Get("rec_pip_PT2vsz");
    xq2[type][1] = (TH2F*) file[type]->Get("rec_pim_QQvsx");
    zpt[type][1] = (TH2F*) file[type]->Get("rec_pim_PT2vsz");

    xq2[type][0]->GetXaxis()->SetTitle("x");
    xq2[type][1]->GetXaxis()->SetTitle("x");
    xq2[type][0]->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
    xq2[type][1]->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
    zpt[type][0]->GetXaxis()->SetTitle("z");
    zpt[type][1]->GetXaxis()->SetTitle("z");
    zpt[type][0]->GetYaxis()->SetTitle("P_{T}^{2} (GeV^{2})");
    zpt[type][1]->GetYaxis()->SetTitle("P_{T}^{2} (GeV^{2})");
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

  can->cd(2);
  xq2[0][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.4, 0.92, "Data (#pi^{-})");

  can->cd(3);
  xq2[1][0]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{+})");

  can->cd(4);
  xq2[1][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{-})");

  can->Print("/volatile/clas12/dmriser/plots/nathan/xq2.pdf");
}
