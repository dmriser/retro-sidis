float PTsqFcn(float x, float QQ, float z, float MX)
{
float prot_mass = 0.938272; // GeV
float pip_mass = 0.13957; // GeV
float Beam_Energy = 5.498; // GeV

float y = QQ/(2.0*Beam_Energy*prot_mass*x);
float nu = y*Beam_Energy;
float Eh = z*nu;

float bigterm = 0.5*(MX*MX - prot_mass*prot_mass - (QQ/x)*(1.0 - z) - pip_mass*pip_mass + QQ + 2.0*nu*Eh);

float PTsq = bigterm*bigterm + nu*nu*pip_mass*pip_mass + QQ*pip_mass*pip_mass - nu*nu*Eh*Eh - QQ*Eh*Eh; // not done yet!
PTsq = PTsq/(nu*nu + QQ); // not done yet!
PTsq = -PTsq;

return PTsq;
}
