#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void test_evNum02()
{
  TFile *fin = new TFile("test.root");
  TTree *tree = (TTree*)fin->Get("tree");

  int evDRS, evHRT;
  double  amp1[16],  amp2[16];
  double  tdc1[16],  tdc2[16];
  double ltdc1[16], ltdc2[16];

  tree->SetBranchAddress("evDRS", &evDRS);
  tree->SetBranchAddress("evHRT", &evHRT);

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("tdc1", tdc1);
  tree->SetBranchAddress("tdc2", tdc2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  
  TH2F *hist[6];
  hist[0] = new TH2F("h1", "AMP vs DT", 100, 0, 300, 100, -3, 3);
  hist[1] = new TH2F("h2", "AMP vs DT", 100, 0, 300, 100, -3, 3);

  hist[2] = new TH2F("h3", "TDC1 vs lTDC1", 100, 170, 190, 100, 175, 190);
  hist[3] = new TH2F("h4", "TDC2 vs lTDC2", 100, 170, 190, 100, 175, 190);

  // hist[4] = new TH2F("h5", "AMPvsTDC", 300, 0, 600, 200, 170, 180);
  // hist[5] = new TH2F("h6", "AMPvsTDC", 300, 0, 600, 200, 160, 170);

  int n = tree->GetEntries();
  for(int i = 0; i<n-1; ++i){
    tree->GetEntry(i);
    double a1 = amp1[0];
    double t1 = tdc1[0]-tdc2[0];
    double lt1 = ltdc1[0]-ltdc2[0];

    if(tdc1[0]>0&&tdc2[0]>0) hist[0]->Fill(a1,t1);
    if(ltdc1[0]>0&&ltdc2[0]>0) hist[1]->Fill(a1,lt1);

    if(tdc1[0]>0&&ltdc1[0]>0) hist[2]->Fill(ltdc1[0],tdc1[0]);
    if(tdc2[0]>0&&ltdc2[0]>0) hist[3]->Fill(ltdc2[0],tdc2[0]);

    // tree->GetEntry(i+1);
    // double a2 = amp1[0];
    // double t2 = tdc1[0];
    // double lt2 = ltdc1[0];

    // hist[2]->Fill(a2,t1);
    // hist[3]->Fill(a2,lt1);

    // hist[4]->Fill(a1,t2);
    // hist[5]->Fill(a1,lt2);
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,2);
  for(int i=0; i<4; ++i){
    c1->cd(i+1);
    hist[i]->Draw("col");
  }

}
