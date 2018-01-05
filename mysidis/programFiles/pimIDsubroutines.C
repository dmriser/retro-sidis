Bool_t pim_vvp_pass(int strict, int ExpOrSim, Int_t e_sector, float velocity, float p)
{
if(strict == 9) return 1;

int NpBins = 70;
float pMin = 0.2;
float pMax = 3.25;

int pBin = static_cast<int>((p - pMin)/((pMax - pMin)/NpBins));
if(pBin < 0) pBin = 0;
if(pBin >= NpBins) pBin = NpBins-1;

float cutlow = -1;
float cuthigh = 2;
float sigma = 0.01;

ifstream infile;
infile.open(Form("/home/kjgroup/mysidis/programFiles/pimVelocityCuts/t%i_pimVelocityCut_es%i_pBin%i.txt", ExpOrSim, e_sector, pBin));
if(infile) infile>>cutlow>>cuthigh>>sigma;
infile.close();

sigma = fabs(sigma); // just to be safe... root sometimes gives negative sigmas

float Nsig_systematics = 0.25;

if(velocity > cutlow + strict*Nsig_systematics*sigma && velocity < cuthigh - strict*Nsig_systematics*sigma) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t pim_R1fid_pass(int strict, Int_t sector, Float_t tl1_x, Float_t tl1_y)
{
if(strict == 9) return 1;

const int Nstrictnesses = 3;
int strictIndex = strict + 1;

float height[Nstrictnesses] = {19, 20, 21};
float angle[Nstrictnesses] = {81, 80, 79};
float horizontal[Nstrictnesses] = {23.5, 24, 24.5};

float slope[Nstrictnesses];

for(int st = 0; st < Nstrictnesses; st++)
{
slope[st] = 1.0/tan(0.5*(3.141592653/180.0)*angle[st]);
}

int sm1 = sector-1;
float rotx = tl1_y*sin(sm1*(3.14159/180.0)*60.0)+tl1_x*cos(sm1*(3.14159/180.0)*60.0);
float roty = tl1_y*cos(sm1*(3.14159/180.0)*60.0)-tl1_x*sin(sm1*(3.14159/180.0)*60.0);

if(rotx > height[strictIndex] - slope[strictIndex]*roty && rotx > height[strictIndex] + slope[strictIndex]*roty && rotx > horizontal[strictIndex]) return 1;

return 0;
}

// __________________________________________________________________________________________ //

Bool_t pim_MXcut_pass(int strict, float MX)
{
if(strict == 9) return 1;

if(MX > 1.35 + strict*0.025) return 1;

return 0;
}

// __________________________________________________________________________________________ //

//   Bool_t pim_DtVp_pass(Float_t Dt, Float_t p)
//   {
//   Float_t Dt_pi_cutUpper[2][2][6][3] = {{{{0.910139,-0.176902,0.0304629},{0.843087,-0.0653259,-0.00328711},{0.862763,-0.0986849,0.00924304},{0.883381,-0.165825,0.0325076},{0.894543,-0.134258,0.0170612},{0.884652,-0.140827,0.022762}},{{0.720285,-0.0408605,-0.0135548},{0.757669,-0.0492098,-0.00679556},{0.752593,-0.0257385,-0.0150494},{0.747952,-0.0300356,-0.00988201},{0.749907,-0.0163518,-0.0194349},{0.70282,-0.0567048,-0.006749}}},{{{0.606988,-0.118234,0.0200837},{0.561737,-0.0420583,-0.00303742},{0.575539,-0.0670123,0.00632411},{0.591373,-0.11452,0.0227254},{0.594292,-0.0866717,0.0103998},{0.588591,-0.0920729,0.0144036}},{{0.474394,-0.0195558,-0.0123427},{0.513072,-0.0266181,-0.00659581},{0.51394,-0.0100214,-0.011968},{0.501095,-0.00440032,-0.00972932},{0.508166,0.0027123,-0.0174871},{0.468664,-0.0374875,-0.00572585}}}}; // [strictness of cut][ExpOrSim][sector#][2nd order poly (3 params)] ... strict=0 -> 3sigma ; strict=1 -> 2sigma
//   
//   Float_t Dt_pi_cutLower[2][2][6][3] = {{{{-0.908769,0.175101,-0.0318122},{-0.845015,0.0742798,-0.00178897},{-0.860581,0.0913509,-0.00827058},{-0.868666,0.142003,-0.0261853},{-0.906963,0.15126,-0.0229073},{-0.891716,0.151697,-0.0273885}},{{-0.755059,0.0869676,-0.00628253},{-0.715026,0.0932549,-0.00724442},{-0.679327,0.0685638,0.00343877},{-0.733192,0.123776,-0.00896587},{-0.700543,0.0980328,-0.00774834},{-0.706106,0.0639918,-0.00189503}}},{{{-0.605618,0.116434,-0.021433},{-0.563664,0.0510122,-0.00203866},{-0.573357,0.0596783,-0.00535164},{-0.576658,0.0906986,-0.0164032},{-0.606712,0.103674,-0.0162459},{-0.595655,0.102943,-0.0190301}},{{-0.509169,0.0656629,-0.00749457},{-0.469406,0.0692803,-0.0071147},{-0.440673,0.0528468,0.000357405},{-0.486335,0.098141,-0.00911856},{-0.458801,0.0789687,-0.0096961},{-0.471152,0.0436959,-0.00266119}}}}; // [strictness of cut][ExpOrSim][sector#][2nd order poly (3 params)] ... strict=0 -> 3sigma ; strict=1 -> 2sigma
//   
//   int Dt_pi_strictness = 1;
//   int ExpOrSim = 1;
//   int randomSector = 4;
//   if(Dt < Dt_pi_cutUpper[Dt_pi_strictness][ExpOrSim][randomSector-1][0] + Dt_pi_cutUpper[Dt_pi_strictness][ExpOrSim][randomSector-1][1]*p + Dt_pi_cutUpper[Dt_pi_strictness][ExpOrSim][randomSector-1][2]*p*p && Dt > Dt_pi_cutLower[Dt_pi_strictness][ExpOrSim][randomSector-1][0] + Dt_pi_cutLower[Dt_pi_strictness][ExpOrSim][randomSector-1][1]*p + Dt_pi_cutLower[Dt_pi_strictness][ExpOrSim][randomSector-1][2]*p*p) return 1;
//   
//   return 0;
//   }
//   
//   // __________________________________________________________________________________________ //
//   
//   Bool_t pim_npheCC_pass(Int_t nphe)
//   {
//   if(nphe < 25) return 1;
//   return 0;
//   }
//   
//   // __________________________________________________________________________________________ //
//   
//   Bool_t pim_ECoutVin_pass(Float_t ec_eo, Float_t ec_ei)
//   {
//   // if(ec_eo > 0.01 && ec_ei < 0.055 && ec_ei > 0.01) return 1; // way too strict
//   if(ec_ei < 0.055) return 1;
//   return 0;
//   }
