#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void average()
{
  TFile *fin = new TFile("../../test_run018.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double tdc1[16],tdc2[16];
  double width1[16],width2[16];

  tree->SetBranchAddress("tdc1", tdc1);
  tree->SetBranchAddress("tdc2", tdc2);
  tree->SetBranchAddress("width1", width1);
  tree->SetBranchAddress("width2", width2);
  
  TH1F *hist[1];
  hist[0] = new TH1F("h1", "TOF1", 100, -3, 3);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
      tree->GetEntry(i);
      if(178.0<tdc1[0]&&tdc1[0]<187.0&&178.0<tdc2[0]&&tdc2[0]<187.0 ){
	hist[0]->Fill((tdc1[0]-tdc2[0])-0.50);
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
