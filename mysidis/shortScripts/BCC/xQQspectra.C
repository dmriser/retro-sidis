{
gStyle->SetOptStat(0);
TFile *tf = new TFile("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it2.s1.n32255.BiSc5.__0000000000000000__.root");

string genORrec = "gen"; // don't change this! BCC uses only generated MC!
string pipORpim = "pip";

const int NxBins = 5;
float xLimits[NxBins + 1] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
const int NQQBins = 2;
float QQLimits[NxBins][NQQBins + 1] = {{1.0, 1.3, 5.0}, {1.0, 1.7, 5.0}, {1.0, 2.2, 5.0}, {1.0, 2.9, 5.0}, {1.0, 5.0, 99.9}};

float yMax = 0.85;
float WMin = 2.05;
float QQMin = 1.0;
TF1 *yFcn = new TF1("yFcn", Form("2.0*5.498*%3.4f*0.938*x", yMax), 0, 1); 
TF1 *WFcn = new TF1("WFcn", Form("(pow(%3.4f, 2.0) - pow(0.938, 2.0))/((1.0/x) - 1.0)", WMin), 0, 1); 
TF1 *QQFcn = new TF1("QQFcn", Form("0.0*x + %3.4f", QQMin), 0, 1);
yFcn->SetLineWidth(2);
WFcn->SetLineWidth(2);
QQFcn->SetLineWidth(2);
yFcn->SetLineColor(kBlack);
WFcn->SetLineColor(kBlack);
QQFcn->SetLineColor(kBlack);

TCanvas *can1 = new TCanvas("can1", "can1", 10, 10, 1000, 700);

can1->cd(1)->SetLogz();
TH2F *QQvsx = (TH2F*) tf->Get(Form("%s_%s_QQvsx", genORrec.c_str(), pipORpim.c_str()));
QQvsx->GetXaxis()->SetTitle("x");
QQvsx->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
if(pipORpim == "pip") QQvsx->SetTitle("#pi+ channel");
if(pipORpim == "pim") QQvsx->SetTitle("#pi- channel");
QQvsx->Draw("colz");
yFcn->Draw("same");
WFcn->Draw("same");
QQFcn->Draw("same");
TLine *xLine[NxBins + 1];
TLine *QQLine[NxBins]; // each x bin has 2 QQ bins, I just want to draw the one QQ line that divides each x bin in half
for(int k = 0; k < NxBins + 1; k++)
{
xLine[k] = new TLine(xLimits[k], QQvsx->GetYaxis()->GetXmin(), xLimits[k], QQvsx->GetYaxis()->GetXmax());
xLine[k]->SetLineColor(kGray);
xLine[k]->SetLineWidth(2);
xLine[k]->Draw();
	if(k < NxBins)
	{
	QQLine[k] = new TLine(xLimits[k], QQLimits[k][1], xLimits[k+1], QQLimits[k][1]);
	QQLine[k]->SetLineColor(kGray);
	QQLine[k]->SetLineWidth(2);
	if(k!=4) QQLine[k]->Draw();
	}
}

//-------------------------------------------------------------------------------------//
//---------------------- now draw sub-bins --------------------------------------------//

float xLow[NxBins], xHigh[NxBins], QQLow[NxBins][NQQBins], QQHigh[NxBins][NQQBins];

xLow[0] = 0.17 - 0.01;
xLow[1] = 0.25 - 0.01;
xLow[2] = 0.35 - 0.01;
xLow[3] = 0.42 - 0.01;
xLow[4] = 0.53 - 0.01;

xHigh[0] = 0.17 + 0.01;
xHigh[1] = 0.25 + 0.01;
xHigh[2] = 0.35 + 0.01;
xHigh[3] = 0.42 + 0.01;
xHigh[4] = 0.53 + 0.01;

QQLow[0][0] = 1.15 - 0.05;
QQLow[0][1] = 1.35 - 0.05;
QQLow[1][0] = 1.45 - 0.05;
QQLow[1][1] = 1.95 - 0.05;
QQLow[2][0] = 2.05 - 0.05;
QQLow[2][1] = 2.55 - 0.05;
QQLow[3][0] = 2.65 - 0.05;
QQLow[3][1] = 3.15 - 0.05;
QQLow[4][0] = 4.05 - 0.05;
QQLow[4][1] = 5.55 - 0.05;

QQHigh[0][0] = 1.15 + 0.05;
QQHigh[0][1] = 1.35 + 0.05;
QQHigh[1][0] = 1.45 + 0.05;
QQHigh[1][1] = 1.95 + 0.05;
QQHigh[2][0] = 2.05 + 0.05;
QQHigh[2][1] = 2.55 + 0.05;
QQHigh[3][0] = 2.65 + 0.05;
QQHigh[3][1] = 3.15 + 0.05;
QQHigh[4][0] = 4.05 + 0.05;
QQHigh[4][1] = 5.55 + 0.05;

for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {
if(!(x == 4 && QQ == 1))
{

TLine *v1 = new TLine(xLow[x], QQLow[x][QQ], xLow[x], QQHigh[x][QQ]);
v1->Draw();
TLine *v2 = new TLine(xHigh[x], QQLow[x][QQ], xHigh[x], QQHigh[x][QQ]);
v2->Draw();
TLine *h1 = new TLine(xLow[x], QQLow[x][QQ], xHigh[x], QQLow[x][QQ]);
h1->Draw();
TLine *h2 = new TLine(xLow[x], QQHigh[x][QQ], xHigh[x], QQHigh[x][QQ]);
h2->Draw();

}
}}

}
