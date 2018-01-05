/* from ihaprad.f: semi = (16.d0*an*amp*Eb)/(q2**2)*Eh/ys/sx*(xs*ys**2*H1 + rt*H2 + pth/sqrt(q2)*(2.d0-ys)*rtz*cos(phidif)*H3 + pth**2/q2*rtz**2*cos(2.d0*phidif)*H4)
 *                        ^________ "prefactor0" __________^  ^___ "term0" _____^   ^____ "prefactor1" ______^                  ^_"prefactor2"_^
 *
 * 	where ys = y ; xs = x ; sx = QQ/x ; amp = proton mass ; pth = PT ; an, rt, rtz defined below
 *
 * H3 = h3(x,QQ,z)*GTMD*pi_thresh
 * H4 = h4(x,QQ,z)*GTMD*pi_thresh
 *
 * 	GTMD and pi_thresh defined below
 *
 * 	will get h3, h4, and term0 from the fit (h3 will be parameter(3), h4 will be parameter(4), and term0 will be parameter(0)/prefactor0)
 * 	term0 is and always will be (for now) the default value from haprad. in principle I could dig through the code and see how it's calculated, but getting it from the fit is easier for now.
 */

#include <algorithm> // for std::max

void extract_h3h4(int xBin = 0, int QQBin = 0, int zBin = 5, int PT2Bin = 5, string pipORpim = "pip")
{
gStyle->SetOptStat(0);

bool doSaveTxt = 0;

int NphihBins = 36;
int binSchemeOpt = 5;
int accItN = 0;

TFile *tfdata = new TFile(Form("/home/kjgroup/mysidis/histos/data.s1.n12114.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));
TFile *tfmc = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc%i.__0000000000000000__.root", accItN, binSchemeOpt));

// this is BiSc5:
const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
const int NPT2Bins = 20;
float xpts[NxBins] = {0.15, 0.25, 0.35, 0.45, 0.55}; // center values
float QQpts[NxBins][NQQBins] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}}; // approx. center values
float zpts[NzBins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875}; // center values
float PT2pts[NPT2Bins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.925, 0.975}; // center values

float x = xpts[xBin];
float QQ = QQpts[xBin][QQBin];
float z = zpts[zBin];
float PT2 = PT2pts[PT2Bin];

float Eb = 5.498;
float alpha = 0.0072973525698; // ~1/137
float barn = 389379.0; // (GeV^-2 = ~0.3894 mb)
float prot_mass = 0.938272; // GeV
float pip_mass = 0.13957; // GeV
float pi = 3.14159265359;

float y = QQ/(2.0*Eb*prot_mass*x);
float nu = Eb*y;
float Eh = z*nu;
float rt = 1.0 - y - (prot_mass*x*y)/(2.0*Eb);
float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x)/(y*Eb))));
float an = pi*alpha*alpha*y*(QQ/x)*barn*(1.0/(4*sqrt(Eh*Eh - pip_mass*pip_mass - PT2)));
float MX2 = prot_mass*prot_mass + (QQ/x)*(1.0 - z) + pip_mass*pip_mass - QQ + 2.0*(sqrt(nu*nu + QQ)*sqrt(Eh*Eh - pip_mass*pip_mass - PT2) - nu*Eh); // missing mass squared

// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
float ac = 1.2025E-10;
float bc = -5.2703E-02;
float cc = 3.7467E-01;
float dc = 6.5397E-02;
float ec = -2.2136E-01;
float fc = -1.0621E-01;
float sgmpt = ac + bc*x + cc*z + dc*x*x + ec*z*z + fc*x*z;
if(sgmpt < 0.02) sgmpt = 0.02;
if(sgmpt > 0.15) sgmpt = 0.15;
float GTMD = exp((-(PT2 + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
float pi_thresh = sqrt(1.0 - ((prot_mass + pip_mass)*(prot_mass + pip_mass))/MX2);

float prefactor0 = ((16.0*an*prot_mass*Eb)/(QQ*QQ))*(Eh/(y*(QQ/x)));
float prefactor1 = (sqrt(PT2)/sqrt(QQ))*(2.0 - y)*rtz;
float prefactor2 = (PT2/QQ)*rtz*rtz;

///////////////////////////////////////
// define the fit function to extract h3 and h4

TF1 *ff_h3h4 = new TF1("ff_h3h4", "[0] + [1]*[3]*cos((3.14159265359/180.0)*x) + [2]*[4]*cos(2.0*(3.14159265359/180.0)*x)", -180, 180);
// parameter [0] is prefactor0*(xs*ys**2*H1 + rt*H2), but I don't care about these details here
ff_h3h4->FixParameter(1, prefactor0*prefactor1*GTMD*pi_thresh);
ff_h3h4->FixParameter(2, prefactor0*prefactor2*GTMD*pi_thresh);
ff_h3h4->SetParName(3, "h3");
ff_h3h4->SetParName(4, "h4");

///////////////////////////////////////
// read in the default cross section from haprad, use this to set the normalization

TH1F *hsib1000 = new TH1F("hsib1000", "hsib1000", NphihBins, -180, 180); // sib*1000 ("semi" calculated in semi_inclusive_model = 1000*sib)
ifstream hapfile(Form("hapradResults/hap0/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin)); // default haprad values are same for pip and pim, so always using pip here
if(!hapfile) cout<<"PROBLEM PROBLEM PROBLEM!"<<endl;
else
{
for(int phih = 0; phih < NphihBins; phih++)
{
float crap, sib;
hapfile>>crap>>sib>>crap;
hsib1000->SetBinContent(phih+1, sib*1000.0); // factor of 1000 for unit conversion ("semi" calculated in semi_inclusive_model = 1000*sib)
}
}
hapfile.close();

///////////////////////////////////////
// get and plot experimental results (with phih cuts)

TH1F *hdataphih = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *hgenphih = (TH1F*) tfmc->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *hrecphih = (TH1F*) tfmc->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

int category;
ifstream categoryFile;
categoryFile.open(Form("binCategories/%sCategory.BiSc%i.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(categoryFile) categoryFile>>category; // if the file exists, get the category value
if(!categoryFile) category = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();

int NemptyPhihBins = 0; // for data
for(int phih = 0; phih < NphihBins; phih++)
{
if(hdataphih->GetBinContent(phih+1) < 10) // important condition here! (and more a few lines down)
{
hdataphih->SetBinContent(phih+1, 0);
hdataphih->SetBinError(phih+1, 0);
}
if(category > 0.5 && fabs(hdataphih->GetXaxis()->GetBinCenter(phih+1)) < category)
{
hdataphih->SetBinContent(phih+1, 0);
hdataphih->SetBinError(phih+1, 0);
}
if(hdataphih->GetBinContent(phih+1) < 0.1) NemptyPhihBins++;
}
// update/redefine category here:
if(hdataphih->Integral() < 180 && NemptyPhihBins >= 26) category = -99; // bad statistics and bad coverage, don't use this bin
else if(category != -1 && hdataphih->Integral() >= 360 && NemptyPhihBins <= 6) category = 1; // not "suspicious (-1)" bin, good statistics, and good coverage. Measure M, A, and A2 for this bin
else category = -13; // Measure only M for all other bins

TH1F *haccphih = new TH1F("haccphih", "haccphih", NphihBins, -180, 180);
haccphih->Sumw2();
haccphih->Divide(hrecphih, hgenphih);
TH1F *hcorrphih = new TH1F("hcorrphih", "hcorrphih", NphihBins, -180, 180);
hcorrphih->Sumw2();
hcorrphih->Divide(hdataphih, haccphih);

///////////////////////////////////////
// normalize the experimental results to default haprad normalization:

TCanvas *can = new TCanvas();
can->Divide(2,1);
can->cd(1);

if(hsib1000->Integral() > 0.0000000001 && NemptyPhihBins < 20) // 20 is kind of random here (but it should be generous), I'm more precise below when saving (or not saving) the fit results
{
float emptyBinsCorrectionFactor  = ((float)NphihBins)/(((float)NphihBins)-((float)NemptyPhihBins)); // don't want cut/empty bins to count as 0 in the integral (sum)
hcorrphih->Scale(hsib1000->Integral()/(hcorrphih->Integral()*emptyBinsCorrectionFactor));

ff_h3h4->SetParameter(0, hcorrphih->GetMaximum());
ff_h3h4->SetParameter(3, 0);
ff_h3h4->SetParameter(4, 0);
hcorrphih->Fit("ff_h3h4", "", "", -180, 180);
}

can->cd(1);
hsib1000->GetYaxis()->SetRangeUser(0, max(0.000000001, 1.1*hsib1000->GetMaximum()));
hsib1000->Draw();
can->cd(2);
hcorrphih->GetYaxis()->SetRangeUser(0, max(0.000000001, 1.1*hcorrphih->GetMaximum()));
hcorrphih->Draw();

///////////////////////////////////////
// save:

if(category == 1 && doSaveTxt && hsib1000->Integral() > 0.0000000001)
{
ofstream outfile(Form("term0h3h4_%s_it%i_BiSc%i_x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
outfile<<ff_h3h4->GetParameter(0)/prefactor0<<" "<<ff_h3h4->GetParameter(3)<<" "<<ff_h3h4->GetParameter(4);
outfile.close();
}

///////////////////////////////////////
// check:

//float calcAc = (prefactor0*prefactor1*GTMD*pi_thresh*ff_h3h4->GetParameter(3))/(hsib1000->Integral()/NphihBins);
//float calcAcc = (prefactor0*prefactor2*GTMD*pi_thresh*ff_h3h4->GetParameter(4))/(hsib1000->Integral()/NphihBins);
//cout<<"HEY "<<calcAcc<<endl;
//
//TF1 *ff_test = new TF1("ff_test", "[0]*(1.0 + [1]*cos((3.14159265359/180.0)*x) + [2]*cos(2.0*(3.14159265359/180.0)*x))", -180, 180);
//ff_test->SetParameters(hcorrphih->GetMaximum(), 0, 0);
//
//hcorrphih->Fit("ff_test", "", "", -180, 180);
}
