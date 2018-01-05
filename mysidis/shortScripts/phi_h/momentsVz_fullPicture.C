void momentsVz_fullPicture(string pipORpim = "pip", string AcORAcc = "Acc")
{
gStyle->SetOptStat(0);

int accItN = 0;
int binSchemeOpt = 5;
string RCoptString = "";

const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
float zMin = 0;
float zMax = 0.9;
const int NPT2Bins = 20;
const int useNPT2Bins = 5; // number of PT2 bins to actually use

//int whichPT2bins[useNPT2Bins] = {1, 5, 9, 13, 17};
//int whichPT2bins[useNPT2Bins] = {1, 2, 3, 4, 5};
int whichPT2bins[useNPT2Bins] = {1, 3, 5, 7, 9};
int markstyle[useNPT2Bins] = {24, 20, 5, 22, 21};

TH1F *hXXvsz[NxBins][NQQBins][NPT2Bins];

TCanvas *can = new TCanvas("can", "can", 10, 10, 1600, 1000);
can->Divide(NxBins, NQQBins, 0, 0);

for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {
for(int PT2 = 0; PT2 < useNPT2Bins; PT2++) {
can->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ);

hXXvsz[x][QQ][PT2] = new TH1F(Form("hXXvsz_x%iQQ%iPT2%i", x, QQ, PT2), Form("hXXvsz_x%iQQ%iPT2%i", x, QQ, PT2), NzBins, zMin, zMax);
hXXvsz[x][QQ][PT2]->SetMarkerStyle(markstyle[PT2]);

for(int z = 0; z < NzBins; z++)
{
ifstream myfile(Form("/scratch/AcAccfiles/AcAcc%s_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", RCoptString.c_str(), pipORpim.c_str(), accItN, binSchemeOpt, x, QQ, z, whichPT2bins[PT2]));
float Ac, Ace, Acc, Acce;
Ac = Ace = Acc = Acce = 0;
if(myfile)
{
myfile>>Ac>>Ace>>Acc>>Acce;
if(AcORAcc == "Ac")
{
hXXvsz[x][QQ][PT2]->SetBinContent(z+1, Ac);
hXXvsz[x][QQ][PT2]->SetBinError(z+1, Ace);
}
if(AcORAcc == "Acc")
{
hXXvsz[x][QQ][PT2]->SetBinContent(z+1, Acc);
hXXvsz[x][QQ][PT2]->SetBinError(z+1, Acce);
}
}
myfile.close();
}

hXXvsz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-1, 1); // default
if(pipORpim == "pip" && AcORAcc == "Ac") hXXvsz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.55, 0.3);
if(pipORpim == "pip" && AcORAcc == "Acc") hXXvsz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.3, 0.3);
if(hXXvsz[x][QQ][PT2]->GetEntries() > 0) hXXvsz[x][QQ][PT2]->Draw("E same");

}}}


can->cd(NxBins);
TLegend *leg = new TLegend(0.45, 0.55, 0.95, 0.95);
leg->AddEntry(hXXvsz[0][0][0], "low PT2", "p");
leg->AddEntry(hXXvsz[0][0][1], "low mid PT2", "p");
leg->AddEntry(hXXvsz[0][0][2], "mid PT2", "p");
leg->AddEntry(hXXvsz[0][0][3], "high mid PT2", "p");
leg->AddEntry(hXXvsz[0][0][4], "high PT2", "p");
leg->Draw();

}
