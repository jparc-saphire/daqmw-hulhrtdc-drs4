#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void average()
{
  TFile *fin = new TFile("../../test_run031.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double ltdc1[16],ltdc2[16];
  double ttdc1[16],ttdc2[16];

  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);
  
  TH1F *hist[1];
  hist[0] = new TH1F("h1", "TOF1", 100, -3, 3);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
      tree->GetEntry(i);
      if(180.0<ltdc1[0]&&ltdc1[0]<190.0&&180.0<ltdc2[0]&&ltdc2[0]<190.0&&
	 176.0<ttdc1[0]&&ttdc1[0]<186.0&&176.0<ttdc2[0]&&ttdc2[0]<186.0){

	hist[0]->Fill((ltdc1[0]-ltdc2[0])+0.58);
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
