The function can now be used to correct momenta of all particles, electrons and hadrons.


1) Include MomCorr.C in your analysis program

2) At the beginning of the program, you have to declare the pointer to the class

MomCorr_e1f *MomCorr = new MomCorr_e1f();

3) For each electron, assuming that you have a TLorentzVector P with the uncorrected momentum,
you can apply the correction:

P = MomCorr->PcorN(P, q, pid);

where "Int_t q" is the cherge of the particle and "Int_t pid" is the particle ID code 
(11 for electrons, -211 for pi- and so on).

4) Copy all *.out files in the directory where you run your analysis program

5) Run your program, if everithing is OK at the beginning you will see a bunch of numbers 
(the correction parameters).

6) The correction has been calculated after application of the energy loss correction (see 
http://www.jlab.org/Hall-B/secure/e1f/mirazita/eloss/), however this correction does nothing 
to electrons and very little for hadrons apart from low energy protons, so not using it should 
not change so much the results.


Feb 12, 2010 - Update
The function GetSector(Int_t) is also necessary to run the code and can be found in the 
file GetSector.C
