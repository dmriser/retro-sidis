{
gStyle->SetOptStat(0);

bool includeTail = 0;

const int xBin = 0;
const int QQBin = 0;

const int NphihBins = 36;

int binSchemeOpt = 5;

int zStartBin = 5;
int cutLastNzBins = 7;
const int NzBins = 18 - zStartBin - cutLastNzBins; // this is the effective number of z bins
int PTsqStartBin = 0;
int cutLastNPTsqBins = 13;
const int NPTsqBins = 20 - PTsqStartBin - cutLastNPTsqBins; // this is the effective number of PTsq bins


TH1F *hdefault_RC[NzBins][NPTsqBins];
TH1F *h00_RC[NzBins][NPTsqBins];
TH1F *hNickPip_RC[NzBins][NPTsqBins];
TH1F *hNickPim_RC[NzBins][NPTsqBins];

TCanvas *can = new TCanvas("can", "can", 10, 10, 1500, 1000);
can->Divide(NzBins, NPTsqBins, 0.00001, 0.00001);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PTsq = PTsqStartBin; PTsq < NPTsqBins + PTsqStartBin; PTsq++) {

hdefault_RC[z-zStartBin][PTsq-PTsqStartBin] = new TH1F(Form("hdefault_RC_z%iPTsq%i", z, PTsq), Form("hdefault_RC_z%iPTsq%i", z, PTsq), NphihBins, -180, 180);
h00_RC[z-zStartBin][PTsq-PTsqStartBin] = new TH1F(Form("h00_RC_z%iPTsq%i", z, PTsq), Form("h00_RC_z%iPTsq%i", z, PTsq), NphihBins, -180, 180);
hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin] = new TH1F(Form("hNickPip_RC_z%iPTsq%i", z, PTsq), Form("hNickPip_RC_z%iPTsq%i", z, PTsq), NphihBins, -180, 180);
hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin] = new TH1F(Form("hNickPim_RC_z%iPTsq%i", z, PTsq), Form("hNickPim_RC_z%iPTsq%i", z, PTsq), NphihBins, -180, 180);

/////////////////////////////////////

ifstream defaultfile(Form("hapradResults/hapDefault/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, z, PTsq));
if(defaultfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
defaultfile>>sig>>sib>>tail;
if(sib > 0.000000000001 && includeTail) hdefault_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, sig/sib);
if(sib > 0.000000000001 && !includeTail) hdefault_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, (sig-tail)/sib);
hdefault_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinError(phih+1, 0);
}
}
defaultfile.close();

/////////////////////////////////////

ifstream file00(Form("hapradResults/hap00/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, z, PTsq));
if(file00)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
file00>>sig>>sib>>tail;
if(sib > 0.000000000001 && includeTail) h00_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, sig/sib);
if(sib > 0.000000000001 && !includeTail) h00_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, (sig-tail)/sib);
h00_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinError(phih+1, 0);
}
}
file00.close();

/////////////////////////////////////

ifstream fileNickPip(Form("hapradResults/NickPipModel/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, z, PTsq));
if(fileNickPip)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
fileNickPip>>sig>>sib>>tail;
if(sib > 0.000000000001 && includeTail) hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, sig/sib);
if(sib > 0.000000000001 && !includeTail) hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, (sig-tail)/sib);
hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinError(phih+1, 0);
}
}
fileNickPip.close();

/////////////////////////////////////

ifstream fileNickPim(Form("hapradResults/NickPimModel/pim_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, z, PTsq));
if(fileNickPim)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
fileNickPim>>sig>>sib>>tail;
if(sib > 0.000000000001 && includeTail) hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, sig/sib);
if(sib > 0.000000000001 && !includeTail) hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinContent(phih+1, (sig-tail)/sib);
hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin]->SetBinError(phih+1, 0);
}
}
fileNickPim.close();

/////////////////////////////////////

can->cd((NzBins*(NPTsqBins-1) - NzBins*(PTsq-PTsqStartBin)) + z-zStartBin + 1);

hdefault_RC[z-zStartBin][PTsq-PTsqStartBin]->GetYaxis()->SetRangeUser(0.85, 1.6);
hdefault_RC[z-zStartBin][PTsq-PTsqStartBin]->Draw();
h00_RC[z-zStartBin][PTsq-PTsqStartBin]->SetLineColor(kRed);
h00_RC[z-zStartBin][PTsq-PTsqStartBin]->Draw("same");
hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin]->SetLineColor(kGreen);
hNickPip_RC[z-zStartBin][PTsq-PTsqStartBin]->Draw("same");
hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin]->SetLineColor(kMagenta);
hNickPim_RC[z-zStartBin][PTsq-PTsqStartBin]->Draw("same");

}}


}
