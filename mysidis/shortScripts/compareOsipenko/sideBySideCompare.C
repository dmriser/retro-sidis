#include <algorithm> // for std::max

//void sideBySideCompare(int xBin = 8, int QQBin = 3, int zBin = 10, int PT2Bin = 1)
void sideBySideCompare(int xBin = 8, int QQBin = 4, int zBin = 11, int PT2Bin = 0)
{
gStyle->SetOptStat(0);

const int NphihBins = 18;

TLatex *tlat = new TLatex();
tlat->SetNDC();
tlat->SetTextSize(0.06);

TCanvas *can = new TCanvas("can", "can", 5, 5, 1400, 900);
can->Divide(3, 3, 0.00001, 0.00001);

///////////////////////// Osipenko results: //////////////////////

TFile *osifile = new TFile("pip_sidis_cs_table_zh_pt2.root");
TH1F *osiphih = (TH1F*) osifile->Get(Form("hphih_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin));
TH1F *osiphih_sys = (TH1F*) osifile->Get(Form("hphih_sys_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin));
TH1F *osiphih_RC = (TH1F*) osifile->Get(Form("hphih_RC_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin));
osiphih->GetYaxis()->SetRangeUser(-0.2*osiphih->GetMaximum(), 1.2*osiphih->GetMaximum());
can->cd(1);
osiphih->Draw();
osiphih_sys->SetFillColor(kBlue);
osiphih_sys->SetFillStyle(3005);
osiphih_sys->Draw("E3 same");

TF1 *osiff = new TF1("osiff", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
osiff->SetParameters(osiphih->GetMaximum(), 0.0, 0.0);
osiphih->Fit("osiff", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.38, Form("Ac = %3.4f #pm %3.4f", osiff->GetParameter(1), osiff->GetParError(1)));
tlat->DrawLatex(0.15, 0.32, Form("Acc = %3.4f #pm %3.4f", osiff->GetParameter(2), osiff->GetParError(2)));

can->cd(2);
osiphih_RC->GetYaxis()->SetRangeUser(0, 1.2*osiphih_RC->GetMaximum());
osiphih_RC->Draw();

can->cd(3);
TH1F *osiNORCphih = new TH1F("osiNORCphih", "osiNORCphih", NphihBins, -180, 180);
osiNORCphih->Sumw2();
osiNORCphih->Multiply(osiphih, osiphih_RC);
osiNORCphih->GetYaxis()->SetRangeUser(0, 1.2*osiNORCphih->GetMaximum());
osiNORCphih->Draw();

TF1 *osiffNORC = new TF1("osiffNORC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
osiffNORC->SetParameters(osiNORCphih->GetMaximum(), 0.0, 0.0);
osiNORCphih->Fit("osiffNORC", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.38, Form("Ac = %3.4f #pm %3.4f", osiffNORC->GetParameter(1), osiffNORC->GetParError(1)));
tlat->DrawLatex(0.15, 0.32, Form("Acc = %3.4f #pm %3.4f", osiffNORC->GetParameter(2), osiffNORC->GetParError(2)));

///////////////////////// my results: ///////////////////////////

TFile *tfdata = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc6.MoCo11.__0000000000000000__.root");
TFile *tfmc = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc6.__0000000000000000__.root");
//TFile *tfdata = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc6.MoCo11.__0000000000000000__.minParticleEnergy0.3.root");
//TFile *tfmc = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc6.__0000000000000000__.minParticleEnergy0.3.root");
//TFile *tfdata = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc6.MoCo11.__0000000000000000__.minParticleEnergy0.4.root");
//TFile *tfmc = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc6.__0000000000000000__.minParticleEnergy0.4.root");

TH1F *hdataphih = (TH1F*) tfdata->Get(Form("rec_pip_phih_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin));
can->cd(4);
hdataphih->GetYaxis()->SetRangeUser(0, 1.2*hdataphih->GetMaximum());
hdataphih->Draw();

///////////////////////// MC: ///////////////////////////
can->cd(5);
TH1F *hgenphih, *hrecphih, *haccphih, *haccphihScaled, *hpurephih, *himpurephih, *hpurityphih, *hpurityphihScaled;
hgenphih = (TH1F*) tfmc->Get(Form("gen_pip_phih_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin));
hrecphih = (TH1F*) tfmc->Get(Form("rec_pip_phih_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin));
haccphih = new TH1F("haccphih", "haccphih", NphihBins, -180, 180);
haccphih->Sumw2();
haccphih->Divide(hrecphih, hgenphih);
haccphihScaled = new TH1F("haccphihScaled", "haccphihScaled", NphihBins, -180, 180);
haccphihScaled->Sumw2();
haccphihScaled->Divide(hrecphih, hgenphih);

hpurephih = (TH1F*) tfmc->Get(Form("pip_phih_pure_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin));
himpurephih = (TH1F*) tfmc->Get(Form("pip_phih_impure_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin));
hpurityphih = new TH1F("hpurityphih", "hpurityphih", NphihBins, -180, 180);
hpurityphihScaled = new TH1F("hpurityphihScaled", "hpurityphihScaled", NphihBins, -180, 180);

for(int k = 0; k < NphihBins; k++)
{
float N = hpurephih->GetBinContent(k+1); // numerator
float Ne = hpurephih->GetBinError(k+1);
float D = hpurephih->GetBinContent(k+1) + himpurephih->GetBinContent(k+1); // denominator
float De = sqrt(hpurephih->GetBinError(k+1)*hpurephih->GetBinError(k+1) + himpurephih->GetBinError(k+1)*himpurephih->GetBinError(k+1));
if(D > 0)
{
hpurityphih->SetBinContent(k+1, N/D);
hpurityphih->SetBinError(k+1, sqrt(pow(Ne/D, 2) + pow((N*De)/(D*D), 2)));
hpurityphihScaled->SetBinContent(k+1, N/D);
hpurityphihScaled->SetBinError(k+1, sqrt(pow(Ne/D, 2) + pow((N*De)/(D*D), 2)));
}
}

float mcscale = 1.2*hgenphih->GetMaximum();
hgenphih->GetYaxis()->SetRangeUser(0, mcscale);
hgenphih->SetLineColor(kBlue);
hgenphih->GetXaxis()->SetTitle("phi_h (deg.)");
hgenphih->SetTitle("MC gen(blue), rec(red), acc(l. green), purity(d. green)");
hgenphih->Draw("E");
hrecphih->SetLineColor(kRed);
hrecphih->Draw("E same");

haccphihScaled->Scale(mcscale);
haccphihScaled->SetLineColor(kGreen);
haccphihScaled->Draw("E same");

hpurityphihScaled->Scale(mcscale);
hpurityphihScaled->SetLineColor(kGreen+3);
hpurityphihScaled->Draw("E same");

///////////////////////// RC: ///////////////////////////

can->cd(6);
TH1F *hsig = new TH1F("hsig", "hsig", NphihBins, -180, 180);
hsig->Sumw2();
TH1F *hsib = new TH1F("hsib", "hsib", NphihBins, -180, 180);
hsib->Sumw2();
TH1F *hsigMtail = new TH1F("hsigMtail", "hsigMtail", NphihBins, -180, 180);
TH1F *htail = new TH1F("htail", "htail", NphihBins, -180, 180);
TH1F *hRC = new TH1F("hRC", "hRC", NphihBins, -180, 180);
hRC->Sumw2();

ifstream hapfile(Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hapDefault/pip_BiSc6_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin));

if(hapfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
hapfile>>sig>>sib>>tail;
hsig->SetBinContent(phih+1, sig);
hsig->SetBinError(phih+1, 0);
hsib->SetBinContent(phih+1, sib);
hsib->SetBinError(phih+1, 0);
hsigMtail->SetBinContent(phih+1, sig-tail);
hsigMtail->SetBinError(phih+1, 0);
htail->SetBinContent(phih+1, tail);
htail->SetBinError(phih+1, 0);
}
}
hapfile.close();

hRC->Divide(hsig, hsib);

hsib->GetYaxis()->SetRangeUser(0, 1.2*max(max(hsib->GetMaximum(), htail->GetMaximum()), hsigMtail->GetMaximum()));
hsib->SetLineColor(kBlue);
hsib->GetXaxis()->SetTitle("phi_h (deg.)");
hsib->SetTitle("haprad: #sigma_{Born} (blue), #sigma_{rad} (red), #sigma_{tail} (teal)");
hsib->Draw();
hsigMtail->SetLineColor(kRed);
hsigMtail->Draw("same");
htail->SetLineColor(kTeal);
htail->Draw("same");

can->cd(7);
hRC->GetYaxis()->SetRangeUser(0, 1.2*hRC->GetMaximum());
hRC->Draw();

///////////////////////// corr: ///////////////////////////

can->cd(8);

TH1F *hcorr = new TH1F("hcorr", "hcorr", NphihBins, -180, 180);
hcorr->Sumw2();
hcorr->Divide(hdataphih, haccphih);
hcorr->GetYaxis()->SetRangeUser(0, 1.2*hcorr->GetMaximum());
hcorr->GetXaxis()->SetTitle("phi_h (deg.)");
hcorr->SetTitle("acceptance corrected data");
hcorr->Draw();

TF1 *ff = new TF1("ff", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ff->SetParameters(hcorr->GetMaximum(), 0.0, 0.0);
hcorr->Fit("ff", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.38, Form("Ac = %3.4f #pm %3.4f", ff->GetParameter(1), ff->GetParError(1)));
tlat->DrawLatex(0.15, 0.32, Form("Acc = %3.4f #pm %3.4f", ff->GetParameter(2), ff->GetParError(2)));

///////////////////////// corrRC: ///////////////////////////

can->cd(9);

TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", NphihBins, -180, 180);
hcorrRC->Sumw2();
hcorrRC->Divide(hcorr, hRC);
hcorrRC->GetYaxis()->SetRangeUser(0, 1.2*hcorrRC->GetMaximum());
hcorrRC->GetXaxis()->SetTitle("phi_h (deg.)");
hcorrRC->SetTitle("acc. corr. and rad. corr data");
hcorrRC->Draw();

TF1 *ffRC = new TF1("ffRC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffRC->SetParameters(hcorrRC->GetMaximum(), 0.0, 0.0);
hcorrRC->Fit("ffRC", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.38, Form("Ac = %3.4f #pm %3.4f", ffRC->GetParameter(1), ffRC->GetParError(1)));
tlat->DrawLatex(0.15, 0.32, Form("Acc = %3.4f #pm %3.4f", ffRC->GetParameter(2), ffRC->GetParError(2)));



//////////////

TH1F *rat = new TH1F("rat", "rat", hRC->GetXaxis()->GetNbins(), -180, 180);
rat->Divide(osiphih_RC, hRC);
new TCanvas;
rat->GetYaxis()->SetRangeUser(0, 1.1);
rat->Draw();

}
