#ifndef FUNCTIONS_CXX 
#define FUNCTIONS_CXX 

/*
  
  Author: Nathan Harrison 
  Edited: David Riser 
  Date:   August 15, 2018 

  Helper functions used in the analysis of 
  E1-F, performed primarily by the file 
  mysidis.C, but could be used by any. 

*/

// Helper classes for pi+ and pi- cuts on 
// beta(p).
struct BetaPCutLimits {
  float min, max, sigma; 
};

class BetaPTable {

public:
  BetaPTable(std::string path, std::string meson){
    if (meson == "pip"){
      pMin = 0.20; 
      pMax = 3.75; 
    } else if (meson == "pim"){
      pMin = 0.20; 
      pMax = 3.25; 
    } else {
      std::cout << "[BetaPTable::BetaPTable] Fatal: Trouble loading for meson type " << meson << std::endl; 
      exit(0); 
    }

    loadValues(path, meson); 
  }
  
  ~BetaPTable(){
  }

  void setMinimumP(float p){
    pMin = p; 
  }

  void setMaximumP(float p){
    pMax = p; 
  }

  float getMinimumP() const {
    return pMin; 
  }

  float getMaximumP() const {
    return pMax; 
  }

  int getNumberBins() const {
    return fNumberBins; 
  }

  int getMomentumBin(float p) const {
    int bin = ((p-pMin)/((pMax-pMin)/fNumberBins));
    if(bin < 0){ 
      bin = 0; 
    }
    
    if(bin >= fNumberBins){ 
      bin = fNumberBins-1; 
    }
    return bin; 
  }

  BetaPCutLimits & getValues(int type, int sector, int bin){
    return fValues[type][sector][bin]; 
  } 

  void loadValues(std::string path, std::string meson){
    float min, max, sigma; 

    for(int type=0; type<2; type++){
      for(int sector=1; sector<7; sector++){
	for(int bin=0; bin<fNumberBins; bin++){
      
	  std::ifstream infile;
	  infile.open(Form("%s/%sVelocityCuts/t%i_%sVelocityCut_es%i_pBin%i.txt", 
			   path.c_str(), meson.c_str(), type, meson.c_str(), sector, bin)); 

	  if(infile){
	    infile >> min >> max >> sigma;
	    sigma = fabs(sigma); 
	          
	    BetaPCutLimits limits; 
	    limits.min   = min; 
	    limits.max   = max; 
	    limits.sigma = sigma; 
	    
	    fValues[type][sector-1][bin] = limits; 
	  }
	  infile.close();
	  
	}
      }
    }
  }

private:
  const static int fNumberBins = 70; 
  float pMin, pMax; 
  
  // Main content of the class is this 
  // data object containing all the parameters. 
  BetaPCutLimits fValues[2][6][fNumberBins]; 
};

Int_t getRunNumberFromFilename(TString filename){
  Int_t run = 0; 
  TRegexp runnoRegex("[1-9][0-9][0-9][0-9][0-9]");
  TString srunno = filename(runnoRegex);
  run = srunno.Atoi();
  return run; 
}

/*
  
  Functions below original to 
  funtions.C file from Nathan 
  Harrison. 

*/

// dc_xsc etc. are renamed to tl3_x etc. in the h10 --> h22 conversion
Float_t get_thetaCC(Float_t dc_xsc, Float_t dc_ysc, Float_t dc_zsc, Float_t dc_cxsc, Float_t dc_cysc, Float_t dc_czsc)
{
// Copied (and adjusted) from CLAS_Event.cc
// local variables
// ccplane definition from clas note 2004-020 in sector reference coord sys
// A x + B y + C z + D = 0
// - (7.840784063e-4) x - (1.681461571e-3) z + 1 = 0
//                           A           B          C
Float_t cc_pln[3] = {-0.0007840784063, 0.0, -0.001681461571};
Float_t d = 1.0;

Float_t dir[3] = {dc_cxsc, dc_cysc, dc_czsc};
// Need the point in cm for this to work
Float_t cm_factor = 1.0; // this is 10 in the original CLAS_Event.cc script
Float_t P1[3] = {dc_xsc/cm_factor, dc_ysc/cm_factor, dc_zsc/cm_factor};

// R = P1 - DIR t  (minus sign since dir has opposite sign (SC > CC opposite than DC > SC)
// We want t such that it satisfy the plane equation
// A x + B y + C z + D = 0
// Substitute x,y,z with x = p1_x + t dir_x
Float_t t = (cc_pln[0]*P1[0] + cc_pln[1]*P1[1] + cc_pln[2]*P1[2] + d)/(cc_pln[0]*dir[0] + cc_pln[1]*dir[1] + cc_pln[2]*dir[2]);

Float_t CCx = (P1[0] + dir[0]*t)*10;
Float_t CCy = (P1[1] + dir[1]*t)*10;
Float_t CCz = (P1[2] + dir[2]*t)*10;

Float_t thetaCC = atan2(sqrt(CCx*CCx + CCy*CCy), CCz);
return thetaCC;
}

Float_t shift180180to30330(Float_t phi)
{
if(phi < -30) phi = phi + 360;
return phi;
}

Float_t get_rel_phi(Float_t phi)
{
if(phi < -150) return phi + 180;
if(phi >= -150 && phi < -90) return phi + 120;
if(phi >= -90 && phi < -30) return phi + 60;
if(phi >= -30 && phi < 30) return phi;
if(phi >= 30 && phi < 90) return phi - 60;
if(phi >= 90 && phi < 150) return phi - 120;
if(phi >= 150 && phi < 210) return phi - 180;
if(phi >= 210 && phi < 270) return phi - 240;
if(phi >= 270 && phi < 330) return phi - 300;
if(phi >= 330) return phi - 360;
return 0;
}

Float_t get_rel_phi2(Float_t phi, Int_t sector) // phi should be in the -30 to 330 range convention
{
if(sector == 2) phi = phi - 60;
if(sector == 3) phi = phi - 120;
if(sector == 4) phi = phi - 180;
if(sector == 5) phi = phi - 240;
if(sector == 6) phi = phi - 300;
return phi;
}

Int_t ccphimatching(Int_t cc_segm, Float_t phi)
{
Int_t ccpmt = cc_segm/1000 - 1; // -1 left pmt; +1 right pmt; 0 both
Float_t relphi = get_rel_phi(phi);

if(relphi > 0 && ccpmt > 0) return 1;
if(relphi > 0 && ccpmt < 0) return 2;
if(relphi < 0 && ccpmt < 0) return -1;
if(relphi < 0 && ccpmt > 0) return -2;
if(ccpmt == 0 || relphi == 0) return 0;
return 0;
}

Float_t atan3(Float_t y, Float_t x) // same as atan2 but returns a value between 0 and 2pi instead of between -pi and pi
{
	if(atan2(y, x) >= 0)
	{
	return atan2(y, x);
	}
	else if(atan2(y, x) < 0)
	{
	return atan2(y, x) + 2*3.14159265359;
	}
	else
	{
	return -123;
	}
}

int getBinN(float val, int arrayLength, float LimitsArray[])
{
if(val < LimitsArray[0] || val >= LimitsArray[arrayLength-1]) return -123;

int binN = 0;
while(val >= LimitsArray[binN+1]) binN++;

return binN;
}

int getBinN2(float val, int Nbins, float min, float max)
{
if(val < min || val >= max) return -123;

int binN = static_cast<int>((val - min)/((max - min)/Nbins));
return binN;
}

#endif
