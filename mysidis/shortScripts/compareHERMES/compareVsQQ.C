// still needs improvements, search "NOTE" for comments

void compareVsQQ()
{
//// desired kinematic point: ////

//float xpt = 0.11; // decent
//float zpt = 0.34;
//float PT2pt = 0.66;

//float xpt = 0.19; // decent
//float zpt = 0.26;
//float PT2pt = 0.31;


float xpt = 0.11; // very nice! (and in relevant kinematics!!)
float zpt = 0.39;
float PT2pt = 0.39;



//float xpt = 0.11; // reasonable, but big coverage gap
//float zpt = 0.29;
//float PT2pt = 0.59;

//float xpt = 0.11; // so-so
//float zpt = 0.39;
//float PT2pt = 0.61;

//float xpt = 0.11; // decent
//float zpt = 0.41;
//float PT2pt = 0.61;



//float xpt = 0.21; // very nice
//float zpt = 0.39;
//float PT2pt = 0.39;

//float xpt = 0.31; // no HERMES data
//float zpt = 0.39;
//float PT2pt = 0.39;


//float xpt = 0.21; // so-so
//float zpt = 0.39;
//float PT2pt = 0.61;

//float xpt = 0.21; // pretty good
//float zpt = 0.41;
//float PT2pt = 0.61;

//float xpt = 0.21; // no data
//float zpt = 0.29;
//float PT2pt = 0.59;

//// approx. HERMES binning scheme: (this just needs to be close enough since all I have is average kinematic values, not the full distributions) ////
const int NxBinsHER = 5;
const int NQQBinsHER = 5;
const int NzBinsHER = 6;
const int NPT2BinsHER = 6;
float xLimitsHER[NxBinsHER+1] = {0.02, 0.05, 0.09, 0.15, 0.25, 0.45};
float zLimitsHER[NzBinsHER+1] = {0.22, 0.31, 0.4, 0.5, 0.6, 0.75, 0.9};
float PT2LimitsHER[NPT2BinsHER+1] = {0.0, 0.05, 0.13, 0.24, 0.45, 0.9, 1.4};

//// my binning scheme: ////
const int NxBinsCLAS = 5;
const int NQQBinsCLAS = 2;
const int NzBinsCLAS = 18;
const int NPT2BinsCLAS = 20;
float xLimitsCLAS[NxBinsCLAS+1] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
float zLimitsCLAS[NzBinsCLAS+1] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9};
float PT2LimitsCLAS[NPT2BinsCLAS+1] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};

//// find the best bins given the above kinematic point: ////
int xBinHER = getBinNo(xpt, NxBinsHER, xLimitsHER);
int zBinHER = getBinNo(zpt, NzBinsHER, zLimitsHER);
int PT2BinHER = getBinNo(PT2pt, NPT2BinsHER, PT2LimitsHER);

int xBinCLAS = getBinNo(xpt, NxBinsCLAS, xLimitsCLAS);
int zBinCLAS = getBinNo(zpt, NzBinsCLAS, zLimitsCLAS);
int PT2BinCLAS = getBinNo(PT2pt, NPT2BinsCLAS, PT2LimitsCLAS);

//// my results: ////
TFile *tfCLASpip = new TFile("pip_5Dtable.root");
TFile *tfCLASpim = new TFile("pim_5Dtable.root");

TCanvas *CLASphihCan = new TCanvas("CLASphihCan", "CLASphihCan", 720, 5, 700, 500);
CLASphihCan->Divide(NQQBinsCLAS, 2); // top row - pip ; bottom row - pim
CLASphihCan->cd(1);

TH1F *hphih_pip[NQQBinsCLAS];
TH1F *hphih_pim[NQQBinsCLAS];
TF1 *ff_pip[NQQBinsCLAS];
TF1 *ff_pim[NQQBinsCLAS];

float CLASpip_aveQQ[NQQBinsCLAS];
float CLASpim_aveQQ[NQQBinsCLAS];
float CLAS_kinFactor1_pip[NQQBinsCLAS], CLAS_kinFactor2a_pip[NQQBinsCLAS], CLAS_kinFactor2b_pip[NQQBinsCLAS];
float CLAS_kinFactor1_pim[NQQBinsCLAS], CLAS_kinFactor2a_pim[NQQBinsCLAS], CLAS_kinFactor2b_pim[NQQBinsCLAS];

for(int iQQ = 0; iQQ < NQQBinsCLAS; iQQ++)
{
if(xBinCLAS > -1 && zBinCLAS > -1 && PT2BinCLAS > -1 && !(xBinCLAS == 4 && iQQ == 1))
{
hphih_pip[iQQ] = (TH1F*) tfCLASpip->Get(Form("CLASpip_hphih_x%i_QQ%i_z%i_PT2%i", xBinCLAS, iQQ, zBinCLAS, PT2BinCLAS));
hphih_pim[iQQ] = (TH1F*) tfCLASpim->Get(Form("CLASpim_hphih_x%i_QQ%i_z%i_PT2%i", xBinCLAS, iQQ, zBinCLAS, PT2BinCLAS));

ff_pip[iQQ] = new TF1(Form("ff_pip_%i", iQQ), "[0]*(1.0 + [1]*cos((3.14159/180.0)*x) + [2]*cos((3.14159/180.0)*2.0*x))", -180, 180);
ff_pip[iQQ]->SetParameters(hphih_pip[iQQ]->GetMaximum(), 0.0, 0.0);
ff_pim[iQQ] = new TF1(Form("ff_pim_%i", iQQ), "[0]*(1.0 + [1]*cos((3.14159/180.0)*x) + [2]*cos((3.14159/180.0)*2.0*x))", -180, 180);
ff_pim[iQQ]->SetParameters(hphih_pim[iQQ]->GetMaximum(), 0.0, 0.0);

CLASphihCan->cd(iQQ+1);
hphih_pip[iQQ]->Fit(ff_pip[iQQ], "", "", -180, 180);
hphih_pip[iQQ]->GetYaxis()->SetRangeUser(0, 1.2*hphih_pip[iQQ]->GetMaximum());
hphih_pip[iQQ]->Draw("E");

CLASphihCan->cd(NQQBinsCLAS+iQQ+1);
hphih_pim[iQQ]->Fit(ff_pim[iQQ], "", "", -180, 180);
hphih_pim[iQQ]->GetYaxis()->SetRangeUser(0, 1.2*hphih_pim[iQQ]->GetMaximum());
hphih_pim[iQQ]->Draw("E");

TH1F *binInfo_pip = (TH1F*) tfCLASpip->Get(Form("CLASpip_binInfo_x%i_QQ%i_z%i_PT2%i", xBinCLAS, iQQ, zBinCLAS, PT2BinCLAS));

float avex = binInfo_pip->GetBinContent(1);
CLASpip_aveQQ[iQQ] = binInfo_pip->GetBinContent(2);
float avey = binInfo_pip->GetBinContent(5);

float gamma = (2.0*0.938*avex)/sqrt(CLASpip_aveQQ[iQQ]);
float epsilon = (1.0 - avey + 0.25*gamma*gamma*avey*avey) / (1.0 - avey + 0.5*avey*avey + 0.25*gamma*gamma*avey*avey);

CLAS_kinFactor1_pip[iQQ] = sqrt(CLASpip_aveQQ[iQQ])/sqrt(2.0*epsilon*(1.0 + epsilon));
CLAS_kinFactor2a_pip[iQQ] = epsilon;
CLAS_kinFactor2b_pip[iQQ] = CLASpip_aveQQ[iQQ]*epsilon;


TH1F *binInfo_pim = (TH1F*) tfCLASpim->Get(Form("CLASpim_binInfo_x%i_QQ%i_z%i_PT2%i", xBinCLAS, iQQ, zBinCLAS, PT2BinCLAS));

avex = binInfo_pim->GetBinContent(1);
CLASpim_aveQQ[iQQ] = binInfo_pim->GetBinContent(2);
avey = binInfo_pim->GetBinContent(5);

gamma = (2.0*0.938*avex)/sqrt(CLASpim_aveQQ[iQQ]);
epsilon = (1.0 - avey + 0.25*gamma*gamma*avey*avey) / (1.0 - avey + 0.5*avey*avey + 0.25*gamma*gamma*avey*avey);

CLAS_kinFactor1_pim[iQQ] = sqrt(CLASpim_aveQQ[iQQ])/sqrt(2.0*epsilon*(1.0 + epsilon));
CLAS_kinFactor2a_pim[iQQ] = epsilon;
CLAS_kinFactor2b_pim[iQQ] = CLASpim_aveQQ[iQQ]*epsilon;
}
else
{
ff_pip[iQQ] = new TF1(Form("ff_pip_%i", iQQ), "[0]*(1.0 + [1]*cos((3.14159/180.0)*x) + [2]*cos((3.14159/180.0)*2.0*x))", -180, 180);
ff_pip[iQQ]->SetParameters(0.0, 0.0, 0.0);
ff_pim[iQQ] = new TF1(Form("ff_pim_%i", iQQ), "[0]*(1.0 + [1]*cos((3.14159/180.0)*x) + [2]*cos((3.14159/180.0)*2.0*x))", -180, 180);
ff_pim[iQQ]->SetParameters(0.0, 0.0, 0.0);
CLAS_kinFactor1_pip[iQQ] = 0.0;
CLAS_kinFactor2a_pip[iQQ] = 0.0;
CLAS_kinFactor2b_pip[iQQ] = 0.0;
CLAS_kinFactor1_pim[iQQ] = 0.0;
CLAS_kinFactor2a_pim[iQQ] = 0.0;
CLAS_kinFactor2b_pim[iQQ] = 0.0;
}
}

//// HERMES results: ////
TFile *tfHERpip = new TFile("HERMES_Hyd_pip.mom.root");
TFile *tfHERpim = new TFile("HERMES_Hyd_pim.mom.root");

float HERpip_aveQQ[NQQBinsHER], HERpip_Ac[NQQBinsHER], HERpip_Ac_e[NQQBinsHER], HERpip_Acc[NQQBinsHER], HERpip_Acc_e[NQQBinsHER];
float HERpim_aveQQ[NQQBinsHER], HERpim_Ac[NQQBinsHER], HERpim_Ac_e[NQQBinsHER], HERpim_Acc[NQQBinsHER], HERpim_Acc_e[NQQBinsHER];
float HER_kinFactor1_pip[NQQBinsHER], HER_kinFactor2a_pip[NQQBinsHER], HER_kinFactor2b_pip[NQQBinsHER];
float HER_kinFactor1_pim[NQQBinsHER], HER_kinFactor2a_pim[NQQBinsHER], HER_kinFactor2b_pim[NQQBinsHER];

for(int iQQ = 0; iQQ < NQQBinsHER; iQQ++)
{
if(xBinHER > -1 && zBinHER > -1 && PT2BinHER > -1)
{
TH1F *tempHistoPip = (TH1F*) tfHERpip->Get(Form("HERpip_binInfo_x%i_QQ%i_z%i_PT2%i", xBinHER, iQQ, zBinHER, PT2BinHER));
float avex = tempHistoPip->GetBinContent(1);
HERpip_aveQQ[iQQ] = tempHistoPip->GetBinContent(2);
float avey = tempHistoPip->GetBinContent(5);
HERpip_Ac[iQQ] = tempHistoPip->GetBinContent(6);
HERpip_Ac_e[iQQ] = tempHistoPip->GetBinContent(7);
HERpip_Acc[iQQ] = tempHistoPip->GetBinContent(9);
HERpip_Acc_e[iQQ] = tempHistoPip->GetBinContent(10);

float gamma = (2.0*0.938*avex)/sqrt(HERpip_aveQQ[iQQ]);
float epsilon = (1.0 - avey + 0.25*gamma*gamma*avey*avey) / (1.0 - avey + 0.5*avey*avey + 0.25*gamma*gamma*avey*avey);

HER_kinFactor1_pip[iQQ] = sqrt(HERpip_aveQQ[iQQ])/sqrt(2.0*epsilon*(1.0 + epsilon));
HER_kinFactor2a_pip[iQQ] = epsilon;
HER_kinFactor2b_pip[iQQ] = HERpip_aveQQ[iQQ]*epsilon;


TH1F *tempHistoPim = (TH1F*) tfHERpim->Get(Form("HERpim_binInfo_x%i_QQ%i_z%i_PT2%i", xBinHER, iQQ, zBinHER, PT2BinHER));
avex = tempHistoPim->GetBinContent(1);
HERpim_aveQQ[iQQ] = tempHistoPim->GetBinContent(2);
avey = tempHistoPim->GetBinContent(5);
HERpim_Ac[iQQ] = tempHistoPim->GetBinContent(6);
HERpim_Ac_e[iQQ] = tempHistoPim->GetBinContent(7);
HERpim_Acc[iQQ] = tempHistoPim->GetBinContent(9);
HERpim_Acc_e[iQQ] = tempHistoPim->GetBinContent(10);

gamma = (2.0*0.938*avex)/sqrt(HERpim_aveQQ[iQQ]);
epsilon = (1.0 - avey + 0.25*gamma*gamma*avey*avey) / (1.0 - avey + 0.5*avey*avey + 0.25*gamma*gamma*avey*avey);

HER_kinFactor1_pim[iQQ] = sqrt(HERpim_aveQQ[iQQ])/sqrt(2.0*epsilon*(1.0 + epsilon));
HER_kinFactor2a_pim[iQQ] = epsilon;
HER_kinFactor2b_pim[iQQ] = HERpim_aveQQ[iQQ]*epsilon;
}
else
{
HERpip_aveQQ[iQQ] = 0.0;
HERpip_Ac[iQQ] = 0.0;
HERpip_Ac_e[iQQ] = 0.0;
HERpip_Acc[iQQ] = 0.0;
HERpip_Acc_e[iQQ] = 0.0;

HERpim_aveQQ[iQQ] = 0.0;
HERpim_Ac[iQQ] = 0.0;
HERpim_Ac_e[iQQ] = 0.0;
HERpim_Acc[iQQ] = 0.0;
HERpim_Acc_e[iQQ] = 0.0;

HER_kinFactor1_pip[iQQ] = 0.0;
HER_kinFactor2a_pip[iQQ] = 0.0;
HER_kinFactor2b_pip[iQQ] = 0.0;

HER_kinFactor1_pim[iQQ] = 0.0;
HER_kinFactor2a_pim[iQQ] = 0.0;
HER_kinFactor2b_pim[iQQ] = 0.0;
}
}

//// compare results: ////
TGraphErrors *gCLASpipAc = new TGraphErrors();
gCLASpipAc->SetMarkerStyle(20);
gCLASpipAc->SetLineColor(kRed);
gCLASpipAc->SetMarkerColor(kRed);
TGraphErrors *gCLASpimAc = new TGraphErrors();
gCLASpimAc->SetMarkerStyle(20);
gCLASpimAc->SetLineColor(kBlue);
gCLASpimAc->SetMarkerColor(kBlue);
TGraphErrors *gHERpipAc = new TGraphErrors();
gHERpipAc->SetMarkerStyle(22);
gHERpipAc->SetLineColor(kRed);
gHERpipAc->SetMarkerColor(kRed);
TGraphErrors *gHERpimAc = new TGraphErrors();
gHERpimAc->SetMarkerStyle(22);
gHERpimAc->SetLineColor(kBlue);
gHERpimAc->SetMarkerColor(kBlue);

TGraphErrors *gCLASpipAcc = new TGraphErrors();
gCLASpipAcc->SetMarkerStyle(20);
gCLASpipAcc->SetLineColor(kRed);
gCLASpipAcc->SetMarkerColor(kRed);
TGraphErrors *gCLASpimAcc = new TGraphErrors();
gCLASpimAcc->SetMarkerStyle(20);
gCLASpimAcc->SetLineColor(kBlue);
gCLASpimAcc->SetMarkerColor(kBlue);
TGraphErrors *gHERpipAcc = new TGraphErrors();
gHERpipAcc->SetMarkerStyle(22);
gHERpipAcc->SetLineColor(kRed);
gHERpipAcc->SetMarkerColor(kRed);
TGraphErrors *gHERpimAcc = new TGraphErrors();
gHERpimAcc->SetMarkerStyle(22);
gHERpimAcc->SetLineColor(kBlue);
gHERpimAcc->SetMarkerColor(kBlue);

TGraphErrors *gCLASpipQuantity1 = new TGraphErrors();
gCLASpipQuantity1->SetMarkerStyle(20);
gCLASpipQuantity1->SetLineColor(kRed);
gCLASpipQuantity1->SetMarkerColor(kRed);
TGraphErrors *gCLASpimQuantity1 = new TGraphErrors();
gCLASpimQuantity1->SetMarkerStyle(20);
gCLASpimQuantity1->SetLineColor(kBlue);
gCLASpimQuantity1->SetMarkerColor(kBlue);
TGraphErrors *gHERpipQuantity1 = new TGraphErrors();
gHERpipQuantity1->SetMarkerStyle(22);
gHERpipQuantity1->SetLineColor(kRed);
gHERpipQuantity1->SetMarkerColor(kRed);
TGraphErrors *gHERpimQuantity1 = new TGraphErrors();
gHERpimQuantity1->SetMarkerStyle(22);
gHERpimQuantity1->SetLineColor(kBlue);
gHERpimQuantity1->SetMarkerColor(kBlue);

TGraphErrors *gCLASpipQuantity2a = new TGraphErrors();
gCLASpipQuantity2a->SetMarkerStyle(20);
gCLASpipQuantity2a->SetLineColor(kRed);
gCLASpipQuantity2a->SetMarkerColor(kRed);
TGraphErrors *gCLASpimQuantity2a = new TGraphErrors();
gCLASpimQuantity2a->SetMarkerStyle(20);
gCLASpimQuantity2a->SetLineColor(kBlue);
gCLASpimQuantity2a->SetMarkerColor(kBlue);
TGraphErrors *gHERpipQuantity2a = new TGraphErrors();
gHERpipQuantity2a->SetMarkerStyle(22);
gHERpipQuantity2a->SetLineColor(kRed);
gHERpipQuantity2a->SetMarkerColor(kRed);
TGraphErrors *gHERpimQuantity2a = new TGraphErrors();
gHERpimQuantity2a->SetMarkerStyle(22);
gHERpimQuantity2a->SetLineColor(kBlue);
gHERpimQuantity2a->SetMarkerColor(kBlue);

TGraphErrors *gCLASpipQuantity2b = new TGraphErrors();
gCLASpipQuantity2b->SetMarkerStyle(20);
gCLASpipQuantity2b->SetLineColor(kRed);
gCLASpipQuantity2b->SetMarkerColor(kRed);
TGraphErrors *gCLASpimQuantity2b = new TGraphErrors();
gCLASpimQuantity2b->SetMarkerStyle(20);
gCLASpimQuantity2b->SetLineColor(kBlue);
gCLASpimQuantity2b->SetMarkerColor(kBlue);
TGraphErrors *gHERpipQuantity2b = new TGraphErrors();
gHERpipQuantity2b->SetMarkerStyle(22);
gHERpipQuantity2b->SetLineColor(kRed);
gHERpipQuantity2b->SetMarkerColor(kRed);
TGraphErrors *gHERpimQuantity2b = new TGraphErrors();
gHERpimQuantity2b->SetMarkerStyle(22);
gHERpimQuantity2b->SetLineColor(kBlue);
gHERpimQuantity2b->SetMarkerColor(kBlue);

for(int iQQ = 0; iQQ < NQQBinsCLAS; iQQ++)
{
if(fabs(ff_pip[iQQ]->GetParameter(1)) > 0.000001 && fabs(ff_pip[iQQ]->GetParError(1)) > 0.000001)
{
gCLASpipAc->SetPoint(gCLASpipAc->GetN(), CLASpip_aveQQ[iQQ], ff_pip[iQQ]->GetParameter(1));
gCLASpipAc->SetPointError(gCLASpipAc->GetN()-1, 0.0, ff_pip[iQQ]->GetParError(1));
gCLASpipAcc->SetPoint(gCLASpipAcc->GetN(), CLASpip_aveQQ[iQQ], ff_pip[iQQ]->GetParameter(2));
gCLASpipAcc->SetPointError(gCLASpipAcc->GetN()-1, 0.0, ff_pip[iQQ]->GetParError(2));
gCLASpipQuantity1->SetPoint(gCLASpipQuantity1->GetN(), CLASpip_aveQQ[iQQ], ff_pip[iQQ]->GetParameter(1)*CLAS_kinFactor1_pip[iQQ]);
gCLASpipQuantity1->SetPointError(gCLASpipQuantity1->GetN()-1, 0.0, ff_pip[iQQ]->GetParError(1)*CLAS_kinFactor1_pip[iQQ]);
gCLASpipQuantity2a->SetPoint(gCLASpipQuantity2a->GetN(), CLASpip_aveQQ[iQQ], ff_pip[iQQ]->GetParameter(2)*CLAS_kinFactor2a_pip[iQQ]);
gCLASpipQuantity2a->SetPointError(gCLASpipQuantity2a->GetN()-1, 0.0, ff_pip[iQQ]->GetParError(2)*CLAS_kinFactor2a_pip[iQQ]);
gCLASpipQuantity2b->SetPoint(gCLASpipQuantity2b->GetN(), CLASpip_aveQQ[iQQ], ff_pip[iQQ]->GetParameter(2)*CLAS_kinFactor2b_pip[iQQ]);
gCLASpipQuantity2b->SetPointError(gCLASpipQuantity2b->GetN()-1, 0.0, ff_pip[iQQ]->GetParError(2)*CLAS_kinFactor2b_pip[iQQ]);
}

if(fabs(ff_pim[iQQ]->GetParameter(1)) > 0.000001 && fabs(ff_pim[iQQ]->GetParError(1)) > 0.000001)
{
gCLASpimAc->SetPoint(gCLASpimAc->GetN(), CLASpim_aveQQ[iQQ], ff_pim[iQQ]->GetParameter(1));
gCLASpimAc->SetPointError(gCLASpimAc->GetN()-1, 0.0, ff_pim[iQQ]->GetParError(1));
gCLASpimAcc->SetPoint(gCLASpimAcc->GetN(), CLASpim_aveQQ[iQQ], ff_pim[iQQ]->GetParameter(2));
gCLASpimAcc->SetPointError(gCLASpimAcc->GetN()-1, 0.0, ff_pim[iQQ]->GetParError(2));
gCLASpimQuantity1->SetPoint(gCLASpimQuantity1->GetN(), CLASpim_aveQQ[iQQ], ff_pim[iQQ]->GetParameter(1)*CLAS_kinFactor1_pim[iQQ]);
gCLASpimQuantity1->SetPointError(gCLASpimQuantity1->GetN()-1, 0.0, ff_pim[iQQ]->GetParError(1)*CLAS_kinFactor1_pim[iQQ]);
gCLASpimQuantity2a->SetPoint(gCLASpimQuantity2a->GetN(), CLASpim_aveQQ[iQQ], ff_pim[iQQ]->GetParameter(2)*CLAS_kinFactor2a_pim[iQQ]);
gCLASpimQuantity2a->SetPointError(gCLASpimQuantity2a->GetN()-1, 0.0, ff_pim[iQQ]->GetParError(2)*CLAS_kinFactor2a_pim[iQQ]);
gCLASpimQuantity2b->SetPoint(gCLASpimQuantity2b->GetN(), CLASpim_aveQQ[iQQ], ff_pim[iQQ]->GetParameter(2)*CLAS_kinFactor2b_pim[iQQ]);
gCLASpimQuantity2b->SetPointError(gCLASpimQuantity2b->GetN()-1, 0.0, ff_pim[iQQ]->GetParError(2)*CLAS_kinFactor2b_pim[iQQ]);
}
}

for(int iQQ = 0; iQQ < NQQBinsHER; iQQ++)
{
if(fabs(HERpip_Ac[iQQ]) > 0.000001 && fabs(HERpip_Ac_e[iQQ]) > 0.000001)
{
gHERpipAc->SetPoint(gHERpipAc->GetN(), HERpip_aveQQ[iQQ], HERpip_Ac[iQQ]);
gHERpipAc->SetPointError(gHERpipAc->GetN()-1, 0.0, HERpip_Ac_e[iQQ]);
gHERpipAcc->SetPoint(gHERpipAcc->GetN(), HERpip_aveQQ[iQQ], HERpip_Acc[iQQ]);
gHERpipAcc->SetPointError(gHERpipAcc->GetN()-1, 0.0, HERpip_Acc_e[iQQ]);
gHERpipQuantity1->SetPoint(gHERpipQuantity1->GetN(), HERpip_aveQQ[iQQ], HERpip_Ac[iQQ]*HER_kinFactor1_pip[iQQ]);
gHERpipQuantity1->SetPointError(gHERpipQuantity1->GetN()-1, 0.0, HERpip_Ac_e[iQQ]*HER_kinFactor1_pip[iQQ]);
gHERpipQuantity2a->SetPoint(gHERpipQuantity2a->GetN(), HERpip_aveQQ[iQQ], HERpip_Acc[iQQ]*HER_kinFactor2a_pip[iQQ]);
gHERpipQuantity2a->SetPointError(gHERpipQuantity2a->GetN()-1, 0.0, HERpip_Acc_e[iQQ]*HER_kinFactor2a_pip[iQQ]);
gHERpipQuantity2b->SetPoint(gHERpipQuantity2b->GetN(), HERpip_aveQQ[iQQ], HERpip_Acc[iQQ]*HER_kinFactor2b_pip[iQQ]);
gHERpipQuantity2b->SetPointError(gHERpipQuantity2b->GetN()-1, 0.0, HERpip_Acc_e[iQQ]*HER_kinFactor2b_pip[iQQ]);
}

if(fabs(HERpim_Ac[iQQ]) > 0.000001 && fabs(HERpim_Ac_e[iQQ]) > 0.000001)
{
gHERpimAc->SetPoint(gHERpimAc->GetN(), HERpim_aveQQ[iQQ], HERpim_Ac[iQQ]);
gHERpimAc->SetPointError(gHERpimAc->GetN()-1, 0.0, HERpim_Ac_e[iQQ]);
gHERpimAcc->SetPoint(gHERpimAcc->GetN(), HERpim_aveQQ[iQQ], HERpim_Acc[iQQ]);
gHERpimAcc->SetPointError(gHERpimAcc->GetN()-1, 0.0, HERpim_Acc_e[iQQ]);
gHERpimQuantity1->SetPoint(gHERpimQuantity1->GetN(), HERpim_aveQQ[iQQ], HERpim_Ac[iQQ]*HER_kinFactor1_pim[iQQ]);
gHERpimQuantity1->SetPointError(gHERpimQuantity1->GetN()-1, 0.0, HERpim_Ac_e[iQQ]*HER_kinFactor1_pim[iQQ]);
gHERpimQuantity2a->SetPoint(gHERpimQuantity2a->GetN(), HERpim_aveQQ[iQQ], HERpim_Acc[iQQ]*HER_kinFactor2a_pim[iQQ]);
gHERpimQuantity2a->SetPointError(gHERpimQuantity2a->GetN()-1, 0.0, HERpim_Acc_e[iQQ]*HER_kinFactor2a_pim[iQQ]);
gHERpimQuantity2b->SetPoint(gHERpimQuantity2b->GetN(), HERpim_aveQQ[iQQ], HERpim_Acc[iQQ]*HER_kinFactor2b_pim[iQQ]);
gHERpimQuantity2b->SetPointError(gHERpimQuantity2b->GetN()-1, 0.0, HERpim_Acc_e[iQQ]*HER_kinFactor2b_pim[iQQ]);
}
}

TCanvas *canAc = new TCanvas("canAc", "canAc");
canAc->cd(1);
TMultiGraph *mgAc = new TMultiGraph();
mgAc->Add(gCLASpimAc);
mgAc->Add(gHERpimAc);
mgAc->Add(gCLASpipAc);
mgAc->Add(gHERpipAc);
mgAc->Draw("AP");

TCanvas *canAcc = new TCanvas("canAcc", "canAcc");
canAcc->cd(1);
TMultiGraph *mgAcc = new TMultiGraph();
mgAcc->Add(gCLASpimAcc);
mgAcc->Add(gHERpimAcc);
mgAcc->Add(gCLASpipAcc);
mgAcc->Add(gHERpipAcc);
mgAcc->Draw("AP");

TCanvas *canQ1 = new TCanvas("canQ1", "canQ1");
canQ1->cd(1);
TMultiGraph *mgQuantity1 = new TMultiGraph();
mgQuantity1->Add(gCLASpimQuantity1);
mgQuantity1->Add(gHERpimQuantity1);
mgQuantity1->Add(gCLASpipQuantity1);
mgQuantity1->Add(gHERpipQuantity1);
mgQuantity1->Draw("AP");

TCanvas *canQ2a = new TCanvas("canQ2a", "canQ2a");
canQ2a->cd(1);
TMultiGraph *mgQuantity2a = new TMultiGraph();
mgQuantity2a->Add(gCLASpimQuantity2a);
mgQuantity2a->Add(gHERpimQuantity2a);
mgQuantity2a->Add(gCLASpipQuantity2a);
mgQuantity2a->Add(gHERpipQuantity2a);
mgQuantity2a->Draw("AP");

TCanvas *canQ2b = new TCanvas("canQ2b", "canQ2b");
canQ2b->cd(1);
TMultiGraph *mgQuantity2b = new TMultiGraph();
mgQuantity2b->Add(gCLASpimQuantity2b);
mgQuantity2b->Add(gHERpimQuantity2b);
mgQuantity2b->Add(gCLASpipQuantity2b);
mgQuantity2b->Add(gHERpipQuantity2b);
mgQuantity2b->Draw("AP");
}

// %%%%%%%%%%%%%%%%%%%% my functions: %%%%%%%%%%%%%%%%%%%% //

int getBinNo(float val, int length, float limits[]) // will return a value between 0 and length-1
{
if(val < limits[0] || val >= limits[length-1]) return -123;
int binNo = 0;
while(val >= limits[binNo+1]) binNo++;
return binNo;
}
