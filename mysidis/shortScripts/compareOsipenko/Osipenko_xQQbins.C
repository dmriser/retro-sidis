{
gStyle->SetOptStat(0);
ifstream infile("pip_sidis_cs_table_zh_pt2.dat");
int Nentries = 112566; // number of rows in the data table

string dummyString = "";
for(int dummyInt = 1; dummyInt < 27; dummyInt++) infile>>dummyString;

TH2F *hQQvx = new TH2F("hQQvx", "hQQvx", 100, 0.05, 1.00, 100, 0.5, 8.5);
TH1F *hx = new TH1F("hx", "hx", 1000, 0.00, 1.00);
TH1F *hQQ = new TH1F("hQQ", "hQQ", 1000, 0.00, 10.00);

for(int k = 0; k < Nentries; k++)
{
float QQ, x, z, PT2, phih, CS, stat_e, sys_e, RC;
infile>>QQ>>x>>z>>PT2>>phih>>CS>>stat_e>>sys_e>>RC;

hQQvx->Fill(x, QQ);
hx->Fill(x);
hQQ->Fill(QQ);
}

infile.close();


hQQvx->Draw("colz");

TLine *vline0 = new TLine(0.128, 0.5, 0.128, 8.5);
vline0->Draw();
TLine *vline1 = new TLine(0.161, 0.5, 0.161, 8.5);
vline1->Draw();
TLine *vline2 = new TLine(0.1905, 0.5, 0.1905, 8.5);
vline2->Draw();
TLine *vline3 = new TLine(0.221, 0.5, 0.221, 8.5);
vline3->Draw();
TLine *vline4 = new TLine(0.2525, 0.5, 0.2525, 8.5);
vline4->Draw();
TLine *vline5 = new TLine(0.2855, 0.5, 0.2855, 8.5);
vline5->Draw();
TLine *vline6 = new TLine(0.32, 0.5, 0.32, 8.5);
vline6->Draw();
TLine *vline7 = new TLine(0.356, 0.5, 0.356, 8.5);
vline7->Draw();
TLine *vline8 = new TLine(0.394, 0.5, 0.394, 8.5);
vline8->Draw();
TLine *vline9 = new TLine(0.4335, 0.5, 0.4335, 8.5);
vline9->Draw();
TLine *vline10 = new TLine(0.4745, 0.5, 0.4745, 8.5);
vline10->Draw();
TLine *vline11 = new TLine(0.5175, 0.5, 0.5175, 8.5);
vline11->Draw();
TLine *vline12 = new TLine(0.5625, 0.5, 0.5625, 8.5);
vline12->Draw();
TLine *vline13 = new TLine(0.6095, 0.5, 0.6095, 8.5);
vline13->Draw();
TLine *vline14 = new TLine(0.6585, 0.5, 0.6585, 8.5);
vline14->Draw();
TLine *vline15 = new TLine(0.7095, 0.5, 0.7095, 8.5);
vline15->Draw();
TLine *vline16 = new TLine(0.763, 0.5, 0.763, 8.5);
vline16->Draw();
TLine *vline17 = new TLine(0.8185, 0.5, 0.8185, 8.5);
vline17->Draw();
TLine *vline18 = new TLine(0.8765, 0.5, 0.8765, 8.5);
vline18->Draw();
TLine *vline19 = new TLine(0.9425, 0.5, 0.9425, 8.5);
vline19->Draw();

TLine *hline0 = new TLine(0.05, 1.38, 0.99, 1.38);
hline0->Draw();
TLine *hline1 = new TLine(0.05, 1.62, 0.99, 1.62);
hline1->Draw();
TLine *hline2 = new TLine(0.05, 1.88, 0.99, 1.88);
hline2->Draw();
TLine *hline3 = new TLine(0.05, 2.19, 0.99, 2.19);
hline3->Draw();
TLine *hline4 = new TLine(0.05, 2.65, 0.99, 2.65);
hline4->Draw();
TLine *hline5 = new TLine(0.05, 3.18, 0.99, 3.18);
hline5->Draw();
TLine *hline6 = new TLine(0.05, 3.76, 0.99, 3.76);
hline6->Draw();
TLine *hline7 = new TLine(0.05, 4.47, 0.99, 4.47);
hline7->Draw();
TLine *hline8 = new TLine(0.05, 5.28, 0.99, 5.28);
hline8->Draw();
TLine *hline9 = new TLine(0.05, 6.165, 0.99, 6.165);
hline9->Draw();
TLine *hline10 = new TLine(0.05, 7.05, 0.99, 7.05);
hline10->Draw();


}
