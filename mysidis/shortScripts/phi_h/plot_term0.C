{
gStyle->SetOptStat(0);

int binSchemeOpt = 5;

const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
float zMin = 0.0;
float zMax = 0.9;
const int NPTsqBins = 20;
float PTsqMin = 0.0;
float PTsqMax = 1.0;

TH2F *hterm0[NxBins][NQQBins];

TCanvas *CANcc = new TCanvas("CANcc", "CANcc", 10, 10, 1500, 1000);
CANcc->Divide(NxBins, NQQBins, 0.00001, 0.00001);

for(int x = 0; x < NxBins; x++)
{
for(int QQ = 0; QQ < NQQBins; QQ++)
{
if(!(x == 4 && QQ == 1))
{

hterm0[x][QQ] = new TH2F(Form("hterm0%i%i", x, QQ), Form("hterm0%i%i", x, QQ), NzBins, zMin, zMax, NPTsqBins, PTsqMin, PTsqMax);
hterm0[x][QQ]->GetXaxis()->SetTitle("z");
hterm0[x][QQ]->GetYaxis()->SetTitle("P_{h#perp}^{2}");

for(int z = 0; z < NzBins; z++)
{
for(int PTsq = 0; PTsq < NPTsqBins; PTsq++)
{
float term0, h3, h4;
ifstream infile(Form("/scratch/h3h4files/term0h3h4_pip_it0_BiSc%i_x%iQQ%iz%iPTsq%i.txt", binSchemeOpt, x, QQ, z, PTsq));
if(infile)
{
infile>>term0>>h3>>h4;
hterm0[x][QQ]->SetBinContent(z+1, PTsq+1, term0);
}
infile.close();
}}

CANcc->cd(x + 1 + (NQQBins-1)*NxBins - NxBins*QQ)->SetLogz();
hterm0[x][QQ]->Draw("colz");

}
}}


}
