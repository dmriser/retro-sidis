{
TGraph *gr1 = new TGraph();
TGraph *gr2 = new TGraph();
TGraph *gr3 = new TGraph();

float min = 0.0;
float max = 2.0;
int N = 250;

float norm = 3.0;
float mu = 0.0;
float sig = 1.5;

for(int k = 0; k < N; k++)
{
	float pt = min + ((max-min)/N)*k;
	float pt2 = pt*pt;
	float g = norm*TMath::Exp(-1.0*sig*TMath::Power(pt-mu, 2.0));

	gr1->SetPoint(gr1->GetN(), pt, g);
	gr2->SetPoint(gr2->GetN(), pt2, g);
	gr3->SetPoint(gr3->GetN(), fabs(pt), g);
}

TCanvas *can = new TCanvas("can", "can", 1200, 400);
can->Divide(3, 1);
can->cd(1);
gr1->Draw("ALP*");
can->cd(2);
gr2->Draw("ALP*");
can->cd(3);
gr3->Draw("ALP*");



}
