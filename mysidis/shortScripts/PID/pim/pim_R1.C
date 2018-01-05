void pim_R1(int sector = 4)
{
gStyle->SetOptStat(0);

TFile *fD = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");
TFile *fMC = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");

TH2F *hD[2][6];
TH2F *hMC[2][6];

const int Nstrictnesses = 3;
float height[Nstrictnesses] = {19, 20, 21};
float angle[Nstrictnesses] = {81, 80, 79};
float horizontal[Nstrictnesses] = {23.5, 24, 24.5};

float slope[Nstrictnesses];

TF1 *L1[Nstrictnesses], *L2[Nstrictnesses], *L3[Nstrictnesses];

for(int st = 0; st < Nstrictnesses; st++)
{
slope[st] = 1.0/tan(0.5*(3.141592653/180.0)*angle[st]);
L1[st] = new TF1(Form("L1%i", st), Form("%3.4f + %3.4f*x", height[st], slope[st]), 0, 60);
L2[st] = new TF1(Form("L2%i", st), Form("%3.4f - %3.4f*x", height[st], slope[st]), -60, 0);
L3[st] = new TF1(Form("L3%i", st), Form("%3.4f + 0.0*x", horizontal[st]), -20, 20);
}

TCanvas *can = new TCanvas("can", "can", 1500, 60, 1600, 1000);
can->Divide(6, 4, 0.00001, 0.00001);

for(int c = 0; c < 2; c++)
{
for(int s = 0; s < 6; s++)
{
can->cd(c*6 + s + 1)->SetLogz();
hD[c][s] = (TH2F*) fD->Get(Form("pimIDplots/tl1_xVtl1_y_pim_hist_c%i_s%i", c, s+1));
hD[c][s]->GetXaxis()->SetTitle("Y (cm)");
hD[c][s]->GetYaxis()->SetTitle("X (cm)");
hD[c][s]->Draw("colz");
if(c==1)
{
for(int st = 0; st < Nstrictnesses; st++)
{
L1[st]->Draw("same");
L2[st]->Draw("same");
L3[st]->Draw("same");
}
}

can->cd(12 + c*6 + s + 1)->SetLogz();
hMC[c][s] = (TH2F*) fMC->Get(Form("pimIDplots/tl1_xVtl1_y_pim_hist_c%i_s%i", c, s+1));
hMC[c][s]->GetXaxis()->SetTitle("Y (cm)");
hMC[c][s]->GetYaxis()->SetTitle("X (cm)");
hMC[c][s]->Draw("colz");
if(c==1)
{
for(int st = 0; st < Nstrictnesses; st++)
{
L1[st]->Draw("same");
L2[st]->Draw("same");
L3[st]->Draw("same");
}
}
}
}

// _____________________________________ //

TCanvas *can2 = new TCanvas("can2", "can2", 60, 60, 1000, 800);
can2->Divide(2, 2, 0.00001, 0.00001);

can2->cd(1)->SetLogz();
hD[0][sector-1]->Draw("colz");
can2->cd(2)->SetLogz();
hD[1][sector-1]->Draw("colz");
for(int st = 0; st < Nstrictnesses; st++)
{
L1[st]->Draw("same");
L2[st]->Draw("same");
L3[st]->Draw("same");
}

can2->cd(3)->SetLogz();
hMC[0][sector-1]->Draw("colz");
can2->cd(4)->SetLogz();
hMC[1][sector-1]->Draw("colz");
for(int st = 0; st < Nstrictnesses; st++)
{
L1[st]->Draw("same");
L2[st]->Draw("same");
L3[st]->Draw("same");
}

//can2->SaveAs(Form("pim_R1_s%i.png", sector));

}
