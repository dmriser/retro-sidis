float getCorrZ(int ExpOrSim, float vx, float vy, float vz, float px, float py, float pz, int s)
{
s--;
float s0, sp, sv;

float n[3][6];

for(int abc = 0; abc < 3; abc++) // initialize to zero
{
for(int def = 0; def < 6; def++)
{
n[abc][def] = 0.0;
}
}

n[0][0] = 1.0;
n[1][0] = 0.0;

n[0][1] = 0.5;
n[1][1] = 0.866025388;

n[0][2] = -0.5;
n[1][2] = 0.866025388;

n[0][3] = -1.0;
n[1][3] = 0.0;

n[0][4] = -0.5;
n[1][4] = -0.866025388;

n[0][5] = 0.5;
n[1][5] = -0.866025388;

float x0, y0, z0; // beam position (cm)
if(ExpOrSim == 1)
{
x0 = 0.15;
y0 = -0.25;
z0 = 0.0;
}
if(ExpOrSim == 0)
{
x0 = 0.0;
y0 = 0.0;
z0 = 0.0;
}

float A;

s0 = x0*n[0][s] + y0*n[1][s] + z0*n[2][s];
sp = px*n[0][s] + py*n[1][s] + pz*n[2][s];
sv = vx*n[0][s] + vy*n[1][s] + vz*n[2][s];

float cvz;

if(fabs(sp) > 0.0000000001)
{
A = (s0-sv)/sp;
cvz = vz + A*pz;
}
else
{
cvz = vz;
}

return cvz;
}

/* from Mauri:

V3 vertex_corr_selection::vertex_corr(V3 v, V4 p)
{
        V3 vc;

        double s0, sp, sv;
        static double n[3][6];
        n[0][0] = 1.;
        n[1][0] = 0.;

        n[0][1] = 0.5 ;
        n[1][1] = 0.866025388;

        n[0][2] = -0.5 ;
        n[1][2] = 0.866025388;

        n[0][3] = -1. ;
        n[1][3] = 0.;

        n[0][4] = -0.5 ;
        n[1][4] = -0.866025388;

        n[0][5] = 0.5 ;
        n[1][5] = -0.866025388;

        static double x0 =  beam_pos[0]/cm;
        static double y0 =  beam_pos[1]/cm;
        static double z0 =   0.;

        int s = sector(p) - 1;
        double A;

        s0 =     x0  * n[0][s]  +     y0  * n[1][s] +     z0  * n[2][s];
        sp = p.x/GeV * n[0][s]  + p.y/GeV * n[1][s] + p.z/GeV * n[2][s];
        sv = v.x/cm  * n[0][s]  +  v.y/cm * n[1][s] +  v.z/cm * n[2][s];

        if(sp)
        {
                A = (s0-sv)/sp;
                vc.x = (v.x/cm + A*p.x/GeV)*cm;
                vc.y = (v.y/cm + A*p.y/GeV)*cm;
                vc.z = (v.z/cm + A*p.z/GeV)*cm;
                return vc;
        }

        return v;
}

*/
