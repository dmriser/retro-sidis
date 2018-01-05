// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void MAcAccVx(int QQBin = 0, int zBin = 5, int PT2Bin = 5, int accItN_temp = 2, int hapItN_temp = 2)
{
gStyle->SetOptStat(0);

bool doSave = 0;
string YscaleOpt = "smart"; // "fixed" or "smart"

//const int totalNits = 5; // number of acceptance iterations plus number of RC iterations... 4 (0, 1, 2, 3) + 1 (default haprad) = 5 for now
//string RCoptString[totalNits] = {"", "", "", "", "wRC"};
//int accItN[totalNits] = {0, 1, 2, 3, 3};
//int iMarkerStyle[totalNits] = {4, 20, 5, 22, 21};

const int totalNits = 1; // number of acceptance iterations plus number of RC iterations... 4 (0, 1, 2, 3) + 1 (default haprad) = 5 for now
int accItN[totalNits] = {accItN_temp}; // kind of a kludge
int hapItN[totalNits] = {hapItN_temp};
int iMarkerStyle[totalNits] = {4};

const int NpipxBins = 5;
float pipxMin = 0.1;
float pipxMax = 0.6;
const int NpimxBins = 5;
float pimxMin = 0.1;
float pimxMax = 0.6;

TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 600);
can->Divide(3, 1, 0.00001, 0.00001);

TH1F *hpipM[totalNits], *hpipAc[totalNits], *hpipAcc[totalNits];
TH1F *hpimM[totalNits], *hpimAc[totalNits], *hpimAcc[totalNits];

for(int i = 0; i < totalNits; i++)
{
hpipM[i] = new TH1F(Form("hpipM_it%i_%i", accItN[i], hapItN[i]), Form("hpipM_it%i_%i", accItN[i], hapItN[i]), NpipxBins, pipxMin, pipxMax);
hpipAc[i] = new TH1F(Form("hpipAc_it%i_%i", accItN[i], hapItN[i]), Form("hpipAc_it%i_%i", accItN[i], hapItN[i]), NpipxBins, pipxMin, pipxMax);
hpipAcc[i] = new TH1F(Form("hpipAcc_it%i_%i", accItN[i], hapItN[i]), Form("hpipAcc_it%i_%i", accItN[i], hapItN[i]), NpipxBins, pipxMin, pipxMax);
hpimM[i] = new TH1F(Form("hpimM_it%i_%i", accItN[i], hapItN[i]), Form("hpimM_it%i_%i", accItN[i], hapItN[i]), NpimxBins, pimxMin, pimxMax);
hpimAc[i] = new TH1F(Form("hpimAc_it%i_%i", accItN[i], hapItN[i]), Form("hpimAc_it%i_%i", accItN[i], hapItN[i]), NpimxBins, pimxMin, pimxMax);
hpimAcc[i] = new TH1F(Form("hpimAcc_it%i_%i", accItN[i], hapItN[i]), Form("hpimAcc_it%i_%i", accItN[i], hapItN[i]), NpimxBins, pimxMin, pimxMax);

/// pip: ///
for(int k = 0; k < NpipxBins; k++)
{
ifstream pipMfile(Form("/scratch/MAcAccfiles/pip_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", accItN[i], hapItN[i], k, QQBin, zBin, PT2Bin));
ifstream pipAcAccfile(Form("/scratch/MAcAccfiles/pip_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", accItN[i], hapItN[i], k, QQBin, zBin, PT2Bin));

float pipM, pipMe, pipAc, pipAce, pipAcc, pipAcce;

if(pipMfile)
{
pipMfile>>pipM>>pipMe;

hpipM[i]->SetBinContent(k+1, pipM);
hpipM[i]->SetBinError(k+1, pipMe);
}

if(pipAcAccfile)
{
pipAcAccfile>>pipAc>>pipAce>>pipAcc>>pipAcce;

hpipAc[i]->SetBinContent(k+1, pipAc);
hpipAc[i]->SetBinError(k+1, pipAce);
hpipAcc[i]->SetBinContent(k+1, pipAcc);
hpipAcc[i]->SetBinError(k+1, pipAcce);
}

pipMfile.close();
pipAcAccfile.close();
}

/// pim: ///
for(int k = 0; k < NpimxBins; k++)
{
ifstream pimMfile(Form("/scratch/MAcAccfiles/pim_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", accItN[i], hapItN[i], k, QQBin, zBin, PT2Bin));
ifstream pimAcAccfile(Form("/scratch/MAcAccfiles/pim_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", accItN[i], hapItN[i], k, QQBin, zBin, PT2Bin));

float pimM, pimMe, pimAc, pimAce, pimAcc, pimAcce;

if(pimMfile)
{
pimMfile>>pimM>>pimMe;

hpimM[i]->SetBinContent(k+1, pimM);
hpimM[i]->SetBinError(k+1, pimMe);
}

if(pimAcAccfile)
{
pimAcAccfile>>pimAc>>pimAce>>pimAcc>>pimAcce;

hpimAc[i]->SetBinContent(k+1, pimAc);
hpimAc[i]->SetBinError(k+1, pimAce);
hpimAcc[i]->SetBinContent(k+1, pimAcc);
hpimAcc[i]->SetBinError(k+1, pimAcce);
}

pimMfile.close();
pimAcAccfile.close();
}

/// Draw: ///
can->cd(1)->SetGrid();
can->cd(1)->SetLogy();
hpipM[i]->SetLineColor(kRed);
hpipM[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hpipM[i]->GetYaxis()->SetRangeUser(10, 2.0*max(hpipM[i]->GetMaximum(), hpimM[i]->GetMaximum()));
if(YscaleOpt == "fixed") hpipM[i]->GetYaxis()->SetRangeUser(10, 30000);
hpipM[i]->SetTitle(Form("Multiplicity for #pi+ (red) and #pi- (blue), QQ%iz%iPT2%i", QQBin, zBin, PT2Bin));
hpipM[i]->GetXaxis()->SetTitle("x");
hpipM[i]->Draw("same E1");
hpimM[i]->SetLineColor(kBlue);
hpimM[i]->SetMarkerStyle(iMarkerStyle[i]);
hpimM[i]->Draw("same E1");

can->cd(2)->SetGrid();
hpipAc[i]->SetLineColor(kRed);
hpipAc[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hpipAc[i]->GetYaxis()->SetRangeUser(min(hpipAc[i]->GetMinimum(), hpimAc[i]->GetMinimum()) - 0.1, max(hpipAc[i]->GetMaximum(), hpimAc[i]->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAc[i]->GetYaxis()->SetRangeUser(-0.35, 0.25);
hpipAc[i]->SetTitle(Form("A^{cos#phi} for #pi+ (red) and #pi- (blue), QQ%iz%iPT2%i", QQBin, zBin, PT2Bin));
hpipAc[i]->GetXaxis()->SetTitle("x");
hpipAc[i]->Draw("same E1");
hpimAc[i]->SetLineColor(kBlue);
hpimAc[i]->SetMarkerStyle(iMarkerStyle[i]);
hpimAc[i]->Draw("same E1");

can->cd(3)->SetGrid();
hpipAcc[i]->SetLineColor(kRed);
hpipAcc[i]->SetMarkerStyle(iMarkerStyle[i]);
if(YscaleOpt == "smart") hpipAcc[i]->GetYaxis()->SetRangeUser(min(hpipAcc[i]->GetMinimum(), hpimAcc[i]->GetMinimum()) - 0.1, max(hpipAcc[i]->GetMaximum(), hpimAcc[i]->GetMaximum()) + 0.1);
if(YscaleOpt == "fixed") hpipAcc[i]->GetYaxis()->SetRangeUser(-0.15, 0.35);
hpipAcc[i]->SetTitle(Form("A^{cos2#phi} for #pi+ (red) and #pi- (blue), QQ%iz%iPT2%i", QQBin, zBin, PT2Bin));
hpipAcc[i]->GetXaxis()->SetTitle("x");
hpipAcc[i]->Draw("same E1");
hpimAcc[i]->SetLineColor(kBlue);
hpimAcc[i]->SetMarkerStyle(iMarkerStyle[i]);
hpimAcc[i]->Draw("same E1");

}

//can->cd(1);
//TLegend *leg = new TLegend(0.55, 0.65, 0.9, 0.9);
//leg->AddEntry(hpipM[0], "acc. it. 0, no RC", "p");
//leg->AddEntry(hpipM[1], "acc. it. 1", "p");
//leg->AddEntry(hpipM[2], "acc. it. 2", "p");
//leg->AddEntry(hpipM[3], "acc. it. 3", "p");
//leg->AddEntry(hpipM[4], "acc. it. 3 w/ RC", "p");
//leg->Draw();

if(doSave) can->SaveAs(Form("MAcAccVx_it%i_hap%i_BiSc5_QQ%iz%iPT2%i.png", accItN_temp, hapItN_temp, QQBin, zBin, PT2Bin));

}
