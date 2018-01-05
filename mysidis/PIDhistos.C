#include <fstream>
#include <iostream>
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "programFiles/functions.C"
#include "programFiles/eIDsubroutines.C"
#include "programFiles/vertexCorr.C"
#include "programFiles/pipIDsubroutines.C"
#include "programFiles/pimIDsubroutines.C"
#include "programFiles/protIDsubroutines.C"
#include "MomCorr.C"
#include "programFiles/sctimeCorr.C"

void PIDhistos(int filestart = 1, int Nfiles = 4, int ExpOrSim = 1) // ExpOrSim = 0(MC) or 1(data)
{
TStopwatch *stopwat = new TStopwatch();

int MCversion = 12;

MomCorr_e1f *MomCorr = new MomCorr_e1f();
bool do_momCorr_e = 1; // this is applied AFTER the electron has been identified... data only (even if turned on for MC)

// %%%%% cut values and strictness %%%%%
// values can be: ... loose..., -1, 0 (nominal), 1, ... tight...
int e_zvertex_strict = 0;
int e_ECsampling_strict = 0;
int e_ECoutVin_strict = 0;
int e_ECgeometric_strict = 0;
int e_CCthetaMatching_strict = 0;
int e_R1fid_strict = 0;
int e_R3fid_strict = 0;
int e_CCphiMatching_strict = 0;
int e_CCfiducial_strict = 0;

int pip_vvp_strict = 0;
int pip_R1fid_strict = 0;
int pip_MXcut_strict = 0;

int pim_vvp_strict = 0;
int pim_R1fid_strict = 0;
int pim_MXcut_strict = 0;
// %%%%% end cut values and strictness %%%%%

// %%%%% read the input files into a TChain %%%%%
int NtotalFiles = 11625;
if(ExpOrSim == 0 && MCversion == 3) NtotalFiles = 33471;
if(ExpOrSim == 0 && MCversion == 8) NtotalFiles = 32171;
if(ExpOrSim == 0 && MCversion == 9) NtotalFiles = 1995;
if(ExpOrSim == 0 && MCversion == 10) NtotalFiles = 3950;
if(ExpOrSim == 0 && MCversion == 11) NtotalFiles = 3931;
if(ExpOrSim == 0 && MCversion == 12) NtotalFiles = 32255;

string firstfilename = "";
string lastfilename = "";

ifstream filelist;
if(ExpOrSim == 0) filelist.open(Form("programFiles/v%i_MCFiles.txt", MCversion));
else filelist.open("programFiles/dataFiles.txt");

TChain *h22chain = new TChain("h22");

int kStop = Nfiles + filestart;
if(kStop > NtotalFiles+1) kStop = NtotalFiles+1;
for(int k = 1; k < kStop; k++)
{
string filename;
filelist>>filename;
if(k >= filestart) h22chain->Add(filename.c_str());
if(k == filestart) firstfilename = filename;
if(k == kStop - 1) lastfilename = filename;
}

if(ExpOrSim == 1)
{
firstfilename = firstfilename.substr(36,9); // trim the string down so it's just the run#.subrun# (e.g. 38458.a00)
lastfilename = lastfilename.substr(36,9);
}
cout<<"first file: "<<firstfilename<<" ... last file: "<<lastfilename<<endl;
// %%%%% end read the input files into a TChain %%%%%

// %%%%% define variables %%%%%
Float_t e_mass = 0.000511; // GeV
Float_t prot_mass = 0.938272; // GeV
Float_t pip_mass = 0.13957; // GeV
Float_t pim_mass = 0.13957; // GeV
Float_t speed_of_light = 29.9792458; // cm/ns
Float_t Beam_Energy = 5.498; // GeV
Float_t pi = 3.14159265359;
Float_t pi180 = pi/180.0;
Float_t pi180_inv = 180.0/pi;

Int_t mcnentr;
Int_t mcid[35];
Float_t mctheta[35];
Float_t mcphi[35];
Float_t mcp[35];
Float_t mcvz[35];

Int_t gpart;

Float_t p[35];
Int_t q[35];
Float_t cx[35];
Float_t cy[35];
Float_t cz[35];
Float_t vz[35];
Float_t vy[35];
Float_t vx[35];
Float_t b[35];

UChar_t dc_sect[35];
Float_t tl1_x[35];
Float_t tl1_y[35];
Float_t tl3_x[35]; // used to be dc_xsc in h10
Float_t tl3_y[35];
Float_t tl3_z[35];
Float_t tl3_cx[35]; // used to be dc_cxsc in h10
Float_t tl3_cy[35];
Float_t tl3_cz[35];

UChar_t ec_sect[35];
Float_t etot[35];
Float_t ec_ei[35];
Float_t ec_eo[35];
Float_t ec_t[35];
Float_t ech_x[35];
Float_t ech_y[35];
Float_t ech_z[35];

UChar_t sc_sect[35];
Float_t sc_t[35];
Float_t sc_r[35];
UChar_t sc_pd[35];

UChar_t cc_sect[35];
UShort_t cc_segm[35];
UShort_t nphe[35];
// %%%%% end define variables %%%%%

// %%%%% set branch addresses %%%%%
if(ExpOrSim == 0)
{
h22chain->SetBranchAddress("mcnentr", &mcnentr);
h22chain->SetBranchAddress("mcid", mcid);
h22chain->SetBranchAddress("mctheta", mctheta);
h22chain->SetBranchAddress("mcphi", mcphi);
h22chain->SetBranchAddress("mcp", mcp);
h22chain->SetBranchAddress("mcvz", mcvz);
}

h22chain->SetBranchAddress("gpart", &gpart);
h22chain->SetBranchAddress("p", p);
h22chain->SetBranchAddress("q", q);
h22chain->SetBranchAddress("cx", cx);
h22chain->SetBranchAddress("cy", cy);
h22chain->SetBranchAddress("cz", cz);
h22chain->SetBranchAddress("vz", vz);
h22chain->SetBranchAddress("vy", vy);
h22chain->SetBranchAddress("vx", vx);
h22chain->SetBranchAddress("b", b);

h22chain->SetBranchAddress("dc_sect", dc_sect);
h22chain->SetBranchAddress("tl1_x", tl1_x);
h22chain->SetBranchAddress("tl1_y", tl1_y);
h22chain->SetBranchAddress("tl3_x", tl3_x);
h22chain->SetBranchAddress("tl3_y", tl3_y);
h22chain->SetBranchAddress("tl3_z", tl3_z);
h22chain->SetBranchAddress("tl3_cx", tl3_cx);
h22chain->SetBranchAddress("tl3_cy", tl3_cy);
h22chain->SetBranchAddress("tl3_cz", tl3_cz);

h22chain->SetBranchAddress("ec_sect", ec_sect);
h22chain->SetBranchAddress("etot", etot);
h22chain->SetBranchAddress("ec_ei", ec_ei);
h22chain->SetBranchAddress("ec_eo", ec_eo);
h22chain->SetBranchAddress("ec_t", ec_t);
h22chain->SetBranchAddress("ech_x", ech_x);
h22chain->SetBranchAddress("ech_y", ech_y);
h22chain->SetBranchAddress("ech_z", ech_z);

h22chain->SetBranchAddress("sc_sect", sc_sect);
h22chain->SetBranchAddress("sc_t", sc_t);
h22chain->SetBranchAddress("sc_r", sc_r);
h22chain->SetBranchAddress("sc_pd", sc_pd);

h22chain->SetBranchAddress("cc_sect", cc_sect);
h22chain->SetBranchAddress("cc_segm", cc_segm);
h22chain->SetBranchAddress("nphe", nphe);
// %%%%% end set branch addresses %%%%%

// %%%%% stuff for output root files %%%%%
string outfilename;
TFile *outputfile;
if(ExpOrSim == 1) outfilename = Form("pid.data.s%i.n%i.root", filestart, Nfiles);
if(ExpOrSim == 0) outfilename = Form("pid.MonteCarlo_v%i.s%i.n%i.root", MCversion, filestart, Nfiles);
cout<<"output filename is "<<outfilename<<endl;
outputfile = new TFile(outfilename.c_str(),"recreate");
// %%%%% end stuff for output root files %%%%%

// %%%%% define histograms %%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%

outputfile->mkdir("eIDplots");
outputfile->cd("eIDplots");

TH1F *vz_e_hist[3][6];
TH1F *corrvz_e_hist[3][6];
TH2F *etotVp_e_hist[3][6];
TH2F *ec_eoVec_ei_e_hist[3][6];
TH2F *ech_xVech_y_e_hist[3][6];
TH2F *thetaCCVcc_seg_e_hist[3][6];
TH2F *tl1_xVtl1_y_e_hist[3][6];
TH2F *tl3_xVtl3_y_e_hist[3][6];
TH1F *ccphimatching_e_hist[3][6];
TH2F *CCfid_e_hist[3][6];

for(int s = 0; s < 6; s++)
{
for(int i = 0; i < 3; i++)
{
vz_e_hist[i][s] = new TH1F(Form("vz_e_hist_c%i_s%i", i, s+1), Form("vz_e_hist_c%i_s%i", i, s+1), 300, -32, 2);
corrvz_e_hist[i][s] = new TH1F(Form("corrvz_e_hist_c%i_s%i", i, s+1), Form("corrvz_e_hist_c%i_s%i", i, s+1), 300, -32, 2);
etotVp_e_hist[i][s] = new TH2F(Form("etotVp_e_hist_c%i_s%i", i, s+1), Form("etotVp_e_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, 0, 0.65);
ec_eoVec_ei_e_hist[i][s] = new TH2F(Form("ec_eoVec_ei_e_hist_c%i_s%i", i, s+1), Form("ec_eoVec_ei_e_hist_c%i_s%i", i, s+1), 300, 0, 0.4, 300, 0, 0.4);
ech_xVech_y_e_hist[i][s] = new TH2F(Form("ech_xVech_y_e_hist_c%i_s%i", i, s+1), Form("ech_xVech_y_e_hist_c%i_s%i", i, s+1), 300, -250, 250, 300, 0, 450);
thetaCCVcc_seg_e_hist[i][s] = new TH2F(Form("thetaCCVcc_seg_e_hist_c%i_s%i", i, s+1), Form("thetaCCVcc_seg_e_hist_c%i_s%i", i, s+1), 18, 1, 19, 120, 0, 60);
tl1_xVtl1_y_e_hist[i][s] = new TH2F(Form("tl1_xVtl1_y_e_hist_c%i_s%i", i, s+1), Form("tl1_xVtl1_y_e_hist_c%i_s%i", i, s+1), 300, -50, 50, 300, 10, 60);
tl3_xVtl3_y_e_hist[i][s] = new TH2F(Form("tl3_xVtl3_y_e_hist_c%i_s%i", i, s+1), Form("tl3_xVtl3_y_e_hist_c%i_s%i", i, s+1), 300, -300, 300, 300, 20, 400);
ccphimatching_e_hist[i][s] = new TH1F(Form("ccphimatching_e_hist_c%i_s%i", i, s+1), Form("ccphimatching_e_hist_c%i_s%i", i, s+1), 5, -2, 3);
CCfid_e_hist[i][s] = new TH2F(Form("CCfid_e_hist_c%i_s%i", i, s+1), Form("CCfid_e_hist_c%i_s%i", i, s+1), 400, -30, 30, 400, 0, 60);
}
}

// additional ones
TH1F *nphe_e_hist[2][6];

for(int s = 0; s < 6; s++)
{
for(int i = 0; i < 2; i++)
{
nphe_e_hist[i][s] = new TH1F(Form("nphe_e_hist_c%i_s%i", i, s+1), Form("nphe_e_hist_c%i_s%i", i, s+1), 200, 0, 200);
}
}

	// pip ID

	outputfile->mkdir("pipIDplots");
	outputfile->cd("pipIDplots");

	TH2F *vvp_pip_hist[3][6];
	TH2F *vvp_pip_esect[3][6];
	TH2F *tl1_xVtl1_y_pip_hist[3][6];
	TH1F *MX_epipX_hist[3][6];

	for(int s = 0; s < 6; s++)
	{
	for(int i = 0; i < 3; i++)
	{
	vvp_pip_hist[i][s] = new TH2F(Form("vvp_pip_hist_c%i_s%i", i, s+1), Form("vvp_pip_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, 0.3, 1.1);
	vvp_pip_esect[i][s] = new TH2F(Form("vvp_pip_esect_c%i_s%i", i, s+1), Form("vvp_pip_esect_c%i_s%i", i, s+1), 300, 0, 5, 300, 0.3, 1.1);
	tl1_xVtl1_y_pip_hist[i][s] = new TH2F(Form("tl1_xVtl1_y_pip_hist_c%i_s%i", i, s+1), Form("tl1_xVtl1_y_pip_hist_c%i_s%i", i, s+1), 300, -50, 50, 300, 10, 60);
	MX_epipX_hist[i][s] = new TH1F(Form("MX_epipX_hist_c%i_s%i", i, s+1), Form("MX_epipX_hist_c%i_s%i", i, s+1), 300, 0.5, 4);
	}
	}

	// additional ones
	TH2F *tl3_xVtl3_y_pip_hist[2][6];
	TH2F *DtVp_pip_hist[2][6];
	TH2F *DtVp_pip_esect[2][6];
	TH2F *thetaVphi_pip_hist[2][6];

	for(int s = 0; s < 6; s++)
	{
	for(int i = 0; i < 2; i++)
	{
	tl3_xVtl3_y_pip_hist[i][s] = new TH2F(Form("tl3_xVtl3_y_pip_hist_c%i_s%i", i, s+1), Form("tl3_xVtl3_y_pip_hist_c%i_s%i", i, s+1), 300, -300, 300, 300, 20, 500);
	DtVp_pip_hist[i][s] = new TH2F(Form("DtVp_pip_hist_c%i_s%i", i, s+1), Form("DtVp_pip_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, -5, 5);
	DtVp_pip_esect[i][s] = new TH2F(Form("DtVp_pip_esect_c%i_s%i", i, s+1), Form("DtVp_pip_esect_c%i_s%i", i, s+1), 300, 0, 5, 300, -5, 5);
	thetaVphi_pip_hist[i][s] = new TH2F(Form("thetaVphi_pip_hist_c%i_s%i", i, s+1), Form("thetaVphi_pip_hist_c%i_s%i", i, s+1), 300, -35, 35, 300, 0, 100);
	}
	}

	TH2F *vvp_pip_esect_notCorr[6]; // no time correction
	for(int s = 0; s < 6; s++)
	{
	vvp_pip_esect_notCorr[s] = new TH2F(Form("vvp_pip_esect_notCorr_s%i", s+1), Form("vvp_pip_esect_notCorr_s%i", s+1), 300, 0, 5, 300, 0.3, 1.1);
	}

// pim ID

outputfile->mkdir("pimIDplots");
outputfile->cd("pimIDplots");

TH2F *vvp_pim_hist[3][6];
TH2F *vvp_pim_esect[3][6];
TH2F *tl1_xVtl1_y_pim_hist[3][6];
TH1F *MX_epimX_hist[3][6];

for(int s = 0; s < 6; s++)
{
for(int i = 0; i < 3; i++)
{
vvp_pim_hist[i][s] = new TH2F(Form("vvp_pim_hist_c%i_s%i", i, s+1), Form("vvp_pim_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, 0.3, 1.1);
vvp_pim_esect[i][s] = new TH2F(Form("vvp_pim_esect_c%i_s%i", i, s+1), Form("vvp_pim_esect_c%i_s%i", i, s+1), 300, 0, 5, 300, 0.3, 1.1);
tl1_xVtl1_y_pim_hist[i][s] = new TH2F(Form("tl1_xVtl1_y_pim_hist_c%i_s%i", i, s+1), Form("tl1_xVtl1_y_pim_hist_c%i_s%i", i, s+1), 300, -50, 50, 300, 10, 60);
MX_epimX_hist[i][s] = new TH1F(Form("MX_epimX_hist_c%i_s%i", i, s+1), Form("MX_epimX_hist_c%i_s%i", i, s+1), 300, 0.5, 4);
}
}

// additional ones
TH2F *tl3_xVtl3_y_pim_hist[2][6];
TH2F *DtVp_pim_hist[2][6];
TH2F *DtVp_pim_esect[2][6];

for(int s = 0; s < 6; s++)
{
for(int i = 0; i < 2; i++)
{
tl3_xVtl3_y_pim_hist[i][s] = new TH2F(Form("tl3_xVtl3_y_pim_hist_c%i_s%i", i, s+1), Form("tl3_xVtl3_y_pim_hist_c%i_s%i", i, s+1), 300, -300, 300, 300, 20, 500);
DtVp_pim_hist[i][s] = new TH2F(Form("DtVp_pim_hist_c%i_s%i", i, s+1), Form("DtVp_pim_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, -5, 5);
DtVp_pim_esect[i][s] = new TH2F(Form("DtVp_pim_esect_c%i_s%i", i, s+1), Form("DtVp_pim_esect_c%i_s%i", i, s+1), 300, 0, 5, 300, -5, 5);
}
}

TH2F *vvp_pim_esect_notCorr[6]; // no time correction
for(int s = 0; s < 6; s++)
{
vvp_pim_esect_notCorr[s] = new TH2F(Form("vvp_pim_esect_notCorr_s%i", s+1), Form("vvp_pim_esect_notCorr_s%i", s+1), 300, 0, 5, 300, 0.3, 1.1);
}

	// prot ID

	outputfile->mkdir("protIDplots");
	outputfile->cd("protIDplots");
	
	TH2F *DtVp_prot_hist[3][6];

	for(int s = 0; s < 6; s++)
	{
	for(int i = 0; i < 3; i++)
	{
	DtVp_prot_hist[i][s] = new TH2F(Form("DtVp_prot_hist_c%i_s%i", i, s+1), Form("DtVp_prot_hist_c%i_s%i", i, s+1), 300, 0, 5, 300, -5, 5);
	}
	}

// %%%%% end define histograms %%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// %%%%% POTATOES %%%%%
// %%%%%%%%%%%%%%%%%%%%

cout<<"entries: "<<h22chain->GetEntries()/1000<<" thousand"<<endl;

for(int i = 0; i < h22chain->GetEntries(); i++) // loop over entries
{
h22chain->GetEntry(i);

bool genExclusiveEvent = 0; // if the generated event is an exclusive event, i.e. ep --> e pi+ n, we don't want to analyze it at all (the generator give unrealistic kinematics)
if(ExpOrSim == 0 && mcnentr == 3)
{
if((mcid[1] == 211 && mcid[2] == 2112) || (mcid[1] == 2112 && mcid[2] == 211)) genExclusiveEvent = 1; // the first (zeroth) generated particle is always the scattered electron
}

if(genExclusiveEvent == 0)
{

string currentrunno_string = "-123";
if(ExpOrSim == 1)
{
currentrunno_string = h22chain->GetCurrentFile()->GetName();
currentrunno_string = currentrunno_string.substr(36,5);
}
int currentrunno = atoi(currentrunno_string.c_str()); // converts the string to an int

// %%%%% begin eID part %%%%% //
vector<int> Veindex;
int e_index = -123;
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == -1 && cc_sect[k] != 0 && sc_sect[k] != 0 && ec_sect[k] != 0 && dc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
Float_t phi = atan3(cy[k],cx[k])*57.2957795;
Float_t relphi = get_rel_phi2(shift180180to30330(atan2(cy[k],cx[k])*57.2957795), dc_sect[k]);
Float_t thetaCC = 57.2957795*get_thetaCC(tl3_x[k], tl3_y[k], tl3_z[k], tl3_cx[k], tl3_cy[k], tl3_cz[k]);
Float_t rot_tl1_x = tl1_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + tl1_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
Float_t rot_tl1_y = tl1_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - tl1_x[k]*sin((dc_sect[k]-1)*pi180*60.0);
Float_t rot_ech_x = ech_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + ech_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
Float_t rot_ech_y = ech_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - ech_x[k]*sin((dc_sect[k]-1)*pi180*60.0);
Float_t corrvz = getCorrZ(ExpOrSim, vx[k], vy[k], vz[k], p[k]*cx[k], p[k]*cy[k], p[k]*cz[k], dc_sect[k]);

// fill "no cuts" histos:
vz_e_hist[0][dc_sect[k]-1]->Fill(vz[k]);
corrvz_e_hist[0][dc_sect[k]-1]->Fill(corrvz);
etotVp_e_hist[0][dc_sect[k]-1]->Fill(p[k],etot[k]/p[k]);
ec_eoVec_ei_e_hist[0][dc_sect[k]-1]->Fill(ec_ei[k],ec_eo[k]);
ech_xVech_y_e_hist[0][dc_sect[k]-1]->Fill(rot_ech_y, rot_ech_x);
thetaCCVcc_seg_e_hist[0][dc_sect[k]-1]->Fill((cc_segm[k]%1000)/10,thetaCC);
tl1_xVtl1_y_e_hist[0][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
tl3_xVtl3_y_e_hist[0][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
ccphimatching_e_hist[0][dc_sect[k]-1]->Fill(ccphimatching(cc_segm[k], phi));
CCfid_e_hist[0][dc_sect[k]-1]->Fill(relphi, thetaCC);

nphe_e_hist[0][dc_sect[k]-1]->Fill(nphe[k]);

// check if particle passes eID cuts:
Bool_t zvertex_pass, ECsampling_pass, ECoutVin_pass, ECgeometric_pass, CCthetaMatching_pass, R1fid_pass, R3fid_pass, CCphiMatching_pass, CCfiducial_pass;
zvertex_pass = ECsampling_pass = ECoutVin_pass = ECgeometric_pass = CCthetaMatching_pass = R1fid_pass = R3fid_pass = CCphiMatching_pass = CCfiducial_pass = 0;

zvertex_pass = e_zvertex_pass(e_zvertex_strict, ExpOrSim, getCorrZ(ExpOrSim, vx[k], vy[k], vz[k], p[k]*cx[k], p[k]*cy[k], p[k]*cz[k], dc_sect[k]));
ECsampling_pass = e_ECsampling_pass(e_ECsampling_strict, ExpOrSim, dc_sect[k], etot[k], p[k]);
ECoutVin_pass = e_ECoutVin_pass(e_ECoutVin_strict, ec_ei[k]);
ECgeometric_pass = e_ECgeometric_pass(e_ECgeometric_strict, ech_x[k], ech_y[k], ech_z[k]);
CCthetaMatching_pass = e_CCthetaMatching_pass(e_CCthetaMatching_strict, ExpOrSim, dc_sect[k], thetaCC, (cc_segm[k]%1000)/10);
R1fid_pass = e_R1fid_pass(e_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]);
R3fid_pass = e_R3fid_pass(e_R3fid_strict, dc_sect[k], tl3_x[k], tl3_y[k]);
CCphiMatching_pass = e_CCphiMatching_pass(e_CCphiMatching_strict, ccphimatching(cc_segm[k], phi));
CCfiducial_pass = e_CCfiducial_pass(e_CCfiducial_strict, thetaCC, relphi);

// fill "other cuts" histos:
if(     1       && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) vz_e_hist[1][dc_sect[k]-1]->Fill(vz[k]);
if(     1       && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) corrvz_e_hist[1][dc_sect[k]-1]->Fill(corrvz);
if(zvertex_pass &&       1         && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) etotVp_e_hist[1][dc_sect[k]-1]->Fill(p[k],etot[k]/p[k]);
if(zvertex_pass && ECsampling_pass &&        1      && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) ec_eoVec_ei_e_hist[1][dc_sect[k]-1]->Fill(ec_ei[k],ec_eo[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass &&         1        && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) ech_xVech_y_e_hist[1][dc_sect[k]-1]->Fill(rot_ech_y, rot_ech_x);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass &&           1          && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) thetaCCVcc_seg_e_hist[1][dc_sect[k]-1]->Fill((cc_segm[k]%1000)/10,thetaCC);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass &&     1      && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) tl1_xVtl1_y_e_hist[1][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass &&     1      && CCphiMatching_pass && CCfiducial_pass) tl3_xVtl3_y_e_hist[1][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass &&         1          && CCfiducial_pass) ccphimatching_e_hist[1][dc_sect[k]-1]->Fill(ccphimatching(cc_segm[k], phi));
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass &&        1       ) CCfid_e_hist[1][dc_sect[k]-1]->Fill(relphi, thetaCC);

// fill "all cuts" histos:
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass)
{
vz_e_hist[2][dc_sect[k]-1]->Fill(vz[k]);
corrvz_e_hist[2][dc_sect[k]-1]->Fill(corrvz);
etotVp_e_hist[2][dc_sect[k]-1]->Fill(p[k],etot[k]/p[k]);
ec_eoVec_ei_e_hist[2][dc_sect[k]-1]->Fill(ec_ei[k],ec_eo[k]);
ech_xVech_y_e_hist[2][dc_sect[k]-1]->Fill(rot_ech_y, rot_ech_x);
thetaCCVcc_seg_e_hist[2][dc_sect[k]-1]->Fill((cc_segm[k]%1000)/10,thetaCC);
tl1_xVtl1_y_e_hist[2][dc_sect[k]-1]->Fill(tl1_y[k]*cos((dc_sect[k]-1)*(3.14159/180.0)*60.0)-tl1_x[k]*sin((dc_sect[k]-1)*(3.14159/180.0)*60.0), tl1_y[k]*sin((dc_sect[k]-1)*(3.14159/180.0)*60.0)+tl1_x[k]*cos((dc_sect[k]-1)*(3.14159/180.0)*60.0));
tl3_xVtl3_y_e_hist[2][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
ccphimatching_e_hist[2][dc_sect[k]-1]->Fill(ccphimatching(cc_segm[k], phi));
CCfid_e_hist[2][dc_sect[k]-1]->Fill(relphi, thetaCC);

nphe_e_hist[1][dc_sect[k]-1]->Fill(nphe[k]);

Veindex.push_back(k);
}

} // end if(q[k] == -1 && cc_sect[k] != 0 && sc_sect[k] != 0 && ec_sect[k] != 0 && dc_sect[k] != 0)
} // end loop over gpart

// find highest momentum electron:
if(Veindex.size() > 0) e_index = Veindex[0];
if(Veindex.size() > 1)
{
for(unsigned int v = 1; v < Veindex.size(); v++)
{
if(p[Veindex[v]] > p[e_index]) e_index = Veindex[v];
}
}
// %%%%% end eID part %%%%% //

TLorentzVector V4k(0.0, 0.0, Beam_Energy, Beam_Energy); // x, y, z, t
TLorentzVector V4ISproton(0.0, 0.0, 0.0, prot_mass); // IS = Initial State
TVector3 V3_e;
TLorentzVector V4_e, V4_q, V4_H;
float W, QQ, x, y;
W = QQ = x = y = 0;

if(e_index > -1)
{
V3_e.SetXYZ(p[e_index]*cx[e_index], p[e_index]*cy[e_index], p[e_index]*cz[e_index]);
V4_e.SetXYZT(V3_e.X(), V3_e.Y(), V3_e.Z(), sqrt(V3_e.Mag2() + pow(e_mass,2)));
if(do_momCorr_e && ExpOrSim == 1) V4_e = MomCorr->PcorN(V4_e, -1, 11);

V4_q = V4k - V4_e;
V4_H = V4_q + V4ISproton;

W = V4_H.Mag();
QQ = -V4_q*V4_q;
x = QQ/(2.0*V4_q.T()*prot_mass);
y = V4_q.T()/Beam_Energy;
}

// %%%%% begin pipID part %%%%% //
if(e_index > -1 && W > 2.0 && QQ > 1.0 && y < 0.8)
{
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == 1 && dc_sect[k] != 0 && sc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
float corrStartTime = e_sctimeCorr(ExpOrSim, sc_t[e_index], dc_sect[e_index], sc_pd[e_index], currentrunno) - (sc_r[e_index]/speed_of_light);
float ehtcorrMeasuredTime = h_sctimeCorr(ExpOrSim, sc_t[k], dc_sect[k], sc_pd[k], currentrunno) - corrStartTime;
float CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(pip_mass,2));
float ehtcorrDt = ehtcorrMeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);
float velocity = (sc_r[k]/ehtcorrMeasuredTime)/speed_of_light;
float rot_tl1_x = tl1_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + tl1_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
float rot_tl1_y = tl1_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - tl1_x[k]*sin((dc_sect[k]-1)*pi180*60.0);

TVector3 V3_pip;
V3_pip.SetXYZ(p[k]*cx[k], p[k]*cy[k], p[k]*cz[k]);
TLorentzVector V4_pip;
V4_pip.SetXYZT(V3_pip.X(), V3_pip.Y(), V3_pip.Z(), sqrt(V3_pip.Mag2() + pow(pip_mass,2))); // pip candidate, assuming pip mass
TLorentzVector V4_pip_prime = V4_pip;
V4_pip_prime.RotateZ(-1.0*V4_q.Phi() - pi);
V4_pip_prime.RotateY(V4_q.Theta());
float z = V4_pip.T()/V4_q.T();
float PT = V4_pip_prime.Perp();
TLorentzVector V4_X_epipX = V4_H - V4_pip;

// fill "no cuts" histos:
vvp_pip_hist[0][dc_sect[k]-1]->Fill(p[k], velocity);
vvp_pip_esect[0][dc_sect[e_index]-1]->Fill(p[k], velocity);
tl1_xVtl1_y_pip_hist[0][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
MX_epipX_hist[0][dc_sect[k]-1]->Fill(V4_X_epipX.Mag());

tl3_xVtl3_y_pip_hist[0][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
DtVp_pip_hist[0][dc_sect[k]-1]->Fill(p[k], ehtcorrDt);
DtVp_pip_esect[0][dc_sect[e_index]-1]->Fill(p[k], ehtcorrDt);
thetaVphi_pip_hist[0][dc_sect[k]-1]->Fill(get_rel_phi2(shift180180to30330(V3_pip.Phi()*pi180_inv), dc_sect[k]), V3_pip.Theta()*pi180_inv);

vvp_pip_esect_notCorr[dc_sect[e_index]-1]->Fill(p[k], (sc_r[k]/(sc_t[k] - sc_t[e_index] + sc_r[e_index]/speed_of_light))/speed_of_light);

// check if particle passes pipID cuts:
bool vvp_pass, R1fid_pass, MXcut_pass;
vvp_pass = R1fid_pass = MXcut_pass = 0;

vvp_pass = pip_vvp_pass(pip_vvp_strict, ExpOrSim, dc_sect[e_index], velocity, p[k]);
R1fid_pass = pip_R1fid_pass(pip_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]);
MXcut_pass = pip_MXcut_pass(pip_MXcut_strict, V4_X_epipX.Mag());

// fill "other cuts" histos:
if(    1    && R1fid_pass && MXcut_pass) vvp_pip_hist[1][dc_sect[k]-1]->Fill(p[k], velocity);
if(    1    && R1fid_pass && MXcut_pass) vvp_pip_esect[1][dc_sect[e_index]-1]->Fill(p[k], velocity);
if(vvp_pass &&     1      && MXcut_pass) tl1_xVtl1_y_pip_hist[1][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
if(vvp_pass && R1fid_pass &&     1     ) MX_epipX_hist[1][dc_sect[k]-1]->Fill(V4_X_epipX.Mag());

// fill "all cuts" histos:
if(vvp_pass && R1fid_pass && MXcut_pass)
{
vvp_pip_hist[2][dc_sect[k]-1]->Fill(p[k], velocity);
vvp_pip_esect[2][dc_sect[e_index]-1]->Fill(p[k], velocity);
tl1_xVtl1_y_pip_hist[2][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
MX_epipX_hist[2][dc_sect[k]-1]->Fill(V4_X_epipX.Mag());

tl3_xVtl3_y_pip_hist[1][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
DtVp_pip_hist[1][dc_sect[k]-1]->Fill(p[k], ehtcorrDt);
DtVp_pip_esect[1][dc_sect[e_index]-1]->Fill(p[k], ehtcorrDt);
thetaVphi_pip_hist[1][dc_sect[k]-1]->Fill(get_rel_phi2(shift180180to30330(V3_pip.Phi()*pi180_inv), dc_sect[k]), V3_pip.Theta()*pi180_inv);
}

}
}
}
// %%%%% end pipID part %%%%% //

// %%%%% begin pimID part %%%%% // (a bit inefficient to do the loop twice, but this is simpler and time isn't an issue here)
if(e_index > -1 && W > 2.0 && QQ > 1.0 && y < 0.8)
{
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == -1 && dc_sect[k] != 0 && sc_sect[k] != 0 && k != e_index && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
float corrStartTime = e_sctimeCorr(ExpOrSim, sc_t[e_index], dc_sect[e_index], sc_pd[e_index], currentrunno) - (sc_r[e_index]/speed_of_light);
float ehtcorrMeasuredTime = h_sctimeCorr(ExpOrSim, sc_t[k], dc_sect[k], sc_pd[k], currentrunno) - corrStartTime;
float CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(pim_mass,2));
float ehtcorrDt = ehtcorrMeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);
float velocity = (sc_r[k]/ehtcorrMeasuredTime)/speed_of_light;
float rot_tl1_x = tl1_y[k]*sin((dc_sect[k]-1)*pi180*60.0) + tl1_x[k]*cos((dc_sect[k]-1)*pi180*60.0);
float rot_tl1_y = tl1_y[k]*cos((dc_sect[k]-1)*pi180*60.0) - tl1_x[k]*sin((dc_sect[k]-1)*pi180*60.0);

TVector3 V3_pim;
V3_pim.SetXYZ(p[k]*cx[k], p[k]*cy[k], p[k]*cz[k]);
TLorentzVector V4_pim;
V4_pim.SetXYZT(V3_pim.X(), V3_pim.Y(), V3_pim.Z(), sqrt(V3_pim.Mag2() + pow(pim_mass,2))); // pim candidate, assuming pim mass
TLorentzVector V4_pim_prime = V4_pim;
V4_pim_prime.RotateZ(-1.0*V4_q.Phi() - pi);
V4_pim_prime.RotateY(V4_q.Theta());
float z = V4_pim.T()/V4_q.T();
float PT = V4_pim_prime.Perp();
TLorentzVector V4_X_epimX = V4_H - V4_pim;

// fill "no cuts" histos:
vvp_pim_hist[0][dc_sect[k]-1]->Fill(p[k], velocity);
vvp_pim_esect[0][dc_sect[e_index]-1]->Fill(p[k], velocity);
tl1_xVtl1_y_pim_hist[0][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
MX_epimX_hist[0][dc_sect[k]-1]->Fill(V4_X_epimX.Mag());

tl3_xVtl3_y_pim_hist[0][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
DtVp_pim_hist[0][dc_sect[k]-1]->Fill(p[k], ehtcorrDt);
DtVp_pim_esect[0][dc_sect[e_index]-1]->Fill(p[k], ehtcorrDt);
vvp_pim_esect_notCorr[dc_sect[e_index]-1]->Fill(p[k], (sc_r[k]/(sc_t[k] - sc_t[e_index] + sc_r[e_index]/speed_of_light))/speed_of_light);

// check if particle passes pimID cuts:
bool vvp_pass, R1fid_pass, MXcut_pass;
vvp_pass = R1fid_pass = MXcut_pass = 0;

vvp_pass = pim_vvp_pass(pim_vvp_strict, ExpOrSim, dc_sect[e_index], velocity, p[k]);
R1fid_pass = pim_R1fid_pass(pim_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]);
MXcut_pass = pim_MXcut_pass(pim_MXcut_strict, V4_X_epimX.Mag());

// fill "other cuts" histos:
if(    1    && R1fid_pass && MXcut_pass) vvp_pim_hist[1][dc_sect[k]-1]->Fill(p[k], velocity);
if(    1    && R1fid_pass && MXcut_pass) vvp_pim_esect[1][dc_sect[e_index]-1]->Fill(p[k], velocity);
if(vvp_pass &&     1      && MXcut_pass) tl1_xVtl1_y_pim_hist[1][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
if(vvp_pass && R1fid_pass &&     1     ) MX_epimX_hist[1][dc_sect[k]-1]->Fill(V4_X_epimX.Mag());

// fill "all cuts" histos:
if(vvp_pass && R1fid_pass && MXcut_pass)
{
vvp_pim_hist[2][dc_sect[k]-1]->Fill(p[k], velocity);
vvp_pim_esect[2][dc_sect[e_index]-1]->Fill(p[k], velocity);
tl1_xVtl1_y_pim_hist[2][dc_sect[k]-1]->Fill(rot_tl1_y, rot_tl1_x);
MX_epimX_hist[2][dc_sect[k]-1]->Fill(V4_X_epimX.Mag());

tl3_xVtl3_y_pim_hist[1][dc_sect[k]-1]->Fill(tl3_y[k], tl3_x[k]);
DtVp_pim_hist[1][dc_sect[k]-1]->Fill(p[k], ehtcorrDt);
DtVp_pim_esect[1][dc_sect[e_index]-1]->Fill(p[k], ehtcorrDt);
}

}
}
}
// %%%%% end pimID part %%%%% //

// %%%%% begin protID part %%%%% // (a bit inefficient to do the loop again, but this is simpler and time isn't an issue here)
if(e_index > -1 && W > 2.0 && QQ > 1.0 && y < 0.8)
{
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == 1 && dc_sect[k] != 0 && sc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
Float_t StartTime = sc_t[e_index] - (sc_r[e_index]/speed_of_light);
Float_t MeasuredTime = sc_t[k] - StartTime;
Float_t CalcBeta = p[k]/sqrt(pow(p[k],2) + pow(prot_mass,2));
Float_t Dt = MeasuredTime - sc_r[k]/(speed_of_light*CalcBeta);

// fill "no cuts" histos:
DtVp_prot_hist[0][dc_sect[k]-1]->Fill(p[k], Dt);

// check if particle passes protID cuts:
bool DtVp_pass;
DtVp_pass = 0;

DtVp_pass = prot_DtVp_pass(Dt);

// fill "other cuts" histos:

// fill "all cuts" histos:
if(DtVp_pass)
{
DtVp_prot_hist[2][dc_sect[k]-1]->Fill(p[k], Dt);
}

}
}
}
// %%%%% end protID part %%%%% //

if(i%1000 == 0) cout<<"\ranalyzed "<<i/1000<<" thousand events"<<flush;
} // end if(genExclusiveEvent == 0)
} // end of loop over entries
// %%%%%%%%%%%%%%%%%%%%%%%%
// %%%%% END POTATOES %%%%%

outputfile->Write();
cout<<endl<<"Done!"<<endl;
stopwat->Print();

} // end of program
