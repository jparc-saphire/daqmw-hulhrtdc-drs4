#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void tcorr1()
{
  TFile *fin = new TFile("../../test_run076.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16],ltdc3[16],ltdc4[16];
  double ttdc1[16],ttdc2[16],ttdc3[16],ttdc4[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ltdc3", ltdc3);
  tree->SetBranchAddress("ltdc4", ltdc4);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);
  tree->SetBranchAddress("ttdc3", ttdc3);
  tree->SetBranchAddress("ttdc4", ttdc4);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF1vsWIDTH1", 100, 0, 6, 50, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsWIDTH2", 100, 0, 6, 50, -3, 3);

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
    if(172.0<ltdc1[0]&&ltdc1[0]<182.0&&172.0<ltdc2[0]&&ltdc2[0]<182.0&& 
       160.0<ltdc3[0]&&ltdc3[0]<170.0&&159.0<ltdc4[0]&&ltdc4[0]<169.0&&
       150.0<ttdc3[0]&&ttdc3[0]<165.0&&151.0<ttdc4[0]&&ttdc4[0]<166.0){
      
      double pe1 = 0.109*amp1[0]+0.80;
      double pe2 = 0.102*amp2[0]+1.24;

      double w1 = ltdc3[0]-ttdc3[0]-6.8;
      double w2 = ltdc4[0]-ttdc4[0]-6.2;
      
      // double T1 =  a01+b01*w1+c01*w1*w1;
      // double T2 =  a02+b02*w2+c02*w2*w2;
      double T1 =  a01+b01/pow(w1,0.5)+c01*(w1);
      double T2 =  a02+b02/pow(w2,0.5)+c02*(w2);
      
      double TOFa = ((ltdc1[0]-ltdc2[0])+0.23)-T1;
      double TOF  = TOFa-T2;
      
      if( w1>0.0 && w2>0.0 && pe1>10.0 && pe2>10.0 ){
	hist[0]->Fill(w1,TOF);
	hist[0]->GetXaxis()->SetTitle("WIDTH1");
	hist[0]->GetYaxis()->SetTitle("TOF");
	hist[1]->Fill(w2,TOF);
	hist[1]->GetXaxis()->SetTitle("WIDTH2");
	hist[1]->GetYaxis()->SetTitle("TOF");
      }
    }
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(1,2);
  for(int i=0; i<2; ++i)
    {
      c1->cd(i+1);
      hist[i]->Draw("colz");
    }

  c1->Print("tcorr1.pdf");

}
