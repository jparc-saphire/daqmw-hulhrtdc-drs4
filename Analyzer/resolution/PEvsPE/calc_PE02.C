void calc_PE02()
{
  const int N = 6;

  double ph1[N]  =  {18.92, 29.25, 37.70, 46.36, 55.67, 65.62};  
  double ph1E[N] =  { 0.10,  0.06,  0.04,  0.18,  0.19,  0.22};  

  double ph2[N]  =  {17.66, 25.71, 35.72, 47.03, 54.98, 64.20};  
  double ph2E[N] =  { 0.06,  0.08,  0.09,  0.10,  0.17,  0.17};  

  double pe1[N]  =  {3., 4., 5., 6., 7., 8.};  
  double pe1E[N] =  {0., 0., 0., 0., 0., 0.};  

  double pe2[N]  =  {3., 4., 5., 6., 7., 8.};  
  double pe2E[N] =  {0., 0., 0., 0., 0., 0.};  

  TCanvas *c1 = new TCanvas("c1","c1"); 
  c1->Divide(1,2);
  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.10);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  TH1F* Hc1=c1->cd(1)->DrawFrame( 10., 0.0, 80.0, 10.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("# of P.E.");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(0.7);
  Hc1->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc1->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph1 = new TGraphErrors(N, ph1, pe1, ph1E, pe1E);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");
  graph1->Fit("pol1","","",15,70);

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.10);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  TH1F* Hc2=c1->cd(2)->DrawFrame( 10., 0.0, 80.0, 10.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetTitle("# of P.E.");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(0.7);
  Hc2->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc2->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph2 = new TGraphErrors(N, ph2, pe2, ph2E, pe2E);
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(1);
  graph2->Draw("PEsame");
  graph2->Fit("pol1","","",15,70);

 c1 -> Print("pe_plot2.pdf");

}
