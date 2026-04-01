#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void calc_PE01()
{
  const int N = 10;

  double ph1[N]  =  { 2.99, 3.88, 4.59, 5.18, 5.65, 6.06, 6.41, 6.72, 7.01, 7.27};  
  double ph1E[N] =  { 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};  

  double ph2[N]  =  { 2.00, 3.21, 3.99, 4.63, 5.12, 5.54, 5.91, 6.24, 6.52, 6.78};  
  double ph2E[N] =  { 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};  

  double pe1[N]  =  { 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.};  
  double pe1E[N] =  { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};  

  double pe2[N]  =  { 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}; 
  double pe2E[N] =  { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};  

  TCanvas *c1 = new TCanvas("c1","c1"); 
  c1->Divide(1,2);
  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.10);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0.0, 0.0, 14.0, 30.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("# of P.E.");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(0.7);
  Hc1->GetXaxis()->SetTitle("Width [ns]");
  Hc1->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph1 = new TGraphErrors(N, ph1, pe1, ph1E, pe1E);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");

  graph1->Fit("pol3","","", 0.5,12.0);

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.10);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  TH1F* Hc2=c1->cd(2)->DrawFrame( 0.0, 0.0, 14.0, 30.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetTitle("# of P.E.");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(0.7);
  Hc2->GetXaxis()->SetTitle("Width [ns]");
  Hc2->GetXaxis()->SetTitleSize(0.07);

  TGraphErrors *graph2 = new TGraphErrors(N, ph2, pe2, ph2E, pe2E);
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(1);
  graph2->Draw("PEsame");

  graph2->Fit("pol3","","", 0.5,12.0);

  c1 -> Print("pe_plot.pdf");
}
