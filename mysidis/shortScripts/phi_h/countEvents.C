{
gStyle->SetOptStat(0);
gStyle->SetTitleSize(0.045, "XYZ");
gStyle->SetLabelSize(0.045, "XYZ");

string pipORpim = "pim";

int binSchemeOpt = 5;

TFile *tfdata = new TFile(Form("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));

if(binSchemeOpt == 5)
{
int NphihBins = 36;
int NxBins = 5;
int NQQBins = 2;
int zStartBin = 8;
int cutLastNzBins = 4;
const int NzBins = 18 - zStartBin - cutLastNzBins; // this is the effective number of z bins
int PT2StartBin = 0;
int cutLastNPT2Bins = 0;
const int NPT2Bins = 20 - PT2StartBin - cutLastNPT2Bins; // this is the effective number of PT2 bins
}

int count = 0;

for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {
for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {

TH1F *hist = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), x, QQ, z, PT2));

//cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<hist->GetEntries()<<endl;
count = count + hist->GetEntries();

}}}}

cout<<count<<endl;

}
