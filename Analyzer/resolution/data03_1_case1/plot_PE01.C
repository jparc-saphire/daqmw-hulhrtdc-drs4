#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void plot_PE01()
{
  TFile *fin = new TFile("../../test_run042.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);

  TH1F *hist[2];
  hist[0] = new TH1F("h1", "PE1", 300, 0, 30);
  hist[1] = new TH1F("h2", "PE2", 300, 0, 30);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( ltdc1[0]>0.&&ltdc2[0]>0.&&amp1[0]>0.&&amp2[0]>0.){

      double pe1 = 0.0512*amp1[0]-1.80;
      double pe2 = 0.0516*amp2[0]-0.81;

      hist[0]->Fill(pe1);
      hist[1]->Fill(pe2);
    } 
  } 

  TCanvas *c1 = new TCanvas("c1","c1"); 
  c1->Divide(2,2);

  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.15);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  c1->cd(1)->SetLogy(0);
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0, 0.0, 30.0, 3000.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("Counts");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(1.1);
  Hc1->GetXaxis()->SetTitle("# of P.E.");
  Hc1->GetXaxis()->SetTitleSize(0.07);
  hist[0]->SetLineColor(1);
  hist[0]->Draw("same");

  c1->cd(3);
  c1->cd(3)->SetLeftMargin(0.15);
  c1->cd(3)->SetRightMargin(0.1);
  c1->cd(3)->SetTopMargin(0.1);
  c1->cd(3)->SetBottomMargin(0.15);
  c1->cd(3)->SetGrid();
  c1->cd(3)->SetLogy();
  TH1F* Hc3=c1->cd(3)->DrawFrame( 0, 0.1, 30.0, 4000.0 );
  Hc3->GetXaxis()->SetLabelSize(0.060);
  Hc3->GetYaxis()->SetLabelSize(0.060);
  Hc3->GetYaxis()->SetTitle("Counts (Log scale)");
  Hc3->GetYaxis()->SetTitleSize(0.07);
  Hc3->GetYaxis()->SetTitleOffset(1.1);
  Hc3->GetXaxis()->SetTitle("# of P.E.");
  Hc3->GetXaxis()->SetTitleSize(0.07);
  hist[0]->SetLineColor(1);
  hist[0]->Draw("same");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.15);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  TH1F* Hc2=c1->cd(2)->DrawFrame( 0, 0.0, 30.0, 3000.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetTitle("Counts");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(1.1);
  Hc2->GetXaxis()->SetTitle("# of P.E.");
  Hc2->GetXaxis()->SetTitleSize(0.07);
  hist[1]->SetLineColor(1);
  hist[1]->Draw("same");

  c1->cd(4);
  c1->cd(4)->SetLeftMargin(0.15);
  c1->cd(4)->SetRightMargin(0.1);
  c1->cd(4)->SetTopMargin(0.1);
  c1->cd(4)->SetBottomMargin(0.15);
  c1->cd(4)->SetGrid();
  c1->cd(4)->SetLogy();
  TH1F* Hc4=c1->cd(4)->DrawFrame( 0, 0.1, 30.0, 4000.0 );
  Hc4->GetXaxis()->SetLabelSize(0.060);
  Hc4->GetYaxis()->SetLabelSize(0.060);
  Hc4->GetYaxis()->SetTitle("Counts (Log scale)");
  Hc4->GetYaxis()->SetTitleSize(0.07);
  Hc4->GetYaxis()->SetTitleOffset(1.1);
  Hc4->GetXaxis()->SetTitle("# of P.E.");
  Hc4->GetXaxis()->SetTitleSize(0.07);
  hist[1]->SetLineColor(1);
  hist[1]->Draw("same");

  c1 -> Print("plot_pe.pdf");
}
