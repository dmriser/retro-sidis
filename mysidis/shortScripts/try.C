{
TFile *tf = new TFile("try.root", "recreate");
TTree *tt = new TTree("tt", "a tree");

Float_t x, QQ, z, PT2, MX;

tt->Branch("x", &x, "x/F");
tt->Branch("QQ", &QQ, "QQ/F");
tt->Branch("z", &z, "z/F");
tt->Branch("PT2", &PT2, "PT2/F");
tt->Branch("MX", &MX, "MX/F");

gROOT->ProcessLine(".L PTfcn.C");

for(float dummyx = 0.1; dummyx <= 0.6; dummyx = dummyx + 0.1) {
for(float dummyQQ = 1.0; dummyQQ <= 4.5; dummyQQ = dummyQQ + 0.7) {
for(float dummyz = 0.1; dummyz <= 0.9; dummyz = dummyz + 0.1) {
for(float dummyMX = 1.0; dummyMX <= 4.5; dummyMX = dummyMX + 0.7) {

x = dummyx;
QQ = dummyQQ;
z = dummyz;
MX = dummyMX;
PT2 = PTfcn(dummyx, dummyQQ, dummyz, dummyMX);
if(PT2 > -0.5) PT2 = PT2*PT2; // -1 is a default value for no solution

tt->Fill();

cout<<x<<" "<<QQ<<" "<<z<<" "<<MX<<endl;

}}}}

tt->Write();

}
