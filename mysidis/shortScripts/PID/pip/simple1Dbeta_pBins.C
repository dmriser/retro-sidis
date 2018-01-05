void simple1Dbeta_pBins(int sector = 1)
{
gStyle->SetOptStat(0);

const int Ntypes = 2; // data and MC
const int Nslices = 30;
float xStart = 0.2; // x is momentum here
float xEnd = 4.3;

TFile *file[Ntypes];
//file[0] = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");
//file[0] = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n20.root");
file[0] = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root"); // if you change this back to MC, also change the Get lines a few lines down
file[1] = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");

TH2F *hvvp[Ntypes];
TH1D *slice[Ntypes][Nslices];

TCanvas *can2D = new TCanvas("can2D", "can2D", 100, 100, 1000, 500);
can2D->Divide(2, 1);

TCanvas *slicecan = new TCanvas(Form("slicecan_s%i", sector), Form("slicecan_s%i", sector), 10, 10, 1600, 1000);
slicecan->Divide(6, 5, 0.00001, 0.00001); // might need manual change

for(int t = 0; t < Ntypes; t++)
{
can2D->cd(2-t)->SetLogz();
if(t==0) hvvp[t] = (TH2F*) file[t]->Get(Form("pipIDplots/vvp_pip_esect_c1_s%i", sector));
if(t==1) hvvp[t] = (TH2F*) file[t]->Get(Form("pipIDplots/vvp_pip_esect_c0_s%i", sector));
//hvvp[t] = (TH2F*) file[t]->Get(Form("pipIDplots/vvp_pip_MXgt1p35_c0_s%i", sector));
//hvvp[t] = (TH2F*) file[t]->Get(Form("pipIDplots/vvp_pip_MXgt1p1_c0_s%i", sector));
//hvvp[t] = (TH2F*) file[t]->Get(Form("pipIDplots/vvp_pip_hist_c0_s%i", sector));
hvvp[t]->GetXaxis()->SetTitle("p (GeV)");
hvvp[t]->GetYaxis()->SetTitle("velocity (c)");
if(t == 1) hvvp[t]->SetTitle(Form("data, e- s%i, +tracks, with all other ID cuts", sector));
if(t == 0) hvvp[t]->SetTitle(Form("MC, e- s%i, +tracks, with all other ID cuts", sector));
hvvp[t]->GetYaxis()->SetRangeUser(0.45, 1.1);
hvvp[t]->Draw("colz");

float binDensity = hvvp[t]->GetXaxis()->GetNbins()/(hvvp[t]->GetXaxis()->GetXmax() - hvvp[t]->GetXaxis()->GetXmin());
float sliceWidth = (xEnd - xStart)/Nslices;
int firstxBin = (xStart - hvvp[t]->GetXaxis()->GetXmin())*binDensity + 1;
float binsPerSlice = sliceWidth*binDensity;

   for(int a = 0; a < Nslices; a++)
   {
   slicecan->cd(a+1);

   slice[t][a] = hvvp[t]->ProjectionY(Form("projection%i_t%i_s%i", a, t, sector), firstxBin + a*(binsPerSlice), firstxBin + (a+1)*(binsPerSlice) - 1);
   slice[t][a]->GetXaxis()->SetTitleSize(0.05);
   slice[t][a]->GetXaxis()->SetTitle("velocity (c)");
   slice[t][a]->SetTitleSize(0.05);
   slice[t][a]->SetTitle(Form("(%i)  %3.2f < p < %3.2f GeV", a, xStart + sliceWidth*a, xStart + sliceWidth*(a+1)));
   slice[t][a]->GetXaxis()->SetRangeUser(0.75, 1.1);
	slice[t][a]->GetXaxis()->SetLabelSize(0.05);
	if(t == 1) slice[t][a]->SetLineColor(kBlue);
	if(t == 0) slice[t][a]->SetLineColor(kRed);
	slice[t][a]->Scale(1.0/slice[t][a]->GetMaximum());
	slice[t][a]->Draw("same");
	//slice[t][a]->DrawNormalized("same");
	}
}

}
