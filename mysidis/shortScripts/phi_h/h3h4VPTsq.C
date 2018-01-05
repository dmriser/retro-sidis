// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void h3h4VPTsq(int xBin = 0, int QQBin = 0, int zBin = 5)
{
gStyle->SetOptStat(0);

string YscaleOpt = "smart"; // "fixed" or "smart"

// this is BiSc5:
const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
const int NPT2Bins = 20;
float xpts[NxBins] = {0.15, 0.25, 0.35, 0.45, 0.55}; // center values
float QQpts[NxBins][NQQBins] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}}; // approx. center values
float zpts[NzBins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875}; // center values
float PT2pts[NPT2Bins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.925, 0.975}; // center values

const int NpipPT2Bins = 20; // redundant but okay
float pipPT2Min = 0;
float pipPT2Max = 1;
const int NpimPT2Bins = 20;
float pimPT2Min = 0;
float pimPT2Max = 1;

TF3 *h3_vs_xQQz = new TF3("h3_vs_xQQz", "[0]*pow(x, [1])*pow(1.0 - x, [2])*pow(z, [3])*pow(1.0 - z, [4])*pow(log(y/[7])/log([6]/[7]), [5])", 0.05, 0.65, 0.5, 5.0, 0, 1); // fcn from haprad
h3_vs_xQQz->SetParName(0, "a");
h3_vs_xQQz->SetParName(1, "a1");
h3_vs_xQQz->SetParName(2, "a2");
h3_vs_xQQz->SetParName(3, "b1");
h3_vs_xQQz->SetParName(4, "b2");
h3_vs_xQQz->SetParName(5, "bb");
h3_vs_xQQz->SetParName(6, "q0");
h3_vs_xQQz->SetParName(7, "lambdaSquared");
h3_vs_xQQz->FixParameter(6, 1.0); // parameters from haprad
h3_vs_xQQz->FixParameter(7, 0.25*0.25);
h3_vs_xQQz->SetParameter("a", -0.36544E-03);
h3_vs_xQQz->SetParameter("a1", -2.1855);
h3_vs_xQQz->SetParameter("a2", 3.4176);
h3_vs_xQQz->SetParameter("b1", -1.7567);
h3_vs_xQQz->SetParameter("b2", 1.1272);
h3_vs_xQQz->SetParameter("bb", 8.9985);
float h3default = h3_vs_xQQz->Eval(xpts[xBin], QQpts[xBin][QQBin], zpts[zBin]); // no PTsq dependence so just a constant here

// add h4 fcn next

//const int totalNits = 5; // number of acceptance iterations plus number of RC iterations... 4 (0, 1, 2, 3) + 1 (default haprad) = 5 for now
//string RCoptString[totalNits] = {"", "", "", "", "wRC"};
//int accItN[totalNits] = {0, 1, 2, 3, 3};
//int iMarkerStyle[totalNits] = {4, 20, 5, 22, 21};

const int totalNits = 1; // number of acceptance iterations plus number of RC iterations... 4 (0, 1, 2, 3) + 1 (default haprad) = 5 for now
string RCoptString[totalNits] = {""};
int accItN[totalNits] = {0};
int iMarkerStyle[totalNits] = {4};

TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 600);
can->Divide(2, 1, 0.00001, 0.00001);

TH1F *hpipAc[totalNits], *hpipAcc[totalNits];
TH1F *hpimAc[totalNits], *hpimAcc[totalNits];

for(int i = 0; i < totalNits; i++)
{
hpiph3[i] = new TH1F(Form("hpiph3_it%i%s", accItN[i], RCoptString[i].c_str()), Form("hpiph3_it%i%s", accItN[i], RCoptString[i].c_str()), NpipPT2Bins, pipPT2Min, pipPT2Max);
hpiph4[i] = new TH1F(Form("hpiph4_it%i%s", accItN[i], RCoptString[i].c_str()), Form("hpiph4_it%i%s", accItN[i], RCoptString[i].c_str()), NpipPT2Bins, pipPT2Min, pipPT2Max);
hpimh3[i] = new TH1F(Form("hpimh3_it%i%s", accItN[i], RCoptString[i].c_str()), Form("hpimh3_it%i%s", accItN[i], RCoptString[i].c_str()), NpimPT2Bins, pimPT2Min, pimPT2Max);
hpimh4[i] = new TH1F(Form("hpimh4_it%i%s", accItN[i], RCoptString[i].c_str()), Form("hpimh4_it%i%s", accItN[i], RCoptString[i].c_str()), NpimPT2Bins, pimPT2Min, pimPT2Max);

/// pip: ///
for(int k = 0; k < NpipPT2Bins; k++)
{
ifstream piph3h4file(Form("/scratch/h3h4files/term0h3h4%s_pip_it%i_BiSc5_x%iQQ%iz%iPTsq%i.txt", RCoptString[i].c_str(), accItN[i], xBin, QQBin, zBin, k));

float term0, piph3, piph4;

if(piph3h4file)
{
piph3h4file>>term0>>piph3>>piph4;

hpiph3[i]->SetBinContent(k+1, piph3);
hpiph4[i]->SetBinContent(k+1, piph4);
}

piph3h4file.close();
}

/// pim: ///
for(int k = 0; k < NpimPT2Bins; k++)
{
ifstream pimh3h4file(Form("/scratch/h3h4files/term0h3h4%s_pim_it%i_BiSc5_x%iQQ%iz%iPTsq%i.txt", RCoptString[i].c_str(), accItN[i], xBin, QQBin, zBin, k));

float term0, pimh3, pimh4;

if(pimh3h4file)
{
pimh3h4file>>term0>>pimh3>>pimh4;

hpimh3[i]->SetBinContent(k+1, pimh3);
hpimh4[i]->SetBinContent(k+1, pimh4);
}

pimh3h4file.close();
}


/// Draw: ///
can->cd(1)->SetGrid();
hpiph3[i]->SetMarkerColor(kRed);
hpiph3[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hpiph3[i]->GetYaxis()->SetRangeUser(min(hpiph3[i]->GetMinimum(), hpimh3[i]->GetMinimum()) - 0.1, max(hpiph3[i]->GetMaximum(), hpimh3[i]->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpiph3[i]->GetYaxis()->SetRangeUser(-0.35, 0.25);
hpiph3[i]->SetTitle(Form("h3 for #pi+ (red) and #pi- (blue), x%iQQ%iz%i", xBin, QQBin, zBin));
hpiph3[i]->GetXaxis()->SetTitle("P_{T}^{2} (GeV^{2})");
hpiph3[i]->Draw("same p");
hpimh3[i]->SetMarkerColor(kBlue);
hpimh3[i]->SetMarkerStyle(iMarkerStyle[i]);
hpimh3[i]->Draw("same p");

TLine *line = new TLine(0, h3default, 1, h3default);
line->SetLineWidth(2);
line->SetLineColor(kBlack);
line->Draw();

can->cd(2)->SetGrid();
hpiph4[i]->SetMarkerColor(kRed);
hpiph4[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hpiph4[i]->GetYaxis()->SetRangeUser(min(hpiph4[i]->GetMinimum(), hpimh4[i]->GetMinimum()) - 0.1, max(hpiph4[i]->GetMaximum(), hpimh4[i]->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpiph4[i]->GetYaxis()->SetRangeUser(-0.15, 0.35);
hpiph4[i]->SetTitle(Form("h4 for #pi+ (red) and #pi- (blue), x%iQQ%iz%i", xBin, QQBin, zBin));
hpiph4[i]->GetXaxis()->SetTitle("P_{T}^{2} (GeV^{2})");
hpiph4[i]->Draw("same p");
hpimh4[i]->SetMarkerColor(kBlue);
hpimh4[i]->SetMarkerStyle(iMarkerStyle[i]);
hpimh4[i]->Draw("same p");

}

//can->cd(1);
//TLegend *leg = new TLegend(0.55, 0.65, 0.9, 0.9);
//leg->AddEntry(hpipM[0], "acc. it. 0, no RC", "p");
//leg->AddEntry(hpipM[1], "acc. it. 1", "p");
//leg->AddEntry(hpipM[2], "acc. it. 2", "p");
//leg->AddEntry(hpipM[3], "acc. it. 3", "p");
//leg->AddEntry(hpipM[4], "acc. it. 3 w/ RC", "p");
//leg->Draw();


}
