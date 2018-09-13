// this is a quick comparision of 3 different structure functions (h3 and h4) used in haprad for one bin
// the 0th model is h3=h4=0, the 1th model is the default, and 2th model is Nick's (pip) model

{
gStyle->SetOptStat(0);

// these numbers from haprad:
float sig[3][18] = {{2.788, 2.784, 2.778, 2.772, 2.756, 2.731, 2.713, 2.701, 2.695, 2.695, 2.701, 2.713, 2.731, 2.756, 2.772, 2.778, 2.784, 2.788}, {2.905, 2.885, 2.850, 2.806, 2.750, 2.688, 2.638, 2.605, 2.588, 2.588, 2.605, 2.638, 2.688, 2.750, 2.806, 2.850, 2.885, 2.905}, {3.105, 3.058, 2.973, 2.865, 2.740, 2.613, 2.510, 2.439, 2.403, 2.403, 2.439, 2.510, 2.613, 2.740, 2.865, 2.973, 3.058, 3.105}};

float sib[3][18] = {{2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685, 2.685}, {2.809, 2.792, 2.762, 2.722, 2.680, 2.640, 2.607, 2.583, 2.571, 2.571, 2.583, 2.607, 2.640, 2.680, 2.722, 2.762, 2.792, 2.809}, {3.016, 2.971, 2.890, 2.785, 2.671, 2.564, 2.475, 2.413, 2.381, 2.381, 2.413, 2.475, 2.564, 2.671, 2.785, 2.890, 2.971, 3.016}};

float tail[3][18] = {{0.1029E-01, 0.1242E-01, 0.1820E-01, 0.2648E-01, 0.2560E-01, 0.1562E-01, 0.9042E-02, 0.5930E-02, 0.4760E-02, 0.4760E-02, 0.5930E-02, 0.9042E-02, 0.1562E-01, 0.2560E-01, 0.2648E-01, 0.1820E-01, 0.1242E-01, 0.1029E-01}, {0.1029E-01, 0.1242E-01, 0.1820E-01, 0.2648E-01, 0.2560E-01, 0.1562E-01, 0.9042E-02, 0.5930E-02, 0.4760E-02, 0.4760E-02, 0.5930E-02, 0.9042E-02, 0.1562E-01, 0.2560E-01, 0.2648E-01, 0.1820E-01, 0.1242E-01, 0.1029E-01}, {0.1029E-01, 0.1242E-01, 0.1820E-01, 0.2648E-01, 0.2560E-01, 0.1562E-01, 0.9042E-02, 0.5930E-02, 0.4760E-02, 0.4760E-02, 0.5930E-02, 0.9042E-02, 0.1562E-01, 0.2560E-01, 0.2648E-01, 0.1820E-01, 0.1242E-01, 0.1029E-01}};

TH1F *hsig[3], *hsib[3], *htail[3], *hRC[3];

TCanvas *can = new TCanvas();
can->Divide(3, 2, 0.00001, 0.00001);

for(int m = 0; m < 3; m++)
{
hsig[m] = new TH1F(Form("hsig_%i", m), Form("hsig_%i", m), 18, -180, 180);
hsig[m]->SetLineColor(kRed);
hsig[m]->SetLineWidth(2);
hsib[m] = new TH1F(Form("hsib_%i", m), Form("hsib_%i", m), 18, -180, 180);
hsib[m]->SetLineColor(kBlue);
hsib[m]->SetLineWidth(2);
htail[m] = new TH1F(Form("htail_%i", m), Form("htail_%i", m), 18, -180, 180);
htail[m]->SetLineColor(kTeal);
htail[m]->SetLineWidth(2);
hRC[m] = new TH1F(Form("hRC_%i", m), Form("hRC_%i", m), 18, -180, 180);
hRC[m]->SetLineWidth(2);

for(int k = 0; k < 18; k++)
{
hsig[m]->SetBinContent(k+1, sig[m][k]);
hsib[m]->SetBinContent(k+1, sib[m][k]);
htail[m]->SetBinContent(k+1, tail[m][k]);
hRC[m]->SetBinContent(k+1, (sig[m][k]+tail[m][k])/sib[m][k]);
}

can->cd(m+1);
hsig[m]->GetYaxis()->SetRangeUser(0, 3.5);
hsig[m]->GetXaxis()->SetTitle("#phi (deg.)");
hsig[m]->Draw();
hsib[m]->Draw("same");
htail[m]->Draw("same");
}

can->cd(4);
hRC[0]->GetYaxis()->SetRangeUser(0, 1.1);
hRC[0]->SetLineColor(kGreen+3);
hRC[0]->GetXaxis()->SetTitle("#phi (deg.)");
hRC[0]->Draw();
hRC[1]->SetLineColor(kMagenta);
hRC[1]->Draw("same");
hRC[2]->SetLineColor(kOrange);
hRC[2]->Draw("same");

// clone and zoom in (annoying workaround):

TH1F *clone0 = (TH1F*) hRC[0]->Clone("clone0");
TH1F *clone1 = (TH1F*) hRC[1]->Clone("clone1");
TH1F *clone2 = (TH1F*) hRC[2]->Clone("clone2");

can->cd(5);
clone0->GetYaxis()->SetRangeUser(1.00, 1.05);
clone0->Draw();
clone1->Draw("same");
clone2->Draw("same");


}
