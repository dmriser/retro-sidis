{
gStyle->SetOptStat(0);
ifstream infile("pip_sidis_cs_table_zh_pt2.dat");
int Nentries = 112566; // number of rows in the data table

string dummyString = "";
for(int dummyInt = 1; dummyInt < 27; dummyInt++) infile>>dummyString;

TH1F *hx = new TH1F("hx", "hx", 1000, 0.00, 1.00);
TH1F *hQQ = new TH1F("hQQ", "hQQ", 1000, 0.00, 10.00);
TH1F *hz = new TH1F("hz", "hz", 1000, 0.00, 1.00);
TH1F *hPT2 = new TH1F("hPT2", "hPT2", 1000, 0.00, 2.00);

for(int k = 0; k < Nentries; k++)
{
float QQ, x, z, PT2, phih, CS, stat_e, sys_e, RC;
infile>>QQ>>x>>z>>PT2>>phih>>CS>>stat_e>>sys_e>>RC;

hx->Fill(x);
hQQ->Fill(QQ);
hz->Fill(z);
hPT2->Fill(PT2);
}

infile.close();

for(int k = 0; k < hx->GetXaxis()->GetNbins(); k++) {
if(hx->GetBinContent(k+1) > 0.5) cout<<hx->GetBinCenter(k+1)<<endl;
}
cout<<endl;

for(int k = 0; k < hQQ->GetXaxis()->GetNbins(); k++) {
if(hQQ->GetBinContent(k+1) > 0.5) cout<<hQQ->GetBinCenter(k+1)<<endl;
}
cout<<endl;

for(int k = 0; k < hz->GetXaxis()->GetNbins(); k++) {
if(hz->GetBinContent(k+1) > 0.5) cout<<hz->GetBinCenter(k+1)<<endl;
}
cout<<endl;

for(int k = 0; k < hPT2->GetXaxis()->GetNbins(); k++) {
if(hPT2->GetBinContent(k+1) > 0.5) cout<<hPT2->GetBinCenter(k+1)<<endl;
}
cout<<endl;


}
