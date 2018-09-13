// this is set up to work with bin scheme "5"

void multiplicity_fit_loop()
{
	const int NxBins = 5;
	const int NQQBins = 2;
	const int NzBins = 18;
	const int Nparticles = 2;
	string hadrons[Nparticles] = {"pip", "pim"};
	TH1F *hN = new TH1F("hN", "hN", 22, 0, 22);

	for(int ix = 0; ix < NxBins; ix++)
	{
		for(int iQQ = 0; iQQ < NQQBins; iQQ++)
		{
			for(int iz = 0; iz < NzBins; iz++)
			{
				const int NPT2Bins = 20;
				float PT2Min = 0;
				float PT2Max = 1;
				
				for(int ipart = 0; ipart < Nparticles; ipart++)
				{
					int Nentries = 0;

					TH1F *hpionM = new TH1F("hpionM", "hpionM", NPT2Bins, PT2Min, PT2Max);
	
					for(int k = 0; k < NPT2Bins; k++)
					{
						ifstream pionMfile(Form("/scratch/MAcAccfiles/%s_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", hadrons[ipart].c_str(), ix, iQQ, iz, k));
						
						float pionM, pionMe;
						
						if(pionMfile)
						{
							pionMfile>>pionM>>pionMe;
						
							hpionM->SetBinContent(k+1, pionM);
							hpionM->SetBinError(k+1, pionMe);
							Nentries++;
						}
						
						pionMfile.close();
					}
	
					float maxParVal = 5.0*TMath::Power(10.0, 7.0); // ideally infinity, any bigger causes problems (???), this should suffice

					TF1 *ffpion = new TF1("ffpion", "[0]*TMath::Exp(-1.0*[1]*x)", PT2Min, PT2Max);
					ffpion->SetParameters(hpionM->GetMaximum(), 1.0);
					ffpion->SetParLimits(0, 0.0, maxParVal);
					ffpion->SetParLimits(1, 0.0, maxParVal);
					if(Nentries > 0) hpionM->Fit(ffpion);
					hN->Fill(Nentries + 0.1);
					//if(Nentries == 1) cout<<ipart<<" "<<ix<<" "<<iQQ<<" "<<iz<<endl;
	
					delete hpionM;
					hpionM = NULL;
					delete ffpion;
					ffpion = NULL;
				}
			}
		}
	}

hN->GetXaxis()->SetTitle("N pts");
hN->Draw();
}
