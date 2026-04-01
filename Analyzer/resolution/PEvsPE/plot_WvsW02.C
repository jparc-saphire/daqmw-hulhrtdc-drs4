#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void plot_WvsW02()
{
  TFile *fin = new TFile("../../test_run081.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];
  double ltdc3[16],ttdc3[16];
  double ltdc4[16],ttdc4[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ltdc3", ltdc3);
  tree->SetBranchAddress("ltdc4", ltdc4);
  tree->SetBranchAddress("ttdc3", ttdc3);
  tree->SetBranchAddress("ttdc4", ttdc4);

  TH2F *hist1[2];
  hist1[0] = new TH2F("h11", "W1 vs W2", 150, 0, 15, 150, 0, 15);
  hist1[1] = new TH2F("h12", "W1 vs W2", 150, 0, 15, 150, 0, 15);

  TH1F *hist2[2];
  hist2[0] = new TH1F("h21", "W1-W2", 300, -15, 15);
  hist2[1] = new TH1F("h22", "W1-W2", 300, -15, 15);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( ltdc1[0]>0.&&ltdc2[0]>0.&&amp1[0]>0.&&amp2[0]>0.){

      double w1 = ltdc3[0]-ttdc3[0];
      double w2 = ltdc4[0]-ttdc4[0];

      if( w1>0 && w2>0 ){
	hist1[0]->Fill(w1,w2);
	hist2[0]->Fill(w1-w2);
	if( abs(w1-w2)<3.0 ) hist2[1]->Fill(w1-w2);
      } 
    } 
  }

  TCanvas *c1 = new TCanvas("c1","c1", 1200,600); 
  c1->Divide(2,1);

  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.15);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  c1->cd(1)->SetLogy(0);
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0.0, 0.0, 12.0, 12.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("Width [ns]");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(1.1);
  Hc1->GetXaxis()->SetTitle("Width [ns]");
  Hc1->GetXaxis()->SetTitleSize(0.07);
  //hist1[0]->SetLineColor(1);
  hist1[0]->Draw("same,col");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.15);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  TH1F* Hc2=c1->cd(2)->DrawFrame( -12, 0.0, 12.0, 900.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  //Hc2->GetYaxis()->SetTitle("Counts");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(1.1);
  Hc2->GetXaxis()->SetTitle("Width [ns] (Subtraction)");
  Hc2->GetXaxis()->SetTitleSize(0.07);
  hist2[0]->SetLineColor(1);
  hist2[0]->Draw("same");
  // hist2[1]->SetLineColor(2);
  // hist2[1]->Draw("same");

  c1 -> Print("plot_W_vs_W02.pdf");
}
