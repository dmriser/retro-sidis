// this is a long script... search "Checkpoint" (w/ lowercase C) for key locations
// a lot of things are hard-coded, be very careful if you change anything
// this works for BiSc5

//void systematics_v1(int xBin = 0, int QQBin = 0, int zBin = 5, int PT2Bin = 5, string pipORpim = "pip")
//void systematics_v1(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 3, string pipORpim = "pip")
void systematics_v1(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 0, string pipORpim = "pip")
{
gStyle->SetOptStat(0);

bool doSaveTxt = 0;

int NphihBins = 36;

const int Nsources = 13; // number of sources of systematic errors
const int variationsPerSource = 2; // can't get vector<TFile> to work so this must be a constant for now ... should be okay for the time being

string sourceName[Nsources] = {"e_zvert", "e_ECsamp", "e_ECoVi", "e_ECgeo", "e_CCthMatch", "e_R1fid", "e_R3fid", "e_CCfid", "pi_vvp", "pi_R1fid", "phih_fid", "accModel", "hapModel"};

float M_sysErrorPiece[Nsources]; // pieces of the whole sys error
float Ac_sysErrorPiece[Nsources];
float Acc_sysErrorPiece[Nsources];

float M_sumOfSquaredErrors = 0;
float Ac_sumOfSquaredErrors = 0;
float Acc_sumOfSquaredErrors = 0;

float M_sysErrorTotal;
float Ac_sysErrorTotal;
float Acc_sysErrorTotal;

TH1F *hM_sysEcontributions = new TH1F("hM_sysEcontributions", "hM_sysEcontributions", Nsources, 0, Nsources);
TH1F *hAc_sysEcontributions = new TH1F("hAc_sysEcontributions", "hAc_sysEcontributions", Nsources, 0, Nsources);
TH1F *hAcc_sysEcontributions = new TH1F("hAcc_sysEcontributions", "hAcc_sysEcontributions", Nsources, 0, Nsources);

TFile *tfdata[Nsources][variationsPerSource];
tfdata[0][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__-1000000000000000__.root");
tfdata[0][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__1000000000000000__.root");
tfdata[1][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0-100000000000000__.root");
tfdata[1][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0100000000000000__.root");
tfdata[2][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__00-10000000000000__.root");
tfdata[2][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0010000000000000__.root");
tfdata[3][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__000-1000000000000__.root");
tfdata[3][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0001000000000000__.root");
tfdata[4][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000-100000000000__.root");
tfdata[4][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000100000000000__.root");
tfdata[5][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__00000-10000000000__.root");
tfdata[5][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000010000000000__.root");
tfdata[6][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__000000-1000000000__.root");
tfdata[6][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000001000000000__.root");
tfdata[7][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__00000000-10000000__.root"); // note: skipping some on purpose (not testing systematics for those)
tfdata[7][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000010000000__.root");
tfdata[8][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000-100-100__.root");
tfdata[8][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000100100__.root");
tfdata[9][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__00000000000-100-10__.root");
tfdata[9][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000010010__.root");
tfdata[10][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root"); // [10] is for testing phih cuts (bins with a phih cut only)... don't change the number!
tfdata[10][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root");
tfdata[11][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root"); // for acc. model dependence. See note below (at tfmc[11][0])!!
tfdata[11][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root");
tfdata[12][0] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root"); // for hap. model dependence. See note below (at tfmc[12][0])!!
tfdata[12][1] = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root");

TFile *tfmc[Nsources][variationsPerSource];
tfmc[0][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__-1000000000000000__.root");
tfmc[0][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__1000000000000000__.root");
tfmc[1][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0-100000000000000__.root");
tfmc[1][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0100000000000000__.root");
tfmc[2][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__00-10000000000000__.root");
tfmc[2][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0010000000000000__.root");
tfmc[3][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__000-1000000000000__.root");
tfmc[3][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0001000000000000__.root");
tfmc[4][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000-100000000000__.root");
tfmc[4][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000100000000000__.root");
tfmc[5][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__00000-10000000000__.root");
tfmc[5][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000010000000000__.root");
tfmc[6][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__000000-1000000000__.root");
tfmc[6][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000001000000000__.root");
tfmc[7][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__00000000-10000000__.root"); // note: skipping some on purpose (not testing systematics for those)
tfmc[7][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000010000000__.root");
tfmc[8][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000-100-100__.root");
tfmc[8][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000100100__.root");
tfmc[9][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__00000000000-100-10__.root");
tfmc[9][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000010010__.root");
tfmc[10][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root"); // [10] is for testing phih cuts (bins with a phih cut only)... don't change the number!
tfmc[10][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");
tfmc[11][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it1.s1.n32255.BiSc5.__0000000000000000__.root"); // need to do a work-around here since there's only one variation... the "loose" one is the variation and the "tight" one is just the nominal case so it won't contribute anything to the error (nominal - tight = 0)... just remember to divide by sqrt(1) instead of sqrt(2)!!
tfmc[11][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");
tfmc[12][0] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root"); // for hap. model dependence. similar work-around here as above. see below where the haprad files are read in for details
tfmc[12][1] = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");

TLatex *tlat = new TLatex();
tlat->SetNDC();
tlat->SetTextSize(0.06);

//----------- read in the initial bin category --------------

int category;
ifstream categoryFile;
categoryFile.open(Form("binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
if(categoryFile) categoryFile>>category; // if the file exists, get the category value
if(!categoryFile) category = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();

//-----------------------------------------------------------
//--------------- do the nominal case -----------------------
//-----------------------------------------------------------

//-------------------- data: --------------------------------

TCanvas *nomcan = new TCanvas();

TFile *tfdataNom = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc5.MoCo11.__0000000000000000__.root");
TFile *tfmcNom = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");

TH1F *hdataphihModified = (TH1F*) tfdataNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

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

//--------------------- MC: ---------------------------------

TH1F *hgenphih, *hrecphih, *haccphih;

hgenphih = (TH1F*) tfmcNom->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hrecphih = (TH1F*) tfmcNom->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
haccphih = new TH1F(Form("haccphih_z%iPT2%i", zBin, PT2Bin), Form("haccphih_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
haccphih->Sumw2();
haccphih->Divide(hrecphih, hgenphih);

//------------------ haprad: --------------------------------

TH1F *hsig = new TH1F("hsig", "hsig", NphihBins, -180, 180);
hsig->Sumw2();
TH1F *hsib = new TH1F("hsib", "hsib", NphihBins, -180, 180);
hsib->Sumw2();
TH1F *hRC = new TH1F("hRC", "hRC", NphihBins, -180, 180);
hRC->Sumw2();

string happath;
if(pipORpim == "pip") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin);
if(pipORpim == "pim") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin);
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
}
}
hapfile.close();

hRC->Divide(hsig, hsib);

//------------------- corr: ---------------------------------

TH1F *hcorr = new TH1F("hcorr", "hcorr", NphihBins, -180, 180);
hcorr->Sumw2();
hcorr->Divide(hdataphihModified, haccphih);

//------------------- corrRC: -------------------------------

if(hsib->Integral() < 0.00000001 || hsig->Integral() < 0.00000001) category = -2; // defining category = -2 here means no haprad results

TH1F *hcorrRC = new TH1F("hcorrRC", "hcorrRC", NphihBins, -180, 180);
hcorrRC->Sumw2();
hcorrRC->Divide(hcorr, hRC);
hcorrRC->GetYaxis()->SetRangeUser(0, 1.1*hcorrRC->GetMaximum());
hcorrRC->GetXaxis()->SetTitle("phi_h (deg.)");
hcorrRC->SetTitle("acc. corr. and rad. corr. data");
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
if(hcorrRC->Integral() > 36) hcorrRC->Fit("ffcorrRC", "q", "", -180, 180);

tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRC->GetParameter(0), ffcorrRC->GetParError(0)));
tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRC->GetParameter(1), ffcorrRC->GetParError(1)));
tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRC->GetParameter(2), ffcorrRC->GetParError(2)));

//tlat->DrawLatex(0.25, 0.14, Form("category %i", category));

int finalNomCategory = category;

//-----------------------------------------------------------
//----------------- do the variations ----------------------- checkpoint
//-----------------------------------------------------------

TCanvas *can = new TCanvas("can", "can", 5, 5, 1600, 1000);
can->Divide(6, 4, 0.00001, 0.00001); // may need manual update

TH1F *hdataphihModifiedS[Nsources][variationsPerSource]; // S for Systematics
TH1F *hgenphihS[Nsources][variationsPerSource], *hrecphihS[Nsources][variationsPerSource], *haccphihS[Nsources][variationsPerSource];
TH1F *hsigS[Nsources][variationsPerSource], *hsibS[Nsources][variationsPerSource], *hRCS[Nsources][variationsPerSource];
TH1F *hcorrS[Nsources][variationsPerSource];
TH1F *hcorrRCS[Nsources][variationsPerSource];
TF1 *ffcorrRCS[Nsources][variationsPerSource];

int count = 0;
for(int s = 0; s < Nsources; s++) {
for(int v = 0; v < variationsPerSource; v++) {
if(!((s == 11 && v == 1) || (s == 12 && v == 1))) // part of the two work-arounds mentioned above
{
count++;
can->cd(count);

//----------- read in the initial bin category --------------

categoryFile.open(Form("binCategories/%sCategory.BiSc5.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
if(categoryFile) categoryFile>>category; // if the file exists, get the category value
if(!categoryFile) category = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();

if(category > 0.5 && s == 10 && v == 0) category = category - 10;
if(category > 0.5 && s == 10 && v == 1) category = category + 10;

//-------------------- data: --------------------------------

hdataphihModifiedS[s][v] = (TH1F*) tfdata[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hdataphihModifiedS[s][v]->SetName(Form("hdataMS_%i_%i", s, v)); // can't let them all have the same name

// apply some modifications and further define the bin category:
NemptyPhihBins = 0;
for(int phih = 0; phih < NphihBins; phih++)
{
if(hdataphihModifiedS[s][v]->GetBinContent(phih+1) < 10) // important condition here! (and more a few lines down)
{
hdataphihModifiedS[s][v]->SetBinContent(phih+1, 0);
hdataphihModifiedS[s][v]->SetBinError(phih+1, 0);
}
if(category > 0.5 && fabs(hdataphihModifiedS[s][v]->GetXaxis()->GetBinCenter(phih+1)) < category)
{
hdataphihModifiedS[s][v]->SetBinContent(phih+1, 0);
hdataphihModifiedS[s][v]->SetBinError(phih+1, 0);
}
if(hdataphihModifiedS[s][v]->GetBinContent(phih+1) < 0.1) NemptyPhihBins++;
}

// update/redefine category here:
if(hdataphihModifiedS[s][v]->Integral() < 180 && NemptyPhihBins >= 26) category = -99; // bad statistics and bad coverage, don't use this bin
else if(category != -1 && hdataphihModifiedS[s][v]->Integral() >= 360 && NemptyPhihBins <= 6) category = 1; // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, Ac, and Acc for this bin
else category = -13; // Measure only M for all other bins
// note: category is further updated at the top of the corrRC section

//--------------------- MC: ---------------------------------

hgenphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hgenphihS[s][v]->SetName(Form("hgenphihS_%i_%i", s, v));
hrecphihS[s][v] = (TH1F*) tfmc[s][v]->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hrecphihS[s][v]->SetName(Form("hrecphihS_%i_%i", s, v));
haccphihS[s][v] = new TH1F(Form("haccphihS_z%iPT2%i_%i_%i", zBin, PT2Bin, s, v), Form("haccphihS_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
haccphihS[s][v]->Sumw2();
haccphihS[s][v]->Divide(hrecphihS[s][v], hgenphihS[s][v]);

//------------------ haprad: --------------------------------

hsigS[s][v] = new TH1F(Form("hsigS_%i_%i", s, v), Form("hsigS_%i_%i", s, v), NphihBins, -180, 180);
hsigS[s][v]->Sumw2();
hsibS[s][v] = new TH1F(Form("hsibS_%i_%i", s, v), Form("hsibS_%i_%i", s, v), NphihBins, -180, 180);
hsibS[s][v]->Sumw2();
hRCS[s][v] = new TH1F(Form("hRCS_%i_%i", s, v), Form("hRCS_%i_%i", s, v), NphihBins, -180, 180);
hRCS[s][v]->Sumw2();

string happathS;
if(s == 12 && v == 0)
{
happathS = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hapDefault/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin); // always pip
}
else
{
if(pipORpim == "pip") happathS = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPipModel/pip_BiSc5_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin);
if(pipORpim == "pim") happathS = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPimModel/pim_BiSc5_x%iQQ%iz%iPT2%i.dat", xBin, QQBin, zBin, PT2Bin);
}
ifstream hapfileS(happathS.c_str());

if(hapfileS)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
hapfileS>>sig>>sib>>tail;
if(pipORpim == "pim")
{
sig = sig - tail;
tail = 0;
}
hsigS[s][v]->SetBinContent(phih+1, sig);
hsigS[s][v]->SetBinError(phih+1, 0);
hsibS[s][v]->SetBinContent(phih+1, sib);
hsibS[s][v]->SetBinError(phih+1, 0);
}
}
hapfileS.close();

hRCS[s][v]->Divide(hsigS[s][v], hsibS[s][v]);

//------------------- corr: ---------------------------------

hcorrS[s][v] = new TH1F(Form("hcorrS_%i_%i", s, v), Form("hcorrS_%i_%i", s, v), NphihBins, -180, 180);
hcorrS[s][v]->Sumw2();
hcorrS[s][v]->Divide(hdataphihModifiedS[s][v], haccphihS[s][v]);
hcorrS[s][v]->Draw();

//------------------- corrRC: -------------------------------

if(hsibS[s][v]->Integral() < 0.00000001 || hsigS[s][v]->Integral() < 0.00000001) category = -2; // defining category = -2 here means no haprad results

hcorrRCS[s][v] = new TH1F(Form("hcorrRCS_%i_%i", s, v), Form("hcorrRCS_%i_%i", s, v), NphihBins, -180, 180);
hcorrRCS[s][v]->Sumw2();
hcorrRCS[s][v]->Divide(hcorrS[s][v], hRCS[s][v]);
hcorrRCS[s][v]->GetYaxis()->SetRangeUser(0, 1.1*hcorrRCS[s][v]->GetMaximum());
hcorrRCS[s][v]->GetXaxis()->SetTitle("phi_h (deg.)");
//hcorrRCS[s][v]->SetTitle("acc. corr. and rad. corr. data");
hcorrRCS[s][v]->SetTitle(sourceName[s].c_str());
hcorrRCS[s][v]->Draw();

ffcorrRCS[s][v] = new TF1(Form("ffcorrRCS_%i_%i", s, v), "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
ffcorrRCS[s][v]->SetLineColor(kBlue);
ffcorrRCS[s][v]->SetParameters(hcorrRCS[s][v]->GetMaximum(), 0.0, 0.0);
if(category == 1)
{
ffcorrRCS[s][v]->SetParLimits(1, -0.99, 0.99);
ffcorrRCS[s][v]->SetParLimits(2, -0.99, 0.99);
}
else
{
ffcorrRCS[s][v]->SetParLimits(1, -0.30, 0.30);
ffcorrRCS[s][v]->SetParLimits(2, -0.30, 0.30);
}
if(hcorrRCS[s][v]->Integral() > 36) hcorrRCS[s][v]->Fit(Form("ffcorrRCS_%i_%i", s, v), "q", "", -180, 180);

tlat->DrawLatex(0.15, 0.32, Form("M = %3.2f #pm %3.2f", ffcorrRCS[s][v]->GetParameter(0), ffcorrRCS[s][v]->GetParError(0)));
tlat->DrawLatex(0.15, 0.26, Form("Ac = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(1), ffcorrRCS[s][v]->GetParError(1)));
tlat->DrawLatex(0.15, 0.20, Form("Acc = %3.4f #pm %3.4f", ffcorrRCS[s][v]->GetParameter(2), ffcorrRCS[s][v]->GetParError(2)));

//tlat->DrawLatex(0.25, 0.14, Form("category %i", category));
} // endif
} // end v loop

if(s <= 10) // part of the work-around mentioned above
{
M_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(0) - ffcorrRC->GetParameter(0), 2.0) + pow(ffcorrRCS[s][1]->GetParameter(0) - ffcorrRC->GetParameter(0), 2.0))/sqrt(2.0);
Ac_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(1) - ffcorrRC->GetParameter(1), 2.0) + pow(ffcorrRCS[s][1]->GetParameter(1) - ffcorrRC->GetParameter(1), 2.0))/sqrt(2.0);
Acc_sysErrorPiece[s] = sqrt(pow(ffcorrRCS[s][0]->GetParameter(2) - ffcorrRC->GetParameter(2), 2.0) + pow(ffcorrRCS[s][1]->GetParameter(2) - ffcorrRC->GetParameter(2), 2.0))/sqrt(2.0);
}
if(s > 10)
{
M_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(0) - ffcorrRC->GetParameter(0)); // one variation, so it simplifies to this
Ac_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(1) - ffcorrRC->GetParameter(1));
Acc_sysErrorPiece[s] = fabs(ffcorrRCS[s][0]->GetParameter(2) - ffcorrRC->GetParameter(2));
}

cout<<s<<" "<<sourceName[s]<<" "<<M_sysErrorPiece[s]<<" "<<Ac_sysErrorPiece[s]<<" "<<Acc_sysErrorPiece[s]<<endl;

hM_sysEcontributions->SetBinContent(s+1, M_sysErrorPiece[s]);
hAc_sysEcontributions->SetBinContent(s+1, Ac_sysErrorPiece[s]);
hAcc_sysEcontributions->SetBinContent(s+1, Acc_sysErrorPiece[s]);

M_sumOfSquaredErrors = M_sumOfSquaredErrors + M_sysErrorPiece[s]*M_sysErrorPiece[s];
Ac_sumOfSquaredErrors = Ac_sumOfSquaredErrors + Ac_sysErrorPiece[s]*Ac_sysErrorPiece[s];
Acc_sumOfSquaredErrors = Acc_sumOfSquaredErrors + Acc_sysErrorPiece[s]*Acc_sysErrorPiece[s];

} // end s loop

M_sysErrorTotal = sqrt(M_sumOfSquaredErrors);
Ac_sysErrorTotal = sqrt(Ac_sumOfSquaredErrors);
Acc_sysErrorTotal = sqrt(Acc_sumOfSquaredErrors);

cout<<endl<<M_sysErrorTotal<<" "<<Ac_sysErrorTotal<<" "<<Acc_sysErrorTotal<<endl;

TCanvas *can2 = new TCanvas("can2", "can2", 15, 15, 1400, 700);
can2->Divide(3, 1, 0.00001, 0.00001);
can2->cd(1);
hM_sysEcontributions->GetXaxis()->SetTitle("source");
hM_sysEcontributions->SetTitle("A_{0} systematic errors");
hM_sysEcontributions->Draw();
can2->cd(2);
hAc_sysEcontributions->GetXaxis()->SetTitle("source");
hAc_sysEcontributions->SetTitle("A^{cos#phi}_{UU} systematic errors");
hAc_sysEcontributions->Draw();
can2->cd(3);
hAcc_sysEcontributions->GetXaxis()->SetTitle("source");
hAcc_sysEcontributions->SetTitle("A^{cos2#phi}_{UU} systematic errors");
hAcc_sysEcontributions->Draw();

//-----------------------------------------------------------
//------------------------- save ----------------------------
//-----------------------------------------------------------

if(doSaveTxt)
{

if(finalNomCategory == 1)
{
ofstream outfile1(Form("%s_M_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
outfile1<<M_sysErrorTotal;
outfile1.close();

ofstream outfile2(Form("%s_AcAcc_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
outfile2<<Ac_sysErrorTotal<<" "<<Acc_sysErrorTotal;
outfile2.close();
}

if(finalNomCategory == -13)
{
ofstream outfile3(Form("%s_M_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
outfile3<<M_sysErrorTotal;
outfile3.close();
}

}

} // end of program
