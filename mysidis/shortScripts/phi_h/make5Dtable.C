void make5Dtable()
{
gStyle->SetOptStat(0);

string pipORpim = "pip";
int accItN = 2; // 0(flat), 1(default haprad), or 2(Nick)
int hapItN = 2; // 0(flat), 1(default haprad), or 2(Nick)

int binSchemeOpt = 5; // careful, some hardcoded values below
int NphihBins;
if(binSchemeOpt == 5) NphihBins = 36;
if(binSchemeOpt == 6) NphihBins = 18;

TFile *tfdata = new TFile(Form("/home/kjgroup/mysidis/histos/data.s1.n11625.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));
TFile *tfmc = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc%i.__0000000000000000__.root", accItN, binSchemeOpt));
TFile *tf_aves = new TFile(Form("/home/kjgroup/mysidis/histos/data_aves.s1.n11625.BiSc%i.MoCo11.__0000000000000000__.root", binSchemeOpt));

ofstream outfile(Form("%s_5Dtable.txt", pipORpim.c_str()));

for(int xBin = 0; xBin < 5; xBin++) { // hardcoded Nbins for now
for(int QQBin = 0; QQBin < 2; QQBin++) {
for(int zBin = 0; zBin < 18; zBin++) {
for(int PT2Bin = 0; PT2Bin < 20; PT2Bin++) {
if(!(xBin == 4 && QQBin == 1))
{
//----------- read in the initial bin category --------------

int category;
ifstream categoryFile;
categoryFile.open(Form("binCategories/%sCategory.BiSc%i.x%iQQ%iz%iPTsq%i.txt", pipORpim.c_str(), binSchemeOpt, xBin, QQBin, zBin, PT2Bin));
if(categoryFile) categoryFile>>category; // if the file exists, get the category value
if(!categoryFile) category = 0; // if the file does not exist, then the bin passes for now (more cuts to come later)
categoryFile.close();

int phihcut = -123;
if(category > 0.5) phihcut = category;

//-------------------- data: --------------------------------

TH1F *hdataphih = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hdataphih->SetName("data"); // so above and below don't have same name (which messes stuff up)
TH1F *hdataphihModified = (TH1F*) tfdata->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));

// apply some modifications
for(int phih = 0; phih < NphihBins; phih++)
{
if(hdataphihModified->GetBinContent(phih+1) < 10) // important condition here! (and more a few lines down)
{
hdataphihModified->SetBinContent(phih+1, 0);
hdataphihModified->SetBinError(phih+1, 0);
}
if(category > 0.5 && fabs(hdataphihModified->GetXaxis()->GetBinCenter(phih+1)) < category)
{
hdataphihModified->SetBinContent(phih+1, 0);
hdataphihModified->SetBinError(phih+1, 0);
}
}

//--------------------- MC: ---------------------------------

TH1F *hgenphih, *hrecphih, *haccphih;

hgenphih = (TH1F*) tfmc->Get(Form("gen_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
hrecphih = (TH1F*) tfmc->Get(Form("rec_%s_phih_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
haccphih = new TH1F(Form("haccphih_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), Form("haccphih_z%iPT2%i", zBin, PT2Bin), NphihBins, -180, 180);
haccphih->Sumw2();
haccphih->Divide(hrecphih, hgenphih);

//------------------ haprad: --------------------------------

TH1F *hsig = new TH1F(Form("hsig_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hsig", NphihBins, -180, 180);
hsig->Sumw2();
TH1F *hsib = new TH1F(Form("hsib_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hsib", NphihBins, -180, 180);
hsib->Sumw2();
TH1F *hsigMtail = new TH1F(Form("hsigMtail_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hsigMtail", NphihBins, -180, 180);
TH1F *htail = new TH1F(Form("htail_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "htail", NphihBins, -180, 180);
TH1F *hRC = new TH1F(Form("hRC_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hRC", NphihBins, -180, 180);
hRC->Sumw2();

string happath;
if(hapItN == 0) happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hap00/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin); // always pip
if(hapItN == 1) happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/hapDefault/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin); // always pip
if(hapItN == 2 && pipORpim == "pip") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPipModel/pip_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin);
if(hapItN == 2 && pipORpim == "pim") happath = Form("/home/kjgroup/mysidis/shortScripts/phi_h/hapradResults/NickPimModel/pim_BiSc%i_x%iQQ%iz%iPT2%i.dat", binSchemeOpt, xBin, QQBin, zBin, PT2Bin);
ifstream hapfile(happath.c_str());

if(hapfile)
{
for(int phih = 0; phih < NphihBins; phih++)
{
float sig, sib, tail;
hapfile>>sig>>sib>>tail;
if(pipORpim == "pim")
{
sig = sig - tail;
tail = 0;
}
hsig->SetBinContent(phih+1, sig);
hsig->SetBinError(phih+1, 0);
hsib->SetBinContent(phih+1, sib);
hsib->SetBinError(phih+1, 0);
hsigMtail->SetBinContent(phih+1, sig-tail);
hsigMtail->SetBinError(phih+1, 0);
htail->SetBinContent(phih+1, tail);
htail->SetBinError(phih+1, 0);
}
}
hapfile.close();

hRC->Divide(hsig, hsib);

//------------------- corr: ---------------------------------

TH1F *hcorr = new TH1F(Form("hcorr_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hcorr", NphihBins, -180, 180);
hcorr->Sumw2();
hcorr->Divide(hdataphihModified, haccphih);

//------------------- corrRC: -------------------------------

TH1F *hcorrRC = new TH1F(Form("hcorrRC_x%iQQ%iz%iPT2%i", xBin, QQBin, zBin, PT2Bin), "hcorrRC", NphihBins, -180, 180);
hcorrRC->Sumw2();
hcorrRC->Divide(hcorr, hRC);

//-------------------- averages: --------------------------------

TH1F *hx = (TH1F*) tf_aves->Get(Form("rec_%s_x_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
float avex = hx->GetMean();
TH1F *hQQ = (TH1F*) tf_aves->Get(Form("rec_%s_QQ_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
float aveQQ = hQQ->GetMean();
TH1F *hz = (TH1F*) tf_aves->Get(Form("rec_%s_z_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
float avez = hz->GetMean();
TH1F *hPT2 = (TH1F*) tf_aves->Get(Form("rec_%s_PT2_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
float avePT2 = hPT2->GetMean();
TH1F *hy = (TH1F*) tf_aves->Get(Form("rec_%s_y_x%i_QQ%i_z%i_PT2%i", pipORpim.c_str(), xBin, QQBin, zBin, PT2Bin));
float avey = hy->GetMean();

//-------------------- save: --------------------------------

for(int iphih = 0; iphih < NphihBins; iphih++)
{
outfile<<xBin<<" "<<QQBin<<" "<<zBin<<" "<<PT2Bin<<" "<<iphih<<" "<<avex<<" "<<aveQQ<<" "<<avez<<" "<<avePT2<<" "<<-175.0+10.0*iphih<<" "<<avey<<" "<<hcorrRC->GetBinContent(iphih+1)<<" "<<hcorrRC->GetBinError(iphih+1)<<" "<<hRC->GetBinContent(iphih+1)<<endl; // hard-coded phih bin width here!
}

cout<<xBin<<" "<<QQBin<<" "<<zBin<<" "<<PT2Bin<<" "<<endl;
}
}}}}
outfile.close();

}
