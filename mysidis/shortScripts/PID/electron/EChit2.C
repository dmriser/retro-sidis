void EChit2(int sector = 1)
{
gStyle->SetOptStat(0);

TFile *fD = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");
TFile *fMC = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");

TH2F *hD[2][6];
TH2F *hMC[2][6];

float uMin[5] = {58, 64, 70, 76, 82}; // loosest, loose, nominal, tight, tightest
float uMax[5] = {412, 406, 400, 394, 388};
float vMax[5] = {374, 368, 362, 356, 350};
float wMax[5] = {407, 401, 395, 389, 383};

TF1 *fuMin[5], *fuMax[5], *fvMax[5], *fwMax[5];

for(int k = 0; k < 5; k++)
{
fuMin[k] = new TF1(Form("fuMin%i", k), Form("49.4 + 0.808*%3.2f", uMin[k]), -1000, 1000);
fuMax[k] = new TF1(Form("fuMax%i", k), Form("49.4 + 0.808*%3.2f", uMax[k]), -1000, 1000);
fvMax[k] = new TF1(Form("fvMax%i", k), Form("-1.732*x + 723.0 - 1.76*%3.2f", vMax[k]), -1000, 1000);
fwMax[k] = new TF1(Form("fwMax%i", k), Form("1.732*x + 714.17 - 1.57*%3.2f", wMax[k]), -1000, 1000);
}

//fuMin[2]->SetLineColor(kBlack); // highlight the nominal cut
//fuMax[2]->SetLineColor(kBlack);
//fvMax[2]->SetLineColor(kBlack);
//fwMax[2]->SetLineColor(kBlack);

TCanvas *can = new TCanvas("can", "can", 1500, 60, 1600, 1000);
can->Divide(6, 4, 0.00001, 0.00001);

for(int c = 0; c < 2; c++)
{
for(int s = 0; s < 6; s++)
{
can->cd(c*6 + s + 1);
hD[c][s] = (TH2F*) fD->Get(Form("eIDplots/ech_xVech_y_e_hist_c%i_s%i", c, s+1));
hD[c][s]->GetXaxis()->SetTitle("Y_{EC} (cm)");
hD[c][s]->GetYaxis()->SetTitle("X_{EC} (cm)");
hD[c][s]->Draw("colz");
if(c == 1)
{
for(int k = 0; k < 5; k++)
{
fuMin[k]->Draw("same");
fuMax[k]->Draw("same");
fvMax[k]->Draw("same");
fwMax[k]->Draw("same");
}
}

can->cd(12 + c*6 + s + 1);
hMC[c][s] = (TH2F*) fMC->Get(Form("eIDplots/ech_xVech_y_e_hist_c%i_s%i", c, s+1));
hMC[c][s]->GetXaxis()->SetTitle("Y_{EC} (cm)");
hMC[c][s]->GetYaxis()->SetTitle("X_{EC} (cm)");
hMC[c][s]->Draw("colz");
if(c == 1)
{
for(int k = 0; k < 5; k++)
{
fuMin[k]->Draw("same");
fuMax[k]->Draw("same");
fvMax[k]->Draw("same");
fwMax[k]->Draw("same");
}
}

}
}

// _____________________________________ //

TCanvas *can2 = new TCanvas("can2", "can2", 60, 60, 1000, 800);
can2->Divide(2, 2, 0.00001, 0.00001);

can2->cd(1);
hD[0][sector-1]->Draw("colz");
can2->cd(2);
hD[1][sector-1]->Draw("colz");
for(int k = 0; k < 5; k++)
{
fuMin[k]->Draw("same");
fuMax[k]->Draw("same");
fvMax[k]->Draw("same");
fwMax[k]->Draw("same");
}
can2->cd(3);
hMC[0][sector-1]->Draw("colz");
can2->cd(4);
hMC[1][sector-1]->Draw("colz");
for(int k = 0; k < 5; k++)
{
fuMin[k]->Draw("same");
fuMax[k]->Draw("same");
fvMax[k]->Draw("same");
fwMax[k]->Draw("same");
}

//can2->SaveAs(Form("EChit_s%i.png", sector));

}
