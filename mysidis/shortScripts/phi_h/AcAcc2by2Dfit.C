{
gStyle->SetOptStat(0);

string pipORpim = "pip";
int accItN = 0;
int binSchemeOpt = 5;

const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
float zMin = 0.0;
float zMax = 0.9;
const int NPTsqBins = 20;
float PTsqMin = 0.0;
float PTsqMax = 1.0;

TH2F *hM[NxBins][NQQBins];
TH2F *hAc[NxBins][NQQBins];
TH2F *hAcc[NxBins][NQQBins];
TF2 *ffc[NxBins][NQQBins];
TF2 *ffcc[NxBins][NQQBins];

TCanvas *CANM = new TCanvas("CANM", "CANM", 10, 10, 1500, 1000);
CANM->Divide(NxBins, NQQBins, 0.00001, 0.00001);
TCanvas *CANc = new TCanvas("CANc", "CANc", 10, 10, 1500, 1000);
CANc->Divide(NxBins, NQQBins, 0.00001, 0.00001);
TCanvas *CANc2 = new TCanvas("CANc2", "CANc2", 10, 10, 1500, 1000);
CANc2->Divide(NxBins, NQQBins, 0.00001, 0.00001);
TCanvas *CANcc = new TCanvas("CANcc", "CANcc", 10, 10, 1500, 1000);
CANcc->Divide(NxBins, NQQBins, 0.00001, 0.00001);
TCanvas *CANcc2 = new TCanvas("CANcc2", "CANcc2", 10, 10, 1500, 1000);
CANcc2->Divide(NxBins, NQQBins, 0.00001, 0.00001);

for(int x = 0; x < NxBins; x++)
{
for(int QQ = 0; QQ < NQQBins; QQ++)
{
if(!(x == 4 && QQ == 1))
{

hM[x][QQ] = new TH2F(Form("hM%i%i", x, QQ), Form("hM%i%i", x, QQ), NzBins, zMin, zMax, NPTsqBins, PTsqMin, PTsqMax);
hM[x][QQ]->GetXaxis()->SetTitle("z");
hM[x][QQ]->GetYaxis()->SetTitle("P_{h#perp}^{2}");
hAc[x][QQ] = new TH2F(Form("hAc%i%i", x, QQ), Form("hAc%i%i", x, QQ), NzBins, zMin, zMax, NPTsqBins, PTsqMin, PTsqMax);
hAc[x][QQ]->GetXaxis()->SetTitle("z");
hAc[x][QQ]->GetYaxis()->SetTitle("P_{h#perp}^{2}");
hAcc[x][QQ] = new TH2F(Form("hAcc%i%i", x, QQ), Form("hAcc%i%i", x, QQ), NzBins, zMin, zMax, NPTsqBins, PTsqMin, PTsqMax);
hAcc[x][QQ]->GetXaxis()->SetTitle("z");
hAcc[x][QQ]->GetYaxis()->SetTitle("P_{h#perp}^{2}");

//"[0] + [1]*x + [2]*y + [3]*x*x + [4]*y*y + [5]*x*y + [6]*x*x*y + [7]*x*y*y + [8]*x*x*y*y"
//"[0] + [1]*x + [2]*y + [3]*x*x + [4]*y*y + [5]*x*y + [6]*x*x*x + [7]*y*y*y + [8]*.....
//"pow(x, [0])*pow(1.0 - x, [1])*pow(y, [2])*pow(1.0 - y, [3])" // didn't work well

ffc[x][QQ] = new TF2(Form("ffc%i%i", x, QQ), "[0]*sqrt(y)*pow(x, [1])*pow(1.0 - x, [2])*pow(1.0 - y, [3])", 0, 1, 0, 1);
ffcc[x][QQ] = new TF2(Form("ffcc%i%i", x, QQ), "[0]*y*pow(x, [1])*pow(1.0 - x, [2])*pow(1.0 - y, [3])", 0, 1, 0, 1);
ffc[x][QQ]->SetLineWidth(1);
ffcc[x][QQ]->SetLineWidth(1);

for(int z = 0; z < NzBins; z++)
{
for(int PTsq = 0; PTsq < NPTsqBins; PTsq++)
{
float M, Me;
ifstream infile(Form("/scratch/AcAccfiles/M_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, x, QQ, z, PTsq));
if(infile)
{
infile>>M>>Me;
hM[x][QQ]->SetBinContent(z+1, PTsq+1, M);
hM[x][QQ]->SetBinError(z+1, PTsq+1, Me);
}
infile.close();
float Ac, Ace, Acc, Acce;
ifstream infile2(Form("/scratch/AcAccfiles/AcAcc_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, x, QQ, z, PTsq));
if(infile2)
{
infile2>>Ac>>Ace>>Acc>>Acce;
hAc[x][QQ]->SetBinContent(z+1, PTsq+1, Ac);
hAc[x][QQ]->SetBinError(z+1, PTsq+1, Ace);
hAcc[x][QQ]->SetBinContent(z+1, PTsq+1, Acc);
hAcc[x][QQ]->SetBinError(z+1, PTsq+1, Acce);
}
infile2.close();
}}

if(pipORpim == "pip")
{
hAc[x][QQ]->SetMinimum(-0.5);
hAc[x][QQ]->SetMaximum(0.2);
hAcc[x][QQ]->SetMinimum(-0.2);
hAcc[x][QQ]->SetMaximum(0.4);
}
if(pipORpim == "pim")
{
hAc[x][QQ]->SetMinimum(-0.3);
hAc[x][QQ]->SetMaximum(0.2);
hAcc[x][QQ]->SetMinimum(-0.2);
hAcc[x][QQ]->SetMaximum(0.4);
}

string fitMethod = "LL";

CANM->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ)->SetLogz();
hM[x][QQ]->Draw("colz");

CANc->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ);
hAc[x][QQ]->Fit(Form("ffc%i%i", x, QQ), fitMethod.c_str());
hAc[x][QQ]->Fit(Form("ffc%i%i", x, QQ), fitMethod.c_str());
hAc[x][QQ]->Draw("lego hist");
ffc[x][QQ]->Draw("surf3 same");

CANc2->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ);
hAc[x][QQ]->Draw("colz hist");
ffc[x][QQ]->Draw("cont1 same");

CANcc->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ);
hAcc[x][QQ]->Fit(Form("ffcc%i%i", x, QQ), fitMethod.c_str());
hAcc[x][QQ]->Fit(Form("ffcc%i%i", x, QQ), fitMethod.c_str());
hAcc[x][QQ]->Draw("lego hist");
ffcc[x][QQ]->Draw("surf3 same");

CANcc2->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ);
hAcc[x][QQ]->Draw("colz hist");
ffcc[x][QQ]->Draw("cont1 same");

}
}}


}
