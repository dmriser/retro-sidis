{
gStyle->SetOptStat(0);

//TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000009009__.root");
TFile *tf = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it0.s1.n32255.BiSc5.__0000000000000000__.root");

TH2F *hist = (TH2F*) tf->Get("rec_pim_PT2vsz"); // change pip or pim here

TCanvas *can = new TCanvas();
can->cd(1)->SetLogz();
hist->GetXaxis()->SetTitle("z");
hist->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hist->Draw("colz");

// scheme 5:
//const int NzBins = 18;
//float zLimits[NzBins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90};
//const int NPT2Bins = 20;
//float PT2Limits[NPT2Bins + 1] = {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00};

// scheme 8:
const int NzBins = 18*2;
float zLimits[NzBins + 1] = {0.000, 0.025, 0.050, 0.075, 0.100, 0.125, 0.150, 0.175, 0.200, 0.225, 0.250, 0.275, 0.300, 0.325, 0.350, 0.375, 0.400, 0.425, 0.450, 0.475, 0.500, 0.525, 0.550, 0.575, 0.600, 0.625, 0.650, 0.675, 0.700, 0.725, 0.750, 0.775, 0.800, 0.825, 0.850, 0.875, 0.900};
const int NPT2Bins = 20*2;
float PT2Limits[NPT2Bins + 1] = {0.000, 0.025, 0.050, 0.075, 0.100, 0.125, 0.150, 0.175, 0.200, 0.225, 0.250, 0.275, 0.300, 0.325, 0.350, 0.375, 0.400, 0.425, 0.450, 0.475, 0.500, 0.525, 0.550, 0.575, 0.600, 0.625, 0.650, 0.675, 0.700, 0.725, 0.750, 0.775, 0.800, 0.825, 0.850, 0.875, 0.900, 0.925, 0.950, 0.975, 1.000};

TLine *PT2Line[NPT2Bins + 1];
TLine *zLine[NzBins + 1];

for(int k = 0; k < NzBins + 1; k++)
{
zLine[k] = new TLine(zLimits[k], PT2Limits[0], zLimits[k], PT2Limits[NPT2Bins]);
zLine[k]->SetLineColor(kGray+1);
zLine[k]->SetLineWidth(2);
zLine[k]->Draw();
}
for(int k = 0; k < NPT2Bins + 1; k++)
{
PT2Line[k] = new TLine(zLimits[0], PT2Limits[k], zLimits[NzBins], PT2Limits[k]);
PT2Line[k]->SetLineColor(kGray+1);
PT2Line[k]->SetLineWidth(2);
PT2Line[k]->Draw();
}

}
