void acc_5Dplots_forGIFs(int phihBBin = 0)
{
gStyle->SetOptStat(0);

TFile *tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n12114.BiSc5.MoCo11.90phihBBins.__0000000000000000__.root");

string pipORpim = "pim";

const int NxBins = 5;
const int NQQBins = 2;

TCanvas *can = new TCanvas("can", "can", 5, 5, 1200, 900);

TPad *pad[NxBins][NQQBins];
pad[0][0] = new TPad("pad00", "pad00", 0.17, 0.125, 0.31, 0.250); // these coordinates estimated by eye
pad[0][1] = new TPad("pad01", "pad01", 0.17, 0.250, 0.31, 0.375);
pad[1][0] = new TPad("pad10", "pad10", 0.31, 0.175, 0.45, 0.300);
pad[1][1] = new TPad("pad11", "pad11", 0.31, 0.300, 0.45, 0.450);
pad[2][0] = new TPad("pad20", "pad20", 0.45, 0.210, 0.59, 0.375);
pad[2][1] = new TPad("pad21", "pad21", 0.45, 0.375, 0.59, 0.550);
pad[3][0] = new TPad("pad30", "pad30", 0.59, 0.310, 0.73, 0.480);
pad[3][1] = new TPad("pad31", "pad31", 0.59, 0.480, 0.73, 0.650);
pad[4][0] = new TPad("pad40", "pad40", 0.73, 0.570, 0.87, 0.750);
//don't need this one pad[4][1] = new TPad("pad41", "pad41",...);

TH2F *hPT2vsz[NxBins][NQQBins];

for(int ix = 0; ix < NxBins; ix++)
{
for(int iQQ = 0; iQQ < NQQBins; iQQ++)
{
if(!(ix == 4 && iQQ == 1)) // since there's only 1 QQ bin at the largest x bin instead of 2 (for bin scheme 5)
{

hPT2vsz[ix][iQQ] = (TH2F*) tf->Get(Form("rec_%s_PT2vsz_x%iQQ%iphihB%i", pipORpim.c_str(), ix, iQQ, phihBBin));

can->cd();
pad[ix][iQQ]->Draw();
pad[ix][iQQ]->cd();
if(pipORpim == "pip") hPT2vsz[ix][iQQ]->SetTitle(Form("#pi+  p_{T}^{2} vs z (%i < #phi_{h} < %i deg)", phihBBin*2, (phihBBin+1)*2)); // assuming 2 deg bin width in phih
if(pipORpim == "pim") hPT2vsz[ix][iQQ]->SetTitle(Form("#pi-  p_{T}^{2} vs z (%i < #phi_{h} < %i deg)", phihBBin*2, (phihBBin+1)*2)); // assuming 2 deg bin width in phih
hPT2vsz[ix][iQQ]->GetXaxis()->SetTitle("z");
hPT2vsz[ix][iQQ]->GetXaxis()->SetTitleSize(0.04);
hPT2vsz[ix][iQQ]->GetXaxis()->SetLabelSize(0.04);
hPT2vsz[ix][iQQ]->GetYaxis()->SetTitle("p_{T}^{2} (GeV^{2})");
hPT2vsz[ix][iQQ]->GetYaxis()->SetTitleSize(0.04);
hPT2vsz[ix][iQQ]->GetYaxis()->SetLabelSize(0.04);
hPT2vsz[ix][iQQ]->Draw("colz");

}}}

can->cd();
TGaxis *xaxis = new TGaxis(0.17, 0.1, 0.88, 0.1, 0.1, 0.6, 6, "");
xaxis->SetTitle("x");
xaxis->SetLabelSize(0.03);
xaxis->Draw();

TGaxis *QQaxis = new TGaxis(0.13, 0.13, 0.13, 0.77, 1.0, 4.5, 8, "");
QQaxis->SetTitle("Q^{2} (GeV^{2})");
QQaxis->SetLabelSize(0.03);
QQaxis->Draw();

// draw a plot in the upper right which displays the phih range:
TH1F *hrange = new TH1F("hrange", "", 10, 0, 180);
hrange->GetXaxis()->SetTitle("| #phi_{h}| (deg)");
hrange->GetXaxis()->SetTitleSize(0.20);
hrange->GetXaxis()->SetTitleOffset(-0.55);
hrange->GetXaxis()->SetLabelSize(0.13);
hrange->GetYaxis()->SetRangeUser(0, 1);
TPad *rangePad = new TPad("rangePad", "rangePad", 0.47, 0.80, 1.00, 0.94);
can->cd();
rangePad->Draw();
rangePad->cd();
hrange->Draw();
TLine *l1 = new TLine(2.0*phihBBin, 0.0, 2.0*phihBBin, 1.0); // assuming 2 deg bin width in phih
l1->SetLineColor(kRed);
l1->SetLineWidth(2);
l1->Draw();
TLine *l2 = new TLine(2.0*(phihBBin+1), 0.0, 2.0*(phihBBin+1), 1.0); // assuming 2 deg bin width in phih
l2->SetLineColor(kRed);
l2->SetLineWidth(2);
l2->Draw();

// draw a zoomed in plot with an arrow pointing to it:
TPad *zoomPad = new TPad("zoomPad", "zoomPad", 0.18, 0.55, 0.50, 1.00);
can->cd();
zoomPad->Draw();
zoomPad->cd();
hPT2vsz[2][1]->Draw("colz");

TPad *arrowPad = new TPad("arrowPad", "arrowPad", 0.05, 0.05, 0.95, 0.95);
arrowPad->SetFillStyle(4000); // these lines make the pad transparent
arrowPad->SetFillColor(0); // this is so I can draw the arrow on this pad and it will show up in the foreground rather than behind the pads
arrowPad->SetFrameFillStyle(4000);
can->cd();
arrowPad->Draw();

TArrow *ar = new TArrow(0.5, 0.5, 0.37, 0.70, 0.03, "|>");
ar->SetLineWidth(5);
arrowPad->cd();
ar->Draw();

can->SaveAs(Form("%s_5DaccPlot_phihB%i.png", pipORpim.c_str(), phihBBin));
}
