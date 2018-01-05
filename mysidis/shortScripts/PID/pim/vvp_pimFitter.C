void vvp_pimFitter(int sector = 1)
{
gStyle->SetOptStat(0);

const int Ntypes = 2; // data and MC
const int Nslices = 70;
float xStart = 0.2; // x is momentum here
float xEnd = 3.25;
const int Nsectors = 6;

TFile *file[Ntypes];
file[0] = new TFile("/home/kjgroup/mysidis/histos/pid.MonteCarlo_v12.s1.n32255.root");
file[1] = new TFile("/home/kjgroup/mysidis/histos/pid.data.s1.n12114.root");

TH2F *hvvp[Ntypes];
TH1D *slice[Ntypes][Nslices];
TF1 *sliceFF[Ntypes][Nslices];
float sliceXmid[Nslices];
float lowerCutVal[Ntypes][Nslices];
float upperCutVal[Ntypes][Nslices];

TCanvas *can2D = new TCanvas("can2D", "can2D", 100, 100, 1600, 1000);
can2D->Divide(3, 2, 0.00001, 0.00001);
TCanvas *slicecan[Ntypes];

TLatex *lat = new TLatex();
lat->SetNDC();
lat->SetTextSize(0.09);

for(int t = Ntypes-1; t >= 0; t--)
{
can2D->cd(3*(2-t))->SetLogz();
hvvp[t] = (TH2F*) file[t]->Get(Form("pimIDplots/vvp_pim_esect_c1_s%i", sector));
hvvp[t]->GetXaxis()->SetTitle("p (GeV)");
hvvp[t]->GetYaxis()->SetTitle("velocity (c)");
if(t == 1) hvvp[t]->SetTitle(Form("data, e- s%i, -tracks, with all other ID cuts", sector));
if(t == 0) hvvp[t]->SetTitle(Form("MC, e- s%i, -tracks, with all other ID cuts", sector));
hvvp[t]->Draw("colz");

float binDensity = hvvp[t]->GetXaxis()->GetNbins()/(hvvp[t]->GetXaxis()->GetXmax() - hvvp[t]->GetXaxis()->GetXmin());
float sliceWidth = (xEnd - xStart)/Nslices;
int firstxBin = (xStart - hvvp[t]->GetXaxis()->GetXmin())*binDensity + 1;
float binsPerSlice = sliceWidth*binDensity;

slicecan[t] = new TCanvas(Form("slicecan_t%i_s%i", t, sector), Form("slicecan_t%i_s%i", t, sector), 50*t+20, 30*t+10, 1600, 1000);
slicecan[t]->Divide(10, 7, 0.00001, 0.00001); // might need manual change

   for(int a = 0; a < Nslices; a++)
   {
   slicecan[t]->cd(a+1);

   sliceXmid[a] = xStart + 0.5*sliceWidth + a*sliceWidth;
   slice[t][a] = hvvp[t]->ProjectionY(Form("projection%i_t%i_s%i", a, t, sector), firstxBin + a*(binsPerSlice), firstxBin + (a+1)*(binsPerSlice) - 1);
   slice[t][a]->GetXaxis()->SetTitle("velocity (c)");
   slice[t][a]->SetTitle(Form("(%i)  %3.2f < p < %3.2f GeV", a, xStart + sliceWidth*a, xStart + sliceWidth*(a+1)));
   slice[t][a]->GetXaxis()->SetRangeUser(0.75, 1.1);
	slice[t][a]->GetXaxis()->SetLabelSize(0.05);

	float pion_mu = sqrt((sliceXmid[a]*sliceXmid[a])/(0.14*0.14 + sliceXmid[a]*sliceXmid[a])); // theoretical value

   sliceFF[t][a] = new TF1(Form("sliceFF%i_t%i_s%i", a, t, sector), "gaus(0)", pion_mu - 0.2, pion_mu + 0.2);
   sliceFF[t][a]->SetParameters(slice[t][a]->GetMaximum(), pion_mu, 0.01);
   sliceFF[t][a]->SetParLimits(0, 0.1*slice[t][a]->GetMaximum(), 2.0*slice[t][a]->GetMaximum());
   sliceFF[t][a]->SetParLimits(1, pion_mu-0.035, pion_mu+0.035);
   sliceFF[t][a]->SetParLimits(2, 0.00000001, 0.03);

	slice[t][a]->Draw();
   slice[t][a]->Fit(Form("sliceFF%i_t%i_s%i", a, t, sector), "q M", "", pion_mu - 0.02, pion_mu + 0.02);
   slice[t][a]->Fit(Form("sliceFF%i_t%i_s%i", a, t, sector), "q M", "", sliceFF[t][a]->GetParameter(1) - 0.015, sliceFF[t][a]->GetParameter(1) + 0.015);
   if(t==0) slice[t][a]->Fit(Form("sliceFF%i_t%i_s%i", a, t, sector), "q M", "", sliceFF[t][a]->GetParameter(1) - 0.014, sliceFF[t][a]->GetParameter(1) + 0.014);
   if(t==1) slice[t][a]->Fit(Form("sliceFF%i_t%i_s%i", a, t, sector), "q M", "", sliceFF[t][a]->GetParameter(1) - 0.012, sliceFF[t][a]->GetParameter(1) + 0.012);
	lat->DrawLatex(0.2, 0.7, Form("#sigma = %3.3f", sliceFF[t][a]->GetParameter(2)));

	float Nsigma = 3.0; // just a starting point
	lowerCutVal[t][a] = sliceFF[t][a]->GetParameter(1) - Nsigma*sliceFF[t][a]->GetParameter(2);
	upperCutVal[t][a] = sliceFF[t][a]->GetParameter(1) + Nsigma*sliceFF[t][a]->GetParameter(2);

	TLine *LNsig = new TLine(lowerCutVal[t][a], 0, lowerCutVal[t][a], 0.7*slice[t][a]->GetMaximum()); // draw the nieve Nsigma cut (for comparison)
	LNsig->Draw();
	TLine *RNsig = new TLine(upperCutVal[t][a], 0, upperCutVal[t][a], 0.7*slice[t][a]->GetMaximum());
	RNsig->Draw();

	// Exceptions: (data only) ... (note that the for loop goes backwards (1,0 instead of 0,1) to do data first)
	if(t == 1)
	{
	if(a >= 25 && a <= 28) lowerCutVal[1][a] = 0.965;
	if(a >= 29 && a <= 41) lowerCutVal[1][a] = 0.970;
	if(a >= 42 && a <= 999) lowerCutVal[1][a] = 0.975;
	}
	// end exceptions

	// adjust the MC based on the above exceptions:
	if(t == 0)
	{
	float recalcNsig_low = (sliceFF[1][a]->GetParameter(1) - lowerCutVal[1][a])/sliceFF[1][a]->GetParameter(2); // calculate for data, use result in MC
	float recalcNsig_up = (upperCutVal[1][a] - sliceFF[1][a]->GetParameter(1))/sliceFF[1][a]->GetParameter(2);

	lowerCutVal[0][a] = sliceFF[0][a]->GetParameter(1) - recalcNsig_low*sliceFF[0][a]->GetParameter(2);
	upperCutVal[0][a] = sliceFF[0][a]->GetParameter(1) + recalcNsig_up*sliceFF[0][a]->GetParameter(2);
	}

	// Draw the actual cut:
	TLine *Lcut = new TLine(lowerCutVal[t][a], 0, lowerCutVal[t][a], 0.5*slice[t][a]->GetMaximum());
	Lcut->SetLineColor(kRed);
	Lcut->SetLineWidth(2);
	Lcut->Draw();
	TLine *Rcut = new TLine(upperCutVal[t][a], 0, upperCutVal[t][a], 0.5*slice[t][a]->GetMaximum());
	Rcut->SetLineColor(kRed);
	Rcut->SetLineWidth(2);
	Rcut->Draw();

	int markerstyle = 2;
	
	can2D->cd(3*(2-t));
	TMarker *lowmarker = new TMarker(sliceXmid[a], lowerCutVal[t][a], markerstyle);
	lowmarker->Draw();
	TMarker *upmarker = new TMarker(sliceXmid[a], upperCutVal[t][a], markerstyle);
	upmarker->Draw();

//	float Nsig_systematics = 0.25;
//	TMarker *lowmarker_syslow = new TMarker(sliceXmid[a], lowerCutVal[t][a] - Nsig_systematics*sliceFF[t][a]->GetParameter(2), markerstyle);
//	lowmarker_syslow->Draw();
//	TMarker *lowmarker_sysup = new TMarker(sliceXmid[a], lowerCutVal[t][a] + Nsig_systematics*sliceFF[t][a]->GetParameter(2), markerstyle);
//	lowmarker_sysup->Draw();
//	TMarker *upmarker_syslow = new TMarker(sliceXmid[a], upperCutVal[t][a] - Nsig_systematics*sliceFF[t][a]->GetParameter(2), markerstyle);
//	upmarker_syslow->Draw();
//	TMarker *upmarker_sysup = new TMarker(sliceXmid[a], upperCutVal[t][a] + Nsig_systematics*sliceFF[t][a]->GetParameter(2), markerstyle);
//	upmarker_sysup->Draw();

	// Print the values
	//if(t == 1 && a == 0) cout<<"data sector "<<sector<<endl;
	//if(t == 0 && a == 0) cout<<"MC sector "<<sector<<endl;
	//cout<<"lowerVal"<<a<<" "<<lowerCutVal[t][a]<<endl;
	//cout<<"upperVal"<<a<<" "<<upperCutVal[t][a]<<endl;
	
	//ofstream outfile;
	//outfile.open(Form("t%i_pimVelocityCut_es%i_pBin%i.txt", t, sector, a));
	//outfile<<lowerCutVal[t][a]<<" "<<upperCutVal[t][a]<<" "<<sliceFF[t][a]->GetParameter(2);
	//outfile.close();

   }
}

// ________________________________ //

// Add "no cuts" histos to can2D

TH2F *notCorrData = (TH2F*) file[1]->Get(Form("pimIDplots/vvp_pim_esect_notCorr_s%i", sector));
notCorrData->GetXaxis()->SetTitle("p (GeV)");
notCorrData->GetYaxis()->SetTitle("velocity (c)");
notCorrData->SetTitle(Form("data, e- s%i, negative tracks", sector));
//TH2F *notCorrMC = (TH2F*) file[0]->Get(Form("pimIDplots/vvp_pim_esect_notCorr_s%i", sector));
//notCorrMC->GetXaxis()->SetTitle("p (GeV)");
//notCorrMC->GetYaxis()->SetTitle("velocity (c)");
//notCorrMC->SetTitle(Form("data, e- s%i, negative tracks", sector));

can2D->cd(1)->SetLogz();
notCorrData->Draw("colz");

TH2F *nocutsData = (TH2F*) file[1]->Get(Form("pimIDplots/vvp_pim_esect_c0_s%i", sector));
nocutsData->GetXaxis()->SetTitle("p (GeV)");
nocutsData->GetYaxis()->SetTitle("velocity (c)");
nocutsData->SetTitle(Form("data, e- s%i, -tracks, t corr", sector));
TH2F *nocutsMC = (TH2F*) file[0]->Get(Form("pimIDplots/vvp_pim_esect_c0_s%i", sector));
nocutsMC->GetXaxis()->SetTitle("p (GeV)");
nocutsMC->GetYaxis()->SetTitle("velocity (c)");
nocutsMC->SetTitle(Form("MC, e- s%i, -tracks", sector));

can2D->cd(2)->SetLogz();
nocutsData->Draw("colz");
can2D->cd(5)->SetLogz();
nocutsMC->Draw("colz");

// ________________________________ //

// save:

//slicecan[0]->SaveAs(Form("pim_vvp_MC_1D_s%i.png", sector));
//slicecan[1]->SaveAs(Form("pim_vvp_data_1D_s%i.png", sector));
//can2D->SaveAs(Form("pim_vvp_2D_s%i.png", sector));

}
