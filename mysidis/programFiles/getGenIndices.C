vector<int> getGenIndices(Int_t mcnentr, Int_t mcid[], Float_t mcp[])
{
int pip_index = -123;
int pim_index = -123;
int prot_index = -123;

// positive pions:
vector<int> pipIndex;
for(int particle = 0; particle < mcnentr; particle++)
{
if(mcid[particle] == 211) pipIndex.push_back(particle);
}

if(pipIndex.size() == 1) pip_index = pipIndex[0];
if(pipIndex.size() > 1)
{
for(unsigned int particle = 1; particle < pipIndex.size(); particle++)
{
if(mcp[pipIndex[particle]] > mcp[pip_index]) pip_index = pipIndex[particle]; // highest momentum pi+
}
}

// negative pions:
vector<int> pimIndex;
for(int particle = 0; particle < mcnentr; particle++)
{
if(mcid[particle] == -211) pimIndex.push_back(particle);
}

if(pimIndex.size() == 1) pim_index = pimIndex[0];
if(pimIndex.size() > 1)
{
for(unsigned int particle = 1; particle < pimIndex.size(); particle++)
{
if(mcp[pimIndex[particle]] > mcp[pim_index]) pim_index = pimIndex[particle]; // highest momentum pi-
}
}

// protons:
vector<int> protIndex;
for(int particle = 0; particle < mcnentr; particle++)
{
if(mcid[particle] == 2212) protIndex.push_back(particle);
}

if(protIndex.size() == 1) prot_index = protIndex[0];
if(protIndex.size() > 1)
{
for(unsigned int particle = 1; particle < protIndex.size(); particle++)
{
if(mcp[protIndex[particle]] < mcp[prot_index]) prot_index = protIndex[particle]; // lowest momentum proton
}
}

vector<int> result;
result.push_back(pip_index);
result.push_back(pim_index);
result.push_back(prot_index);

return result;

}
