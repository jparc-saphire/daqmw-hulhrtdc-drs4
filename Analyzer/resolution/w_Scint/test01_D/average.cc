#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void average()
{
  TFile *fin = new TFile("../../test_run011.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16],amp3[16],amp4[16];
  double tdc1[16],tdc2[16],tdc3[16],tdc4[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("amp3", amp3);
  tree->SetBranchAddress("amp4", amp4);
  tree->SetBranchAddress("tdc1", tdc1);
  tree->SetBranchAddress("tdc2", tdc2);
  tree->SetBranchAddress("tdc3", tdc3);
  tree->SetBranchAddress("tdc4", tdc4);
  
  TH1F *hist[1];
  hist[0] = new TH1F("h1", "TOF1", 100, -3, 3);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
      tree->GetEntry(i);
      if( tdc2[0]>0.&&tdc3[0]>0.&&tdc4[0]>0. ){
	hist[0]->Fill(tdc2[0]-(tdc3[0]+tdc4[0])/2.0-5.73);
      }
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(1,1);
  for(int i=0; i<1; ++i)
    {
      c1->cd(i+1);
      hist[i]->Draw();
    }
  
  hist[0]->Fit("gaus","","",-1.2,1.2);

  hist[0]->GetXaxis()->SetTitle("time[ns]");
  hist[0]->GetYaxis()->SetTitle("counts");

  c1 -> Print("average.pdf)");
}
