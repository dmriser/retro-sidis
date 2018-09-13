// Note: HERMES bins in y instead of QQ, but since they're proportional, swapping variables is straight forward
{
int NxBins = 5;
int NQQBins = 5; // QQ scheme is different for each x bin; some x bins have fewer than 5 QQ bins but 5 is the most
int NzBins = 6;
int NPT2Bins = 6;

int Nlines = 900;

string pipORpim = "pim";

ifstream infile;
if(pipORpim == "pip") infile.open("900bins/Hyd_pi+.mom.noheading");
if(pipORpim == "pim") infile.open("900bins/Hyd_pi-.mom.noheading");
TFile *tf = new TFile(Form("HERMES_Hyd_%s.mom.root", pipORpim.c_str()), "recreate");

TH1F *binInfo[NxBins][NQQBins][NzBins][NPT2Bins];

for(int k = 0; k < Nlines; k++)
{
int ix, iQQ, iz, iPT2;
float crap, x, y, z, PT, QQ, PT2, Ac, Ac_e, Ac_sys, Acc, Acc_e, Acc_sys;
infile>>ix>>iQQ>>iz>>iPT2>>crap>>x>>y>>z>>PT>>Ac>>Ac_e>>Ac_sys>>Acc>>Acc_e>>Acc_sys;
ix--;
iQQ--;
iz--;
iPT2--;
// !!! NOTE COMMENTS BELOW !!!
PT2 = PT*PT; // <PT^2> = <PT>^2 is this safe???
QQ = 2.0*27.6*y*0.938*x; // <QQ> = 2E<y>M<x> is this safe??? Lamb thesis quotes Ebeam = 27.6 GeV, is this same experiment???
// !!! NOTE COMMENTS ABOVE !!!

binInfo[ix][iQQ][iz][iPT2] = new TH1F(Form("HER%s_binInfo_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), ix, iQQ, iz, iPT2), Form("HER%s_binInfo_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), ix, iQQ, iz, iPT2), 10, 0, 10);

binInfo[ix][iQQ][iz][iPT2]->SetBinContent(1, x);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(2, QQ);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(3, z);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(4, PT2);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(5, y);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(6, Ac);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(7, Ac_e);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(8, Ac_sys);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(9, Acc);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(10, Acc_e);
binInfo[ix][iQQ][iz][iPT2]->SetBinContent(11, Acc_sys);
}

tf->Write();
infile.close();
}
