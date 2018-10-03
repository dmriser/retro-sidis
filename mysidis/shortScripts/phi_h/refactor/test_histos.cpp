#include "histograms.cpp"

void test_histos(){
  HistogramPack histos = createPack("pip", 0, 0, 3, 4); 

  std::cout << histos.data->GetTitle() << std::endl; 
  std::cout << histos.rec->GetTitle() << std::endl; 
  std::cout << histos.gen->GetTitle() << std::endl; 
  std::cout << histos.acc->GetTitle() << std::endl; 
  std::cout << histos.rc->GetTitle() << std::endl; 
  std::cout << histos.corrAcc->GetTitle() << std::endl; 
  std::cout << histos.corrRC->GetTitle() << std::endl; 
  std::cout << histos.corr->GetTitle() << std::endl; 
}
