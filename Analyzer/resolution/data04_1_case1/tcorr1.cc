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
  double ltdc1[16],ltdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF1vsPE1", 1000, 0, 30, 100, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsPE2", 1000, 0, 30, 100, -3, 3);

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
    if( ltdc1[0]>0&&ltdc2[0]>0&&amp1[0]>0&&amp2[0]>0){

      double pe1 = 0.109*amp1[0]+0.80;
      double pe2 = 0.102*amp2[0]+1.24;

      double Amp1 = pe1-2.0;
      double Amp2 = pe2-2.0;

      double T1 =  a01+b01/pow(Amp1,0.5)+c01*(Amp1);
      double T2 =  a02+b02/pow(Amp2,0.5)+c02*(Amp2);
      
      double TOFa = ((ltdc1[0]-ltdc2[0])+0.17)-T1;
      double TOF  = TOFa-T2;

      if( Amp1>0 && Amp2>0 ){
	hist[0]->Fill(Amp1,TOF);
	hist[0]->GetXaxis()->SetTitle("PE1");
	hist[0]->GetYaxis()->SetTitle("TOF");
	hist[1]->Fill(Amp2,TOF);
	hist[1]->GetXaxis()->SetTitle("PE2");
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
