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

void get_term0_fromHAPRAD()
{
gStyle->SetOptStat(0);

bool doSave = 1;

int binSchemeOpt = 5;

// this is BiSc5:
int NphihBins = 36;
const int NxBins = 5;
const int NQQBins = 2;
const int NzBins = 18;
const int NPT2Bins = 20;
float xpts[NxBins] = {0.15, 0.25, 0.35, 0.45, 0.55}; // center values
float QQpts[NxBins][NQQBins] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}}; // approx. center values
float zpts[NzBins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875}; // center values
float zMin = 0.0;
float zMax = 1.0;
float PT2pts[NPT2Bins] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.925, 0.975}; // center values
float PT2Min = 0.0;
float PT2Max = 1.0;

float Eb = 5.498;
float alpha = 0.0072973525698; // ~1/137
float barn = 389379.0; // (GeV^-2 = ~0.3894 mb)
float prot_mass = 0.938272; // GeV
float pip_mass = 0.13957; // GeV
float pi = 3.14159265359;

// from semi_inclusive_model.f: "Parton transverse momentum distribution (Gaussian fit)"
float ac = 1.2025E-10;
float bc = -5.2703E-02;
float cc = 3.7467E-01;
float dc = 6.5397E-02;
float ec = -2.2136E-01;
float fc = -1.0621E-01;

///////////////////////////////////////

TH2F *hterm0[NxBins][NQQBins];

TCanvas *can = new TCanvas("can", "can", 10, 10, 1500, 1000);
can->Divide(NxBins, NQQBins, 0.00001, 0.00001);

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {

can->cd(ix + 1 + (NQQBins-1)*NxBins - NxBins*iQQ)->SetLogz();
//can->cd(ix + 1 + (NQQBins-1)*NxBins - NxBins*iQQ);
hterm0[ix][iQQ] = new TH2F(Form("hterm0_x%iQQ%i", ix, iQQ), Form("hterm0_x%iQQ%i", ix, iQQ), NzBins, zMin, zMax, NPT2Bins, PT2Min, PT2Max);

for(int iz = 0; iz < NzBins; iz++) {
for(int iPT2 = 0; iPT2 < NPT2Bins; iPT2++) {

float x = xpts[ix];
float QQ = QQpts[ix][iQQ];
float z = zpts[iz];
float PT2 = PT2pts[iPT2];

float y = QQ/(2.0*Eb*prot_mass*x);
float nu = Eb*y;
float Eh = z*nu;
float rt = 1.0 - y - (prot_mass*x*y)/(2.0*Eb);
float rtz = sqrt(rt/(1.0 + ((2.0*prot_mass*x)/(y*Eb))));
float an = pi*alpha*alpha*y*(QQ/x)*barn*(1.0/(4*sqrt(Eh*Eh - pip_mass*pip_mass - PT2)));
float MX2 = prot_mass*prot_mass + (QQ/x)*(1.0 - z) + pip_mass*pip_mass - QQ + 2.0*(sqrt(nu*nu + QQ)*sqrt(Eh*Eh - pip_mass*pip_mass - PT2) - nu*Eh); // missing mass squared

float sgmpt = ac + bc*x + cc*z + dc*x*x + ec*z*z + fc*x*z;
if(sgmpt < 0.02) sgmpt = 0.02;
if(sgmpt > 0.15) sgmpt = 0.15;
float GTMD = exp((-(PT2 + 0.0))/(2.0*sgmpt))*(1.0/(2.0*pi*sgmpt)); // the 0.0 part comes from "2*pl^2" in haprad, but "pl" seems to always be zero
float pi_thresh = sqrt(1.0 - ((prot_mass + pip_mass)*(prot_mass + pip_mass))/MX2);

float prefactor0 = ((16.0*an*prot_mass*Eb)/(QQ*QQ))*(Eh/(y*(QQ/x)));
float prefactor1 = (sqrt(PT2)/sqrt(QQ))*(2.0 - y)*rtz;
float prefactor2 = (PT2/QQ)*rtz*rtz;

// define the fit function to extract h3 and h4:
TF1 *ff_h3h4 = new TF1("ff_h3h4", "[0] + [1]*[3]*cos((3.14159265359/180.0)*x) + [2]*[4]*cos(2.0*(3.14159265359/180.0)*x)", -180, 180);
ff_h3h4->SetParName(0, "prefactor0xterm0"); // prefactor0*term0
ff_h3h4->FixParameter(1, prefactor0*prefactor1*GTMD*pi_thresh);
ff_h3h4->FixParameter(2, prefactor0*prefactor2*GTMD*pi_thresh);
ff_h3h4->SetParName(3, "h3");
ff_h3h4->SetParName(4, "h4");

// read in histogram from haprad results:
TH1F *hsib1000 = new TH1F("hsib1000", "hsib1000", NphihBins, -180, 180); // sib*1000 ("semi" calculated in semi_inclusive_model = 1000*sib)
ifstream hapfile(Form("hapradResults/hapDefault/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, ix, iQQ, iz, iPT2));
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

// fit the histogram:
ff_h3h4->SetParameter(0, hsib1000->GetMaximum());
ff_h3h4->SetParameter(3, 0);
ff_h3h4->SetParameter(4, 0);
if(hsib1000->Integral() > 0.00000001)
{
hsib1000->Fit("ff_h3h4", "q", "", -180, 180);
hterm0[ix][iQQ]->SetBinContent(iz+1, iPT2+1, ff_h3h4->GetParameter(0)/prefactor0);
ofstream outfile(Form("dflt_term0_fromHAPRAD_BiSc%i_x%iQQ%iz%iPT2%i.txt", binSchemeOpt, ix, iQQ, iz, iPT2));
if(doSave) outfile<<ff_h3h4->GetParameter(0)/prefactor0;
outfile.close();
}

delete ff_h3h4;
delete hsib1000;
}}

hterm0[ix][iQQ]->GetZaxis()->SetRangeUser(1.0E-5, 10);
hterm0[ix][iQQ]->Draw("colz");

}}


}
