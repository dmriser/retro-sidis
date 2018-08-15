#ifndef PROTON_SUBROUTINES_CXX
#define PROTON_SUBROUTINES_CXX

Bool_t prot_DtVp_pass(Float_t Dt_protmass){
  return (Dt_protmass > -0.45 && Dt_protmass < 0.5); 
}

#endif 
