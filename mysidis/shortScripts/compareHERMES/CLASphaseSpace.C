void CLASphaseSpace(string pipORpim = "pip", float zMin = 0.54, float zMax = 0.585, float PT2Min = 0.25, float PT2Max = 0.29)
{
gStyle->SetOptStat(0);
TH2F *hQQvsx = new TH2F("hQQvsx", "hQQvsx", 100, 0.01, 0.5, 100, 0.01, 5.0);
TH2F *hPT2vsz = new TH2F("hPT2vsz", "hPT2vsz", 100, 0.2, 0.9, 100, 0, 1.1);
TH2F *hQQvsxCut = new TH2F("hQQvsxCut", "hQQvsxCut", 100, 0.01, 0.5, 100, 0.01, 5.0);

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
hQQvsx->Fill(x, QQ);
hPT2vsz->Fill(z, PT2);
if(z > zMin && z < zMax && PT2 > PT2Min && PT2 < PT2Max) hQQvsxCut->Fill(x, QQ);
}

infile.close();

TCanvas *can = new TCanvas("can", "can", 5, 5, 1300, 600);
can->Divide(3, 1, 0.00001, 0.00001);

can->cd(1);
hQQvsx->Draw("colz");
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
can->cd(3);
hQQvsxCut->Draw("colz");

}
