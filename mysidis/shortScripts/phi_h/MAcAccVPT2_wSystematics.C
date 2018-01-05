// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void MAcAccVPT2_wSystematics(int xBin = 0, int QQBin = 0, int zBin = 6)
{
gStyle->SetOptStat(0);

string YscaleOpt = "fixed"; // "fixed" or "smart"

bool Mlog = 0;

int iMarkerStyle = 4;

const int NPT2Bins = 20;
float PT2Min = 0;
float PT2Max = 1;

//TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 600);
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

/// pip: ///
for(int k = 0; k < NPT2Bins; k++)
{
ifstream pipMfile(Form("/scratch/MAcAccfiles/pip_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
ifstream pipAcAccfile(Form("/scratch/MAcAccfiles/pip_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));

float pipM, pipMe, pipAc, pipAce, pipAcc, pipAcce;

if(pipMfile)
{
pipMfile>>pipM>>pipMe;

hpipM->SetBinContent(k+1, pipM);
hpipM->SetBinError(k+1, pipMe);
}

if(pipAcAccfile)
{
pipAcAccfile>>pipAc>>pipAce>>pipAcc>>pipAcce;

hpipAc->SetBinContent(k+1, pipAc);
hpipAc->SetBinError(k+1, pipAce);
hpipAcc->SetBinContent(k+1, pipAcc);
hpipAcc->SetBinError(k+1, pipAcce);
}

pipMfile.close();
pipAcAccfile.close();
}

/// pim: ///
for(int k = 0; k < NPT2Bins; k++)
{
ifstream pimMfile(Form("/scratch/MAcAccfiles/pim_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
ifstream pimAcAccfile(Form("/scratch/MAcAccfiles/pim_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));

float pimM, pimMe, pimAc, pimAce, pimAcc, pimAcce;

if(pimMfile)
{
pimMfile>>pimM>>pimMe;

hpimM->SetBinContent(k+1, pimM);
hpimM->SetBinError(k+1, pimMe);
}

if(pimAcAccfile)
{
pimAcAccfile>>pimAc>>pimAce>>pimAcc>>pimAcce;

hpimAc->SetBinContent(k+1, pimAc);
hpimAc->SetBinError(k+1, pimAce);
hpimAcc->SetBinContent(k+1, pimAcc);
hpimAcc->SetBinError(k+1, pimAcce);
}

pimMfile.close();
pimAcAccfile.close();
}

/// Draw: ///
can->cd(1)->SetGrid();
if(Mlog) can->cd(1)->SetLogy();
hpipM->SetLineColor(kRed);
hpipM->SetMarkerStyle(iMarkerStyle);
if(YscaleOpt == "smart" && Mlog) hpipM->GetYaxis()->SetRangeUser(1, 2.0*max(hpipM->GetMaximum(), hpimM->GetMaximum()));
if(YscaleOpt == "smart" && !Mlog) hpipM->GetYaxis()->SetRangeUser(-0.2*max(hpipM->GetMaximum(), hpimM->GetMaximum()), 1.1*max(hpipM->GetMaximum(), hpimM->GetMaximum()));
if(YscaleOpt == "fixed") hpipM->GetYaxis()->SetRangeUser(-2000, 12500);
hpipM->SetTitle(Form("Multiplicity for #pi+ (red) and #pi- (blue), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipM->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipM->Draw("same E1");
hpimM->SetLineColor(kBlue);
hpimM->SetMarkerStyle(iMarkerStyle);
hpimM->Draw("same E1");

can->cd(2)->SetGrid();
hpipAc->SetLineColor(kRed);
hpipAc->SetMarkerStyle(iMarkerStyle);
if(YscaleOpt == "smart") hpipAc->GetYaxis()->SetRangeUser(min(hpipAc->GetMinimum(), hpimAc->GetMinimum()) - 0.20, max(hpipAc->GetMaximum(), hpimAc->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAc->GetYaxis()->SetRangeUser(-0.42, 0.3);
hpipAc->SetTitle(Form("A^{cos#phi}_{UU} for #pi+ (red) and #pi- (blue), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipAc->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipAc->Draw("same E1");
hpimAc->SetLineColor(kBlue);
hpimAc->SetMarkerStyle(iMarkerStyle);
hpimAc->Draw("same E1");

can->cd(3)->SetGrid();
hpipAcc->SetLineColor(kRed);
hpipAcc->SetMarkerStyle(iMarkerStyle);
if(YscaleOpt == "smart") hpipAcc->GetYaxis()->SetRangeUser(min(hpipAcc->GetMinimum(), hpimAcc->GetMinimum()) - 0.20, max(hpipAcc->GetMaximum(), hpimAcc->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAcc->GetYaxis()->SetRangeUser(-0.4, 0.42);
hpipAcc->SetTitle(Form("A^{cos2#phi}_{UU} for #pi+ (red) and #pi- (blue), x%i QQ%i z%i", xBin, QQBin, zBin));
hpipAcc->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hpipAcc->Draw("same E1");
hpimAcc->SetLineColor(kBlue);
hpimAcc->SetMarkerStyle(iMarkerStyle);
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
float Msys;

ifstream file1(Form("systematicErrors/pip_M_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
if(file1)
{
file1>>Msys;
grpipSys_M->SetPoint(grpipSys_M->GetN(), hpipM->GetBinCenter(k+1), MsysYval);
grpipSys_M->SetPointError(grpipSys_M->GetN()-1, 0, 0.5*Msys); // factor of 0.5 since error bar goes up and down... want the full height to = Msys
}
file1.close();

ifstream file2(Form("systematicErrors/pim_M_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
if(file2)
{
file2>>Msys;
grpimSys_M->SetPoint(grpimSys_M->GetN(), hpimM->GetBinCenter(k+1), MsysYval);
grpimSys_M->SetPointError(grpimSys_M->GetN()-1, 0, 0.5*Msys);
}
file2.close();


float Acsys, Accsys;

ifstream file3(Form("systematicErrors/pip_AcAcc_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
if(file3)
{
file3>>Acsys>>Accsys;
grpipSys_Ac->SetPoint(grpipSys_Ac->GetN(), hpipAc->GetBinCenter(k+1), AcsysYval);
grpipSys_Ac->SetPointError(grpipSys_Ac->GetN()-1, 0, 0.5*Acsys);
grpipSys_Acc->SetPoint(grpipSys_Acc->GetN(), hpipAcc->GetBinCenter(k+1), AccsysYval);
grpipSys_Acc->SetPointError(grpipSys_Acc->GetN()-1, 0, 0.5*Accsys);
}
file3.close();

ifstream file4(Form("systematicErrors/pim_AcAcc_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
if(file4)
{
file4>>Acsys>>Accsys;
grpimSys_Ac->SetPoint(grpimSys_Ac->GetN(), hpimAc->GetBinCenter(k+1), AcsysYval);
grpimSys_Ac->SetPointError(grpimSys_Ac->GetN()-1, 0, 0.5*Acsys);
grpimSys_Acc->SetPoint(grpimSys_Acc->GetN(), hpimAcc->GetBinCenter(k+1), AccsysYval);
grpimSys_Acc->SetPointError(grpimSys_Acc->GetN()-1, 0, 0.5*Accsys);
}
file4.close();

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
hpipM->Draw("same E1");
hpimM->Draw("same E1");
grpipSys_M->Draw("E3 same");
grpimSys_M->Draw("E3 same");
vcan->cd(2);
hpipAc->Draw("same E1");
hpimAc->Draw("same E1");
grpipSys_Ac->Draw("E3 same");
grpimSys_Ac->Draw("E3 same");
vcan->cd(3);
hpipAcc->Draw("same E1");
hpimAcc->Draw("same E1");
grpipSys_Acc->Draw("E3 same");
grpimSys_Acc->Draw("E3 same");


}
