{

string pipORpim = "pip";

int NxBins = 5;
int NQQBins = 2;
int NzBins = 18;
int NPT2Bins = 20;

for(int x = 0; x < NxBins; x++) {
for(int QQ = 0; QQ < NQQBins; QQ++) {
for(int z = 0; z < NzBins; z++) {
for(int PT2 = 0; PT2 < NPT2Bins; PT2++) {
if(!(x == 4 && QQ == 1))
{

float M, Ac, Acc;
float Me, Ace, Acce;
float Msys, Acsys, Accsys;

ifstream Mfile(Form("/scratch/MAcAccfiles/%s_M_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), x, QQ, z, PT2));
ifstream AcAccfile(Form("/scratch/MAcAccfiles/%s_AcAcc_it2_hap2_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), x, QQ, z, PT2));
ifstream MsysEfile(Form("/home/kjgroup/mysidis/shortScripts/phi_h/systematicErrors/%s_M_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), x, QQ, z, PT2));
ifstream AcAccsysEfile(Form("/home/kjgroup/mysidis/shortScripts/phi_h/systematicErrors/%s_AcAcc_sys_BiSc5_x%iQQ%iz%iPT2%i.txt", pipORpim.c_str(), x, QQ, z, PT2));

if(Mfile) Mfile>>M>>Me;
else M = Me = -123.456789;

if(AcAccfile) AcAccfile>>Ac>>Ace>>Acc>>Acce;
else Ac = Ace = Acc = Acce = -123.456789;

if(MsysEfile) MsysEfile>>Msys;
else Msys = -123.456789;

if(AcAccsysEfile) AcAccsysEfile>>Acsys>>Accsys;
else Acsys = Accsys = -123.456789;

if(!(!Mfile && !AcAccfile && !MsysEfile && !AcAccsysEfile))
{
cout<<x<<" "<<QQ<<" "<<z<<" "<<PT2<<" "<<fixed<<setprecision(1)<<M<<" "<<Me<<" "<<Msys<<" "<<fixed<<setprecision(4)<<Ac<<" "<<Ace<<" "<<Acsys<<" "<<Acc<<" "<<Acce<<" "<<Accsys<<endl;
}

Mfile.close();
AcAccfile.close();
MsysEfile.close();
AcAccsysEfile.close();

}
}}}}


}
