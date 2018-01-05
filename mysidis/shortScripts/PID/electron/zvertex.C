{
gStyle->SetOptStat(0);

TFile *dfile = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");
TFile *Mfile = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");

// ______________________ //

//   int Nstrictnesses = 5;
//   float leftcut[Nstrictnesses] = {-27.7302 - 0.4, -27.7302 - 0.2, -27.7302, -27.7302 + 0.2, -27.7302 + 0.4};
//   float rightcut[Nstrictnesses] = {-22.6864 + 0.4, -22.6864 + 0.2, -22.6864, -22.6864 - 0.2, -22.6864 - 0.4};
//   //float MCzshift = 0.2083;

//int Nstrictnesses = 1;
//float leftcut[Nstrictnesses] = {-27.7302};
//float rightcut[Nstrictnesses] = {-22.6864};

int Nstrictnesses = 3;
float leftcut[Nstrictnesses] = {-27.7302 - 0.2, -27.7302, -27.7302 + 0.2};
float rightcut[Nstrictnesses] = {-22.6864 + 0.2, -22.6864, -22.6864 - 0.2};

TLine *leftline_data[Nstrictnesses], *rightline_data[Nstrictnesses];
TLine *leftline_MC[Nstrictnesses], *rightline_MC[Nstrictnesses];
float lineHeight = 800000;
for(int st = 0; st < Nstrictnesses; st++)
{
leftline_data[st] = new TLine(leftcut[st], 0, leftcut[st], lineHeight);
leftline_data[st]->SetLineWidth(2);
leftline_data[st]->SetLineColor(kBlack);
rightline_data[st] = new TLine(rightcut[st], 0, rightcut[st], lineHeight);
rightline_data[st]->SetLineWidth(2);
rightline_data[st]->SetLineColor(kBlack);

//leftline_MC[st] = new TLine(leftcut[st] + MCzshift, 0, leftcut[st] + MCzshift, lineHeight);
leftline_MC[st] = new TLine(-29.0, 0, -29.0, lineHeight);
leftline_MC[st]->SetLineWidth(2);
leftline_MC[st]->SetLineColor(kBlack);
//rightline_MC[st] = new TLine(rightcut[st] + MCzshift, 0, rightcut[st] + MCzshift, lineHeight);
rightline_MC[st] = new TLine(-21.0, 0, -21.0, lineHeight);
rightline_MC[st]->SetLineWidth(2);
rightline_MC[st]->SetLineColor(kBlack);
}

// ______________________ //

Int_t colors[6] = {kCyan+1, kBlue, kViolet, kRed, kOrange, kGreen};

TH1F *hD[2][6];
TH1F *hMC[2][6];
TH1F *hDcorr[2][6];
TH1F *hMCcorr[2][6];

for(int c = 0; c < 2; c++)
{
for(int s = 0; s < 6; s++)
{
hD[c][s] = (TH1F*) dfile->Get(Form("eIDplots/vz_e_hist_c%i_s%i", c, s+1));
hD[c][s]->GetXaxis()->SetTitle("z vertex (cm)");
hD[c][s]->SetLineColor(colors[s]);
hD[c][s]->GetYaxis()->SetRangeUser(0, 1.2*hD[c][s]->GetMaximum());

hDcorr[c][s] = (TH1F*) dfile->Get(Form("eIDplots/corrvz_e_hist_c%i_s%i", c, s+1));
hDcorr[c][s]->GetXaxis()->SetTitle("corrected z vertex (cm)");
hDcorr[c][s]->SetLineColor(colors[s]);
hDcorr[c][s]->GetYaxis()->SetRangeUser(0, 1.2*hDcorr[c][s]->GetMaximum());

hMC[c][s] = (TH1F*) Mfile->Get(Form("eIDplots/vz_e_hist_c%i_s%i", c, s+1));
hMC[c][s]->GetXaxis()->SetTitle("z vertex (cm)");
hMC[c][s]->SetLineColor(colors[s]);
hMC[c][s]->GetYaxis()->SetRangeUser(0, 1.2*hMC[c][s]->GetMaximum());

hMCcorr[c][s] = (TH1F*) Mfile->Get(Form("eIDplots/corrvz_e_hist_c%i_s%i", c, s+1));
hMCcorr[c][s]->GetXaxis()->SetTitle("corrected z vertex (cm)");
hMCcorr[c][s]->SetLineColor(colors[s]);
hMCcorr[c][s]->GetYaxis()->SetRangeUser(0, 1.2*hMCcorr[c][s]->GetMaximum());
}
}

TCanvas *can = new TCanvas("can", "can", 50, 50, 1800, 1000);
can->Divide(4, 2, 0.00001, 0.00001);

can->cd(1);
hD[0][0]->SetTitle("neg. tracks w/ dc, sc, cc, and ec hit");
for(int s = 0; s < 6; s++) hD[0][s]->Draw("same");

TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
leg->AddEntry(hD[0][0], "sector 1", "l");
leg->AddEntry(hD[0][1], "sector 2", "l");
leg->AddEntry(hD[0][2], "sector 3", "l");
leg->AddEntry(hD[0][3], "sector 4", "l");
leg->AddEntry(hD[0][4], "sector 5", "l");
leg->AddEntry(hD[0][5], "sector 6", "l");
leg->Draw();

can->cd(2);
hDcorr[0][0]->SetTitle("neg. tracks w/ dc, sc, cc, and ec hit");
for(int s = 0; s < 6; s++) hDcorr[0][s]->Draw("same");

can->cd(3);
hD[1][0]->SetTitle("with other e- ID cuts");
for(int s = 0; s < 6; s++) hD[1][s]->Draw("same");

can->cd(4);
hDcorr[1][0]->SetTitle("with other e- ID cuts");
for(int s = 0; s < 6; s++)
{
hDcorr[1][s]->Draw("same");
for(int st = 0; st < Nstrictnesses; st++)
{
leftline_data[st]->Draw();
rightline_data[st]->Draw();
}
}

can->cd(5);
hMC[0][0]->SetTitle("neg. tracks w/ dc, sc, cc, and ec hit");
for(int s = 0; s < 6; s++) hMC[0][s]->Draw("same");

can->cd(6);
hMCcorr[0][0]->SetTitle("neg. tracks w/ dc, sc, cc, and ec hit");
for(int s = 0; s < 6; s++) hMCcorr[0][s]->Draw("same");

can->cd(7);
hMC[1][0]->SetTitle("with other e- ID cuts");
for(int s = 0; s < 6; s++) hMC[1][s]->Draw("same");

can->cd(8);
hMCcorr[1][0]->SetTitle("with other e- ID cuts");
for(int s = 0; s < 6; s++)
{
hMCcorr[1][s]->Draw("same");
for(int st = 0; st < Nstrictnesses; st++)
{
leftline_MC[st]->Draw();
rightline_MC[st]->Draw();
}
}

// _______ redraw a zoomed in version _______ //

TH1F *zoomD[6], *zoomMC[6];

TCanvas *can2 = new TCanvas("can2", "can2", 60, 60, 1300, 700);
can2->Divide(2, 1, 0.00001, 0.00001);

for(int s = 0; s < 6; s++)
{
can2->cd(1);
zoomD[s] = (TH1F*) hDcorr[1][s]->Clone(Form("zoomD_s%i", s+1));
zoomD[s]->GetXaxis()->SetRangeUser(-30, -20);
zoomD[s]->Draw("same");
for(int st = 0; st < Nstrictnesses; st++)
{
leftline_data[st]->Draw();
rightline_data[st]->Draw();
}

can2->cd(2);
zoomMC[s] = (TH1F*) hMCcorr[1][s]->Clone(Form("zoomMC_s%i", s+1));
zoomMC[s]->GetXaxis()->SetRangeUser(-30, -20);
zoomMC[s]->Draw("same");
for(int st = 0; st < Nstrictnesses; st++)
{
leftline_MC[st]->Draw();
rightline_MC[st]->Draw();
}
}


}
