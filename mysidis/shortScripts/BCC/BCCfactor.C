{
TFile *tf_normal = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");
TFile *tf_BCC = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12_BCC.it2.s1.n32255.BiSc55.__0000000000000000__.root");

string pipORpim = "pip";

int NxBins = 5;
int NQQBins = 2;
int NzBins = 18;
int NPT2Bins = 20;
int NphihBins = 36;

for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {
if(!(x == 4 && QQ == 1))
{
for(int z = 0; z < NzBins; z++) {
for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {

TH1F *h_normal = (TH1F*) tf_normal->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), x, QQ, z, PT2));
TH1F *h_BCC = (TH1F*) tf_BCC->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), x, QQ, z, PT2));

for(int phih = 0; phih < NphihBins; phih++) {
float totalCounts = h_normal->GetBinContent(phih+1);
float subBinCounts = h_BCC->GetBinContent(5*(phih+1)-2);
//if(!(totalCounts == 0 && subBinCounts == 0)) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<phih<<" "<<totalCounts<<" "<<subBinCounts<<endl;
//if(subBinCounts > 642) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<phih<<" "<<totalCounts<<" "<<subBinCounts<<endl;
//if(x == 0 && QQ == 0 && z == 6 && PT2 == 6 && phih == 18) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<phih<<" "<<totalCounts<<" "<<subBinCounts<<endl;
if(x == 0 && QQ == 0 && z == 1 && PT2 == 0) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<phih<<" "<<totalCounts<<" "<<subBinCounts<<endl;
}

}}
}
}}



}
