// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void MAcAccVPT2_modelDependence(int xBin = 1, int QQBin = 1, int zBin = 7, string pipORpim = "pip", string compareModel = "acc") // compareModel = "acc" or "hap"
{
gStyle->SetOptStat(0);

string YscaleOpt = "smart"; // "fixed" or "smart"

if(compareModel == "acc")
{
const int totalNits = 3; // number of different things to compare
int accItN[totalNits] = {0, 1, 2};
int hapItN[totalNits] = {2, 2, 2};
int iMarkerStyle[totalNits] = {4, 20, 5};
int icolor[totalNits] = {kBlack, kBlue, kRed};
}

if(compareModel == "hap")
{
const int totalNits = 3; // number of different things to compare
int accItN[totalNits] = {2, 2, 2};
int hapItN[totalNits] = {0, 1, 2}; // could also add a "9" to this list to compare results without any RC
int iMarkerStyle[totalNits] = {4, 20, 5};
int icolor[totalNits] = {kBlack, kBlue, kRed};
}

const int NPT2Bins = 20;
float PT2Min = 0;
float PT2Max = 1;

//TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 600);
//TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 300);
TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 500);
can->Divide(3, 1, 0.00001, 0.00001);

TH1F *hM[totalNits], *hAc[totalNits], *hAcc[totalNits];

for(int i = 0; i < totalNits; i++)
{
hM[i] = new TH1F(Form("hM_it%i_%i", accItN[i], hapItN[i]), Form("hM_it%i_%i", accItN[i], hapItN[i]), NPT2Bins, PT2Min, PT2Max);
hAc[i] = new TH1F(Form("hAc_it%i_%i", accItN[i], hapItN[i]), Form("hAc_it%i_%i", accItN[i], hapItN[i]), NPT2Bins, PT2Min, PT2Max);
hAcc[i] = new TH1F(Form("hAcc_it%i_%i", accItN[i], hapItN[i]), Form("hAcc_it%i_%i", accItN[i], hapItN[i]), NPT2Bins, PT2Min, PT2Max);

for(int k = 0; k < NPT2Bins; k++)
{
ifstream Mfile(Form("/scratch/MAcAccfiles/%s_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN[i], hapItN[i], xBin, QQBin, zBin, k));
ifstream AcAccfile(Form("/scratch/MAcAccfiles/%s_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN[i], hapItN[i], xBin, QQBin, zBin, k));

float M, Me, Ac, Ace, Acc, Acce;

if(Mfile)
{
Mfile>>M>>Me;

hM[i]->SetBinContent(k+1, M);
hM[i]->SetBinError(k+1, Me);
}

if(AcAccfile)
{
AcAccfile>>Ac>>Ace>>Acc>>Acce;

hAc[i]->SetBinContent(k+1, Ac);
hAc[i]->SetBinError(k+1, Ace);
hAcc[i]->SetBinContent(k+1, Acc);
hAcc[i]->SetBinError(k+1, Acce);
}

Mfile.close();
AcAccfile.close();
}

/// Draw: ///
can->cd(1)->SetGrid();
//can->cd(1)->SetLogy();
hM[i]->SetLineColor(icolor[i]);
hM[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hM[i]->GetYaxis()->SetRangeUser(10, 1.2*hM[i]->GetMaximum());
if(YscaleOpt == "fixed") hM[i]->GetYaxis()->SetRangeUser(10, 30000);
hM[i]->SetTitle(Form("Multiplicity, %s, x%i QQ%i z%i", pipORpim.c_str(), xBin, QQBin, zBin));
hM[i]->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hM[i]->Draw("same E1");

can->cd(2)->SetGrid();
hAc[i]->SetLineColor(icolor[i]);
hAc[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hAc[i]->GetYaxis()->SetRangeUser(hAc[i]->GetMinimum() - 0.1, hAc[i]->GetMaximum() + 0.1);
if(YscaleOpt == "fixed") hAc[i]->GetYaxis()->SetRangeUser(-0.35, 0.25);
hAc[i]->SetTitle(Form("A^{cos#phi}, %s, x%i QQ%i z%i", pipORpim.c_str(), xBin, QQBin, zBin));
hAc[i]->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hAc[i]->Draw("same E1");

can->cd(3)->SetGrid();
hAcc[i]->SetLineColor(icolor[i]);
hAcc[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hAcc[i]->GetYaxis()->SetRangeUser(hAcc[i]->GetMinimum() - 0.1, hAcc[i]->GetMaximum() + 0.1);
if(YscaleOpt == "fixed") hAcc[i]->GetYaxis()->SetRangeUser(-0.15, 0.35);
hAcc[i]->SetTitle(Form("A^{cos2#phi}, %s, x%i QQ%i z%i", pipORpim.c_str(), xBin, QQBin, zBin));
hAcc[i]->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hAcc[i]->Draw("same E1");

}

can->cd(1);
TLegend *leg = new TLegend(0.55, 0.65, 0.9, 0.9);
if(compareModel == "acc") leg->SetHeader("Acceptance Model:");
if(compareModel == "hap") leg->SetHeader("RC Model:");
leg->AddEntry(hM[0], "flat", "lp");
leg->AddEntry(hM[1], "default", "lp");
leg->AddEntry(hM[2], "my model", "lp");
leg->Draw();

}
