{
// in haprad, the default h3.f and h4.f functions depend on x, QQ, and z (not PT)

TF3 *h3_vs_xQQz = new TF3("h3_vs_xQQz", "[0]*pow(x, [1])*pow(1.0 - x, [2])*pow(z, [3])*pow(1.0 - z, [4])*pow(log(y/[7])/log([6]/[7]), [5])", 0.05, 0.65, 0.5, 5.0, 0, 1); // fcn from haprad
h3_vs_xQQz->SetParName(0, "a");
h3_vs_xQQz->SetParName(1, "a1");
h3_vs_xQQz->SetParName(2, "a2");
h3_vs_xQQz->SetParName(3, "b1");
h3_vs_xQQz->SetParName(4, "b2");
h3_vs_xQQz->SetParName(5, "bb");
h3_vs_xQQz->SetParName(6, "q0");
h3_vs_xQQz->SetParName(7, "lambdaSquared");

h3_vs_xQQz->FixParameter(6, 1.0); // parameters from haprad
h3_vs_xQQz->FixParameter(7, 0.25*0.25);
h3_vs_xQQz->SetParameter("a", -0.36544E-03);
h3_vs_xQQz->SetParameter("a1", -2.1855);
h3_vs_xQQz->SetParameter("a2", 3.4176);
h3_vs_xQQz->SetParameter("b1", -1.7567);
h3_vs_xQQz->SetParameter("b2", 1.1272);
h3_vs_xQQz->SetParameter("bb", 8.9985);

///////////////////////


}
