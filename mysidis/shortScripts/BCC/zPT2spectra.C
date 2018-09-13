{
gStyle->SetOptStat(0);
TFile *tf = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");

gROOT->ProcessLine(".L ../PTsqFcn.C");

string genORrec = "gen"; // don't change this! BCC uses only generated MC!
string pipORpim = "pip";

// BiSc5:
const int NxBins = 5;
//float xLimits[NxBins + 1] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
const int NQQBins = 2;
//float QQLimits[NxBins][NQQBins + 1] = {{1.0, 1.3, 5.0}, {1.0, 1.7, 5.0}, {1.0, 2.2, 5.0}, {1.0, 2.9, 5.0}, {1.0, 5.0, 99.9}};
const int NzBins = 18;
float zLimits[NzBins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90};
const int NPT2Bins = 20;
float PT2Limits[NPT2Bins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00};

//float approx_xQQmid[NxBins][NQQBins][2] = {{{0.15, 1.1}, {0.15, 1.4}}, {{0.25, 1.5}, {0.25, 1.8}}, {{0.35, 1.9}, {0.35, 2.4}}, {{0.45, 2.5}, {0.45, 3.1}}, {{0.55, 4.0}, {0.55, 4.0}}};
float approx_xQQmid[NxBins][NQQBins][2] = {{{0.15, 1.1}, {0.18, 1.4}}, {{0.24, 1.5}, {0.28, 1.8}}, {{0.32, 1.9}, {0.35, 2.4}}, {{0.42, 2.5}, {0.44, 3.1}}, {{0.52, 4.0}, {0.52, 4.0}}};

//-------------------------------------------------

TCanvas *can2 = new TCanvas("can2", "can2", 20, 20, 1500, 900);
can2->Divide(NxBins, NQQBins, 0.00001, 0.00001);

TH2F *PT2vsz_xQQbins[NxBins][NQQBins];
TF1 *MXcutLoose[NxBins][NQQBins];
TF1 *MXcutMid[NxBins][NQQBins];
TF1 *MXcutTight[NxBins][NQQBins];
TF1 *MXcut938[NxBins][NQQBins];
TLine *PT2Line[NPT2Bins + 1];
TLine *zLine[NzBins + 1];

for(int x = 0; x < NxBins; x++)
{
for(int QQ = 0; QQ < NQQBins; QQ++)
{
if(!(x == 4 && QQ == 1))
{
can2->cd(NxBins*(NQQBins-1) - NxBins*QQ + x + 1)->SetLogz();

PT2vsz_xQQbins[x][QQ] = (TH2F*) tf->Get(Form("%s_%s_PT2vsz_x%iQQ%i", genORrec.c_str(), pipORpim.c_str(), x, QQ));
//PT2vsz_xQQbins[x][QQ] = (TH2F*) tf->Get(Form("%s_%s_PT2vsz_x%iQQ%iphihB0", genORrec.c_str(), pipORpim.c_str(), x, QQ));
PT2vsz_xQQbins[x][QQ]->SetTitle("");
PT2vsz_xQQbins[x][QQ]->GetXaxis()->SetTitle("z");
PT2vsz_xQQbins[x][QQ]->GetYaxis()->SetTitle("P_{h #perp}^{2} (GeV)");
PT2vsz_xQQbins[x][QQ]->Draw("colz");

for(int k = 0; k < NzBins + 1; k++)
{
zLine[k] = new TLine(zLimits[k], PT2Limits[0], zLimits[k], PT2Limits[NPT2Bins]);
zLine[k]->SetLineColor(kGray);
zLine[k]->Draw();
}
for(int k = 0; k < NPT2Bins + 1; k++)
{
PT2Line[k] = new TLine(zLimits[0], PT2Limits[k], zLimits[NzBins], PT2Limits[k]);
PT2Line[k]->SetLineColor(kGray);
PT2Line[k]->Draw();
}

MXcutLoose[x][QQ] = new TF1(Form("MXcutLoose%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.1)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), 0, 1.1);
MXcutLoose[x][QQ]->Draw("same");
MXcutMid[x][QQ] = new TF1(Form("MXcutMid%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.35)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), 0, 1.1);
MXcutMid[x][QQ]->Draw("same");
MXcutTight[x][QQ] = new TF1(Form("MXcutTight%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.5)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), 0, 1.1);
MXcutTight[x][QQ]->Draw("same");
MXcut938[x][QQ] = new TF1(Form("MXcut938%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,0.938)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), 0, 1.1);
MXcut938[x][QQ]->Draw("same");

//-------------------------------------------------------------------------------------//
//---------------------- now draw sub-bins --------------------------------------------//

float zLow[NzBins], zHigh[NzBins], PT2Low[NPT2Bins], PT2High[NPT2Bins];

zLow[0] = 0.025 - 0.005;
zLow[1] = 0.075 - 0.005;
zLow[2] = 0.125 - 0.005;
zLow[3] = 0.175 - 0.005;
zLow[4] = 0.225 - 0.005;
zLow[5] = 0.275 - 0.005;
zLow[6] = 0.325 - 0.005;
zLow[7] = 0.375 - 0.005;
zLow[8] = 0.425 - 0.005;
zLow[9] = 0.475 - 0.005;
zLow[10] = 0.525 - 0.005;
zLow[11] = 0.575 - 0.005;
zLow[12] = 0.625 - 0.005;
zLow[13] = 0.675 - 0.005;
zLow[14] = 0.725 - 0.005;
zLow[15] = 0.775 - 0.005;
zLow[16] = 0.825 - 0.005;
zLow[17] = 0.875 - 0.005;

zHigh[0] = 0.025 + 0.005;
zHigh[1] = 0.075 + 0.005;
zHigh[2] = 0.125 + 0.005;
zHigh[3] = 0.175 + 0.005;
zHigh[4] = 0.225 + 0.005;
zHigh[5] = 0.275 + 0.005;
zHigh[6] = 0.325 + 0.005;
zHigh[7] = 0.375 + 0.005;
zHigh[8] = 0.425 + 0.005;
zHigh[9] = 0.475 + 0.005;
zHigh[10] = 0.525 + 0.005;
zHigh[11] = 0.575 + 0.005;
zHigh[12] = 0.625 + 0.005;
zHigh[13] = 0.675 + 0.005;
zHigh[14] = 0.725 + 0.005;
zHigh[15] = 0.775 + 0.005;
zHigh[16] = 0.825 + 0.005;
zHigh[17] = 0.875 + 0.005;

PT2Low[0] = 0.025 - 0.005;
PT2Low[1] = 0.075 - 0.005;
PT2Low[2] = 0.125 - 0.005;
PT2Low[3] = 0.175 - 0.005;
PT2Low[4] = 0.225 - 0.005;
PT2Low[5] = 0.275 - 0.005;
PT2Low[6] = 0.325 - 0.005;
PT2Low[7] = 0.375 - 0.005;
PT2Low[8] = 0.425 - 0.005;
PT2Low[9] = 0.475 - 0.005;
PT2Low[10] = 0.525 - 0.005;
PT2Low[11] = 0.575 - 0.005;
PT2Low[12] = 0.625 - 0.005;
PT2Low[13] = 0.675 - 0.005;
PT2Low[14] = 0.725 - 0.005;
PT2Low[15] = 0.775 - 0.005;
PT2Low[16] = 0.825 - 0.005;
PT2Low[17] = 0.875 - 0.005;
PT2Low[18] = 0.925 - 0.005;
PT2Low[19] = 0.975 - 0.005;

PT2High[0] = 0.025 + 0.005;
PT2High[1] = 0.075 + 0.005;
PT2High[2] = 0.125 + 0.005;
PT2High[3] = 0.175 + 0.005;
PT2High[4] = 0.225 + 0.005;
PT2High[5] = 0.275 + 0.005;
PT2High[6] = 0.325 + 0.005;
PT2High[7] = 0.375 + 0.005;
PT2High[8] = 0.425 + 0.005;
PT2High[9] = 0.475 + 0.005;
PT2High[10] = 0.525 + 0.005;
PT2High[11] = 0.575 + 0.005;
PT2High[12] = 0.625 + 0.005;
PT2High[13] = 0.675 + 0.005;
PT2High[14] = 0.725 + 0.005;
PT2High[15] = 0.775 + 0.005;
PT2High[16] = 0.825 + 0.005;
PT2High[17] = 0.875 + 0.005;
PT2High[18] = 0.925 + 0.005;
PT2High[19] = 0.975 + 0.005;

for(int z = 0; z < NzBins; z++) {
for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {

TLine *v1 = new TLine(zLow[z], PT2Low[PT2], zLow[z], PT2High[PT2]);
v1->Draw();
TLine *v2 = new TLine(zHigh[z], PT2Low[PT2], zHigh[z], PT2High[PT2]);
v2->Draw();
TLine *h1 = new TLine(zLow[z], PT2Low[PT2], zHigh[z], PT2Low[PT2]);
h1->Draw();
TLine *h2 = new TLine(zLow[z], PT2High[PT2], zHigh[z], PT2High[PT2]);
h2->Draw();

}}
//-------------------------------------------------------------------------------------//

}
}}

}
