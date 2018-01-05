{
gStyle->SetOptStat(0);

const int NxBins = 5;
const int NQQBins = 2;
int xBin = 0;
int QQBin = 0;
float xpts[NxBins] = {0.15, 0.25, 0.35, 0.45, 0.55}; // center values
float QQpts[NxBins][NQQBins] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}}; // approx. center values

TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n12114.BiSc5.MoCo11.__0000000000000000__.root");

TH2F *hist = (TH2F*) tf->Get(Form("rec_pip_PT2vsz_x%iQQ%i", xBin, QQBin));

TCanvas *can = new TCanvas();
can->cd(1)->SetLogz();

hist->GetXaxis()->SetTitle("z");
hist->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hist->Draw("colz");

gROOT->ProcessLine(".L PTfcn.C");

TF1 *ff1 = new TF1("ff1", Form("pow(PTfcn(%3.4f,%3.4f,x,%3.4f),2.0)", xpts[xBin], QQpts[xBin][QQBin], 0.938), 0.1, 1);
ff1->Draw("same");
TF1 *ff2 = new TF1("ff2", Form("pow(PTfcn(%3.4f,%3.4f,x,%3.4f),2.0)", xpts[xBin], QQpts[xBin][QQBin], 1.1), 0.1, 0.94);
ff2->Draw("same");
TF1 *ff3 = new TF1("ff3", Form("pow(PTfcn(%3.4f,%3.4f,x,%3.4f),2.0)", xpts[xBin], QQpts[xBin][QQBin], 1.35), 0.1, 0.84);
ff3->Draw("same");
TF1 *ff4 = new TF1("ff4", Form("pow(PTfcn(%3.4f,%3.4f,x,%3.4f),2.0)", xpts[xBin], QQpts[xBin][QQBin], 1.5), 0.1, 0.78);
ff4->Draw("same");



}
