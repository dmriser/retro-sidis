float PTfcn(float x, float QQ, float z, float MX)
{
float prot_mass = 0.938272; // GeV
float pip_mass = 0.13957; // GeV
float Beam_Energy = 5.498; // GeV
float pi = 3.14159265359;

//float etheta = asin(sqrt((prot_mass*x*QQ)/(4.0*prot_mass*x*Beam_Energy*Beam_Energy - 2.0*QQ*Beam_Energy)));
float etheta = 2.0*asin(sqrt((prot_mass*x*QQ)/(4.0*prot_mass*x*Beam_Energy*Beam_Energy - 2.0*QQ*Beam_Energy))); // original was off by factor of 2!
float ez = (2.0*Beam_Energy*Beam_Energy - ((QQ*Beam_Energy)/(prot_mass*x)) - QQ)/(2.0*Beam_Energy);
float emom = ez/cos(etheta);

TVector3 V3e(1.0, 1.0, 1.0);
V3e.SetMag(emom);
V3e.SetTheta(etheta);
V3e.SetPhi(0.0); // set to 0 by choice

TLorentzVector V4e(V3e, V3e.Mag()); // neglecting e- mass
TLorentzVector V4k(0.0, 0.0, Beam_Energy, Beam_Energy);
TLorentzVector V4q = V4k - V4e;
TLorentzVector V4ISproton(0.0, 0.0, 0.0, prot_mass); // IS = Initial State

TLorentzVector V4H = V4q + V4ISproton;
TVector3 V3H;
V3H.SetXYZ(V4H.X(), V4H.Y(), V4H.Z());

float abc = pow(V4q.T() + prot_mass - V4q.T()*z, 2);
float pipx, pipy, pipz;
pipy = sqrt(-1);

int loopCount = 0;
while(pipy != pipy && loopCount < 10000)
{
pipz = gRandom->Uniform(-8.0, 8.0); // guess
pipx = (MX*MX - abc + V4q.X()*V4q.X() + V4q.T()*V4q.T()*z*z - pip_mass*pip_mass + V4q.Z()*V4q.Z() - 2.0*V4q.Z()*pipz)/(2.0*V4q.X());
pipy = sqrt(-1.0*pipx*pipx + V4q.T()*V4q.T()*z*z - pipz*pipz - pip_mass*pip_mass);
loopCount++;
}

TVector3 V3pip;
V3pip.SetX(pipx);
V3pip.SetY(pipy);
V3pip.SetZ(pipz);

TLorentzVector V4pip(V3pip, sqrt(V3pip.Mag2() + pip_mass*pip_mass));

V4pip.RotateZ(-1.0*V4q.Phi() - pi);
V4pip.RotateY(V4q.Theta());
V4pip.Boost(0.0, 0.0, V3H.Mag()/V4H.T());

if(loopCount > 9998) return -1;
return V4pip.Perp();
}
