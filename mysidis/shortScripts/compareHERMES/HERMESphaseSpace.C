void HERMESphaseSpace(string pipORpim = "pip", float zMin = 0.51, float zMax = 0.58, float PT2Min = 0.25, float PT2Max = 0.4)
{
gStyle->SetOptStat(0);
TH2F *hQQvsx = new TH2F("hQQvsx", "hQQvsx", 100, 0.01, 0.5, 100, 0.01, 15.0);
TH2F *hPT2vsz = new TH2F("hPT2vsz", "hPT2vsz", 100, 0.2, 0.9, 100, 0, 1.5);
TH2F *hQQvsxCut = new TH2F("hQQvsxCut", "hQQvsxCut", 100, 0.01, 0.5, 100, 0.01, 15.0);

//// approx. HERMES binning scheme: (this just needs to be close enough since all I have is average kinematic values, not the full distributions) ////
const int NxBins = 5;
float xLimits[NxBins+1] = {0.02, 0.05, 0.09, 0.15, 0.25, 0.45};
const int NQQBins = 5; // QQ scheme is different for each x bin; some x bins have fewer than 5 QQ bins but 5 is the most... use dummy values for x bins with < 5
float QQLimits[NxBins][NQQBins+1] = { {1.0, 1.15, 1.24, 1.35, 99.0, 101.0},
                                      {1.0, 1.15, 1.40, 1.70, 2.00, 2.500},
                                      {1.0, 1.60, 2.40, 3.20, 3.80, 4.600},
                                      {2.0, 3.00, 4.50, 5.80, 7.00, 8.200},
                                      {6.0, 8.00, 11.0, 13.5, 99.0, 101.0} };
const int NzBins = 6;
float zLimits[NzBins+1] = {0.22, 0.31, 0.4, 0.5, 0.6, 0.75, 0.9};
const int NPT2Bins = 6;
float PT2Limits[NPT2Bins+1] = {0.0, 0.05, 0.13, 0.24, 0.45, 0.9, 1.4};

int Nlines = 900;

ifstream infile;
if(pipORpim == "pip") infile.open("900bins/Hyd_pi+.mom.noheading");
if(pipORpim == "pim") infile.open("900bins/Hyd_pi-.mom.noheading");

for(int k = 0; k < Nlines; k++)
{
float crap, x, y, z, PT, QQ, PT2;
infile>>crap>>crap>>crap>>crap>>crap>>x>>y>>z>>PT>>crap>>crap>>crap>>crap>>crap>>crap;
// !!! NOTE COMMENTS BELOW !!!
PT2 = PT*PT; // <PT^2> = <PT>^2 is this safe???
QQ = 2.0*27.6*y*0.938*x; // <QQ> = 2E<y>M<x> is this safe??? Lamb thesis quotes Ebeam = 27.6 GeV, is this same experiment???
// !!! NOTE COMMENTS ABOVE !!!
hQQvsx->Fill(x, QQ);
hPT2vsz->Fill(z, PT2);
if(z > zMin && z < zMax && PT2 > PT2Min && PT2 < PT2Max) hQQvsxCut->Fill(x, QQ);
}

infile.close();

TCanvas *can = new TCanvas("can", "can", 5, 5, 1300, 600);
can->Divide(3, 1, 0.00001, 0.00001);

can->cd(1);
hQQvsx->Draw("colz");

for(int ix = 0; ix <= NxBins; ix++)
{
TLine *vl = new TLine(xLimits[ix], hQQvsx->GetYaxis()->GetXmin(), xLimits[ix], hQQvsx->GetYaxis()->GetXmax());
vl->Draw();
}

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ <= NQQBins; iQQ++) {
TLine *hl = new TLine(xLimits[ix], QQLimits[ix][iQQ], xLimits[ix+1], QQLimits[ix][iQQ]);
hl->Draw();
}}


can->cd(2);
hPT2vsz->Draw("colz");
TLine *bline = new TLine(zMin, PT2Min, zMax, PT2Min);
bline->SetLineColor(kRed);
bline->SetLineWidth(2);
bline->Draw();
TLine *tline = new TLine(zMin, PT2Max, zMax, PT2Max);
tline->SetLineColor(kRed);
tline->SetLineWidth(2);
tline->Draw();
TLine *lline = new TLine(zMin, PT2Min, zMin, PT2Max);
lline->SetLineColor(kRed);
lline->SetLineWidth(2);
lline->Draw();
TLine *rline = new TLine(zMax, PT2Min, zMax, PT2Max);
rline->SetLineColor(kRed);
rline->SetLineWidth(2);
rline->Draw();

for(int iz = 0; iz <= NzBins; iz++)
{
TLine *vl = new TLine(zLimits[iz], hPT2vsz->GetYaxis()->GetXmin(), zLimits[iz], hPT2vsz->GetYaxis()->GetXmax());
vl->Draw();
}

for(int iPT2 = 0; iPT2 <= NPT2Bins; iPT2++)
{
TLine *hl = new TLine(hPT2vsz->GetXaxis()->GetXmin(), PT2Limits[iPT2], hPT2vsz->GetXaxis()->GetXmax(), PT2Limits[iPT2]);
hl->Draw();
}


can->cd(3);
hQQvsxCut->Draw("colz");

for(int ix = 0; ix <= NxBins; ix++)
{
TLine *vl = new TLine(xLimits[ix], hQQvsx->GetYaxis()->GetXmin(), xLimits[ix], hQQvsx->GetYaxis()->GetXmax());
vl->Draw();
}

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ <= NQQBins; iQQ++) {
TLine *hl = new TLine(xLimits[ix], QQLimits[ix][iQQ], xLimits[ix+1], QQLimits[ix][iQQ]);
hl->Draw();
}}



}
