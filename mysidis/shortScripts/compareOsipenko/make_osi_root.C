{
gROOT->ProcessLine(".L /home/kjgroup/mysidis/programFiles/functions.C");

ifstream infile("pip_sidis_cs_table_zh_pt2.dat");
int Nentries = 112566; // number of rows in the data table

string dummyString = "";
for(int dummyInt = 1; dummyInt < 27; dummyInt++) infile>>dummyString;

const int NxBins = 19;
const int NQQBins = 10;
const int NzBins = 22;
const int NPT2Bins = 10;
float xLimits[NxBins+1] = {0.128, 0.161, 0.1905, 0.221, 0.2525, 0.2855, 0.32, 0.356, 0.394, 0.4335, 0.4745, 0.5175, 0.5625, 0.6095, 0.6585, 0.7095, 0.763, 0.8185, 0.8765, 0.9425};
float QQLimits[NQQBins+1] = {1.38, 1.62, 1.88, 2.19, 2.65, 3.18, 3.76, 4.47, 5.28, 6.165, 7.05};
float zLimits[NzBins+1] = {0.059, 0.081, 0.1065, 0.133, 0.161, 0.1905, 0.221, 0.2525, 0.2855, 0.32, 0.356, 0.394, 0.4335, 0.4745, 0.5175, 0.5625, 0.6095, 0.6585, 0.7095, 0.763, 0.8185, 0.8765, 0.93};
float PT2Limits[NPT2Bins+1] = {0.0, 0.0153, 0.0459, 0.0972, 0.1728, 0.279, 0.4239, 0.6246, 0.9081, 1.314, 1.75};
int NphihBins = 18;

TH1F *hphih[NxBins][NQQBins][NzBins][NPT2Bins];
TH1F *hphih_sys[NxBins][NQQBins][NzBins][NPT2Bins];
TH1F *hphih_RC[NxBins][NQQBins][NzBins][NPT2Bins];

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {
for(int iz = 0; iz < NzBins; iz++) {
for(int iPT2 = 0; iPT2 < NPT2Bins; iPT2++) {
hphih[ix][iQQ][iz][iPT2] = new TH1F(Form("hphih_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), Form("hphih_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), NphihBins, -180, 180);
hphih[ix][iQQ][iz][iPT2]->Sumw2();
hphih_sys[ix][iQQ][iz][iPT2] = new TH1F(Form("hphih_sys_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), Form("hphih_sys_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), NphihBins, -180, 180);
hphih_sys[ix][iQQ][iz][iPT2]->Sumw2();
hphih_RC[ix][iQQ][iz][iPT2] = new TH1F(Form("hphih_RC_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), Form("hphih_RC_x%iQQ%iz%iPT2%i", ix, iQQ, iz, iPT2), NphihBins, -180, 180);
hphih_RC[ix][iQQ][iz][iPT2]->Sumw2();
}}}}

for(int k = 0; k < Nentries; k++)
{
float QQ, x, z, PT2, phih, CS, stat_e, sys_e, RC;
infile>>QQ>>x>>z>>PT2>>phih>>CS>>stat_e>>sys_e>>RC;

int thisxBin = getBinN(x, NxBins+1, xLimits);
int thisQQBin = getBinN(QQ, NQQBins+1, QQLimits);
int thiszBin = getBinN(z, NzBins+1, zLimits);
int thisPT2Bin = getBinN(PT2, NPT2Bins+1, PT2Limits);
int thisphihBin = getBinN2(phih, NphihBins, 0, 360);
int thisphihBin_prime; // to convert from 0-360 convention to -180-180 convention
if(thisphihBin < NphihBins/2) thisphihBin_prime = thisphihBin + NphihBins/2;
if(thisphihBin >= NphihBins/2) thisphihBin_prime = thisphihBin - NphihBins/2;

if(thisxBin > -1 && thisQQBin > -1 && thiszBin > -1 && thisPT2Bin > -1 && thisphihBin > -1)
{
hphih[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinContent(thisphihBin_prime + 1, CS);
hphih[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinError(thisphihBin_prime + 1, stat_e);

float sysYval = 0.0;
hphih_sys[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinContent(thisphihBin_prime + 1, sysYval);
hphih_sys[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinError(thisphihBin_prime + 1, 0.5*sys_e); // factor of 0.5 since error bar goes up and down... want the full height to = sys_e

hphih_RC[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinContent(thisphihBin_prime + 1, RC);
hphih_RC[thisxBin][thisQQBin][thiszBin][thisPT2Bin]->SetBinError(thisphihBin_prime + 1, 0);
}
}
infile.close();

TFile *outfile = new TFile("pip_sidis_cs_table_zh_pt2.root", "recreate");

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {
for(int iz = 0; iz < NzBins; iz++) {
for(int iPT2 = 0; iPT2 < NPT2Bins; iPT2++) {
hphih[ix][iQQ][iz][iPT2]->Write();
hphih_sys[ix][iQQ][iz][iPT2]->Write();
hphih_RC[ix][iQQ][iz][iPT2]->Write();
}}}}

outfile->Write();
outfile->Close();

}
