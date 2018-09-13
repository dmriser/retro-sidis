{
int xBin = 0;
int QQBin = 1;
int binSchemeOpt = 5;
string pipORpim = "pip";

string hapDIRstring;
if(pipORpim == "pip") hapDIRstring = "NickPipModel";
if(pipORpim == "pim") hapDIRstring = "NickPimModel";

if(binSchemeOpt == 5)
{
int NphihBins = 36;
int zStartBin = 2;
int cutLastNzBins = 5;
const int NzBins = 18 - zStartBin - cutLastNzBins; // this is the effective number of z bins
int PT2StartBin = 0;
int cutLastNPT2Bins = 0;
const int NPT2Bins = 20 - PT2StartBin - cutLastNPT2Bins; // this is the effective number of PT2 bins
float PT2Min = 0.0;
float PT2BW = 0.05;
}

// ---------- haprad: ---------- //
TH1F *hsib[NzBins][NPT2Bins];
TF1 *ffsib[NzBins][NPT2Bins];
TGraphErrors *gAcVPT2_hap[NzBins];
TGraphErrors *gAccVPT2_hap[NzBins];
TGraphErrors *gAcVPT2_data[NzBins];
TGraphErrors *gAccVPT2_data[NzBins];
TCanvas *hapradcan = new TCanvas("hapradcan", "hapradcan", 10, 10, 1600, 900);
hapradcan->Divide(NzBins, NPT2Bins, 0, 0);
TCanvas *can = new TCanvas("can", "can", 10, 10, 1400, 500);
can->Divide(NzBins, 2, 0.00001, 0.00001);

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
	gAcVPT2_hap[z-zStartBin] = new TGraphErrors();
	gAccVPT2_hap[z-zStartBin] = new TGraphErrors();
	gAcVPT2_data[z-zStartBin] = new TGraphErrors();
	gAccVPT2_data[z-zStartBin] = new TGraphErrors();
	
	for(int PT2 = PT2StartBin; PT2 < NPT2Bins + PT2StartBin; PT2++) {
		hapradcan->cd((NzBins*(NPT2Bins-1) - NzBins*(PT2-PT2StartBin)) + z-zStartBin + 1);
		hsib[z-zStartBin][PT2-PT2StartBin] = new TH1F(Form("hsib_z%iPT2%i", z, PT2), Form("hsib_z%iPT2%i", z, PT2), NphihBins, -180, 180);
		ffsib[z-zStartBin][PT2-PT2StartBin] = new TF1(Form("ffsib_z%iPT2%i", z, PT2), "[0]*(1.0 + [1]*cos((3.1415926/180.0)*x) + [2]*cos(2.0*(3.1415926/180.0)*x))", -180, 180);
		
		ifstream datfile(Form("hapradResults/%s/%s_BiSc%i_x%iQQ%iz%iPT2%i.dat", hapDIRstring.c_str(), pipORpim.c_str(), binSchemeOpt, xBin, QQBin, z, PT2));
		if(datfile)
		{
			for(int phih = 0; phih < NphihBins; phih++)
			{
				float tempsig, tempsib, temptail;
				datfile>>tempsig>>tempsib>>temptail;
				
				hsib[z-zStartBin][PT2-PT2StartBin]->SetBinContent(phih+1, tempsib);
				hsib[z-zStartBin][PT2-PT2StartBin]->SetBinError(phih+1, 0.02*tempsib); // fake error bar to make fit work
			}

			if(hsib[z-zStartBin][PT2-PT2StartBin]->GetMaximum() > 0.000000001)
			{
				ffsib[z-zStartBin][PT2-PT2StartBin]->SetParameters(hsib[z-zStartBin][PT2-PT2StartBin]->GetMaximum(), 0.0, 0.0);
				hsib[z-zStartBin][PT2-PT2StartBin]->Fit(Form("ffsib_z%iPT2%i", z, PT2), "q", "", -180, 180);
				gAcVPT2_hap[z-zStartBin]->SetPoint(gAcVPT2_hap[z-zStartBin]->GetN(), PT2*PT2BW + 0.5*PT2BW, ffsib[z-zStartBin][PT2-PT2StartBin]->GetParameter(1));
				gAcVPT2_hap[z-zStartBin]->SetPointError(gAcVPT2_hap[z-zStartBin]->GetN()-1, 0.0, 0.0);
				gAccVPT2_hap[z-zStartBin]->SetPoint(gAccVPT2_hap[z-zStartBin]->GetN(), PT2*PT2BW + 0.5*PT2BW, ffsib[z-zStartBin][PT2-PT2StartBin]->GetParameter(2));
				gAccVPT2_hap[z-zStartBin]->SetPointError(gAccVPT2_hap[z-zStartBin]->GetN()-1, 0.0, 0.0);
			}
		}
		datfile.close();
		hsib[z-zStartBin][PT2-PT2StartBin]->Draw();
	}

	can->cd(z-zStartBin+1);
	gAcVPT2_hap[z-zStartBin]->Draw("AP*");
	gAcVPT2_hap[z-zStartBin]->GetYaxis()->SetRangeUser(-0.6, 0.6);
	gAcVPT2_hap[z-zStartBin]->Draw("AP*");
	gAcVPT2_hap[z-zStartBin]->GetXaxis()->SetLabelSize(0.08);
	gAcVPT2_hap[z-zStartBin]->GetYaxis()->SetLabelSize(0.08);
	can->Update();

	can->cd(NzBins+z-zStartBin+1);
	gAccVPT2_hap[z-zStartBin]->Draw("AP*");
	gAccVPT2_hap[z-zStartBin]->GetYaxis()->SetRangeUser(-0.6, 0.6);
	gAccVPT2_hap[z-zStartBin]->Draw("AP*");
	gAccVPT2_hap[z-zStartBin]->GetXaxis()->SetLabelSize(0.08);
	gAccVPT2_hap[z-zStartBin]->GetYaxis()->SetLabelSize(0.08);
	can->Update();
}

// ---------- data: ---------- //
int Nlines;
if(pipORpim == "pip") Nlines = 1331;
if(pipORpim == "pim") Nlines = 1134;
ifstream infile(Form("%s_4Dtable.txt", pipORpim.c_str()));

for(int k = 0; k < Nlines; k++)
{
	int ix, iQQ, iz, iPT2;
	float junk, A0, Ac, Acc, A0E, AcE, AccE;
	infile>>ix>>iQQ>>iz>>iPT2>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>A0>>A0E>>Ac>>AcE>>Acc>>AccE;
	
	if(ix == xBin && iQQ == QQBin && iz >= zStartBin && iz < zStartBin+NzBins && iPT2 >= PT2StartBin && iPT2 < PT2StartBin+NPT2Bins)
	{
		gAcVPT2_data[iz-zStartBin]->SetPoint(gAcVPT2_data[iz-zStartBin]->GetN(),  iPT2*PT2BW + 0.5*PT2BW, Ac);
		gAcVPT2_data[iz-zStartBin]->SetPointError(gAcVPT2_data[iz-zStartBin]->GetN()-1,  0.5*PT2BW, AcE);
		gAccVPT2_data[iz-zStartBin]->SetPoint(gAccVPT2_data[iz-zStartBin]->GetN(),  iPT2*PT2BW + 0.5*PT2BW, Acc);
		gAccVPT2_data[iz-zStartBin]->SetPointError(gAccVPT2_data[iz-zStartBin]->GetN()-1,  0.5*PT2BW, AccE);
	}
}

for(int z = zStartBin; z < NzBins + zStartBin; z++) {
	can->cd(z-zStartBin+1);
	gAcVPT2_data[z-zStartBin]->SetMarkerColor(kRed);
	gAcVPT2_data[z-zStartBin]->SetLineColor(kRed);
	gAcVPT2_data[z-zStartBin]->Draw("P*");
	can->cd(NzBins+z-zStartBin+1);
	gAccVPT2_data[z-zStartBin]->SetMarkerColor(kRed);
	gAccVPT2_data[z-zStartBin]->SetLineColor(kRed);
	gAccVPT2_data[z-zStartBin]->Draw("P*");
}

infile.close();
}
