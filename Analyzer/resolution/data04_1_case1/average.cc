#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void average()
{
  TFile *fin = new TFile("../../test_run076.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  
  TH1F *hist[1];
  hist[0] = new TH1F("h1", "TOF1", 100, -3, 3);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
      tree->GetEntry(i);
      if( ltdc1[0]>0.&&ltdc2[0]>0.){
	hist[0]->Fill((ltdc1[0]-ltdc2[0])+0.17);
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

  c1 -> Print("average.pdf");
}
