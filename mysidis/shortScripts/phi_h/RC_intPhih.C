{
gStyle->SetOptStat(0);

const int NxBins = 5;
const int NQQBins = 2;
float approx_xQQmid[NxBins][NQQBins][2] = {{{0.15, 1.1}, {0.18, 1.4}}, {{0.24, 1.5}, {0.28, 1.8}}, {{0.32, 1.9}, {0.35, 2.4}}, {{0.42, 2.5}, {0.44, 3.1}}, {{0.52, 4.0}, {0.52, 4.0}}};
gROOT->ProcessLine(".L ../PTsqFcn.C");

int NzBins = 18;
float zMin = 0.0;
float zMax = 0.9;
int NPT2Bins = 20;
float PT2Min = 0.0;
float PT2Max = 1.0;

int NphihBins = 36;

TH2F *hPT2Vz[NxBins][NQQBins];
TF1 *MXcutLoose[NxBins][NQQBins];
TF1 *MXcutMid[NxBins][NQQBins];
TF1 *MXcutTight[NxBins][NQQBins];
TF1 *MXcut938[NxBins][NQQBins];

TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 900);
can->Divide(NxBins, NQQBins, 0.00001, 0.00001);

for(int x = 0; x < NxBins; x++)
{
for(int QQ = 0; QQ < NQQBins; QQ++)
{
if(!(x == 4 && QQ == 1))
{
can->cd(NxBins*(1-QQ) + x + 1);
hPT2Vz[x][QQ] = new TH2F(Form("hPT2Vz_%i_%i", x, QQ), Form("P_{h#perp}^{2} vs z, x%i QQ%i", x, QQ), NzBins, zMin, zMax, NPT2Bins, PT2Min, PT2Max);

for(int z = 0; z < NzBins; z++)
{
for(int PT2 = 0; PT2 < NPT2Bins; PT2++)
{

float integrated_sig = 0;
float integrated_sib = 0;
ifstream hapfile(Form("hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", x, QQ, z, PT2)); // hardcoding pip for now
//ifstream hapfile(Form("hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat", x, QQ, z, PT2)); // hardcoding pim for now
if(hapfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
hapfile>>sig>>sib>>tail;

//integrated_sig = integrated_sig + sig - tail; // use if you want to check results w/o tail
integrated_sig = integrated_sig + sig;
integrated_sib = integrated_sib + sib;

}
}
hapfile.close();

//if(integrated_sib > 0.00000001) hPT2Vz[x][QQ]->SetBinContent(z+1, PT2+1, integrated_sig/integrated_sib);
if(integrated_sib > 0.00000001 && !(z == 4 && PT2 > 16)) hPT2Vz[x][QQ]->SetBinContent(z+1, PT2+1, integrated_sig/integrated_sib); // 2 weird pts I don't want to show

}}

hPT2Vz[x][QQ]->GetXaxis()->SetTitle("z");
hPT2Vz[x][QQ]->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hPT2Vz[x][QQ]->GetYaxis()->SetTitleOffset(1.4);
hPT2Vz[x][QQ]->GetZaxis()->SetRangeUser(0.0, 5);
hPT2Vz[x][QQ]->Draw("colz");

MXcutLoose[x][QQ] = new TF1(Form("MXcutLoose%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.1)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), zMin, zMax);
//MXcutLoose[x][QQ]->Draw("same");
MXcutMid[x][QQ] = new TF1(Form("MXcutMid%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.35)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), zMin, zMax);
MXcutMid[x][QQ]->Draw("same");
MXcutTight[x][QQ] = new TF1(Form("MXcutTight%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,1.5)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), zMin, zMax);
//MXcutTight[x][QQ]->Draw("same");
MXcut938[x][QQ] = new TF1(Form("MXcut938%i%i", x, QQ), Form("PTsqFcn(%3.4f,%3.4f,x,0.938)", approx_xQQmid[x][QQ][0], approx_xQQmid[x][QQ][1]), zMin, zMax);
MXcut938[x][QQ]->Draw("same");

}
}}



}
