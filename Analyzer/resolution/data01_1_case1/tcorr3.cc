#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void tcorr3()
{
  TFile *fin = new TFile("../../test_run029.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF2vsPE1", 300, 0, 30, 300, -3, 3);
  hist[1] = new TH2F("h2", "TOF2vsPE2", 300, 0, 30, 300, -3, 3);

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

  double a11,b11,c11;
  double a12,b12,c12;
  char check2[256];
  ifstream ifs2("corr2.dat");
  
  while(ifs2.getline(check2,256)){
    stringstream st2;
    st2<<check2;
    if(strlen(check2)==0) continue;
    else{
      st2 >> a11 >> b11 >> c11 
	  >> a12 >> b12 >> c12; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a11 << "  " << b11 << "  " << c11 << endl;
  cout<< a12 << "  " << b12 << "  " << c12 << endl;

  double a21,b21,c21;
  double a22,b22,c22;
  char check3[256];
  ifstream ifs3("corr3.dat");
  
  while(ifs3.getline(check3,256)){
    stringstream st3;
    st3<<check3;
    if(strlen(check3)==0) continue;
    else{
      st3 >> a21 >> b21 >> c21 
	  >> a22 >> b22 >> c22; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a21 << "  " << b21 << "  " << c21 << endl;
  cout<< a22 << "  " << b22 << "  " << c22 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( ltdc1[0]>0&&ltdc2[0]>0&&amp1[0]>0&&amp2[0]>0){

      double pe1 = 0.0520*amp1[0]-1.72;
      double pe2 = 0.0525*amp2[0]-0.77;

      double Amp1 = pe1-2.5;
      double Amp2 = pe2-3.0;

      double T1 =  a01+b01/pow(Amp1,0.5)+c01*(Amp1);
      double T2 =  a02+b02/pow(Amp2,0.5)+c02*(Amp2);
      
      double TOFa = ((ltdc1[0]-ltdc2[0])+0.60)-T1;
      double TOF1 = TOFa-T2;

      double T11 =  a11+b11*Amp1+c11*Amp1*Amp1;
      double T12 =  a12+b12*Amp2+c12*Amp2*Amp2;
      // double T11 =  a11+b11/pow(Amp1,0.5)+c11*(Amp1);
      // double T12 =  a12+b12/pow(Amp2,0.5)+c12*(Amp2);

      double TOFa2 = TOF1-T11;
      double TOF2  = TOFa2-T12;

      // double T21 =  a21+b21*Amp1+c21*Amp1*Amp1;
      // double T22 =  a22+b22*Amp2+c22*Amp2*Amp2;
      double T21 =  a21+b21/pow(Amp1,0.5)+c21*(Amp1);
      double T22 =  a22+b22/pow(Amp2,0.5)+c22*(Amp2);
      
      double TOFa3 = TOF2-T21;
      double TOF   = TOFa3-T22;
      
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

  c1->Print("tcorr3.pdf");

}
