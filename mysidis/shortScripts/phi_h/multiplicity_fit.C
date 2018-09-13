// this is set up to work with bin scheme "5"

#include <algorithm> // for std::max

void multiplicity_fit(int xBin = 0, int QQBin = 0, int zBin = 6)
{
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.04, "XY");
	gStyle->SetTitleSize(0.04, "XY");
	gStyle->SetTitleOffset(0.95, "XY");
	gStyle->SetLineWidth(2);
	gStyle->SetHistLineWidth(2);
	gStyle->SetMarkerSize(1.25);
	
	const int NPT2Bins = 20;
	float PT2Min = 0;
	float PT2Max = 1;
	
	TCanvas *can = new TCanvas("can", "can", 20, 20, 1200, 600);
	can->Divide(2, 1);
	
	TH1F *hpipM;
	TH1F *hpimM;
	
	hpipM = new TH1F("hpipM", "hpipM", NPT2Bins, PT2Min, PT2Max);
	hpimM = new TH1F("hpimM", "hpimM", NPT2Bins, PT2Min, PT2Max);
	
	/// pip: ///
	for(int k = 0; k < NPT2Bins; k++)
	{
		ifstream pipMfile(Form("/scratch/MAcAccfiles/pip_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
		
		float pipM, pipMe;
		
		if(pipMfile)
		{
			pipMfile>>pipM>>pipMe;
		
			hpipM->SetBinContent(k+1, pipM);
			hpipM->SetBinError(k+1, pipMe);
		}
		
		pipMfile.close();
	}
	
	/// pim: ///
	for(int k = 0; k < NPT2Bins; k++)
	{
		ifstream pimMfile(Form("/scratch/MAcAccfiles/pim_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", xBin, QQBin, zBin, k));
		
		float pimM, pimMe;
		
		if(pimMfile)
		{
			pimMfile>>pimM>>pimMe;
		
			hpimM->SetBinContent(k+1, pimM);
			hpimM->SetBinError(k+1, pimMe);
		}
		
		pimMfile.close();
	}
	
	/// Draw: ///
	can->cd(1)->SetGrid();
	hpipM->SetLineColor(kRed);
	hpipM->SetMarkerStyle(22);
	hpipM->SetMarkerColor(kRed);
	hpipM->SetTitle(Form("Multiplicity for #pi+ (red) and #pi- (blue), x%i QQ%i z%i", xBin, QQBin, zBin));
	hpipM->GetXaxis()->SetTitle("P_{h#perp}^{2} (GeV^{2})");
	hpipM->Draw("E1");

	hpimM->SetLineColor(kBlue);
	hpimM->SetMarkerStyle(23);
	hpimM->SetMarkerColor(kBlue);
	hpimM->Draw("same E1");

	can->cd(2)->SetGrid();
	can->cd(2)->SetLogy();
	hpipM->Draw("E1");
	hpimM->Draw("same E1");

	/// Fit: ///
	float maxParVal = 5.0*TMath::Power(10.0, 7.0); // ideally infinity, any bigger causes problems (???), this should suffice

	TF1 *ffpip = new TF1("ffpip", "[0]*TMath::Exp(-1.0*[1]*x)", PT2Min, PT2Max);
	ffpip->SetLineColor(kRed);
	ffpip->SetParameters(hpipM->GetMaximum(), 1.0);
	ffpip->SetParLimits(0, 0.0, maxParVal);
	ffpip->SetParLimits(1, 0.0, maxParVal);
	hpipM->Fit(ffpip, "R");
	ffpip->Draw("same");

	TF1 *ffpim = new TF1("ffpim", "[0]*TMath::Exp(-1.0*[1]*x)", PT2Min, PT2Max);
	ffpim->SetLineColor(kBlue);
	ffpim->SetParameters(hpimM->GetMaximum(), 1.0);
	ffpim->SetParLimits(0, 0.0, maxParVal);
	ffpim->SetParLimits(1, 0.0, maxParVal);
	hpimM->Fit(ffpim, "R");
	ffpim->Draw("same");
}
