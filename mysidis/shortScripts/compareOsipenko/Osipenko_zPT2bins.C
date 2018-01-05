{
gStyle->SetOptStat(0);

gROOT->ProcessLine(".L /home/kjgroup/mysidis/programFiles/functions.C");

ifstream infile("pip_sidis_cs_table_zh_pt2.dat");
int Nentries = 112566; // number of rows in the data table

string dummyString = "";
for(int dummyInt = 1; dummyInt < 27; dummyInt++) infile>>dummyString;

const int NxBins = 19;
const int NQQBins = 10;
float xLimits[NxBins+1] = {0.128, 0.161, 0.1905, 0.221, 0.2525, 0.2855, 0.32, 0.356, 0.394, 0.4335, 0.4745, 0.5175, 0.5625, 0.6095, 0.6585, 0.7095, 0.763, 0.8185, 0.8765, 0.9425};
float QQLimits[NQQBins+1] = {1.38, 1.62, 1.88, 2.19, 2.65, 3.18, 3.76, 4.47, 5.28, 6.165, 7.05};

TH2F *hPT2vz[NxBins][NQQBins];
for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {
hPT2vz[ix][iQQ] = new TH2F(Form("hPT2vz_x%i_QQ%i", ix, iQQ), Form("hPT2vz_x%i_QQ%i", ix, iQQ), 150, 0, 1, 150, 0, 1.8);
}}

TH2F *hPT2vz_int = new TH2F("hPT2vz_int", "hPT2vz_int", 150, 0, 1, 150, 0, 1.8);
TH1F *hz = new TH1F("hz", "hz", 1000, 0, 1);
TH1F *hPT2 = new TH1F("hPT2", "hPT2", 1000, 0, 1.8);

for(int k = 0; k < Nentries; k++)
{
float QQ, x, z, PT2, phih, CS, stat_e, sys_e, RC;
infile>>QQ>>x>>z>>PT2>>phih>>CS>>stat_e>>sys_e>>RC;

int xBin = getBinN(x, NxBins+1, xLimits);
int QQBin = getBinN(QQ, NQQBins+1, QQLimits);

if(xBin > -1 && QQBin > -1) hPT2vz[xBin][QQBin]->Fill(z, PT2);
hPT2vz_int->Fill(z, PT2);
hz->Fill(z);
hPT2->Fill(PT2);
}
infile.close();


TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 900);
can->Divide(NxBins, NQQBins, 0, 0);

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {
can->cd(NxBins*(NQQBins-1) - NxBins*iQQ + ix + 1);
if(hPT2vz[ix][iQQ]->GetEntries() > 0.5) hPT2vz[ix][iQQ]->Draw("colz");
}}

new TCanvas;
hPT2vz_int->Draw("colz");

TLine *vline0 = new TLine(0.059, 0, 0.059, 1.8);
vline0->Draw();
TLine *vline1 = new TLine(0.081, 0, 0.081, 1.8);
vline1->Draw();
TLine *vline2 = new TLine(0.1065, 0, 0.1065, 1.8);
vline2->Draw();
TLine *vline3 = new TLine(0.133, 0, 0.133, 1.8);
vline3->Draw();
TLine *vline4 = new TLine(0.161, 0, 0.161, 1.8);
vline4->Draw();
TLine *vline5 = new TLine(0.1905, 0, 0.1905, 1.8);
vline5->Draw();
TLine *vline6 = new TLine(0.221, 0, 0.221, 1.8);
vline6->Draw();
TLine *vline7 = new TLine(0.2525, 0, 0.2525, 1.8);
vline7->Draw();
TLine *vline8 = new TLine(0.2855, 0, 0.2855, 1.8);
vline8->Draw();
TLine *vline9 = new TLine(0.32, 0, 0.32, 1.8);
vline9->Draw();
TLine *vline10 = new TLine(0.356, 0, 0.356, 1.8);
vline10->Draw();
TLine *vline11 = new TLine(0.394, 0, 0.394, 1.8);
vline11->Draw();
TLine *vline12 = new TLine(0.4335, 0, 0.4335, 1.8);
vline12->Draw();
TLine *vline13 = new TLine(0.4745, 0, 0.4745, 1.8);
vline13->Draw();
TLine *vline14 = new TLine(0.5175, 0, 0.5175, 1.8);
vline14->Draw();
TLine *vline15 = new TLine(0.5625, 0, 0.5625, 1.8);
vline15->Draw();
TLine *vline16 = new TLine(0.6095, 0, 0.6095, 1.8);
vline16->Draw();
TLine *vline17 = new TLine(0.6585, 0, 0.6585, 1.8);
vline17->Draw();
TLine *vline18 = new TLine(0.7095, 0, 0.7095, 1.8);
vline18->Draw();
TLine *vline19 = new TLine(0.763, 0, 0.763, 1.8);
vline19->Draw();
TLine *vline20 = new TLine(0.8185, 0, 0.8185, 1.8);
vline20->Draw();
TLine *vline21 = new TLine(0.8765, 0, 0.8765, 1.8);
vline21->Draw();
TLine *vline22 = new TLine(0.93, 0, 0.93, 1.8);
vline22->Draw();

TLine *hline0 = new TLine(0, 0.0, 1.0, 0.0);
hline0->Draw();
TLine *hline1 = new TLine(0, 0.0153, 1.0, 0.0153);
hline1->Draw();
TLine *hline2 = new TLine(0, 0.0459, 1.0, 0.0459);
hline2->Draw();
TLine *hline3 = new TLine(0, 0.0972, 1.0, 0.0972);
hline3->Draw();
TLine *hline4 = new TLine(0, 0.1728, 1.0, 0.1728);
hline4->Draw();
TLine *hline5 = new TLine(0, 0.279, 1.0, 0.279);
hline5->Draw();
TLine *hline6 = new TLine(0, 0.4239, 1.0, 0.4239);
hline6->Draw();
TLine *hline7 = new TLine(0, 0.6246, 1.0, 0.6246);
hline7->Draw();
TLine *hline8 = new TLine(0, 0.9081, 1.0, 0.9081);
hline8->Draw();
TLine *hline9 = new TLine(0, 1.314, 1.0, 1.314);
hline9->Draw();
TLine *hline10 = new TLine(0, 1.75, 1.0, 1.75);
hline10->Draw();

new TCanvas;
hPT2vz[8][3]->Draw("colz");


}
