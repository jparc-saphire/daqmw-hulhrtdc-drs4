void calc_PE01()
{
  const int N = 6;

  double ph1[N]  =  {34.70, 45.88, 57.55, 69.14, 81.12, 93.63};  
  double ph1E[N] =  { 0.20,  0.10,  0.13,  0.16,  0.18,  0.24};  

  double ph2[N]  =  {30.49, 41.22, 52.98, 64.03, 75.37, 87.78};  
  double ph2E[N] =  { 0.07,  0.10,  0.10,  0.14,  0.15,  0.15};  

  double pe1[N]  =  {4., 5., 6., 7., 8., 9.};  
  double pe1E[N] =  {0., 0., 0., 0., 0., 0.};  

  double pe2[N]  =  {4., 5., 6., 7., 8., 9.};  
  double pe2E[N] =  {0., 0., 0., 0., 0., 0.};  

  TCanvas *c1 = new TCanvas("c1","c1"); 
  c1->Divide(1,2);
  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.10);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  TH1F* Hc1=c1->cd(1)->DrawFrame( 20., 0.0, 100.0, 10.0 );
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
  graph1->Fit("pol1","","",25,95);

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.10);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  TH1F* Hc2=c1->cd(2)->DrawFrame( 20., 0.0, 100.0, 10.0 );
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
  graph2->Fit("pol1","","",25,95);

 c1 -> Print("pe_plot.pdf");

}
