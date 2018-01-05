// sloppy binning scheme consistency here, everything is for Sc5, be very careful if you change anything

void categorizeBinFiducial_Sc5_v2(int ExpOrSim = 1, int accItN = 0, string pipORpim = "pim")
{
gStyle->SetOptStat(0);

const int NphihBBins = 6; // "B" because this binning is for a different purpose (Phih0_10...Phih170_180), not the usual 36 bins
//string phihRangeString[NphihBBins] = {"Phih0_10", "Phih10_20", "Phih20_30", "Phih30_40", "Phih40_50", "Phih50_60", "Phih60_70", "Phih70_80", "Phih80_90", "Phih90_100", "Phih100_110", "Phih110_120", "Phih120_130", "Phih130_140", "Phih140_150", "Phih150_160", "Phih160_170", "Phih170_180"};
string phihRangeString[NphihBBins] = {"Phih0_10", "Phih10_20", "Phih20_30", "Phih30_40", "Phih40_50", "Phih50_60"};
int colors[NphihBBins] = {6, 5, 4, 3, 2, 1};

const int NxBins = 5;
const int NQQBins = 2;

const int NzBins = 18;
float zMin = 0.0;
float zMax = 0.9;
float zBW = (zMax - zMin)/NzBins;
const int NPT2Bins = 20;
float PT2Min = 0.0;
float PT2Max = 1.0;
float PT2BW = (PT2Max - PT2Min)/NPT2Bins;
TLine *vLine[NzBins+1];
TLine *hLine[NPT2Bins+1];

TFile *tf;
if(ExpOrSim == 1) tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n12114.BiSc5.MoCo11.__0000000000000000__.root");
if(ExpOrSim == 0) tf = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc5.__0000000000000000__.root", accItN));

TCanvas *can = new TCanvas("can", "can", 20, 20, 1550, 950);
can->Divide(NxBins, NQQBins, 0.00001, 0.00001);

TH2F *hPT2Vz[NxBins][NQQBins][NphihBBins];

for(int phihB = NphihBBins - 1; phihB >= 0; phihB--) { // count backwards so bigger coverage plots are drawn first
for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {

can->cd(x + 1 + NxBins*(NQQBins-1) - NxBins*QQ)->SetLogz();
hPT2Vz[x][QQ][phihB] = (TH2F*) tf->Get(Form("rec_%s_PT2vsz_%s_x%iQQ%i", pipORpim.c_str(), phihRangeString[phihB].c_str(), x, QQ));
if(ExpOrSim == 1) hPT2Vz[x][QQ][phihB]->SetTitle(Form("data %s PT2 vs z x%i QQ%i", pipORpim.c_str(), x, QQ));
if(ExpOrSim == 0) hPT2Vz[x][QQ][phihB]->SetTitle(Form("mc it%i %s PT2 vs z x%i QQ%i", accItN, pipORpim.c_str(), x, QQ));
hPT2Vz[x][QQ][phihB]->GetYaxis()->SetTitle("PT2 (GeV^{2})");
hPT2Vz[x][QQ][phihB]->GetXaxis()->SetTitle("z");
hPT2Vz[x][QQ][phihB]->SetMarkerColor(colors[phihB]);
hPT2Vz[x][QQ][phihB]->SetMarkerStyle(6);
hPT2Vz[x][QQ][phihB]->Draw("scat same");

	for(int z = 0; z < NzBins+1; z++) {
	vLine[z] = new TLine(z*zBW, PT2Min, z*zBW, PT2Max);
	vLine[z]->SetLineColor(kGray);
	vLine[z]->SetLineWidth(1);
	vLine[z]->Draw();
	}
	for(int PT2 = 0; PT2 < NPT2Bins+1; PT2++) {
	hLine[PT2] = new TLine(zMin, PT2*PT2BW, zMax, PT2*PT2BW);
	hLine[PT2]->SetLineColor(kGray);
	hLine[PT2]->SetLineWidth(1);
	hLine[PT2]->Draw();
	}

}}}

}
