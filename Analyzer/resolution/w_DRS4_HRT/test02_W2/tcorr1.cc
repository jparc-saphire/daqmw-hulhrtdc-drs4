#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void tcorr1()
{
  TFile *fin = new TFile("../../test_run023.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double ltdc1[16],ltdc2[16];
  double ttdc1[16],ttdc2[16];

  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF1vsWIDTH1", 1000, 0, 5, 100, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsWIDTH2", 1000, 0, 5, 100, -3, 3);

  double a01,b01,c01;
  double a02,b02,c02;
  char check[256];
  ifstream ifs1("corr1.dat");
  
  while(ifs1.getline(check,256)){
    stringstream st1;
    st1<<check;
    if(strlen(check)==0) continue;
    else{
      st1 >> a01 >> b01 >> c01 
	  >> a02 >> b02 >> c02; 
    }
  }
  
  cout<< a01 << "  " << b01 << "  " << c01 << endl;
  cout<< a02 << "  " << b02 << "  " << c02 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if(181.0<ltdc1[0]&&ltdc1[0]<191.0&&181.0<ltdc2[0]&&ltdc2[0]<191.0&&
       177.0<ttdc1[0]&&ttdc1[0]<187.0&&177.0<ttdc2[0]&&ttdc2[0]<187.0){

      double w1 = ltdc1[0]-ttdc1[0]-2.2;
      double w2 = ltdc2[0]-ttdc2[0]-1.9;
      
      double T1 =  a01+b01*w1+c01*w1*w1;
      double T2 =  a02+b02*w2+c02*w2*w2;
      
      double TOFa = ((ltdc1[0]-ltdc2[0])+0.57)-T1;
      double TOF  = TOFa-T2;
      
      hist[0]->Fill(w1,TOF);
      hist[0]->GetXaxis()->SetTitle("WIDTH1");
      hist[0]->GetYaxis()->SetTitle("TOF");
      hist[1]->Fill(w2,TOF);
      hist[1]->GetXaxis()->SetTitle("WIDTH2");
      hist[1]->GetYaxis()->SetTitle("TOF");
    }
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(1,2);
  for(int i=0; i<2; ++i)
    {
      c1->cd(i+1);
      hist[i]->Draw("colz");
    }

  c1->Print("tcorr1.pdf)");

}
