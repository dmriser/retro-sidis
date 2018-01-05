{
gStyle->SetOptStat(0);

TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000009009__.root");

TH2F *hist = (TH2F*) tf->Get("rec_pim_PT2vsz"); // change pip or pim here

TCanvas *can = new TCanvas();
can->cd(1)->SetLogz();
hist->GetXaxis()->SetTitle("z");
hist->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hist->Draw("colz");

const int NzBins = 18;
float zLimits[NzBins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90};
const int NPT2Bins = 20;
float PT2Limits[NPT2Bins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00};

TLine *PT2Line[NPT2Bins + 1];
TLine *zLine[NzBins + 1];

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

}
