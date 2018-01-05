#include <algorithm> // for std::max

//int accItN = 2; // 0(flat), 1(default haprad), or 2(Nick)
//int hapItN = 2; // 0(flat), 1(default haprad), 2(Nick), or 9(no RC applied)

void phih_simple(int xBin = 0, int QQBin = 0, int zBin = 4, int PT2Bin = 1, int accItN = 2, int hapItN = 2, string pipORpim = "pim")
{
gStyle->SetOptStat(0);

bool doSaveTxt = 0;
bool doSavePic = 0;
if(hapItN == 9) doSavePic = 0; // saving pics for hapItN == 9 would be redundant and unnecessary

int binSchemeOpt = 6;
int NphihBins;
if(binSchemeOpt == 5) NphihBins = 36;
if(binSchemeOpt == 6) NphihBins = 18;

TFile *tfdata = new TFile(Form("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));
TFile *tfmc = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc%i.__0000000000000000__.root", accItN, binSchemeOpt));

TCanvas *can = new TCanvas("can", "can", 5, 5, 1400, 800);
can->Divide(3, 2);

TLatex *tlat = new TLatex();
tlat->SetNDC();
tlat->SetTextSize(0.06);

//----------- read in the initial bin category --------------

int category;
ifstream categoryFile;
categoryFile.open(Form("binCategories/%sCategory.BiSc%i.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(categoryFile) categoryFile>>category; // if the file exists, get the category value
if(!categoryFile) category = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();

int phihcut = -123;
if(category > 0.5) phihcut = category;

//-------------------- data: --------------------------------

TH1F *hdataphih = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hdataphih->SetName("data"); // so above and below don't have same name (which messes stuff up)
TH1F *hdataphihModified = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

can->cd(1);

// apply some modifications and further define the bin category:
int NemptyPhihBins = 0;
for(int phih = 0; phih < NphihBins; phih++)
{
if(hdataphihModified->GetBinContent(phih+1) < 10) // important condition here! (and more a few lines down)
{
hdataphihModified->SetBinContent(phih+1, 0);
hdataphihModified->SetBinError(phih+1, 0);
}
if(category > 0.5 && fabs(hdataphihModified->GetXaxis()->GetBinCenter(phih+1)) < category)
{
hdataphihModified->SetBinContent(phih+1, 0);
hdataphihModified->SetBinError(phih+1, 0);
}
if(hdataphihModified->GetBinContent(phih+1) < 0.1) NemptyPhihBins++;
}

// update/redefine category here:
if(hdataphihModified->Integral() < 180 && NemptyPhihBins >= 26) category = -99; // bad statistics and bad coverage, don't use this bin
else if(category != -1 && hdataphihModified->Integral() >= 360 && NemptyPhihBins <= 6) category = 1; // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, Ac, and Acc for this bin
else category = -13; // Measure only M for all other bins
// note: category is further updated at the top of the corrRC section

hdataphih->GetYaxis()->SetRangeUser(0, 1.2*hdataphih->GetMaximum());
hdataphih->GetXaxis()->SetTitle("phi_h (deg.)");
hdataphih->SetTitle(Form("raw data (%i %i %i %i)", xBin, QQBin, zBin, PT2Bin));
hdataphih->Draw();

TLine *dataLineLeft = new TLine(-phihcut, 0, -phihcut, hdataphih->GetMaximum());
dataLineLeft->SetLineColor(kRed);
dataLineLeft->SetLineWidth(2);
TLine *dataLineRight = new TLine(phihcut, 0, phihcut, hdataphih->GetMaximum());
dataLineRight->SetLineColor(kRed);
dataLineRight->SetLineWidth(2);
if(phihcut > 0.5) dataLineLeft->Draw();
if(phihcut > 0.5) dataLineRight->Draw();
tlat->DrawLatex(0.32, 0.82, Form("%i events", (int) hdataphih->Integral()));

can->cd(2);
hdataphihModified->GetYaxis()->SetRangeUser(0, 1.2*hdataphihModified->GetMaximum());
hdataphihModified->GetXaxis()->SetTitle("phi_h (deg.)");
hdataphihModified->SetTitle("modified data");
hdataphihModified->Draw();
tlat->DrawLatex(0.32, 0.82, Form("%i events", (int) hdataphihModified->Integral()));

//--------------------- MC: ---------------------------------

TH1F *hgenphih, *hrecphih, *haccphih, *haccphihScaled, *hpurephih, *himpurephih, *hpurityphih, *hpurityphihScaled;

can->cd(3);
hgenphih = (TH1F*) tfmc->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hrecphih = (TH1F*) tfmc->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
haccphih = new TH1F(Form("haccphih_z%iPT2%i", zBin, PT2Bin), Form("haccphih_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
haccphih->Sumw2();
haccphih->Divide(hrecphih, hgenphih);
haccphihScaled = new TH1F(Form("haccphihScaled_z%iPT2%i", zBin, PT2Bin), Form("haccphihScaled_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
haccphihScaled->Sumw2();
haccphihScaled->Divide(hrecphih, hgenphih);

hpurephih = (TH1F*) tfmc->Get(Form("%s_phih_pure_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
himpurephih = (TH1F*) tfmc->Get(Form("%s_phih_impure_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hpurityphih = new TH1F(Form("hpurityphih_z%iPT2%i", zBin, PT2Bin), Form("hpurityphih_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
hpurityphihScaled = new TH1F(Form("hpurityphihScaled_z%iPT2%i", zBin, PT2Bin), Form("hpurityphihScaled_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);

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

TLine *mcLineLeft = new TLine(-phihcut, 0, -phihcut, hgenphih->GetMaximum());
mcLineLeft->SetLineColor(kRed);
mcLineLeft->SetLineWidth(2);
TLine *mcLineRight = new TLine(phihcut, 0, phihcut, hgenphih->GetMaximum());
mcLineRight->SetLineColor(kRed);
mcLineRight->SetLineWidth(2);
if(phihcut > 0.5) mcLineLeft->Draw();
if(phihcut > 0.5) mcLineRight->Draw();

TF1 *ffgen = new TF1("ffgen", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffgen->SetLineColor(kBlue);
ffgen->SetParameters(hgenphih->GetMaximum(), 0.0, 0.0);
ffgen->SetParLimits(1, -0.99, 0.99);
ffgen->SetParLimits(2, -0.99, 0.99);
if(hgenphih->Integral() > 36) hgenphih->Fit("ffgen", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.82, Form("(%i, %3.4f, %3.4f)", (int) ffgen->GetParameter(0), ffgen->GetParameter(1), ffgen->GetParameter(2)));

//------------------ haprad: --------------------------------

if(hapItN != 9)
{
can->cd(4);

TH1F *hsig = new TH1F("hsig", "hsig", NphihBins, -180, 180);
hsig->Sumw2();
TH1F *hsib = new TH1F("hsib", "hsib", NphihBins, -180, 180);
hsib->Sumw2();
TH1F *hsigMtail = new TH1F("hsigMtail", "hsigMtail", NphihBins, -180, 180);
TH1F *htail = new TH1F("htail", "htail", NphihBins, -180, 180);
TH1F *hRC = new TH1F("hRC", "hRC", NphihBins, -180, 180);
hRC->Sumw2();

string happath;
if(hapItN == 0) happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hap00/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin); // always pip
if(hapItN == 1) happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hapDefault/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin); // always pip
if(hapItN == 2 && pipORpim == "pip") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPipModel/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin);
if(hapItN == 2 && pipORpim == "pim") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPimModel/pim_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin);
ifstream hapfile(happath.c_str());

if(hapfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
hapfile>>sig>>sib>>tail;
if(pipORpim == "pim")
{
sig = sig - tail;
tail = 0;
}
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

TF1 *ffsib = new TF1("ffsib", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffsib->SetLineColor(kBlue);
ffsib->SetParameters(hsib->GetMaximum(), 0.0, 0.0);
ffsib->SetParLimits(1, -0.99, 0.99);
ffsib->SetParLimits(2, -0.99, 0.99);
if(hsib->Integral() > 0.00000001) hsib->Fit("ffsib", "W", "", -180, 180); // "W"  Set all weights to 1 for non empty bins; ignore error bars (convenient since no errors on this histo)

tlat->DrawLatex(0.15, 0.82, Form("(%3.2f, %3.4f, %3.4f)", ffsib->GetParameter(0), ffsib->GetParameter(1), ffsib->GetParameter(2)));
}

//------------------- corr: ---------------------------------

can->cd(5);

TH1F *hcorr = new TH1F("hcorr", "hcorr", NphihBins, -180, 180);
hcorr->Sumw2();
hcorr->Divide(hdataphihModified, haccphih);
hcorr->GetYaxis()->SetRangeUser(0, 1.1*hcorr->GetMaximum());
hcorr->GetXaxis()->SetTitle("phi_h (deg.)");
hcorr->SetTitle("acceptance corrected data");
hcorr->Draw();

TF1 *ffcorr = new TF1("ffcorr", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffcorr->SetLineColor(kBlue);
ffcorr->SetParameters(hcorr->GetMaximum(), 0.0, 0.0);
if(category == 1)
{
ffcorr->SetParLimits(1, -0.99, 0.99);
ffcorr->SetParLimits(2, -0.99, 0.99);
}
else
{
ffcorr->SetParLimits(1, -0.30, 0.30);
ffcorr->SetParLimits(2, -0.30, 0.30);
}
if(hcorr->Integral() > 36) hcorr->Fit("ffcorr", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.32, Form("A0 = %3.2f #pm %3.2f", ffcorr->GetParameter(0), ffcorr->GetParError(0)));
tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorr->GetParameter(1), ffcorr->GetParError(1)));
tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorr->GetParameter(2), ffcorr->GetParError(2)));

//------------------- corrRC: -------------------------------

if(hapItN != 9)
{
can->cd(6);

if(hsib->Integral() < 0.00000001 || hsig->Integral() < 0.00000001) category = -2; // defining category = -2 here means no haprad results

TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", NphihBins, -180, 180);
hcorrRC->Sumw2();
hcorrRC->Divide(hcorr, hRC);
hcorrRC->GetYaxis()->SetRangeUser(0, 1.1*hcorrRC->GetMaximum());
hcorrRC->GetXaxis()->SetTitle("phi_h (deg.)");
hcorrRC->SetTitle("acc. corr. and rad. corr data");
hcorrRC->Draw();

TF1 *ffcorrRC = new TF1("ffcorrRC", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffcorrRC->SetLineColor(kBlue);
ffcorrRC->SetParameters(hcorrRC->GetMaximum(), 0.0, 0.0);
if(category == 1)
{
ffcorrRC->SetParLimits(1, -0.99, 0.99);
ffcorrRC->SetParLimits(2, -0.99, 0.99);
}
else
{
ffcorrRC->SetParLimits(1, -0.30, 0.30);
ffcorrRC->SetParLimits(2, -0.30, 0.30);
}
if(hcorrRC->Integral() > 36) hcorrRC->Fit("ffcorrRC", "", "", -180, 180);

tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRC->GetParameter(0), ffcorrRC->GetParError(0)));
tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRC->GetParameter(1), ffcorrRC->GetParError(1)));
tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRC->GetParameter(2), ffcorrRC->GetParError(2)));

tlat->DrawLatex(0.25, 0.14, Form("category %i", category));
}

//-------------------- save: --------------------------------

if(doSaveTxt)
{
if(category == 1)
{
ofstream outfile(Form("%s_AcAcc_it%i_hap%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, hapItN, binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(hapItN == 9) outfile<<ffcorr->GetParameter(1)<<" "<<ffcorr->GetParError(1)<<" "<<ffcorr->GetParameter(2)<<" "<<ffcorr->GetParError(2);
if(hapItN != 9) outfile<<ffcorrRC->GetParameter(1)<<" "<<ffcorrRC->GetParError(1)<<" "<<ffcorrRC->GetParameter(2)<<" "<<ffcorrRC->GetParError(2);
outfile.close();

ofstream outfileM(Form("%s_M_it%i_hap%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, hapItN, binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(hapItN == 9) outfileM<<ffcorr->GetParameter(0)<<" "<<ffcorr->GetParError(0);
if(hapItN != 9) outfileM<<ffcorrRC->GetParameter(0)<<" "<<ffcorrRC->GetParError(0);
outfileM.close();
}

if(category == -13)
{
ofstream outfileM(Form("%s_M_it%i_hap%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, hapItN, binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(hapItN == 9) outfileM<<ffcorr->GetParameter(0)<<" "<<ffcorr->GetParError(0);
if(hapItN != 9) outfileM<<ffcorrRC->GetParameter(0)<<" "<<ffcorrRC->GetParError(0);
outfileM.close();
}
}

if(doSavePic) can->SaveAs(Form("%s_it%i_hap%i_BiSc%i_x%iQQ%iz%iPT2%i.png", pipORpim.c_str(), accItN, hapItN, binSchemeOpt, xBin, QQBin, zBin, PT2Bin));



}
