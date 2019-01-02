
void zpt(){

  // constants 
  const int nType = 2;
  const std::string baseFilePath("/u/home/dmriser/mysidis-histos");
  std::string fileName[nType] = {"data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root", 
				 "MonteCarlo_v12.it0.s1.n32255.BiSc5.__0000000000000000__.root"};
  std::string typeName[nType] = {"Data", "Monte Carlo"};

  // root objects that persist 
  TFile *file[nType];
  TH2F  *zpt[nType][2];

  // limits on z bins 
  const int n_z_bins = 18; 
  const int n_pt2_bins = 20; 
  float z_limits[n_z_bins + 1];
  float pt2_limits[n_pt2_bins + 1];
  TLine *z_lines[n_z_bins + 1];
  TLine *pt2_lines[n_pt2_bins + 1];

  for(int i = 0; i < n_z_bins + 1; i++){
    z_limits[i] = i * (0.9 / n_z_bins); 
  }

  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_limits[i] = i * (1.0 / n_pt2_bins); 
  }

  for(int i = 0; i < n_z_bins + 1; i++){
    z_lines[i] = new TLine(z_limits[i], 0, z_limits[i], 1); 
  }

  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_lines[i] = new TLine(0.0, pt2_limits[i], 0.9, pt2_limits[i]); 
  }

  for (int type = 0; type < nType; type++){

    // open files 
    file[type] = TFile::Open(Form("%s/%s", baseFilePath.c_str(), fileName[type].c_str()));

    // get histos 
    zpt[type][0] = (TH2F*) file[type]->Get("rec_pip_PT2vsz");
    zpt[type][1] = (TH2F*) file[type]->Get("rec_pim_PT2vsz");
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
  zpt[0][0]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.4, 0.92, "Data (#pi^{+})");

  for(int i = 0; i < n_z_bins + 1; i++){
    z_lines[i]->Draw(); 
  }
  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_lines[i]->Draw(); 
  }

  can->cd(2);
  zpt[0][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.4, 0.92, "Data (#pi^{-})");
  for(int i = 0; i < n_z_bins + 1; i++){
    z_lines[i]->Draw(); 
  }
  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_lines[i]->Draw(); 
  }

  can->cd(3);
  zpt[1][0]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{+})");
  for(int i = 0; i < n_z_bins + 1; i++){
    z_lines[i]->Draw(); 
  }
  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_lines[i]->Draw(); 
  }

  can->cd(4);
  zpt[1][1]->Draw("colz");
  gPad->SetLogz();
  label->DrawLatex(0.35, 0.92, "Monte Carlo (#pi^{-})");
  for(int i = 0; i < n_z_bins + 1; i++){
    z_lines[i]->Draw(); 
  }
  for(int i = 0; i < n_pt2_bins + 1; i++){
    pt2_lines[i]->Draw(); 
  }

  can->Print("/volatile/clas12/dmriser/plots/nathan/zpt.pdf");
}
