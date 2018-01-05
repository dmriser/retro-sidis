float e_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno)
{
if(ExpOrSim == 0) return sctime;

if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.5;
if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 0.9;

if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.3;

if(sector == 4 && paddle == 5 && runno >= 38548) return sctime + 2.0;

if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;

if(sector == 5 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime + 1.1;

if(sector == 5 && paddle == 20 && runno >= 37777) return sctime - 0.5;

if(sector == 6 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime - 1.6;

// note: electrons very rarely hit paddle 2, so the below values were copied from the hadron correction function further down (having these vs not having these makes very little difference)
if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;

if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;

if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;

return sctime;
}

// ____________________________________ //

float h_sctimeCorr(int ExpOrSim, float sctime, int sector, int paddle, int runno) // h for hadron
{
if(ExpOrSim == 0) return sctime;

// calibrated using negative tracks: (low paddle numbers)
if(sector == 6 && paddle == 1 && runno >= 0) return sctime + 18.25;

if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;

if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;

if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;

if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;

// calibrated using positive tracks:
if(sector == 1 && paddle == 24 && runno >= 37749) return sctime + 1.13334;

if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.565033;
if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 1.04168;

if(sector == 2 && paddle == 38 && runno >= 38535) return sctime - 1.89592;

if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.26126;

if(sector == 3 && paddle == 24 && runno >= 37855 && runno <= 38546) return sctime - 1.78266;

if(sector == 3 && paddle == 25 && runno >= 37743 && runno <= 38266) return sctime + 2.44804;

if(sector == 3 && paddle == 27 && runno >= 37854 && runno <= 38546) return sctime - 1.85815;

if(sector == 3 && paddle == 28 && runno >= 37854) return sctime + 1.21704;

if(sector == 4 && paddle == 5 && runno >= 38549) return sctime + 1.91688;

if(sector == 4 && paddle == 19 && runno >= 37854) return sctime - 0.365798;

if(sector == 4 && paddle == 34 && runno >= 37854) return sctime - 2.33721;

if(sector == 4 && paddle == 42 && runno >= 37750) return sctime - 1.4118;

if(sector == 4 && paddle == 45 && runno >= 38551) return sctime - 3.36406;

if(sector == 5 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime + 1.24884;

if(sector == 5 && paddle == 20 && runno >= 37809) return sctime - 0.468722;

if(sector == 5 && paddle == 34 && runno <= 37853) return sctime - 1.0;
if(sector == 5 && paddle == 34 && runno >= 37854) return sctime + 6.0;

if(sector == 5 && paddle == 36 && runno >= 37748) return sctime + 1.07962;

if(sector == 6 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime - 1.69106;

if(sector == 6 && paddle == 42 && runno >= 37854 && runno <= 38545) return sctime - 6.0;

return sctime;
}

// ____________________________________ //

bool goodORbadSCpaddle(int sector, int paddle)
{
if(sector == 3 && paddle == 2) return 0;
if(sector == 5 && paddle == 3) return 0;
if(sector == 2 && paddle == 16) return 0;
if(sector == 2 && paddle == 40) return 0;
if(sector == 3 && paddle == 40) return 0;
if(sector == 5 && paddle == 40) return 0;
if(sector == 6 && paddle == 40) return 0;
if(sector == 1 && paddle == 41) return 0;
if(sector == 2 && paddle == 41) return 0;
if(sector == 3 && paddle == 41) return 0;
if(sector == 5 && paddle == 41) return 0;
if(sector == 1 && paddle == 42) return 0;
if(sector == 2 && paddle == 42) return 0;
if(sector == 3 && paddle == 42) return 0;
if(sector == 5 && paddle == 42) return 0;
if(sector == 6 && paddle == 42) return 0;
if(sector == 2 && paddle == 43) return 0;
if(sector == 3 && paddle == 43) return 0;
if(sector == 4 && paddle == 43) return 0;
if(sector == 5 && paddle == 43) return 0;
if(sector == 1 && paddle == 44) return 0;
if(sector == 3 && paddle == 44) return 0;
if(sector == 5 && paddle == 44) return 0;
if(sector == 6 && paddle == 44) return 0;
if(sector == 1 && paddle == 45) return 0;
if(sector == 2 && paddle == 45) return 0;
if(sector == 3 && paddle == 45) return 0;
if(sector == 6 && paddle == 45) return 0;
if(sector == 1 && paddle == 46) return 0;
if(sector == 2 && paddle == 46) return 0;
if(sector == 3 && paddle == 46) return 0;
if(sector == 4 && paddle == 46) return 0;
if(sector == 5 && paddle == 46) return 0;
if(sector == 1 && paddle == 47) return 0;
if(sector == 5 && paddle == 47) return 0;

return 1;
}
