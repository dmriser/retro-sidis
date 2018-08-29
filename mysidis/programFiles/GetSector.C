Int_t GetSector(Double_t phi) {
  /* phi between -180 and 180. */


  /* phi2 between 0 and 360 */
  Double_t phi2 = phi;
  if (phi<0) phi2 = phi2+360.;

  Int_t sect = 1 + ((int)phi2+30)/60;
  if (sect == 7) sect = 1;


  return sect;
}


