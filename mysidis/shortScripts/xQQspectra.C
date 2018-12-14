{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kViridis);

  //  TFile *tf = new TFile("/u/home/dmriser/mysidis-histos/MonteCarlo_v12.it0.s1.n32255.BiSc5.__0000000000000000__.root");
  TFile *tf = new TFile("/u/home/dmriser/mysidis-histos/MonteCarlo_v12.it0.s1.n32255.BiSc5.__0000000000000000__.root");

  string pipORpim = "pim";
  string genORrec = "rec";

  //scheme 5:
  const int NxBins = 5;
  float xLimits[NxBins + 1] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
  const int NQQBins = 2;
  float QQLimits[NxBins][NQQBins + 1] = {{1.0, 1.3, 5.0}, {1.0, 1.7, 5.0}, {1.0, 2.2, 5.0}, {1.0, 2.9, 5.0}, {1.0, 5.0, 99.9}};

  // //scheme 8:
  // const int NxBins = 5*2;
  // float xLimits[NxBins + 1] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60};
  // const int NQQBins = 2*2;
  // float QQLimits[NxBins][NQQBins + 1];
  // QQLimits[0][0] = 1.00; QQLimits[0][1] = 1.15; QQLimits[0][2] = 1.30; QQLimits[0][3] = 1.45; QQLimits[0][4] = 1.80; QQLimits[1][0] = 1.00; QQLimits[1][1] = 1.15; QQLimits[1][2] = 1.30; QQLimits[1][3] = 1.45; QQLimits[1][4] = 1.80; QQLimits[2][0] = 1.00; QQLimits[2][1] = 1.50; QQLimits[2][2] = 1.70; QQLimits[2][3] = 2.00; QQLimits[2][4] = 2.70; QQLimits[3][0] = 1.00; QQLimits[3][1] = 1.50; QQLimits[3][2] = 1.70; QQLimits[3][3] = 2.00; QQLimits[3][4] = 2.70; QQLimits[4][0] = 1.40; QQLimits[4][1] = 1.90; QQLimits[4][2] = 2.20; QQLimits[4][3] = 2.70; QQLimits[4][4] = 3.50; QQLimits[5][0] = 1.40; QQLimits[5][1] = 1.90; QQLimits[5][2] = 2.20; QQLimits[5][3] = 2.70; QQLimits[5][4] = 3.50; QQLimits[6][0] = 2.20; QQLimits[6][1] = 2.70; QQLimits[6][2] = 2.90; QQLimits[6][3] = 3.40; QQLimits[6][4] = 4.30; QQLimits[7][0] = 2.20; QQLimits[7][1] = 2.70; QQLimits[7][2] = 2.90; QQLimits[7][3] = 3.40; QQLimits[7][4] = 4.30; QQLimits[8][0] = 3.30; QQLimits[8][1] = 4.00; QQLimits[8][2] = 5.00; QQLimits[8][3] = 6.00; QQLimits[8][4] = 7.00; QQLimits[9][0] = 3.30; QQLimits[9][1] = 4.00; QQLimits[9][2] = 5.00; QQLimits[9][3] = 6.00; QQLimits[9][4] = 7.00;

  float yMax = 0.85;
  float WMin = 2.05;
  float QQMin = 1.0;
  TF1 *yFcn = new TF1("yFcn", Form("2.0*5.498*%3.4f*0.938*x", yMax), 0, 1); 
  TF1 *WFcn = new TF1("WFcn", Form("(pow(%3.4f, 2.0) - pow(0.938, 2.0))/((1.0/x) - 1.0)", WMin), 0, 1); 
  TF1 *QQFcn = new TF1("QQFcn", Form("0.0*x + %3.4f", QQMin), 0, 1);
  yFcn->SetLineWidth(2);
  WFcn->SetLineWidth(2);
  QQFcn->SetLineWidth(2);
  yFcn->SetLineColor(kBlack);
  WFcn->SetLineColor(kBlack);
  QQFcn->SetLineColor(kBlack);

  TCanvas *can1 = new TCanvas("can1", "can1", 10, 10, 1000, 700);

  can1->cd(1)->SetLogz();
  TH2F *QQvsx = (TH2F*) tf->Get(Form("%s_%s_QQvsx", genORrec.c_str(), pipORpim.c_str()));
  QQvsx->GetXaxis()->SetTitle("x");
  QQvsx->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
  if(pipORpim == "pip") QQvsx->SetTitle("#pi+ channel");
  if(pipORpim == "pim") QQvsx->SetTitle("#pi- channel");
  QQvsx->Draw("colz");
  TLine *xLine[NxBins + 1];
  TLine *QQLine[NxBins][NQQBins + 1];

  for(int k = 0; k < NxBins + 1; k++)
    {
      xLine[k] = new TLine(xLimits[k], QQvsx->GetYaxis()->GetXmin(), xLimits[k], QQvsx->GetYaxis()->GetXmax());
      xLine[k]->SetLineColor(kGray+1);
      xLine[k]->SetLineWidth(2);
      xLine[k]->Draw();
      if(k < NxBins)
	{
	  for(int j = 0; j < NQQBins + 1; j++)
	    {
	      QQLine[k][j] = new TLine(xLimits[k], QQLimits[k][j], xLimits[k+1], QQLimits[k][j]);
	      QQLine[k][j]->SetLineColor(kGray+1);
	      QQLine[k][j]->SetLineWidth(2);
	      //QQLine[k][j]->Draw();
	      if(!(j == 0 || j == NQQBins)) QQLine[k][j]->Draw(); // don't want to draw the bottom most or top most line
	    }
	}
    }

  yFcn->Draw("same");
  WFcn->Draw("same");
  QQFcn->Draw("same");

}
