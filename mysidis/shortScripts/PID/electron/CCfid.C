{
gStyle->SetOptStat(0);

int sector = 1;

TFile *fD = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");
TFile *fMC = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");

TH2F *hD[2][6];
TH2F *hMC[2][6];

TCanvas *can = new TCanvas("can", "can", 1500, 60, 1600, 1000);
can->Divide(6, 4, 0.00001, 0.00001);

for(int c = 0; c < 2; c++)
{
for(int s = 0; s < 6; s++)
{
can->cd(c*6 + s + 1);
hD[c][s] = (TH2F*) fD->Get(Form("eIDplots/CCfid_e_hist_c%i_s%i", c, s+1));
hD[c][s]->GetXaxis()->SetTitle("phi (deg.)");
hD[c][s]->GetYaxis()->SetTitle("thetaCC (deg.)");
hD[c][s]->SetTitle(Form("sector %i", s+1));
hD[c][s]->Draw("colz");

can->cd(12 + c*6 + s + 1);
hMC[c][s] = (TH2F*) fMC->Get(Form("eIDplots/CCfid_e_hist_c%i_s%i", c, s+1));
hMC[c][s]->GetXaxis()->SetTitle("phi (deg.)");
hMC[c][s]->GetYaxis()->SetTitle("thetaCC (deg.)");
hMC[c][s]->Draw("colz");
}
}

// _____________________________________ //

TCanvas *can2 = new TCanvas("can2", "can2", 60, 60, 1000, 800);
can2->Divide(2, 2, 0.00001, 0.00001);

can2->cd(1);
hD[0][sector-1]->Draw("colz");
can2->cd(2);
hD[1][sector-1]->Draw("colz");

can2->cd(3);
hMC[0][sector-1]->Draw("colz");
can2->cd(4);
hMC[1][sector-1]->Draw("colz");

// _____________________________________ //
// quick fix:

// copied from eIDsubroutines:
//if(strict == -2 && thetaCC > 45.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/375.0)) return 1;
//if(strict == -1 && thetaCC > 45.5 - 35.0*sqrt(1.0 - pow(relphi,2.0)/362.5)) return 1;
//if(strict == 0 && thetaCC > 46.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/350.0)) return 1; // nominal cut
//if(strict == 1 && thetaCC > 46.5 - 35.0*sqrt(1.0 - pow(relphi,2.0)/337.5)) return 1;
//if(strict == 2 && thetaCC > 47.0 - 35.0*sqrt(1.0 - pow(relphi,2.0)/325.0)) return 1;

can2->cd(2);
TF1 *loose = new TF1("loose", "45.5 - 35.0*sqrt(1.0 - pow(x,2.0)/362.5)", -18, 18);
//loose->Draw("same");
TF1 *nominal = new TF1("nominal", "46.0 - 35.0*sqrt(1.0 - pow(x,2.0)/350.0)", -18.7, 18.7);
nominal->Draw("same");
TF1 *tight = new TF1("tight", "46.5 - 35.0*sqrt(1.0 - pow(x,2.0)/337.5)", -18, 18);
//tight->Draw("same");


can->cd(7);
nominal->Draw("same");
can->cd(8);
nominal->Draw("same");
can->cd(9);
nominal->Draw("same");
can->cd(10);
nominal->Draw("same");
can->cd(11);
nominal->Draw("same");
can->cd(12);
nominal->Draw("same");
}
