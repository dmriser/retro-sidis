{
string mystring = Form("one=%i", 1);
cout<<mystring<<endl;
bool doSave = 0;

TFile *rootfile;

if(doSave)
{
	rootfile = new TFile("testrootfile.root", "update");

	cout<<rootfile->FindKey("abc")<<" "<<rootfile->FindKey("hh")<<endl; // == 0 if not found
}

//TH1F *h = new TH1F("h", "h", 10, 0, 10);
//h->Fill(0.5);

//TH1F *hh = new TH1F("hh", "hh", 10, 0, 10);
//hh->Fill(9.5);

if(doSave)
{
	rootfile->Write();
}

}
