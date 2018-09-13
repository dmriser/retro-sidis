void showSectorDependence_oneBin(int xBin = 1, int QQBin = 1, int zBin = 6, int PT2Bin = 8)
{

TFile *sysFile = new TFile("/home/naharrison/mysidis-histos/Sector_systematics.root", "READ");

// pi+
TH1F *hA0pip = (TH1F*) sysFile->Get(Form("hM_pip_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hA0pip->SetMarkerStyle(4);
hA0pip->SetLineColor(kRed);
hA0pip->SetLineWidth(2);
hA0pip->GetXaxis()->SetTitle("e- sector");

TH1F *hAcpip = (TH1F*) sysFile->Get(Form("hAc_pip_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hAcpip->SetMarkerStyle(4);
hAcpip->SetLineColor(kRed);
hAcpip->SetLineWidth(2);
hAcpip->GetXaxis()->SetTitle("e- sector");

TH1F *hAccpip = (TH1F*) sysFile->Get(Form("hAcc_pip_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hAccpip->SetMarkerStyle(4);
hAccpip->SetLineColor(kRed);
hAccpip->SetLineWidth(2);
hAccpip->GetXaxis()->SetTitle("e- sector");

// pi-
TH1F *hA0pim = (TH1F*) sysFile->Get(Form("hM_pim_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hA0pim->SetMarkerStyle(23);
hA0pim->SetLineColor(kBlue);
hA0pim->SetLineWidth(2);

TH1F *hAcpim = (TH1F*) sysFile->Get(Form("hAc_pim_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hAcpim->SetMarkerStyle(23);
hAcpim->SetLineColor(kBlue);
hAcpim->SetLineWidth(2);

TH1F *hAccpim = (TH1F*) sysFile->Get(Form("hAcc_pim_%i_%i_%i_%i", xBin, QQBin, zBin, PT2Bin));
hAccpim->SetMarkerStyle(23);
hAccpim->SetLineColor(kBlue);
hAccpim->SetLineWidth(2);

// draw
TCanvas *can = new TCanvas("can", "can", 20, 20, 1500, 500);
can->Divide(3, 1, 0.00001, 0.00001);

can->cd(1);
hA0pip->Draw();
hA0pim->Draw("same");

can->cd(2);
hAcpip->Draw();
hAcpim->Draw("same");

can->cd(3);
hAccpip->Draw();
hAccpim->Draw("same");

}
