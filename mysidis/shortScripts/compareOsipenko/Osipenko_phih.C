{
gStyle->SetOptStat(0);

int xBin = 8;
int QQBin = 3;
int zBin = 10;
int PT2Bin = 1;

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

TH1F *hphih = new TH1F("hphih", "hphih", NphihBins, -180, 180);
TH1F *hphih_sys = new TH1F("hphih_sys", "hphih_sys", NphihBins, -180, 180);
TH1F *hphih_RC = new TH1F("hphih_RC", "hphih_RC", NphihBins, -180, 180);

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

if(thisxBin == xBin && thisQQBin == QQBin && thiszBin == zBin && thisPT2Bin == PT2Bin)
{
hphih->SetBinContent(thisphihBin_prime + 1, CS);
hphih->SetBinError(thisphihBin_prime + 1, stat_e);

float sysYval = 0.0;
hphih_sys->SetBinContent(thisphihBin_prime + 1, sysYval);
hphih_sys->SetBinError(thisphihBin_prime + 1, 0.5*sys_e); // factor of 0.5 since error bar goes up and down... want the full height to = sys_e

hphih_RC->SetBinContent(thisphihBin_prime + 1, RC);
hphih_RC->SetBinError(thisphihBin_prime + 1, 0);
}

}
infile.close();

float annoyingMin = -0.15*hphih->GetMaximum(); // if you put these expressions right into SetRangeUser, it doesn't work correctly... annoying
float annoyingMax = 1.25*hphih->GetMaximum();
hphih->GetYaxis()->SetRangeUser(annoyingMin, annoyingMax);
hphih->Draw("E");
hphih_sys->SetFillColor(kBlue);
//hphih_sys->SetFillStyle(3001);
hphih_sys->Draw("E3 same");

TF1 *ff = new TF1("ff", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ff->SetLineColor(kBlue);
ff->SetParameters(hphih->GetMaximum(), 0.0, 0.0);
ff->SetParLimits(1, -0.99, 0.99);
ff->SetParLimits(2, -0.99, 0.99);
if(hphih->Integral() > 0) hphih->Fit("ff", "", "", -180, 180);

TLatex *tlat = new TLatex();
tlat->SetNDC();
tlat->SetTextSize(0.05);
tlat->DrawLatex(0.15, 0.34, Form("A0 = %3.4f #pm %3.4f", ff->GetParameter(0), ff->GetParError(0)));
tlat->DrawLatex(0.15, 0.28, Form("Ac = %3.4f #pm %3.4f", ff->GetParameter(1), ff->GetParError(1)));
tlat->DrawLatex(0.15, 0.22, Form("Acc = %3.4f #pm %3.4f", ff->GetParameter(2), ff->GetParError(2)));

new TCanvas;
hphih_RC->GetYaxis()->SetRangeUser(0, 1.2*hphih_RC->GetMaximum());
hphih_RC->Draw();

}
