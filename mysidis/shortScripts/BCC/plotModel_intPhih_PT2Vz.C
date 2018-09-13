// note: this takes a while to run

void plotModel_intPhih_PT2Vz(int xSubBin = 3, int QQSubBin = 2)
{
gStyle->SetOptStat(0);

string pipORpim = "pip";

int NxSubbins = 50;
int NQQSubbins = 50;
int NzSubBins = 90;
float zMin = 0.0;
float zMax = 0.9;
int NPT2SubBins = 100;
float PT2Min = 0.0;
float PT2Max = 1.0;
int NphihSubBins = 180;

TH2F *hPT2Vz = new TH2F("hPT2Vz", "hPT2Vz", NzSubBins, zMin, zMax, NPT2SubBins, PT2Min, PT2Max);

for(int iz = 1; iz <= NzSubBins; iz++) {
for(int iPT2 = 1; iPT2 <= NPT2SubBins; iPT2++) {

gSystem->Exec(Form("tar -zxf datfiles/go_%s_%i_%i.dat.tar.gz --transform \"s/test/test_%s_%i_%i/\"", pipORpim.c_str(), iz, iPT2, pipORpim.c_str(), iz, iPT2));
ifstream datfile(Form("test_%s_%i_%i.dat", pipORpim.c_str(), iz, iPT2));

int ignoreNlines = xSubBin*NxSubbins*NphihSubBins + QQSubBin*NphihSubBins;
for(int k = 0; k < ignoreNlines; k++) {
float dummy;
datfile>>dummy;
}

float summedCS = 0;
for(int iphih = 0; iphih < NphihSubBins; iphih++) {
float CS;
datfile>>CS;
summedCS = summedCS + CS;
}

hPT2Vz->SetBinContent(iz, iPT2, summedCS);

datfile.close();
gSystem->Exec(Form("rm test_%s_%i_%i.dat", pipORpim.c_str(), iz, iPT2));
}
cout<<iz<<"/"<<NzSubBins<<endl;
}

hPT2Vz->Draw("colz");

}
