void BCCfactor_v2(int zBin = 4, int PT2Bin = 4) // these refer to the bin number for bin scheme 5.
{
/* NOTE: the indicies in the file names start at 1 not 0.
 *
 * first, need to get all of the files that correspond to zBin, PT2Bin
 * each z bin and each PT2 bin are divided into 5, so need to open 5*5 = 25 files
 */

gROOT->ProcessLine(".L /home/kjgroup/mysidis/programFiles/functions.C");

string pipORpim = "pim";

int NzSubdivisions = 5; // this means each normal z bin is divided into 5 sub-bins
int NPT2Subdivisions = 5;

const int NxBins = 5;
int NxSubbins = 50;
float xMin = 0.1;
float xMax = 0.6;
float xSubbinW = 0.5/NxSubbins; // W = width
const int NQQBins = 2;
int NQQSubbins = 50;
float QQMin = 1.0;
float QQSubbinW = 4.0/NQQSubbins;
const int NphihBins = 36;
int NphihSubbins = 180;
float phihMin = -180.0;
float phihMax = 180.0;
float phihSubbinW = 360.0/NphihSubbins;

float summedBornCS[NxBins][NQQBins][NphihBins];
int NnonzeroSubbins[NxBins][NQQBins][NphihBins];
float centerSubbinValue[NxBins][NQQBins][NphihBins];

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {
for(int iphih = 0; iphih < NphihBins; iphih++) {
summedBornCS[ix][iQQ][iphih] = 0; // initialize to zero
NnonzeroSubbins[ix][iQQ][iphih] = 0;
centerSubbinValue[ix][iQQ][iphih] = 0;
}}}

int zSubIndexStart = NzSubdivisions*zBin + 1;
int PT2SubIndexStart = NPT2Subdivisions*PT2Bin + 1;

for(int iz = zSubIndexStart; iz < zSubIndexStart + NzSubdivisions; iz++) {
for(int iPT2 = PT2SubIndexStart; iPT2 < PT2SubIndexStart + NPT2Subdivisions; iPT2++) {
gSystem->Exec(Form("tar -zxvf datfiles/go_%s_%i_%i.dat.tar.gz --transform \"s/test/test_%s_%i_%i/\"", pipORpim.c_str(), iz, iPT2, pipORpim.c_str(), iz, iPT2));

// now open the file, loop over the entries, and read in the Born CS
ifstream datfile(Form("test_%s_%i_%i.dat", pipORpim.c_str(), iz, iPT2));
for(int ix = 0; ix < NxSubbins; ix++) {
for(int iQQ = 0; iQQ < NQQSubbins; iQQ++) {
for(int iphih = 0; iphih < NphihSubbins; iphih++) {

float sib;
datfile>>sib;

float xSubbinCenterValue = xMin + ix*xSubbinW + 0.5*xSubbinW;
float QQSubbinCenterValue = QQMin + iQQ*QQSubbinW + 0.5*QQSubbinW;
float y = QQSubbinCenterValue/(2.0*5.498*0.938*xSubbinCenterValue); // kinematic variable y
float W = sqrt(QQSubbinCenterValue*((1.0/xSubbinCenterValue) - 1.0) + 0.938*0.938); // kinematic variable W
float phihSubbinCenterValue = phihMin + iphih*phihSubbinW + 0.5*phihSubbinW;

if(sib > 0 && y < 0.85 && W > 2.05)
{
int xBin = getBinN2(xSubbinCenterValue, NxBins, xMin, xMax);
int QQBin;
if(xBin == 0) QQBin = getBinN2(QQSubbinCenterValue, NQQBins, 1.3 - 10, 1.3 + 10); // 10 is completely arbitrary, but should be a big number
if(xBin == 1) QQBin = getBinN2(QQSubbinCenterValue, NQQBins, 1.7 - 10, 1.7 + 10);
if(xBin == 2) QQBin = getBinN2(QQSubbinCenterValue, NQQBins, 2.2 - 10, 2.2 + 10);
if(xBin == 3) QQBin = getBinN2(QQSubbinCenterValue, NQQBins, 2.9 - 10, 2.9 + 10);
if(xBin == 4) QQBin = getBinN2(QQSubbinCenterValue, NQQBins, 3.0, 100.0);
int phihBin = getBinN2(phihSubbinCenterValue, NphihBins, phihMin, phihMax);

NnonzeroSubbins[xBin][QQBin][phihBin]++;
summedBornCS[xBin][QQBin][phihBin] = summedBornCS[xBin][QQBin][phihBin] + sib;
if(iz == zSubIndexStart + 2 && iPT2 == PT2SubIndexStart + 2 && (iphih-2)%5 == 0)
{
if((ix == 6 && iQQ == 1) || (ix == 8 && iQQ == 5) || (ix == 14 && iQQ == 5) || (ix == 16 && iQQ == 12) || (ix == 23 && iQQ == 12) || (ix == 26 && iQQ == 20) || (ix == 32 && iQQ == 21) || (ix == 35 && iQQ == 29) || (ix == 44 && iQQ == 42))
{
centerSubbinValue[xBin][QQBin][phihBin] = sib;
}
}
}

}}}
datfile.close();

gSystem->Exec(Form("rm test_%s_%i_%i.dat", pipORpim.c_str(), iz, iPT2));
cout<<"finished "<<iz<<" "<<iPT2<<endl;
}}


/////////////////////////////////////////////////////////////////////////////////////////
////////////////////// now simply print the results: ////////////////////////////////////

TFile *tf = new TFile(Form("BCC_%s_%i_%i.root", pipORpim.c_str(), zBin, PT2Bin), "recreate");

TH1F *outhist[NxBins][NQQBins];

for(int ix = 0; ix < NxBins; ix++) {
for(int iQQ = 0; iQQ < NQQBins; iQQ++) {

outhist[ix][iQQ] = new TH1F(Form("BCCvsphih_x%iQQ%i", ix, iQQ), Form("BCCvsphih_x%iQQ%i", ix, iQQ), NphihBins, -180, 180);
outhist[ix][iQQ]->Sumw2();

for(int iphih = 0; iphih < NphihBins; iphih++) {

if(NnonzeroSubbins[ix][iQQ][iphih] > 0 && centerSubbinValue[ix][iQQ][iphih] > 0) outhist[ix][iQQ]->SetBinContent(iphih+1, summedBornCS[ix][iQQ][iphih]/(NnonzeroSubbins[ix][iQQ][iphih]*centerSubbinValue[ix][iQQ][iphih]));
outhist[ix][iQQ]->SetBinError(iphih+1, 0);
//cout<<ix<<" "<<iQQ<<" "<<" "<<iphih<<" "<<NnonzeroSubbins[ix][iQQ][iphih]<<" "<<summedBornCS[ix][iQQ][iphih]<<" "<<centerSubbinValue[ix][iQQ][iphih]<<endl;

}

outhist[ix][iQQ]->Write();
}}

}
