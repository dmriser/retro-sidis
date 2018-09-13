{
string pipORpim = "pim";
string varName = "z";

TFile *tf = new TFile("MonteCarlo_v12.it0.s1.n11625.BiSc7.__0000000000000000__.root");

float minSig = 1000;
float maxSig = -1000;

for(int ix = 0; ix < 5; ix++)
{
	for(int iQQ = 0; iQQ < 2; iQQ++)
	{
		for(int iz = 0; iz < 9; iz++)
		{
			for(int iPTsq = 0; iPTsq < 10; iPTsq++)
			{
				for(int iphi = 0; iphi < 12; iphi++)
				{
					TH1F *res = (TH1F*) tf->Get(Form("%s_%s_res_x%i_QQ%i_z%i_PT2%i_phih%i", pipORpim.c_str(), varName.c_str(), ix, iQQ, iz, iPTsq, iphi));
					if(res->Integral() > 500)
					{
						TF1 *ff = new TF1("ff", "gaus", 0.5*res->GetXaxis()->GetXmin(), 0.5*res->GetXaxis()->GetXmax());
						ff->SetParameter(0, res->GetMaximum());
						ff->SetParameter(1, 0.0);
						ff->SetParameter(2, res->GetRMS());
						res->Fit(ff, "Q", "Q", 0.5*res->GetXaxis()->GetXmin(), 0.5*res->GetXaxis()->GetXmax());
						if(ff->GetParameter(2) < minSig) minSig = ff->GetParameter(2);
						if(ff->GetParameter(2) > maxSig) maxSig = ff->GetParameter(2);
						//if(ff->GetParameter(2) > 5) cout<<ix<<" "<<iQQ<<" "<<iz<<" "<<iPTsq<<" "<<iphi<<endl;
						//if(ff->GetParameter(2) > 5) cout<<ff->GetParError(2)<<endl;
					}
				}
			}
		}
	}
}

cout<<endl<<minSig<<" "<<maxSig<<endl<<endl;

}
