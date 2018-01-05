// sloppy binning scheme consistency here, everything is for Sc5, be very careful if you change anything

void categorizeBinFiducial_Sc5_v1(int ExpOrSim = 1, int accItN = 0, string pipORpim = "pip", int phihBBin = 0)
{
gStyle->SetOptStat(0);

bool doSaveTxt = 0;
bool doSavePic = 1;
bool docout = 0;

const int NxBins = 5;
const int NQQBins = 2;

const int NzBins = 18;
float zMin = 0.0;
float zMax = 0.9;
float zBW = (zMax - zMin)/NzBins;
const int NPT2Bins = 20;
float PT2Min = 0.0;
float PT2Max = 1.0;
float PT2BW = (PT2Max - PT2Min)/NPT2Bins;
TLine *vLine[NzBins+1];
TLine *hLine[NPT2Bins+1];

TFile *tf;
if(ExpOrSim == 1) tf = new TFile("/home/kjgroup/mysidis/histos/data.s1.n12114.BiSc5.MoCo11.__0000000000000000__.root");
if(ExpOrSim == 0) tf = new TFile(Form("/home/kjgroup/mysidis/histos/MonteCarlo_v12.it%i.s1.n32255.BiSc5.__0000000000000000__.root", accItN));

TCanvas *can = new TCanvas("can", "can", 20, 20, 1550, 950);
can->Divide(NxBins, NQQBins, 0.00001, 0.00001);

TH2F *hPT2Vz[NxBins][NQQBins];

TF1 *f1[NxBins][NQQBins];
TF1 *f2[NxBins][NQQBins];
TF1 *f3[NxBins][NQQBins];
TF1 *f4[NxBins][NQQBins];

for(int x = 0; x < NxBins; x++)
{
for(int QQ = 0; QQ < NQQBins; QQ++)
{
can->cd(x + 1 + NxBins*(NQQBins-1) - NxBins*QQ)->SetLogz();
hPT2Vz[x][QQ] = (TH2F*) tf->Get(Form("rec_%s_PT2vsz_x%iQQ%iphihB%i", pipORpim.c_str(), x, QQ, phihBBin));
if(ExpOrSim == 1) hPT2Vz[x][QQ]->SetTitle(Form("data %s PT2 vs z x%i QQ%i phihB%i", pipORpim.c_str(), x, QQ, phihBBin));
if(ExpOrSim == 0) hPT2Vz[x][QQ]->SetTitle(Form("mc it%i %s PT2 vs z x%i QQ%i phihB%i", accItN, pipORpim.c_str(), x, QQ, phihBBin));
hPT2Vz[x][QQ]->GetYaxis()->SetTitle("PT2 (GeV^{2})");
hPT2Vz[x][QQ]->GetXaxis()->SetTitle("z");
hPT2Vz[x][QQ]->Draw("colz");

	for(int z = 0; z < NzBins+1; z++) {
	vLine[z] = new TLine(z*zBW, PT2Min, z*zBW, PT2Max);
	vLine[z]->SetLineColor(kGray);
	vLine[z]->SetLineWidth(1);
	vLine[z]->Draw();
	}
	for(int PT2 = 0; PT2 < NPT2Bins+1; PT2++) {
	hLine[PT2] = new TLine(zMin, PT2*PT2BW, zMax, PT2*PT2BW);
	hLine[PT2]->SetLineColor(kGray);
	hLine[PT2]->SetLineWidth(1);
	hLine[PT2]->Draw();
	}

// _____ define fiducial regions _____ //

//   if(pipORpim == "pip" && x == 0 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "0.85*x*x - 0.05*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.02*x + 0.00", 0, 1);
//   f2[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) { // it's inefficient to do these loops for each x,QQ bin "if" statement, but necessary since some bins use 1 line or 2 lines or 4 lines
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 0 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.05*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.01*x + 0.00", 0, 1);
//   f2[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 1 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.05*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.70*x*x - 0.10*x + 0.01", 0, 1);
//   f2[x][QQ]->Draw("same");
//   f3[x][QQ] = new TF1(Form("f3_%i%i", x, QQ), "0.50*x*x - 0.10*x + 0.01", 0, 1);
//   f3[x][QQ]->Draw("same");
//   f4[x][QQ] = new TF1(Form("f4_%i%i", x, QQ), "0.05*x + 0.00", 0, 1);
//   f4[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if((PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter)) || (PT2Center < f3[x][QQ]->Eval(zCenter) && PT2Center > f4[x][QQ]->Eval(zCenter)))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 1 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.15*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.04*x + 0.00", 0, 1);
//   f2[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 2 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.15*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.85*x*x - 0.10*x + 0.01", 0, 1);
//   f2[x][QQ]->Draw("same");
//   f3[x][QQ] = new TF1(Form("f3_%i%i", x, QQ), "0.55*x*x - 0.10*x + 0.01", 0, 1);
//   f3[x][QQ]->Draw("same");
//   f4[x][QQ] = new TF1(Form("f4_%i%i", x, QQ), "0.10*x - 0.01", 0, 1);
//   f4[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if((PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter)) || (PT2Center < f3[x][QQ]->Eval(zCenter) && PT2Center > f4[x][QQ]->Eval(zCenter)))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 2 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.25*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.90*x*x - 0.10*x + 0.01", 0, 1);
//   f2[x][QQ]->Draw("same");
//   f3[x][QQ] = new TF1(Form("f3_%i%i", x, QQ), "0.60*x*x - 0.10*x + 0.01", 0, 1);
//   f3[x][QQ]->Draw("same");
//   f4[x][QQ] = new TF1(Form("f4_%i%i", x, QQ), "0.08*x - 0.01", 0, 1);
//   f4[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if((PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter)) || (PT2Center < f3[x][QQ]->Eval(zCenter) && PT2Center > f4[x][QQ]->Eval(zCenter)))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 3 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.40*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.95*x*x - 0.10*x + 0.01", 0, 1);
//   f2[x][QQ]->Draw("same");
//   f3[x][QQ] = new TF1(Form("f3_%i%i", x, QQ), "0.60*x*x - 0.10*x + 0.01", 0, 1);
//   f3[x][QQ]->Draw("same");
//   f4[x][QQ] = new TF1(Form("f4_%i%i", x, QQ), "0.10*x - 0.01", 0, 1);
//   f4[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if((PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter)) || (PT2Center < f3[x][QQ]->Eval(zCenter) && PT2Center > f4[x][QQ]->Eval(zCenter)))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 3 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.45*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.11*x - 0.02", 0, 1);
//   f2[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pip" && x == 4 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "1.50*x*x - 0.10*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   f2[x][QQ] = new TF1(Form("f2_%i%i", x, QQ), "0.10*x - 0.02", 0, 1);
//   f2[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter) && PT2Center > f2[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pip_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 0 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.60*x*x + 0.25*x + 0.02", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 0 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.60*x*x + 0.25*x + 0.02", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 1 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.10*x*x + 0.25*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 1 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.50*x*x + 0.30*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 2 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.10*x*x + 0.25*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 2 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.50*x*x + 0.30*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 3 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "2.50*x*x + 0.30*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 3 && QQ == 1)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "3.00*x*x + 0.30*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }
//   
//   /////////////////////////////////////
//   
//   if(pipORpim == "pim" && x == 4 && QQ == 0)
//   {
//   f1[x][QQ] = new TF1(Form("f1_%i%i", x, QQ), "3.00*x*x + 0.30*x + 0.01", 0, 1);
//   f1[x][QQ]->Draw("same");
//   
//   	for(int z = 0; z < NzBins; z++) {
//   	for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
//   	float zCenter = z*zBW + 0.5*zBW;
//   	float PT2Center = PT2*PT2BW + 0.5*PT2BW;
//   	if(PT2Center < f1[x][QQ]->Eval(zCenter))
//   	{
//   	if(docout) cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" is in fid region"<<endl;
//   	if(doSaveTxt)
//   	{
//   	ofstream outfile(Form("fidcat_pim_BiSc5_x%iQQ%iz%iPT2%i.txt", x, QQ, z, PT2));
//   	outfile<<"this bin is in the fiducial region";
//   	outfile.close();
//   	}
//   	}
//   	}}
//   }

/////////////////////////////////////
}}

/////////////////////////////////////

if(doSavePic && ExpOrSim == 1) can->SaveAs(Form("%s_data_phihB%i_zPT2fid.png", pipORpim.c_str(), phihBBin));
if(doSavePic && ExpOrSim == 0) can->SaveAs(Form("%s_mc_it%i_phihB%i_zPT2fid.png", pipORpim.c_str(), accItN, phihBBin));

}
