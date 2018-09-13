#include <algorithm> // for std::max

void compareMoCo(int xBin = 1, int QQBin = 1, string pipORpim = "pim") // don't compile
{
gStyle->SetOptStat(0);

int binSchemeOpt = 5;
TFile *tfdata11 = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root");
TFile *tfdata00 = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo00.__0000000000000000__.root");

const int NzBins = 4; // number to show, not total
const int zStart = 4;
const int NPT2Bins = 4;
const int PT2Start = 7;

TH1F *hphih11[NzBins][NPT2Bins];
TH1F *hphih00[NzBins][NPT2Bins];
TH1F *hphihrat[NzBins][NPT2Bins];

TCanvas *can = new TCanvas("can", "can", 50, 50, 1200, 900);
can->Divide(NzBins, NPT2Bins, 0.00001, 0.00001);

TCanvas *canrat = new TCanvas("canrat", "canrat", 70, 70, 1200, 900);
canrat->Divide(NzBins, NPT2Bins, 0.00001, 0.00001);

for(int iz = 0; iz < NzBins; iz++)
{
	for(int iPT2 = 0; iPT2 < NPT2Bins; iPT2++)
	{
		hphih11[iz][iPT2] = (TH1F*) tfdata11->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, iz + zStart, iPT2 + PT2Start)); // already Sumw2'ed
		hphih11[iz][iPT2]->GetXaxis()->SetTitle("#phi (deg)");
		hphih11[iz][iPT2]->SetLabelSize(0.05, "XYZ");
		hphih11[iz][iPT2]->SetTitleSize(0.05, "XYZ");

		hphih00[iz][iPT2] = (TH1F*) tfdata00->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, iz + zStart, iPT2 + PT2Start)); // already Sumw2'ed
		hphih00[iz][iPT2]->SetLineColor(kRed);
		hphih00[iz][iPT2]->SetLabelSize(0.05, "XYZ");

		hphih11[iz][iPT2]->GetYaxis()->SetRangeUser(0.0, 1.15*std::max(hphih11[iz][iPT2]->GetMaximum(), hphih00[iz][iPT2]->GetMaximum()));

		can->cd(NzBins*(NPT2Bins - iPT2 - 1) + iz + 1);
		hphih11[iz][iPT2]->Draw();
		hphih00[iz][iPT2]->Draw("same");

		hphihrat[iz][iPT2] = new TH1F(Form("hphihrat_%i_%i", iz + zStart, iPT2 + PT2Start), Form("hphihrat_%i_%i", iz + zStart, iPT2 + PT2Start), hphih11[iz][iPT2]->GetXaxis()->GetNbins(), hphih11[iz][iPT2]->GetXaxis()->GetXmin(), hphih11[iz][iPT2]->GetXaxis()->GetXmax());
		hphihrat[iz][iPT2]->Sumw2();
		hphihrat[iz][iPT2]->Divide(hphih11[iz][iPT2], hphih00[iz][iPT2]);
		hphihrat[iz][iPT2]->GetYaxis()->SetRangeUser(0.0, 2.0);
		hphihrat[iz][iPT2]->GetXaxis()->SetTitle("#phi (deg)");
		hphihrat[iz][iPT2]->SetTitle(Form("%s #phi w/ : w/o p corr. ratio x%i QQ%i z%i PTsq%i", pipORpim.c_str(), xBin, QQBin, iz + zStart, iPT2 + PT2Start));
		hphihrat[iz][iPT2]->SetLabelSize(0.05, "XYZ");
		hphihrat[iz][iPT2]->SetTitleSize(0.05, "XYZ");

		canrat->cd(NzBins*(NPT2Bins - iPT2 - 1) + iz + 1);
		hphihrat[iz][iPT2]->Draw();
	}
}



}
