void quick(string pipORpim = "pip", int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 3, int phihBin = 2)
{
TFile *tf = new TFile("MonteCarlo_v12.it0.s1.n11625.BiSc7.__0000000000000000__.root");

TH1F *xres = (TH1F*) tf->Get(Form("%s_x_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));
TH1F *QQres = (TH1F*) tf->Get(Form("%s_QQ_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));
TH1F *zres = (TH1F*) tf->Get(Form("%s_z_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));
TH1F *PT2res = (TH1F*) tf->Get(Form("%s_PT2_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));
TH1F *phihres = (TH1F*) tf->Get(Form("%s_phih_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));

TCanvas *can = new TCanvas("can", "can", 5, 5, 1200, 800);
can->Divide(3, 2, 0.00001, 0.00001);

can->cd(1);
xres->GetXaxis()->SetTitle("#Delta x");
xres->Draw();
can->cd(2);
QQres->GetXaxis()->SetTitle("#Delta Q^{2} (GeV^{2})");
QQres->Draw();
can->cd(3);
zres->GetXaxis()->SetTitle("#Delta z");
zres->Draw();
can->cd(4);
PT2res->GetXaxis()->SetTitle("#Delta P_{h#perp}^{2} (GeV^{2})");
PT2res->Draw();
can->cd(5);
phihres->GetXaxis()->SetTitle("#Delta #phi_{h} (deg.)");
phihres->Draw();

can->SaveAs(Form("%s_res_x%i_QQ%i_z%i_PT2%i_phih%i.png", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin, phihBin));

}
