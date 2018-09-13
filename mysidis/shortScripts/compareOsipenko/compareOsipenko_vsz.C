// sloppy (but fairly simple) script... a number of things are hard coded... be careful when changing things
{
float prot_mass = 0.938272; // GeV

int Nlines = 80; // number of lines in H3 (H4) table

float systematicsYvalue = -0.19;

string H3orH4 = "H4";
int myPT2Bin = 6; // the PT2 bin that I want to look at (using my convention, PT2 = 0, ..., 19) ... Osipenko has coverage from 0 < PT2 < 0.8
float PT2Min, PT2Max;
if(myPT2Bin == 0) // few points
{
PT2Min = 0.00;
PT2Max = 0.05;
}
else if(myPT2Bin == 1) // few points
{
PT2Min = 0.05;
PT2Max = 0.10;
}
else if(myPT2Bin == 2) // very nice agreement (H4)
{
PT2Min = 0.10;
PT2Max = 0.15;
}
else if(myPT2Bin == 3) // nothing to compare
{
PT2Min = 0.15;
PT2Max = 0.20;
}
else if(myPT2Bin == 4) // okay
{
PT2Min = 0.20;
PT2Max = 0.25;
}
else if(myPT2Bin == 5) // nothing to compare
{
PT2Min = 0.25;
PT2Max = 0.30;
}
else if(myPT2Bin == 6) // okay
{
PT2Min = 0.30;
PT2Max = 0.35;
}
else if(myPT2Bin == 7) // nothing to compare
{
PT2Min = 0.35;
PT2Max = 0.40;
}
else if(myPT2Bin == 8) // nothing to compare
{
PT2Min = 0.40;
PT2Max = 0.45;
}
else if(myPT2Bin == 9) // nothing to compare
{
PT2Min = 0.45;
PT2Max = 0.50;
}
else cout<<"UNDEFINED VALUES"<<endl;

TGraphErrors *grmomentVz = new TGraphErrors();
TGraphErrors *grsys = new TGraphErrors();

ifstream tablefile;
if(H3orH4 == "H3") tablefile.open("H3table.txt");
if(H3orH4 == "H4") tablefile.open("H4table.txt");
for(int k = 0; k < Nlines; k++)
{
float x, QQ, z, PT2, y, SFrat, SFrate, sys; // SFrat = structure function ratio (depending on what table you use), sys = systematic error
string crap;

tablefile>>z>>PT2>>y>>crap>>QQ>>crap>>x>>crap>>SFrat>>crap>>SFrate>>crap>>sys;

float gamma = (2.0*prot_mass*x)/sqrt(QQ);
float xi = 1.0 - y - 0.25*gamma*gamma*y*y;
float kappa = 1.0/(1.0 + gamma*gamma);
float moment;
if(H3orH4 == "H3") moment = SFrat*2.0*(2.0 - y)*sqrt(kappa/xi);
if(H3orH4 == "H4") moment = SFrat*2.0*kappa;
float scaled_e;
if(H3orH4 == "H3") scaled_e = SFrate*2.0*(2.0 - y)*sqrt(kappa/xi);
if(H3orH4 == "H4") scaled_e = SFrate*2.0*kappa;
float scaled_sys;
if(H3orH4 == "H3") scaled_sys = sys*2.0*(2.0 - y)*sqrt(kappa/xi);
if(H3orH4 == "H4") scaled_sys = sys*2.0*kappa;

if(x >= 0.2 && x <= 0.3 && QQ >= 1.7 && QQ <= 2.5) // I want his results comparable to my x1QQ1 bin (this is where most of his results are anyway, so not much flexibility to change this)
{
if(PT2 >= PT2Min && PT2 <= PT2Max)
{
grmomentVz->SetPoint(grmomentVz->GetN(), z, moment);
grmomentVz->SetPointError(grmomentVz->GetN()-1, 0, scaled_e);
grsys->SetPoint(grsys->GetN(), z, systematicsYvalue);
grsys->SetPointError(grsys->GetN()-1, 0, 0.5*scaled_sys); // error bar will go from -0.5sys to 0.5sys for a total size of sys
}}

}
tablefile.close();

///////////////////////////////////////////////////////////////////////
// now compare to my results: /////////////////////////////////////////

int NzBins = 20;
float zMin = 0.0;
float zMax = 1.0;

TH1F *hmomentVz = new TH1F("hmomentVz", "hmomentVz", NzBins, zMin, zMax);
TGraphErrors *grmysys = new TGraphErrors();

for(int k = 0; k < NzBins; k++)
{
ifstream file1(Form("/scratch/MAcAccfiles/pip_AcAcc_it2_hap2_BiSc5_x1QQ1z%iPT2%i.txt", k, myPT2Bin));
float tempAc, tempAce, tempAcc, tempAcce;
if(file1)
{
file1>>tempAc>>tempAce>>tempAcc>>tempAcce;
if(!(tempAc < -0.17)) // to not plot the few points that look very suspicious (per Harut's suggestion) ... also, see note below
{
if(H3orH4 == "H3")
{
hmomentVz->SetBinContent(k+1, tempAc);
hmomentVz->SetBinError(k+1, tempAce);
}
if(H3orH4 == "H4")
{
hmomentVz->SetBinContent(k+1, tempAcc);
hmomentVz->SetBinError(k+1, tempAcce);
}
}
}
file1.close();

// my systematic errors:
float tempAcsys, tempAccsys;
ifstream file2(Form("/home/kjgroup/mysidis/shortScripts/phi_h/systematicErrors/pip_AcAcc_sys_BiSc5_x1QQ1z%iPT2%i.txt", k, myPT2Bin));
if(file2)
{
if(hmomentVz->GetBinError(k+1) > 0.00000001) // don't want to plot error for points I'm not plotting (see note above about Harut's suggestion)
{
file2>>tempAcsys>>tempAccsys;
grmysys->SetPoint(grmysys->GetN(), hmomentVz->GetBinCenter(k+1), systematicsYvalue);
if(H3orH4 == "H3") grmysys->SetPointError(grmysys->GetN()-1, 0, 0.5*tempAcsys); // error bar will go from -0.5sys to 0.5sys for a total size of sys
if(H3orH4 == "H4") grmysys->SetPointError(grmysys->GetN()-1, 0, 0.5*tempAccsys); // error bar will go from -0.5sys to 0.5sys for a total size of sys
}
}
}

//////// Draw: ///////

gStyle->SetErrorX(0.0);
gStyle->SetOptStat(0);

hmomentVz->SetMarkerStyle(20);
hmomentVz->SetMarkerColor(kRed);
hmomentVz->SetLineColor(kRed);
hmomentVz->GetYaxis()->SetRangeUser(-0.5,0.5);
if(H3orH4 == "H3") hmomentVz->SetTitle("A_{UU}^{cos#phi_{h}}");
if(H3orH4 == "H4") hmomentVz->SetTitle("A_{UU}^{cos2#phi_{h}}");
hmomentVz->GetXaxis()->SetTitle("z");
hmomentVz->Draw("E1");

grmysys->SetFillColor(kRed);
grmysys->SetFillStyle(3002);
grmysys->Draw("E3 same");

grmomentVz->SetMarkerStyle(22);
grmomentVz->Draw("p same");

grsys->SetFillStyle(3005);
grsys->Draw("E3 same");

TLegend *leg = new TLegend(0.7, 0.75, 0.9, 0.9);
leg->AddEntry(grmomentVz, "Osipenko", "lep");
leg->AddEntry(hmomentVz, "Harrison", "lep");
leg->Draw();

}
