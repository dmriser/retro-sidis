// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void MAcAccVPT2_wSystematics(int xBin = 0, int QQBin = 0, int zBin = 6)
{
gStyle->SetOptStat(0);

string YscaleOpt = "smart"; // "fixed" or "smart"

bool Mlog = 0;

int iMarkerStyle_pip = 4;
int iMarkerStyle_pim = 23;

const int NPT2Bins = 20;
float PT2Min = 0;
float PT2Max = 1;

TCanvas *can = new TCanvas("can", "can", 20, 20, 1550, 500);
can->Divide(3, 1, 0.00001, 0.00001);

TH1F *hpipM, *hpipAc, *hpipAcc;
TH1F *hpimM, *hpimAc, *hpimAcc;

hpipM = new TH1F("hpipM", "hpipM", NPT2Bins, PT2Min, PT2Max);
hpipAc = new TH1F("hpipAc", "hpipAc", NPT2Bins, PT2Min, PT2Max);
hpipAcc = new TH1F("hpipAcc", "hpipAcc", NPT2Bins, PT2Min, PT2Max);
hpimM = new TH1F("hpimM", "hpimM", NPT2Bins, PT2Min, PT2Max);
hpimAc = new TH1F("hpimAc", "hpimAc", NPT2Bins, PT2Min, PT2Max);
hpimAcc = new TH1F("hpimAcc", "hpimAcc", NPT2Bins, PT2Min, PT2Max);

TFile *MAcAccFile = new TFile("/home/nah/mysidis-histos/MAcAcc.root", "READ");
TFile *sysFile = new TFile("/home/nah/mysidis-histos/Total_systematics.root", "READ");

/// pip: ///
for(int k = 0; k < NPT2Bins; k++)
{
float pipM, pipMe, pipAc, pipAce, pipAcc, pipAcce;
string MhistoName = Form("pip_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);
string AcAcchistoName = Form("pip_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);

if(MAcAccFile->GetListOfKeys()->Contains(MhistoName.c_str()))
{
TH1F *Mhisto = (TH1F*) MAcAccFile->Get(MhistoName.c_str());
pipM = Mhisto->GetBinContent(1);
pipMe = Mhisto->GetBinError(1);

hpipM->SetBinContent(k+1, pipM);
hpipM->SetBinError(k+1, pipMe);
}

if(MAcAccFile->GetListOfKeys()->Contains(AcAcchistoName.c_str()))
{
TH1F *AcAcchisto = (TH1F*) MAcAccFile->Get(AcAcchistoName.c_str());
pipAc = AcAcchisto->GetBinContent(1);
pipAce = AcAcchisto->GetBinError(1);
pipAcc = AcAcchisto->GetBinContent(2);
pipAcce = AcAcchisto->GetBinError(2);

hpipAc->SetBinContent(k+1, pipAc);
hpipAc->SetBinError(k+1, pipAce);
hpipAcc->SetBinContent(k+1, pipAcc);
hpipAcc->SetBinError(k+1, pipAcce);
}

}

/// pim: ///
for(int k = 0; k < NPT2Bins; k++)
{
float pimM, pimMe, pimAc, pimAce, pimAcc, pimAcce;
string MhistoName = Form("pim_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);
string AcAcchistoName = Form("pim_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);

if(MAcAccFile->GetListOfKeys()->Contains(MhistoName.c_str()))
{
TH1F *Mhisto = (TH1F*) MAcAccFile->Get(MhistoName.c_str());
pimM = Mhisto->GetBinContent(1);
pimMe = Mhisto->GetBinError(1);

hpimM->SetBinContent(k+1, pimM);
hpimM->SetBinError(k+1, pimMe);
}

if(MAcAccFile->GetListOfKeys()->Contains(AcAcchistoName.c_str()))
{
TH1F *AcAcchisto = (TH1F*) MAcAccFile->Get(AcAcchistoName.c_str());
pimAc = AcAcchisto->GetBinContent(1);
pimAce = AcAcchisto->GetBinError(1);
pimAcc = AcAcchisto->GetBinContent(2);
pimAcce = AcAcchisto->GetBinError(2);

hpimAc->SetBinContent(k+1, pimAc);
hpimAc->SetBinError(k+1, pimAce);
hpimAcc->SetBinContent(k+1, pimAcc);
hpimAcc->SetBinError(k+1, pimAcce);
}

}

/// Draw: ///
can->cd(1)->SetTopMargin(0.08);
can->cd(1)->SetBottomMargin(0.18);
can->cd(1)->SetLeftMargin(0.14);
can->cd(1)->SetRightMargin(0.06);
can->cd(1)->SetGrid();
if(Mlog) can->cd(1)->SetLogy();
hpipM->SetLineColor(kRed);
hpipM->SetMarkerStyle(iMarkerStyle_pip);
if(YscaleOpt == "smart" && Mlog) hpipM->GetYaxis()->SetRangeUser(1, 2.0*max(hpipM->GetMaximum(), hpimM->GetMaximum()));
if(YscaleOpt == "smart" && !Mlog) hpipM->GetYaxis()->SetRangeUser(-0.2*max(hpipM->GetMaximum(), hpimM->GetMaximum()), 1.1*max(hpipM->GetMaximum(), hpimM->GetMaximum()));
if(YscaleOpt == "fixed") hpipM->GetYaxis()->SetRangeUser(-2000, 12500);
hpipM->SetTitle(Form("A_{0} for #pi+ (red circ.) and #pi- (bl. tri.), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipM->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipM->GetXaxis()->SetTitleSize(0.05);
hpipM->GetXaxis()->SetLabelSize(0.05);
hpipM->GetYaxis()->SetLabelSize(0.05);
hpipM->SetLineWidth(2);
hpipM->Draw("same E1");
hpimM->SetLineColor(kBlue);
hpimM->SetMarkerStyle(iMarkerStyle_pim);
hpimM->SetLineWidth(2);
hpimM->Draw("same E1");

can->cd(2)->SetTopMargin(0.08);
can->cd(2)->SetBottomMargin(0.18);
can->cd(2)->SetLeftMargin(0.14);
can->cd(2)->SetRightMargin(0.06);
can->cd(2)->SetGrid();
hpipAc->SetLineColor(kRed);
hpipAc->SetMarkerStyle(iMarkerStyle_pip);
if(YscaleOpt == "smart") hpipAc->GetYaxis()->SetRangeUser(min(hpipAc->GetMinimum(), hpimAc->GetMinimum()) - 0.20, max(hpipAc->GetMaximum(), hpimAc->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAc->GetYaxis()->SetRangeUser(-0.42, 0.3);
hpipAc->SetTitle(Form("A^{cos#phi_{h}}_{UU} for #pi+ (red circ.) and #pi- (bl. tri.), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipAc->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipAc->GetXaxis()->SetTitleSize(0.05);
hpipAc->GetXaxis()->SetLabelSize(0.05);
hpipAc->GetYaxis()->SetLabelSize(0.05);
hpipAc->SetLineWidth(2);
hpipAc->Draw("same E1");
hpimAc->SetLineColor(kBlue);
hpimAc->SetMarkerStyle(iMarkerStyle_pim);
hpimAc->SetLineWidth(2);
hpimAc->Draw("same E1");

can->cd(3)->SetTopMargin(0.08);
can->cd(3)->SetBottomMargin(0.18);
can->cd(3)->SetLeftMargin(0.14);
can->cd(3)->SetRightMargin(0.06);
can->cd(3)->SetGrid();
hpipAcc->SetLineColor(kRed);
hpipAcc->SetMarkerStyle(iMarkerStyle_pip);
if(YscaleOpt == "smart") hpipAcc->GetYaxis()->SetRangeUser(min(hpipAcc->GetMinimum(), hpimAcc->GetMinimum()) - 0.20, max(hpipAcc->GetMaximum(), hpimAcc->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAcc->GetYaxis()->SetRangeUser(-0.4, 0.42);
hpipAcc->SetTitle(Form("A^{cos2#phi_{h}}_{UU} for #pi+ (red circ.) and #pi- (bl. tri.), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipAcc->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipAcc->GetXaxis()->SetTitleSize(0.05);
hpipAcc->GetXaxis()->SetLabelSize(0.05);
hpipAcc->GetYaxis()->SetLabelSize(0.05);
hpipAcc->SetLineWidth(2);
hpipAcc->Draw("same E1");
hpimAcc->SetLineColor(kBlue);
hpimAcc->SetMarkerStyle(iMarkerStyle_pim);
hpimAcc->SetLineWidth(2);
hpimAcc->Draw("same E1");

///////////////////////////// systematics: //////////////////////

TGraphErrors *grpipSys_M = new TGraphErrors();
TGraphErrors *grpimSys_M = new TGraphErrors();
can->cd(1)->Update();
float MsysYval;
if(Mlog) MsysYval = 10;
if(!Mlog) MsysYval = gPad->GetUymin() + 0.15*(gPad->GetUymax() - gPad->GetUymin());

TGraphErrors *grpipSys_Ac = new TGraphErrors();
TGraphErrors *grpimSys_Ac = new TGraphErrors();
can->cd(2)->Update();
float AcsysYval = gPad->GetUymin() + 0.15*(gPad->GetUymax() - gPad->GetUymin());

TGraphErrors *grpipSys_Acc = new TGraphErrors();
TGraphErrors *grpimSys_Acc = new TGraphErrors();
can->cd(3)->Update();
float AccsysYval = gPad->GetUymin() + 0.15*(gPad->GetUymax() - gPad->GetUymin());

for(int k = 0; k < NPT2Bins; k++)
{
float Msys, Acsys, Accsys;

// pip:
string pipSysHistName = Form("hMAcAccSys_pip_%i_%i_%i_%i", xBin, QQBin, zBin, k);
string pipMhistoName = Form("pip_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);
string pipAcAcchistoName = Form("pip_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);

if(sysFile->GetListOfKeys()->Contains(pipSysHistName.c_str()))
{
TH1F *pipSysHist = (TH1F*) sysFile->Get(pipSysHistName.c_str());

if(MAcAccFile->GetListOfKeys()->Contains(pipMhistoName.c_str()))
{
Msys = pipSysHist->GetBinContent(1);
grpipSys_M->SetPoint(grpipSys_M->GetN(), hpipM->GetBinCenter(k+1), MsysYval);
grpipSys_M->SetPointError(grpipSys_M->GetN()-1, 0, 0.5*Msys); // factor of 0.5 since error bar goes up and down... want the full height to = Msys
}
if(MAcAccFile->GetListOfKeys()->Contains(pipAcAcchistoName.c_str()))
{
Acsys = pipSysHist->GetBinContent(2);
Accsys = pipSysHist->GetBinContent(3);
grpipSys_Ac->SetPoint(grpipSys_Ac->GetN(), hpipAc->GetBinCenter(k+1), AcsysYval);
grpipSys_Ac->SetPointError(grpipSys_Ac->GetN()-1, 0, 0.5*Acsys);
grpipSys_Acc->SetPoint(grpipSys_Acc->GetN(), hpipAcc->GetBinCenter(k+1), AccsysYval);
grpipSys_Acc->SetPointError(grpipSys_Acc->GetN()-1, 0, 0.5*Accsys);
}
}

// pim:
string pimSysHistName = Form("hMAcAccSys_pim_%i_%i_%i_%i", xBin, QQBin, zBin, k);
string pimMhistoName = Form("pim_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);
string pimAcAcchistoName = Form("pim_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, k);

if(sysFile->GetListOfKeys()->Contains(pimSysHistName.c_str()))
{
TH1F *pimSysHist = (TH1F*) sysFile->Get(pimSysHistName.c_str());

if(MAcAccFile->GetListOfKeys()->Contains(pimMhistoName.c_str()))
{
Msys = pimSysHist->GetBinContent(1);
grpimSys_M->SetPoint(grpimSys_M->GetN(), hpimM->GetBinCenter(k+1), MsysYval);
grpimSys_M->SetPointError(grpimSys_M->GetN()-1, 0, 0.5*Msys); // factor of 0.5 since error bar goes up and down... want the full height to = Msys
}
if(MAcAccFile->GetListOfKeys()->Contains(pimAcAcchistoName.c_str()))
{
Acsys = pimSysHist->GetBinContent(2);
Accsys = pimSysHist->GetBinContent(3);
grpimSys_Ac->SetPoint(grpimSys_Ac->GetN(), hpimAc->GetBinCenter(k+1), AcsysYval);
grpimSys_Ac->SetPointError(grpimSys_Ac->GetN()-1, 0, 0.5*Acsys);
grpimSys_Acc->SetPoint(grpimSys_Acc->GetN(), hpimAcc->GetBinCenter(k+1), AccsysYval);
grpimSys_Acc->SetPointError(grpimSys_Acc->GetN()-1, 0, 0.5*Accsys);
}
}

}


can->cd(1);
grpipSys_M->SetFillColor(kRed);
grpipSys_M->SetFillStyle(3002);
grpipSys_M->Draw("E3 same");
grpimSys_M->SetFillColor(kBlue);
grpimSys_M->SetFillStyle(3005);
grpimSys_M->Draw("E3 same");

can->cd(2);
grpipSys_Ac->SetFillColor(kRed);
grpipSys_Ac->SetFillStyle(3002);
grpipSys_Ac->Draw("E3 same");
grpimSys_Ac->SetFillColor(kBlue);
grpimSys_Ac->SetFillStyle(3005);
grpimSys_Ac->Draw("E3 same");

can->cd(3);
grpipSys_Acc->SetFillColor(kRed);
grpipSys_Acc->SetFillStyle(3002);
grpipSys_Acc->Draw("E3 same");
grpimSys_Acc->SetFillColor(kBlue);
grpimSys_Acc->SetFillStyle(3005);
grpimSys_Acc->Draw("E3 same");


TCanvas *vcan = new TCanvas("vcan", "vcan", 1400, 5, 500, 1000);
vcan->Divide(1, 3, 0, 0);

vcan->cd(1);
vcan->cd(1)->SetGrid();
if(Mlog) vcan->cd(1)->SetLogy();
hpipM->Draw("same E1");
hpimM->Draw("same E1");
grpipSys_M->Draw("E3 same");
grpimSys_M->Draw("E3 same");
vcan->cd(2);
vcan->cd(2)->SetGrid();
hpipAc->Draw("same E1");
hpimAc->Draw("same E1");
grpipSys_Ac->Draw("E3 same");
grpimSys_Ac->Draw("E3 same");
vcan->cd(3);
vcan->cd(3)->SetGrid();
hpipAcc->Draw("same E1");
hpimAcc->Draw("same E1");
grpipSys_Acc->Draw("E3 same");
grpimSys_Acc->Draw("E3 same");

TLine *zeroLine = new TLine(hpipM->GetXaxis()->GetXmin(), 0.0, hpipM->GetXaxis()->GetXmax(), 0.0);
zeroLine->SetLineStyle(2);
zeroLine->SetLineWidth(3);

vcan->cd(2);
zeroLine->Draw();
vcan->cd(3);
zeroLine->Draw();

can->cd(2);
zeroLine->Draw();
can->cd(3);
zeroLine->Draw();

//can->SaveAs(Form("PLOT_%i_%i_%i.png", xBin, QQBin, zBin));

}
