
void total_systematics(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 5, string pipORpim = "pip")
//void total_systematics(int xBin = 1, int QQBin = 1, int zBin = 6, int PT2Bin = 7, string pipORpim = "pip")
{
gStyle->SetOptStat(0);

bool doSaveRoot = 0;

TFile *sys13 = new TFile("/home/naharrison/mysidis-histos/Systematics_v2.root"); // systematics from first 13 sources
TFile *sysSector = new TFile("/home/naharrison/mysidis-histos/Sector_systematics.root"); // systematics from sector dependence

TH1F *h13M = (TH1F*) sys13->Get(Form("hM_sysEcontributions_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *h13Ac = (TH1F*) sys13->Get(Form("hAc_sysEcontributions_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *h13Acc = (TH1F*) sys13->Get(Form("hAcc_sysEcontributions_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

TH1F *hSectorM = (TH1F*) sysSector->Get(Form("hM_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *hSectorAc = (TH1F*) sysSector->Get(Form("hAc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
TH1F *hSectorAcc = (TH1F*) sysSector->Get(Form("hAcc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

// ### calculate the sector dependence systematic error contribution ###
// ### from the 6 measurements (one for each sector) ###
// ### Delta^2 = delta_stat^2 + delta_sys^2 ###
float MAverage, AcAverage, AccAverage;
MAverage = AcAverage = AccAverage = 0.0;
for(int s = 0; s < 6; s++) {
	MAverage += hSectorM->GetBinContent(s+1);
	AcAverage += hSectorAc->GetBinContent(s+1);
	AccAverage += hSectorAcc->GetBinContent(s+1);
}
MAverage = MAverage/6.0;
AcAverage = AcAverage/6.0;
AccAverage = AccAverage/6.0;

float MDelta, AcDelta, AccDelta;
MDelta = AcDelta = AccDelta = 0.0;
for(int s = 0; s < 6; s++) {
	MDelta += (hSectorM->GetBinContent(s+1) - MAverage)*(hSectorM->GetBinContent(s+1) - MAverage);
	AcDelta += (hSectorAc->GetBinContent(s+1) - AcAverage)*(hSectorAc->GetBinContent(s+1) - AcAverage);
	AccDelta += (hSectorAcc->GetBinContent(s+1) - AccAverage)*(hSectorAcc->GetBinContent(s+1) - AccAverage);
}
MDelta = sqrt(MDelta)/sqrt(6.0);
AcDelta = sqrt(AcDelta)/sqrt(6.0);
AccDelta = sqrt(AccDelta)/sqrt(6.0);

float Mdelta_stat, Acdelta_stat, Accdelta_stat;
Mdelta_stat = Acdelta_stat = Accdelta_stat = 0.0;
for(int s = 0; s < 6; s++) {
	Mdelta_stat += hSectorM->GetBinError(s+1)*hSectorM->GetBinError(s+1);
	Acdelta_stat += hSectorAc->GetBinError(s+1)*hSectorAc->GetBinError(s+1);
	Accdelta_stat += hSectorAcc->GetBinError(s+1)*hSectorAcc->GetBinError(s+1);
}
Mdelta_stat = sqrt(Mdelta_stat)/sqrt(6.0);
Acdelta_stat = sqrt(Acdelta_stat)/sqrt(6.0);
Accdelta_stat = sqrt(Accdelta_stat)/sqrt(6.0);

float Mdelta_sys, Acdelta_sys, Accdelta_sys;
if(MDelta > Mdelta_stat) Mdelta_sys = sqrt(MDelta*MDelta - Mdelta_stat*Mdelta_stat);
else if(pipORpim == "pip") Mdelta_sys = 115.0; // use average of all bins instead of 0
else if(pipORpim == "pim") Mdelta_sys = 140.0; // use average of all bins instead of 0
else Mdelta_sys = 0.0;
if(AcDelta > Acdelta_stat) Acdelta_sys = sqrt(AcDelta*AcDelta - Acdelta_stat*Acdelta_stat);
else if(pipORpim == "pip") Acdelta_sys = 0.037; // use average of all bins instead of 0
else if(pipORpim == "pim") Acdelta_sys = 0.037; // use average of all bins instead of 0
else Acdelta_sys = 0.0;
if(AccDelta > Accdelta_stat) Accdelta_sys = sqrt(AccDelta*AccDelta - Accdelta_stat*Accdelta_stat);
else if(pipORpim == "pip") Accdelta_sys = 0.028; // use average of all bins instead of 0
else if(pipORpim == "pim") Accdelta_sys = 0.030; // use average of all bins instead of 0
else Accdelta_sys = 0.0;

// ### create new histos w/ systematics w/ 14 instead of 13 bins ###
TH1F *h14M = new TH1F("hSysM", "A_{0} Systematic Errors", 14, 0, 14);
h14M->GetXaxis()->SetTitle("source");
TH1F *h14Ac = new TH1F("hSysAc", "A_{c} Systematic Errors", 14, 0, 14);
h14Ac->GetXaxis()->SetTitle("source");
TH1F *h14Acc = new TH1F("hSysAcc", "A_{cc} Systematic Errors", 14, 0, 14);
h14Acc->GetXaxis()->SetTitle("source");
for(int b = 1; b <= 13; b++) {
	h14M->SetBinContent(b, h13M->GetBinContent(b));
	h14Ac->SetBinContent(b, h13Ac->GetBinContent(b));
	h14Acc->SetBinContent(b, h13Acc->GetBinContent(b));
}
h14M->SetBinContent(14, Mdelta_sys);
h14Ac->SetBinContent(14, Acdelta_sys);
h14Acc->SetBinContent(14, Accdelta_sys);

// ### add all 14 contributions in quadrature ###
float MSysErr, AcSysErr, AccSysErr;
MSysErr = AcSysErr = AccSysErr = 0.0;
for(int k = 0; k < 14; k++) {
	MSysErr += h14M->GetBinContent(k+1)*h14M->GetBinContent(k+1);
	AcSysErr += h14Ac->GetBinContent(k+1)*h14Ac->GetBinContent(k+1);
	AccSysErr += h14Acc->GetBinContent(k+1)*h14Acc->GetBinContent(k+1);
}
MSysErr = sqrt(MSysErr);
AcSysErr = sqrt(AcSysErr);
AccSysErr = sqrt(AccSysErr);
cout<<MSysErr<<"   "<<AcSysErr<<"   "<<AccSysErr<<endl;

// ### draw ###
TCanvas *can = new TCanvas("can", "can", 5, 5, 1200, 1000);
can->Divide(3, 3);
can->cd(1);
h13M->Draw();
can->cd(2);
hSectorM->Draw();
can->cd(3);
h14M->Draw();
can->cd(4);
h13Ac->Draw();
can->cd(5);
hSectorAc->Draw();
can->cd(6);
h14Ac->Draw();
can->cd(7);
h13Acc->Draw();
can->cd(8);
hSectorAcc->Draw();
can->cd(9);
h14Acc->Draw();

TFile *rootFile;
if(doSaveRoot)
{
   rootFile = new TFile("Total_systematics.root", "update");
	TH1F *hist = new TH1F(Form("hMAcAccSys_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), Form("hMAcAccSys_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 3, 0, 3);
	hist->SetBinContent(1, MSysErr);
	hist->SetBinContent(2, AcSysErr);
	hist->SetBinContent(3, AccSysErr);
	rootFile->Write();
}

TCanvas *hcan = new TCanvas("hcan", "hcan", 50, 50, 1200, 400);
hcan->Divide(3, 1, 0.0001, 0.0001);
hcan->cd(1);
h14M->Draw();
hcan->cd(2);
h14Ac->Draw();
hcan->cd(3);
h14Acc->Draw();

}
