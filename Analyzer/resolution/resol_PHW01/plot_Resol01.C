void plot_Resol01()
{
  const int N = 8;

  double ph1[N]  =  { 9.2, 10.6, 12.6, 14.4, 16.1, 17.7, 19.2, 20.5 };  
  double ph1E[N] =  { 0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1 };  

  double resol1[N]  = { 140.3, 121.7, 107.0, 94.19, 87.25, 80.95, 79.07, 79.96 };  
  double resol1E[N] = {   1.2,   1.2,   1.4,  1.58,  2.06,  2.93,  4.49,  10.3 };  

  double resol2[N]  = { 156.2, 136.5, 119.6, 109.1, 99.38, 91.91, 83.78, 74.80 };  
  double resol2E[N] = {   1.7,   1.6,   1.9,   2.3,  2.81,  4.50,  5.25,  10.1 };  


  double dresol1[N], dresol2[N];
  double dresol1E[N], dresol2E[N];

  double iresol1[N], iresol2[N];
  double iresol1E[N], iresol2E[N];

  for(int i=0; i<N; ++i){

    if( resol2[i] > resol1[i] ){
      dresol1[i] = sqrt(resol2[i]*resol2[i]-resol1[i]*resol1[i]);
      dresol1E[i] = sqrt(resol2E[i]*resol2E[i]+resol1E[i]*resol1E[i]);
    }
    else{
      dresol1[i] = sqrt(resol1[i]*resol1[i]-resol2[i]*resol2[i]);
      dresol1E[i] = sqrt(resol1E[i]*resol1E[i]+resol2E[i]*resol2E[i]);
    }

    iresol1[i] = resol1[i]/sqrt(2.0);
    iresol1E[i] = resol1E[i]/sqrt(2.0);
    iresol2[i] = resol2[i]/sqrt(2.0);
    iresol2E[i] = resol2E[i]/sqrt(2.0);

    if( iresol2[i] > iresol1[i] ){
      dresol2[i] = sqrt(iresol2[i]*iresol2[i]-iresol1[i]*iresol1[i]);
      dresol2E[i] = sqrt(iresol2E[i]*iresol2E[i]+iresol1E[i]*iresol1E[i]);
    }
    else{
      dresol2[i] = sqrt(iresol1[i]*iresol1[i]-iresol2[i]*iresol2[i]);
      dresol2E[i] = sqrt(iresol1E[i]*iresol1E[i]+iresol2E[i]*iresol2E[i]);
    }
  }

  TCanvas *c1 = new TCanvas("c1","c1", 900, 600); 
  c1->Divide(1,2);
  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.10);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  TH1F* Hc1=c1->cd(1)->DrawFrame( 6., 0.0, 23.0, 170.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("Time resolution [ps]");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(0.7);
  Hc1->GetXaxis()->SetTitle("# of P.E.");
  Hc1->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph1 = new TGraphErrors(N, ph1, resol1, ph1E, resol1E);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");

  TGraphErrors *graph2 = new TGraphErrors(N, ph1, resol2, ph1E, resol2E);
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(2);
  graph2->Draw("PEsame");

  TGraphErrors *graph3 = new TGraphErrors(N, ph1, dresol1, ph1E, dresol1E);
  graph3->SetMarkerStyle(8);
  graph3->SetMarkerColor(4);
  graph3->Draw("PEsame");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.10);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  TH1F* Hc2=c1->cd(2)->DrawFrame( 6., 0.0, 23.0, 120.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetTitle("Time resolution [ps]");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(0.7);
  Hc2->GetXaxis()->SetTitle("# of P.E.");
  Hc2->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph1i = new TGraphErrors(N, ph1, iresol1, ph1E, iresol1E);
  graph1i->SetMarkerStyle(8);
  graph1i->SetMarkerColor(1);
  graph1i->Draw("PEsame");

  TGraphErrors *graph2i = new TGraphErrors(N, ph1, iresol2, ph1E, iresol2E);
  graph2i->SetMarkerStyle(8);
  graph2i->SetMarkerColor(2);
  graph2i->Draw("PEsame");

  TGraphErrors *graph3i = new TGraphErrors(N, ph1, dresol2, ph1E, dresol2E);
  graph3i->SetMarkerStyle(8);
  graph3i->SetMarkerColor(4);
  graph3i->Draw("PEsame");

 c1 -> Print("Resol01.pdf");

}
