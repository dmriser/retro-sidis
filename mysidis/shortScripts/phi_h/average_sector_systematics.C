#include <algorithm> // for std::find

void average_sector_systematics(string pipORpim = "pim")
{
gStyle->SetOptStat(0);
TFile *sysSector = new TFile("/home/naharrison/mysidis-histos/Sector_systematics.root");

ifstream goodBinFile("finalKeptBins.txt");
vector<string> goodBins;
for(int k = 0; k < 3618; k++) {
	string bin;
	goodBinFile>>bin;
	goodBins.push_back(bin);
}
goodBinFile.close();

vector<float> MSys;
vector<float> AcSys;
vector<float> AccSys;
TH1F *hMSys = new TH1F("hMSys", "hMSys", 50, 0, 8000);
TH1F *hAcSys = new TH1F("hAcSys", "hAcSys", 50, 0, 0.2);
TH1F *hAccSys = new TH1F("hAccSys", "hAccSys", 50, 0, 0.2);

cout<<endl<<pipORpim<<endl<<endl;

for(int xBin = 0; xBin < 5; xBin++) {
for(int QQBin = 0; QQBin < 2; QQBin++) {
for(int zBin = 0; zBin < 18; zBin++) {
for(int PT2Bin = 0; PT2Bin < 20; PT2Bin++) {
if(!(xBin == 4 && QQBin == 1)) {

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
	else Mdelta_sys = 0.0;
	if(AcDelta > Acdelta_stat) Acdelta_sys = sqrt(AcDelta*AcDelta - Acdelta_stat*Acdelta_stat);
	else Acdelta_sys = 0.0;
	if(AccDelta > Accdelta_stat) Accdelta_sys = sqrt(AccDelta*AccDelta - Accdelta_stat*Accdelta_stat);
	else Accdelta_sys = 0.0;
	
	// Get average sector systematic error
	// Only use good bins in calculation
	string MBinName = Form("%s_M_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);
	string AcAccBinName = Form("%s_AcAcc_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin);

	if(std::find(goodBins.begin(), goodBins.end(), MBinName) != goodBins.end()) {
		MSys.push_back(Mdelta_sys);
		hMSys->Fill(Mdelta_sys);
	}

	if(std::find(goodBins.begin(), goodBins.end(), AcAccBinName) != goodBins.end()) {
		AcSys.push_back(Acdelta_sys);
		AccSys.push_back(Accdelta_sys);
		hAcSys->Fill(Acdelta_sys);
		hAccSys->Fill(Accdelta_sys);
	}

}}}}}

// calculate average
float MSysSum = 0.0;
for(int k = 0; k < MSys.size(); k++) {
	MSysSum += MSys[k];
}
float MSysAve = MSysSum/MSys.size();

float AcSysSum = 0.0;
float AccSysSum = 0.0;
for(int k = 0; k < AcSys.size(); k++) {
	AcSysSum += AcSys[k];
	AccSysSum += AccSys[k];
}
float AcSysAve = AcSysSum/AcSys.size();
float AccSysAve = AccSysSum/AccSys.size();

// calculate stdev
float MDeviationSum = 0.0;
for(int k = 0; k < MSys.size(); k++) {
	MDeviationSum += pow(MSys[k] - MSysAve, 2);
}
float MStdev = sqrt((1.0/((float) MSys.size()))*MDeviationSum);

float AcDeviationSum = 0.0;
float AccDeviationSum = 0.0;
for(int k = 0; k < AcSys.size(); k++) {
	AcDeviationSum += pow(AcSys[k] - AcSysAve, 2);
	AccDeviationSum += pow(AccSys[k] - AccSysAve, 2);
}
float AcStdev = sqrt((1.0/((float) AcSys.size()))*AcDeviationSum);
float AccStdev = sqrt((1.0/((float) AccSys.size()))*AccDeviationSum);


// print & draw
cout<<MSysAve<<" "<<MStdev<<endl;
cout<<AcSysAve<<" "<<AcStdev<<endl;
cout<<AccSysAve<<" "<<AccStdev<<endl;
TCanvas *can = new TCanvas();
can->Divide(3, 1);
can->cd(1);
hMSys->Draw();
can->cd(2);
hAcSys->Draw();
can->cd(3);
hAccSys->Draw();

}
