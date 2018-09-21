/*

  File: processOneBinSectorSystematics.cpp

  Author: Nathan Harrison
  Revisions: David Riser
  Date: September 21, 2018

  Changelog:
  ---------
      2018-09-21: Created this entry, starting modifications. Fixed indentation.  Setting 
                  local variable for the base directory and project name. 

*/

#include <algorithm> // for std::max

//void sector_systematics(int xBin = 1, int QQBin = 1, int zBin = 7, int PT2Bin = 9) // good for pi+, not great for pi-
//void sector_systematics(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 4) // decent for both
void processOneBinSectorSystematics(int xBin = 0, int QQBin = 0, int zBin = 3, int PT2Bin = 5, string pipORpim = "pim"){

  static const int nSec = 6;

  const std::string baseDirectory("/volatile/clas12/dmriser/farm_out"); 
  const std::string projectName("sidis_batch_11"); 
  const std::string message("[ProcessOneBinSectorSystematics] "); 

  gStyle->SetOptStat(0);
  bool doSaveRoot = 1;

  TH1F *hdataphih[nSec];
  TH1F *hgenphih[nSec];
  TH1F *hrecphih[nSec];
  TH1F *haccphih[nSec];
  TH1F *hcorrphih[nSec];
  TF1 *ff[nSec];

  TH1F *hM = new TH1F(Form("hM_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
		      Form("hM_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
		      6, 0, 6);
  TH1F *hAc = new TH1F(Form("hAc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
		       Form("hAc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
		       6, 0, 6);
  TH1F *hAcc = new TH1F(Form("hAcc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
			Form("hAcc_%s_%i_%i_%i_%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin), 
			6, 0, 6);

  TLatex *tlat = new TLatex();
  tlat->SetNDC();
  tlat->SetTextSize(0.06);

  TCanvas *can = new TCanvas("can", "can", 10, 10, 1400, 1000);
  can->Divide(6, 4, 0.00001, 0.00001);

  float corrYmax = 10.0;
  for(int is = 0; is < nSec; is++){
    TFile *tfdata = new TFile(Form("%s/%s/data/variation_nominal/merged.root", 
				   baseDirectory.c_str(), projectName.c_str()));
    TFile *tfmc   = new TFile(Form("%s/%s/mc/variation_nominal/merged.root", 
				   baseDirectory.c_str(), projectName.c_str()));

    if (tfdata && tfdata->IsOpen()){
      cout << message << "Opened data." << endl; 
    } else {
      cerr << message << "Troubling opening data." << endl; 
    }

    if (tfmc && tfmc->IsOpen()){
      cout << message << "Opened MC." << endl; 
    } else {
      cerr << message << "Troubling opening MC." << endl; 
    }

    hdataphih[is] = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
    can->cd(is+1);
    hdataphih[is]->Draw();

    hgenphih[is] = (TH1F*) tfmc->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
    hgenphih[is]->SetLineColor(kGreen);
    hgenphih[is]->GetYaxis()->SetRangeUser(0, 1.1*hgenphih[is]->GetMaximum());
    hrecphih[is] = (TH1F*) tfmc->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
    hrecphih[is]->SetLineColor(kRed);
    can->cd(nSec+is+1);
    hgenphih[is]->Draw();
    hrecphih[is]->Draw("same");

    haccphih[is] = new TH1F(Form("haccphih_x%i_QQ%i_z%iPT2%i", xBin, QQBin, zBin, PT2Bin), 
			    Form("haccphih_x%i_QQ%i_z%iPT2%i", xBin, QQBin, zBin, PT2Bin), 
			    hgenphih[is]->GetXaxis()->GetNbins(), 
			    hgenphih[is]->GetXaxis()->GetXmin(), 
			    hgenphih[is]->GetXaxis()->GetXmax());
    haccphih[is]->Sumw2();
    haccphih[is]->Divide(hrecphih[is], hgenphih[is]);
    can->cd(2*nSec+is+1);
    haccphih[is]->Draw();

    hcorrphih[is] = new TH1F(Form("hcorrphih_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin), 
			     Form("hcorrphih_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin), 
			     hdataphih[is]->GetXaxis()->GetNbins(), 
			     hdataphih[is]->GetXaxis()->GetXmin(), 
			     hdataphih[is]->GetXaxis()->GetXmax());
    hcorrphih[is]->Sumw2();
    hcorrphih[is]->Divide(hdataphih[is], haccphih[is]);
    if(is == 0){ 
      corrYmax = 1.2*hcorrphih[is]->GetMaximum(); 
    }

    hcorrphih[is]->GetYaxis()->SetRangeUser(-0.3*corrYmax, corrYmax);
    can->cd(3*nSec+is+1);
    hcorrphih[is]->Draw();
    TLine *hline0 = new TLine(-180, 0, 180, 0);
    hline0->Draw();

    ff[is] = new TF1(Form("ff_x%i_QQ%i_z%i_PT2%i", xBin, QQBin, zBin, PT2Bin), 
		     "[0]*(1.0 + [1]*cos((3.1415926/180.0)*x) + [2]*cos(2.0*(3.1415926/180.0)*x))", 
		     hdataphih[is]->GetXaxis()->GetXmin(), 
		     hdataphih[is]->GetXaxis()->GetXmax());
    ff[is]->SetParameters(hcorrphih[is]->GetMaximum(), 0.0, 0.0);

    hcorrphih[is]->Fit(ff[is], "", "", hdataphih[is]->GetXaxis()->GetXmin(), hdataphih[is]->GetXaxis()->GetXmax());

    tlat->DrawLatex(0.13, 0.35, Form("A^{cos#phi} = %3.3f #pm %3.3f", ff[is]->GetParameter(1), ff[is]->GetParError(1)));
    tlat->DrawLatex(0.13, 0.25, Form("A^{cos2#phi} = %3.3f #pm %3.3f", ff[is]->GetParameter(2), ff[is]->GetParError(2)));
    tlat->DrawLatex(0.13, 0.15, Form("M = %3.1f #pm %3.1f", ff[is]->GetParameter(0), ff[is]->GetParError(0)));

    hM->SetBinContent(is+1, ff[is]->GetParameter(0));
    hM->SetBinError(is+1, ff[is]->GetParError(0));
    hAc->SetBinContent(is+1, ff[is]->GetParameter(1));
    hAc->SetBinError(is+1, ff[is]->GetParError(1));
    hAcc->SetBinContent(is+1, ff[is]->GetParameter(2));
    hAcc->SetBinError(is+1, ff[is]->GetParError(2));
  }

  /* 
     TCanvas *sumcan = new TCanvas("sumcan", "sumcan", 10, 10, 1000, 600);
     sumcan->Divide(3, 1, 0.00001, 0.00001);
     sumcan->cd(1);
     hM->Draw();
     sumcan->cd(2);
     hAc->Draw();
     sumcan->cd(3);
     hAcc->Draw();

     hM->GetYaxis()->SetRangeUser(0, 0.8*hcorrphih[0]->GetMaximum());
     hAc->GetYaxis()->SetRangeUser(-0.5, 0.5);
     hAcc->GetYaxis()->SetRangeUser(-0.5, 0.5);

     sumcan->cd(1);
     hM->Draw();
     sumcan->cd(2);
     hAc->Draw();
     sumcan->cd(3);
     hAcc->Draw();
  */

  TFile *rootFile;
  if(doSaveRoot){
    rootFile = new TFile("Sector_systematics.root", "update");
    hM      ->Write();
    hAc     ->Write();
    hAcc    ->Write();
    rootFile->Write();
  }


}
