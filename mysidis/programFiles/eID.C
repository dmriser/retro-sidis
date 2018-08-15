#ifndef EID_CXX 
#define EID_CXX

#include "eIDsubroutines.C"
#include "vertexCorr.C"
#include "sctimeCorr.C"

int eID(Int_t gpart, Int_t q[], Float_t p[], UChar_t cc_sect[], UChar_t sc_sect[], UChar_t ec_sect[], UChar_t dc_sect[], Float_t cx[], Float_t cy[], Float_t cz[], Float_t tl1_x[], Float_t tl1_y[], Float_t tl3_x[], Float_t tl3_y[], Float_t tl3_z[], Float_t tl3_cx[], Float_t tl3_cy[], Float_t tl3_cz[], int e_zvertex_strict, Float_t vz[], Float_t vy[], Float_t vx[], int e_ECsampling_strict, int ExpOrSim, Float_t etot[], int e_ECoutVin_strict, Float_t ec_ei[], Float_t ech_x[], Float_t ech_y[], Float_t ech_z[], int e_CCthetaMatching_strict, UShort_t cc_segm[], int e_ECgeometric_strict, int e_R1fid_strict, int e_R3fid_strict, int e_CCphiMatching_strict, UChar_t sc_pd[], int e_CCfiducial_strict)
{
vector<int> Veindex;
for(int k = 0; k < gpart; k++) // loop over particles
{
if(q[k] == -1 && cc_sect[k] != 0 && sc_sect[k] != 0 && ec_sect[k] != 0 && dc_sect[k] != 0 && goodORbadSCpaddle(dc_sect[k], sc_pd[k]) == 1)
{
Float_t phi = atan3(cy[k],cx[k])*57.2957795;
Float_t relphi = get_rel_phi2(shift180180to30330(atan2(cy[k],cx[k])*57.2957795), dc_sect[k]);
Float_t thetaCC = 57.2957795*get_thetaCC(tl3_x[k], tl3_y[k], tl3_z[k], tl3_cx[k], tl3_cy[k], tl3_cz[k]);

// check if particle passes eID cuts:
Bool_t zvertex_pass, ECsampling_pass, ECoutVin_pass, ECgeometric_pass, CCthetaMatching_pass, R1fid_pass, R3fid_pass, CCphiMatching_pass, CCfiducial_pass;
zvertex_pass = ECsampling_pass = ECoutVin_pass = ECgeometric_pass = CCthetaMatching_pass = R1fid_pass = R3fid_pass = CCphiMatching_pass = CCfiducial_pass = 0;

zvertex_pass = e_zvertex_pass(e_zvertex_strict, ExpOrSim, getCorrZ(ExpOrSim, vx[k], vy[k], vz[k], p[k]*cx[k], p[k]*cy[k], p[k]*cz[k], dc_sect[k]));
if(zvertex_pass) ECsampling_pass = e_ECsampling_pass(e_ECsampling_strict, ExpOrSim, dc_sect[k], etot[k], p[k]);
if(zvertex_pass && ECsampling_pass) ECoutVin_pass = e_ECoutVin_pass(e_ECoutVin_strict, ec_ei[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass) ECgeometric_pass = e_ECgeometric_pass(e_ECgeometric_strict, ech_x[k], ech_y[k], ech_z[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass) CCthetaMatching_pass = e_CCthetaMatching_pass(e_CCthetaMatching_strict, ExpOrSim, dc_sect[k], thetaCC, (cc_segm[k]%1000)/10);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass) R1fid_pass = e_R1fid_pass(e_R1fid_strict, dc_sect[k], tl1_x[k], tl1_y[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass) R3fid_pass = e_R3fid_pass(e_R3fid_strict, dc_sect[k], tl3_x[k], tl3_y[k]);
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass) CCphiMatching_pass = e_CCphiMatching_pass(e_CCphiMatching_strict, ccphimatching(cc_segm[k], phi));
if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass) CCfiducial_pass = e_CCfiducial_pass(e_CCfiducial_strict, thetaCC, relphi);

if(zvertex_pass && ECsampling_pass && ECoutVin_pass && ECgeometric_pass && CCthetaMatching_pass && R1fid_pass && R3fid_pass && CCphiMatching_pass && CCfiducial_pass) Veindex.push_back(k);
}
} // end of loop over gpart

// %%%%% find highest momentum electron %%%%%
int electron_index = -123;
if(Veindex.size() > 0) electron_index = Veindex[0];
if(Veindex.size() > 1)
{
for(unsigned int v = 1; v < Veindex.size(); v++)
{
if(p[Veindex[v]] > p[electron_index]) electron_index = Veindex[v];
}
}

return electron_index;
}

#endif 
