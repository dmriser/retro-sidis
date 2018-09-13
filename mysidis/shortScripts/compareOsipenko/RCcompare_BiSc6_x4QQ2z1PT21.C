{
gStyle->SetOptStat(0);

TCanvas *can1 = new TCanvas();

// from lines 23880-23897 from pip_sidis_cs_table_zh_pt2.dat (translated from the 0-360 convention to the -180-180 convention)
TH1F *hrc = new TH1F("hrc", "hrc", 18, -180, 180);
hrc->SetBinContent(1, 1.1218);
hrc->SetBinContent(2, 1.1085);
hrc->SetBinContent(3, 1.0891);
hrc->SetBinContent(4, 1.0701);
hrc->SetBinContent(5, 1.0487);
hrc->SetBinContent(6, 1.0263);
hrc->SetBinContent(7, 1.0106);
hrc->SetBinContent(8, 1.0011);
hrc->SetBinContent(9, 0.9966);

hrc->SetBinContent(10, 0.9966);
hrc->SetBinContent(11, 1.0011);
hrc->SetBinContent(12, 1.0106);
hrc->SetBinContent(13, 1.0263);
hrc->SetBinContent(14, 1.0487);
hrc->SetBinContent(15, 1.0701);
hrc->SetBinContent(16, 1.0891);
hrc->SetBinContent(17, 1.1085);
hrc->SetBinContent(18, 1.1217);

hrc->GetYaxis()->SetRangeUser(0, 1.1*hrc->GetMaximum());
hrc->SetLineWidth(2);
hrc->SetLineColor(kBlack);
hrc->GetXaxis()->SetTitle("phi (deg.)");
hrc->Draw();

///////////////////////
// my results from haprad w/ same kinematics:

TH1F *myrc = new TH1F("myrc", "myrc", 18, -180, 180);
myrc->SetBinContent(1, 2.905/2.809);
myrc->SetBinContent(2, 2.885/2.792);
myrc->SetBinContent(3, 2.850/2.762);
myrc->SetBinContent(4, 2.806/2.722);
myrc->SetBinContent(5, 2.750/2.680);
myrc->SetBinContent(6, 2.688/2.640);
myrc->SetBinContent(7, 2.638/2.607);
myrc->SetBinContent(8, 2.605/2.583);
myrc->SetBinContent(9, 2.588/2.571);
myrc->SetBinContent(10, 2.588/2.571);
myrc->SetBinContent(11, 2.605/2.583);
myrc->SetBinContent(12, 2.638/2.607);
myrc->SetBinContent(13, 2.688/2.640);
myrc->SetBinContent(14, 2.750/2.680);
myrc->SetBinContent(15, 2.806/2.722);
myrc->SetBinContent(16, 2.850/2.762);
myrc->SetBinContent(17, 2.885/2.792);
myrc->SetBinContent(18, 2.905/2.809);

myrc->SetLineWidth(2);
myrc->SetLineColor(kRed);
myrc->Draw("same");

///////////////////////
// my results using the updated but unpublished version of haprad Osipenko sent to me by email on Dec 9, 2015
// using same kinematics

TH1F *hnewsig = new TH1F("hnewsig", "hnewsig", 18, -180, 180);
hnewsig->SetBinContent(1, 0.1151);
hnewsig->SetBinContent(2, 0.1138);
hnewsig->SetBinContent(3, 0.1118);
hnewsig->SetBinContent(4, 0.1098);
hnewsig->SetBinContent(5, 0.1076);
hnewsig->SetBinContent(6, 0.1053);
hnewsig->SetBinContent(7, 0.1036);
hnewsig->SetBinContent(8, 0.1026);
hnewsig->SetBinContent(9, 0.1021);
hnewsig->SetBinContent(10, 0.1021);
hnewsig->SetBinContent(11, 0.1026);
hnewsig->SetBinContent(12, 0.1036);
hnewsig->SetBinContent(13, 0.1053);
hnewsig->SetBinContent(14, 0.1076);
hnewsig->SetBinContent(15, 0.1098);
hnewsig->SetBinContent(16, 0.1118);
hnewsig->SetBinContent(17, 0.1138);
hnewsig->SetBinContent(18, 0.1151);

TH1F *hnewsib = new TH1F("hnewsib", "hnewsib", 18, -180, 180);
hnewsib->SetBinContent(1, 0.1028);
hnewsib->SetBinContent(2, 0.1028);
hnewsib->SetBinContent(3, 0.1028);
hnewsib->SetBinContent(4, 0.1027);
hnewsib->SetBinContent(5, 0.1027);
hnewsib->SetBinContent(6, 0.1026);
hnewsib->SetBinContent(7, 0.1025);
hnewsib->SetBinContent(8, 0.1025);
hnewsib->SetBinContent(9, 0.1025);
hnewsib->SetBinContent(10, 0.1025);
hnewsib->SetBinContent(11, 0.1025);
hnewsib->SetBinContent(12, 0.1025);
hnewsib->SetBinContent(13, 0.1026);
hnewsib->SetBinContent(14, 0.1027);
hnewsib->SetBinContent(15, 0.1027);
hnewsib->SetBinContent(16, 0.1028);
hnewsib->SetBinContent(17, 0.1028);
hnewsib->SetBinContent(18, 0.1028);

TH1F *hnewtail = new TH1F("hnewtail", "hnewtail", 18, -180, 180);
hnewtail->SetBinContent(1, 0.5186E-03);
hnewtail->SetBinContent(2, 0.6249E-03);
hnewtail->SetBinContent(3, 0.9115E-03);
hnewtail->SetBinContent(4, 0.1371E-02);
hnewtail->SetBinContent(5, 0.1361E-02);
hnewtail->SetBinContent(6, 0.8226E-03);
hnewtail->SetBinContent(7, 0.4713E-03);
hnewtail->SetBinContent(8, 0.3104E-03);
hnewtail->SetBinContent(9, 0.2493E-03);
hnewtail->SetBinContent(10, 0.2493E-03);
hnewtail->SetBinContent(11, 0.3104E-03);
hnewtail->SetBinContent(12, 0.4713E-03);
hnewtail->SetBinContent(13, 0.8226E-03);
hnewtail->SetBinContent(14, 0.1361E-02);
hnewtail->SetBinContent(15, 0.1371E-02);
hnewtail->SetBinContent(16, 0.9115E-03);
hnewtail->SetBinContent(17, 0.6249E-03);
hnewtail->SetBinContent(18, 0.5186E-03);

TH1F *hnewrad = (TH1F*) hnewsig->Clone("hnewrad");
hnewrad->SetTitle("hnewrad");
hnewrad->Add(hnewtail, -1.0);

TH1F *hnewrc = new TH1F("hnewrc", "hnewrc", 18, -180, 180);
hnewrc->Divide(hnewsig, hnewsib);

hnewrc->SetLineColor(kBlue);
hnewrc->SetLineWidth(2);
hnewrc->Draw("same");

TCanvas *can2 = new TCanvas();
hnewrad->GetYaxis()->SetRangeUser(0, 1.1*hnewrad->GetMaximum());
hnewrad->SetLineColor(kRed);
hnewrad->SetLineWidth(2);
hnewrad->Draw();
hnewsib->SetLineColor(kBlue);
hnewsib->SetLineWidth(2);
hnewsib->Draw("same");
hnewtail->SetLineColor(kTeal);
hnewtail->SetLineWidth(2);
hnewtail->Draw("same");

///////////////////////
// my results using the updated but unpublished version of haprad Osipenko sent to me by email on Dec 9, 2015
// difference from above: in ihaprad.f, I set h3=0 and h4=0
// using same kinematics

TH1F *hnew00sig = new TH1F("hnew00sig", "hnew00sig", 18, -180, 180);
hnew00sig->SetBinContent(1,  0.1146);
hnew00sig->SetBinContent(2,  0.1133);
hnew00sig->SetBinContent(3,  0.1114);
hnew00sig->SetBinContent(4,  0.1094);
hnew00sig->SetBinContent(5,  0.1073);
hnew00sig->SetBinContent(6,  0.1051);
hnew00sig->SetBinContent(7,  0.1035);
hnew00sig->SetBinContent(8,  0.1025);
hnew00sig->SetBinContent(9,  0.1021);
hnew00sig->SetBinContent(10, 0.1021);
hnew00sig->SetBinContent(11, 0.1025);
hnew00sig->SetBinContent(12, 0.1035);
hnew00sig->SetBinContent(13, 0.1051);
hnew00sig->SetBinContent(14, 0.1073);
hnew00sig->SetBinContent(15, 0.1094);
hnew00sig->SetBinContent(16, 0.1114);
hnew00sig->SetBinContent(17, 0.1133);
hnew00sig->SetBinContent(18, 0.1146);

TH1F *hnew00sib = new TH1F("hnew00sib", "hnew00sib", 18, -180, 180);
hnew00sib->SetBinContent(1,  0.1024);
hnew00sib->SetBinContent(2,  0.1024);
hnew00sib->SetBinContent(3,  0.1024);
hnew00sib->SetBinContent(4,  0.1024);
hnew00sib->SetBinContent(5,  0.1024);
hnew00sib->SetBinContent(6,  0.1024);
hnew00sib->SetBinContent(7,  0.1024);
hnew00sib->SetBinContent(8,  0.1024);
hnew00sib->SetBinContent(9,  0.1024);
hnew00sib->SetBinContent(10, 0.1024);
hnew00sib->SetBinContent(11, 0.1024);
hnew00sib->SetBinContent(12, 0.1024);
hnew00sib->SetBinContent(13, 0.1024);
hnew00sib->SetBinContent(14, 0.1024);
hnew00sib->SetBinContent(15, 0.1024);
hnew00sib->SetBinContent(16, 0.1024);
hnew00sib->SetBinContent(17, 0.1024);
hnew00sib->SetBinContent(18, 0.1024);

TH1F *hnew00tail = new TH1F("hnew00tail", "hnew00tail", 18, -180, 180);
hnew00tail->SetBinContent(1,  0.5186E-03);
hnew00tail->SetBinContent(2,  0.6249E-03);
hnew00tail->SetBinContent(3,  0.9115E-03);
hnew00tail->SetBinContent(4,  0.1371E-02);
hnew00tail->SetBinContent(5,  0.1361E-02);
hnew00tail->SetBinContent(6,  0.8226E-03);
hnew00tail->SetBinContent(7,  0.4713E-03);
hnew00tail->SetBinContent(8,  0.3104E-03);
hnew00tail->SetBinContent(9,  0.2493E-03);
hnew00tail->SetBinContent(10, 0.2493E-03);
hnew00tail->SetBinContent(11, 0.3104E-03);
hnew00tail->SetBinContent(12, 0.4713E-03);
hnew00tail->SetBinContent(13, 0.8226E-03);
hnew00tail->SetBinContent(14, 0.1361E-02);
hnew00tail->SetBinContent(15, 0.1371E-02);
hnew00tail->SetBinContent(16, 0.9115E-03);
hnew00tail->SetBinContent(17, 0.6249E-03);
hnew00tail->SetBinContent(18, 0.5186E-03);

TH1F *hnew00rad = (TH1F*) hnew00sig->Clone("hnew00rad");
hnew00rad->SetTitle("hnew00rad");
hnew00rad->Add(hnew00tail, -1.0);

TH1F *hnew00rc = new TH1F("hnew00rc", "hnew00rc", 18, -180, 180);
hnew00rc->Divide(hnew00sig, hnew00sib);

hnew00rc->SetLineColor(kGreen);
hnew00rc->SetLineWidth(2);
can1->cd();
hnew00rc->Draw("same");

TCanvas *can3 = new TCanvas();
hnew00rad->GetYaxis()->SetRangeUser(0, 1.1*hnew00rad->GetMaximum());
hnew00rad->SetLineColor(kRed);
hnew00rad->SetLineWidth(2);
hnew00rad->Draw();
hnew00sib->SetLineColor(kBlue);
hnew00sib->SetLineWidth(2);
hnew00sib->Draw("same");
hnew00tail->SetLineColor(kTeal);
hnew00tail->SetLineWidth(2);
hnew00tail->Draw("same");



}
