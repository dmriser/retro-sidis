void plotBCC(int zBin = 4, int PT2Bin = 4)
{
string pipORpim = "pip";

TFile *tf = new TFile(Form("rootfiles/BCC_%s_%i_%i.root", pipORpim.c_str(), zBin, PT2Bin));

const int NxBins = 5;
const int NQQBins = 2;

TH1F *h[NxBins][NQQBins];

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {

h[ix][iQQ] = (TH1F*) tf->Get(Form("BCCvsphih_x%iQQ%i", ix, iQQ));
h[ix][iQQ]->GetYaxis()->SetRangeUser(0, 1.1*h[ix][iQQ]->GetMaximum());

}}

TCanvas *can = new TCanvas();
can->Divide(NxBins, NQQBins, 0.00001, 0.00001);

can->cd(1);
h[0][1]->Draw();
can->cd(2);
h[1][1]->Draw();
can->cd(3);
h[2][1]->Draw();
can->cd(4);
h[3][1]->Draw();
can->cd(5);
h[4][1]->Draw();

can->cd(6);
h[0][0]->Draw();
can->cd(7);
h[1][0]->Draw();
can->cd(8);
h[2][0]->Draw();
can->cd(9);
h[3][0]->Draw();
can->cd(10);
h[4][0]->Draw();

}
