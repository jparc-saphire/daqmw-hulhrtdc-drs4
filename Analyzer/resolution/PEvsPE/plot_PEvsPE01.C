#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void plot_PEvsPE01()
{
  TFile *fin = new TFile("../../test_run077.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);

  TH2F *hist1[2];
  hist1[0] = new TH2F("h11", "PE1 vs PE2", 150, 0, 30, 150, 0, 30);
  hist1[1] = new TH2F("h12", "PE1 vs PE2", 150, 0, 30, 150, 0, 30);

  TH1F *hist2[2];
  hist2[0] = new TH1F("h21", "PE1-PE2", 300, -30, 30);
  hist2[1] = new TH1F("h22", "PE1-PE2", 300, -30, 30);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( ltdc1[0]>0.&&ltdc2[0]>0.&&amp1[0]>0.&&amp2[0]>0.){

      double pe1 = 0.105*amp1[0]+0.97;
      double pe2 = 0.102*amp2[0]+1.21;

      hist1[0]->Fill(pe1,pe2);
      hist2[0]->Fill(pe1-pe2);
      if( abs(pe1-pe2)<3.0 ) {
	hist1[1]->Fill(pe1,pe2);
	hist2[1]->Fill(pe1-pe2);
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
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0.0, 0.0, 30.0, 30.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("# of P.E.");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(1.1);
  Hc1->GetXaxis()->SetTitle("# of P.E.");
  Hc1->GetXaxis()->SetTitleSize(0.07);
  hist1[0]->Draw("same,col");
  hist1[1]->SetMarkerColor(2);
  hist1[1]->Draw("same");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.15);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  TH1F* Hc2=c1->cd(2)->DrawFrame( -30, 0.0, 30.0, 900.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  //Hc2->GetYaxis()->SetTitle("Counts");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(1.1);
  Hc2->GetXaxis()->SetTitle("# of P.E. (Subtraction)");
  Hc2->GetXaxis()->SetTitleSize(0.07);
  hist2[0]->SetLineColor(1);
  hist2[0]->Draw("same");
  hist2[1]->SetLineColor(2);
  hist2[1]->Draw("same");

  c1 -> Print("plot_PE_vs_PE01.pdf");
}
