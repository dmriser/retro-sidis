void Osipenko_zPT2bins_phihBBins(int xBin = 8, int QQBin = 4, int phihBBin = 4)
{
gStyle->SetOptStat(0);

bool doSave = 0;

TCanvas *can = new TCanvas();

TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc6.MoCo11.90phihBBins.__0000000000000000__.root");
TH2F *hist = (TH2F*) tf->Get(Form("rec_pip_PT2vsz_x%iQQ%iphihB%i", xBin, QQBin, phihBBin));
hist->GetXaxis()->SetTitle("z");
hist->GetYaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
hist->SetTitle(Form("x%i QQ%i, %3.1f#circ < |#phi_{h}| < %3.1f#circ", xBin, QQBin, phihBBin*2.0, (phihBBin+1)*2.0)); // 2.0 is the bin width
hist->Draw("colz");

TLine *vline0 = new TLine(0.059, 0, 0.059, 1.1);
vline0->Draw();
TLine *vline1 = new TLine(0.081, 0, 0.081, 1.1);
vline1->Draw();
TLine *vline2 = new TLine(0.1065, 0, 0.1065, 1.1);
vline2->Draw();
TLine *vline3 = new TLine(0.133, 0, 0.133, 1.1);
vline3->Draw();
TLine *vline4 = new TLine(0.161, 0, 0.161, 1.1);
vline4->Draw();
TLine *vline5 = new TLine(0.1905, 0, 0.1905, 1.1);
vline5->Draw();
TLine *vline6 = new TLine(0.221, 0, 0.221, 1.1);
vline6->Draw();
TLine *vline7 = new TLine(0.2525, 0, 0.2525, 1.1);
vline7->Draw();
TLine *vline8 = new TLine(0.2855, 0, 0.2855, 1.1);
vline8->Draw();
TLine *vline9 = new TLine(0.32, 0, 0.32, 1.1);
vline9->Draw();
TLine *vline10 = new TLine(0.356, 0, 0.356, 1.1);
vline10->Draw();
TLine *vline11 = new TLine(0.394, 0, 0.394, 1.1);
vline11->Draw();
TLine *vline12 = new TLine(0.4335, 0, 0.4335, 1.1);
vline12->Draw();
TLine *vline13 = new TLine(0.4745, 0, 0.4745, 1.1);
vline13->Draw();
TLine *vline14 = new TLine(0.5175, 0, 0.5175, 1.1);
vline14->Draw();
TLine *vline15 = new TLine(0.5625, 0, 0.5625, 1.1);
vline15->Draw();
TLine *vline16 = new TLine(0.6095, 0, 0.6095, 1.1);
vline16->Draw();
TLine *vline17 = new TLine(0.6585, 0, 0.6585, 1.1);
vline17->Draw();
TLine *vline18 = new TLine(0.7095, 0, 0.7095, 1.1);
vline18->Draw();
TLine *vline19 = new TLine(0.763, 0, 0.763, 1.1);
vline19->Draw();
TLine *vline20 = new TLine(0.8185, 0, 0.8185, 1.1);
vline20->Draw();
TLine *vline21 = new TLine(0.8765, 0, 0.8765, 1.1);
vline21->Draw();
TLine *vline22 = new TLine(0.93, 0, 0.93, 1.1);
vline22->Draw();

TLine *hline0 = new TLine(0, 0.0, 1.05, 0.0);
hline0->Draw();
TLine *hline1 = new TLine(0, 0.0153, 1.05, 0.0153);
hline1->Draw();
TLine *hline2 = new TLine(0, 0.0459, 1.05, 0.0459);
hline2->Draw();
TLine *hline3 = new TLine(0, 0.0972, 1.05, 0.0972);
hline3->Draw();
TLine *hline4 = new TLine(0, 0.1728, 1.05, 0.1728);
hline4->Draw();
TLine *hline5 = new TLine(0, 0.279, 1.05, 0.279);
hline5->Draw();
TLine *hline6 = new TLine(0, 0.4239, 1.05, 0.4239);
hline6->Draw();
TLine *hline7 = new TLine(0, 0.6246, 1.05, 0.6246);
hline7->Draw();
TLine *hline8 = new TLine(0, 0.9081, 1.05, 0.9081);
hline8->Draw();
TLine *hline9 = new TLine(0, 1.314, 1.05, 1.314);
hline9->Draw();
TLine *hline10 = new TLine(0, 1.75, 1.05, 1.75);
hline10->Draw();

if(doSave) can->SaveAs(Form("PT2Vz_x%iQQ%iphihB%i.png", xBin, QQBin, phihBBin));


}
