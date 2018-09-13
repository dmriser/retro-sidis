{


int hapNumbers[4] = {0, 1, 2, 9};
string hadrons[2] = {"pip", "pim"};

TFile *rootFile = new TFile("MAcAcc.root", "recreate");

for(int ix = 0; ix < 5; ix++) {
for(int iQQ = 0; iQQ < 2; iQQ++) {
if(!(ix == 4 && iQQ == 1)) {
for(int iz = 0; iz < 18; iz++) {
for(int iPT2 = 0; iPT2 < 20; iPT2++) {
for(int iit = 0; iit < 3; iit++) {
for(int ihap : hapNumbers) {
for(string hadron : hadrons) {

	ifstream Mfile(Form("/home/naharrison/MAcAccfiles/%s_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2));
	if(Mfile) {
		float M, Me;
		Mfile>>M>>Me;
		TH1F *hM = new TH1F(Form("%s_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2), Form("%s_M_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2), 1, 0, 1);
		hM->SetBinContent(1, M);
		hM->SetBinError(1, Me);
	}
	Mfile.close();

	ifstream AcAccfile(Form("/home/naharrison/MAcAccfiles/%s_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i.txt", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2));
	if(AcAccfile) {
		float Ac, Ace, Acc, Acce;
		AcAccfile>>Ac>>Ace>>Acc>>Acce;
		TH1F *hAcAcc = new TH1F(Form("%s_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2), Form("%s_AcAcc_it%i_hap%i_BiSc5_x%iQQ%iz%iPT2%i", hadron.c_str(), iit, ihap, ix, iQQ, iz, iPT2), 2, 0, 2);
		hAcAcc->SetBinContent(1, Ac);
		hAcAcc->SetBinError(1, Ace);
		hAcAcc->SetBinContent(2, Acc);
		hAcAcc->SetBinError(2, Acce);
	}
	AcAccfile.close();

}}}}}}}}

rootFile->Write();


}
