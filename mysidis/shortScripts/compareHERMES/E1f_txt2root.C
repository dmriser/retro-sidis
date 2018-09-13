{
int NxBins = 5;
int NQQBins = 2;
int NzBins = 18;
int NPT2Bins = 20;
int NphihBins = 36;

string pipORpim = "pim";

ifstream infile(Form("%s_5Dtable.txt", pipORpim.c_str()));
TFile *tf = new TFile(Form("%s_5Dtable.root", pipORpim.c_str()), "recreate");

TH1F *hphih[NxBins][NQQBins][NzBins][NPT2Bins];
TH1F *binInfo[NxBins][NQQBins][NzBins][NPT2Bins];

int Nbins = NxBins*NQQBins*NzBins*NPT2Bins*NphihBins;

for(int i = 0; i < NxBins; i++) {
for(int j = 0; j < NQQBins; j++) {
for(int k = 0; k < NzBins; k++) {
for(int l = 0; l < NPT2Bins; l++) {

if(!(i == 4 && j == 1))
{

hphih[i][j][k][l] = new TH1F(Form("CLAS%s_hphih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), i, j, k, l), Form("CLAS%s_hphih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), i, j, k, l), NphihBins, -180, 180);
binInfo[i][j][k][l] = new TH1F(Form("CLAS%s_binInfo_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), i, j, k, l), Form("CLAS%s_binInfo_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), i, j, k, l), 5, 0, 5);

float avex, aveQQ, avez, avePT2, avey;
avex = aveQQ = avez = avePT2 = avey = 0;

int NnonZeroBins = 0;

for(int m = 0; m < NphihBins; m++)
{
int ix, iQQ, iz, iPT2, iphih;
float x, QQ, z, PT2, phih, y, N, N_e, RC;

infile>>ix>>iQQ>>iz>>iPT2>>iphih>>x>>QQ>>z>>PT2>>phih>>y>>N>>N_e>>RC;

hphih[ix][iQQ][iz][iPT2]->SetBinContent(iphih+1, N);
hphih[ix][iQQ][iz][iPT2]->SetBinError(iphih+1, N_e);

if(N > 0.000001)
{
avex += x;
aveQQ += QQ;
avez += z;
avePT2 += PT2;
avey += y;
NnonZeroBins++;
}
}

if(NnonZeroBins > 0)
{
binInfo[i][j][k][l]->SetBinContent(1, avex/NnonZeroBins);
binInfo[i][j][k][l]->SetBinContent(2, aveQQ/NnonZeroBins);
binInfo[i][j][k][l]->SetBinContent(3, avez/NnonZeroBins);
binInfo[i][j][k][l]->SetBinContent(4, avePT2/NnonZeroBins);
binInfo[i][j][k][l]->SetBinContent(5, avey/NnonZeroBins);
}

}

}}}}

tf->Write();
infile.close();

}
