{
gStyle->SetOptStat(0);

int hapORdata = 1; // data points from haprad (0) or from data (1)
int regORrobust = 0; // 0 for Eval, 1 for EvalRobust
string pipORpim = "pip";

int accItN = 0;
int binSchemeOpt = 5;

TLinearFitter *LFc = new TLinearFitter(4); // function of 4 variables (x[0]=x, x[1]=QQ, x[2]=z, x[3]=PT2)
LFc->StoreData(1);
LFc->SetFormula("1.0 ++ x[0] ++ x[1] ++ sqrt(x[3]) ++ x[0]*x[1] ++ x[0]*sqrt(x[3]) ++ x[1]*sqrt(x[3]) ++ x[0]*x[1]*sqrt(x[3]) ++ x[2] ++ x[0]*x[2] ++ x[1]*x[2] ++ sqrt(x[3])*x[2] ++ x[0]*x[1]*x[2] ++ x[0]*sqrt(x[3])*x[2] ++ x[1]*sqrt(x[3])*x[2] ++ x[0]*x[1]*sqrt(x[3])*x[2] ++ x[2]*x[2] ++ x[0]*x[2]*x[2] ++ x[1]*x[2]*x[2] ++ sqrt(x[3])*x[2]*x[2] ++ x[0]*x[1]*x[2]*x[2] ++ x[0]*sqrt(x[3])*x[2]*x[2] ++ x[1]*sqrt(x[3])*x[2]*x[2] ++ x[0]*x[1]*sqrt(x[3])*x[2]*x[2]"); // 1st order poly in x, 1st order poly in QQ, 2nd order poly in z, 1st order poly in sqrt(PT2) = 24 terms

TLinearFitter *LFcc = new TLinearFitter(4); // function of 4 variables (x, QQ, z, PT2)
LFcc->StoreData(1);
LFcc->SetFormula("1.0 ++ x[0] ++ x[1] ++ x[2] ++ x[3] ++ x[0]*x[1] ++ x[0]*x[2] ++ x[0]*x[3] ++ x[1]*x[2] ++ x[1]*x[3] ++ x[2]*x[3] ++ x[0]*x[1]*x[2] ++ x[0]*x[1]*x[3] ++ x[0]*x[2]*x[3] ++ x[1]*x[2]*x[3] ++ x[0]*x[1]*x[2]*x[3]"); // 1st order poly in x, 1st order poly in QQ, 1st order poly in z, 1st order poly in PT2 = 16 terms

// ----- make 3D array of 1D graphs to compare the fit results to -----
if(hapORdata == 0)
{
int Nx = 5;
int NQQ = 2;
int Nz = 8;
int NPT2 = 5;
float xpts[Nx] = {0.15, 0.25, 0.35, 0.45, 0.55};
float QQpts[Nx][NQQ] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}};
float zpts[Nz] = {0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85};
float PT2pts[NPT2] = {0.1, 0.3, 0.5, 0.7, 0.9};

int markstyle[NPT2] = {24, 20, 5, 22, 21};
int lcolor[NPT2] = {kBlack, kBlue, kTeal, kYellow, kRed};
}

if(hapORdata == 1 && binSchemeOpt == 5)
{
int Nx = 5;
float xpts[Nx] = {0.15, 0.25, 0.35, 0.45, 0.55};

int NQQ = 2;
float QQpts[Nx][NQQ] = {{1.1, 1.4}, {1.5, 1.8}, {1.9, 2.4}, {2.5, 3.1}, {4.0, 4.0}};

int Nz = 18;
float zpts[Nz] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875};

int NPT2 = 5; // originally 20, but only using a subset
//int whichPT2bins[NPT2] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
//float PT2pts[NPT2] = {0.025, 0.075, 0.125, 0.175, 0.225, 0.275, 0.325, 0.375, 0.425, 0.475, 0.525, 0.575, 0.625, 0.675, 0.725, 0.775, 0.825, 0.875, 0.925, 0.975};
//int markstyle[NPT2] = {2, 3, 4, 5, 7, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
//int lcolor[NPT2] = {60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98};
int whichPT2bins[NPT2] = {3,5,7,9,11};
float PT2pts[NPT2] = {0.025, 0.275, 0.525, 0.775, 0.975};
int markstyle[NPT2] = {24, 20, 5, 22, 21};
int lcolor[NPT2] = {kBlack, kBlue, kTeal, kYellow, kRed};
//int whichPT2bins[NPT2] = {0, 9, 14};
//float PT2pts[NPT2] = {0.025, 0.475, 0.725};
//int markstyle[NPT2] = {24, 20, 5};
//int lcolor[NPT2] = {kBlack, kTeal, kRed};
}

TGraphErrors *gAcVz[Nx][NQQ][NPT2];
TGraphErrors *gAccVz[Nx][NQQ][NPT2];

for(int x = 0; x < Nx; x++) {
for(int QQ = 0; QQ < NQQ; QQ++) {
for(int PT2 = 0; PT2 < NPT2; PT2++) {
gAcVz[x][QQ][PT2] = new TGraphErrors(Nz);
gAcVz[x][QQ][PT2]->SetMarkerStyle(markstyle[PT2]);
gAcVz[x][QQ][PT2]->SetMarkerColor(lcolor[PT2]);
gAccVz[x][QQ][PT2] = new TGraphErrors(Nz);
gAccVz[x][QQ][PT2]->SetMarkerStyle(markstyle[PT2]);
gAccVz[x][QQ][PT2]->SetMarkerColor(lcolor[PT2]);
}}}
// ----- end make 3D array of 1D histograms to compare the fit results to -----

// ---------- read in values ----------
if(hapORdata == 0)
{
ifstream datafile("someDefaultHRresults.txt");
string dummy;
datafile>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy;
for(int k = 0; k < 218; k++) // 218 rows in the table
{
int ix, iQQ, iz, iPT2;
float xval, QQval, zval, PT2val, Ac, Acc;
datafile>>dummy>>dummy>>dummy>>ix>>dummy>>xval>>dummy>>iQQ>>dummy>>QQval>>dummy>>iz>>dummy>>zval>>dummy>>iPT2>>dummy>>PT2val>>dummy>>Ac>>dummy>>Acc>>dummy;

gAcVz[ix][iQQ][iPT2]->SetPoint(iz+1, zval, Ac);
gAccVz[ix][iQQ][iPT2]->SetPoint(iz+1, zval, Acc);

Double_t *p = new Double_t[4];
p[0] = xval;
p[1] = QQval;
p[2] = zval;
p[3] = PT2val;

LFc->AddPoint(p, Ac, 1); // last arg = "weight(measurement error) of this point (=1 by default)"
LFcc->AddPoint(p, Acc, 1); // last arg = "weight(measurement error) of this point (=1 by default)"
}
datafile.close();
}

if(hapORdata == 1)
{
for(int x = 0; x < Nx; x++) {
for(int QQ = 0; QQ < NQQ; QQ++) {
for(int z = 0; z < Nz; z++) {
for(int PT2 = 0; PT2 < NPT2; PT2++) {
ifstream datafile(Form("/scratch/AcAccfiles/AcAcc_%s_it%i_BiSc%i_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), accItN, binSchemeOpt, x, QQ, z, whichPT2bins[PT2]));
float Ac, Ace, Acc, Acce;
if(datafile)
{
datafile>>Ac>>Ace>>Acc>>Acce;
gAcVz[x][QQ][PT2]->SetPoint(z+1, zpts[z], Ac);
gAcVz[x][QQ][PT2]->SetPointError(z+1, 0.05, Ace);
gAccVz[x][QQ][PT2]->SetPoint(z+1, zpts[z], Acc);
gAccVz[x][QQ][PT2]->SetPointError(z+1, 0.05, Acce);

Double_t *p = new Double_t[4];
p[0] = xpts[x];
p[1] = QQpts[QQ];
p[2] = zpts[z];
p[3] = PT2pts[PT2];

LFc->AddPoint(p, Ac, Ace); // last arg = "weight(measurement error) of this point (=1 by default)"
LFcc->AddPoint(p, Acc, Acce); // last arg = "weight(measurement error) of this point (=1 by default)"
}
datafile.close();
}}}}
}
// ---------- end read in values ----------

if(regORrobust == 0)
{
LFc->Eval();
LFcc->Eval();
}
if(regORrobust == 1)
{
LFc->EvalRobust();
LFcc->EvalRobust();
}

LFc->PrintResults(3, 1);
LFcc->PrintResults(3, 1);

// ----------  draw results ----------
TCanvas *CANc = new TCanvas("CANc", "CANc", 10, 10, 1600, 1000);
CANc->Divide(Nx, NQQ, 0, 0);
TCanvas *CANcc = new TCanvas("CANcc", "CANcc", 10, 10, 1600, 1000);
CANcc->Divide(Nx, NQQ, 0, 0);

TF1 *FFc[Nx][NQQ][NPT2];
TF1 *FFcc[Nx][NQQ][NPT2];

for(int x = 0; x < Nx; x++) {
for(int QQ = 0; QQ < NQQ; QQ++) {
for(int PT2 = 0; PT2 < NPT2; PT2++) {

FFc[x][QQ][PT2] = new TF1(Form("FFc_x%iQQ%iPT2%i", x, QQ, PT2), "[3] + [4]*[0] + [5]*[1] + [6]*sqrt([2]) + [7]*[0]*[1] + [8]*[0]*sqrt([2]) + [9]*[1]*sqrt([2]) + [10]*[0]*[1]*sqrt([2]) + [11]*x + [12]*[0]*x + [13]*[1]*x + [14]*sqrt([2])*x + [15]*[0]*[1]*x + [16]*[0]*sqrt([2])*x + [17]*[1]*sqrt([2])*x + [18]*[0]*[1]*sqrt([2])*x + [19]*x*x + [20]*[0]*x*x + [21]*[1]*x*x + [22]*sqrt([2])*x*x + [23]*[0]*[1]*x*x + [24]*[0]*sqrt([2])*x*x + [25]*[1]*sqrt([2])*x*x + [26]*[0]*[1]*sqrt([2])*x*x", 0, 1); // [0] = x, [1] = QQ, sqrt([2]) = sqrt(PT2)
FFc[x][QQ][PT2]->SetLineColor(lcolor[PT2]);
FFc[x][QQ][PT2]->SetParameter(0, xpts[x]);
FFc[x][QQ][PT2]->SetParameter(1, QQpts[x][QQ]);
FFc[x][QQ][PT2]->SetParameter(2, PT2pts[PT2]);
FFc[x][QQ][PT2]->SetParameter(3, LFc->GetParameter(0));
FFc[x][QQ][PT2]->SetParameter(4, LFc->GetParameter(1));
FFc[x][QQ][PT2]->SetParameter(5, LFc->GetParameter(2));
FFc[x][QQ][PT2]->SetParameter(6, LFc->GetParameter(3));
FFc[x][QQ][PT2]->SetParameter(7, LFc->GetParameter(4));
FFc[x][QQ][PT2]->SetParameter(8, LFc->GetParameter(5));
FFc[x][QQ][PT2]->SetParameter(9, LFc->GetParameter(6));
FFc[x][QQ][PT2]->SetParameter(10, LFc->GetParameter(7));
FFc[x][QQ][PT2]->SetParameter(11, LFc->GetParameter(8));
FFc[x][QQ][PT2]->SetParameter(12, LFc->GetParameter(9));
FFc[x][QQ][PT2]->SetParameter(13, LFc->GetParameter(10));
FFc[x][QQ][PT2]->SetParameter(14, LFc->GetParameter(11));
FFc[x][QQ][PT2]->SetParameter(15, LFc->GetParameter(12));
FFc[x][QQ][PT2]->SetParameter(16, LFc->GetParameter(13));
FFc[x][QQ][PT2]->SetParameter(17, LFc->GetParameter(14));
FFc[x][QQ][PT2]->SetParameter(18, LFc->GetParameter(15));
FFc[x][QQ][PT2]->SetParameter(19, LFc->GetParameter(16));
FFc[x][QQ][PT2]->SetParameter(20, LFc->GetParameter(17));
FFc[x][QQ][PT2]->SetParameter(21, LFc->GetParameter(18));
FFc[x][QQ][PT2]->SetParameter(22, LFc->GetParameter(19));
FFc[x][QQ][PT2]->SetParameter(23, LFc->GetParameter(20));
FFc[x][QQ][PT2]->SetParameter(24, LFc->GetParameter(21));
FFc[x][QQ][PT2]->SetParameter(25, LFc->GetParameter(22));
FFc[x][QQ][PT2]->SetParameter(26, LFc->GetParameter(23));

FFcc[x][QQ][PT2] = new TF1(Form("FFcc_x%iQQ%iPT2%i", x, QQ, PT2), "[3] + [4]*[0] + [5]*[1] + [6]*x + [7]*[2] + [8]*[0]*[1] + [9]*[0]*x + [10]*[0]*[2] + [11]*[1]*x + [12]*[1]*[2] + [13]*x*[2] + [14]*[0]*[1]*x + [15]*[0]*[1]*[2] + [16]*[0]*x*[2] + [17]*[1]*x*[2] + [18]*[0]*[1]*x*[2]", 0, 1); // [0] = x, [1] = QQ, [2] = PT2
FFcc[x][QQ][PT2]->SetLineColor(lcolor[PT2]);
FFcc[x][QQ][PT2]->SetParameter(0, xpts[x]);
FFcc[x][QQ][PT2]->SetParameter(1, QQpts[x][QQ]);
FFcc[x][QQ][PT2]->SetParameter(2, PT2pts[PT2]);
FFcc[x][QQ][PT2]->SetParameter(3, LFcc->GetParameter(0));
FFcc[x][QQ][PT2]->SetParameter(4, LFcc->GetParameter(1));
FFcc[x][QQ][PT2]->SetParameter(5, LFcc->GetParameter(2));
FFcc[x][QQ][PT2]->SetParameter(6, LFcc->GetParameter(3));
FFcc[x][QQ][PT2]->SetParameter(7, LFcc->GetParameter(4));
FFcc[x][QQ][PT2]->SetParameter(8, LFcc->GetParameter(5));
FFcc[x][QQ][PT2]->SetParameter(9, LFcc->GetParameter(6));
FFcc[x][QQ][PT2]->SetParameter(10, LFcc->GetParameter(7));
FFcc[x][QQ][PT2]->SetParameter(11, LFcc->GetParameter(8));
FFcc[x][QQ][PT2]->SetParameter(12, LFcc->GetParameter(9));
FFcc[x][QQ][PT2]->SetParameter(13, LFcc->GetParameter(10));
FFcc[x][QQ][PT2]->SetParameter(14, LFcc->GetParameter(11));
FFcc[x][QQ][PT2]->SetParameter(15, LFcc->GetParameter(12));
FFcc[x][QQ][PT2]->SetParameter(16, LFcc->GetParameter(13));
FFcc[x][QQ][PT2]->SetParameter(17, LFcc->GetParameter(14));
FFcc[x][QQ][PT2]->SetParameter(18, LFcc->GetParameter(15));

CANc->cd(x + 1 + (NQQ-1)*Nx - Nx*QQ);
gAcVz[x][QQ][PT2]->GetXaxis()->SetLimits(zpts[0], zpts[Nz-1]);
gAcVz[x][QQ][PT2]->GetXaxis()->SetTitle("z");
if(hapORdata == 0) gAcVz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.25, 0.02);
if(hapORdata == 1) gAcVz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.60, 0.20);
if(PT2==0) gAcVz[x][QQ][PT2]->Draw("AP");
else gAcVz[x][QQ][PT2]->Draw("P");
//FFc[x][QQ][PT2]->Draw("same");

CANcc->cd(x + 1 + (NQQ-1)*Nx - Nx*QQ);
gAccVz[x][QQ][PT2]->GetXaxis()->SetLimits(zpts[0], zpts[Nz-1]);
gAccVz[x][QQ][PT2]->GetXaxis()->SetTitle("z");
if(hapORdata == 0) gAccVz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.02, 0.09);
if(hapORdata == 1) gAccVz[x][QQ][PT2]->GetYaxis()->SetRangeUser(-0.40, 0.40);
if(PT2==0) gAccVz[x][QQ][PT2]->Draw("AP");
else gAccVz[x][QQ][PT2]->Draw("P");
//FFcc[x][QQ][PT2]->Draw("same");

}}}

}
