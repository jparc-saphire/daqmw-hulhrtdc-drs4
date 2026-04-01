#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void plot_PE03()
{
  double amp1_1[16],amp2_1[16];
  double ltdc1_1[16],ltdc2_1[16];
  double ttdc1_1[16],ttdc2_1[16];
  
  TH1F *hist1[2];
  hist1[0] = new TH1F("h1_1", "PE1", 300, 0, 30);
  hist1[1] = new TH1F("h2_1", "PE2", 300, 0, 30);

  TH1F *hist2[2];
  hist2[0] = new TH1F("h1_2", "Width1", 200, 0, 20);
  hist2[1] = new TH1F("h2_2", "Width2", 200, 0, 20);

  TH2F *hist3[2];
  hist3[0] = new TH2F("h1_3", "Width vs PE 1", 300, 0, 30, 100, 0, 20);
  hist3[1] = new TH2F("h2_4", "Width vs PE 2", 300, 0, 30, 100, 0, 20);

  TFile *fin1 = new TFile("../../test_run051.root");
  TTree *tree1 = (TTree*)fin1->Get("tree");

  tree1->SetBranchAddress("amp1", amp1_1);
  tree1->SetBranchAddress("ltdc1", ltdc1_1);
  tree1->SetBranchAddress("ttdc1", ttdc1_1);

  int n1 = tree1->GetEntries();
  for(int i = 0; i<n1; ++i){
    tree1->GetEntry(i);
    if( ltdc1_1[0]>0.&&ttdc1_1[0]>0.&&amp1_1[0]>0.){

      double pe1 = 0.0860*amp1_1[0]+1.01;
      hist1[0]->Fill(pe1);

      double w1 = ltdc1_1[0]-ttdc1_1[0];
      hist2[0]->Fill(w1);
      hist3[0]->Fill(pe1,w1);
    } 
  } 

  TFile *fin2 = new TFile("../../test_run052.root");
  TTree *tree2 = (TTree*)fin2->Get("tree");

  tree2->SetBranchAddress("amp1", amp2_1);
  tree2->SetBranchAddress("ltdc1", ltdc2_1);
  tree2->SetBranchAddress("ttdc1", ttdc2_1);

  int n2 = tree2->GetEntries();
  for(int i = 0; i<n2; ++i){
    tree2->GetEntry(i);
    if( ltdc2_1[0]>0.&&ttdc2_1[0]>0.&&amp2_1[0]>0.){

      double pe2 = 0.0860*amp2_1[0]+1.01;
      hist1[1]->Fill(pe2);

      double w2 = ltdc2_1[0]-ttdc2_1[0];
      hist2[1]->Fill(w2);
      hist3[1]->Fill(pe2,w2);
    } 
  } 

  TCanvas *c1 = new TCanvas("c1","c1",1600,900); 
  c1->Divide(2,3);

  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.10);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  c1->cd(1)->SetLogy(0);
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0, 0.0, 30.0, 200.0 );
  Hc1->GetXaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetLabelSize(0.060);
  Hc1->GetYaxis()->SetTitle("Counts");
  Hc1->GetYaxis()->SetTitleSize(0.07);
  Hc1->GetYaxis()->SetTitleOffset(0.7);
  Hc1->GetXaxis()->SetTitle("# of P.E.");
  Hc1->GetXaxis()->SetTitleSize(0.07);
  hist1[0]->SetLineColor(1);
  hist1[0]->Draw("same");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.10);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  TH1F* Hc2=c1->cd(2)->DrawFrame( 0, 0.0, 30.0, 200.0 );
  Hc2->GetXaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetLabelSize(0.060);
  Hc2->GetYaxis()->SetTitle("Counts");
  Hc2->GetYaxis()->SetTitleSize(0.07);
  Hc2->GetYaxis()->SetTitleOffset(0.7);
  Hc2->GetXaxis()->SetTitle("# of P.E.");
  Hc2->GetXaxis()->SetTitleSize(0.07);
  hist1[1]->SetLineColor(1);
  hist1[1]->Draw("same");

  c1->cd(3);
  c1->cd(3)->SetLeftMargin(0.10);
  c1->cd(3)->SetRightMargin(0.1);
  c1->cd(3)->SetTopMargin(0.1);
  c1->cd(3)->SetBottomMargin(0.15);
  c1->cd(3)->SetGrid();
  c1->cd(3)->SetLogy(0);
  TH1F* Hc3=c1->cd(3)->DrawFrame( 0, 0.0, 15.0, 250.0 );
  Hc3->GetXaxis()->SetLabelSize(0.060);
  Hc3->GetYaxis()->SetLabelSize(0.060);
  Hc3->GetYaxis()->SetTitle("Counts");
  Hc3->GetYaxis()->SetTitleSize(0.07);
  Hc3->GetYaxis()->SetTitleOffset(0.7);
  Hc3->GetXaxis()->SetTitle("Width [ns]");
  Hc3->GetXaxis()->SetTitleSize(0.07);
  hist2[0]->SetLineColor(1);
  hist2[0]->Draw("same");

  c1->cd(4);
  c1->cd(4)->SetLeftMargin(0.10);
  c1->cd(4)->SetRightMargin(0.1);
  c1->cd(4)->SetTopMargin(0.1);
  c1->cd(4)->SetBottomMargin(0.15);
  c1->cd(4)->SetGrid();
  c1->cd(4)->SetLogy(0);
  TH1F* Hc4=c1->cd(4)->DrawFrame( 0, 0.0, 15.0, 200.0 );
  Hc4->GetXaxis()->SetLabelSize(0.060);
  Hc4->GetYaxis()->SetLabelSize(0.060);
  Hc4->GetYaxis()->SetTitle("Counts");
  Hc4->GetYaxis()->SetTitleSize(0.07);
  Hc4->GetYaxis()->SetTitleOffset(0.7);
  Hc4->GetXaxis()->SetTitle("Width [ns]");
  Hc4->GetXaxis()->SetTitleSize(0.07);
  hist2[1]->SetLineColor(1);
  hist2[1]->Draw("same");

  c1->cd(5);
  c1->cd(5)->SetLeftMargin(0.10);
  c1->cd(5)->SetRightMargin(0.1);
  c1->cd(5)->SetTopMargin(0.1);
  c1->cd(5)->SetBottomMargin(0.15);
  c1->cd(5)->SetGrid();
  c1->cd(5)->SetLogy(0);
  c1->cd(5)->SetLogz();
  TH1F* Hc5=c1->cd(5)->DrawFrame( 0, 0.0, 30.0, 15.0 );
  Hc5->GetXaxis()->SetLabelSize(0.060);
  Hc5->GetYaxis()->SetLabelSize(0.060);
  Hc5->GetYaxis()->SetTitle("TOT(Width) [ns]");
  Hc5->GetYaxis()->SetTitleSize(0.07);
  Hc5->GetYaxis()->SetTitleOffset(0.7);
  Hc5->GetXaxis()->SetTitle("# of P.E.");
  Hc5->GetXaxis()->SetTitleSize(0.07);
  //hist2[0]->SetLineColor(1);
  hist3[0]->Draw("same,col");

  c1->cd(6);
  c1->cd(6)->SetLeftMargin(0.10);
  c1->cd(6)->SetRightMargin(0.1);
  c1->cd(6)->SetTopMargin(0.1);
  c1->cd(6)->SetBottomMargin(0.15);
  c1->cd(6)->SetGrid();
  c1->cd(6)->SetLogy(0);
  c1->cd(6)->SetLogz();
  TH1F* Hc6=c1->cd(6)->DrawFrame( 0, 0.0, 30.0, 15.0 );
  Hc6->GetXaxis()->SetLabelSize(0.060);
  Hc6->GetYaxis()->SetLabelSize(0.060);
  Hc6->GetYaxis()->SetTitle("TOT(Width) [ns]");
  Hc6->GetYaxis()->SetTitleSize(0.07);
  Hc6->GetYaxis()->SetTitleOffset(0.7);
  Hc6->GetXaxis()->SetTitle("# of P.E.");
  Hc6->GetXaxis()->SetTitleSize(0.07);
  hist3[1]->Draw("same,col");

  c1 -> Print("plot_TOT_PE_03.pdf");
}
