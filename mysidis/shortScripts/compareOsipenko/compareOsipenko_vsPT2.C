// sloppy (but fairly simple) script... a number of things are hard coded... be careful when changing things
{
float prot_mass = 0.938272; // GeV

int Nlines = 80; // number of lines in H3 (H4) table

string H3orH4 = "H4";
int myzBin = 7; // the z bin that I want to look at (using my convention, z = 0, ..., 17) ... Osipenko has coverage from 0 < z < 0.6
float zMin, zMax;
if(myzBin == 7)
{
zMin = 0.35;
zMax = 0.4;
}
else cout<<"UNDEFINED VALUES"<<endl;

TGraphErrors *grmomentVPT2 = new TGraphErrors();

ifstream tablefile;
if(H3orH4 == "H3") tablefile.open("H3table.txt");
if(H3orH4 == "H4") tablefile.open("H4table.txt");
for(int k = 0; k < Nlines; k++)
{
float x, QQ, z, PT2, y, SFrat, SFrate; // SFrat = structure function ratio (depending on what table you use)... only going to plot stat. error for now
string crap;

tablefile>>z>>PT2>>y>>crap>>QQ>>crap>>x>>crap>>SFrat>>crap>>SFrate>>crap>>crap;

float gamma = (2.0*prot_mass*x)/sqrt(QQ);
float xi = 1.0 - y - 0.25*gamma*gamma*y*y;
float kappa = 1.0/(1.0 + gamma*gamma);
float moment;
if(H3orH4 == "H3") moment = SFrat*2.0*(2.0 - y)*sqrt(kappa/xi);
if(H3orH4 == "H4") moment = SFrat*2.0*kappa;
float scaled_e;
if(H3orH4 == "H3") scaled_e = SFrate*2.0*(2.0 - y)*sqrt(kappa/xi);
if(H3orH4 == "H4") scaled_e = SFrate*2.0*kappa;

if(x >= 0.2 && x <= 0.3 && QQ >= 1.7 && QQ <= 2.5) // I want his results comparable to my x1QQ1 bin (this is where most of his results are anyway, so not much flexibility to change this)
{
if(z >= zMin && z <= zMax)
{
grmomentVPT2->SetPoint(grmomentVPT2->GetN(), PT2, moment);
grmomentVPT2->SetPointError(grmomentVPT2->GetN()-1, 0, scaled_e);
}}

}
tablefile.close();

///////////////////////////////////////////////////////////////////////
// now compare to my results: /////////////////////////////////////////

int NPT2Bins = 20;
float PT2Min = 0.0;
float PT2Max = 1.0;

TH1F *hmomentVPT2 = new TH1F("hmomentVPT2", "hmomentVPT2", NPT2Bins, PT2Min, PT2Max);

for(int k = 0; k < NPT2Bins; k++)
{
ifstream file1(Form("/scratch/MAcAccfiles/pip_AcAcc_it2_hap2_BiSc5_x1QQ1z%iPT2%i.txt", myzBin, k));
float tempAc, tempAce, tempAcc, tempAcce;
if(file1)
{
file1>>tempAc>>tempAce>>tempAcc>>tempAcce;
if(H3orH4 == "H3")
{
hmomentVPT2->SetBinContent(k+1, tempAc);
hmomentVPT2->SetBinError(k+1, tempAce);
}
if(H3orH4 == "H4")
{
hmomentVPT2->SetBinContent(k+1, tempAcc);
hmomentVPT2->SetBinError(k+1, tempAcce);
}
}
file1.close();
}

//////// Draw: ///////

gStyle->SetErrorX(0.0);
gStyle->SetOptStat(0);

hmomentVPT2->SetMarkerStyle(20);
hmomentVPT2->SetMarkerColor(kRed);
hmomentVPT2->SetLineColor(kRed);
hmomentVPT2->Draw("E1");

grmomentVPT2->SetMarkerStyle(22);
grmomentVPT2->Draw("p same");

}
