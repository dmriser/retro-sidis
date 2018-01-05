#include <algorithm> // for std::max

void phih_vs_PT2Vz_v3(int accItN = 2, int xBin = 0, int QQBin = 0, string pipORpim = "pip") // don't compile
{
gStyle->SetOptStat(0);

bool doSaveTxt = 0; // make sure zStartBin and PT2StartBin both = 0 if saving, also cutLast... = 0
bool doSaveTxtRC = 0; // make sure zStartBin and PT2StartBin both = 0 if saving, also cutLast... = 0 ... only do this with final accIt

bool drawPurity = 0;
float accYscaleMax = 0.5;

string hapDIRstring;
if(pipORpim == "pip") hapDIRstring = "NickPipModel";
if(pipORpim == "pim") hapDIRstring = "NickPimModel";
// hapDIRstring = "hap00"; // if you want to use this with pim, you will have to do some modifications below (pip and pim models are the same so only pip files exist)
// hapDIRstring = "hapDefault"; // if you want to use this with pim, you will have to do some modifications below (pip and pim models are the same so only pip files exist)

int binSchemeOpt = 5;

TFile *tfdata = new TFile(Form("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));
TFile *tfmc = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc%i.__0000000000000000__.root", accItN, binSchemeOpt));

bool sameScale = 1; // for the acceptance corrected plots

if(binSchemeOpt == 5)
{
int NphihBins = 36;
int zStartBin = 5;
int cutLastNzBins = 8;
const int NzBins = 18 - zStartBin - cutLastNzBins; // this is the effective number of z bins
int PT2StartBin = 6;
int cutLastNPT2Bins = 9;
const int NPT2Bins = 20 - PT2StartBin - cutLastNPT2Bins; // this is the effective number of PT2 bins
}

TLatex *tlat = new TLatex();
tlat->SetNDC();
tlat->SetTextSize(0.09);

//----------- read in the initial bin category --------------

int category[NzBins][NPT2Bins];

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
ifstream categoryFile;
//categoryFile.open(Form("binCategories/fidcat_%s_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), binSchemeOpt, xBin, QQBin, z, PT2));
categoryFile.open(Form("binCategories/%sCategory.BiSc%i.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), binSchemeOpt, xBin, QQBin, z, PT2));
if(categoryFile) categoryFile>>category[z-zStartBin][PT2-PT2StartBin]; // if the file exists, get the category value
if(!categoryFile) category[z-zStartBin][PT2-PT2StartBin] = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();
}}

//-------------------- data: --------------------------------

TH1F *hdataphih[NzBins][NPT2Bins];
TH1F *hdataphihModified[NzBins][NPT2Bins];

TCanvas *datacan = new TCanvas("datacan", "datacan", 10, 10, 1600, 900);
datacan->Divide(NzBins, NPT2Bins, 0, 0);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
datacan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);
hdataphih[z-zStartBin][PT2-PT2StartBin] = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, z, PT2));

// apply some modifications and further define the bin category:
int Nevents_integratedOverPhih = 0;
int NemptyPhihBins = 0;
for(int phih = 0; phih < NphihBins; phih++)
{
if(hdataphih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(phih+1) < 10) // important condition here! (and more a few lines down)
{
hdataphih[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, 0);
hdataphih[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
}
if(category[z-zStartBin][PT2-PT2StartBin] > 0.5 && fabs(hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetBinCenter(phih+1)) < category[z-zStartBin][PT2-PT2StartBin])
{
hdataphih[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, 0);
hdataphih[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
}
Nevents_integratedOverPhih = Nevents_integratedOverPhih + hdataphih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(phih+1);
if(hdataphih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(phih+1) < 0.1) NemptyPhihBins++;
}

// update/redefine category here:
if(Nevents_integratedOverPhih < 180 && NemptyPhihBins >= 26) category[z-zStartBin][PT2-PT2StartBin] = -99; // bad statistics and bad coverage, don't use this bin
else if(category[z-zStartBin][PT2-PT2StartBin] != -1 && Nevents_integratedOverPhih >= 360 && NemptyPhihBins <= 6) category[z-zStartBin][PT2-PT2StartBin] = 1; // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, A, and A2 for this bin
else category[z-zStartBin][PT2-PT2StartBin] = -13; // Measure only M for all other bins
// end apply some modifications and further define the bin category:

hdataphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.05*hdataphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum());
hdataphih[z-zStartBin][PT2-PT2StartBin]->Draw();
//tlat->DrawLatex(0.25, 0.65, Form("%i events", (int) hdataphih[z-zStartBin][PT2-PT2StartBin]->GetEntries()));
tlat->DrawLatex(0.25, 0.65, Form("%i events", Nevents_integratedOverPhih));
}}

//--------------------- MC: ---------------------------------

TH1F *hgenphih[NzBins][NPT2Bins];
TH1F *hrecphih[NzBins][NPT2Bins];
TH1F *haccphih[NzBins][NPT2Bins];
TH1F *haccphihScaled[NzBins][NPT2Bins];

TH1F *hpurephih[NzBins][NPT2Bins];
TH1F *himpurephih[NzBins][NPT2Bins];
TH1F *hpurityphih[NzBins][NPT2Bins];
TH1F *hpurityphihScaled[NzBins][NPT2Bins];

TCanvas *mccan = new TCanvas("mccan", "mccan", 20, 20, 1600, 900);
mccan->Divide(NzBins, NPT2Bins, 0, 0);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
mccan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);
hgenphih[z-zStartBin][PT2-PT2StartBin] = (TH1F*) tfmc->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, z, PT2));
hrecphih[z-zStartBin][PT2-PT2StartBin] = (TH1F*) tfmc->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, z, PT2));
haccphih[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("haccphih_z%iPT2%i", z, PT2), Form("haccphih_z%iPT2%i", z, PT2), hgenphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(), hgenphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hgenphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
haccphih[z-zStartBin][PT2-PT2StartBin]->Sumw2();
haccphih[z-zStartBin][PT2-PT2StartBin]->Divide(hrecphih[z-zStartBin][PT2-PT2StartBin], hgenphih[z-zStartBin][PT2-PT2StartBin]);

hpurephih[z-zStartBin][PT2-PT2StartBin] = (TH1F*) tfmc->Get(Form("%s_phih_pure_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, z, PT2));
himpurephih[z-zStartBin][PT2-PT2StartBin] = (TH1F*) tfmc->Get(Form("%s_phih_impure_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, z, PT2));
hpurityphih[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hpurityphih_z%iPT2%i", z, PT2), Form("hpurityphih_z%iPT2%i", z, PT2), hpurephih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(), hpurephih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hpurephih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());

for(int k = 0; k < hpurephih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(); k++)
{
float N = hpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(k+1); // numerator
float Ne = hpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinError(k+1);
float D = hpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(k+1) + himpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(k+1); // denominator
float De = sqrt(hpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinError(k+1)*hpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinError(k+1) + himpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinError(k+1)*himpurephih[z-zStartBin][PT2-PT2StartBin]->GetBinError(k+1));
if(D > 0)
{
hpurityphih[z-zStartBin][PT2-PT2StartBin]->SetBinContent(k+1, N/D);
hpurityphih[z-zStartBin][PT2-PT2StartBin]->SetBinError(k+1, sqrt(pow(Ne/D, 2) + pow((N*De)/(D*D), 2)));
}
}

float annoyingScale = 1.1*hgenphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum();
hgenphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, annoyingScale);
hgenphih[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kBlue);
hgenphih[z-zStartBin][PT2-PT2StartBin]->Draw("E");
hrecphih[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kRed);
hrecphih[z-zStartBin][PT2-PT2StartBin]->Draw("E same");

//haccphihScaled[z-zStartBin][PT2-PT2StartBin] = (TH1F*) haccphih[z-zStartBin][PT2-PT2StartBin]->Clone(Form("haccphihScaled_z%iPT2%i", z, PT2));
haccphihScaled[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("haccphihScaled_z%iPT2%i", z, PT2), Form("haccphihScaled_z%iPT2%i", z, PT2), NphihBins, -180, 180);
haccphihScaled[z-zStartBin][PT2-PT2StartBin]->Divide(hrecphih[z-zStartBin][PT2-PT2StartBin], hgenphih[z-zStartBin][PT2-PT2StartBin]);
haccphihScaled[z-zStartBin][PT2-PT2StartBin]->Scale((1.0/accYscaleMax)*annoyingScale);
haccphihScaled[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kGreen);
haccphihScaled[z-zStartBin][PT2-PT2StartBin]->Draw("E same");

hpurityphihScaled[z-zStartBin][PT2-PT2StartBin] = (TH1F*) hpurityphih[z-zStartBin][PT2-PT2StartBin]->Clone(Form("hpurityphihScaled_z%iPT2%i", z, PT2));
hpurityphihScaled[z-zStartBin][PT2-PT2StartBin]->Scale(annoyingScale);
hpurityphihScaled[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kGreen+3);
if(drawPurity) hpurityphihScaled[z-zStartBin][PT2-PT2StartBin]->Draw("E same");
}}

//------------------ data:mc_rec ratio ----------------------

TH1F *hdataphihNormalized[NzBins][NPT2Bins];
TH1F *hrecphihNormalized[NzBins][NPT2Bins];
TH1F *hdata2recRat[NzBins][NPT2Bins];

TCanvas *data2recRatcan = new TCanvas("data2recRatcan", "data2recRatcan", 30, 30, 1600, 900);
data2recRatcan->Divide(NzBins, NPT2Bins, 0, 0);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
data2recRatcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);

float Nevents_data = 0; // GetEntries is not accurate for weighted histos... using float since weighted value is not necessarily an int
float Nevents_rec = 0;
for(int k = 0; k < hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(); k++)
{
Nevents_data = Nevents_data + hdataphih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(k+1);
Nevents_rec = Nevents_rec + hrecphih[z-zStartBin][PT2-PT2StartBin]->GetBinContent(k+1);
}

hdataphihNormalized[z-zStartBin][PT2-PT2StartBin] = (TH1F*) hdataphih[z-zStartBin][PT2-PT2StartBin]->Clone(Form("hdataphihNormalized_z%iPT2%i", z, PT2));
if(Nevents_data > 0) hdataphihNormalized[z-zStartBin][PT2-PT2StartBin]->Scale(1.0/Nevents_data);
hrecphihNormalized[z-zStartBin][PT2-PT2StartBin] = (TH1F*) hrecphih[z-zStartBin][PT2-PT2StartBin]->Clone(Form("hrecphihNormalized_z%iPT2%i", z, PT2));
if(Nevents_rec > 0) hrecphihNormalized[z-zStartBin][PT2-PT2StartBin]->Scale(1.0/Nevents_rec);

hdata2recRat[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hdata2recRat_z%iPT2%i", z, PT2), Form("hdata2recRat_z%iPT2%i", z, PT2), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
hdata2recRat[z-zStartBin][PT2-PT2StartBin]->Divide(hdataphihNormalized[z-zStartBin][PT2-PT2StartBin], hrecphihNormalized[z-zStartBin][PT2-PT2StartBin]);
hdata2recRat[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0.5, 1.5);
hdata2recRat[z-zStartBin][PT2-PT2StartBin]->Draw("E");

}}

//--------------------- acc. corrected ----------------------

TH1F *hcorrphih[NzBins][NPT2Bins];
TF1 *ff[NzBins][NPT2Bins];

TCanvas *corrcan = new TCanvas("corrcan", "corrcan", 40, 40, 1600, 900);
corrcan->Divide(NzBins, NPT2Bins, 0, 0);

float corrmax = -1;

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
corrcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);

hcorrphih[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hcorrphih_z%iPT2%i", z, PT2), Form("hcorrphih_z%iPT2%i", z, PT2), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
hcorrphih[z-zStartBin][PT2-PT2StartBin]->Sumw2();
hcorrphih[z-zStartBin][PT2-PT2StartBin]->Divide(hdataphih[z-zStartBin][PT2-PT2StartBin], haccphih[z-zStartBin][PT2-PT2StartBin]);
hcorrphih[z-zStartBin][PT2-PT2StartBin]->Draw("E");

ff[z-zStartBin][PT2-PT2StartBin] = new TF1(Form("ff_z%iPT2%i", z, PT2), "[0]*(1.0 + [1]*cos((3.1415926/180.0)*x) + [2]*cos(2.0*(3.1415926/180.0)*x))", hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
ff[z-zStartBin][PT2-PT2StartBin]->SetParameters(hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum(), 0.0, 0.0);
if(category[z-zStartBin][PT2-PT2StartBin] == 1)
{
ff[z-zStartBin][PT2-PT2StartBin]->SetParLimits(1, -0.99, 0.99); // if it's a good bin, it's safe to give the parameters freedom (should match below)
ff[z-zStartBin][PT2-PT2StartBin]->SetParLimits(2, -0.99, 0.99);
}
else
{
ff[z-zStartBin][PT2-PT2StartBin]->SetParLimits(1, -0.30, 0.30); // if it's a not so good bin, restrict the parameters to prevent crazy fits (should match below)
ff[z-zStartBin][PT2-PT2StartBin]->SetParLimits(2, -0.30, 0.30);
}
if(category[z-zStartBin][PT2-PT2StartBin] == 1 || category[z-zStartBin][PT2-PT2StartBin] == -13) hcorrphih[z-zStartBin][PT2-PT2StartBin]->Fit(Form("ff_z%iPT2%i", z, PT2), "q", "", hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());

if(hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum() > corrmax) corrmax = hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum();
}}

// re-draw with (optional) same scale, and write out fit results for M, A, and A2 to a file:
for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
corrcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);
if(sameScale) hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.05*corrmax);
if(!sameScale) hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.1*hcorrphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum());
hcorrphih[z-zStartBin][PT2-PT2StartBin]->Draw("E");
//tlat->SetTextSize(0.09);
if(category[z-zStartBin][PT2-PT2StartBin] == 1) tlat->DrawLatex(0.13, 0.41, Form("A^{cos#phi} = %3.3f #pm %3.3f", ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(1), ff[z-zStartBin][PT2-PT2StartBin]->GetParError(1)));
if(category[z-zStartBin][PT2-PT2StartBin] == 1) tlat->DrawLatex(0.13, 0.29, Form("A^{cos2#phi} = %3.3f #pm %3.3f", ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(2), ff[z-zStartBin][PT2-PT2StartBin]->GetParError(2)));
if(category[z-zStartBin][PT2-PT2StartBin] == 1 || category[z-zStartBin][PT2-PT2StartBin] == -13) tlat->DrawLatex(0.13, 0.17, Form("M = %3.1f #pm %3.1f", ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(0), ff[z-zStartBin][PT2-PT2StartBin]->GetParError(0)));

if(category[z-zStartBin][PT2-PT2StartBin] == 1 && doSaveTxt)
{
ofstream AcAccoutfile(Form("AcAcc_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
AcAccoutfile<<ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(1)<<" "<<ff[z-zStartBin][PT2-PT2StartBin]->GetParError(1)<<" "<<ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(2)<<" "<<ff[z-zStartBin][PT2-PT2StartBin]->GetParError(2);
AcAccoutfile.close();
ofstream Moutfile(Form("M_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
Moutfile<<ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(0)<<" "<<ff[z-zStartBin][PT2-PT2StartBin]->GetParError(0);
Moutfile.close();
}
if(category[z-zStartBin][PT2-PT2StartBin] == -13 && doSaveTxt)
{
ofstream Moutfile(Form("M_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
Moutfile<<ff[z-zStartBin][PT2-PT2StartBin]->GetParameter(0)<<" "<<ff[z-zStartBin][PT2-PT2StartBin]->GetParError(0);
Moutfile.close();
}
}}

//------------------- haprad: -------------------------------

TH1F *hsig[NzBins][NPT2Bins];
TH1F *hsib[NzBins][NPT2Bins];
TH1F *htail[NzBins][NPT2Bins];
TH1F *hsigMtail[NzBins][NPT2Bins]; // sigma_rad minus exclusive tail
TH1F *hRC[NzBins][NPT2Bins];

TCanvas *hapradcan = new TCanvas("hapradcan", "hapradcan", 10, 10, 1600, 900);
hapradcan->Divide(NzBins, NPT2Bins, 0, 0);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
hapradcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);

hsig[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hsig_z%iPT2%i", z, PT2), Form("hsig_z%iPT2%i", z, PT2), NphihBins, -180, 180);
hsig[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kOrange-3);
hsib[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hsib_z%iPT2%i", z, PT2), Form("hsib_z%iPT2%i", z, PT2), NphihBins, -180, 180);
hsib[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kBlue);
htail[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("htail_z%iPT2%i", z, PT2), Form("htail_z%iPT2%i", z, PT2), NphihBins, -180, 180);
htail[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kTeal);
hsigMtail[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hsigMtail_z%iPT2%i", z, PT2), Form("hsigMtail_z%iPT2%i", z, PT2), NphihBins, -180, 180);
hsigMtail[z-zStartBin][PT2-PT2StartBin]->SetLineColor(kRed);
hRC[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hRC_z%iPT2%i", z, PT2), Form("hRC_z%iPT2%i", z, PT2), NphihBins, -180, 180);

ifstream datfile(Form("hapradResults/%s/%s_BiSc%i_x%iQQ%iz%iPT2%i.dat", hapDIRstring.c_str(), pipORpim.c_str(), binSchemeOpt, xBin, QQBin, z, PT2));
if(datfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float tempsig, tempsib, temptail;
datfile>>tempsig>>tempsib>>temptail;
if(pipORpim == "pim")
{
tempsig = tempsig - temptail;
temptail = 0;
}
hsig[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, tempsig);
hsig[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
hsib[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, tempsib);
hsib[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
htail[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, temptail);
htail[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
hsigMtail[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, tempsig-temptail);
hsigMtail[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0);
}
}
datfile.close();
hRC[z-zStartBin][PT2-PT2StartBin]->Divide(hsig[z-zStartBin][PT2-PT2StartBin], hsib[z-zStartBin][PT2-PT2StartBin]);

float tempmax = max(hsib[z-zStartBin][PT2-PT2StartBin]->GetMaximum(), hsigMtail[z-zStartBin][PT2-PT2StartBin]->GetMaximum());
tempmax = max(tempmax, htail[z-zStartBin][PT2-PT2StartBin]->GetMaximum());
hsib[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.1*tempmax);
hsib[z-zStartBin][PT2-PT2StartBin]->Draw();
hsigMtail[z-zStartBin][PT2-PT2StartBin]->Draw("same");
htail[z-zStartBin][PT2-PT2StartBin]->Draw("same");
}}

//-------------- acc. and rad. corrected phih ---------------

TH1F *hcorrRCphih[NzBins][NPT2Bins];
TF1 *ffRC[NzBins][NPT2Bins];

TCanvas *corrRCcan = new TCanvas("corrRCcan", "corrRCcan", 40, 40, 1600, 900);
corrRCcan->Divide(NzBins, NPT2Bins, 0, 0);

// float corrmax = -1; already defined above, can reuse it below
corrmax = -1;

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
corrRCcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);

hcorrRCphih[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hcorrRCphih_z%iPT2%i", z, PT2), Form("hcorrRCphih_z%iPT2%i", z, PT2), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetNbins(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->Sumw2();
hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->Divide(hcorrphih[z-zStartBin][PT2-PT2StartBin], hRC[z-zStartBin][PT2-PT2StartBin]);
hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->Draw("E");

ffRC[z-zStartBin][PT2-PT2StartBin] = new TF1(Form("ffRC_z%iPT2%i", z, PT2), "[0]*(1.0 + [1]*cos((3.1415926/180.0)*x) + [2]*cos(2.0*(3.1415926/180.0)*x))", hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());
ffRC[z-zStartBin][PT2-PT2StartBin]->SetParameters(hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum(), 0.0, 0.0);
if(category[z-zStartBin][PT2-PT2StartBin] == 1)
{
ffRC[z-zStartBin][PT2-PT2StartBin]->SetParLimits(1, -0.99, 0.99); // if it's a good bin, it's safe to give the parameters freedom (should match above)
ffRC[z-zStartBin][PT2-PT2StartBin]->SetParLimits(2, -0.99, 0.99);
}
else
{
ffRC[z-zStartBin][PT2-PT2StartBin]->SetParLimits(1, -0.30, 0.30); // if it's a not so good bin, restrict the parameters to prevent crazy fits (should match above)
ffRC[z-zStartBin][PT2-PT2StartBin]->SetParLimits(2, -0.30, 0.30);
}
if(category[z-zStartBin][PT2-PT2StartBin] == 1 || category[z-zStartBin][PT2-PT2StartBin] == -13) hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->Fit(Form("ffRC_z%iPT2%i", z, PT2), "q", "", hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmin(), hdataphih[z-zStartBin][PT2-PT2StartBin]->GetXaxis()->GetXmax());

if(hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum() > corrmax) corrmax = hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum();
}}

// re-draw with (optional) same scale, and write out fit results for M, A, and A2 to a file:
for(int z = zStartBin; z < NzBins + zStartBin; z++) {
for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
corrRCcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);
if(sameScale) hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.05*corrmax);
if(!sameScale) hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetYaxis()->SetRangeUser(0, 1.1*hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->GetMaximum());
hcorrRCphih[z-zStartBin][PT2-PT2StartBin]->Draw("E");
tlat->SetTextSize(0.09);
if(category[z-zStartBin][PT2-PT2StartBin] == 1) tlat->DrawLatex(0.20, 0.81, Form("A^{cos#phi} = %3.3f #pm %3.3f", ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(1), ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(1)));
if(category[z-zStartBin][PT2-PT2StartBin] == 1) tlat->DrawLatex(0.20, 0.69, Form("A^{cos2#phi} = %3.3f #pm %3.3f", ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(2), ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(2)));
if(category[z-zStartBin][PT2-PT2StartBin] == 1 || category[z-zStartBin][PT2-PT2StartBin] == -13) tlat->DrawLatex(0.20, 0.57, Form("M = %3.1f #pm %3.1f", ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(0), ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(0)));

if(category[z-zStartBin][PT2-PT2StartBin] == 1 && doSaveTxtRC)
{
ofstream AcAccoutfile(Form("AcAccwRC_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
AcAccoutfile<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(1)<<" "<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(1)<<" "<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(2)<<" "<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(2);
AcAccoutfile.close();
ofstream Moutfile(Form("MwRC_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
Moutfile<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(0)<<" "<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(0);
Moutfile.close();
}
if(category[z-zStartBin][PT2-PT2StartBin] == -13 && doSaveTxtRC)
{
ofstream Moutfile(Form("MwRC_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, z, PT2));
Moutfile<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParameter(0)<<" "<<ffRC[z-zStartBin][PT2-PT2StartBin]->GetParError(0);
Moutfile.close();
}
}}




}
