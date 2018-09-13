// using phihBBin = 0 here because I want to only look at bins w/ full coverage

void mark_zPT2_points(int xBin = 8, int QQBin = 4)
{
gStyle->SetOptStat(0);

int phihBBin = 0;

TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc6.MoCo11.90phihBBins.__0000000000000000__.root");
TH2F *hist = (TH2F*) tf->Get(Form("rec_pip_PT2vsz_x%iQQ%iphihB%i", xBin, QQBin, phihBBin));
hist->GetXaxis()->SetTitle("z");
hist->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hist->SetTitle(Form("x%i QQ%i, %3.1f#circ < |#phi_{h}| < %3.1f#circ", xBin, QQBin, phihBBin*2.0, (phihBBin+1)*2.0)); // 2.0 is the bin width
hist->Draw("colz");

//// draw the bin scheme lines: ////
const int NzBins = 22;
const int NPT2Bins = 10;

float zLimits[NzBins+1] = {0.059, 0.081, 0.1065, 0.133, 0.161, 0.1905, 0.221, 0.2525, 0.2855, 0.32, 0.356, 0.394, 0.4335, 0.4745, 0.5175, 0.5625, 0.6095, 0.6585, 0.7095, 0.763, 0.8185, 0.8765, 0.93};
float PT2Limits[NPT2Bins+1] = {0.0, 0.0153, 0.0459, 0.0972, 0.1728, 0.279, 0.4239, 0.6246, 0.9081, 1.314, 1.75};

for(int z = 0; z <= NzBins; z++) {
for(int PT2 = 0; PT2 <= NPT2Bins; PT2++) {
TLine *vline = new TLine(zLimits[z], 0, zLimits[z], 1);
vline->Draw();
TLine *hline = new TLine(0, PT2Limits[PT2], 1, PT2Limits[PT2]);
hline->Draw();
}}

//// mark the points where I'm making a measurement: ////
float zCenter[NzBins] = {0.0685, 0.0935, 0.1195, 0.1465, 0.1755, 0.2055, 0.2365, 0.2685, 0.3025, 0.3375, 0.3745, 0.4135, 0.4535, 0.4955, 0.5395, 0.5855, 0.6335, 0.6835, 0.7355, 0.7905, 0.8465, 0.9065};
float PT2Center[NPT2Bins] = {0.005, 0.025, 0.065, 0.129, 0.217, 0.341, 0.507, 0.743, 1.075, 1.555};

// copy these three lines from sideBySideCompare_VPT2andz.C:

//const int NspecialBins = 31; // these numbers work well with xBin = 8, QQBin = 4; xBin = 9, QQBin = 5; ...
//int zBin[NspecialBins] =   {0, 1, 1, 2, 3, 4, 2, 3, 4, 5, 6, 4, 5, 6, 7, 8, 7, 8, 9, 10, 11, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//int PT2Bin[NspecialBins] = {1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,  5,  5, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0};
const int NspecialBins = 34; // these numbers work well with xBin = 4, QQBin = 2; ...
int zBin[NspecialBins] =   {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 3, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 13};
int PT2Bin[NspecialBins] = {0, 1, 1, 2, 2, 3, 3, 2, 3, 4, 4, 2, 3, 4, 5, 3, 4, 5, 3, 4, 5, 6, 4, 5, 6, 4, 5, 6, 5 , 6 , 5 , 6 , 6 , 6 };

for(int k = 0; k < NspecialBins; k++)
{
TMarker *m = new TMarker(zCenter[zBin[k]], PT2Center[PT2Bin[k]], 29);
m->SetMarkerSize(4.5);
m->SetMarkerColor(kRed);
m->Draw();

TLatex *tl = new TLatex(zCenter[zBin[k]]-0.007, PT2Center[PT2Bin[k]]-0.005, Form("%i", k)); // -0.005 centers it a little better
tl->SetTextSize(0.018);
tl->SetTextColor(kWhite);
tl->Draw();
}


}
