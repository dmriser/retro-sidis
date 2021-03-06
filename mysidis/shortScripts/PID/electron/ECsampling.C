void ECsampling(int sector = 1)
{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kViridis);
  TFile *fD = new TFile("/u/home/dmriser/mysidis-histos/pid.data.s1.n12114.root");
  TFile *fMC = new TFile("/u/home/dmriser/mysidis-histos/pid.MonteCarlo_v12.s1.n32255.root");

  TH2F *hD[2][6];
  TH2F *hMC[2][6];

  const int Nstrictnesses = 5;

  // looser cut:  6 sigma on top, 4 sigma on bottom
  // loose cut:   5.5 sigma on top, 3.5 sigma on bottom
  // nominal cut: 5 sigma on top, 3 sigma on bottom
  // tight cut:   4.5 sigma on top, 2.5 sigma on bottom
  // tighter cut: 4 sigma on top, 2 sigma on bottom

  // upper cut, y = a + b*x + c*x*x
  // [exp/sim][strictness][sector]
  float a[2][Nstrictnesses][6] = {{{0.420613,0.424858,0.424702,0.429074,0.427833,0.428191}, {0.404299,0.408212,0.407958,0.41203,0.41093,0.411107}, {0.387985,0.391566,0.391215,0.394985,0.394027,0.394024}, {0.371671,0.374919,0.374471,0.377941,0.377124,0.376941}, {0.355357,0.358273,0.357728,0.360896,0.360221,0.359858}}, {{0.482389,0.598243,0.503293,0.493301,0.488109,0.48344}, {0.464078,0.572589,0.483542,0.474487,0.468019,0.464093}, {0.445767,0.546936,0.463791,0.455673,0.447928,0.444746}, {0.427455,0.521282,0.44404,0.436859,0.427838,0.425399}, {0.409144,0.495628,0.424289,0.418045,0.407748,0.408474}}};
  float b[2][Nstrictnesses][6] = {{{-0.0495682,-0.0568233,-0.0539308,-0.059438,-0.0569282,-0.0604589}, {-0.0446684,-0.0512794,-0.0484624,-0.0537194,-0.0513468,-0.0546141}, {-0.0397687,-0.0457355,-0.0429939,-0.0480007,-0.0457654,-0.0487694}, {-0.0348689,-0.0401916,-0.0375255,-0.042282,-0.0401839,-0.0429246}, {-0.0299691,-0.0346476,-0.0320571,-0.0365633,-0.0346025,-0.0370799}}, {{-0.023125,-0.0296968,-0.0240658,0.0155969,-0.0219532,-0.0119168}, {-0.0190486,-0.0237774,-0.0192748,0.0167098,-0.0174922,-0.00806353}, {-0.0149722,-0.017858,-0.0144838,0.0178227,-0.0130313,-0.00421029}, {-0.0108958,-0.0119386,-0.00969275,0.0189356,-0.00857032,-0.000357051}, {-0.00681948,-0.00601912,-0.00490176,0.0200485,-0.00410937,0.00121479}}};
  float c[2][Nstrictnesses][6] = {{{0.00514962,0.00749162,0.00646194,0.0077146,0.00744141,0.00844627}, {0.0045421,0.00669672,0.00570553,0.00688928,0.00664832,0.00756436}, {0.00393459,0.00590182,0.00494909,0.00606393,0.00585522,0.00668245}, {0.00332707,0.00510691,0.00419267,0.00523861,0.00506211,0.00580053}, {0.00271954,0.004312,0.00343625,0.00441326,0.00426903,0.00491862}}, {{0.00404757,0.004256,0.00488331,-0.00314474,0.00393541,0.00139946}, {0.00354432,0.00352811,0.00424241,-0.00310631,0.00330154,0.000932067}, {0.00304107,0.00280023,0.00360153,-0.00306787,0.00266766,0.000464671}, {0.00253782,0.00207234,0.00296064,-0.00302943,0.00203378,-2.72665e-06}, {0.00203457,0.00134445,0.00231975,-0.00299099,0.00139991,-2.84297e-05}}};

  // lower cut, y = d + e*x + f*x*x
  // [exp/sim][strictness][sector]
  float d[2][Nstrictnesses][6] = {{{0.0943313,0.0919313,0.0898338,0.0881846,0.0897731,0.0865283}, {0.110645,0.108578,0.106577,0.105229,0.106676,0.103611}, {0.12696,0.125224,0.123321,0.122274,0.123579,0.120695}, {0.143274,0.14187,0.140064,0.139318,0.140482,0.137778}, {0.159588,0.158517,0.156807,0.156363,0.157385,0.154861}}, {{0.116163,0.085166,0.108276,0.117021,0.0863021,0.0965012}, {0.134474,0.11082,0.128026,0.135835,0.106392,0.115848}, {0.152785,0.136474,0.147777,0.154649,0.126483,0.135195}, {0.171097,0.162128,0.167528,0.173463,0.146573,0.154542}, {0.189408,0.187781,0.187279,0.192277,0.166663,0.173889}}};
  float e[2][Nstrictnesses][6] = {{{0.048427,0.0540548,0.0554377,0.054936,0.0547004,0.056436}, {0.0435273,0.0485109,0.0499693,0.0492173,0.049119,0.0505913}, {0.0386275,0.042967,0.0445008,0.0434986,0.0435376,0.0447465}, {0.0337277,0.0374231,0.0390324,0.0377799,0.0379561,0.0389018}, {0.028828,0.0318792,0.033564,0.0320612,0.0323747,0.033057}}, {{0.0584026,0.0886916,0.0717544,0.0378552,0.0672658,0.0651479}, {0.0543262,0.0827722,0.0669633,0.0367423,0.0628048,0.0612946}, {0.0502498,0.0768528,0.0621723,0.0356294,0.0583439,0.0574414}, {0.0461734,0.0709334,0.0573813,0.0345165,0.0538829,0.0535882}, {0.0420971,0.0650139,0.0525903,0.0334035,0.049422,0.0497349}}};
  float f[2][Nstrictnesses][6] = {{{-0.0070008,-0.0084064,-0.00866651,-0.00879214,-0.00842053,-0.00919199}, {-0.00639328,-0.0076115,-0.00791009,-0.0079668,-0.00762743,-0.00831007}, {-0.00578576,-0.0068166,-0.00715367,-0.00714147,-0.00683433,-0.00742816}, {-0.00517824,-0.0060217,-0.00639724,-0.00631613,-0.00604123,-0.00654625}, {-0.00457072,-0.0052268,-0.00564082,-0.00549079,-0.00524814,-0.00566434}}, {{-0.00601745,-0.0103018,-0.00793447,-0.00237599,-0.00874211,-0.00794845}, {-0.0055142,-0.00957388,-0.00729358,-0.00241442,-0.00810823,-0.00748105}, {-0.00501095,-0.00884599,-0.00665269,-0.00245286,-0.00747436,-0.00701365}, {-0.00450769,-0.0081181,-0.0060118,-0.0024913,-0.00684048,-0.00654626}, {-0.00400444,-0.00739022,-0.00537091,-0.00252974,-0.00620661,-0.00607887}}};

  TF1 *Ltop[2][6][Nstrictnesses], *Lbottom[2][6][Nstrictnesses];

  for(int t = 0; t < 2; t++)
    {
      for(int s = 0; s < 6; s++)
	{
	  for(int st = 0; st < Nstrictnesses; st++)
	    {
	      Ltop[t][s][st] = new TF1(Form("Ltop%i%i%i", t, s, st), Form("%3.4f + %3.4f*x + %3.4f*x*x", a[t][st][s], b[t][st][s], c[t][st][s]), 0, 5);
	      
	      Lbottom[t][s][st] = new TF1(Form("Lbottom%i%i%i", t, s, st), Form("%3.4f + %3.4f*x + %3.4f*x*x", d[t][st][s], e[t][st][s], f[t][st][s]), 0, 5);

	      Ltop[t][s][st]->SetLineWidth(1);
	      Ltop[t][s][st]->SetLineStyle(9);
	      Lbottom[t][s][st]->SetLineWidth(1);
	      Lbottom[t][s][st]->SetLineStyle(9);

	    }
	}
    }

  TCanvas *can = new TCanvas("can", "can", 2400, 600);
  can->Divide(6, 2, 0.00001, 0.00001);
  /*
  int cut = 1;
  for(int s = 0; s < 6; s++){
    can->cd(cut*6 + s + 1);
    hD[cut][s] = (TH2F*) fD->Get(Form("eIDplots/etotVp_e_hist_c%i_s%i", cut, s+1));
    hD[cut][s]->GetXaxis()->SetTitle("p (GeV)");
    hD[cut][s]->GetYaxis()->SetTitle("E_{dep}/p");
    hD[cut][s]->GetYaxis()->SetTitleOffset(1.5);
    hD[cut][s]->SetTitle(Form("sector %i", s+1));
    hD[cut][s]->Draw("colz");
    
    if(cut == 1){
      for(int st = 0; st < Nstrictnesses; st++){
	Ltop[1][s][st]->Draw("same");
	Lbottom[1][s][st]->Draw("same");
      }
    }
    
    can->cd(6 + cut*6 + s + 1);
    hMC[cut][s] = (TH2F*) fMC->Get(Form("eIDplots/etotVp_e_hist_c%i_s%i", cut, s+1));
    hMC[cut][s]->GetXaxis()->SetTitle("p (GeV)");
    hMC[cut][s]->GetYaxis()->SetTitle("E_{dep}/p");
    hMC[cut][s]->GetYaxis()->SetTitleOffset(1.5);
    hMC[cut][s]->Draw("colz");
    if(cut==1)
      {
	for(int st = 0; st < Nstrictnesses; st++)
	  {
	    Ltop[0][s][st]->Draw("same");
	    Lbottom[0][s][st]->Draw("same");
	  }
      }
  }
  */

  for(int sect = 0; sect < 6; sect++){
    can->cd(sect + 1);
    
    // load data histogram 
    hD[1][sect] = (TH2F*) fD->Get(Form("eIDplots/etotVp_e_hist_c%i_s%i", 1, sect + 1));
    hD[1][sect]->GetXaxis()->SetTitle("p (GeV)");
    hD[1][sect]->GetYaxis()->SetTitle("E_{dep}/p");
    hD[1][sect]->GetYaxis()->SetTitleOffset(1.5);
    hD[1][sect]->SetTitle(Form("Sector %i (data)", sect + 1));
    hD[1][sect]->Draw("colz");
    
    // display lines to denote cut boundaries 
    for(int st = 0; st < Nstrictnesses; st++){
      Ltop[1][sect][st]->Draw("same");
      Lbottom[1][sect][st]->Draw("same");
    }

  }

  for(int sect = 0; sect < 6; sect++){
    can->cd(sect + 7);
    
    // load mc histogram 
    hMC[1][sect] = (TH2F*) fMC->Get(Form("eIDplots/etotVp_e_hist_c%i_s%i", 1, sect + 1));
    hMC[1][sect]->GetXaxis()->SetTitle("p (GeV)");
    hMC[1][sect]->GetYaxis()->SetTitle("E_{dep}/p");
    hMC[1][sect]->GetYaxis()->SetTitleOffset(1.5);
    hMC[1][sect]->SetTitle(Form("Sector %i (MC)", sect + 1));
    hMC[1][sect]->Draw("colz");

    // display lines to denote cut boundaries 
    for(int st = 0; st < Nstrictnesses; st++){
      Ltop[0][sect][st]->Draw("same");
      Lbottom[0][sect][st]->Draw("same");
    }

  }

  can->Print("/volatile/clas12/dmriser/plots/nathan/eid/sampling_fraction.pdf");

  // _____________________________________ //
  /*
  TCanvas *can2 = new TCanvas("can2", "can2", 60, 60, 1000, 800);
  can2->Divide(2, 2, 0.00001, 0.00001);

  can2->cd(1);
  hD[0][sector-1]->Draw("colz");
  can2->cd(2);
  hD[1][sector-1]->Draw("colz");
  //for(int st = 0; st < Nstrictnesses; st++)
  for(int st = 2; st <= 2; st++)
    {
      Ltop[1][sector-1][st]->Draw("same");
      Lbottom[1][sector-1][st]->Draw("same");
    }

  can2->cd(3);
  hMC[0][sector-1]->Draw("colz");
  can2->cd(4);
  hMC[1][sector-1]->Draw("colz");
  //for(int st = 0; st < Nstrictnesses; st++)
  for(int st = 2; st <= 2; st++)
    {
      Ltop[0][sector-1][st]->Draw("same");
      Lbottom[0][sector-1][st]->Draw("same");
    }

  //can2->SaveAs(Form("ECsampling_s%i.png", sector));
  */
}
