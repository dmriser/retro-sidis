{
gStyle->SetOptStat(0);

string pipORpim = "pip";

TH2F *hQQvsxHER = new TH2F("hQQvsxHER", "hQQvsxHER", 100, 0.01, 0.61, 100, 0.01, 15.0);
TH2F *hPT2vszHER = new TH2F("hPT2vszHER", "hPT2vszHER", 100, 0.01, 0.99, 100, 0, 1.5);

TH2F *hQQvsxCLAS = new TH2F("hQQvsxCLAS", "hQQvsxCLAS", 100, 0.01, 0.61, 100, 0.01, 15.0);
TH2F *hPT2vszCLAS = new TH2F("hPT2vszCLAS", "hPT2vszCLAS", 100, 0.0, 1.0, 100, 0, 1.5);

////// read in HERMES data //////
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
hQQvsxHER->Fill(x, QQ);
hPT2vszHER->Fill(z, PT2);
}

infile.close();

////// read in CLAS data //////
int Nlines = 116640;
int NphihBins = 36;

ifstream infile(Form("%s_5Dtable.txt", pipORpim.c_str()));

for(int k = 0; k < Nlines; k++)
{
float crap, x, QQ, z, PT2;
infile>>crap>>crap>>crap>>crap>>crap>>x>>QQ>>z>>PT2>>crap>>crap>>crap>>crap>>crap;
	for(int j = 0; j < NphihBins - 1; j++)
	{
	infile>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap>>crap;
	}
hQQvsxCLAS->Fill(x, QQ);
hPT2vszCLAS->Fill(z, PT2);
}

infile.close();


////// draw results /////

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



TCanvas *can = new TCanvas("can", "can", 5, 5, 1200, 600);
can->Divide(2, 1, 0.00001, 0.00001);

can->cd(1);
hQQvsxHER->SetMarkerColor(kRed);
hQQvsxHER->SetMarkerStyle(6);
hQQvsxHER->Draw();
hQQvsxCLAS->SetMarkerColor(kBlue);
hQQvsxCLAS->SetMarkerStyle(6);
hQQvsxCLAS->Draw("same");

for(int ix = 0; ix <= NxBins; ix++)
{
TLine *vl = new TLine(xLimits[ix], hQQvsxHER->GetYaxis()->GetXmin(), xLimits[ix], hQQvsxHER->GetYaxis()->GetXmax());
vl->SetLineColor(kRed);
vl->Draw();
}

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ <= NQQBins; iQQ++) {
TLine *hl = new TLine(xLimits[ix], QQLimits[ix][iQQ], xLimits[ix+1], QQLimits[ix][iQQ]);
hl->SetLineColor(kRed);
hl->Draw();
}}


can->cd(2);
hPT2vszHER->SetMarkerColor(kRed);
hPT2vszHER->SetMarkerStyle(6);
hPT2vszHER->Draw();
hPT2vszCLAS->SetMarkerColor(kBlue);
hPT2vszCLAS->SetMarkerStyle(6);
hPT2vszCLAS->Draw("same");

for(int iz = 0; iz <= NzBins; iz++)
{
TLine *vl = new TLine(zLimits[iz], hPT2vszHER->GetYaxis()->GetXmin(), zLimits[iz], hPT2vszHER->GetYaxis()->GetXmax());
vl->SetLineColor(kRed);
vl->Draw();
}

for(int iPT2 = 0; iPT2 <= NPT2Bins; iPT2++)
{
TLine *hl = new TLine(hPT2vszHER->GetXaxis()->GetXmin(), PT2Limits[iPT2], hPT2vszHER->GetXaxis()->GetXmax(), PT2Limits[iPT2]);
hl->SetLineColor(kRed);
hl->Draw();
}



}
