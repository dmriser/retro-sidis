TH1F* MarkovRebin(TH1F *datahist, TH1F *acchist) // see pg. 93 of http://www.jlab.org/Hall-B/secure/e1/markov/2GeV/newDesign/review/anoteMarkovV1.pdf
{
if(datahist->GetXaxis()->GetNbins() != acchist->GetXaxis()->GetNbins()) cout<<"Problem! Nbins not equal!"<<endl;
if(datahist->GetXaxis()->GetNbins()%4 != 0) cout<<"Problem! Nbins must be divisable by 4"<<endl;

int Nbins = datahist->GetXaxis()->GetNbins();
TH1F *resulthist = new TH1F("MarkovCorrection", "MarkovCorrection", Nbins/4, datahist->GetXaxis()->GetXmin(), datahist->GetXaxis()->GetXmax());

int loopcount = 0;
for(int k = 1; k <= Nbins - 3; k = k + 4)
{
loopcount++;
float dataval[4], dataerr[4], accval[4], accerr[4], corrval[2], correrr[2];

for(int j = 0; j < 4; j++)
{
dataval[j] = datahist->GetBinContent(k + j);
dataerr[j] = datahist->GetBinError(k + j);
accval[j] = acchist->GetBinContent(k + j);
accerr[j] = acchist->GetBinError(k + j);
}

for(int j = 0; j <= 1; j++) // 0 for the first pair, 1 for the second pair
{
if(dataval[2*j] > 0 && dataval[2*j+1] > 0 && accval[2*j] > 0 && accval[2*j+1] > 0)
{
corrval[j] = dataval[2*j]/accval[2*j] + dataval[2*j+1]/accval[2*j+1];
correrr[j] = sqrt(pow(dataerr[2*j]/accval[2*j], 2) + pow((dataval[2*j]*accerr[2*j])/(accval[2*j]*accval[2*j]), 2) + pow(dataerr[2*j+1]/accval[2*j+1], 2) + pow((dataval[2*j+1]*accerr[2*j+1])/(accval[2*j+1]*accval[2*j+1]), 2));
}
else if(dataval[2*j] > 0 && dataval[2*j+1] > 0 && (accval[2*j] > 0 || accval[2*j+1] > 0))
{
cout<<"special case 2"<<endl;
float goodacc = accval[2*j];
float goodaccerr = accerr[2*j];
if(goodacc == 0) goodacc = accval[2*j+1];
if(goodacc == 0) goodaccerr = accerr[2*j+1];
corrval[j] = dataval[2*j]/goodacc + dataval[2*j+1]/goodacc;
correrr[j] = sqrt(pow(dataerr[2*j]/goodacc, 2) + pow(dataerr[2*j+1]/goodacc, 2) + pow(((dataval[2*j] + dataval[2*j+1])*goodaccerr)/(goodacc*goodacc), 2));
}
else if((dataval[2*j] == 0 || dataval[2*j+1] == 0) && (dataval[2*j] > 0 || dataval[2*j+1] > 0) && (accval[2*j] > 0 || accval[2*j+1] > 0))
{
cout<<"special case 3"<<endl;
float gooddata = dataval[2*j];
float gooddataerr = dataerr[2*j];
if(gooddata == 0) gooddata = dataval[2*j+1];
if(gooddata == 0) gooddataerr = dataerr[2*j+1];
float aveacc = (accval[2*j] + accval[2*j+1])/2.0;
corrval[j] = gooddata/aveacc;
correrr[j] = sqrt(pow((2*gooddataerr)/(accval[2*j] + accval[2*j+1]), 2) + (accerr[2*j]*accerr[2*j] + accerr[2*j+1]*accerr[2*j+1])*pow((2*gooddata)/pow(accval[2*j] + accval[2*j+1], 2), 2));
}
else if((dataval[2*j] == 0 && dataval[2*j+1] == 0) || (accval[2*j] == 0 && accval[2*j+1] == 0))
{
cout<<"special case 4"<<endl;
corrval[j] = 0;
correrr[j] = 0;
}
else cout<<"If you see this message, you have bad logic"<<endl;
}

resulthist->SetBinContent(loopcount, corrval[0] + corrval[1]);
resulthist->SetBinError(loopcount, sqrt(correrr[0]*correrr[0] + correrr[1]*correrr[1]));
} // end of for loop over k

return resulthist;
} // end of function


